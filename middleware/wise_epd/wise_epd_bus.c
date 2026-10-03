/*
 * Copyright (C) 2026 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/*
 * SPI / GPIO access to the panel, shared by every EPD driver.
 *
 * All SPI traffic goes byte by byte through wise_spi_write_byte /
 * wise_spi_read_byte. CS and D/C are plain GPIOs, so one CS window spans as
 * many bytes as a command or frame needs.
 *
 * The panel has a single bidirectional data line (SDA), so the channel is
 * opened in 3-wire mode: MOSI drives SDA on writes and samples it on reads.
 *
 * Bring-up: build with WISE_EPD_LOG_LEVEL=1 to log which step failed (BUSY
 * timeouts, SPI errors).
 */

/* WISE_EPD_LOG_LEVEL overrides WISE_LOG_LEVEL for this module only. */
#ifdef WISE_EPD_LOG_LEVEL
#undef WISE_LOG_LEVEL
#define WISE_LOG_LEVEL WISE_EPD_LOG_LEVEL
#endif

#include <stdbool.h>
#include <stddef.h>

#include "wise_epd_drv.h"
#include "wise_tick_api.h"
#include "util_debug_log.h"

static WISE_EPD_CFG_T epdBusCfg;
static GPIO_STATUS epdBusyLevel = GPIO_LOW;

static void _epd_pin_output(uint8_t pin, GPIO_STATUS level)
{
    wise_gpio_func_cfg(pin, MODE_PIO_FUNC_GPIO);
    wise_gpio_set_direction(pin, GPIO_DIR_OUTPUT);
    wise_gpio_write(pin, level);
    wise_gpio_set_driv_str(pin, DRIV_STR_4_5MA);
}

static void _epd_pin_input(uint8_t pin, uint8_t pull)
{
    wise_gpio_func_cfg(pin, MODE_PIO_FUNC_GPIO);
    wise_gpio_set_direction(pin, GPIO_DIR_INPUT);
    wise_gpio_set_pull_sel(pin, pull);
}

static void _epd_power(bool on)
{
    GPIO_STATUS offLevel;

    if (epdBusCfg.pwr_pin == WISE_EPD_PIN_NONE) {
        return;
    }
    offLevel = (epdBusCfg.pwr_active_level == GPIO_HIGH) ? GPIO_LOW : GPIO_HIGH;
    wise_gpio_write(epdBusCfg.pwr_pin, on ? epdBusCfg.pwr_active_level : offLevel);
}

static void _epd_cs(bool assert)
{
    wise_gpio_write(epdBusCfg.cs_pin, assert ? GPIO_LOW : GPIO_HIGH);
}

/*
 * Pin state while the panel is not in use.
 *
 * Switched power: pull every signal low. A high level on an unpowered panel's
 * inputs back-powers it through the ESD diodes.
 * Always powered: keep CS / RST deasserted so the sleeping panel ignores the
 * bus, and keep BUSY pulled to a defined level.
 */
static void _epd_park_pins(void)
{
    if (epdBusCfg.pwr_pin == WISE_EPD_PIN_NONE) {
        _epd_pin_output(epdBusCfg.cs_pin, GPIO_HIGH);
        _epd_pin_output(epdBusCfg.rst_pin, GPIO_HIGH);
        _epd_pin_output(epdBusCfg.dc_pin, GPIO_LOW);
        _epd_pin_output(epdBusCfg.sclk_pin, GPIO_LOW);
        _epd_pin_output(epdBusCfg.mosi_pin, GPIO_LOW);
        _epd_pin_input(epdBusCfg.busy_pin, PUSEL_PULL_UP);
        return;
    }

    _epd_pin_input(epdBusCfg.cs_pin, PUSEL_PULL_DOWN);
    _epd_pin_input(epdBusCfg.rst_pin, PUSEL_PULL_DOWN);
    _epd_pin_input(epdBusCfg.dc_pin, PUSEL_PULL_DOWN);
    _epd_pin_input(epdBusCfg.sclk_pin, PUSEL_PULL_DOWN);
    _epd_pin_input(epdBusCfg.mosi_pin, PUSEL_PULL_DOWN);
    _epd_pin_input(epdBusCfg.busy_pin, PUSEL_PULL_DOWN);
}

static WISE_STATUS _epd_wait_level(GPIO_STATUS level, uint32_t timeout_ms, const char *what)
{
    uint32_t elapsed = 0;
    GPIO_STATUS busy = wise_gpio_read(epdBusCfg.busy_pin);

    while (busy != level) {
        if (elapsed >= timeout_ms) {
            WISE_LOG_ERR("EPD wait %s: timeout %lu ms, BUSY=%d\n", what, (unsigned long)timeout_ms, busy);
            return WISE_EPD_ERR_TIMEOUT;
        }
        wise_tick_delay_ms(1);
        elapsed++;
        busy = wise_gpio_read(epdBusCfg.busy_pin);
    }
    return WISE_SUCCESS;
}

static WISE_STATUS _epd_spi_write(const uint8_t *data, uint32_t len)
{
    while (len-- > 0) {
        WISE_STATUS status = wise_spi_write_byte(epdBusCfg.spi_channel, *data++);

        if (status != WISE_SUCCESS) {
            WISE_LOG_ERR("EPD spi write failed: %ld\n", (long)status);
            return WISE_EPD_ERR_BUS;
        }
    }
    return WISE_SUCCESS;
}

static WISE_STATUS _epd_spi_read(uint8_t *data, uint32_t len)
{
    while (len-- > 0) {
        WISE_STATUS status = wise_spi_read_byte(epdBusCfg.spi_channel, data++);

        if (status != WISE_SUCCESS) {
            WISE_LOG_ERR("EPD spi read failed: %ld\n", (long)status);
            return WISE_EPD_ERR_BUS;
        }
    }
    return WISE_SUCCESS;
}

void wise_epd_bus_setup(const WISE_EPD_CFG_T *cfg, GPIO_STATUS busy_level)
{
    epdBusCfg    = *cfg;
    epdBusyLevel = busy_level;

    if (epdBusCfg.pwr_pin != WISE_EPD_PIN_NONE) {
        GPIO_STATUS offLevel = (cfg->pwr_active_level == GPIO_HIGH) ? GPIO_LOW : GPIO_HIGH;
        _epd_pin_output(epdBusCfg.pwr_pin, offLevel);
    }
    _epd_park_pins();
}

WISE_STATUS wise_epd_bus_open(const WISE_EPD_TIMING_T *timing)
{
    const WISE_SPI_CONF_T spiCfg = {
        .clock_mode     = CLOCK_MODE0,
        .bit_order      = SPI_MSB_FIRST,
        .io_mode        = WISE_SPI_IO_3WIRE,
        .data_bit_width = 8,
        .data_merge     = WISE_SPI_ENABLE,
        .clock_sel      = epdBusCfg.spi_clock,
        .block_mode     = E_SPI_BLOCK_MODE,
        .dma_enable     = WISE_SPI_DISABLE,
    };
    uint8_t sclkFunc = (epdBusCfg.spi_channel == 0) ? MODE_PIO_FUNC_SPI0_CLK : MODE_PIO_FUNC_SPI1_CLK;
    uint8_t mosiFunc = (epdBusCfg.spi_channel == 0) ? MODE_PIO_FUNC_SPI0_MOSI : MODE_PIO_FUNC_SPI1_MOSI;

    _epd_power(true);
    wise_tick_delay_ms(timing->power_on_ms);

    if (wise_spi_master_open(epdBusCfg.spi_channel, &spiCfg) != WISE_SUCCESS) {
        WISE_LOG_ERR("EPD failed to open SPI%d\n", epdBusCfg.spi_channel);
        _epd_power(false);
        return WISE_EPD_ERR_BUS;
    }

    _epd_pin_output(epdBusCfg.cs_pin, GPIO_HIGH);
    _epd_pin_output(epdBusCfg.dc_pin, GPIO_HIGH);
    _epd_pin_output(epdBusCfg.rst_pin, GPIO_HIGH);
    _epd_pin_input(epdBusCfg.busy_pin, PUSEL_PULL_UP);
    wise_gpio_func_cfg(epdBusCfg.sclk_pin, sclkFunc);
    wise_gpio_func_cfg(epdBusCfg.mosi_pin, mosiFunc);
    wise_tick_delay_ms(timing->io_ready_ms);

    wise_gpio_write(epdBusCfg.rst_pin, GPIO_LOW);
    wise_tick_delay_ms(timing->reset_low_ms);
    wise_gpio_write(epdBusCfg.rst_pin, GPIO_HIGH);
    wise_tick_delay_ms(timing->reset_high_ms);

    return WISE_SUCCESS;
}

void wise_epd_bus_close(void)
{
    wise_spi_close(epdBusCfg.spi_channel);
    _epd_park_pins();
    _epd_power(false);
}

WISE_STATUS wise_epd_bus_write_cmd(uint8_t cmd)
{
    return wise_epd_bus_write_cmd_data(cmd, NULL, 0);
}

WISE_STATUS wise_epd_bus_write_cmd_data(uint8_t cmd, const uint8_t *data, uint32_t len)
{
    WISE_STATUS status;

    _epd_cs(true);
    wise_gpio_write(epdBusCfg.dc_pin, GPIO_LOW);
    status = _epd_spi_write(&cmd, 1);
    wise_gpio_write(epdBusCfg.dc_pin, GPIO_HIGH);
    if ((status == WISE_SUCCESS) && (len > 0)) {
        status = _epd_spi_write(data, len);
    }
    _epd_cs(false);

    return status;
}

WISE_STATUS wise_epd_bus_read_cmd_data(uint8_t cmd, uint8_t *data, uint32_t len)
{
    WISE_STATUS status;

    _epd_cs(true);
    wise_gpio_write(epdBusCfg.dc_pin, GPIO_LOW);
    status = _epd_spi_write(&cmd, 1);
    wise_gpio_write(epdBusCfg.dc_pin, GPIO_HIGH);
    if ((status == WISE_SUCCESS) && (len > 0)) {
        status = _epd_spi_read(data, len);
    }
    _epd_cs(false);

    return status;
}

void wise_epd_bus_data_begin(void)
{
    wise_gpio_write(epdBusCfg.dc_pin, GPIO_HIGH);
    _epd_cs(true);
}

WISE_STATUS wise_epd_bus_data_write(const uint8_t *data, uint32_t len)
{
    return _epd_spi_write(data, len);
}

void wise_epd_bus_data_end(void)
{
    _epd_cs(false);
}

WISE_STATUS wise_epd_bus_wait_idle(uint32_t timeout_ms)
{
    return _epd_wait_level((epdBusyLevel == GPIO_HIGH) ? GPIO_LOW : GPIO_HIGH, timeout_ms, "idle");
}

WISE_STATUS wise_epd_bus_wait_busy(uint32_t timeout_ms)
{
    return _epd_wait_level(epdBusyLevel, timeout_ms, "busy");
}

void wise_epd_bus_delay_ms(uint32_t ms)
{
    wise_tick_delay_ms(ms);
}

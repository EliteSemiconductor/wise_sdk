/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_PIO
#include "cmd_pio.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wise_core.h"
#include "api/wise_gpio_api.h"

#define MAX_PIN_NUM 16

/*
static int _pio_i2c_cmd(int argc, char **argv);
static int _pio_uart_cmd(int argc, char **argv);
static int _pio_spi_cmd(int argc, char **argv);
*/

static int _pio_func_cmd(int argc, char **argv);


static const struct shellCommand m_CmdList[] = {
    {"help", "function list", _helpCmd},
    {"func", "enable a function on pin", _pio_func_cmd},
    
    /*
    {"i2c", "Set I2C pins and enable them", _pio_i2c_cmd},
    {"uart", "Set UART pins and enable them", _pio_uart_cmd},
    {"spi", "Set SPI pins and enable them", _pio_spi_cmd},
    */
};

int PIOCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static int _pio_func_cmd(int argc, char **argv)
{
    uint8_t pinIdx = 0, pinFunc = 0;
    
    if (argc != 4) {
        goto invalid;
    }

    pinIdx = strtoul(argv[2], NULL, 10);
    pinFunc = strtoul(argv[3], NULL, 10);

    printf("Pin %d -> function %d\n", pinIdx, pinFunc);

    if(WISE_SUCCESS != wise_gpio_func_cfg(pinIdx, pinFunc))
        printf("failed\n");
    else
        printf("Pin %d configured\n", pinIdx);
    
    return SHLCMD_HIST | SHLCMD_DONE;
    
invalid:
    printf("usage: pio func [pin_idx] [funcSel]\n");
    printf("    [pin_idx]   index of selected IO pin\n");
    printf("    [funcSel]   index of selected function\n");
    printf("        0: FUNC_GPIO\n");
    printf("        1: FUNC_UART0_TX\n");
    printf("        2: FUNC_UART0_RX\n");
    printf("        3: FUNC_UART0_RTS\n");
    printf("        4: FUNC_UART0_CTS\n");
    printf("        5: FUNC_UART1_TX\n");
    printf("        6: FUNC_UART1_RX\n");
    printf("        7: FUNC_UART1_RTS\n");
    printf("        8: FUNC_UART1_CTS\n");
    printf("        9: FUNC_UART2_TX\n");
    printf("        10: FUNC_UART2_RX\n");
    printf("        11: FUNC_UART2_RTS\n");
    printf("        12: FUNC_UART2_CTS\n");
    printf("        13: FUNC_SPI0_CLK\n");
    printf("        14: FUNC_SPI0_CS\n");
    printf("        15: FUNC_SPI0_MOSI\n");
    printf("        16: FUNC_SPI0_MISO\n");
    printf("        17: FUNC_SPI0_HOLD\n");
    printf("        18: FUNC_SPI0_WP\n");
    printf("        19: FUNC_SPI1_CLK\n");
    printf("        20: FUNC_SPI1_CS\n");
    printf("        21: FUNC_SPI1_MOSI\n");
    printf("        22: FUNC_SPI1_MISO\n");
    printf("        23: FUNC_SPI1_HOLD\n");
    printf("        24: FUNC_SPI1_WP\n");
    printf("        25: FUNC_I2C0_SCL\n");
    printf("        26: FUNC_I2C0_SDA\n");
    printf("        27: FUNC_I2C1_SCL\n");
    printf("        28: FUNC_I2C1_SDA\n");
    
    return SHLCMD_HIST | SHLCMD_DONE;
}

#if 0
static int _pio_i2c_cmd(int argc, char **argv)
{
    uint8_t i2c_channel, scl_pin, sda_pin, scl_enable, sda_enable;

    if (argc != 7) {
        goto invalid;
    }

    i2c_channel = strtoul(argv[2], NULL, 10);
    scl_pin     = strtoul(argv[3], NULL, 10);
    sda_pin     = strtoul(argv[4], NULL, 10);
    scl_enable  = strtoul(argv[5], NULL, 10);
    sda_enable  = strtoul(argv[6], NULL, 10);

    if (i2c_channel > 1 || scl_pin >= MAX_PIN_NUM || sda_pin >= MAX_PIN_NUM || (scl_enable > 1) || (sda_enable > 1)) {
        goto invalid;
    }

    printf("I2C %d Configuration Summary:\n", i2c_channel);
    printf("--------------------------------------------\n");
    printf("Pin assignments:\n");
    printf("  SCL -> GPIO%d\n", scl_pin);
    printf("  SDA -> GPIO%d\n", sda_pin);
    printf("--------------------------------------------\n");
    printf("Pin enable statuses:\n");
    printf("  SCL = %s\n", (scl_enable ? "Enabled" : "Disabled"));
    printf("  SDA = %s\n", (sda_enable ? "Enabled" : "Disabled"));
    printf("--------------------------------------------\n");

    WISE_GPIO_CFG_WITH_REMAP_T i2c_pins[] = {
        {.pin_idx       = scl_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_I2C,
         .channel       = i2c_channel,
         .signal        = SIGNAL_SCL,
         .signal_enable = scl_enable},

        {.pin_idx       = sda_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_I2C,
         .channel       = i2c_channel,
         .signal        = SIGNAL_SDA,
         .signal_enable = sda_enable},
    };

    for (uint32_t i = 0; i < sizeof(i2c_pins) / sizeof(i2c_pins[0]); i++) {
        wise_gpio_cfg_with_pio_retarget(&i2c_pins[i]);
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    printf("usage: pio i2c [channel] [scl_pin] [sda_pin] [scl_enable] "
           "[sda_enable]\n");
    printf("       [channel] = 0 or 1\n");
    printf("       [scl_pin] = GPIO pin for SCL\n");
    printf("       [sda_pin] = GPIO pin for SDA\n");
    printf("       [scl_enable] = 0 for disable, 1 for enable\n");
    printf("       [sda_enable] = 0 for disable, 1 for enable\n");

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _pio_uart_cmd(int argc, char **argv)
{
    uint8_t uart_channel, tx_pin, rx_pin, rts_pin, cts_pin;
    uint8_t tx_enable, rx_enable, rts_enable, cts_enable;

    if (argc != 11) {
        goto invalid;
    }

    uart_channel = strtoul(argv[2], NULL, 10);
    tx_pin       = strtoul(argv[3], NULL, 10);
    rx_pin       = strtoul(argv[4], NULL, 10);
    rts_pin      = strtoul(argv[5], NULL, 10);
    cts_pin      = strtoul(argv[6], NULL, 10);
    tx_enable    = strtoul(argv[7], NULL, 10);
    rx_enable    = strtoul(argv[8], NULL, 10);
    rts_enable   = strtoul(argv[9], NULL, 10);
    cts_enable   = strtoul(argv[10], NULL, 10);

    if (uart_channel > 1 || tx_pin >= MAX_PIN_NUM || rx_pin >= MAX_PIN_NUM || rts_pin >= MAX_PIN_NUM || cts_pin >= MAX_PIN_NUM || (tx_enable > 1) ||
        (rx_enable > 1) || (rts_enable > 1) || (cts_enable > 1)) {
        goto invalid;
    }

    printf("UART %d Configuration Summary:\n", uart_channel);
    printf("--------------------------------------------\n");
    printf("Pin assignments:\n");
    printf("  TX  -> GPIO%d\n", tx_pin);
    printf("  RX  -> GPIO%d\n", rx_pin);
    printf("  RTS -> GPIO%d\n", rts_pin);
    printf("  CTS -> GPIO%d\n", cts_pin);
    printf("--------------------------------------------\n");
    printf("Pin enable statuses:\n");
    printf("  TX  = %s\n", (tx_enable ? "Enabled" : "Disabled"));
    printf("  RX  = %s\n", (rx_enable ? "Enabled" : "Disabled"));
    printf("  RTS = %s\n", (rts_enable ? "Enabled" : "Disabled"));
    printf("  CTS = %s\n", (cts_enable ? "Enabled" : "Disabled"));
    printf("--------------------------------------------\n");

    WISE_GPIO_CFG_WITH_REMAP_T uart_pins[] = {
        {.pin_idx       = tx_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_UART,
         .channel       = uart_channel,
         .signal        = SIGNAL_TX,
         .signal_enable = tx_enable},

        {.pin_idx       = rx_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_UART,
         .channel       = uart_channel,
         .signal        = SIGNAL_RX,
         .signal_enable = rx_enable},

        {.pin_idx       = rts_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_UART,
         .channel       = uart_channel,
         .signal        = SIGNAL_RTS,
         .signal_enable = rts_enable},

        {.pin_idx       = cts_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_UART,
         .channel       = uart_channel,
         .signal        = SIGNAL_CTS,
         .signal_enable = cts_enable},
    };

    for (uint32_t i = 0; i < sizeof(uart_pins) / sizeof(uart_pins[0]); i++) {
        wise_gpio_cfg_with_pio_retarget(&uart_pins[i]);
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    printf("usage: pio uart [channel] [tx_pin] [rx_pin] [rts_pin] [cts_pin] "
           "[tx_enable] [rx_enable] [rts_enable] [cts_enable]\n");
    printf("       [channel] = 0 or 1\n");
    printf("       [tx_pin] = GPIO pin for TX\n");
    printf("       [rx_pin] = GPIO pin for RX\n");
    printf("       [rts_pin] = GPIO pin for RTS\n");
    printf("       [cts_pin] = GPIO pin for CTS\n");
    printf("       [tx_enable] = 0 for disable, 1 for enable\n");
    printf("       [rx_enable] = 0 for disable, 1 for enable\n");
    printf("       [rts_enable] = 0 for disable, 1 for enable\n");
    printf("       [cts_enable] = 0 for disable, 1 for enable\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _pio_spi_cmd(int argc, char **argv)
{
    uint8_t spi_channel, clk_pin, cs_pin, miso_pin, mosi_pin, hold_pin, wp_pin;
    uint8_t clk_enable, cs_enable, miso_enable, mosi_enable, hold_enable, wp_enable;

    if (argc != 15) {
        goto invalid;
    }

    spi_channel = strtoul(argv[2], NULL, 10);
    clk_pin     = strtoul(argv[3], NULL, 10);
    cs_pin      = strtoul(argv[4], NULL, 10);
    miso_pin    = strtoul(argv[5], NULL, 10);
    mosi_pin    = strtoul(argv[6], NULL, 10);
    hold_pin    = strtoul(argv[7], NULL, 10);
    wp_pin      = strtoul(argv[8], NULL, 10);
    clk_enable  = strtoul(argv[9], NULL, 10);
    cs_enable   = strtoul(argv[10], NULL, 10);
    miso_enable = strtoul(argv[11], NULL, 10);
    mosi_enable = strtoul(argv[12], NULL, 10);
    hold_enable = strtoul(argv[13], NULL, 10);
    wp_enable   = strtoul(argv[14], NULL, 10);

    if (spi_channel > 1 || clk_pin >= MAX_PIN_NUM || cs_pin >= MAX_PIN_NUM || miso_pin >= MAX_PIN_NUM || mosi_pin >= MAX_PIN_NUM ||
        hold_pin >= MAX_PIN_NUM || wp_pin >= MAX_PIN_NUM || (clk_enable > 1) || (cs_enable > 1) || (miso_enable > 1) || (mosi_enable > 1) ||
        (hold_enable > 1) || (wp_enable > 1)) {
        goto invalid;
    }

    printf("SPI %d Configuration Summary:\n", spi_channel);
    printf("--------------------------------------------\n");
    printf("Pin assignments:\n");
    printf("  CLK  -> GPIO%d\n", clk_pin);
    printf("  CS   -> GPIO%d\n", cs_pin);
    printf("  MISO -> GPIO%d\n", miso_pin);
    printf("  MOSI -> GPIO%d\n", mosi_pin);
    printf("  HOLD -> GPIO%d\n", hold_pin);
    printf("  WP   -> GPIO%d\n", wp_pin);
    printf("--------------------------------------------\n");
    printf("Pin enable statuses:\n");
    printf("  CLK  = %s\n", (clk_enable ? "Enabled" : "Disabled"));
    printf("  CS   = %s\n", (cs_enable ? "Enabled" : "Disabled"));
    printf("  MISO = %s\n", (miso_enable ? "Enabled" : "Disabled"));
    printf("  MOSI = %s\n", (mosi_enable ? "Enabled" : "Disabled"));
    printf("  HOLD = %s\n", (hold_enable ? "Enabled" : "Disabled"));
    printf("  WP   = %s\n", (wp_enable ? "Enabled" : "Disabled"));
    printf("--------------------------------------------\n");

    WISE_GPIO_CFG_WITH_REMAP_T spi_pins[] = {
        {.pin_idx       = clk_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_SPI,
         .channel       = spi_channel,
         .signal        = SIGNAL_CLK,
         .signal_enable = clk_enable},

        {.pin_idx       = cs_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_SPI,
         .channel       = spi_channel,
         .signal        = SIGNAL_CS,
         .signal_enable = cs_enable},

        {.pin_idx       = miso_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_SPI,
         .channel       = spi_channel,
         .signal        = SIGNAL_MISO,
         .signal_enable = miso_enable},

        {.pin_idx       = mosi_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_SPI,
         .channel       = spi_channel,
         .signal        = SIGNAL_MOSI,
         .signal_enable = mosi_enable},

        {.pin_idx       = hold_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_SPI,
         .channel       = spi_channel,
         .signal        = SIGNAL_HOLD,
         .signal_enable = hold_enable},

        {.pin_idx       = wp_pin,
         .mode          = MODE_PIO,
         .direction     = GPIO_DIR_INPUT,
         .int_en        = GPIO_INT_DISABLE,
         .int_type      = GPIO_INT_TYPE_NONE,
         .bus_type      = BUS_TYPE_SPI,
         .channel       = spi_channel,
         .signal        = SIGNAL_WP,
         .signal_enable = wp_enable},
    };

    for (uint32_t i = 0; i < sizeof(spi_pins) / sizeof(spi_pins[0]); i++) {
        wise_gpio_cfg_with_pio_retarget(&spi_pins[i]);
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    printf("usage: pio spi [channel] [clk_pin] [cs_pin] [miso_pin] [mosi_pin] "
           "[hold_pin] [wp_pin]\n");
    printf("       [channel] = 0 or 1\n");
    printf("       [clk_pin] = GPIO pin for CLK\n");
    printf("       [cs_pin] = GPIO pin for CS\n");
    printf("       [miso_pin] = GPIO pin for MISO\n");
    printf("       [mosi_pin] = GPIO pin for MOSI\n");
    printf("       [hold_pin] = GPIO pin for HOLD\n");
    printf("       [wp_pin] = GPIO pin for WP\n");
    printf("       [enable] = 0 for disable, 1 for enable\n");

    return SHLCMD_HIST | SHLCMD_DONE;
}
#endif

#endif /* SHELL_PIO */

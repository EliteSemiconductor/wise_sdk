/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */
#include "api/wise_gpio_api.h"
#include "util_debug_log.h"

static uint8_t pinFuncBackup[CHIP_GPIO_NUM];
static uint8_t _gpio_inited = 0;

static void _set_all_pins_default();

#ifdef CHIP_GPIO_HAS_PIO
static void _set_all_pins_default()
{
    int i;

    for (i = 0; i < CHIP_GPIO_NUM; i++) {
        if ((i == 10) || (i == 14) || (i == 15)) { //leave TCXO, SWDIO, SWDCLK as default
            continue;
        }

        hal_intf_gpio_set_mode(i, MODE_GPIO);
        hal_intf_gpio_set_io_dirction(i, GPIO_DIR_INPUT);
        hal_intf_gpio_set_pull_sel(i, PUSEL_PULL_UP);
    }
}
#endif

void wise_gpio_init(void)
{
    int i;

    if(_gpio_inited)
        return;

    for (i = 0; i < CHIP_GPIO_NUM; i++) {
        pinFuncBackup[i] = MODE_PIO_FUNC_GPIO;
    }

    hal_intf_module_clk_enable(GPIO_MODULE);

#ifdef CHIP_GPIO_HAS_PIO
    _set_all_pins_default();
#endif

    _gpio_inited = 1;
}

void wise_gpio_deinit(void)
{
#ifdef CHIP_GPIO_HAS_PIO
    _set_all_pins_default();
#endif
    hal_intf_module_clk_disable(GPIO_MODULE);
    _gpio_inited = 0;
}

int32_t wise_gpio_cfg(WISE_GPIO_CFG_T *gpio_cfg)
{
    WISE_STATUS status = WISE_SUCCESS;
    uint8_t pin_idx    = gpio_cfg->pin_idx;

    if (pin_idx >= CHIP_GPIO_NUM) {
        status = WISE_FAIL;
        goto error;
    }

    hal_intf_gpio_set_mode(pin_idx, gpio_cfg->mode);

    if (gpio_cfg->mode == MODE_GPIO) // kevinyang, 20241222, GPIO mode only
    {
        hal_intf_gpio_set_io_dirction(pin_idx, gpio_cfg->direction);
        hal_intf_gpio_set_intrpt(pin_idx, gpio_cfg->int_en, gpio_cfg->int_type);
    }

error:
    return status;
}

void wise_gpio_set_interrupt(uint8_t pin_idx, WISE_GPIO_INT_CFG_T *int_cfg)
{
    if ((pin_idx >= CHIP_GPIO_NUM) || (int_cfg == NULL)) {
        return;
    }

    hal_intf_gpio_set_intrpt(pin_idx, int_cfg->int_en, int_cfg->int_type);
}

void wise_gpio_clear_int_status(uint8_t pin_idx)
{
    hal_intf_gpio_clear_int_status(pin_idx);
}


int32_t wise_gpio_func_cfg(uint8_t pinIdx, uint8_t pinFunc)
{
    const uint8_t FUNC_MAP[][3] = {
        //type              channel     func
        {PIO_TYPE_UART, 0, PIO_FUNC_UART_TX},  //MODE_PIO_FUNC_UART0_TX
        {PIO_TYPE_UART, 0, PIO_FUNC_UART_RX},  //MODE_PIO_FUNC_UART0_RX
        {PIO_TYPE_UART, 0, PIO_FUNC_UART_RTS}, //MODE_PIO_FUNC_UART0_RTS
        {PIO_TYPE_UART, 0, PIO_FUNC_UART_CTS}, //MODE_PIO_FUNC_UART0_CTS
        {PIO_TYPE_UART, 1, PIO_FUNC_UART_TX},  //MODE_PIO_FUNC_UART1_TX
        {PIO_TYPE_UART, 1, PIO_FUNC_UART_RX},  //MODE_PIO_FUNC_UART1_RX
        {PIO_TYPE_UART, 1, PIO_FUNC_UART_RTS}, //MODE_PIO_FUNC_UART1_RTS
        {PIO_TYPE_UART, 1, PIO_FUNC_UART_CTS}, //MODE_PIO_FUNC_UART1_CTS
        {PIO_TYPE_UART, 2, PIO_FUNC_UART_TX},  //MODE_PIO_FUNC_UART2_TX
        {PIO_TYPE_UART, 2, PIO_FUNC_UART_RX},  //MODE_PIO_FUNC_UART2_RX
        {PIO_TYPE_UART, 2, PIO_FUNC_UART_RTS}, //MODE_PIO_FUNC_UART2_RTS
        {PIO_TYPE_UART, 2, PIO_FUNC_UART_CTS}, //MODE_PIO_FUNC_UART2_CTS
        {PIO_TYPE_SPI, 0, PIO_FUNC_SPI_CLK},   //MODE_PIO_FUNC_SPI0_CLK
        {PIO_TYPE_SPI, 0, PIO_FUNC_SPI_CS},    //MODE_PIO_FUNC_SPI0_CS
        {PIO_TYPE_SPI, 0, PIO_FUNC_SPI_MOSI},  //MODE_PIO_FUNC_SPI0_MOSI
        {PIO_TYPE_SPI, 0, PIO_FUNC_SPI_MISO},  //MODE_PIO_FUNC_SPI0_MISO
        {PIO_TYPE_SPI, 0, PIO_FUNC_SPI_HOLD},  //MODE_PIO_FUNC_SPI0_HOLD
        {PIO_TYPE_SPI, 0, PIO_FUNC_SPI_WP},    //MODE_PIO_FUNC_SPI0_WP
        {PIO_TYPE_SPI, 1, PIO_FUNC_SPI_CLK},   //MODE_PIO_FUNC_SPI1_CLK
        {PIO_TYPE_SPI, 1, PIO_FUNC_SPI_CS},    //MODE_PIO_FUNC_SPI1_CS
        {PIO_TYPE_SPI, 1, PIO_FUNC_SPI_MOSI},  //MODE_PIO_FUNC_SPI1_MOSI
        {PIO_TYPE_SPI, 1, PIO_FUNC_SPI_MISO},  //MODE_PIO_FUNC_SPI1_MISO
        {PIO_TYPE_SPI, 1, PIO_FUNC_SPI_HOLD},  //MODE_PIO_FUNC_SPI1_HOLD
        {PIO_TYPE_SPI, 1, PIO_FUNC_SPI_WP},    //MODE_PIO_FUNC_SPI1_WP
        {PIO_TYPE_I2C, 0, PIO_FUNC_I2C_SCL},   //MODE_PIO_FUNC_I2C0_SCL
        {PIO_TYPE_I2C, 0, PIO_FUNC_I2C_SDA},   //MODE_PIO_FUNC_I2C0_SDA
        {PIO_TYPE_I2C, 1, PIO_FUNC_I2C_SCL},   //MODE_PIO_FUNC_I2C1_SCL
        {PIO_TYPE_I2C, 1, PIO_FUNC_I2C_SDA},   //MODE_PIO_FUNC_I2C1_SDA
    };
    WISE_STATUS status = WISE_FAIL;
    uint8_t presetIdx  = 0;

#ifdef CHIP_GPIO_HAS_PIO
    if (pinIdx >= CHIP_GPIO_NUM) {
        goto finish;
    }

    if (pinFunc == MODE_PIO_FUNC_GPIO) {
        if (pinFuncBackup[pinIdx] != MODE_PIO_FUNC_GPIO) {
            presetIdx = pinFuncBackup[pinIdx] - 1;
            hal_intf_pio_config(FUNC_MAP[presetIdx][0], FUNC_MAP[presetIdx][1], FUNC_MAP[presetIdx][2], PIO_FUNC_DISABLE, pinIdx);
        }

        hal_intf_gpio_set_mode(pinIdx, MODE_GPIO);
    } else if (pinFunc < MODE_PIO_FUNC_UNKOWN) {
        presetIdx = pinFunc - 1;

        if (HAL_NO_ERR != hal_intf_pio_config(FUNC_MAP[presetIdx][0], FUNC_MAP[presetIdx][1], FUNC_MAP[presetIdx][2], PIO_FUNC_ENABLE, pinIdx)) {
            goto finish;
        }

        hal_intf_gpio_set_mode(pinIdx, MODE_PIO);
    } else {
        goto finish;
    }

    pinFuncBackup[pinIdx] = pinFunc;

    status = WISE_SUCCESS;
#endif

finish:
    return status;
}

#if 0
int32_t wise_gpio_cfg_with_pio_retarget(WISE_GPIO_CFG_WITH_REMAP_T *gpio_cfg)
{
    if (gpio_cfg == NULL) {
        return WISE_FAIL;
    }

    WISE_GPIO_CFG_T base_cfg = {
        .pin_idx   = gpio_cfg->pin_idx,
        .mode      = gpio_cfg->mode,
        .direction = gpio_cfg->direction,
        .int_en    = gpio_cfg->int_en,
        .int_type  = gpio_cfg->int_type,
    };

    int32_t status = wise_gpio_cfg(&base_cfg);
    if (status != WISE_SUCCESS) {
        return status;
    }

    if (gpio_cfg->mode != MODE_PIO || gpio_cfg->bus_type == BUS_TYPE_NONE) {
        return WISE_SUCCESS;
    }

    switch (gpio_cfg->bus_type) {
    case BUS_TYPE_SPI: {
        HAL_PIO_SPI_CONFIG_T spi_cfg = {0};
        switch (gpio_cfg->signal) {
        case SIGNAL_CLK:
            spi_cfg.clk.pin    = gpio_cfg->pin_idx;
            spi_cfg.clk.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_CS:
            spi_cfg.cs.pin    = gpio_cfg->pin_idx;
            spi_cfg.cs.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_MISO:
            spi_cfg.miso.pin    = gpio_cfg->pin_idx;
            spi_cfg.miso.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_MOSI:
            spi_cfg.mosi.pin    = gpio_cfg->pin_idx;
            spi_cfg.mosi.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_HOLD:
            spi_cfg.hold.pin    = gpio_cfg->pin_idx;
            spi_cfg.hold.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_WP:
            spi_cfg.wp.pin    = gpio_cfg->pin_idx;
            spi_cfg.wp.enable = gpio_cfg->signal_enable;
            break;
        default:
            return WISE_FAIL;
        }
        hal_intf_pio_spi_io_config(gpio_cfg->channel, &spi_cfg);
        break;
    }
    case BUS_TYPE_UART: {
        HAL_PIO_UART_CONFIG_T uart_cfg = {0};
        switch (gpio_cfg->signal) {
        case SIGNAL_TX:
            uart_cfg.tx.pin    = gpio_cfg->pin_idx;
            uart_cfg.tx.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_RX:
            uart_cfg.rx.pin    = gpio_cfg->pin_idx;
            uart_cfg.rx.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_RTS:
            uart_cfg.rts.pin    = gpio_cfg->pin_idx;
            uart_cfg.rts.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_CTS:
            uart_cfg.cts.pin    = gpio_cfg->pin_idx;
            uart_cfg.cts.enable = gpio_cfg->signal_enable;
            break;
        default:
            return WISE_FAIL;
        }
        hal_intf_pio_uart_io_config(gpio_cfg->channel, &uart_cfg);
        break;
    }
    case BUS_TYPE_I2C: {
        HAL_PIO_I2C_CONFIG_T i2c_cfg = {0};
        switch (gpio_cfg->signal) {
        case SIGNAL_SCL:
            i2c_cfg.scl.pin    = gpio_cfg->pin_idx;
            i2c_cfg.scl.enable = gpio_cfg->signal_enable;
            break;
        case SIGNAL_SDA:
            i2c_cfg.sda.pin    = gpio_cfg->pin_idx;
            i2c_cfg.sda.enable = gpio_cfg->signal_enable;
            break;
        default:
            return WISE_FAIL;
        }
        hal_intf_pio_i2c_io_config(gpio_cfg->channel, &i2c_cfg);
        break;
    }
    default:
        return WISE_FAIL;
    }

    return WISE_SUCCESS;
}
#endif

void wise_gpio_set_mode(uint8_t pin_idx, uint8_t mode)
{
    hal_intf_gpio_set_mode(pin_idx, mode);
}

void wise_gpio_set_direction(uint8_t pin_idx, GPIO_DIRECTION direction)
{
    hal_intf_gpio_set_io_dirction(pin_idx, (uint8_t)direction);
}

uint8_t wise_gpio_get_direction(uint8_t pin_idx)
{
    return hal_intf_gpio_get_io_direction(pin_idx);
}

void wise_gpio_set_intrpt(uint8_t pin_idx, uint8_t enable, uint8_t int_type)
{
    hal_intf_gpio_set_intrpt(pin_idx, enable, int_type);
}

void wise_gpio_set_pull_sel(uint8_t pin_idx, uint8_t pull_sel)
{
    hal_intf_gpio_set_pull_sel(pin_idx, pull_sel);
}

void wise_gpio_set_debs_time(uint8_t pin_idx, uint8_t debs_time)
{
    hal_intf_gpio_set_debs_time(pin_idx, debs_time);
}

void wise_gpio_set_driv_str(uint8_t pin_idx, uint8_t driv_str)
{
    hal_intf_gpio_set_driv_str(pin_idx, driv_str);
}

void wise_gpio_set_schmitt_trig(uint8_t pin_idx, uint8_t enable)
{
    hal_intf_gpio_set_schmitt_trig_en(pin_idx, enable);
}

void wise_gpio_set_pwm(uint8_t pwm_num, uint8_t pin_idx, uint8_t enable)
{
    hal_intf_gpio_set_pwm(pwm_num, pin_idx, enable);
}

void wise_gpio_set_pwmslow(uint8_t pin_idx, uint8_t enable)
{
    hal_intf_gpio_set_pwmslow(pin_idx, enable);
}

void wise_pwm_io_disable_pin(uint8_t gpio_index)
{
    wise_gpio_set_pull_sel(gpio_index, PUSEL_DISABLE);
    wise_gpio_set_pwmslow(gpio_index, DISABLE);
    for (uint32_t pwm_channel = 0; pwm_channel < CHIP_PWM_CHANNEL_NUM; pwm_channel++) {
        wise_gpio_set_pwm(pwm_channel, gpio_index, DISABLE);
    }
}

GPIO_STATUS wise_gpio_read(uint8_t pin_idx)
{
    uint8_t pinSts = hal_intf_gpio_read_pin(pin_idx);

    if (pinSts) {
        return GPIO_HIGH;
    } else {
        return GPIO_LOW;
    }
}

void wise_gpio_write(uint8_t pin_idx, GPIO_STATUS status)
{
    if (wise_gpio_get_direction(pin_idx) == GPIO_DIR_OUTPUT) {
        hal_intf_gpio_write_pin(pin_idx, (uint8_t)status);
    } else {
        debug_printf("Error: GPIO %d is not configured as output!\n", pin_idx);
    }
}

void wise_gpio_toggle(uint8_t pin_idx)
{
    if (wise_gpio_get_direction(pin_idx) == GPIO_DIR_OUTPUT) {
        GPIO_STATUS pinSts = wise_gpio_read(pin_idx);
        wise_gpio_write(pin_idx, (pinSts == GPIO_HIGH) ? GPIO_LOW : GPIO_HIGH);
    } else {
        debug_print("Error: GPIO %d is not configured as output!\n", pin_idx);
    }
}

int32_t wise_gpio_register_callback(uint8_t gpio_idx, CALLBACK_T cb, void *context)
{
    return hal_intf_gpio_register_callback(gpio_idx, cb, context);
}

int32_t wise_gpio_unregister_callback(uint8_t gpio_idx)
{
    return hal_intf_gpio_unregister_callback(gpio_idx);
}

void wise_gpio_set_debug_bus(GPIO_DBG_CFG_INFO* gpio_dbg_cfg)
{
    hal_intf_gpio_set_debug_bus(gpio_dbg_cfg);
}

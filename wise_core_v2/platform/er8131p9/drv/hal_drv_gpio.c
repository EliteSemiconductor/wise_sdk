/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_gpio.h"
#include "hdl/gpio_er8130.h"
#include "hal_intf_gpio.h"

static CALLBACK_ENTRY_T gpio_callbacks[CHIP_GPIO_NUM];

void hal_drv_gpio_set_mode(uint8_t pin_idx, uint8_t mode)
{
    gpio_set_mode_er8130(pin_idx, mode);
}

void hal_drv_gpio_set_io_direction(uint8_t pin_idx, uint8_t in_out)
{
    gpio_set_io_direction_er8130(pin_idx, in_out);
}

uint8_t hal_drv_gpio_get_io_direction(uint8_t pin_idx)
{
    return gpio_get_io_direction_er8130(pin_idx);
}

void hal_drv_gpio_set_debs_time(uint8_t pin_idx, uint8_t debs_time)
{
    gpio_set_debs_time_er8130(pin_idx, debs_time);
}

void hal_drv_gpio_set_driv_str(uint8_t pin_idx, uint8_t driv_str)
{
    gpio_set_driv_str_er8130(pin_idx, driv_str);
}

void hal_drv_gpio_set_pull_sel(uint8_t pin_idx, uint8_t pull_sel)
{
    gpio_set_pull_sel_er8130(pin_idx, pull_sel);
}

void hal_drv_gpio_set_intrpt(uint8_t pin_idx, uint8_t enable, uint8_t int_type)
{
    gpio_set_intrpt_er8130(pin_idx, enable, int_type);
}

void hal_drv_gpio_set_schmitt_trig_en(uint8_t pin_idx, uint8_t trig_en)
{
    gpio_set_schmitt_trig_en_er8130(pin_idx, trig_en);
}

uint8_t hal_drv_gpio_get_pin(uint8_t pin_idx)
{
    return gpio_read_pin_er8130(pin_idx);
}

void hal_drv_gpio_set_pin(uint8_t pin_idx, uint8_t val)
{
    gpio_write_pin_er8130(pin_idx, val);
}

uint8_t hal_drv_gpio_get_int_status(uint8_t pin_idx)
{
    return gpio_get_int_status_er8130(pin_idx);
}

void hal_drv_gpio_clear_int_status(uint8_t pin_idx)
{
    gpio_clear_int_status_er8130(pin_idx);
}

uint32_t hal_drv_gpio_raw_int_status(void)
{
    return gpio_get_raw_int_status_er8130();
}

void hal_drv_gpio_clear_raw_int_status(uint32_t int_status)
{
    gpio_clear_raw_int_status_er8130(int_status);
}

void hal_drv_gpio_set_pwm(uint8_t pwm_num, uint8_t pin_idx, uint8_t enable)
{
    gpio_set_pwm_er8130(pwm_num, pin_idx, enable);
}

void hal_drv_gpio_set_pwmslow(uint8_t pin_idx, uint8_t enable)
{
    gpio_set_pwmslow_er8130(pin_idx, enable);
}

void hal_drv_gpio_set_debug_bus(GPIO_DBG_CFG_INFO* gpio_dbg_cfg)
{
    gpio_set_debug_bus_er8130(gpio_dbg_cfg->gpio_pin_idx, 
                                gpio_dbg_cfg->dbg_bus_module_idx, 
                                gpio_dbg_cfg->dbg_bus_signal_idx, 
                                gpio_dbg_cfg->dbg_id);
}

HAL_STATUS hal_drv_gpio_register_callback(uint8_t gpio_idx, CALLBACK_T cb, void *context)
{
    if (gpio_idx < CHIP_GPIO_NUM) {
        gpio_callbacks[gpio_idx].callback = cb;
        gpio_callbacks[gpio_idx].context  = context;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_gpio_unregister_callback(uint8_t gpio_idx)
{
    if (gpio_idx < CHIP_GPIO_NUM) {
        gpio_callbacks[gpio_idx].callback = NULL;
        gpio_callbacks[gpio_idx].context  = NULL;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

static void hal_drv_gpio_trigger_callback(uint8_t gpio_idx)
{
    if (gpio_callbacks[gpio_idx].callback) {
        gpio_callbacks[gpio_idx].callback(gpio_callbacks[gpio_idx].context, gpio_idx);
    }
}

WEAK_ISR void GPIO_IRQHandler(void)
{
    uint32_t pinMask = hal_drv_gpio_raw_int_status();
    hal_drv_gpio_clear_raw_int_status(pinMask);

    for (uint8_t i = 0; i < CHIP_GPIO_NUM; i++) {
        if (pinMask & (1UL << i)) {
            hal_drv_gpio_trigger_callback(i);
        }
    }
}

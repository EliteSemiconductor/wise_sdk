/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_gpio.h"
#include "drv/hal_drv_gpio.h"

#ifndef HAL_DRV_GPIO_SET_DEBUG_BUS
#define HAL_DRV_GPIO_SET_DEBUG_BUS
#endif

int32_t hal_intf_gpio_set_mode(uint8_t pin_idx, uint8_t mode)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }

    hal_drv_gpio_set_mode(pin_idx, mode);
    return HAL_NO_ERR;
}

int32_t hal_intf_gpio_set_io_dirction(uint8_t pin_idx, uint8_t in_out)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }

    hal_drv_gpio_set_io_direction(pin_idx, in_out);
    return HAL_NO_ERR;
}

uint8_t hal_intf_gpio_get_io_direction(uint8_t pin_idx)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }
    return hal_drv_gpio_get_io_direction(pin_idx);
}

int32_t hal_intf_gpio_set_debs_time(uint8_t pin_idx, uint8_t debs_time)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }

    hal_drv_gpio_set_debs_time(pin_idx, debs_time);
    return HAL_NO_ERR;
}

int32_t hal_intf_gpio_set_driv_str(uint8_t pin_idx, uint8_t driv_str)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }

    hal_drv_gpio_set_driv_str(pin_idx, driv_str);
    return HAL_NO_ERR;
}

int32_t hal_intf_gpio_set_pull_sel(uint8_t pin_idx, uint8_t pull_sel)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }

    hal_drv_gpio_set_pull_sel(pin_idx, pull_sel);
    return HAL_NO_ERR;
}

int32_t hal_intf_gpio_set_intrpt(uint8_t pin_idx, uint8_t enable,
                                 uint8_t int_type)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }

    hal_drv_gpio_set_intrpt(pin_idx, enable, int_type);
    return HAL_NO_ERR;
}

int32_t hal_intf_gpio_set_schmitt_trig_en(uint8_t pin_idx, uint8_t trig_en)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }

    hal_drv_gpio_set_schmitt_trig_en(pin_idx, trig_en);
    return HAL_NO_ERR;
}

uint8_t hal_intf_gpio_read_pin(uint8_t pin_idx)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }
    return hal_drv_gpio_get_pin(pin_idx);
}

int32_t hal_intf_gpio_write_pin(uint8_t pin_idx, uint8_t val)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }
    hal_drv_gpio_set_pin(pin_idx, val);
    return HAL_NO_ERR;
}

uint8_t hal_intf_gpio_get_int_status(uint8_t pin_idx)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }
    return hal_drv_gpio_get_int_status(pin_idx);
}

int32_t hal_intf_gpio_clear_int_status(uint8_t pin_idx)
{
    if(pin_idx >= CHIP_GPIO_NUM) {
        return HAL_ERR;
    }
    hal_drv_gpio_clear_int_status(pin_idx);
    return HAL_NO_ERR;
}

uint32_t hal_intf_gpio_get_raw_int_status(void)
{
    return hal_drv_gpio_raw_int_status();
}

int32_t hal_intf_gpio_set_pwm(uint8_t pwm_num, uint8_t pin_idx, uint8_t enable)
{
    hal_drv_gpio_set_pwm(pwm_num, pin_idx, enable);
    return HAL_NO_ERR;
}

int32_t hal_intf_gpio_set_pwmslow(uint8_t pin_idx, uint8_t enable)
{
    hal_drv_gpio_set_pwmslow(pin_idx, enable);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_gpio_register_callback(uint8_t gpio_idx, CALLBACK_T cb, void *context)
{
    return hal_drv_gpio_register_callback(gpio_idx, cb, context);
}

HAL_STATUS hal_intf_gpio_unregister_callback(uint8_t gpio_idx)
{
    return hal_drv_gpio_unregister_callback(gpio_idx);
}

void hal_intf_gpio_set_debug_bus(GPIO_DBG_CFG_INFO* gpio_dbg_cfg)
{
	HAL_DRV_GPIO_SET_DEBUG_BUS(gpio_dbg_cfg);
}

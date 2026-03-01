/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_GPIO_H
#define __HAL_DRV_GPIO_H

#include "esmt_chip_specific.h"
#include "hal_intf_gpio.h"
#include <stdint.h>


void hal_drv_gpio_set_mode(uint8_t pin_idx, uint8_t mode);
void hal_drv_gpio_set_io_direction(uint8_t pin_idx, uint8_t in_out);
uint8_t hal_drv_gpio_get_io_direction(uint8_t pin_idx);
void hal_drv_gpio_set_debs_time(uint8_t pin_idx, uint8_t debs_time);
void hal_drv_gpio_set_driv_str(uint8_t pin_idx, uint8_t driv_str);
void hal_drv_gpio_set_pull_sel(uint8_t pin_idx, uint8_t pull_sel);
void hal_drv_gpio_set_intrpt(uint8_t pin_idx, uint8_t enable, uint8_t int_type);
void hal_drv_gpio_set_schmitt_trig_en(uint8_t pin_idx, uint8_t trig_en);
uint8_t hal_drv_gpio_get_pin(uint8_t pin_idx);
void hal_drv_gpio_set_pin(uint8_t pin_idx, uint8_t val);
uint8_t hal_drv_gpio_get_int_status(uint8_t pin_idx);
void hal_drv_gpio_clear_int_status(uint8_t pin_idx);
uint32_t hal_drv_gpio_raw_int_status(void);
void hal_drv_gpio_clear_raw_int_status(uint32_t int_status);
void hal_drv_gpio_set_pwm(uint8_t pwm_num, uint8_t pin_idx, uint8_t enable);
void hal_drv_gpio_set_pwmslow(uint8_t pin_idx, uint8_t enable);
void hal_drv_gpio_set_debug_bus(GPIO_DBG_CFG_INFO* gpio_dbg_cfg);

HAL_STATUS hal_drv_gpio_register_callback(uint8_t gpio_idx, CALLBACK_T cb, void *context);
HAL_STATUS hal_drv_gpio_unregister_callback(uint8_t gpio_idx);


#define HAL_DRV_GPIO_SET_DEBUG_BUS          hal_drv_gpio_set_debug_bus


#endif

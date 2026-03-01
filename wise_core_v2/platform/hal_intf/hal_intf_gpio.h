/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_GPIO_H
#define __HAL_INTF_GPIO_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef struct {
    uint8_t gpio_pin_idx;
    uint8_t dbg_bus_module_idx;
    uint8_t dbg_bus_signal_idx;
    uint8_t dbg_id;
} GPIO_DBG_CFG_INFO;


int32_t hal_intf_gpio_set_mode(uint8_t pin_idx, uint8_t mode);
int32_t hal_intf_gpio_set_io_dirction(uint8_t pin_idx, uint8_t in_out);
uint8_t hal_intf_gpio_get_io_direction(uint8_t pin_idx);
int32_t hal_intf_gpio_set_debs_time(uint8_t pin_idx, uint8_t debs_time);
int32_t hal_intf_gpio_set_driv_str(uint8_t pin_idx, uint8_t driv_str);
int32_t hal_intf_gpio_set_pull_sel(uint8_t pin_idx, uint8_t pull_sel);
int32_t hal_intf_gpio_set_intrpt(uint8_t pin_idx, uint8_t enable, uint8_t int_type);
int32_t hal_intf_gpio_set_schmitt_trig_en(uint8_t pin_idx, uint8_t trig_en);
int32_t hal_intf_gpio_set_pwm(uint8_t pwm_num, uint8_t pin_idx, uint8_t enable);
int32_t hal_intf_gpio_set_pwmslow(uint8_t pin_idx, uint8_t enable);
uint8_t hal_intf_gpio_read_pin(uint8_t pin_idx);
int32_t hal_intf_gpio_write_pin(uint8_t pin_idx, uint8_t val);
uint8_t hal_intf_gpio_get_int_status(uint8_t pin_idx);
int32_t hal_intf_gpio_clear_int_status(uint8_t pin_idx);
uint32_t hal_intf_gpio_get_raw_int_status(void);
void hal_intf_gpio_set_debug_bus(GPIO_DBG_CFG_INFO* gpio_dbg_cfg);

HAL_STATUS hal_intf_gpio_register_callback(uint8_t gpio_idx, CALLBACK_T cb, void *context);
HAL_STATUS hal_intf_gpio_unregister_callback(uint8_t gpio_idx);

#endif /* __HAL_INTF_GPIO_H */

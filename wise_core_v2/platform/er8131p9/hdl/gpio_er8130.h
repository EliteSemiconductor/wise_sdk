/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __GPIO_ER8130_H
#define __GPIO_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    INT_TYPE_LEVEL_LOW   = 0,
    INT_TYPE_LEVEL_HIGH  = 1,
    INT_TYPE_EDGE_FALING = 2,
    INT_TYPE_EDGE_RISING = 3,
    INT_TYPE_EDGE_BOTH   = 4,
    INT_TYPE_INVALID     = 5,
} GPIO_INT_TYPE;

enum {
    MODE_INPUT   = 0,
    MODE_OUTPUT  = 1,
    MODE_INVALID = 0xff, // for alternative funcions
};

void gpio_set_mode_er8130(uint8_t pin_idx, uint8_t mode);
void gpio_set_io_direction_er8130(uint8_t pin_idx, uint8_t in_out);
uint8_t gpio_get_io_direction_er8130(uint8_t pin_idx);
void gpio_set_debs_time_er8130(uint8_t pin_idx, uint8_t debs_time);
void gpio_set_driv_str_er8130(uint8_t pin_idx, uint8_t driv_str);
void gpio_set_pull_sel_er8130(uint8_t pin_idx, uint8_t pull_sel);
void gpio_set_intrpt_er8130(uint8_t pin_idx, uint8_t enable, uint8_t int_type);
void gpio_set_schmitt_trig_en_er8130(uint8_t pin_idx, uint8_t trig_en);
void gpio_set_pwm_er8130(uint8_t pwm_num, uint8_t pin_idx, uint8_t enable);
void gpio_set_pwmslow_er8130(uint8_t pin_idx, uint8_t enable);
uint8_t gpio_get_int_status_er8130(uint8_t pin_idx);
void gpio_clear_int_status_er8130(uint8_t pin_idx);
uint32_t gpio_get_raw_int_status_er8130();
void gpio_set_gio_sel_er8130(uint8_t pin_idx, uint8_t gio_sel);
uint8_t gpio_read_pin_er8130(uint8_t pin_idx);
void gpio_write_pin_er8130(uint8_t pin_idx, uint8_t val);
uint8_t gpio_get_int_status_er8130(uint8_t pin_idx);
void gpio_clear_int_status_er8130(uint8_t pin_idx);
uint32_t gpio_get_raw_int_status_er8130(void);
void gpio_clear_raw_int_status_er8130(uint32_t int_status);
void gpio_set_debug_bus_er8130(uint8_t pin_idx, uint8_t dbg_module_idx, uint8_t dbg_single_idx, uint8_t dbg_id);

#endif /* __GPIO_ER8130_H */

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

/*
 * GIO mode function select (GIO_CTRL_REG 0x40014A80)
 * Only supported on GPIO 2, 3, 4.
 *   GPIO 2 : register bits [4:0]
 *   GPIO 3 : register bits [12:8]
 *   GPIO 4 : register bits [20:16]
 *
 *   GIO_FUN_TX_RX_EN      (0) : TX_EN / RX_EN indicator
 *   GIO_FUN_TX_DATA_START (2) : TX start to send data
 *   GIO_FUN_RX_SYNC_WORD  (6) : RX receive sync word
 */
typedef enum {
    GIO_FUN_TX_RX_EN      = 0,
    GIO_FUN_TX_DATA_START = 2,
    GIO_FUN_RX_SYNC_WORD  = 6,
} GPIO_GIO_FUN;

enum {
    MODE_INPUT   = 0,
    MODE_OUTPUT  = 1,
    MODE_INVALID = 0xff, // for alternative funcions
};

enum {
    DEBUG_TARGET_PMU   = 3,
    DEBUG_TARGET_MAC   = 4,
    DEBUG_TARGET_CMU   = 17,
    DEBUG_TARGET_ANCTL = 19,
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
uint8_t gpio_read_pin_er8130(uint8_t pin_idx);
void gpio_write_pin_er8130(uint8_t pin_idx, uint8_t val);
uint8_t gpio_get_int_status_er8130(uint8_t pin_idx);
void gpio_clear_int_status_er8130(uint8_t pin_idx);
uint32_t gpio_get_raw_int_status_er8130(void);
void gpio_clear_raw_int_status_er8130(uint32_t int_status);
void gpio_set_debug_bus_er8130(uint8_t pin_idx, uint8_t target_idx, uint8_t dbg_signal_idx, uint8_t dbg_bus_idx);
void gpio_set_gio_fun_er8130(uint8_t pin_idx, uint8_t gio_fun);

#endif /* __GPIO_ER8130_H */

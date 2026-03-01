/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __PWMSLOW_H__
#define __PWMSLOW_H__

#include "api/wise_gpio_api.h"
#include "esmt_chip_specific.h"
#include "types.h"

void pwmslow_start_er8130(void);
void pwmslow_stop_er8130(void);
void pwmslow_enable_oneshot_interrupt_er8130(void);
void pwmslow_disable_oneshot_interrupt_er8130(void);
void pwmslow_clear_interrupt_flag_er8130(void);
uint8_t pwmslow_get_status_er8130(void);
void pwmslow_cfg_output_period_er8130(uint32_t period, uint32_t active_period,
                                      uint32_t center_align_en,
                                      uint32_t oneshot_en,
                                      uint32_t oneshot_num);
void pwmslow_cfg_output_frequency_er8130(uint32_t frequency,
                                         uint32_t duty_percent,
                                         uint32_t center_align_en,
                                         uint32_t oneshot_en,
                                         uint32_t oneshot_num);
void pwmslow_set_io_pin_er8130(uint32_t idle_status, uint32_t low_active_en);
#endif /* __PWMSLOW_H__ */

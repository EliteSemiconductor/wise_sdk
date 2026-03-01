/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __PWM_H__
#define __PWM_H__

#include "wise_gpio_api.h"
#include "esmt_chip_specific.h"
#include "types.h"

#define PWM_CH_0 (uint32_t)(0x1ul << 0) //!<	PWM Channel 0
#define PWM_CH_1 (uint32_t)(0x1ul << 1) //!<	PWM Channel 1
#define PWM_CH_2 (uint32_t)(0x1ul << 2) //!<	PWM Channel 2
#define PWM_CH_3 (uint32_t)(0x1ul << 3) //!<	PWM Channel 3
#define PWM_CH_4 (uint32_t)(0x1ul << 4) //!<	PWM Channel 4
#define PWM_CH_5 (uint32_t)(0x1ul << 5) //!<	PWM Channel 5

#define PWM_INT_MASK                                                           \
    (PWM_CH_0 | PWM_CH_1 | PWM_CH_2 | PWM_CH_3 | PWM_CH_4 | PWM_CH_5)

void pwm_start_er8130(uint32_t channel_mask);
void pwm_stop_er8130(uint32_t channel_mask);
bool pwm_is_oneshot_interrupt_enabled_er8130(void);
void pwm_enable_oneshot_interrupt_er8130(uint32_t channel_mask);
void pwm_disable_oneshot_interrupt_er8130(uint32_t channel_mask);
void pwm_clear_interrupt_flag_er8130(uint32_t channel_mask);
uint8_t pwm_get_status_er8130(void);
void pwm_cfg_output_period_er8130(uint32_t channel_mask, uint32_t period,
                                  uint32_t active_period,
                                  uint32_t center_align_en, uint32_t oneshot_en,
                                  uint32_t oneshot_num);
void pwm_cfg_output_frequency_er8130(uint32_t channel_mask, uint32_t frequency,
                                     uint32_t duty_percent,
                                     uint32_t center_align_en,
                                     uint32_t oneshot_en, uint32_t oneshot_num);
void pwm_set_io_for_channel_pin_er8130(uint32_t channel, uint32_t idle_status,
                                       uint32_t low_active_en);
#endif /* __PWM_H__ */

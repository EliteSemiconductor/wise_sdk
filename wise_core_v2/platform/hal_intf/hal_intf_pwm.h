/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_PWM_H
#define __HAL_INTF_PWM_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    PWM_EVENT_CH0_TRIGGER,
    PWM_EVENT_CH1_TRIGGER,
    PWM_EVENT_CH2_TRIGGER,
    PWM_EVENT_CH3_TRIGGER,
    PWM_EVENT_CH4_TRIGGER,
    PWM_EVENT_CH5_TRIGGER,
    PWM_MAX_EVENTS,
} PWM_CB_EVENT_T;

typedef struct {
    uint32_t idle_status;
    uint32_t low_active_en;
    uint32_t period;
    uint32_t active_period;
    uint32_t frequency_Hz;
    uint32_t duty_percent;
    uint32_t center_align_en;
    uint32_t oneshot_en;
    uint32_t oneshot_num;
} HAL_PWM_CONF_T;

HAL_STATUS hal_intf_pwm_register_callback(PWM_CB_EVENT_T event, CALLBACK_T cb,
                                           void *context);
HAL_STATUS hal_intf_pwm_unregister_callback(PWM_CB_EVENT_T event);
void hal_intf_pwm_start(uint32_t channel_mask);
void hal_intf_pwm_stop(uint32_t channel_mask);
void hal_intf_pwm_enable_oneshot_interrupt(uint32_t channel_mask);
void hal_intf_pwm_disable_oneshot_interrupt(uint32_t channel_mask);
void hal_intf_pwm_clear_interrupt_flag(uint32_t channel_mask);
uint32_t hal_intf_pwm_get_status(void);
void hal_intf_pwm_set_io_for_channel_pin(uint32_t channel, HAL_PWM_CONF_T *cfg);
void hal_intf_pwm_cfg_output_period(uint32_t channel_mask, HAL_PWM_CONF_T *cfg);
void hal_intf_pwm_cfg_output_frequency(uint32_t channel_mask,
                                       HAL_PWM_CONF_T *cfg);
#endif /* __HAL_INTF_PWM_H */

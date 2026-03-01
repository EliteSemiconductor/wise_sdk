/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_PWM_H
#define __HAL_DRV_PWM_H

#include "hdl/pwm_er8130.h"
#include "hal_intf_pwm.h"
#include <stdint.h>

HAL_STATUS hal_drv_pwm_register_callback(PWM_CB_EVENT_T event, CALLBACK_T cb,
                                          void *context);
HAL_STATUS hal_drv_pwm_unregister_callback(PWM_CB_EVENT_T event);

void hal_drv_pwm_start(uint32_t channel_mask);
void hal_drv_pwm_stop(uint32_t channel_mask);
void hal_drv_pwm_enable_oneshot_interrupt(uint32_t channel_mask);
void hal_drv_pwm_disable_oneshot_interrupt(uint32_t channel_mask);
void hal_drv_pwm_clear_interrupt_flag(uint32_t channel_mask);
uint8_t hal_drv_pwm_get_status(void);
void hal_drv_pwm_cfg_output_period(uint32_t channel_mask, uint32_t period,
                                   uint32_t active_period,
                                   uint32_t center_align_en,
                                   uint32_t oneshot_en, uint32_t oneshot_num);
void hal_drv_pwm_cfg_output_frequency(uint32_t channel_mask, uint32_t frequency,
                                      uint32_t duty_percent,
                                      uint32_t center_align_en,
                                      uint32_t oneshot_en,
                                      uint32_t oneshot_num);
void hal_drv_pwm_set_io_for_channel_pin(uint32_t channel, uint32_t idle_status,
                                        uint32_t low_active_en);

#endif /* __HAL_DRV_PWM_H */

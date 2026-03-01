/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_PWMSLOW_H
#define __HAL_DRV_PWMSLOW_H

#include "hdl/pwmslow_er8130.h"
#include "hal_intf_pwmslow.h"
#include <stdint.h>

void hal_drv_pwmslow_start(void);
void hal_drv_pwmslow_stop(void);
void hal_drv_pwmslow_enable_oneshot_interrupt(void);
void hal_drv_pwmslow_disable_oneshot_interrupt(void);
void hal_drv_pwmslow_clear_interrupt_flag(void);
uint8_t hal_drv_pwmslow_get_status(void);
void hal_drv_pwmslow_cfg_output_period(uint32_t period, uint32_t active_period,
                                       uint32_t center_align_en,
                                       uint32_t oneshot_en,
                                       uint32_t oneshot_num);
void hal_drv_pwmslow_cfg_output_frequency(uint32_t frequency,
                                          uint32_t duty_percent,
                                          uint32_t center_align_en,
                                          uint32_t oneshot_en,
                                          uint32_t oneshot_num);
void hal_drv_pwmslow_set_io_pin(uint32_t idle_status, uint32_t low_active_en);
HAL_STATUS hal_drv_pwmslow_register_callback(PWMSLOW_CB_EVENT_T event,
                                              CALLBACK_T cb, void *context);
HAL_STATUS hal_drv_pwmslow_unregister_callback(PWMSLOW_CB_EVENT_T event);
#endif /* __HAL_DRV_PWMSLOW_H */

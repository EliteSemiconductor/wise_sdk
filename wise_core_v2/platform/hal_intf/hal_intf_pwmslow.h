/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_PWMSLOW_H
#define __HAL_INTF_PWMSLOW_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    PWMSLOW_EVENT_TRIGGER,
    PWMSLOW_MAX_EVENTS,
} PWMSLOW_CB_EVENT_T;

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
} HAL_PWMSLOW_CONF_T;

HAL_STATUS hal_intf_pwmslow_register_callback(PWMSLOW_CB_EVENT_T event,
                                               CALLBACK_T cb, void *context);
HAL_STATUS hal_intf_pwmslow_unregister_callback(PWMSLOW_CB_EVENT_T event);
void hal_intf_pwmslow_start(void);
void hal_intf_pwmslow_stop(void);
void hal_intf_pwmslow_enable_oneshot_interrupt(void);
void hal_intf_pwmslow_disable_oneshot_interrupt(void);
void hal_intf_pwmslow_clear_interrupt_flag(void);
uint32_t hal_intf_pwmslow_get_status(void);
void hal_intf_pwmslow_set_io_pin(HAL_PWMSLOW_CONF_T *cfg);
void hal_intf_pwmslow_cfg_output_period(HAL_PWMSLOW_CONF_T *cfg);
void hal_intf_pwmslow_cfg_output_frequency(HAL_PWMSLOW_CONF_T *cfg);
#endif /* __HAL_INTF_PWMSLOW_H */

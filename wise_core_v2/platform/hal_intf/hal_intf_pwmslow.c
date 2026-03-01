/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_pwmslow.h"
#include "drv/hal_drv_pwmslow.h"

HAL_STATUS hal_intf_pwmslow_register_callback(PWMSLOW_CB_EVENT_T event,
                                               CALLBACK_T cb, void *context)
{
    return hal_drv_pwmslow_register_callback(event, cb, context);
}

HAL_STATUS hal_intf_pwmslow_unregister_callback(PWMSLOW_CB_EVENT_T event)
{
    return hal_drv_pwmslow_unregister_callback(event);
}

void hal_intf_pwmslow_start(void)
{
    hal_drv_pwmslow_start();
}

void hal_intf_pwmslow_stop(void)
{
    hal_drv_pwmslow_stop();
}

void hal_intf_pwmslow_enable_oneshot_interrupt(void)
{
    hal_drv_pwmslow_enable_oneshot_interrupt();
}

void hal_intf_pwmslow_disable_oneshot_interrupt(void)
{
    hal_drv_pwmslow_disable_oneshot_interrupt();
}

void hal_intf_pwmslow_clear_interrupt_flag(void)
{
    hal_drv_pwmslow_clear_interrupt_flag();
}

uint32_t hal_intf_pwmslow_get_status(void)
{
    return hal_drv_pwmslow_get_status();
}

void hal_intf_pwmslow_set_io_pin(HAL_PWMSLOW_CONF_T *cfg)
{
    hal_drv_pwmslow_set_io_pin(cfg->idle_status, cfg->low_active_en);
}

void hal_intf_pwmslow_cfg_output_period(HAL_PWMSLOW_CONF_T *cfg)
{
    hal_drv_pwmslow_cfg_output_period(cfg->period, cfg->active_period,
                                      cfg->center_align_en, cfg->oneshot_en,
                                      cfg->oneshot_num);
}

void hal_intf_pwmslow_cfg_output_frequency(HAL_PWMSLOW_CONF_T *cfg)
{
    hal_drv_pwmslow_cfg_output_frequency(cfg->frequency_Hz, cfg->duty_percent,
                                         cfg->center_align_en, cfg->oneshot_en,
                                         cfg->oneshot_num);
}

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_pwm.h"
#include "drv/hal_drv_pwm.h"

HAL_STATUS hal_intf_pwm_register_callback(PWM_CB_EVENT_T event, CALLBACK_T cb, void *context)
{
    return hal_drv_pwm_register_callback(event, cb, context);
}

HAL_STATUS hal_intf_pwm_unregister_callback(PWM_CB_EVENT_T event)
{
    return hal_drv_pwm_unregister_callback(event);
}

void hal_intf_pwm_start(uint32_t channel_mask)
{
    hal_drv_pwm_start(channel_mask);
}

void hal_intf_pwm_stop(uint32_t channel_mask)
{
    hal_drv_pwm_stop(channel_mask);
}

void hal_intf_pwm_enable_oneshot_interrupt(uint32_t channel_mask)
{
    hal_drv_pwm_enable_oneshot_interrupt(channel_mask);
}

void hal_intf_pwm_disable_oneshot_interrupt(uint32_t channel_mask)
{
    hal_drv_pwm_disable_oneshot_interrupt(channel_mask);
}

void hal_intf_pwm_clear_interrupt_flag(uint32_t channel_mask)
{
    hal_drv_pwm_clear_interrupt_flag(channel_mask);
}

uint32_t hal_intf_pwm_get_status(void)
{
    return hal_drv_pwm_get_status();
}

void hal_intf_pwm_set_io_for_channel_pin(uint32_t channel, HAL_PWM_CONF_T *cfg)
{
    hal_drv_pwm_set_io_for_channel_pin(channel, cfg->idle_status, cfg->low_active_en);
}

void hal_intf_pwm_cfg_output_period(uint32_t channel_mask, HAL_PWM_CONF_T *cfg)
{
    hal_drv_pwm_cfg_output_period(channel_mask, cfg->period, cfg->active_period, cfg->center_align_en, cfg->oneshot_en, cfg->oneshot_num);
}

void hal_intf_pwm_cfg_output_frequency(uint32_t channel_mask, HAL_PWM_CONF_T *cfg)
{
    hal_drv_pwm_cfg_output_frequency(channel_mask, cfg->frequency_Hz, cfg->duty_percent, cfg->center_align_en, cfg->oneshot_en, cfg->oneshot_num);
}

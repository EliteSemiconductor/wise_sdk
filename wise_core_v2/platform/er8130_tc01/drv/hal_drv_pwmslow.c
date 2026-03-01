/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_pwmslow.h"

static CALLBACK_ENTRY_T pwmslow_callbacks[PWMSLOW_MAX_EVENTS];

void hal_drv_pwmslow_start(void)
{
    pwmslow_start_er8130();
}

void hal_drv_pwmslow_stop(void)
{
    pwmslow_stop_er8130();
    NVIC_DisableIRQ((IRQn_Type)PWM_SLOW_IRQn);
}

void hal_drv_pwmslow_enable_oneshot_interrupt(void)
{
    pwmslow_enable_oneshot_interrupt_er8130();
    NVIC_EnableIRQ((IRQn_Type)PWM_SLOW_IRQn);
}

void hal_drv_pwmslow_disable_oneshot_interrupt(void)
{
    pwmslow_disable_oneshot_interrupt_er8130();
    NVIC_DisableIRQ((IRQn_Type)PWM_SLOW_IRQn);
}

void hal_drv_pwmslow_clear_interrupt_flag(void)
{
    pwmslow_clear_interrupt_flag_er8130();
}

uint8_t hal_drv_pwmslow_get_status(void)
{
    return pwmslow_get_status_er8130();
}

void hal_drv_pwmslow_cfg_output_period(uint32_t period, uint32_t active_period,
                                       uint32_t center_align_en,
                                       uint32_t oneshot_en,
                                       uint32_t oneshot_num)
{
    pwmslow_cfg_output_period_er8130(period, active_period, center_align_en,
                                     oneshot_en, oneshot_num);
}

void hal_drv_pwmslow_cfg_output_frequency(uint32_t frequency,
                                          uint32_t duty_percent,
                                          uint32_t center_align_en,
                                          uint32_t oneshot_en,
                                          uint32_t oneshot_num)
{
    pwmslow_cfg_output_frequency_er8130(
        frequency, duty_percent, center_align_en, oneshot_en, oneshot_num);
}

void hal_drv_pwmslow_set_io_pin(uint32_t idle_status, uint32_t low_active_en)
{
    pwmslow_set_io_pin_er8130(idle_status, low_active_en);
}

HAL_STATUS hal_drv_pwmslow_register_callback(PWMSLOW_CB_EVENT_T event,
                                              CALLBACK_T cb, void *context)
{
    if (event < PWMSLOW_MAX_EVENTS) {
        pwmslow_callbacks[event].callback = cb;
        pwmslow_callbacks[event].context  = context;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_pwmslow_unregister_callback(PWMSLOW_CB_EVENT_T event)
{
    if (event < PWMSLOW_MAX_EVENTS) {
        pwmslow_callbacks[event].callback = NULL;
        pwmslow_callbacks[event].context  = NULL;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

static void hal_drv_pwmslow_trigger_callback(PWMSLOW_CB_EVENT_T event,
                                             uint8_t pwmslow_idx)
{
    if (pwmslow_callbacks[event].callback) {
        pwmslow_callbacks[event].callback(pwmslow_callbacks[event].context,
                                          pwmslow_idx);
    }
}

void pwmslow_irq_handler(void)
{
    hal_drv_pwmslow_get_status();
    hal_drv_pwmslow_clear_interrupt_flag();
    hal_drv_pwmslow_trigger_callback(PWMSLOW_EVENT_TRIGGER, 0);
}

WEAK_ISR void PWM_SLOW_IRQHandler(void)
{
    pwmslow_irq_handler();
}

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_pwm.h"

static CALLBACK_ENTRY_T pwm_callbacks[PWM_MAX_EVENTS];

void hal_drv_pwm_start(uint32_t channel_mask)
{
    pwm_start_er8130(channel_mask);
}

void hal_drv_pwm_stop(uint32_t channel_mask)
{
    pwm_stop_er8130(channel_mask);
    NVIC_DisableIRQ((IRQn_Type)PWM_IRQn);
}

void hal_drv_pwm_enable_oneshot_interrupt(uint32_t channel_mask)
{
    pwm_enable_oneshot_interrupt_er8130(channel_mask);
    NVIC_EnableIRQ((IRQn_Type)PWM_IRQn);
}

void hal_drv_pwm_disable_oneshot_interrupt(uint32_t channel_mask)
{
    pwm_disable_oneshot_interrupt_er8130(channel_mask);
    if (pwm_is_oneshot_interrupt_enabled_er8130() == FALSE) {
        NVIC_DisableIRQ((IRQn_Type)PWM_IRQn);
    }
}

void hal_drv_pwm_clear_interrupt_flag(uint32_t channel_mask)
{
    pwm_clear_interrupt_flag_er8130(channel_mask);
}

uint8_t hal_drv_pwm_get_status(void)
{
    return pwm_get_status_er8130();
}

void hal_drv_pwm_cfg_output_period(uint32_t channel_mask, uint32_t period,
                                   uint32_t active_period,
                                   uint32_t center_align_en,
                                   uint32_t oneshot_en, uint32_t oneshot_num)
{
    pwm_cfg_output_period_er8130(channel_mask, period, active_period,
                                 center_align_en, oneshot_en, oneshot_num);
}

void hal_drv_pwm_cfg_output_frequency(uint32_t channel_mask, uint32_t frequency,
                                      uint32_t duty_percent,
                                      uint32_t center_align_en,
                                      uint32_t oneshot_en, uint32_t oneshot_num)
{
    pwm_cfg_output_frequency_er8130(channel_mask, frequency, duty_percent,
                                    center_align_en, oneshot_en, oneshot_num);
}

void hal_drv_pwm_set_io_for_channel_pin(uint32_t channel, uint32_t idle_status,
                                        uint32_t low_active_en)
{
    pwm_set_io_for_channel_pin_er8130(channel, idle_status, low_active_en);
}

HAL_STATUS hal_drv_pwm_register_callback(PWM_CB_EVENT_T event, CALLBACK_T cb,
                                          void *context)
{
    if (event < PWM_MAX_EVENTS) {
        pwm_callbacks[event].callback = cb;
        pwm_callbacks[event].context  = context;
        return WISE_SUCCESS;
    }
    return WISE_FAIL;
}

HAL_STATUS hal_drv_pwm_unregister_callback(PWM_CB_EVENT_T event)
{
    if (event < PWM_MAX_EVENTS) {
        pwm_callbacks[event].callback = NULL;
        pwm_callbacks[event].context  = NULL;
        return WISE_SUCCESS;
    }
    return WISE_FAIL;
}

static void hal_drv_pwm_trigger_callback(PWM_CB_EVENT_T event, uint8_t pwm_idx)
{
    if (pwm_callbacks[event].callback) {
        pwm_callbacks[event].callback(pwm_callbacks[event].context, pwm_idx);
    }
}

void pwm_irq_handler(void)
{
    uint8_t interrupt_status = hal_drv_pwm_get_status();
    hal_drv_pwm_clear_interrupt_flag(interrupt_status);

    for (uint8_t ch = 0; ch < CHIP_PWM_CHANNEL_NUM; ch++) {
        if (interrupt_status & (1u << ch)) {
            hal_drv_pwm_trigger_callback(ch, ch);
        }
    }
}

WEAK_ISR void PWM_IRQHandler(void)
{
    pwm_irq_handler();
}

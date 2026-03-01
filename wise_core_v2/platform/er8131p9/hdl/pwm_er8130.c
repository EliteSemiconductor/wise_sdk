/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/pwm_er8130.h"

static inline void pwm_set_mode(uint32_t channel, uint32_t center_align_en,
                                uint32_t oneshot_en)
{
    PWM->MODE_SEL = (PWM->MODE_SEL & ~(1ul << (channel + PWM_PWM_MODE_0_POS))) |
                    (center_align_en << (channel + PWM_PWM_MODE_0_POS));
    PWM->MODE_SEL = (PWM->MODE_SEL & ~(1ul << (channel + PWM_ONESHOT_0_POS))) |
                    (oneshot_en << (channel + PWM_ONESHOT_0_POS));
}

static inline void pwm_set_timing(uint32_t channel, uint32_t period,
                                  uint32_t active_period, uint32_t oneshot_num)
{
    /** Each channel's registers are spaced 0xC bytes apart */
    *((volatile uint32_t *)(PWM_PERIOD_0_ADDR + (0xC * channel))) = period;
    *((volatile uint32_t *)(PWM_HIGH_PERIOD_0_ADDR + (0xC * channel))) =
        active_period;
    *((volatile uint32_t *)(PWM_SHOT_NUM_0_ADDR + (0xC * channel))) =
        oneshot_num;
}

void pwm_start_er8130(uint32_t channel_mask)
{
    PWM->EN              |= (channel_mask << 1) | 0x1;
    PWM->ONESHOT_TRIGGER  = 1;
}

void pwm_stop_er8130(uint32_t channel_mask)
{
    PWM->EN &= ~(channel_mask << 1);
}

void pwm_enable_oneshot_interrupt_er8130(uint32_t channel_mask)
{
    PWM->INT_EN |= channel_mask;
}

void pwm_disable_oneshot_interrupt_er8130(uint32_t channel_mask)
{
    PWM->INT_EN &= ~(channel_mask);
}

bool pwm_is_oneshot_interrupt_enabled_er8130(void)
{
    return !!(PWM->INT_EN & PWM_INT_MASK);
}

void pwm_clear_interrupt_flag_er8130(uint32_t channel_mask)
{
    PWM->RAW_INT_STATUS = channel_mask;
}

uint8_t pwm_get_status_er8130(void)
{
    return PWM->MASKED_INT_STATUS;
}

void pwm_set_io_for_channel_pin_er8130(uint32_t channel, uint32_t idle_status,
                                       uint32_t low_active_en)
{
    PWM->MODE_SEL = (PWM->MODE_SEL & ~(1ul << (channel + PWM_PARK_0_POS))) |
                    (idle_status << (channel + PWM_PARK_0_POS));
    PWM->MODE_SEL = (PWM->MODE_SEL & ~(1ul << (channel + PWM_INVERSE_0_POS))) |
                    (low_active_en << (channel + PWM_INVERSE_0_POS));
}

void pwm_cfg_output_period_er8130(uint32_t channel_mask, uint32_t period,
                                  uint32_t active_period,
                                  uint32_t center_align_en, uint32_t oneshot_en,
                                  uint32_t oneshot_num)
{
    uint32_t pwm_channel;

    if (period < 2) {
        period = 2;
    }

    if (active_period > period) {
        active_period = period;
    }

    for (pwm_channel = 0ul; pwm_channel < CHIP_PWM_CHANNEL_NUM; pwm_channel++) {
        if (channel_mask & (1ul << pwm_channel)) {
            pwm_set_mode(pwm_channel, center_align_en, oneshot_en);
            pwm_set_timing(pwm_channel, period, active_period, oneshot_num);
        }
    }
}

void pwm_cfg_output_frequency_er8130(uint32_t channel_mask, uint32_t frequency,
                                     uint32_t duty_percent,
                                     uint32_t center_align_en,
                                     uint32_t oneshot_en, uint32_t oneshot_num)
{
    uint32_t pwm_channel;
    uint32_t period, active_period;

    period = SystemCoreClock / frequency;

    if (period < 2) {
        period = 2;
    }

    if (duty_percent > 100) {
        duty_percent = 100;
    }

    active_period = (period * duty_percent) / 100;

    for (pwm_channel = 0ul; pwm_channel < CHIP_PWM_CHANNEL_NUM; pwm_channel++) {
        if (channel_mask & (1ul << pwm_channel)) {
            pwm_set_mode(pwm_channel, center_align_en, oneshot_en);
            pwm_set_timing(pwm_channel, period, active_period, oneshot_num);
        }
    }
}

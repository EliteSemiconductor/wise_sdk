/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/pwmslow_er8130.h"

void pwmslow_start_er8130(void)
{
    PWMSLOW->PWM_EN          = 1;
    PWMSLOW->ONESHOT_TRIGGER = 1;
}

void pwmslow_stop_er8130(void)
{
    PWMSLOW->PWM_EN = 0;
}

void pwmslow_enable_oneshot_interrupt_er8130(void)
{
    PWMSLOW->INT_EN = 1;
}

void pwmslow_disable_oneshot_interrupt_er8130(void)
{
    PWMSLOW->INT_EN = 0;
}

void pwmslow_clear_interrupt_flag_er8130(void)
{
    PWMSLOW->RAW_INT_STATUS = 1;
}

uint8_t pwmslow_get_status_er8130(void)
{
    return PWMSLOW->MASKED_INT_STATUS;
}

static inline void set_pwmslow_mode_sel_bit(uint32_t bit, uint32_t value)
{
    PWMSLOW->PWM_MODE_SEL = (PWMSLOW->PWM_MODE_SEL & ~(1ul << bit)) |
                            (value << bit);
}

static inline void configure_pwm_timing(uint32_t period, uint32_t high_period,
                                        uint32_t shot_num)
{
    PWMSLOW->PWM_PERIOD_0  = period;
    PWMSLOW->HIGH_PERIOD_0 = high_period;
    PWMSLOW->SHOT_NUM_0    = shot_num;
}

static inline void disable_pin_io_er8130(uint32_t pin)
{
    if (pin < 16) {
        IOCTRL->GPIO_PUSEL_REG0 =
            (IOCTRL->GPIO_PUSEL_REG0 & ~(0x3ul << (pin << 1))) |
            (PUSEL_DISABLE << (pin << 1));
    }
    (*((volatile uint32_t *)(IOCTRL_BASE + 0x200))) &= ~(1ul << pin);
    for (uint32_t i = 0; i < 6; i++) {
        (*((volatile uint32_t *)(IOCTRL_BASE + 0x210 + (0x4 * i)))) &=
            ~(1ul << pin);
    }
}

static inline void enable_pin_io_er8130(uint32_t pin)
{
    (*((volatile uint32_t *)(IOCTRL_BASE + 0x200))) |= (1ul << pin);
}

void pwmslow_set_io_pin_er8130(uint32_t idle_status, uint32_t low_active_en)
{
    set_pwmslow_mode_sel_bit(PWMSLOW_PARK_0_POS, idle_status);
    set_pwmslow_mode_sel_bit(PWMSLOW_INVERSE_0_POS, low_active_en);
}

void pwmslow_cfg_output_period_er8130(uint32_t period, uint32_t active_period,
                                      uint32_t center_align_en,
                                      uint32_t oneshot_en, uint32_t oneshot_num)
{
    if (period < 2) {
        period = 2;
    }
    if (active_period > period) {
        active_period = period;
    }

    set_pwmslow_mode_sel_bit(PWMSLOW_PWM_MODE_0_POS, center_align_en);
    set_pwmslow_mode_sel_bit(PWMSLOW_ONESHOT_0_POS, oneshot_en);
    configure_pwm_timing(period, active_period, oneshot_num);
}

void pwmslow_cfg_output_frequency_er8130(uint32_t frequency,
                                         uint32_t duty_percent,
                                         uint32_t center_align_en,
                                         uint32_t oneshot_en,
                                         uint32_t oneshot_num)
{
    uint32_t period, active_period;

    period = __LXT / frequency;

    if (period < 2) {
        period = 2;
    }

    if (duty_percent > 100) {
        duty_percent = 100;
    }

    active_period = (period * duty_percent) / 100;

    set_pwmslow_mode_sel_bit(PWMSLOW_PWM_MODE_0_POS, center_align_en);
    set_pwmslow_mode_sel_bit(PWMSLOW_ONESHOT_0_POS, oneshot_en);
    configure_pwm_timing(period, active_period, oneshot_num);
}

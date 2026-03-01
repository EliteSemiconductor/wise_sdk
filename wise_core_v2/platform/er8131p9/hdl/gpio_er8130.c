/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/gpio_er8130.h"

void gpio_set_mode_er8130(uint8_t pin_idx, uint8_t mode)
{
    uint32_t base_addr = (IOCTRL_GPIO_0_MODE_REG_ADDR + (pin_idx << 2));

    REG_W32(base_addr, mode);
}

void gpio_set_io_direction_er8130(uint8_t pin_idx, uint8_t in_out)
{
    uint32_t reg_val = 0;
    uint8_t in_en    = 0;
    uint8_t out_en   = 0;

    if (in_out == MODE_INPUT) {
        in_en  = 1;
        out_en = 0;
    } else if (in_out == MODE_OUTPUT) {
        in_en  = 1;
        out_en = 1;
    } else {
        // Handle unknown mode (invalid input)
    }

    reg_val = REG_R32(IOCTRL_GPIO_IE_REG_ADDR);
    reg_val = (reg_val & ~BIT(pin_idx)) | (in_en << pin_idx);
    REG_W32(IOCTRL_GPIO_IE_REG_ADDR, reg_val);

    reg_val = REG_R32(IOCTRL_GPIO_OE_REG_ADDR);
    reg_val = (reg_val & ~BIT(pin_idx)) | (out_en << pin_idx);
    REG_W32(IOCTRL_GPIO_OE_REG_ADDR, reg_val);
}

uint8_t gpio_get_io_direction_er8130(uint8_t pin_idx)
{
    uint32_t reg_val = REG_R32(IOCTRL_GPIO_OE_REG_ADDR);
    return (reg_val & (BIT(pin_idx)) ? MODE_OUTPUT : MODE_INPUT);
}

void gpio_set_debs_time_er8130(uint8_t pin_idx, uint8_t debs_time)
{
    uint32_t reg_val;

    reg_val = REG_R32(IOCTRL_GPIO_DEBS_REG_ADDR);
    reg_val = (reg_val | BIT(pin_idx));
    REG_W32(IOCTRL_GPIO_DEBS_REG_ADDR, reg_val);
    REG_W32(IOCTRL_GPIO_DEBS_CFG_REG_ADDR, debs_time);
}

void gpio_set_driv_str_er8130(uint8_t pin_idx, uint8_t driv_str)
{
    uint32_t reg_val  = REG_R32(IOCTRL_GPIO_DS_REG0_ADDR);
    reg_val          &= ~(0x3 << (pin_idx << 1));
    reg_val          |= (driv_str & 0x3) << (pin_idx << 1);
    REG_W32(IOCTRL_GPIO_DS_REG0_ADDR, reg_val);
}

void gpio_set_pull_sel_er8130(uint8_t pin_idx, uint8_t pull_sel)
{
    uint32_t reg_val  = REG_R32(IOCTRL_GPIO_PUSEL_REG0_ADDR);
    reg_val          &= ~(0x3 << (pin_idx << 1));
    reg_val          |= (pull_sel & 0x3) << (pin_idx << 1);
    REG_W32(IOCTRL_GPIO_PUSEL_REG0_ADDR, reg_val);
}

void gpio_set_intrpt_er8130(uint8_t pin_idx, uint8_t enable, uint8_t int_type)
{
    uint32_t reg_val  = 0;
    uint8_t trig_type = 0;
    uint8_t rise_hi   = 0;
    uint8_t fal_lo    = 0;

    if (enable == INT_INVALID) {
        enable = 0;
    }

    switch (int_type) {
    case INT_TYPE_LEVEL_LOW:
        trig_type = 0;
        rise_hi   = 0;
        fal_lo    = 1;
        break;
    case INT_TYPE_LEVEL_HIGH:
        trig_type = 0;
        rise_hi   = 1;
        fal_lo    = 0;
        break;
    case INT_TYPE_EDGE_FALING:
        trig_type = 1;
        rise_hi   = 0;
        fal_lo    = 1;
        break;
    case INT_TYPE_EDGE_RISING:
        trig_type = 1;
        rise_hi   = 1;
        fal_lo    = 0;
        break;
    case INT_TYPE_EDGE_BOTH:
        trig_type = 1;
        rise_hi   = 1;
        fal_lo    = 1;
        break;
    case INT_TYPE_INVALID:
        trig_type = 0;
        rise_hi   = 0;
        fal_lo    = 1;
        break;
    default:
        break;
    }

    // 0x0C0
    reg_val = REG_R32(IOCTRL_GPIO_INT_ENABLE_REG_ADDR);
    reg_val = (reg_val & ~BIT(pin_idx)) | (enable << pin_idx);
    REG_W32(IOCTRL_GPIO_INT_ENABLE_REG_ADDR, reg_val);

    // 0x084
    reg_val = REG_R32(IOCTRL_GPIO_TRIG_TYPE_REG_ADDR);
    reg_val = (reg_val & ~BIT(pin_idx)) | (trig_type << pin_idx);
    REG_W32(IOCTRL_GPIO_TRIG_TYPE_REG_ADDR, reg_val);

    // 0x088
    reg_val = REG_R32(IOCTRL_GPIO_RHIEN_REG_ADDR);
    reg_val = (reg_val & ~BIT(pin_idx)) | (rise_hi << pin_idx);
    REG_W32(IOCTRL_GPIO_RHIEN_REG_ADDR, reg_val);

    // 0x08C
    reg_val = REG_R32(IOCTRL_GPIO_FLIEN_REG_ADDR);
    reg_val = (reg_val & ~BIT(pin_idx)) | (fal_lo << pin_idx);
    REG_W32(IOCTRL_GPIO_FLIEN_REG_ADDR, reg_val);

    NVIC_EnableIRQ((IRQn_Type)GPIO_IRQn);
}

void gpio_set_schmitt_trig_en_er8130(uint8_t pin_idx, uint8_t trig_en)
{
    uint32_t reg_val;

    // 0x09C enable schmitt trig
    reg_val = REG_R32(IOCTRL_GPIO_SMT_REG_ADDR);
    reg_val = (reg_val & ~BIT(pin_idx)) | (trig_en << pin_idx);
    REG_W32(IOCTRL_GPIO_SMT_REG_ADDR, reg_val);
}

uint8_t gpio_read_pin_er8130(uint8_t pin_idx)
{
    return (!!(REG_R32(IOCTRL_GPIO_DI_REG_ADDR) & (1UL << pin_idx)));
}

void gpio_write_pin_er8130(uint8_t pin_idx, uint8_t val)
{
    uint32_t reg_val = REG_R32(IOCTRL_GPIO_DO_REG_ADDR);

    if (val) {
        reg_val |= (1UL << pin_idx);
    } else {
        reg_val &= ~(1UL << pin_idx);
    }
    REG_W32(IOCTRL_GPIO_DO_REG_ADDR, reg_val);
}

uint8_t gpio_get_int_status_er8130(uint8_t pin_idx)
{
    uint32_t reg_val;
    uint8_t ret;

    // 0x0C8
    reg_val = REG_R32(IOCTRL_GPIO_MASKED_INT_STATUS_REG_ADDR);
    reg_val = reg_val & BIT(pin_idx);

    if (reg_val) {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

void gpio_clear_int_status_er8130(uint8_t pin_idx)
{
    // 0x0C4 (RW1C)
    REG_W32(IOCTRL_GPIO_RAW_INT_STATUS_REG_ADDR, BIT(pin_idx));
}

uint32_t gpio_get_raw_int_status_er8130(void)
{
    return REG_R32(IOCTRL_GPIO_MASKED_INT_STATUS_REG_ADDR);
}

void gpio_clear_raw_int_status_er8130(uint32_t int_status)
{
    REG_W32(IOCTRL_GPIO_RAW_INT_STATUS_REG_ADDR, int_status);
}

void gpio_set_pwm_er8130(uint8_t pwm_num, uint8_t pin_idx, uint8_t enable)
{
    uint32_t reg_val = REG_R32((IOCTRL_PWM_0_SEL_REG_ADDR + (0x4 * pwm_num)));

    if (enable) {
        reg_val |= (1ul << pin_idx);
    } else {
        reg_val &= ~(1ul << pin_idx);
    }

    REG_W32((IOCTRL_PWM_0_SEL_REG_ADDR + (0x4 * pwm_num)), reg_val);
}

void gpio_set_pwmslow_er8130(uint8_t pin_idx, uint8_t enable)
{
    uint32_t reg_val = REG_R32(IOCTRL_PWM_SLOW_SEL_REG_ADDR);

    if (enable) {
        reg_val |= (1ul << pin_idx);
    } else {
        reg_val &= ~(1ul << pin_idx);
    }

    REG_W32((IOCTRL_PWM_SLOW_SEL_REG_ADDR), reg_val);
}

#define DEBUG_BUS_DEBUG_MODULE_REG_ADDR 0x400FE000
#define DEBUG_BUS_DEBUG_SINNAL_REG_ADDR 0x400FE004
void gpio_set_debug_bus_er8130(uint8_t gpio_pin_idx, uint8_t dbg_bus_module_idx, uint8_t dbg_bus_signal_idx, uint8_t dbg_id)
{
    uint32_t debug_sel_bass_addr = (IOCTRL_DIO_0_DBG_NUM_REG_ADDR + (gpio_pin_idx << 2));

    //debug bus: module select
    REG_W32(DEBUG_BUS_DEBUG_MODULE_REG_ADDR, dbg_bus_module_idx);

    switch (dbg_bus_module_idx){
    case 17:
        //debug bus: CMU
        REG_W32(DEBUG_BUS_DEBUG_SINNAL_REG_ADDR, dbg_bus_signal_idx);
        break;
    case 19:
        //debug bus: ANCTLs
        REG_W32(ANA_PMU_DBG_REG_ADDR, dbg_bus_signal_idx);
        break;
    default:
        break;
    }

    //set by gpio pin index
    REG_W32(debug_sel_bass_addr, dbg_id);
    printf("<<debug configuration>> \n gpio pin index : [%d]\n module_idx     : [%d]\n singnal_idx    : [%d]\n dbg_id         : [%d]\n", 
                                            gpio_pin_idx, dbg_bus_module_idx, dbg_bus_signal_idx, dbg_id);
}

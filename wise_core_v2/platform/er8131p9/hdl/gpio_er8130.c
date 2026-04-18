/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/gpio_er8130.h"
#include "util_debug_log.h"

#define DEBUG_BUS_DEBUG_MODULE_REG_ADDR 0x400FE000U
#define DEBUG_BUS_DEBUG_SIGNAL_REG_ADDR 0x400FE004U
#define GIO_CTRL_REG_ADDR               0x40014A80U

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
    return ((reg_val & BIT(pin_idx)) ? MODE_OUTPUT : MODE_INPUT);
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
        reg_val |= (1UL << pin_idx);
    } else {
        reg_val &= ~(1UL << pin_idx);
    }

    REG_W32((IOCTRL_PWM_0_SEL_REG_ADDR + (0x4 * pwm_num)), reg_val);
}

void gpio_set_pwmslow_er8130(uint8_t pin_idx, uint8_t enable)
{
    uint32_t reg_val = REG_R32(IOCTRL_PWM_SLOW_SEL_REG_ADDR);

    if (enable) {
        reg_val |= (1UL << pin_idx);
    } else {
        reg_val &= ~(1UL << pin_idx);
    }

    REG_W32(IOCTRL_PWM_SLOW_SEL_REG_ADDR, reg_val);
}

/*
 * Route an internal debug bus signal to a GPIO pin for probing.
 *
 * Parameters:
 *   pin_idx       - GPIO pin to output the debug signal (0-15)
 *   target_idx    - Debug module to observe (DEBUG_TARGET_PMU/MAC/CMU/ANCTL)
 *   dbg_signal_idx- Signal group index within the target module
 *   dbg_bus_idx   - Which bit of the debug bus to route to the GPIO pin
 *
 * Shell command: gpio dbg <pin_idx> <target_idx> <signal_idx> <bus_idx>
 *
 * -----------------------------------------------------------------------
 * [1] Check sleep-to-active transition  (target = DEBUG_TARGET_PMU = 3)
 * -----------------------------------------------------------------------
 *   Shell: gpio dbg <pin> 3 0 <bus_idx>
 *
 *   Debug bus signal map (dbg_signal_idx = 0):
 *     bus[7:5]  0x0 : active
 *               0x3 : sleep
 *     bus[1]    0   : MCU active
 *               1   : MCU deep sleep
 *
 *   Wake-up sequence to verify:
 *     bus[7:5]=3 (sleep) --> bus[7:5]=0 (active) --> bus[1]=0 (MCU active)
 *
 *   Example (observe bit 7 on GPIO 5, bit 1 on GPIO 6):
 *     gpio dbg 5 3 0 7
 *     gpio dbg 6 3 0 1
 *
 * -----------------------------------------------------------------------
 * [2] Check RX ready  (target = DEBUG_TARGET_MAC = 4)
 *     NOTE: cannot be used simultaneously with PMU debug target
 * -----------------------------------------------------------------------
 *   Shell: gpio dbg <pin> 4 0 <bus_idx>
 *
 *   Debug bus signal map (dbg_signal_idx = 0):
 *     bus[11]   RX enable for analog
 *     bus[10]   RX enable for BBP
 *
 *   RX settling sequence to verify:
 *     bus[11]=1 (analog RX on) --> bus[10]=1 (BBP RX on)
 *
 *   Example (observe bit 11 on GPIO 5, bit 10 on GPIO 6):
 *     gpio dbg 5 4 0 11
 *     gpio dbg 6 4 0 10
 */
void gpio_set_debug_bus_er8130(uint8_t pin_idx, uint8_t target_idx, uint8_t dbg_signal_idx, uint8_t dbg_bus_idx)
{
    uint32_t dbg_output_gpio_pin_base_addr = (IOCTRL_DIO_0_DBG_NUM_REG_ADDR + (pin_idx << 2));

    switch (target_idx) {
    case DEBUG_TARGET_PMU:
    case DEBUG_TARGET_MAC:
    case DEBUG_TARGET_CMU:
        REG_W32(DEBUG_BUS_DEBUG_MODULE_REG_ADDR, target_idx);
        REG_W32(DEBUG_BUS_DEBUG_SIGNAL_REG_ADDR, dbg_signal_idx);
        break;
    case DEBUG_TARGET_ANCTL:
        REG_W32(DEBUG_BUS_DEBUG_MODULE_REG_ADDR, target_idx);
        /* ANCTL signals are routed through ANA_PMU_DBG register, not the common signal bus */
        REG_W32(ANA_PMU_DBG_REG_ADDR, dbg_signal_idx);
        break;
    default:
        printf("debug target did not support: %d\n", target_idx);
        return;
    }

    REG_W32(dbg_output_gpio_pin_base_addr, dbg_bus_idx);
}

/*
 * GIO_CTRL_REG (0x40014A80) field layout:
 *   GPIO 2 : bits [4:0]
 *   GPIO 3 : bits [12:8]
 *   GPIO 4 : bits [20:16]
 *
 * gio_fun values (GPIO_GIO_FUN):
 *   GIO_FUN_TX_RX_EN      (0) : TX_EN / RX_EN indicator
 *   GIO_FUN_TX_DATA_START (2) : TX start to send data
 *   GIO_FUN_RX_SYNC_WORD  (6) : RX receive sync word
 */
void gpio_set_gio_fun_er8130(uint8_t pin_idx, uint8_t gio_fun)
{
    uint8_t  bit_offset = 0;
    uint32_t reg_val;

    switch (pin_idx) {
    case 2:
        bit_offset = 0;
        break;
    case 3:
        bit_offset = 8;
        break;
    case 4:
        bit_offset = 16;
        break;
    default:
        printf("gpio_pin_idx is invalid = %d\n", pin_idx);
        return;
    }

    switch (gio_fun) {
    case GIO_FUN_TX_RX_EN:
    case GIO_FUN_TX_DATA_START:
    case GIO_FUN_RX_SYNC_WORD:
        break;
    default:
        printf("gio_fun is invalid = %d, valid values: %d(TX_RX_EN), %d(TX_DATA_START), %d(RX_SYNC_WORD)\n",
               gio_fun, GIO_FUN_TX_RX_EN, GIO_FUN_TX_DATA_START, GIO_FUN_RX_SYNC_WORD);
        return;
    }

    reg_val  = REG_R32(GIO_CTRL_REG_ADDR);
    reg_val &= ~(0xFFU << bit_offset);
    reg_val |= ((uint32_t)gio_fun << bit_offset);
    REG_W32(GIO_CTRL_REG_ADDR, reg_val);
}

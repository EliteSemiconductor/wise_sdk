/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/sys_er8130.h"

static uint8_t board_pa_type = 1;
static uint8_t board_matching_type = 0;
static uint8_t board_40m_gain_ctrl = 1;
static uint8_t board_cap_xtal_i = 64;
static uint8_t board_cap_xtal_o = 64;


uint32_t sys_get_chip_id_er8130(void)
{
    return REG_R32(SYS_PRODUCT_ID_REG_ADDR);
}

void sys_set_remap_er8130(uint32_t remap_addr)
{
    REG_W32(SOC_SYS_LOCK_ADDR, SYS_UNLOCK_VALUE);
    REG_W32(SYS_REMAP_ADR_REG_ADDR, remap_addr);
    REG_W32(SYS_REMAP_TRG_REG_ADDR, 0x1);
}

void sys_lock_er8130(void)
{
    REG_W32(SOC_SYS_LOCK_ADDR, 0x0);
}

void sys_unlock_er8130(void)
{
    REG_W32(SOC_SYS_LOCK_ADDR, 0x1ACCE551);
}

/*
void sys_gpio_cfg_er8130(bool swd_enable, bool swd_pullup, bool tcxo_enable)
{
    REG_W32(SOC_SYS_LOCK_ADDR, SYS_UNLOCK_VALUE);

    uint32_t gpio_cfg = 0;
    if (swd_enable) {
        gpio_cfg |= SYS_SWD_GPIO_EN_MASK;
    }
    if (swd_pullup) {
        gpio_cfg |= SYS_SWD_GPIO_PU_MASK;
    }
    if (tcxo_enable) {
        gpio_cfg |= SYS_TCXO_GPIO_EN_MASK;
    }

    REG_W32(SYS_GPIO_REG_ADDR, gpio_cfg);

    REG_W32(SOC_SYS_LOCK_ADDR, 0x0);
}
*/

void sys_cfg_swd_er8130(bool swd_enable)
{
    REG_W32(SOC_SYS_LOCK_ADDR, SYS_UNLOCK_VALUE);

    uint32_t gpio_cfg = REG_R32(SYS_GPIO_REG_ADDR);
    
    if (swd_enable)
        gpio_cfg |= (SYS_SWD_GPIO_EN_MASK | SYS_SWD_GPIO_PU_MASK);
    else
        gpio_cfg &= ~(SYS_SWD_GPIO_EN_MASK | SYS_SWD_GPIO_PU_MASK);

    REG_W32(SYS_GPIO_REG_ADDR, gpio_cfg);

    REG_W32(SOC_SYS_LOCK_ADDR, 0x0);
}

void sys_cfg_tcxo_er8130(bool tcxo_enable)
{
    REG_W32(SOC_SYS_LOCK_ADDR, SYS_UNLOCK_VALUE);

    uint32_t gpio_cfg = REG_R32(SYS_GPIO_REG_ADDR);

    if (tcxo_enable) 
        gpio_cfg |= SYS_TCXO_GPIO_EN_MASK;
    else
        gpio_cfg &= ~SYS_TCXO_GPIO_EN_MASK;

    REG_W32(SYS_GPIO_REG_ADDR, gpio_cfg);

    REG_W32(SOC_SYS_LOCK_ADDR, 0x0);
}

uint8_t sys_get_pa_type_er8130(void)
{
    return board_pa_type;
}

void sys_set_pa_type_er8130(uint8_t pa_type)
{
    board_pa_type = pa_type;
}

uint8_t sys_get_board_match_type_er8130(void)
{
    return board_matching_type;
}

void sys_set_board_match_type_er8130(uint8_t mat_type)
{
    board_matching_type = mat_type;
}

void sys_set_board_gain_ctrl_er8130(uint8_t gain_ctrl)
{
    board_40m_gain_ctrl = gain_ctrl;
}

uint8_t sys_get_board_gain_ctrl_er8130()
{
    if (board_40m_gain_ctrl == 0)
        board_40m_gain_ctrl = 1;

    return board_40m_gain_ctrl;
}

uint32_t sys_get_xtal_cfg_er8130(void)
{
    return ((board_cap_xtal_o<<16)|board_cap_xtal_i);
}

void sys_set_xtal_cfg_er8130(uint8_t xtal_i, uint8_t xtal_o)
{
    board_cap_xtal_i = xtal_i;
    board_cap_xtal_o = xtal_o;
}


void sys_set_delay_er8130(uint32_t us)
{
    /* It should <= 349525us for each delay loop */
    uint32_t delay = 349525UL;

    do {
        if (us > delay) {
            us -= delay;
        } else {
            delay = us;
            us    = 0UL;
        }

        SysTick->LOAD = delay * CyclesPerUs;
        SysTick->VAL  = (0x0UL);
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

        /* Waiting for down-count to zero */
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0UL)
            ;

        /* Disable SysTick counter */
        SysTick->CTRL = 0UL;

    } while (us > 0UL);
}

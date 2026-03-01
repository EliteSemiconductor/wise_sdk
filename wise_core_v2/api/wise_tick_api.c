/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_tick_api.h"
#include "api/wise_gptmr_api.h"

int32_t wise_tick_init()
{
    hal_intf_sys_tick_init();

    return WISE_SUCCESS;
}

uint32_t wise_tick_get_counter()
{
    return hal_intf_sys_tick_get_counter();
}

void wise_tick_delay_ms(uint32_t ms)
{
    uint32_t startClk = hal_intf_sys_tick_get_counter();
    
    while ((hal_intf_sys_tick_get_counter() - startClk) <= MS_TO_CLK(ms)) 
    {
        __asm volatile("nop");
    }
}

void wise_tick_delay_us(uint32_t us)
{
    uint32_t startClk = hal_intf_sys_tick_get_counter();

    while ((hal_intf_sys_tick_get_counter() - startClk) <= US_TO_CLK(us)) 
    {
        __asm volatile("nop");
    }
}


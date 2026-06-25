/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "wise_tick_api.h"
#include "wise_gptmr_api.h"
#include "wise_wutmr_api.h"

static uint8_t _sysTickInited = 0;
static uint32_t _sysTickBackupCounter = 0;
static uint32_t _backupStartWUTMRTick = 0;

void _systick_backup();
void _systick_restore();

int32_t wise_tick_init()
{
    if(_sysTickInited)
        return WISE_SUCCESS;
    
    hal_intf_sys_tick_init();
    _sysTickInited = 1;
    
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


void _systick_backup()
{
    if(_sysTickInited)
    {
        _backupStartWUTMRTick = hal_intf_wutmr_get_counter();
        _sysTickBackupCounter = hal_intf_sys_tick_get_counter();
    }
}

void _systick_restore()
{
    if(_sysTickInited)
    {
        uint32_t cntrVal = 0;
        
        _backupStartWUTMRTick = (uint32_t)(hal_intf_wutmr_get_counter() - _backupStartWUTMRTick);

        cntrVal = _backupStartWUTMRTick * (_TICK_SRC_HZ / wise_wutmr_get_clk_rate());
        hal_intf_sys_tick_restore(cntrVal + _sysTickBackupCounter);
    }
}


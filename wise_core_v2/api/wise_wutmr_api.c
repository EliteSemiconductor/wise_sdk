/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_wutmr_api.h"
#include "hal_intf_sys.h"
#include "wise_sys_api.h"
#include "wise_tick_api.h"

static uint32_t _wutmr_clk_rate();
static uint32_t wutmrCalClkRate = 0;

void wise_wutmr_init(void)
{
/*
#ifdef CHIP_HAS_LFOSC
    const HAL_INTERNAL_SCLK_CFG_T *cfg_to_use = &sclk_src_16k_run_default;

    if (hal_intf_sys_exec_internal_sclk_calibration(cfg_to_use) == WISE_FAIL) {
        printf("internal sclk calibration fail!\n");
        return;
    }
#endif
    hal_intf_module_clk_enable(WUTMR_MODULE);
    hal_intf_wutmr_set_interrupt_enable();
*/
}

void wise_wutmr_deinit(void)
{
    //return hal_intf_module_clk_disable(WUTMR_MODULE);
}

void wise_wutmr_enable()
{
    if(wutmrCalClkRate == 0)
        wutmrCalClkRate = _wutmr_clk_rate();
    hal_intf_module_clk_enable(WUTMR_MODULE);
}

void wise_wutmr_disable()
{
    hal_intf_module_clk_disable(WUTMR_MODULE);
}

void wise_wutmr_enable_int()
{
    hal_intf_wutmr_set_interrupt_enable();
}

void wise_wutmr_disable_int()
{
    hal_intf_wutmr_set_interrupt_disable();
}

void wise_wutmr_clk_calibrate(uint32_t intervalMs)
{
    uint32_t calTargetStart = hal_intf_wutmr_get_counter();
    uint32_t calSourceStart = hal_intf_sys_tick_get_counter();
    int progressCount = 1;
    
    if(intervalMs == 0)
        intervalMs = 1000;

    debug_print("WUTMR SW clk calibration");
    while(1)
    {
        uint32_t timelapsed = hal_intf_sys_tick_get_counter() - calSourceStart;

        if(timelapsed > MS_TO_CLK(intervalMs))
        {
            wutmrCalClkRate = hal_intf_wutmr_get_counter() - calTargetStart;
            debug_print("finish\n");
            break;
        }

        if(timelapsed >= progressCount * (MS_TO_CLK(200)))
        {
            progressCount++;
            debug_print(".");
        }
        
        __asm("nop");
    }

    wutmrCalClkRate = wutmrCalClkRate * 1000 / intervalMs;
    hal_intf_wutmr_set_clock_base(wutmrCalClkRate);
    
    debug_print("WUTMR cal: %lu\n", wutmrCalClkRate);
}


uint32_t wise_wutmr_get_counter()
{
    return hal_intf_wutmr_get_counter();
}

WISE_STATUS wise_wutmr_start(uint32_t ms, uint8_t exec_type)
{
    WISE_LFOSC_SRC_T lfoscCfg = {0};

    wise_sys_lfosc_clk_get_config(&lfoscCfg);

    // if((lfoscCfg.clk_src != SYS_LFOSC_CLK_SRC_INTERNAL_32K) && (lfoscCfg.clk_src != SYS_LFOSC_CLK_SRC_INTERNAL_16K))
    // {
    //     debug_print("LFOSC is not enabled yet.\n");
    //     return WISE_FAIL;
    // }

    hal_intf_wutmr_clear_int_status();
    
    hal_intf_wutmr_set_time(ms);
    hal_intf_wutmr_set_enable(exec_type);
        
    return WISE_SUCCESS;
}

WISE_STATUS wise_wutmr_start_tick(uint32_t tickCounter, uint8_t exec_type)
{
    WISE_LFOSC_SRC_T lfoscCfg = {0};

    wise_sys_lfosc_clk_get_config(&lfoscCfg);

    // if((lfoscCfg.clk_src != SYS_LFOSC_CLK_SRC_INTERNAL_32K) && (lfoscCfg.clk_src != SYS_LFOSC_CLK_SRC_INTERNAL_16K))
    // {
    //     debug_print("LFOSC is not enabled yet.\n");
    //     return WISE_FAIL;
    // }

    hal_intf_wutmr_clear_int_status();

    if(tickCounter <= 3)
        tickCounter = 3; 
    
    hal_intf_wutmr_set_time_tick(tickCounter);
    hal_intf_wutmr_set_enable(exec_type);
        
    return WISE_SUCCESS;
}


void wise_wutmr_stop()
{
    hal_intf_wutmr_set_disable();
}

WISE_STATUS wise_wutmr_register_callback(WISE_WUTMR_EVT_CALLBACK_T callback, void *context)
{
    return hal_intf_wutmr_register_callback(callback, context);
}

static uint32_t _wutmr_clk_rate()
{
    WISE_LFOSC_SRC_T lfoscCfg = {0};
    uint32_t clkRate = 16384;
    
    wise_sys_lfosc_clk_get_config(&lfoscCfg);

    if(lfoscCfg.clk_src == SYS_LFOSC_CLK_SRC_INTERNAL_16K)
        clkRate = 16384;
    else if(lfoscCfg.clk_src == SYS_LFOSC_CLK_SRC_INTERNAL_32K)
    {
        switch(lfoscCfg.mode.mode_32k)
        {
            case LFOSC_32K_MODE_32K:
                clkRate = 32768;
            break;

            case LFOSC_32K_MODE_16K:
                clkRate = 16384;
            break;

            case LFOSC_32K_MODE_8K:
                clkRate = 8192;
            break;
        }
    }

    return clkRate;
}

uint32_t wise_wutmr_get_clk_rate()
{
    return wutmrCalClkRate;
}

uint32_t wise_wutmr_clk_per_ms()
{    
    return (wutmrCalClkRate / 1000);
}

uint32_t wise_wutmr_ms_to_clk(uint32_t ms)
{
    return (uint32_t)(((uint64_t)ms * wutmrCalClkRate) / 1000);
}

uint32_t wise_wutmr_clk_to_ms(uint32_t clk)
{
    return (uint32_t)((uint64_t)clk * 1000 / wutmrCalClkRate);
}


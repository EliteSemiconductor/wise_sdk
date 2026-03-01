/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_wutmr.h"
#include "hdl/wutmr_er8130.h"
#include "hal_intf_wutmr.h"

#define MS_TO_WUTMR_CNT(ms)                             ((ms * wutmrClockRate) / 1000)

//static CALLBACK_ENTRY_T wutmr_callbacks[WUTMR_MAX_EVENTS];
static uint32_t wutmrClockRate = 32768; //default 32K
static WUTMR_EVT_CALLBACK_T wutmrCallback = NULL;
static void* callbackContext = NULL;

void hal_drv_wutmr_set_clock_base(uint32_t base_clock)
{
    if(base_clock > 0)
        wutmrClockRate = base_clock;
}

void hal_drv_wutmr_set_time(uint32_t ms)
{
    //wutmr_set_cnt_er8130(MS_TO_WUTMR(ms));
    wutmr_set_cnt_er8130(MS_TO_WUTMR_CNT(ms));
}

void hal_drv_wutmr_set_time_tick(uint32_t tick)
{
    wutmr_set_cnt_er8130(tick);
}

void hal_drv_wutmr_set_enable(uint8_t period_en)
{
    wutmr_set_enable_er8130(period_en);
}

void hal_drv_wutmr_set_disable(void)
{
    wutmr_set_disable_er8130();
}

uint32_t hal_drv_wutmr_get_int_status(void)
{
    return wutmr_get_int_status_er8130();
}

void hal_drv_wutmr_clear_int_status(void)
{
    return wutmr_clear_int_status_er8130();
}

void hal_drv_wutmr_set_interrupt_enable(void)
{
    wutmr_set_int_enable_er8130();
}

uint32_t hal_drv_wutmr_get_counter(void)
{
    return wutmr_get_cnt_er8130();
}

void hal_drv_wutmr_set_interrupt_disable(void)
{
    wutmr_set_int_disable_er8130();
}

HAL_STATUS hal_drv_wutmr_register_callback(void (*callback)(void* context), void *context)
{
    wutmrCallback = callback;
    callbackContext = context;
    
    return HAL_ERR;
}

HAL_STATUS hal_drv_wutmr_unregister_callback()
{
    wutmrCallback = NULL;
    callbackContext = NULL;
    
    return HAL_ERR;
}

/*
static void hal_drv_wutmr_trigger_callback(WUTMR_CB_EVENT_T event, uint8_t wutmr_idx)
{
    if (wutmr_callbacks[event].callback) {
        wutmr_callbacks[event].callback(wutmr_callbacks[event].context, wutmr_idx);
    }
}
*/

WEAK_ISR void WUTMR_IRQHandler(void)
{
    if (hal_drv_wutmr_get_int_status()) 
    {
        hal_drv_wutmr_clear_int_status();
        //hal_drv_wutmr_trigger_callback(WUTMR_EVENT_EXPIRED, 0);

        if(wutmrCallback)
        {
            (wutmrCallback)(callbackContext);
        }
    }
}

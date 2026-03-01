/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_wdt.h"
#include "hdl/wdt_er8130.h"
#include "hal_intf_wdt.h"

static CALLBACK_ENTRY_T wdt_callbacks[WDT_MAX_EVENTS];

HAL_STATUS hal_drv_wdt_register_callback(WDT_CB_EVENT_T event, CALLBACK_T cb,
                                          void *context)
{
    if (event < WDT_MAX_EVENTS) {
        wdt_callbacks[event].callback = cb;
        wdt_callbacks[event].context  = context;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_wdt_unregister_callback(WDT_CB_EVENT_T event)
{
    if (event < WDT_MAX_EVENTS) {
        wdt_callbacks[event].callback = NULL;
        wdt_callbacks[event].context  = NULL;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

static void hal_drv_wdt_trigger_callback(WDT_CB_EVENT_T event, uint8_t wdt_idx)
{
    if (wdt_callbacks[event].callback) {
        wdt_callbacks[event].callback(wdt_callbacks[event].context, wdt_idx);
    }
}

WEAK_ISR void WDT_IRQHandler(void)
{
    hal_drv_wdt_trigger_callback(WDT_EVENT_TIMEOUT, 0);
}

void hal_drv_wdt_config(uint32_t period, uint32_t reset_en)
{
    wdt_config_er8130(period, reset_en);
}

void hal_drv_wdt_start(void)
{
    wdt_start_er8130();
}

void hal_drv_wdt_stop(void)
{
    wdt_stop_er8130();
}

uint32_t hal_drv_wdt_get_period(void)
{
    return wdt_get_period_er8130();
}

void hal_drv_wdt_set_period(uint32_t period)
{
    wdt_set_period_er8130(period);
}

void hal_drv_wdt_clear_int_flag(void)
{
    wdt_clear_int_flag_er8130();
}

uint32_t hal_drv_wdt_get_status(void)
{
    return wdt_get_status_er8130();
}

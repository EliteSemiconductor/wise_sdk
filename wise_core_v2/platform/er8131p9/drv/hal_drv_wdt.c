/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_wdt.h"
#include "hdl/wdt_er8130.h"
#include "hal_intf_wdt.h"

typedef void (*wdt_dispatch_fn_t)(void);

static wdt_dispatch_fn_t s_wdt_dispatch;
static CALLBACK_ENTRY_T wdt_callbacks[WDT_MAX_EVENTS];

static void wdt_isr_body(void)
{
    if (wdt_callbacks[WDT_EVENT_TIMEOUT].callback) {
        wdt_callbacks[WDT_EVENT_TIMEOUT].callback(
            wdt_callbacks[WDT_EVENT_TIMEOUT].context, 0);
    }
}

static void hal_drv_wdt_init_dispatch(void)
{
    s_wdt_dispatch = wdt_isr_body;
}

HAL_STATUS hal_drv_wdt_register_callback(WDT_CB_EVENT_T event, CALLBACK_T cb,
                                          void *context)
{
    if (event >= WDT_MAX_EVENTS) {
        return HAL_ERR;
    }
    hal_drv_wdt_init_dispatch();
    wdt_callbacks[event].callback = cb;
    wdt_callbacks[event].context  = context;
    return HAL_NO_ERR;
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

WEAK_ISR void WDT_IRQHandler(void)
{
    wdt_dispatch_fn_t fn = s_wdt_dispatch;
    if (fn) {
        fn();
    }
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

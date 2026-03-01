/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_wdt.h"
#include "drv/hal_drv_wdt.h"

HAL_STATUS hal_intf_wdt_register_callback(WDT_CB_EVENT_T event, CALLBACK_T cb,
                                           void *context)
{
    return hal_drv_wdt_register_callback(event, cb, context);
}

HAL_STATUS hal_intf_wdt_unregister_callback(WDT_CB_EVENT_T event)
{
    return hal_drv_wdt_unregister_callback(event);
}

void hal_intf_wdt_config(uint32_t reset_en, uint32_t timeout_ms)
{
    uint64_t period  = (uint64_t)timeout_ms * (uint64_t)SystemCoreClock;
    period          /= 1000ULL;

    if (period > 0xFFFFFFFFULL) {
        period = 0xFFFFFFFFULL;
    }

    hal_drv_wdt_config(period, reset_en);
}

void hal_intf_wdt_set_timeout(uint32_t timeout_ms)
{
    uint64_t period  = (uint64_t)timeout_ms * (uint64_t)SystemCoreClock;
    period          /= 1000ULL;

    if (period > 0xFFFFFFFFULL) {
        period = 0xFFFFFFFFULL;
    }

    hal_drv_wdt_set_period(period);
}

void hal_intf_wdt_start(void)
{
    hal_drv_wdt_start();
}

void hal_intf_wdt_stop(void)
{
    hal_drv_wdt_stop();
}

void hal_intf_wdt_refresh(void)
{
    hal_drv_wdt_clear_int_flag();
}

uint32_t hal_intf_wdt_get_period(void)
{
    return hal_drv_wdt_get_period();
}

uint32_t hal_intf_wdt_get_status(void)
{
    return hal_drv_wdt_get_status();
}

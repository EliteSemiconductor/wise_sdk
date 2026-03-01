/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_wdt_api.h"

WISE_STATUS wise_wdt_init(void)
{
    return WISE_SUCCESS;
}

WISE_STATUS wise_wdt_deinit(void)
{
    return hal_intf_module_clk_disable(WDT_MODULE);
}

void wise_wdt_start(void)
{
    hal_intf_wdt_start();
}

void wise_wdt_stop(void)
{
    hal_intf_wdt_stop();
}

void wise_wdt_config(uint8_t reset_en, uint32_t timeout_ms)
{
    hal_intf_module_clk_enable(WDT_MODULE);
    hal_intf_wdt_config(reset_en, timeout_ms);
}

void wise_wdt_set_timeout(uint32_t timeout_ms)
{
    hal_intf_wdt_set_timeout(timeout_ms);
}

void wise_wdt_set_refresh(void)
{
    hal_intf_wdt_refresh();
}

uint32_t wise_wdt_get_period(void)
{
    return hal_intf_wdt_get_period();
}

uint32_t wise_wdt_get_status(void)
{
    return hal_intf_wdt_get_status();
}

WISE_STATUS wise_wdt_register_callback(WISE_WDT_CB_EVENT_T event, CALLBACK_T cb,
                                       void *context)
{
    return hal_intf_wdt_register_callback(event, cb, context);
}

WISE_STATUS wise_wdt_unregister_callback(WISE_WDT_CB_EVENT_T event)
{
    return hal_intf_wdt_unregister_callback(event);
}

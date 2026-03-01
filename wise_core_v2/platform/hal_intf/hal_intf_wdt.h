/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_WDT_H
#define __HAL_INTF_WDT_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    WDT_EVENT_TIMEOUT,
    WDT_MAX_EVENTS,
} WDT_CB_EVENT_T;

HAL_STATUS hal_intf_wdt_register_callback(WDT_CB_EVENT_T event, CALLBACK_T cb,
                                           void *context);
HAL_STATUS hal_intf_wdt_unregister_callback(WDT_CB_EVENT_T event);
void hal_intf_wdt_config(uint32_t reset_en, uint32_t timeout_ms);
void hal_intf_wdt_set_timeout(uint32_t timeout_ms);
void hal_intf_wdt_start(void);
void hal_intf_wdt_stop(void);
void hal_intf_wdt_refresh(void);
uint32_t hal_intf_wdt_get_period(void);
uint32_t hal_intf_wdt_get_status(void);

#endif /* __HAL_INTF_WDT_H */

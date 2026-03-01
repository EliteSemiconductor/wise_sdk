/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_WDT_H
#define __HAL_DRV_WDT_H

#include "hal_intf_wdt.h"
#include <stdint.h>

void hal_drv_wdt_config(uint32_t period, uint32_t reset_en);
void hal_drv_wdt_start(void);
void hal_drv_wdt_stop(void);
uint32_t hal_drv_wdt_get_period(void);
void hal_drv_wdt_set_period(uint32_t period);
void hal_drv_wdt_clear_int_flag(void);
uint32_t hal_drv_wdt_get_status(void);
HAL_STATUS hal_drv_wdt_register_callback(WDT_CB_EVENT_T event, CALLBACK_T cb,
                                          void *context);
HAL_STATUS hal_drv_wdt_unregister_callback(WDT_CB_EVENT_T event);
#endif /* __HAL_DRV_WDT_H */

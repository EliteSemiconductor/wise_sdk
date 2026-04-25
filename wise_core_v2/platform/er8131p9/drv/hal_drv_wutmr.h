/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_WUTMR_H
#define __HAL_DRV_WUTMR_H

#include "hal_intf_wutmr.h"
#include <stdint.h>

void hal_drv_wutmr_set_clock_base(uint32_t base_clock);
uint32_t hal_drv_wutmr_get_clock_base();
void hal_drv_wutmr_set_time(uint32_t ms);
void hal_drv_wutmr_set_time_tick(uint32_t tick);
void hal_drv_wutmr_set_enable(uint8_t period_en);
void hal_drv_wutmr_set_disable(void);
uint32_t hal_drv_wutmr_get_counter(void);
uint32_t hal_drv_wutmr_get_int_status(void);
void hal_drv_wutmr_clear_int_status(void);
void hal_drv_wutmr_set_interrupt_enable(void);
void hal_drv_wutmr_set_interrupt_disable(void);
HAL_STATUS hal_drv_wutmr_register_callback(void (*callback)(void* context), void *context);
HAL_STATUS hal_drv_wutmr_unregister_callback();

#endif /* __HAL_DRV_WUTMR_H */

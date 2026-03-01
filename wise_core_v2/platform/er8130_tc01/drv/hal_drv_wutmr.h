/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_WUTMR_H
#define __HAL_DRV_WUTMR_H

#include "hal_intf_wutmr.h"
#include <stdint.h>

#define WUTMR_PER_MS                                    (32768 / 1000)
#define WUTMR_TO_MS(c)                                  ((c) / WUTMR_PER_MS)
#define MS_TO_WUTMR(m)                                  (m * 32768 / 1000)

void hal_drv_wutmr_set_clock_base(uint32_t base_clock);
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

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_WUTMR_H
#define __HAL_INTF_WUTMR_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    WUTMR_EVENT_EXPIRED,
    WUTMR_MAX_EVENTS,
} WUTMR_CB_EVENT_T;

typedef void (*WUTMR_EVT_CALLBACK_T)(void* context);

void hal_intf_wutmr_set_clock_base(uint32_t base_clock);
void hal_intf_wutmr_set_time(uint32_t ms);
void hal_intf_wutmr_set_time_tick(uint32_t tick);
void hal_intf_wutmr_set_enable(uint8_t exec_type);
void hal_intf_wutmr_set_disable();
uint32_t hal_intf_wutmr_get_counter();
void hal_intf_wutmr_set_interrupt_enable(void);
void hal_intf_wutmr_set_interrupt_disable(void);
void hal_intf_wutmr_clear_int_status(void);
HAL_STATUS hal_intf_wutmr_register_callback(void (*callback)(void* context), void *context);

#endif /* __HAL_INTF_WUTMR_H */

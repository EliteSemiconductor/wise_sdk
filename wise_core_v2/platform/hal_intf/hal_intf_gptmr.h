/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_GPTMR_H
#define __HAL_INTF_GPTMR_H

#include "esmt_chip_specific.h"
#include "types.h"

//#define CLK_PER_US (__SYSTEM_CLOCK / 1000000)
//#define CLK_PER_MS (__SYSTEM_CLOCK / 1000)

typedef enum {
    TIMER_PERIODIC = 0, /**< Periodic timer. */
    TIMER_ONE_SHOT,     /**< One-shot timer. */
} TIMER_TYPE_T;

typedef struct {
    TIMER_TYPE_T type;
    uint32_t interval;
    uint32_t start_offset;
    bool interrupt_enable;
} HAL_TIMER_CONF_T;

typedef enum {
    GPTMR_EVENT_T0_TIMEOUT,
    GPTMR_EVENT_T1_TIMEOUT,
    GPTMR_EVENT_T2_TIMEOUT,
    GPTMR_EVENT_T3_TIMEOUT,
    GPTMR_EVENT_T4_TIMEOUT,
    GPTMR_EVENT_T5_TIMEOUT,
    GPTMR_MAX_EVENTS,
} GPTMR_CB_EVENT_T;

typedef struct {
    uint8_t tmr_idx;
} GPTMR_CB_CONTEXT_T;

void hal_intf_gptmr_probe(void);
int32_t hal_intf_gptmr_open(uint8_t gptmr_idx, uint32_t mode);
void hal_intf_gptmr_start(uint8_t gptmr_idx);
void hal_intf_gptmr_stop(uint8_t gptmr_idx);
void hal_intf_gptmr_enable_int(uint8_t gptmr_idx);
void hal_intf_gptmr_disable_int(uint8_t gptmr_idx);
bool hal_intf_gptmr_is_enabled(uint8_t gptmr_idx);
void hal_intf_gptmr_clear_int_flag(uint8_t gptmr_idx);
void hal_intf_gptmr_set_cnt(uint8_t gptmr_idx, uint32_t cnt);
void hal_intf_gptmr_set_freq(uint8_t gptmr_idx, uint32_t freq);
void hal_intf_gptmr_reset_counter(uint8_t gptmr_idx);
uint32_t hal_intf_gptmr_get_cnt(uint8_t gptmr_idx);
uint32_t hal_intf_gptmr_get_cnt_max(uint8_t gptmr_idx);
void hal_intf_gptmr_config(uint8_t gptmr_idx, TIMER_TYPE_T type,
                          bool interrupt_enable, uint32_t start_offset,
                          uint32_t interval);
HAL_STATUS hal_intf_gptmr_register_callback(GPTMR_CB_EVENT_T event,
                                             CALLBACK_T cb, void *context);
HAL_STATUS hal_intf_gptmr_unregister_callback(GPTMR_CB_EVENT_T event);

#endif /* __HAL_INTF_GPTMR_H */

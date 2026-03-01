/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_GPTMR_H
#define __HAL_DRV_GPTMR_H

#include "hal_intf_gptmr.h"
#include <stdint.h>

typedef struct {
    TIMER_TYPE_T type;
    uint32_t start_offset;
    uint32_t interval;
    uint32_t interval_counter;
} GPTMR_CONFIG_CONTEXT_T;

#define GET_TIMER_IRQ(timer_channel)                                           \
    ((timer_channel) == 0   ? GPTMR0_IRQn                                      \
     : (timer_channel) == 1 ? GPTMR1_IRQn                                      \
     : (timer_channel) == 2 ? GPTMR2_IRQn                                      \
     : (timer_channel) == 3 ? GPTMR3_IRQn                                      \
     : (timer_channel) == 4 ? GPTMR4_IRQn                                      \
                            : GPTMR5_IRQn)

#define GPTMR_CLK_PER_US                                (__SYSTEM_CLOCK / 1000000)
#define GPTMR_US_TO_CLK(u)                              ((u) * HAL_CLK_PER_US)


HAL_STATUS hal_drv_gptmr_open(uint8_t gptmr_idx, uint32_t mode);
void hal_drv_gptmr_start(uint8_t gptmr_idx);
void hal_drv_gptmr_stop(uint8_t gptmr_idx);
void hal_drv_gptmr_enable_int(uint8_t gptmr_idx);
void hal_drv_gptmr_disable_int(uint8_t gptmr_idx);
bool hal_drv_gptmr_is_enabled(uint8_t gptmr_idx);
void hal_drv_gptmr_clear_int_flag(uint8_t gptmr_idx);
void hal_drv_gptmr_set_cnt(uint8_t gptmr_idx, uint32_t cnt);
void hal_drv_gptmr_set_freq(uint8_t gptmr_idx, uint32_t freq);
void hal_drv_gptmr_reset_counter(uint8_t gptmr_idx);
uint32_t hal_drv_gptmr_get_cnt(uint8_t gptmr_idx);
uint32_t hal_drv_gptmr_get_max(uint8_t gptmr_idx);
void hal_drv_gptmr_config(uint8_t gptmr_idx, TIMER_TYPE_T type,
                          bool interrupt_enable, uint32_t start_offset,
                          uint32_t interval);
HAL_STATUS hal_drv_gptmr_register_callback(GPTMR_CB_EVENT_T event,
                                            CALLBACK_T cb, void *context);
HAL_STATUS hal_drv_gptmr_unregister_callback(GPTMR_CB_EVENT_T event);
#endif /* __HAL_DRV_GPTMR_H */

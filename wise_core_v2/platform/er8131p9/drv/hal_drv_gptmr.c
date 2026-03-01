/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_gptmr.h"
#include "hdl/gptmr_er8130.h"
#include "hal_intf_gptmr.h"

static CALLBACK_ENTRY_T gptmr_callbacks[GPTMR_MAX_EVENTS];
static GPTMR_CONFIG_CONTEXT_T gptmr_context[CHIP_TIMER_CHANNEL_NUM];
static GPTMR_T *gptmr_channel[CHIP_TIMER_CHANNEL_NUM] = {GPTMR0, GPTMR1, GPTMR2, GPTMR3, GPTMR4, GPTMR5};


HAL_STATUS hal_drv_gptmr_open(uint8_t gptmr_idx, uint32_t mode)
{
    return gptmr_open_er8130(gptmr_channel[gptmr_idx], mode);
}

void hal_drv_gptmr_start(uint8_t gptmr_idx)
{
    gptmr_start_er8130(gptmr_channel[gptmr_idx]);
}

void hal_drv_gptmr_stop(uint8_t gptmr_idx)
{
    gptmr_stop_er8130(gptmr_channel[gptmr_idx]);
}

void hal_drv_gptmr_enable_int(uint8_t gptmr_idx)
{
    gptmr_enable_int_er8130(gptmr_channel[gptmr_idx]);
    NVIC_EnableIRQ((IRQn_Type)GET_TIMER_IRQ(gptmr_idx));
}

void hal_drv_gptmr_disable_int(uint8_t gptmr_idx)
{
    gptmr_disable_int_er8130(gptmr_channel[gptmr_idx]);
    NVIC_DisableIRQ((IRQn_Type)GET_TIMER_IRQ(gptmr_idx));
}

bool hal_drv_gptmr_is_enabled(uint8_t gptmr_idx)
{
    return gptmr_is_enabled_er8130(gptmr_channel[gptmr_idx]);
}

void hal_drv_gptmr_clear_int_flag(uint8_t gptmr_idx)
{
    gptmr_clear_int_flag_er8130(gptmr_channel[gptmr_idx]);
}

void hal_drv_gptmr_set_cnt(uint8_t gptmr_idx, uint32_t cnt)
{
    gptmr_set_cnt_er8130(gptmr_channel[gptmr_idx], cnt);
}

void hal_drv_gptmr_set_freq(uint8_t gptmr_idx, uint32_t freq)
{
    gptmr_set_freq_er8130(gptmr_channel[gptmr_idx], freq);
}

void hal_drv_gptmr_reset_counter(uint8_t gptmr_idx)
{
    gptmr_reset_counter_er8130(gptmr_channel[gptmr_idx]);
}

uint32_t hal_drv_gptmr_get_cnt(uint8_t gptmr_idx)
{
    return gptmr_get_cnt_er8130(gptmr_channel[gptmr_idx]);
}

uint32_t hal_drv_gptmr_get_max(uint8_t gptmr_idx)
{
    return gptmr_get_reload_er8130(gptmr_channel[gptmr_idx]);
}

void hal_drv_gptmr_config(uint8_t gptmr_idx, TIMER_TYPE_T type,
                          bool interrupt_enable, uint32_t start_offset,
                          uint32_t interval)
{
    uint32_t start_offset_counter = start_offset * GPTMR_CLK_PER_US;
    uint32_t interval_counter     = interval * GPTMR_CLK_PER_US;
    uint32_t counter              = (start_offset != 0) ? start_offset_counter
                                                        : interval_counter;

    hal_drv_gptmr_set_cnt(gptmr_idx, counter);
    hal_drv_gptmr_open(gptmr_idx, type);

    if (interrupt_enable) {
        hal_drv_gptmr_enable_int(gptmr_idx);
    } else {
        hal_drv_gptmr_disable_int(gptmr_idx);
    }
    // Save the configuration
    gptmr_context[gptmr_idx].type             = type;
    gptmr_context[gptmr_idx].start_offset     = start_offset;
    gptmr_context[gptmr_idx].interval         = interval;
    gptmr_context[gptmr_idx].interval_counter = interval_counter;
}

HAL_STATUS hal_drv_gptmr_register_callback(GPTMR_CB_EVENT_T event,
                                            CALLBACK_T cb, void *context)
{
    if (event < GPTMR_MAX_EVENTS) {
        gptmr_callbacks[event].callback = cb;
        gptmr_callbacks[event].context  = context;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_gptmr_unregister_callback(GPTMR_CB_EVENT_T event)
{
    if (event < GPTMR_MAX_EVENTS) {
        gptmr_callbacks[event].callback = NULL;
        gptmr_callbacks[event].context  = NULL;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

static void hal_drv_gptmr_trigger_callback(GPTMR_CB_EVENT_T event,
                                           uint8_t gptmr_idx)
{
    if (gptmr_callbacks[event].callback) {
        gptmr_callbacks[event].callback(gptmr_callbacks[event].context,
                                        gptmr_idx);
    }
}

void gptimer_IRQHandler(uint8_t gptmr_idx)
{
    hal_drv_gptmr_clear_int_flag(gptmr_idx);
    GPTMR_CONFIG_CONTEXT_T *ctx = &gptmr_context[gptmr_idx];
    // GPTMR_CB_EVENT_T event = GPTMR_EVENT_TIMEOUT;

    if (ctx->type == TIMER_ONE_SHOT) {
        hal_drv_gptmr_stop(gptmr_idx);
        hal_drv_gptmr_disable_int(gptmr_idx);
        ctx->start_offset = 0;
        ctx->interval     = 0;
    } else {
        if (ctx->start_offset != 0) {
            ctx->start_offset = 0;
            hal_drv_gptmr_set_cnt(gptmr_idx, (ctx->interval_counter));
        }
    }

    switch (gptmr_idx) {
    case 0:
        hal_drv_gptmr_trigger_callback(GPTMR_EVENT_T0_TIMEOUT, gptmr_idx);
        break;
    case 1:
        hal_drv_gptmr_trigger_callback(GPTMR_EVENT_T1_TIMEOUT, gptmr_idx);
        break;
    case 2:
        hal_drv_gptmr_trigger_callback(GPTMR_EVENT_T2_TIMEOUT, gptmr_idx);
        break;
    case 3:
        hal_drv_gptmr_trigger_callback(GPTMR_EVENT_T3_TIMEOUT, gptmr_idx);
        break;
    case 4:
        hal_drv_gptmr_trigger_callback(GPTMR_EVENT_T4_TIMEOUT, gptmr_idx);
        break;
    case 5:
        hal_drv_gptmr_trigger_callback(GPTMR_EVENT_T5_TIMEOUT, gptmr_idx);
        break;
    }
}

WEAK_ISR void GPTMR0_IRQHandler()
{
    gptimer_IRQHandler(0);
}

WEAK_ISR void GPTMR1_IRQHandler()
{
    gptimer_IRQHandler(1);
}

WEAK_ISR void GPTMR2_IRQHandler()
{
    gptimer_IRQHandler(2);
}

WEAK_ISR void GPTMR3_IRQHandler()
{
    gptimer_IRQHandler(3);
}

WEAK_ISR void GPTMR4_IRQHandler()
{
    gptimer_IRQHandler(4);
}

WEAK_ISR void GPTMR5_IRQHandler()
{
    gptimer_IRQHandler(5);
}

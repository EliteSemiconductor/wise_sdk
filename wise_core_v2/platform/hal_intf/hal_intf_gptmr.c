/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_gptmr.h"
#include "drv/hal_drv_gptmr.h"

HAL_STATUS hal_intf_gptmr_register_callback(GPTMR_CB_EVENT_T event,
                                             CALLBACK_T cb, void *context)
{
    return hal_drv_gptmr_register_callback(event, cb, context);
}

HAL_STATUS hal_intf_gptmr_unregister_callback(GPTMR_CB_EVENT_T event)
{
    return hal_drv_gptmr_unregister_callback(event);
}

int32_t hal_intf_gptmr_open(uint8_t gptmr_idx, uint32_t mode)
{
    return hal_drv_gptmr_open(gptmr_idx, mode);
}

void hal_intf_gptmr_start(uint8_t gptmr_idx)
{
    hal_drv_gptmr_start(gptmr_idx);
}

void hal_intf_gptmr_stop(uint8_t gptmr_idx)
{
    hal_drv_gptmr_stop(gptmr_idx);
}

void hal_intf_gptmr_enable_int(uint8_t gptmr_idx)
{
    hal_drv_gptmr_enable_int(gptmr_idx);
}

void hal_intf_gptmr_disable_int(uint8_t gptmr_idx)
{
    hal_drv_gptmr_disable_int(gptmr_idx);
}

bool hal_intf_gptmr_is_enabled(uint8_t gptmr_idx)
{
    return hal_drv_gptmr_is_enabled(gptmr_idx);
}

void hal_intf_gptmr_clear_int_flag(uint8_t gptmr_idx)
{
    hal_drv_gptmr_clear_int_flag(gptmr_idx);
}

void hal_intf_gptmr_set_cnt(uint8_t gptmr_idx, uint32_t cnt)
{
    hal_drv_gptmr_set_cnt(gptmr_idx, cnt);
}

void hal_intf_gptmr_set_freq(uint8_t gptmr_idx, uint32_t freq)
{
    hal_drv_gptmr_set_freq(gptmr_idx, freq);
}

void hal_intf_gptmr_reset_counter(uint8_t gptmr_idx)
{
    hal_drv_gptmr_reset_counter(gptmr_idx);
}

uint32_t hal_intf_gptmr_get_cnt(uint8_t gptmr_idx)
{
#ifdef CHIP_TIMER_COUNTER_DOWN
    uint32_t maxCount = hal_drv_gptmr_get_max(gptmr_idx);
    uint32_t counter = hal_drv_gptmr_get_cnt(gptmr_idx);
    
    return (maxCount - counter);
#elif defined CHIP_TIMER_COUNTER_UP
    return hal_drv_gptmr_get_cnt(gptmr_idx);
#else
#error "chip counter type is not defined!!"
#endif
}

uint32_t hal_intf_gptmr_get_cnt_max(uint8_t gptmr_idx)
{
    return hal_drv_gptmr_get_max(gptmr_idx);
}

//void hal_intf_gptmr_config(uint8_t gptmr_idx, HAL_TIMER_CONF_T *config)
void hal_intf_gptmr_config(uint8_t gptmr_idx, TIMER_TYPE_T type,
                          bool interrupt_enable, uint32_t start_offset,
                          uint32_t interval)
{
    return hal_drv_gptmr_config(gptmr_idx, type,
                                interrupt_enable, start_offset,
                                interval);
}

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_wutmr.h"
#include "hdl/wutmr_er8130.h"
#include "hal_intf_wutmr.h"

typedef void (*wutmr_dispatch_fn_t)(void);

#define MS_TO_WUTMR_CNT(ms)                             ((ms * wutmrClockRate) / 1000)

static wutmr_dispatch_fn_t s_wutmr_dispatch;
static uint32_t wutmrClockRate = 0; //default set to 0, LFOSC must be inited first
static WUTMR_EVT_CALLBACK_T wutmrCallback = NULL;
static void* callbackContext = NULL;

static void wutmr_isr_body(void)
{
    if (hal_drv_wutmr_get_int_status())
    {
        hal_drv_wutmr_clear_int_status();

        if (wutmrCallback)
        {
            (wutmrCallback)(callbackContext);
        }
    }
}

static void hal_drv_wutmr_init_dispatch(void)
{
    s_wutmr_dispatch = wutmr_isr_body;
}

void hal_drv_wutmr_set_clock_base(uint32_t base_clock)
{
    if(base_clock > 0) {
        wutmrClockRate = base_clock;
    }
}

uint32_t hal_drv_wutmr_get_clock_base()
{
    return wutmrClockRate;
}

void hal_drv_wutmr_set_time(uint32_t ms)
{
    //wutmr_set_cnt_er8130(MS_TO_WUTMR(ms));
    wutmr_set_cnt_er8130(MS_TO_WUTMR_CNT(ms));
}

void hal_drv_wutmr_set_time_tick(uint32_t tick)
{
    wutmr_set_cnt_er8130(tick);
}

void hal_drv_wutmr_set_enable(uint8_t period_en)
{
    wutmr_set_enable_er8130(period_en);
}

void hal_drv_wutmr_set_disable(void)
{
    wutmr_set_disable_er8130();
}

uint32_t hal_drv_wutmr_get_int_status(void)
{
    return wutmr_get_int_status_er8130();
}

void hal_drv_wutmr_clear_int_status(void)
{
    return wutmr_clear_int_status_er8130();
}

void hal_drv_wutmr_set_interrupt_enable(void)
{
    wutmr_set_int_enable_er8130();
}

uint32_t hal_drv_wutmr_get_counter(void)
{
    return wutmr_get_cnt_er8130();
}

void hal_drv_wutmr_set_interrupt_disable(void)
{
    wutmr_set_int_disable_er8130();
}

HAL_STATUS hal_drv_wutmr_register_callback(void (*callback)(void* context), void *context)
{
    hal_drv_wutmr_init_dispatch();
    wutmrCallback = callback;
    callbackContext = context;

    return HAL_ERR;
}

HAL_STATUS hal_drv_wutmr_unregister_callback()
{
    wutmrCallback = NULL;
    callbackContext = NULL;

    return HAL_ERR;
}

WEAK_ISR void WUTMR_IRQHandler(void)
{
    wutmr_dispatch_fn_t fn = s_wutmr_dispatch;
    if (fn) {
        fn();
    }
}

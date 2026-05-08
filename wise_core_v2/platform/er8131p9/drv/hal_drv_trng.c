/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_trng.h"
#include "hdl/trng_er8130.h"
#include "hal_intf_trng.h"

typedef void (*trng_dispatch_fn_t)(void);
static trng_dispatch_fn_t s_trng_dispatch;

static void trng_isr_body(void)
{
    uint32_t status = hal_drv_trng_get_status();
    hal_drv_trng_clear_status(status);
}

static void hal_drv_trng_init_dispatch(void)
{
    s_trng_dispatch = trng_isr_body;
}

void hal_drv_trng_sw_reset(void)
{
    trng_sw_rst_er8130();
}

uint8_t hal_drv_trng_is_busy(void)
{
    return trng_get_busy_er8130();
}

uint32_t hal_drv_trng_get_status(void)
{
    return trng_get_status_er8130();
}

void hal_drv_trng_clear_status(uint32_t status)
{
    trng_clear_status_er8130(status);
}

void hal_drv_trng_config(void)
{
    trng_config_er8130();
}

void hal_drv_trng_start(void)
{
    hal_drv_trng_init_dispatch();
    trng_start_er8130();
}

uint8_t hal_drv_trng_get_ehr_data(uint32_t *ehr_buf)
{
    return trng_get_ehr_data_er8130(ehr_buf);
}

WEAK_ISR void TRNG_IRQHandler(void)
{
    trng_dispatch_fn_t fn = s_trng_dispatch;
    if (fn) {
        fn();
    }
}

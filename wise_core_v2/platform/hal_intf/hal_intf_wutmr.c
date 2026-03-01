/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_wutmr.h"
#include "drv/hal_drv_wutmr.h"

void hal_intf_wutmr_set_clock_base(uint32_t base_clock)
{
#ifdef CHIP_HAS_LFOSC
    hal_drv_wutmr_set_clock_base(base_clock);
#endif
}

void hal_intf_wutmr_set_time(uint32_t ms)
{
#ifdef CHIP_HAS_LFOSC
    hal_drv_wutmr_set_time(ms);
#endif
}

void hal_intf_wutmr_set_time_tick(uint32_t tick)
{
#ifdef CHIP_HAS_LFOSC
    hal_drv_wutmr_set_time_tick(tick);
#endif
}

void hal_intf_wutmr_set_enable(uint8_t exec_type)
{
#ifdef CHIP_HAS_LFOSC
    hal_drv_wutmr_set_enable(exec_type);
#endif
}

void hal_intf_wutmr_set_disable()
{
#ifdef CHIP_HAS_LFOSC
    hal_drv_wutmr_set_disable();
#endif
}

uint32_t hal_intf_wutmr_get_counter()
{
#ifdef CHIP_HAS_LFOSC
    return hal_drv_wutmr_get_counter();
#else
    return 0;
#endif
}

void hal_intf_wutmr_set_interrupt_enable(void)
{
#ifdef CHIP_HAS_LFOSC
    hal_drv_wutmr_set_interrupt_enable();
#endif
}

void hal_intf_wutmr_set_interrupt_disable(void)
{
#ifdef CHIP_HAS_LFOSC
    hal_drv_wutmr_set_interrupt_disable();
#endif
}

HAL_STATUS hal_intf_wutmr_register_callback(void (*callback)(void* context), void *context)
{
#ifdef CHIP_HAS_LFOSC
    return hal_drv_wutmr_register_callback(callback, context);
#else
    return HAL_ERR;
#endif
}

void hal_intf_wutmr_clear_int_status(void)
{
#ifdef CHIP_HAS_LFOSC
    hal_drv_wutmr_clear_int_status(); 
    NVIC_ClearPendingIRQ(WUTMR_IRQn);
#endif

}


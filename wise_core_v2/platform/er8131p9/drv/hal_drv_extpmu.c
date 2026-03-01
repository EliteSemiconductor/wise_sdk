/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_extpmu.h"
#include "hdl/extpmu_er8130.h"
#include "hal_intf_extpmu.h"

#include "util.h"

static EXTPMU_EVT_CALLBACK_T extpmuCallback = NULL;
static void* callbackContext = NULL;

void hal_drv_extpmu_enable_external_pmu(uint8_t enable)
{
    extpmu_enable_external_pmu_er8130(enable);
}

void hal_drv_extpmu_set_shutdown(uint8_t wu_src, uint32_t wu_time, uint8_t gpio_pin)
{
    uint8_t wutmr_en = 0;

    extpmu_enable_wakeup_source_er8130(wu_src);

    //kevinyang, 20251222, bit mask
    //if (wu_src == EXTPMU_WU_SRC_WUTMR)
    if(wu_src & EXTPMU_PMU_WUBY_WUTMR_MASK)
    {
        extpmu_set_wakeup_time_er8130(MS_TO_TMR_PROD(wu_time));
        wutmr_en = 1;
    }

    if(wu_src & EXTPMU_PMU_WUBY_GPIO_MASK)
    {
        extpmu_set_wakeup_gpio_pin_er8130(gpio_pin);
    }

    extpmu_set_sleep_er8130(wutmr_en);
}

void hal_drv_extpmu_select_clk_source(uint8_t clk_src)
{
    extpmu_clk_source_sel_er8130(clk_src);
}

void hal_drv_extpmu_disable_shutdown_control_for_32k_16k_osc(void)
{
    extpmu_disable_extpmu_shutdown_control_for_32k_16k_osc();
}
uint32_t hal_drv_extpmu_get_wutmr_counter(void)
{
    return extpmu_get_wutmr_counter_er8130();
}

uint32_t hal_drv_extpmu_get_int_status(void)
{
    return extpmu_get_int_status_er8130();
}

void hal_drv_extpmu_clear_int_status(void)
{
    extpmu_clear_int_status_er8130();
}

void hal_drv_extpmu_set_interrupt_enable(void)
{
    extpmu_set_int_enable_er8130();
}

void hal_drv_extpmu_set_interrupt_disable(void)
{
    extpmu_set_int_disable_er8130();
}

void hal_drv_extpmu_set_wakeup_by_nfc(uint8_t enable)
{
    if (enable)
        extpmu_enable_wakeup_source_er8130(EXTPMU_PMU_WUBY_NFC_MASK);
    else
        extpmu_disable_wakeup_source_er8130(EXTPMU_PMU_WUBY_NFC_MASK);
}

HAL_STATUS hal_drv_extpmu_register_callback(void (*callback)(void* context), void *context)
{
    extpmuCallback = callback;
    callbackContext = context;
    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_extpmu_unregister_callback(void)
{
    extpmuCallback = NULL;
    callbackContext = NULL;
    return HAL_NO_ERR;
}

/**
 * @brief EXTPMU Interrupt Handler (External PMU Interrupt Handler)
 *
 * @note This interrupt handler is only triggered after the system resets from shutdown mode.
 *       NVIC must be manually configured to enable this ISR.
 *       Currently not used in the APP layer.
 *
 * @note Usage example:
 *       1. Set wakeup source (WUTMR or GPIO) before entering shutdown mode
 *       2. After system reset, enable EXTPMU interrupt during APP initialization:
 *          NVIC_EnableIRQ(EXTPMU_IRQn);
 *       3. Register callback function via hal_drv_extpmu_register_callback()
 */
WEAK_ISR void EXTPMU_IRQHandler(void)
{
    if (hal_drv_extpmu_get_int_status())
    {
        hal_drv_extpmu_clear_int_status();

        if (extpmuCallback)
        {
            extpmuCallback(callbackContext);
        }
    }
}
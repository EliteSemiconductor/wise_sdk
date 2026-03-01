/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_extpmu.h"
#ifdef CHIP_HAS_LFOSC
#include "wise_sys_api.h"
#include "drv/hal_drv_extpmu.h"

void hal_intf_extpmu_enable_external_pmu(uint8_t enable)
{
    hal_drv_extpmu_enable_external_pmu(enable);
}

void hal_intf_extpmu_set_shutdown(uint8_t wu_src_en, uint32_t wu_time, uint8_t wake_pin_idx)
{
    uint8_t halEnMask = 0;

    if(wu_src_en & SHUTDOWN_WAKE_SRC_WUTMR)
        halEnMask |= EXTPMU_PMU_WUBY_WUTMR_MASK;
    if(wu_src_en & SHUTDOWN_WAKE_SRC_NFC)
        halEnMask |= EXTPMU_PMU_WUBY_NFC_MASK;
    if(wu_src_en & SHUTDOWN_WAKE_SRC_GPIO)
        halEnMask |= EXTPMU_PMU_WUBY_GPIO_MASK;

#if (ESMT_SOC_CHIP_ID == 0x9005)
    hal_drv_extpmu_set_shutdown(halEnMask, wu_time);
#endif
#if (ESMT_SOC_CHIP_ID == 0x9006)
    hal_drv_extpmu_enable_external_pmu(ENABLE);
    hal_drv_extpmu_set_shutdown(halEnMask, wu_time, wake_pin_idx);
#endif
}

void hal_intf_extpmu_select_clk_source(uint8_t clk_src)
{
    hal_drv_extpmu_select_clk_source(clk_src);
}

void hal_intf_extpmu_disable_shutdown_control_for_32k_16k_osc(void)
{
    hal_drv_extpmu_disable_shutdown_control_for_32k_16k_osc();
}
#if (ESMT_SOC_CHIP_ID == 0x9006)
uint32_t hal_intf_extpmu_get_wutmr_counter(void)
{
    return hal_drv_extpmu_get_wutmr_counter();
}
#endif
#endif

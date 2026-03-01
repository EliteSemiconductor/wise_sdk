/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_extpmu.h"
#include "hdl/extpmu_er8130.h"
#include "hal_intf_extpmu.h"
#include "ana_er8130.h"  // used for workaround, will be removed at next version

void hal_drv_extpmu_enable_external_pmu(uint8_t enable)
{
    extpmu_set_enable_er8130(enable);
}

void hal_drv_extpmu_set_shutdown(uint8_t wu_src, uint32_t wu_time)
{
    uint8_t wutmr_en = 0;
    
    extpmu_set_wakeup_source_er8130(wu_src);

    if (wu_src == EXTPMU_WU_SRC_WUTMR) {
        extpmu_set_wakeup_time_er8130(MS_TO_TMR_PROD(wu_time));
        wutmr_en = 1;
    }

    // --- workaround for soc 9005 ---
    ANA_SET_OSC_BACKUP();
    // -------------------------------
    extpmu_set_enable_er8130(1);
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
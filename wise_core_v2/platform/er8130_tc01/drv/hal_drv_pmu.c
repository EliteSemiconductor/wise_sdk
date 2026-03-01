/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_pmu.h"
#include "hdl/pmu_er8130.h"
#include "hal_intf_pmu.h"

void hal_drv_pmu_set_pwr_mode(uint8_t mode)
{
    pmu_set_pwr_mode_er8130(mode);
}

void hal_drv_pmu_set_sw_reset(void)
{
    pmu_set_sw_reset_er8130();
}

void hal_drv_pmu_module_clk_enable(uint32_t module_idx)
{
    pmu_module_clk_enable_er8130(module_idx);
}

void hal_drv_pmu_module_clk_disable(uint32_t module_idx)
{
    pmu_module_clk_disable_er8130(module_idx);
}

bool hal_drv_pmu_module_clk_is_ebabled(uint32_t module_idx)
{
    return pmu_module_clk_is_enabled_er8130(module_idx);
}

void hal_drv_pmu_set_dyn_gate(uint8_t gate_idx)
{
    pmu_set_dyn_gate_er8130(gate_idx);
}

void hal_drv_pmu_set_clk_src_align(uint8_t clk_src)
{
    pmu_set_clk_src_align_er8130(clk_src);
}

void hal_drv_pmu_set_mcu_clk_factor(uint8_t clk_fact)
{
    pmu_set_mcu_clk_factor_er8130(clk_fact);
}

void hal_drv_pmu_set_bbp_cic_clk(uint8_t osr, uint8_t dr_idx)
{
    pmu_set_bbp_cic_clk_er8130(osr, dr_idx);
}

void hal_drv_pmu_set_bbp_mbus_cic_clk(uint8_t dr_idx)
{
    pmu_set_bbp_mbus_cic_clk_er8130(dr_idx);
}

void hal_drv_pmu_set_bbp_rate(uint8_t phy_mode, uint8_t mod_type, uint32_t data_rate, uint8_t mod_idx)
{
    pmu_set_bbp_rate_er8130(phy_mode, mod_type, data_rate, mod_idx);
}

void hal_drv_pmu_set_csr_clk_gated(uint8_t on_off)
{
    pmu_set_csr_clk_gated_er8130(on_off);
}

void hal_drv_pmu_set_bod_reset_en(uint8_t enable)
{
    pmu_set_bod_reset_en_er8130(enable);
}

void hal_drv_pmu_set_interrupt(uint32_t source)
{
    pmu_set_interrupt_er8130(source);
}

uint32_t hal_drv_pmu_get_interrupt_status(void)
{
    return pmu_get_interrupt_status_er8130();
}

void hal_drv_pmu_clear_interrupt_status(uint32_t source)
{
    pmu_clear_interrupt_status_er8130(source);
}

void hal_drv_pmu_module_sw_reset(uint32_t module)
{
    pmu_module_sw_reset_er8130(module);
}

void hal_drv_pmu_lfosc_clk_disable()
{
    pmu_lfosc_clk_src_disable_er8130();
}

void hal_drv_pmu_clk_src_sel(uint8_t clk_src)
{
    pmu_clk_src_sel_er8130(clk_src);
}

void hal_drv_pmu_switch_nfc_pwr_src(uint8_t src)
{
    pmu_switch_nfc_pwr_src_er8130(src);
}

void hal_drv_pmu_set_cpu_pd(void)
{
    pmu_set_cpu_pd_er8130();
}

void hal_drv_pmu_reset_module_clock(void)
{
    pmu_reset_module_clock_er8130();
}

void hal_drv_pmu_reset_module_core(uint32_t module_idx)
{
    pmu_reset_module_core_er8130(module_idx);
}

void hal_drv_pmu_set_sram_pd_mode(uint8_t sram_32_64)
{
    //not soupport in 9005
}

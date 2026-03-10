/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_pmu.h"
#include "drv/hal_drv_pmu.h"
#include "drv/hal_drv_dma.h"

void hal_intf_pmu_set_sw_reset(void)
{
    hal_drv_pmu_set_sw_reset();
}

int32_t hal_intf_pmu_set_pwr_mode(uint8_t mode)
{
    hal_drv_pmu_set_pwr_mode(mode);
    return HAL_NO_ERR;
}

int32_t hal_intf_pmu_module_clk_disable(uint32_t module_idx)
{
    hal_drv_pmu_module_clk_disable(module_idx);
    return HAL_NO_ERR;
}

int32_t hal_intf_pmu_module_clk_enable(uint32_t module_idx)
{
    hal_drv_pmu_module_clk_enable(module_idx);
    return HAL_NO_ERR;
}

bool hal_intf_pmu_module_clk_is_enabled(uint32_t module_idx)
{
    return hal_drv_pmu_module_clk_is_ebabled(module_idx);
}

int32_t hal_intf_pmu_set_dyn_gate(uint8_t gate_idx)
{
    hal_drv_pmu_set_dyn_gate(gate_idx);
    return HAL_NO_ERR;
}

int32_t hal_intf_pmu_set_mcu_clk_factor(uint8_t clk_fact)
{
    hal_drv_pmu_set_mcu_clk_factor(clk_fact);
    return HAL_NO_ERR;
}

int32_t hal_intf_pmu_set_xip_clk(uint8_t clk_fac)
{
#ifdef CHIP_XIP_SUPPORT_RUNTIME_CONFIG
    hal_drv_pmu_set_xip_clk(clk_fac);
    return HAL_NO_ERR;
#else
    return HAL_ERR;
#endif
}

int32_t hal_intf_pmu_set_bbp_cic_clk(uint8_t osr, uint8_t dr_idx)
{
    hal_drv_pmu_set_bbp_cic_clk(osr, dr_idx);
    return HAL_NO_ERR;
}

int32_t hal_intf_pmu_set_csr_clk_gated(uint8_t on_off)
{
    hal_drv_pmu_set_csr_clk_gated(on_off);
    return HAL_NO_ERR;
}

int32_t hal_intf_pmu_set_bod_reset_en(uint8_t enable)
{
    hal_drv_pmu_set_bod_reset_en(enable);
    return HAL_NO_ERR;
}

int32_t hal_intf_pmu_set_interrupt(uint32_t source)
{
    hal_drv_pmu_set_interrupt(source);
    return HAL_NO_ERR;
}

uint32_t hal_pmu_get_int_status()
{
    return hal_drv_pmu_get_interrupt_status();
}

void hal_pmu_clear_int_status(uint32_t source)
{
    hal_drv_pmu_clear_interrupt_status(source);
}

void hal_intf_pmu_module_sw_reset(uint32_t module)
{
#if (CHIP_RADIO_FLEXIBLE_FMT_VERSION == 2)
    hal_drv_dma_backup_channels();
#endif

    hal_drv_pmu_module_sw_reset(module);

#if (CHIP_RADIO_FLEXIBLE_FMT_VERSION == 2)
    hal_drv_dma_restore_channels();
#endif
}

void hal_intf_pmu_lfosc_clk_disable()
{
    hal_drv_pmu_lfosc_clk_disable();
}

void hal_intf_pmu_set_cpu_pd(void)
{
#ifdef CHIP_PMU_HAS_CPU_PD
    hal_drv_pmu_set_cpu_pd();
#endif
}

void hal_intf_pmu_reset_module_clock(void)
{
    hal_drv_pmu_reset_module_clock();
}

void hal_intf_pmu_reset_module_core(uint32_t module_idx)
{
    hal_drv_pmu_reset_module_core(module_idx);
}

void hal_intf_pmu_set_sram_pd_mode(uint8_t sram_32_64)
{
    hal_drv_pmu_set_sram_pd_mode(sram_32_64);
}

void hal_intf_pmu_enable_bod_reset(uint8_t enable)
{
#ifdef CHIP_PMU_HAS_INT_CTRL //temporarily used this flag to avoid compilation error in 9005
    hal_drv_pmu_enable_bod_reset(enable);
    hal_drv_pmu_set_interrupt(PMU_INT_SRC_BOD);
#endif
}

uint32_t hal_intf_pmu_get_int_status(void)
{
#ifdef CHIP_PMU_HAS_INT_CTRL
    return hal_drv_pmu_get_int_status();
#else
    return 0;
#endif
}

void hal_intf_pmu_clear_int_status(uint32_t mask)
{
#ifdef CHIP_PMU_HAS_INT_CTRL
    hal_drv_pmu_clear_int_status(mask);
#endif
}

HAL_STATUS hal_intf_pmu_register_callback(HAL_ISR_CALLBACK callback, void *context)
{
#ifdef CHIP_PMU_HAS_INT_CTRL
    hal_drv_pmu_register_callback(callback, context);
    return HAL_NO_ERR;
#else
    return HAL_ERR;
#endif
}

HAL_STATUS hal_intf_pmu_unregister_callback(void)
{
#ifdef CHIP_PMU_HAS_INT_CTRL
    hal_drv_pmu_unregister_callback();
    return HAL_NO_ERR;
#else
    return HAL_ERR;
#endif
}


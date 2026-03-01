/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_PMU_H
#define __HAL_DRV_PMU_H

#include "hal_intf_pmu.h"
#include <stdint.h>

#define MODEM_DYN_GATED BIT0
#define ANCTL_DYN_GATED BIT1
#define FLCTL_DYN_GATED BIT2

#define MAC_CORE_RESET BIT22
#define BBP_CORE_RESET BIT23
#define ANA_CORE_RESET BIT28

enum {
    PMU_INT_SRC_BOD = BIT0,
    PMU_INT_SRC_NFC = BIT8,   
};

typedef void (*PMU_EVT_CALLBACK_T)(void* context);

void hal_drv_pmu_set_pwr_mode(uint8_t mode);
void hal_drv_pmu_set_sw_reset(void);
void hal_drv_pmu_module_clk_enable(uint32_t module_idx);
void hal_drv_pmu_module_clk_disable(uint32_t module_idx);
bool hal_drv_pmu_module_clk_is_ebabled(uint32_t module_idx);
void hal_drv_pmu_set_dyn_gate(uint8_t gate_idx);
void hal_drv_pmu_set_clk_src_align(uint8_t clk_src);
void hal_drv_pmu_set_mcu_clk_factor(uint8_t clk_fact);
void hal_drv_pmu_set_xip_clk(uint8_t clk_fac);
void hal_drv_pmu_set_bbp_cic_clk(uint8_t osr, uint8_t dr_idx);
void hal_drv_pmu_set_bbp_mbus_cic_clk(uint8_t dr_idx);
void hal_drv_pmu_set_bbp_rate(uint8_t phy_mode, uint8_t mod_type, uint32_t data_rate, uint8_t mod_idx);
void hal_drv_pmu_set_csr_clk_gated(uint8_t on_off);
void hal_drv_pmu_set_bod_reset_en(uint8_t enable);
void hal_drv_pmu_set_interrupt(uint32_t source);
uint32_t hal_drv_pmu_get_interrupt_status(void);
void hal_drv_pmu_clear_interrupt_status(uint32_t source);
void hal_drv_pmu_lfosc_clk_disable();
void hal_drv_pmu_clk_src_sel(uint8_t clk_src);
void hal_drv_pmu_set_sram_pd_mode(uint8_t sram_32_64);
void hal_drv_pmu_set_wakeup_by_nfc(uint8_t enable);
void hal_drv_pmu_enable_bod_reset(uint8_t enable);
uint32_t hal_drv_pmu_get_int_status(void);
void hal_drv_pmu_clear_int_status(uint32_t mask);
HAL_STATUS hal_drv_pmu_register_callback(PMU_EVT_CALLBACK_T callback, void *context);
HAL_STATUS hal_drv_pmu_unregister_callback(void);

// radio need it
void hal_drv_pmu_module_sw_reset(uint32_t module);
void hal_drv_pmu_switch_nfc_pwr_src(uint8_t src);
void hal_drv_pmu_set_cpu_pd(void);
void hal_drv_pmu_reset_module_clock(void);
void hal_drv_pmu_reset_module_core(uint32_t module_idx);


#endif

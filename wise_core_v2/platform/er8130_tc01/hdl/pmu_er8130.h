/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __PMU_ER8130_H
#define __PMU_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

#define PMU_UNLOCK_KEY 0x1ACCE551

#define PMU_STS_WUING 0x1
#define PMU_STS_PDING 0x2
#define PMU_STS_WU 0x4
#define PMU_STS_PD 0x8

#define PMU_WAKEUP_STATE                                                                                                                             \
    (((PMU_STS_WU << PCRMU_PMU_MODEM_STS_POS) & PCRMU_PMU_MODEM_STS_MASK) | ((PMU_STS_WU << PCRMU_PMU_PERIPH_STS_POS) & PCRMU_PMU_PERIPH_STS_MASK) | \
     ((PMU_STS_WU << PCRMU_PMU_NFC_STS_POS) & PCRMU_PMU_NFC_STS_MASK))

enum PMU_INTERNAL_CLK_SRC {
    HDL_PMU_INTCLK_SRC_32K = 0, // 32KHz clock source
    HDL_PMU_INTCLK_SRC_16K = 1, // 16KHz clock source
    HDL_PMU_EXTCLK_SRC_32K = 2,
};

void pmu_set_pwr_mode_er8130(uint8_t mode);
void pmu_set_sw_reset_er8130(void);
void pmu_module_clk_enable_er8130(uint32_t module_idx);
void pmu_module_clk_disable_er8130(uint32_t module_idx);
bool pmu_module_clk_is_enabled_er8130(uint32_t module_idx);
void pmu_set_dyn_gate_er8130(uint8_t gate_idx);
void pmu_set_clk_src_align_er8130(uint8_t clk_src);
void pmu_set_mcu_clk_factor_er8130(uint8_t clk_fact);
void pmu_set_bbp_cic_clk_er8130(uint8_t osr, uint8_t dr_idx);
void pmu_set_bbp_mbus_cic_clk_er8130(uint8_t dr_idx);
void pmu_set_bbp_rate_er8130(uint8_t phy_mode, uint8_t mod_type, uint32_t data_rate, uint8_t mod_idx);
void pmu_set_csr_clk_gated_er8130(uint8_t on_off);
void pmu_module_sw_reset_er8130(uint32_t module);
void pmu_set_bod_reset_en_er8130(uint8_t enable);
void pmu_set_interrupt_er8130(uint32_t source);
uint32_t pmu_get_interrupt_status_er8130();
void pmu_clear_interrupt_status_er8130(uint32_t source);
void pmu_lfosc_clk_src_disable_er8130();
void pmu_clk_src_sel_er8130(uint8_t clk_src);
void pmu_switch_nfc_pwr_src_er8130(uint8_t src);
void pmu_set_cpu_pd_er8130(void);
void pmu_module_reset_er8130(uint32_t module_idx);
void pmu_reset_module_clock_er8130(void);
void pmu_reset_module_core_er8130(uint32_t module_idx);
#endif

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_PMU_H
#define __HAL_INTF_PMU_H

#include "esmt_chip_specific.h"
#include "types.h"
#include <stdbool.h>
#include <stdint.h>

enum pmu_sw_rst_module {
    PMU_SW_RST_BBP   = BIT0,
    PMU_SW_RST_MAC   = BIT1,
    PMU_SW_RST_FIFO  = BIT2,
    PMU_SW_RST_ANCTL = BIT3,
};

enum es_pwr_mode {
    PWR_MODE_ACTIVITY = 0,
    PWR_MODE_SLEEP,
    PWR_MODE_DEEP_SLEEP,
    PWR_MODE_SHUTDOWN,
};

enum pmu_internal_clk_src {
    PMU_INTCLK_SRC_32K = 0,
    PMU_INTCLK_SRC_16K,
};

#define hal_intf_module_clk_enable hal_intf_pmu_module_clk_enable
#define hal_intf_module_clk_disable hal_intf_pmu_module_clk_disable
#define hal_intf_module_clk_is_enabled hal_intf_pmu_module_clk_is_enabled

void hal_intf_pmu_set_sw_reset(void);
int32_t hal_intf_pmu_set_pwr_mode(uint8_t mode);
int32_t hal_intf_pmu_module_clk_disable(uint32_t module_idx);
int32_t hal_intf_pmu_module_clk_enable(uint32_t module_idx);
bool hal_intf_pmu_module_clk_is_enabled(uint32_t module_idx);
int32_t hal_intf_pmu_set_dyn_gate(uint8_t gate_idx);
int32_t hal_intf_pmu_set_mcu_clk_factor(uint8_t clk_fact);
int32_t hal_intf_pmu_set_xip_clk(uint8_t clk_fac);
int32_t hal_intf_pmu_set_bbp_cic_clk(uint8_t osr, uint8_t dr_idx);
int32_t hal_intf_pmu_set_csr_clk_gated(uint8_t on_off);
int32_t hal_intf_pmu_set_bod_reset_en(uint8_t enable);
int32_t hal_intf_pmu_set_interrupt(uint32_t source);
int32_t hal_intf_pmu_reg_isr_callback(void *cb_proc, void *cb_data);
void hal_intf_pmu_set_cpu_pd(void);
void hal_intf_pmu_set_sram_pd_mode(uint8_t sram_32_64);
void hal_intf_pmu_enable_bod_reset(uint8_t enable);

uint32_t hal_pmu_get_int_status();
void hal_pmu_clear_int_status(uint32_t source);

int32_t hal_intf_pmu_init();
int32_t hal_intf_pmu_deinit();

// radio need it
void hal_intf_pmu_module_sw_reset(uint32_t module);
void hal_intf_pmu_reset_module_clock(void);
void hal_intf_pmu_reset_module_core(uint32_t module_idx);
void hal_intf_pmu_lfosc_clk_disable();

//NFC using
uint32_t hal_intf_pmu_get_int_status(void);
void hal_intf_pmu_clear_int_status(uint32_t mask);
HAL_STATUS hal_intf_pmu_register_callback(HAL_ISR_CALLBACK callback, void *context);
HAL_STATUS hal_intf_pmu_unregister_callback(void);

#endif

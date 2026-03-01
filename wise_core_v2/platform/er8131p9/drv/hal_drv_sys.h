/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_SYS_H
#define __HAL_DRV_SYS_H

#include "hal_intf_sys.h"
#include <stdint.h>

typedef struct {
    INTERNAL_SCLK_16K_OPTION_T sclk_16k_option;
    uint8_t  ldo_cur;
    uint8_t  ldo_sel;
    uint8_t  ldo_tmp;
    uint16_t freq_ctrl_sel;
    uint8_t  sclk_settle;
    uint32_t target_count;
    uint8_t  step_coarse;
    uint8_t  num_slow_sclk_cycle;
    uint16_t init_ctrl_index;
    uint16_t coarse_diff;
    uint8_t  cal_cnt;
    // 9006 only
    uint8_t  step_fine;
    uint8_t  nostop;
    uint8_t reserved[8];
} DRV_SCLK16K_PARAM_T;

typedef struct {
    INTERNAL_SCLK_32K_OPTION_T sclk_32k_option;
    uint8_t tim_osc;
    uint8_t cap_ctrl;
    uint8_t ictrl;
    uint8_t ldo_vdd_lv;
    uint16_t osc_clk_div;
    uint8_t  fref_clk_div;
    uint32_t cycles_target;
    uint8_t reserved[8];
} DRV_SCLK32K_PARAM_T;

uint32_t hal_drv_sys_get_chip_id(void);
void hal_drv_sys_set_remap(uint32_t remap_addr);
void hal_drv_sys_lock(void);
void hal_drv_sys_unlock(void);
//void hal_drv_sys_gpio_cfg(bool swd_enable, bool swd_pullup, bool tcxo_enable);
void hal_drv_sys_swd_cfg(bool swd_enable);
void hal_drv_sys_tcxo_cfg(bool tcxo_enable);
uint8_t hal_drv_sys_get_pa_type(void);
void hal_drv_sys_set_pa_type(uint8_t pa_type);
uint8_t hal_drv_sys_get_board_match_type(void);
void hal_drv_sys_set_board_match_type(uint8_t mat_type);
void hal_drv_sys_set_40m_gain_ctrl(uint8_t gain_ctrl);
uint8_t hal_drv_sys_get_40m_gain_ctrl();
uint32_t hal_drv_sys_get_xtal_cfg(void);
void hal_drv_sys_set_xtal_cfg(uint8_t xtal_i, uint8_t xtal_o);

void hal_drv_sys_tick_init();
uint32_t hal_drv_sys_tick_get_counter();
void hal_drv_sys_tick_delay_ms(uint32_t ms);
void hal_drv_sys_tick_delay_us(uint32_t us);


extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_default;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_run_16k;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_run_8k;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default_lowpwr;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default_lowpwr0p6v;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_32k;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_ext32k;
int8_t hal_drv_sys_internal_16kosc_calibration(const DRV_SCLK16K_PARAM_T *p);
int8_t hal_drv_sys_internal_32kosc_calibration(const DRV_SCLK32K_PARAM_T *p);
void hal_drv_sys_switch_sclk_src(uint8_t sclk_idx);

/* ================== ASARADC =============== */
void hal_drv_asaradc_start(void);
bool hal_drv_asaradc_is_ready(void);
void hal_drv_asaradc_verf_config(bool vref, bool scaler_enable, uint8_t scaler_lv);
void hal_drv_asaradc_basic_config(bool fetch_mode, uint8_t timer_mode1, uint8_t timer_mode2);
void hal_drv_asaradc_enable_digital_average_filter(bool enable, uint8_t shift_n);
void hal_drv_asaradc_set_vin_sel(uint8_t vin_sel);
uint8_t hal_drv_asaradc_get_vin_sel(void);
void hal_drv_asaradc_set_interrupt(bool enable);
bool hal_drv_asaradc_get_int_status(void);
bool hal_drv_asaradc_get_int_masked_status(void);
void hal_drv_asaradc_clear_int_status(void);
uint16_t hal_drv_asaradc_get_data(void);
uint32_t hal_drv_asaradc_get_hr_data(void);
#endif

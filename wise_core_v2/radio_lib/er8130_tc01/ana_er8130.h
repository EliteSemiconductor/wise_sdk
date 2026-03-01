/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __ANA_ER8130_H__
#define __ANA_ER8130_H__

#include "esmt_chip_specific.h"
#include "hal_drv_radio.h"
#include "hal_intf_radio.h"

#define ADC_CLK_8M 1
#define ADC_CLK_4M 2

#ifdef WORKAROUND_490MHZ
typedef struct {
    uint32_t div_int_if;
    uint32_t div_frac_if;
    uint32_t div_int_hb;
    uint32_t div_frac_hb;
    uint32_t div_int_lb;
    uint32_t div_frac_lb;
} TPM_INFO_TBL;
#endif

enum sclk_16k_pwr_mode {
    HDL_SCLK_16K_PWR_NORMAL,
    HDL_SCLK_16K_PWR_LOWPOWER,
    HDL_SCLK_16K_PWR_LOWPOWER_0P6V,
    HDL_SCLK_16KSRC_RUN_32K
};

ANA_INFO_T *ana_get_ana_info_er81xx(void);
void ana_set_ch_freq_er81xx(uint32_t ch);
uint32_t ana_get_ch_freq_er81xx(RADIO_RX_INFO_T *rx_info);
void ana_set_pa_cfg_er81xx(uint8_t pa_type, uint8_t mat_type, uint8_t mod_type, uint32_t data_rate);
void ana_set_tx_pwr_lv_er81xx(uint8_t mod_type, uint8_t pwr_lv);
void ana_set_pwr_mode_er81xx(uint8_t mode);
void ana_set_tx_ramp_backup_er81xx(void);
void ana_get_tx_ramp_backup_er81xx(void);
void ana_set_ctrl_mode_er81xx(uint8_t ctrl_mode);
void ana_enable_singletone_er81xx(uint8_t mod_type, uint32_t pwr_lv, uint8_t on_off);
void ana_set_mod_type_er81xx(uint32_t mod_type);
void ana_set_rx_data_rate_er81xx(uint8_t phy_mode, uint32_t data_rate);
uint32_t ana_get_data_rate_er81xx(void);
void ana_set_basic_cfg_er81xx(void);
void ana_set_adc_cfg_er81xx(uint8_t phy_mode, uint32_t data_rate);
void ana_set_dcdc_cfg_er81xx(uint8_t pa_type, uint8_t mat_type);
void ana_set_module_pd_rst_er81xx(void);
void ana_set_tpm_dmi_er81xx(uint8_t phy_mode, uint32_t mod_type, uint32_t data_rate);
void ana_set_pa_pwr_tbl_er81xx(uint32_t mod_type);
void ana_set_synthesize_er81xx(uint32_t mod_type);
void ana_set_rx_atop_cfg_er81xx(void);
void ana_set_gain_src_er81xx(uint8_t gain_src);
void ana_set_rx_gain_er81xx(void);
void ana_set_rx_filt_er81xx(uint8_t phy_mode, uint32_t data_rate, uint8_t mod_idx);
void ana_set_user_cfg_er81xx(void);
void ana_set_iqk_cfg_er81xx(IQK_MODE mode);
void ana_clean_iqk_cfg_er81xx(IQK_MODE mode);
void ana_set_osc_backup_er81xx(void);
void ana_set_osc_cal_enable_er81xx(uint32_t stable_time);
void ana_set_osc_cal_finish_er81xx();
uint8_t ana_get_rx_status_er81xx(void);
void ana_set_freq_devia_er81xx(uint32_t freq_devia);
void hal_drv_ana_switch_nfc_pwr_src(uint8_t src);
void ana_set_charge_pump_er81xx(uint8_t phy_mode, uint32_t data_rate);
void ana_set_tpm_cal_er81xx(uint32_t mod_type, uint32_t data_rate);
void ana_set_xtal_cfg_er81xx(void);
void ana_set_tx_ramp_cfg_er81xx(uint8_t mod_type);
uint32_t ana_get_tpm_din_val_er81xx(void);
void ana_toggle_tx_en_er81xx(void);
uint8_t ana_is_tpm_cal_done_er81xx(void);
uint32_t ana_get_tpm_din_hb_er81xx(void);
void ana_set_tpm_din_hb_er81xx(uint32_t val);
uint8_t ana_get_kvc_er81xx(void);
void ana_set_kvc_er81xx(uint8_t val);
uint32_t ana_get_mac_status_er81xx(void);
void ana_set_xtal_gain_ctrl_er81xx(uint8_t val);
void ana_set_dcdc_default_val_er81xx(void);
void ana_control_16kosc_power(bool enable);
void ana_control_32kosc_power(bool enable);
void ana_disable_pmu_shutdown_control_for_32k_16k_osc(void);
void ana_tune_16kosc_to_32k_mode(bool boost);
void ana_power_management_for_16kosc(uint8_t option);
int8_t ana_exec_anctl_osc_calibration(void);
int8_t ana_exec_anctl_osc16k_calibration(void);
int8_t ana_exec_anctl_osc32k_calibration(uint8_t tim_osc);
void ana_ldo_paramaters_for_16kosc_calibration(uint8_t ldo_cur, uint8_t ldo_sel, uint8_t ldo_tmp, uint16_t freq_ctrl_sel);
void ana_reg0_paramaters_for_16kosc_calibration(uint8_t sclk_settle, uint32_t target_count, uint8_t step_coarse);
void ana_reg1_paramaters_for_16kosc_calibration(uint8_t num_slow_sclk_cycle, uint16_t init_ctrl_index, uint16_t coarse_diff, uint8_t cal_cnt);
void ana_paramaters_for_32kosc_calibration(uint8_t cap_ctrl, uint8_t ictrl, uint8_t ldo_vdd_lv, uint16_t osc_clk_div, uint8_t fref_clk_div,
                                           uint32_t cycles_target);
/* ================== ASARADC =============== */
void ana_asaradc_vref_level(bool vref);
void ana_asaradc_verf_scaler_level(bool enable, uint8_t scaler_lv);
void ana_asaradc_start_er8130(void);
bool ana_asaradc_is_ready_er8130(void);
void ana_asaradc_set_timing_er8130(uint8_t timer_mode1, uint8_t timer_mode2);
void and_asaradc_set_fetch_mode_er8310(bool fetch_mode);
void ana_asaradc_daf_mode_er8130(bool enable, uint8_t shift_n);
void ana_asaradc_set_vin_sel_er8130(uint8_t sel);
uint8_t ana_asaradc_get_vin_sel_er8130(void);
void ana_asaradc_set_interrupt_er8130(bool enable);
void ana_asaradc_clear_int_status_er8130(void);
bool ana_asaradc_get_int_status_er8130(void);
bool ana_asaradc_get_int_masked_status_er8130(void);
uint16_t ana_asaradc_get_data_er8130(void);
uint32_t ana_asaradc_get_hr_data_er8130(void);
#endif

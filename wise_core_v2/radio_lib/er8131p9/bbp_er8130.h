/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __BBP_ER8130_H__
#define __BBP_ER8130_H__

#include "esmt_chip_specific.h"
#include "hal_drv_radio.h"

BBP_INFO_T *bbp_get_bbp_info_er81xx(void);
void bbp_get_rx_info_er81xx(RADIO_RX_INFO_T *rx_info);
uint8_t bbp_chk_rssi_rdy_er81xx(void);
void bbp_set_basic_cfg_er81xx(void);
void bbp_set_tx_filt_er81xx(uint32_t mod_type, HAL_DATA_RATE_T dr_idx);
void bbp_set_osr_er81xx(uint32_t osr);
void bbp_set_rx_filt_er81xx(uint8_t phy_mode, uint8_t dr_idx, uint8_t mod_idx);
void bbp_set_agc_er81xx(BBP_INFO_T *bbp, uint8_t phy_mode, uint32_t mod_type, uint8_t dr_idx, uint8_t mod_idx);
void bbp_set_bpsk_gain_thd_er81xx(BBP_INFO_T *bbp);
void bbp_set_pop_clip_er81xx(void);
void bbp_set_win_size_er81xx(BBP_INFO_T *bbp, uint8_t phy_mode, uint8_t dr_idx);
void bbp_set_pe_power_thr_er81xx(uint32_t mod_type, uint8_t dr_idx);
void bbp_set_user_cfg_er81xx(void);
void bbp_set_rx_syncword_er81xx(uint32_t *syncword, uint8_t sw_len);
void bbp_set_rx_pn9_er81xx(uint8_t enable);
void bbp_set_rx_fec_er81xx(uint8_t enable);
void bbp_set_iqk_cfg_er81xx(uint8_t dr_idx, uint32_t adc_clk, uint32_t lo);
void bbp_iqk_trigger_er81xx(void);
void bbp_set_iqk_lmt_er81xx(BBP_INFO_T *bbp);
void bbp_clean_iqk_cfg_er81xx(void);
uint32_t bbp_get_iqk_val_er81xx(void);
uint16_t bbp_get_iqk_spp_power_er81xx(void);
void bbp_set_iqk_val_er81xx(uint32_t val);
void bbp_set_ddc_if_phi_er81xx(uint8_t phy_mode, uint32_t data_rate);
void bbp_set_bt_rate_er81xx(uint8_t phy_mode, uint8_t mod_type, uint32_t data_rate, uint8_t mod_idx);
void bbp_set_agc_mode_er81xx(uint8_t mode, uint8_t gain_idx);
uint8_t bbp_get_agc_idx_er81xx(void);
void bbp_set_agc_idx_er81xx(uint8_t idx);
uint8_t bbp_get_backup_agc_idx_er81xx(void);
uint16_t bbp_get_inband_pwr_er81xx(void);
void bbp_set_ed_threshold_er81xx(int16_t ed_thres);
void bbp_enable_cdr_est_er81xx(uint8_t enable);
uint16_t bbp_get_rssi_er81xx(void);
void bbp_set_wmbus_rx_extra_cfg_er81xx(uint8_t wmbus_mode, uint32_t data_rate);
void bbp_setup_er81xx(uint8_t phy_mode, uint32_t data_rate, uint8_t mod_idx, uint8_t wmbus_mode);

#endif

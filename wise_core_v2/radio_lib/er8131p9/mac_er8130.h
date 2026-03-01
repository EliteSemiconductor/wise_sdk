/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __MAC_ER8130_H__
#define __MAC_ER8130_H__

#include "esmt_chip_specific.h"
#include "wise_core.h"
#include "hal_drv_radio.h"

#define PHR_ONE_BYTE_MASK 0x07F
#define PHR_TWO_BYTE_MASK 0x7FF

MAC_INFO_T *mac_get_mac_info_er81xx(void);
void mac_event_get_status_er81xx(EVENT_INFO_U *event_info); //depreciated API
void mac_event_clear_status_er81xx(uint32_t event_status); //depreciated API
void mac_disable_interrupt_er81xx(void);
void mac_enable_interrupt_er81xx(uint32_t intMask);
uint32_t mac_get_interrupt_status_er81xx(void);
void mac_clear_interrupt_status_er81xx(uint32_t status);
void mac_get_rx_val_len_er81xx(RADIO_RX_INFO_T *rx_info);
uint8_t mac_rx_stop_er81xx(uint8_t phy_mode);
void mac_rx_start_er81xx(uint8_t phy_mode);
void mac_set_rx_restart_er81xx(uint8_t enable);
uint8_t mac_get_rx_fsm_busy_er81xx(void);
void mac_tx_start_er81xx(uint8_t *tx_buf, uint32_t tx_len);
void mac_tx_stop_er81xx(void);
void mac_set_mod_type_er81xx(uint32_t mod_type);
void mac_set_tx_data_rate_er81xx(uint32_t data_rate);
void mac_set_tx_phy_format_cfg_er81xx(uint32_t mod_type, uint8_t tx_hw_opt);
void mac_set_rx_manchester_er81xx(uint8_t enable);
void mac_set_tx_preamble_er81xx(uint32_t pream, uint8_t pream_len);
void mac_set_tx_sync_word_er81xx(uint32_t mod_type, uint32_t *sync_word, uint8_t sync_word_len);
void mac_set_tx_wmbus_sync_word_er81xx(uint8_t wmbus_mode, uint32_t data_rate);
void mac_set_tx_whitening_er81xx(uint32_t mod_type, uint8_t enable);
uint8_t mac_get_osr_er81xx(uint32_t mod_type, uint8_t dr_idx);
void mac_set_rx_802154_keepfmt_er81xx(uint8_t keep);
void mac_set_rx_maxlen_er81xx(uint32_t rx_max_len);
uint32_t mac_get_iqk_ch_er81xx(uint8_t mat_type);
void mac_set_panid_er81xx(uint16_t panid);
void mac_set_short_addr_er81xx(uint16_t s_addr);
void mac_set_long_addr_er81xx(const uint8_t *l_addr);
void mac_enable_rx_timeout_er81xx(uint8_t to_base, uint8_t to_num);
void mac_disable_rx_timeout_er81xx(void);
void mac_set_rx_payload_order_er81xx(uint8_t bit_rev, uint8_t byte_rev);
void mac_set_tx_payload_order_er81xx(uint8_t bit_rev, uint8_t byte_rev);
void mac_set_crc_inverse_er81xx(uint8_t enable);
void mac_set_crc_bit_endian_er81xx(uint8_t din, uint8_t dout);
void mac_set_crc_byte_endian_er81xx(uint8_t endian);
void mac_set_crc_config_er81xx(uint8_t bit, uint32_t poly, uint32_t init_val);
void mac_enable_crc_check_er81xx(uint8_t enable);
void mac_enable_phr_check_er81xx(uint8_t enable);
void mac_set_rx_phr_config_er81xx(uint8_t len, uint8_t offset, uint8_t inv_en, uint16_t bits);
void mac_set_rx_phr_exclude_crc_er81xx(uint8_t exclude_crc);
void mac_set_crc_include_phr_er81xx(uint8_t include);
void mac_set_crc_endian_in_fcs_er81xx(uint8_t endian);
void mac_set_crc_altchk_er81xx(uint8_t enable);
void mac_set_tx_phr_config_er81xx(uint8_t len, uint8_t offset, uint8_t inv_en);
void mac_set_rx_wmbus_fmt_config_er81xx(uint8_t wmbus_mode, uint32_t data_rate);
#endif

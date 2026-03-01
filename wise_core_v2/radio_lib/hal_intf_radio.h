/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_RADIO_H__
#define __HAL_INTF_RADIO_H__

#include "esmt_chip_specific.h"
#include "types.h"

/* ==== define ============================================================== */
#ifndef ES_DEVICE_RADIO
#define ES_DEVICE_RADIO                                 1
#endif

#if ES_DEVICE_RADIO
#define RADIO_PHY_0_SUPPORT                             1
#else
#define RADIO_PHY_0_SUPPORT                             0
#endif

#ifdef ES_DEVICE_TRX_RADIO
#define RADIO_PHY_1_SUPPORT                             1
#else
#define RADIO_PHY_1_SUPPORT                             0
#endif

#define RADIO_PHY_NUM                                   (RADIO_PHY_0_SUPPORT + RADIO_PHY_1_SUPPORT)

// radio interrupt flags
#define HAL_RADIO_INT_TX_FIN                            0x00000001
#define HAL_RADIO_INT_TX_ERR                            0x00000002
#define HAL_RADIO_INT_RX_FIN                            0x00000004
#define HAL_RADIO_INT_RX_ERR                            0x00000008
#define HAL_RADIO_INT_CAL                               0x00000010
#define HAL_RADIO_INT_RX_SYNCWORD                       0x00000020
#define HAL_RADIO_INT_MBUS_C_FA                         0x00000040
#define HAL_RADIO_INT_MBUS_C_FB                         0x00000080

#define INT_RX_FIN(event_info)                          (event_info->event_b.rx_ok)
#define INT_RX_ERR(event_info)                          (event_info->event_b.rx_err)
#define INT_TX_FIN(event_info)                          (event_info->event_b.tx_ok)
#define INT_TX_ERR(event_info)                          (event_info->event_b.tx_err)
#define INT_RX_SYNCWORD(evnet_info)                     (evnet_info->event_b.syncw)

// radio rx timeout
#define MAX_RX_TIMOUT                                   1280 // units: ms
// radio rx invalid ed threshold
#define RX_INVALID_SIG_THRES                            0x7FFF

/* -----HW Frame option------------------------------------------------------- */
#define FRAME_HW_PREAMBLE                               BIT0
#define FRAME_HW_SYNC_WORD                              BIT1
#define FRAME_HW_PHR                                    BIT2
#define FRAME_HW_CRC                                    BIT3
#define FRAME_HW_WHITENING                              BIT4
#define FRAME_HW_DMA_REVERSE                            BIT5
#define FRAME_HW_FEC                                    BIT6
#define FRAME_HW_MANCH                                  BIT7


/* ==== enum ============================================================== */
#ifdef CHIP_RADIO_HAS_W_MBUS_FAKE
typedef enum {
    E_HAL_DR_12P5K = 0,
    E_HAL_DR_50K,
    E_HAL_DR_100K,
    E_HAL_DR_125K,
    E_HAL_DR_200K,
    E_HAL_DR_250K,
    E_HAL_DR_500K,
    E_HAL_DR_1M,
    E_HAL_DR_2M,
    E_HAL_DR_MAX, // put it at end
} HAL_DATA_RATE_T;
#else
typedef enum {
    E_HAL_DR_4P8K = 0,
    E_HAL_DR_12P5K,
    E_HAL_DR_32P768K,
    E_HAL_DR_50K,
    E_HAL_DR_100K,
    E_HAL_DR_125K,
    E_HAL_DR_200K,
    E_HAL_DR_250K,
    E_HAL_DR_500K,
    E_HAL_DR_1M,
    E_HAL_DR_2M,
    E_HAL_DR_MAX, // put it at end
} HAL_DATA_RATE_T;
#endif

enum {
    BOARD_MATCHING_915MHZ = 0,
    BOARD_MATCHING_868MHZ = 1,
    BOARD_MATCHING_490MHZ = 2,
    BOARD_MATCHING_MAX    = 3,
};
    
enum hal_wmbus_mode {
    HAL_WMBUS_MODE_S = 0,
    HAL_WMBUS_MODE_T = 1,
    HAL_WMBUS_MODE_C = 2, 
    HAL_WMBUS_MODE_R = 3,
    HAL_WMBUS_MODE_F = 4,
};


/* ==== CRC Config ============================================================== */
typedef enum {
    E_HAL_CRC_TYPE_8 = 0,
    E_HAL_CRC_TYPE_16,
    E_HAL_CRC_TYPE_24,
    E_HAL_CRC_TYPE_32,
} HAL_CRC_TYPE_T;

#define PHR_CFG_BYTE_ENDIAN_MSB_FIRST                   (1 << 0)
#define PHR_CFG_BIT_ENDIAN_MSB_FIRST                    (1 << 1)
#define PHR_CFG_LENGTH_INCLUDE_CRC                      (1 << 2)

#define CRC_CFG_INPUT_MSB_FIRST                         (1 << 0)
#define CRC_CFG_OUTPUT_MSB_FIRST                        (1 << 1)
#define CRC_CFG_OUTPUT_BYTE_MSB_FIRST                   (1 << 3)
#define CRC_CFG_INCLUDE_HEADER                          (1 << 4)
#define CRC_CFG_INVERT                                  (1 << 5)

/* ==== Struct ============================================================== */

typedef struct radio_cfg_t {
    /*=====sys=====*/
    uint32_t mod_type;
    uint32_t ch_freq;
    uint32_t freq_devia; // uint:Hz
    uint32_t data_rate;  // uint:bps
    uint32_t sync_word[2];
    uint32_t sync_word_len; // max=8byte unit:byte
    uint8_t phy_mode;
    uint8_t pwr_mode;
    uint8_t frame_hw_opt;
    uint8_t wmbus_mode;

    /*=====whitening=====*/
    uint16_t whitening_seed;

    /*=====PHR=====*/
    uint8_t hdr_bytes;
    uint8_t hdr_config;
    uint8_t length_bit_size;
    uint8_t length_bit_offset;

    /*=====CRC=====*/
    uint8_t crc_config; //[0:1]HAL_CRC_TYPE_T
    uint32_t crc_polynomial;
    uint32_t crc_seed;
    uint8_t crc_width;
    
    /*=====tx=====*/
    uint8_t tx_pwr_lv;
    uint8_t pream_len;
    uint32_t pream;

    /*=====rx=====*/
    uint32_t rx_max_len;
} RADIO_CFG_T;

typedef struct {
    uint32_t tpm_din_hb;
    uint8_t kvc;
} TPM_CAL_PARA;

typedef struct
{
    int8_t pwr_dbm;
    int8_t gain_val;
} TX_PWR_MAPPING;

/* ==== Function ============================================================ */

void hal_intf_radio_set_adapt(int8_t phy_idx);
void *hal_intf_radio_get_adapt(int8_t phy_idx);
void hal_intf_radio_set_isr_callback(int8_t phy_idx, HAL_ISR_CALLBACK cb_proc, void *cb_data);
void hal_intf_radio_set_int_en(int8_t phy_idx, uint32_t enMask);
uint32_t hal_intf_radio_get_int_en(int8_t phy_idx);
uint32_t hal_intf_radio_get_int_status(int8_t phy_idx);
uint32_t hal_intf_radio_get_int_mask_status(int8_t phy_idx, uint32_t intMask);
void hal_intf_radio_get_rx_info(int8_t phy_idx, void *rx_meta);
void hal_intf_radio_deinit(void);
void hal_intf_radio_set_pwr_mode(int8_t phy_idx, uint8_t mode);
void hal_intf_radio_set_tx_pwr(int8_t phy_idx, uint8_t lv);
int8_t hal_intf_radio_tx_pwr_dbm_to_raw(int8_t pwr_dbm);
int8_t hal_intf_radio_tx_pwr_raw_to_dbm(int8_t pwr_level);
uint16_t hal_intf_radio_get_cca_rssi(int8_t phy_idx);
void hal_intf_radio_enable_singletone(int8_t phy_idx, uint32_t pwr_lv, uint8_t enable);
void hal_intf_radio_print_rx_info(int8_t phy_idx);
void hal_intf_radio_set_ch(int8_t phy_idx, uint32_t ch);
void hal_intf_radio_tx_start(int8_t phy_idx, uint8_t *tx_buf, uint32_t tx_len);
void hal_intf_radio_tx_stop(int8_t phy_idx);
void hal_intf_radio_rx_start(int8_t phy_idx, uint8_t *rx_buf, uint32_t rx_len);
uint8_t hal_intf_radio_rx_stop(int8_t phy_idx);
void hal_intf_radio_set_rx_restart(int8_t phy_idx, uint8_t enable);
uint8_t hal_intf_radio_get_rx_busy(int8_t phy_idx);
HAL_STATUS hal_intf_radio_init(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
void hal_intf_radio_set_panid(int8_t phy_idx, uint16_t panid);
void hal_intf_radio_set_short_addr(int8_t phy_idx, uint16_t short_addr);
void hal_intf_radio_set_long_addr(int8_t phy_idx, const uint8_t *long_addr);
HAL_STATUS hal_intf_radio_set_synthesizer(int8_t phy_idx, uint32_t ch);
HAL_STATUS hal_intf_radio_wor_start(int8_t phy_idx, uint32_t fsh_time_ms, uint32_t rx_to_ms);
uint16_t hal_intf_radio_rx_calib(int8_t phy_idx, uint8_t phy_mode, uint8_t mod_type, uint16_t rx_sig_thres);
void hal_intf_radio_trig_tpm_calib(int8_t phy_idx, RADIO_CFG_T * radio_cfg);
void hal_intf_radio_get_tpm_cal_val(TPM_CAL_PARA *tpm_cal_val);
void hal_intf_radio_set_tpm_cal_val(TPM_CAL_PARA *tpm_cal_val);
void hal_intf_radio_set_dcdc_default_val(void);
HAL_STATUS hal_intf_radio_rx_config(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
HAL_STATUS hal_intf_radio_tx_config(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
HAL_STATUS hal_intf_radio_iq_calib(int8_t phy_idx, uint32_t ch_freq);
void hal_intf_radio_enable_bod(uint8_t enable, uint8_t bod_lv);
void hal_intf_radio_enable_prbs9(uint8_t enable);

#ifdef CHIP_RADIO_HAS_BLE
HAL_STATUS hal_intf_radio_ble_init(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
#endif

#ifdef ES_DEVICE_TRX_RADIO
void hal_intf_radio_get_rx_fifo(uint8_t *rx_buf, uint32_t rx_len);
void hal_intf_radio_reset_rx_fifo(void);
#endif

void hal_intf_radio_test(void);
void hal_intf_radio_set_ulpldo(uint8_t enable);
uint8_t hal_intf_radio_is_state_idle(void);

#endif

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __FUNC_RF_H
#define __FUNC_RF_H

#include <stdio.h>

#include "wise_core.h"
#include "wise.h"

#if (SHELL_RADIO == ENABLE)
#include "api/wise_gptmr_api.h"
#include "radio_lib/wise_radio_api.h"
#include "wise_file_system.h"
#include "wise_radio_802154_api.h"

#include "api/wise_sys_api.h"
#include "radio_lib/hal_intf_radio.h"

#ifdef ES_DEVICE_TRX_RADIO
#include "radio_lib/xcvr04/peri/wise_trx_sys_api.h"
#endif

#if CONFIG_DEBUG_TOOL
typedef struct {
    uint16_t ib_pwr;
    uint16_t tot_pwr;
    uint16_t rssi;
    uint16_t gain_idx;
    uint32_t gfo_est;
    uint8_t rx_ok;
    uint8_t rx_err;
    uint16_t rx_sts;
    uint8_t int_sts;
} RADIO_CTRL_RX_INFO_T;
#endif

typedef enum {
    WMBUS_MODE_S = 0,
    WMBUS_MODE_T = 1,
    WMBUS_MODE_C = 2, 
    WMBUS_MODE_R = 3,
    WMBUS_MODE_F = 4,
}WMBUS_MODE_E;

typedef enum {
    ROLE_OTHER = 0,
    ROLE_METER = 1,    
}WMBUS_APP_ROLE_E;

typedef struct {
    bool wmbus_is_testing;
    uint8_t mode;
    uint8_t role;
    uint8_t activity;
    uint8_t m2o_pat[40];
    uint8_t m2o_pat_len;
    uint8_t o2m_pat[40];
    uint8_t o2m_pat_len;
    WISE_RADIO_CFG_T *wmbus_radio_cfg;
    WISE_RADIO_PKT_FMT_T *wmbus_radio_pkt_fmt;
} WMBUS_TESTING_RADIO_INFO_T;

typedef struct {
    WISE_RADIO_CFG_T meter_tx;
    WISE_RADIO_CFG_T meter_rx;
    WISE_RADIO_CFG_T other_tx;
    WISE_RADIO_CFG_T other_rx;
    uint8_t *m2o_pat;
    uint8_t m2o_pat_len;
    uint8_t *o2m_pat;
    uint8_t o2m_pat_len;
} WMBUS_MODE_CONFIG_T;

// Test data arrays (same as SG)
static const uint8_t wmbus_raw_data_40bytes[] = {
    0x69, 0xa9, 0x59, 0x96, 0xa9, 0x99, 0x95, 0x6a,
    0xaa, 0x95, 0x6a, 0xa5, 0x96, 0x69, 0x59, 0x59,
    0xa5, 0x5a, 0xa6, 0x99, 0x69, 0xa9, 0x59, 0x96,
    0xa9, 0x99, 0x95, 0x6a, 0xaa, 0x95, 0x6a, 0xa5,
    0x96, 0x69, 0x59, 0x59, 0xa5, 0x5a, 0xa6, 0x99
};

static const uint8_t wmbus_raw_data_30bytes[] = {
    0x5a, 0xe9, 0x38, 0xd9, 0xa4, 0x2d, 0x72, 0x63,
    0x5a, 0xb4, 0xc3, 0x72, 0x1a, 0xab, 0xc9, 0x8e,
    0xe3, 0xc8, 0x72, 0xa3, 0xc5, 0x2c, 0xed, 0xd0,
    0x96, 0xd9, 0xc8, 0xec, 0x64, 0x8d
};

static const uint8_t wmbus_raw_data_20bytes[] = {
    0xF0, 0x22, 0x75, 0x30, 0x1E, 0x6A, 0x2C, 0x48, 
    0x80, 0xE0, 0x22, 0xE2, 0x1E, 0xD0, 0xC8, 0xC2, 
    0xA6, 0xE1, 0x78, 0xB6
};

static const uint8_t wmbus_raw_data_40_bytes_manchester[] = {
    0x55, 0xAA, 0x59, 0x59, 0x66, 0x6A, 0x55, 0x5A, 
    0xA9, 0x56, 0x99, 0x69, 0xA5, 0x59, 0x95, 0x65, 
    0x55, 0x95, 0x55, 0xA9, 0x59, 0x59, 0x59, 0xA9, 
    0xA9, 0x56, 0x55, 0xA6, 0x95, 0xA5, 0x59, 0xA5, 
    0x69, 0x99, 0x56, 0xA9, 0x95, 0x6A, 0x69, 0x9A
};


extern int8_t defRadioIntf;
extern WISE_RADIO_CFG_T radioCfg[RADIO_PHY_NUM];
extern WISE_RADIO_PKT_FMT_T radioPktFmt[RADIO_PHY_NUM];
extern WISE_RADIO_CFG_T fs_radio_cfg;
extern void _wise_radio_set_phy_mode(int8_t intf_idx, uint8_t phy_mode);
extern uint8_t _wise_radio_get_phy_mode(int8_t intf_idx);

void radioDebug(void);
void setPhyIntf(uint8_t _phy_intf);
uint8_t getPhyIntf(void);
void radioInit(void);
void radioDeinit(void);
void radioInit802154(void);
void radioInitBleMode(void);
void radioInitWMBUS(uint8_t mode ,uint8_t role, uint8_t act);
void radioEventCb(WISE_RADIO_EVT_T evt);
void radioRxOn(void);
void radioRxOff(void);
uint8_t isRadioConfigured(void);
void clearRadioConfigured(void);
uint8_t isRxOn(void);
void radioTestTxFrame(int testTxLen, uint8_t patternStart);
void radioTestSend(int testCount);
uint8_t isTxProcEnd(void);
void radioSetChannel(uint8_t channelIdx);
uint8_t radioGetChannel(void);
uint32_t radioGetFreq(void);
uint8_t radioGetValidChannelNumber(void);
uint8_t isInputPwrVaild(uint8_t _inputpwr);
void radioSetTxPwr(uint8_t _inputpwr);
uint8_t radioGetTxPwr(void);
void radioSetRxLog(uint8_t _val);
void setPanId(uint16_t _panid);
void setShortAddress(uint16_t _saddr);
void setLongAddress(uint8_t *_laddr);
int8_t radioGetDatarateIndex(uint32_t data_rate);
uint8_t radioTest(uint32_t val);
void set_frame_fmt_msb_first(bool enable);
bool get_frame_fmt_msb_first(void);
void radio_set_pkt_type(WISE_RADIO_PKT_TYPE_T pkt_type);
void radio_set_crc_config(uint32_t cfg);
uint32_t radio_get_crc_config(void);
bool radio_set_crc_poly(WISE_CRC_POLY_SEL_T poly);
int32_t radio_get_crc_poly(void);
void radio_set_crc_seed(uint16_t seed);
uint16_t radio_get_crc_seed(void);
void radio_crc_set_flags(uint32_t mask);
void radio_crc_clr_flags(uint32_t mask);
void set_sg_setting(bool enable);
void radio_clear_test_cnt(uint8_t enable);
void radio_set_ulpldo(uint8_t enable);


#endif //(SHELL_RADIO == ENABLE)

#endif //__FUNC_RF_H

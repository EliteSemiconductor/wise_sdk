/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdint.h>
#include "func_rf.h"
#include "fmt_print.h"
#include "api/wise_tick_api.h"
#include "wise_radio_api.h"
#include "wise_radio_wmbus_api.h"
#include "wise_radio_ble_api.h"
#include "wise_radio_802154_api.h"

#if (SHELL_RADIO == ENABLE)

#if CONFIG_DEBUG_TOOL
#include "wise_ctrl_packet.h"
#include "wise_ctrl_cmd_def.h"
#include "regfiles/bbp_reg_er8130.h"
uint8_t rx_tp_buf[0x100] = {0};
RADIO_CTRL_RX_INFO_T rx_info = {0};
#endif

#ifndef RADIO_MAX_FRAME_LEN // defined in preprocessor if necessary
#define RADIO_MAX_FRAME_LEN                     256
#endif

WMBUS_TESTING_RADIO_INFO_T wmbus_radio_info;

#define EN_TX_HW_CRC
uint8_t actChannel[RADIO_PHY_NUM]      = {0};
uint8_t radioInited[RADIO_PHY_NUM]     = {0};
uint8_t radioMode[RADIO_PHY_NUM]       = {0};
uint8_t radioConfigured[RADIO_PHY_NUM] = {0};
int8_t defRadioIntf                    = 0;
uint8_t txPwrLevel[RADIO_PHY_NUM]      = {
#if RADIO_PHY_0_SUPPORT
    127,
#endif
#if RADIO_PHY_1_SUPPORT
    127
#endif
};

uint8_t radioTestBuf[RADIO_MAX_FRAME_LEN];
int8_t periodicTxSchId;
int8_t rxSchId           = -1;
static uint32_t rxErrCount      = 0;
static uint32_t rxGoodCount     = 0;
static uint32_t rxTotalCount    = 0;
uint8_t rxStarted        = 0;
uint8_t rxLog            = 0;
uint16_t radioPANID      = 0xABCD;
uint16_t radioShortAddr  = 0xFFFF;
uint8_t radioLonfAddr[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define RADIO_BUFFER_LEN 2048
uint8_t radioBufferPool[RADIO_BUFFER_LEN];
WISE_RADIO_BUFFER_T radioBuffer = {RADIO_BUFFER_LEN, (uint32_t)radioBufferPool};

#ifdef EN_TX_HW_CRC
#define CONFIG_FRAME_FMT_CRC (FRAME_FMT_CRC_EN)
#else
#define CONFIG_FRAME_FMT_CRC (FRAME_FMT_NONE)
#endif

extern void _wise_radio_get_rx_info_output(int8_t intf_idx);
extern void _wise_radio_set_sg_test_mode(uint8_t enable);
extern uint8_t _wise_radio_get_sg_test_mode();
extern void _wmbus_set_mode(int8_t intf_idx, wmbus_mode_t mode);
extern void _wise_radio_set_phy_mode(int8_t intf_idx, uint8_t phy_mode);
extern uint8_t _wise_radio_get_phy_mode(int8_t intf_idx);
extern void _wise_radio_test(void);
extern void _wise_radio_set_ulpldo(uint8_t enable);

// WISE_RADIO_CFG_T fs_radio_cfg;
WISE_RADIO_CFG_T radioCfg[RADIO_PHY_NUM] = {
#if RADIO_PHY_0_SUPPORT
    {
        .modulation = E_MOD_TYPE_GFSK,

        // channel configuration
        .ch_freq_min = 915500000, // Hz
        .ch_freq_max = 922800000, // Hz
        .ch_spacing  = 200000,    // Hz

        // modem configuration
        .deviation    = 125000, // Hz
        .data_rate    = E_DATA_RATE_500K,
        .preamble     = 0xAA, // pattern
        .preamble_len = 4,          // bytes

        .sync_word_len = 4,
        .syncword     = 0xD8EC6B99, // will be send out MSB first

        
        
        //.phy_mode = E_PHY_TRANSPARENT,
    },
#endif
#if RADIO_PHY_1_SUPPORT
    {
        .modulation = E_MOD_TYPE_GFSK,

        // channel configuration
        .ch_freq_min = 915500000, // Hz
        .ch_freq_max = 922800000, // Hz
        .ch_spacing  = 200000,    // Hz

        // modem configuration
        .deviation    = 100000, // Hz
        .data_rate    = E_DATA_RATE_500K,
        .preamble     = 0xAAAAAAAA, // pattern
        .preamble_len = 4,          // bytes

        .sync_word_len = 4,
        .syncword     = 0xD8EC6B99, // sg syncword =0x667DF05A

        .frame_codec = E_FRAME_CODEC_NONE,
        
        //.phy_mode = E_PHY_TRANSPARENT,
    },
#endif
};

WISE_RADIO_PKT_FMT_T radioPktFmt[RADIO_PHY_NUM] = {
#if RADIO_PHY_0_SUPPORT
    {
        .pkt_type          = PKT_VARIABLE_LENGTH,
        .max_pkt_length    = RADIO_MAX_FRAME_LEN,
        .frame_format      = (
                              FRAME_FMT_PREAMBLE_EN |
                              FRAME_FMT_SYNCWORD_EN |
                              FRAME_FMT_HEADER_EN |
                              FRAME_FMT_CRC_EN |
                              /*FRAME_FMT_WHITENING_EN |*/
                              /*FRAME_FMT_FEC_EN | */
                              /*FRAME_FMT_MANCH_EN |*/
                            //   FRAME_FMT_MSB_FIRST |
                              CONFIG_FRAME_FMT_CRC
                              ),

        .phr =
        {
            .hdr_config        = PHR_LENGTH_INCLUDE_CRC,
            .hdr_bytes         = 1, //
            .length_bit_size   = 8, // 8~12 bits
            .length_bit_offset = 0, // bit offset in header
        },

        .crc =
        {
#ifdef WISE_WMBUS_SW_HELPER
            .crc_config        = CRC_INPUT_BIT_ENDIAN_MSB_FIRST |
                                 CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST |
                                 CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST |
                                 CRC_INVERT_ON,
#else
            .crc_config        = CRC_INPUT_BIT_ENDIAN_MSB_FIRST |
                                 CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST |
                                 CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST,
#endif            
            .crc_poly_sel      = CRC_POLYNOMIAL_CCITT_16,
            .crc_seed          = 0,
        },

        .frame_codec = E_FRAME_CODEC_NONE,
    },
#endif
#if RADIO_PHY_1_SUPPORT
    {
        .pkt_type          = PKT_VARIABLE_LENGTH,
        .max_pkt_length    = RADIO_MAX_FRAME_LEN,
        .frame_format      = (FRAME_FMT_PREAMBLE_EN |
                              FRAME_FMT_SYNCWORD_EN |
                              FRAME_FMT_HEADER_EN |
                              /* FRAME_FMT_WHITENING_EN | */
                              CONFIG_FRAME_FMT_CRC),
        .phr =
        {
            .hdr_config        = 0,
            .hdr_bytes         = 1, //
            
            .length_bit_size   = 8, // 8~12 bits
            .length_bit_offset = 0, // bit offset in header
        },

        .crc =
        {
            .crc_config        = 0,
            .crc_poly_sel      = CRC_POLYNOMIAL_CCITT_16,
            .crc_seed          = 0,
        },

        .frame_codec = E_FRAME_CODEC_NONE,
    }
#endif
};

/*========== WMBUS testing config ==========*/
// Common WMBUS packet format
static const WISE_RADIO_PKT_FMT_T wmbus_radio_pkt_fmt = {
    .pkt_type          = PKT_FIXED_LENGTH,
    .max_pkt_length    = RADIO_MAX_FRAME_LEN,
    .frame_format      = (FRAME_FMT_PREAMBLE_EN |
                          FRAME_FMT_SYNCWORD_EN
                          /*FRAME_FMT_HEADER_EN |*/
                          /*FRAME_FMT_WHITENING_EN |*/
                          /*FRAME_FMT_FEC_EN | */
                          /*FRAME_FMT_MANCH_EN |*/
                          /*CONFIG_FRAME_FMT_CRC*/
                          ),

    .phr =
    {
        .hdr_config        = PHR_LENGTH_INCLUDE_CRC,
        .hdr_bytes         = 1, //
        .length_bit_size   = 8, // 8~12 bits
        .length_bit_offset = 0, // bit offset in header
    },

    .crc =
    {

        .crc_config        = CRC_INPUT_BIT_ENDIAN_LSB_FIRST |
                             CRC_OUTPUT_BIT_ENDIAN_LSB_FIRST |
                             CRC_OUTPUT_BYTE_ENDIAN_LSB_FIRST,

        .crc_poly_sel      = CRC_POLYNOMIAL_CCITT_16,
        .crc_seed          = 0,
    },
};
/*====================*/

void radioDebug(void)
{
    //uint32_t freq                   = 0;
    const WISE_RADIO_CFG_T *cfg     = &radioCfg[defRadioIntf];
    const WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    fmt_radio_debug(defRadioIntf, actChannel[defRadioIntf], rxStarted, cfg, fmt);

#if 0 //kevin yang, 20251118, moved to fmt_print as a util function
    wise_radio_get_channel_freq(defRadioIntf, actChannel[defRadioIntf], &freq);

    /* Board */
    dbg_box_begin("Board INFO");
    dbg_field("PA type", paString[wise_sys_get_pa_type()]);
    dbg_field("Matching type", matchString[wise_sys_get_board_match_type()]);
    dbg_box_end(); // Board

    /* RADIO */
    dbg_box_begin("RADIO STATUS");
    /* RF-PHY */
    dbg_section("PHY Setting");
    dbg_field("Operate Mode", operateString[cfg->phy_mode]);
    dbg_field("Modulation", modString[cfg->modulation]);
    dbg_field("Base Freq", fmt_hz_u32(cfg->ch_freq_min));
    dbg_field("Channel Num", fmt_int(wise_radio_get_channel_num(defRadioIntf)));
    dbg_field("Chan Spacing", fmt_hz_u32(cfg->ch_spacing));
    dbg_field("Deviation", fmt_hz_u32(cfg->deviation));
    dbg_field("Data Rate", drString[cfg->data_rate]);
    /* RF-SYNC */
    dbg_section("Sync Format");
    dbg_field("Preamble Len", fmt_bytes_u32(cfg->preamble_len));
    dbg_field("Preamble Pat", fmt_hex8(cfg->preamble));
    dbg_field("Sync Len", fmt_bytes_u32(cfg->sync_word_len));
    dbg_field("Sync Word", fmt_hex32(cfg->syncword));
    /* RF-PACKET */
    dbg_section("Packet Format");
    dbg_field("Length Config", fmtLenTypeString[fmt->pkt_type]);
    dbg_field("MAX Frame Len", fmt_bytes_u32(fmt->max_pkt_length));
    dbg_field("HW Preamble", onOffString[!!(fmt->frame_format & FRAME_FMT_PREAMBLE_EN)]);
    dbg_field("HW Sync", onOffString[!!(fmt->frame_format & FRAME_FMT_SYNCWORD_EN)]);
    dbg_field("Whitening", onOffString[!!(fmt->frame_format & FRAME_FMT_WHITENING_EN)]);
    dbg_field("FEC", onOffString[!!(fmt->frame_format & FRAME_FMT_FEC_EN)]);
    dbg_field("Manchester", onOffString[!!(fmt->frame_format & FRAME_FMT_MANCH_EN)]);
    dbg_field("Bit order", (fmt->frame_format & FRAME_FMT_MSB_FIRST) ? "MSB first" : "LSB first");
    dbg_field("HW CRC", onOffString[!!(fmt->frame_format & FRAME_FMT_CRC_EN)]);
    if(!!(fmt->frame_format & FRAME_FMT_CRC_EN)) {
        dbg_section("CRC SETTING");
        uint32_t cfg = radio_get_crc_config();
        dbg_field("Input bit order",(cfg  & CRC_INPUT_BIT_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
        dbg_field("Output bit order",(cfg  & CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
        dbg_field("Output byte order",(cfg  & CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
        dbg_field("CRC includes header",onOffString[!!(cfg  & CRC_INCLUDE_HEADER_ON)]);
        dbg_field("Invert",onOffString[!!(cfg & CRC_INVERT_ON) ]);
        extern uint16_t util_crc16_get_poly(uint8_t crcType);
        dbg_field("poly",fmt_hex16(util_crc16_get_poly(radio_get_crc_poly())));
        dbg_field("seed",fmt_hex16(radio_get_crc_seed()));
    }
    /* RF-RUNTIME */
    dbg_section("Runtime State");
    dbg_field("Current Ch", fmt_int(actChannel[defRadioIntf]));
    dbg_field("Current Freq", fmt_hz_u32(freq));
    dbg_field("RX RECV", onOffString[rxStarted]);
    dbg_box_end(); // RADIO
#endif
}
void radioInit(void)
{
    if (!radioInited[defRadioIntf]) {
        init_rand(wise_tick_get_counter());
        wise_radio_init(defRadioIntf);
        wise_radio_set_evt_callback(defRadioIntf, radioEventCb);
        wise_radio_set_buffer(defRadioIntf, &radioBuffer);
        wise_radio_config(defRadioIntf, &radioCfg[defRadioIntf], &radioPktFmt[defRadioIntf]);
        wise_radio_set_tx_pwr(defRadioIntf, txPwrLevel[defRadioIntf]);
        wise_radio_set_tx_io_mode(defRadioIntf, CORE_IO_BLOCKING);

        radioInited[defRadioIntf] = 1;
    }

    radioMode[defRadioIntf]       = 0;
    radioConfigured[defRadioIntf] = 1;

#if CONFIG_DEBUG_TOOL
    if (_wise_radio_get_phy_mode(defRadioIntf) == E_PHY_MBUS) {
        wise_fs_user_data_read(0x0, radioPktFmt[defRadioIntf].max_pkt_length, rx_tp_buf);
        dump_buffer(rx_tp_buf, radioPktFmt[defRadioIntf].max_pkt_length);
    }
#endif

    radioDebug();
}

void radioDeinit(void)
{
    printf("radio deinit\n");
    radioInited[defRadioIntf] = 0;
    radioMode[defRadioIntf]       = 0;
    radioConfigured[defRadioIntf] = 0;

    wise_radio_deinit(defRadioIntf);
}

void radioInit802154(void)
{
    WISE_RADIO_CFG_T *cfg = &radioCfg[defRadioIntf];

    cfg->syncword      = 0xC803DAE5;
    cfg->sync_word_len = 4;
    //cfg->phy_mode      = E_PHY_802154;

    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    fmt->pkt_type = PKT_VARIABLE_LENGTH, fmt->max_pkt_length = 127;
    fmt->phr.hdr_bytes         = 1;
    fmt->phr.length_bit_size   = 8;
    fmt->phr.length_bit_offset = 0;
    fmt->frame_format          = (FRAME_FMT_PREAMBLE_EN | FRAME_FMT_SYNCWORD_EN | FRAME_FMT_HEADER_EN | FRAME_FMT_WHITENING_EN | FRAME_FMT_CRC_EN);

    if (!radioInited[defRadioIntf]) {
        init_rand(wise_tick_get_counter());

        wise_radio_ieee802154_init(defRadioIntf);
        _wise_radio_set_phy_mode(defRadioIntf, E_PHY_802154);
        wise_radio_set_evt_callback(defRadioIntf, radioEventCb);

        wise_radio_set_buffer(defRadioIntf, &radioBuffer);
        wise_radio_config(defRadioIntf, cfg, fmt);
        wise_radio_set_tx_pwr(defRadioIntf, txPwrLevel[defRadioIntf]);
        wise_radio_set_tx_io_mode(defRadioIntf, CORE_IO_BLOCKING);

        radioInited[defRadioIntf] = 1;
    }

    radioMode[defRadioIntf]       = 1;
    radioConfigured[defRadioIntf] = 1;

    radioDebug();
}

void radioInitBleMode(void)
{
    wise_radio_init(defRadioIntf);
    wise_radio_set_evt_callback(defRadioIntf, radioEventCb);
    wise_radio_set_buffer(defRadioIntf, &radioBuffer);

    wise_radio_ble_config(defRadioIntf, &radioCfg[defRadioIntf], &radioPktFmt[defRadioIntf]);
    
    wise_radio_set_tx_pwr(defRadioIntf, txPwrLevel[defRadioIntf]);
    wise_radio_set_tx_io_mode(defRadioIntf, CORE_IO_BLOCKING);

    radioMode[defRadioIntf]       = 2;
    radioConfigured[defRadioIntf] = 1;

    radioDebug();
}

// Generic WMBUS configuration setup function
static void _wmbus_init_gen_cfg(uint8_t mode, uint8_t role, uint8_t act, const WMBUS_MODE_CONFIG_T *config)
{
    const WISE_RADIO_CFG_T *radio_cfg = NULL;
    uint16_t max_pkt_length = 0;

    // Select configuration based on role and activity
    if (role == 1 && act == 0) { 
        // meter rx
        radio_cfg = &config->meter_rx;
        max_pkt_length = config->o2m_pat_len;
    }
    else if (role == 1 && act == 1) { 
        // meter tx
        radio_cfg = &config->meter_tx;
        max_pkt_length = config->m2o_pat_len;
    }
    else if (role == 0 && act == 0) { 
        // other rx
        radio_cfg = &config->other_rx;
        max_pkt_length = config->m2o_pat_len;
    }
    else if (role == 0 && act == 1) { 
        // other tx
        radio_cfg = &config->other_tx;
        max_pkt_length = config->o2m_pat_len;
    }

    // Copy configurations
    memcpy(&radioCfg[defRadioIntf], radio_cfg, sizeof(WISE_RADIO_CFG_T));
    memcpy(&radioPktFmt[defRadioIntf], &wmbus_radio_pkt_fmt, sizeof(WISE_RADIO_PKT_FMT_T));
    //radioCfg[defRadioIntf].wmbus_mode = mode;
    radioPktFmt[defRadioIntf].max_pkt_length = max_pkt_length;

    // Record into wmbus_radio_info
    wmbus_radio_info.mode = mode;
    wmbus_radio_info.role = role;
    wmbus_radio_info.activity = act;
    wmbus_radio_info.wmbus_radio_cfg = &radioCfg[defRadioIntf];
    wmbus_radio_info.wmbus_radio_pkt_fmt = &radioPktFmt[defRadioIntf];
    memcpy(wmbus_radio_info.m2o_pat, config->m2o_pat, config->m2o_pat_len);
    wmbus_radio_info.m2o_pat_len = config->m2o_pat_len;
    memcpy(wmbus_radio_info.o2m_pat, config->o2m_pat, config->o2m_pat_len);
    wmbus_radio_info.o2m_pat_len = config->o2m_pat_len;
    wmbus_radio_info.wmbus_is_testing = 1;
}

void _wmbus_init_gen_s_mode_cfg(uint8_t role, uint8_t act)
{
    // WMBUS Mode S configuration
    static const WMBUS_MODE_CONFIG_T wmbus_s_config = {
        .meter_tx = {
            .ch_freq_min = 868300000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, 
            .data_rate = E_DATA_RATE_32P768K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .meter_rx = {
            .ch_freq_min = 868300000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, 
            .data_rate = E_DATA_RATE_32P768K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .other_tx = {
            .ch_freq_min = 868300000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, 
            .data_rate = E_DATA_RATE_32P768K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .other_rx = {
            .ch_freq_min = 868300000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, 
            .data_rate = E_DATA_RATE_32P768K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .m2o_pat = (uint8_t *)wmbus_raw_data_40bytes,
        .m2o_pat_len = sizeof(wmbus_raw_data_40bytes),
        .o2m_pat = (uint8_t *)wmbus_raw_data_40bytes,
        .o2m_pat_len = sizeof(wmbus_raw_data_40bytes),
    };

    _wmbus_init_gen_cfg(WMBUS_MODE_S, role, act, &wmbus_s_config);
}


void _wmbus_init_gen_t_mode_cfg(uint8_t role, uint8_t act)
{
    // WMBUS Mode T configuration
    static const WMBUS_MODE_CONFIG_T wmbus_t_config = {
        .meter_tx = {
            .ch_freq_min = 868950000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, 
            .data_rate = E_DATA_RATE_100K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x543D,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .meter_rx = {
            .ch_freq_min = 868300000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, 
            .data_rate = E_DATA_RATE_32P768K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .other_tx = {
            .ch_freq_min = 868300000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, 
            .data_rate = E_DATA_RATE_32P768K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .other_rx = {
            .ch_freq_min = 868950000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, 
            .data_rate = E_DATA_RATE_100K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x543D,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .m2o_pat = (uint8_t *)wmbus_raw_data_30bytes,
        .m2o_pat_len = sizeof(wmbus_raw_data_30bytes),
        .o2m_pat = (uint8_t *)wmbus_raw_data_40_bytes_manchester,
        .o2m_pat_len = sizeof(wmbus_raw_data_40_bytes_manchester),
    };

    _wmbus_init_gen_cfg(WMBUS_MODE_T, role, act, &wmbus_t_config);
}

void _wmbus_init_gen_c_mode_cfg(uint8_t role, uint8_t act)
{
    // WMBUS Mode C configuration
    static const WMBUS_MODE_CONFIG_T wmbus_c_config = {
        .meter_tx = {
            .ch_freq_min = 868950000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, //spec = 45000, sg = 50000
            .data_rate = E_DATA_RATE_100K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 4, 
            .syncword = 0x55543D54,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .meter_rx = {
            .ch_freq_min = 869525000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 25000, 
            .data_rate = E_DATA_RATE_50K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 4, 
            .syncword = 0x55543D54,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_GFSK,
        },
        .other_tx = {
            .ch_freq_min = 869525000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 25000, 
            .data_rate = E_DATA_RATE_50K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 4, 
            .syncword = 0x55543D54,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_GFSK,
        },
        .other_rx = {
            .ch_freq_min = 868950000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 50000, //spec = 45000, sg = 50000
            .data_rate = E_DATA_RATE_100K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 4, 
            .syncword = 0x55543D54,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .m2o_pat = (uint8_t *)wmbus_raw_data_20bytes,
        .m2o_pat_len = sizeof(wmbus_raw_data_20bytes),
        .o2m_pat = (uint8_t *)wmbus_raw_data_20bytes,
        .o2m_pat_len = sizeof(wmbus_raw_data_20bytes),
    };

    _wmbus_init_gen_cfg(WMBUS_MODE_C, role, act, &wmbus_c_config);
}


void _wmbus_init_gen_r_mode_cfg(uint8_t role, uint8_t act)
{
    // WMBUS Mode R configuration
    static const WMBUS_MODE_CONFIG_T wmbus_r_config = {
        .meter_tx = {
            .ch_freq_min = 868030000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 6000, 
            .data_rate = E_DATA_RATE_4P8K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .meter_rx = {
            .ch_freq_min = 868330000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 6000, 
            .data_rate = E_DATA_RATE_4P8K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .other_tx = {
            .ch_freq_min = 868330000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 6000, 
            .data_rate = E_DATA_RATE_4P8K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .other_rx = {
            .ch_freq_min = 868030000, 
            .ch_freq_max = 870000000, 
            .ch_spacing = 200000,
            .deviation = 6000, 
            .data_rate = E_DATA_RATE_4P8K,
            .preamble = 0xAA, 
            .preamble_len = 4,
            .sync_word_len = 2, 
            .syncword = 0x7696,
            //.phy_mode = E_PHY_MBUS, 
            .modulation = E_MOD_TYPE_FSK,
        },
        .m2o_pat = (uint8_t *)wmbus_raw_data_40_bytes_manchester,
        .m2o_pat_len = sizeof(wmbus_raw_data_40_bytes_manchester),
        .o2m_pat = (uint8_t *)wmbus_raw_data_40_bytes_manchester,
        .o2m_pat_len = sizeof(wmbus_raw_data_40_bytes_manchester),
    };

    _wmbus_init_gen_cfg(WMBUS_MODE_R, role, act, &wmbus_r_config);
}

void radioInitWMBUS(uint8_t mode, uint8_t role, uint8_t act)
{
    if (mode == WMBUS_MODE_S1 || mode == WMBUS_MODE_S2 || mode == WMBUS_MODE_S1M)
        _wmbus_init_gen_s_mode_cfg(role, act);
    if (mode == WMBUS_MODE_T1 || mode == WMBUS_MODE_T2)
        _wmbus_init_gen_t_mode_cfg(role, act);
    if (mode == WMBUS_MODE_C1 || mode == WMBUS_MODE_C2)
        _wmbus_init_gen_c_mode_cfg(role, act);
    if (mode == WMBUS_MODE_R2)
        _wmbus_init_gen_r_mode_cfg(role, act);
    
    debug_print("set WMBUS mode = [%s], role = [%s], activity = [%s]\n", 
                                wmbus_modeString[wmbus_radio_info.mode], 
                                wmbus_roleString[wmbus_radio_info.role], 
                                wmbus_actString[wmbus_radio_info.activity]);

    wise_radio_init(defRadioIntf);
    wise_radio_set_evt_callback(defRadioIntf, radioEventCb);
    wise_radio_set_buffer(defRadioIntf, &radioBuffer);
    _wise_radio_set_phy_mode(defRadioIntf, E_PHY_MBUS);
    _wmbus_set_mode(defRadioIntf, mode); //kevin yang, 20251129, wmbus_mode is already removed from WISE_RADIO_CFG_T
    wise_radio_config(defRadioIntf, &radioCfg[defRadioIntf], &radioPktFmt[defRadioIntf]);
    
    wise_radio_set_tx_pwr(defRadioIntf, txPwrLevel[defRadioIntf]);
    wise_radio_set_tx_io_mode(defRadioIntf, CORE_IO_BLOCKING);

    radioMode[defRadioIntf] = 3;
    radioConfigured[defRadioIntf] = 1;

    radioDebug();
}

#if CONFIG_DEBUG_TOOL
int32_t issueEvent(uint8_t evtType, uint8_t *evtData, uint16_t dataLen)
{
    uint32_t bufAddr = 0, bufLen = 0;
    uint8_t *cmd;

    wise_pkt_get_output_buf(&bufAddr, &bufLen);
    if (!bufAddr || !bufLen) {
        return WISE_FAIL;
    }

    if (dataLen > bufLen) {
        return WISE_FAIL;
    }

    cmd = (uint8_t *)bufAddr;

    cmd[0] = WISE_CMD_TYPE_EVENT;
    cmd[1] = evtType;
    memcpy(&cmd[2], evtData, dataLen);

    wise_pkt_start_output(dataLen + 2, PKT_OUTPUT_NO_WAIT);

    return WISE_SUCCESS;
}

void _fill_mac_data(uint8_t *macEvtData, uint16_t *macDataLen)
{
    rx_info.ib_pwr   = (REG_R32(BBP_AGC23_ADDR) & BBP_AGC_INBANDPWR_DBM_MASK) >> BBP_AGC_INBANDPWR_DBM_POS;
    rx_info.tot_pwr  = (REG_R32(BBP_AGC23_ADDR) & BBP_AGC_TOTPWR_DBM_MASK) >> BBP_AGC_TOTPWR_DBM_POS;
    rx_info.rssi     = (REG_R32(BBP_AGC42_ADDR) & BBP_AGC_INBAND_RSSI_MASK) >> BBP_AGC_INBAND_RSSI_POS;
    rx_info.gain_idx = (REG_R32(BBP_AGC_MODE_ADDR) & BBP_AGC_IDX_CUR_DEBUG_MASK) >> BBP_AGC_IDX_CUR_DEBUG_POS;
    rx_info.gfo_est  = REG_R32(BBP_GFO_EST_ADDR);

    uint8_t *ptr = macEvtData;
    PUT_BE_16(ptr, rx_info.ib_pwr);
    ptr += 2;
    PUT_BE_16(ptr, rx_info.tot_pwr);
    ptr += 2;
    PUT_BE_16(ptr, rx_info.rssi);
    ptr += 2;
    PUT_BE_16(ptr, rx_info.gain_idx);
    ptr += 2;
    PUT_BE_32(ptr, rx_info.gfo_est);
    ptr += 4;

    *ptr++ = rx_info.rx_ok;
    *ptr++ = rx_info.rx_err;
    PUT_BE_16(ptr, rx_info.rx_sts);
    ptr += 2;
    *ptr++ = rx_info.int_sts;

    uint8_t len = ptr - macEvtData;

//    uint32_t mode       = LOAD_LE_32((uint32_t *)&rx_para.data[0]);
//    uint32_t rx_len     = LOAD_LE_32((uint32_t *)&rx_para.data[4]);
//    uint32_t rx_pkt_out = LOAD_LE_32((uint32_t *)&rx_para.data[8]);

//    if (rx_pkt_out) {
//        if (mode != MAC_RXEN_TRN_MASK) {
//            rx_len = (REG_R32(MAC_RX_STS_ADDR) & MAC_RXLEN_MASK) >> MAC_RXLEN_POS;
//        }
//#if defined(TARGET_SOC_RADIO)
//        memcpy((void *)&macEvtData[14], (void *)&rx_pkt.data[0], rx_len);
//#elif defined(TARGET_EXTERNAL_RADIO)
//        uint8_t pkt_buf[PKT_MAX_PAYLOAD_LEN];
//        _radio_get_rx_fifo(&pkt_buf[0], rx_len);
//        memcpy((void *)&macEvtData[14], (void *)&pkt_buf[0], rx_len);
//#endif
//        len += rx_len;
//    }

    *macDataLen = len;
//    radio_ctrl_rx_start(mode, rx_len);
}

void wise_ctrl_evt_mac_interrupt()
{
    uint8_t macEvtData[PKT_MAX_PAYLOAD_LEN];
    uint16_t macDataLen = 0;

    memset(macEvtData, 0, PKT_MAX_PAYLOAD_LEN);

    _fill_mac_data(macEvtData, &macDataLen);
    if (macDataLen) {
        issueEvent(EVT_MAC_INTERRUPT, macEvtData, macDataLen);
    }
//#ifdef TARGET_EXTERNAL_RADIO
//    _radio_reset_rx_fifo();
//#endif
}

#endif

int8_t _debug_tool_show_info(WISE_RADIO_EVT_T evt, uint32_t *buf_addr)
{
    WISE_STATUS status = WISE_FAIL;
    
#if CONFIG_DEBUG_TOOL
    if (evt & WISE_RADIO_EVT_RX_FRAME) {

        if (_wise_radio_get_phy_mode(defRadioIntf) == E_PHY_MBUS) {
            int32_t result = memcmp((void *)buf_addr, &rx_tp_buf, radioPktFmt[defRadioIntf].max_pkt_length);
            if (result == 0) {
                rx_info.rx_ok = 1;
                rx_info.rx_err = 0;
            } else {
                rx_info.rx_ok = 0;
                rx_info.rx_err = 1;
            }
//          debug_print("[cmp]result = %ld\n", result);
//          debug_print("[Good]recv len %ld buf=%08lx\n", radioPktFmt[defRadioIntf].max_pkt_length, (uint32_t)buf_addr);
//          debug_print("Received Packet::\n");
//          dump_buffer((unsigned char *)buf_addr, radioPktFmt[defRadioIntf].max_pkt_length);
//          debug_print("RX Pattern::\n");
//          dump_buffer(rx_tp_buf, radioPktFmt[defRadioIntf].max_pkt_length);
        } else {
    //      debug_print("[Good]recv len %d buf=%08lx\n", meta.data_len, bufAddr);
            rx_info.rx_ok = 1;
            rx_info.rx_err = 0;
        }
    }

    if (evt & WISE_RADIO_EVT_RX_ERR) {
        rx_info.rx_ok = 0;
        rx_info.rx_err = 1;
    }
    
    status = WISE_SUCCESS;
#endif

    return status;
}

int8_t _wmbus_test_show_info(WISE_RADIO_EVT_T evt, uint32_t *buf_addr)
{
    WISE_STATUS status = WISE_FAIL;
    uint32_t result = 1;

    if (wmbus_radio_info.wmbus_is_testing) {
        if (evt & WISE_RADIO_EVT_RX_FRAME) {
            if (wmbus_radio_info.role == ROLE_METER)
                result = memcmp((void *)buf_addr, wmbus_radio_info.o2m_pat, wmbus_radio_info.wmbus_radio_pkt_fmt->max_pkt_length);
            if (wmbus_radio_info.role == ROLE_OTHER)
                result = memcmp((void *)buf_addr, wmbus_radio_info.m2o_pat, wmbus_radio_info.wmbus_radio_pkt_fmt->max_pkt_length);
            
            if (result == 0) {
                rxGoodCount++;
            } else {
                rxErrCount++;
            }
        }

       debug_print("[Good]recv len %ld buf=%08lx\n", wmbus_radio_info.wmbus_radio_pkt_fmt->max_pkt_length, (uint32_t)buf_addr);
       debug_print("Received Packet::\n");
       dump_buffer((unsigned char *)buf_addr, wmbus_radio_info.wmbus_radio_pkt_fmt->max_pkt_length);
       status = WISE_SUCCESS;
    }    

    return status;
}

#if 0
void radioEventCb(WISE_RADIO_EVT_T evt)
{
	rxTotalCount++;
    if (evt & WISE_RADIO_EVT_RX_FRAME) {
        WISE_RX_META_T meta;
        uint32_t bufAddr = 0;
        uint8_t show_cnt = 0;
                 
        if (WISE_SUCCESS == wise_radio_get_rx_frame_info(defRadioIntf, &bufAddr, &meta)) {
            if(WISE_SUCCESS == _debug_tool_show_info(evt, (uint32_t *)bufAddr)) {
                show_cnt = 0;
            } else if (WISE_SUCCESS == _wmbus_test_show_info(evt, (uint32_t *)bufAddr)) {
                show_cnt = 1;
            } else {
                show_cnt = 1;
                rxGoodCount++;
            }

            if (show_cnt) {
                debug_print("Good/Err CNT = %ld/%ld\n", rxGoodCount, rxErrCount);
                debug_print("recv len %d buf=%08lx rssi=%d\n", meta.data_len, bufAddr, meta.rssi);\
            }
        } else {
            debug_print("rx read buffer fail\n");
        }

        wise_radio_release_rx_frame(defRadioIntf);

        if (rxLog == 1) {
            debug_print("timestamp=%ld\n", meta.timestamp);
            dump_buffer((unsigned char *)bufAddr, meta.data_len);
        }
    }

    if (evt & WISE_RADIO_EVT_RX_ERR) {
        WISE_RX_META_T meta;
        uint32_t bufAddr = 0;

        if (WISE_SUCCESS != wise_radio_get_rx_frame_info(defRadioIntf, &bufAddr, &meta)) {
            debug_print("rx read buffer fail\n");
        }

        rxErrCount++;
        
        wise_radio_release_rx_frame(defRadioIntf);

        if(WISE_SUCCESS == _debug_tool_show_info(evt, &bufAddr)) {
            // debug_print("debug tool show info \n");
        } else {
            debug_print("Good/Err CNT = %ld/%ld\n", rxGoodCount, rxErrCount);
            debug_print("E F\n");
            debug_print("[Err]recv len %d buf=%08lx\n", meta.data_len, bufAddr);
        }


        if (rxLog == 1) {
            debug_print("timestamp=%ld\n", meta.timestamp);
            debug_print("[Err]recv len %d buf=%08lx\n", meta.data_len, bufAddr);
            dump_buffer((unsigned char *)bufAddr, meta.data_len);
        }
    }
    
#if CONFIG_DEBUG_TOOL
    wise_ctrl_evt_mac_interrupt();
#endif

    _wise_radio_get_rx_info_output(defRadioIntf);
}
#else
#define SHOW_EXTRA_INFO  0

static uint32_t s_last_ts_40m = 0;
static uint8_t  s_ts_inited   = 0;


static void _ticks_to_ms_3(uint32_t ticks, uint32_t *ms, uint32_t *ms_frac3)
{
    uint32_t whole = ticks / 40000U;
    uint32_t rem   = ticks % 40000U;
    uint32_t frac  = (rem * 1000U + 20000U) / 40000U;
    if (frac == 1000U) { whole += 1U; frac = 0U; }
    *ms = whole;
    *ms_frac3 = frac;
}

static void rx_log_header(uint32_t ts_40m, uint16_t len, int rssi_dbm,
                          int crc_ok, uint32_t good_cnt, uint32_t err_cnt, uint32_t total_cnt)
{
    uint32_t t_ms_i, t_ms_f;
    _ticks_to_ms_3(ts_40m, &t_ms_i, &t_ms_f);

    uint32_t dt_ticks = s_ts_inited ? (uint32_t)(ts_40m - s_last_ts_40m) : 0U;
    uint32_t dt_ms_i, dt_ms_f;
    _ticks_to_ms_3(dt_ticks, &dt_ms_i, &dt_ms_f);

    s_last_ts_40m = ts_40m;
    s_ts_inited   = 1;

    debug_print("[%lu.%03lu ms | +%lu.%03lu ms] len:%u rssi:%d %s | G/E/T:%lu/%lu/%lu\n",
                (unsigned long)t_ms_i, (unsigned long)t_ms_f,
                (unsigned long)dt_ms_i, (unsigned long)dt_ms_f,
                (unsigned)len, rssi_dbm, crc_ok ? "CRC:OK" : "CRC:ERR",
                (unsigned long)good_cnt, (unsigned long)err_cnt, (unsigned long)total_cnt);
}

static void rx_log_data(const uint8_t *data, uint16_t len, uint32_t bufptr, int verbose)
{
    debug_print("data:\n");
    dump_buffer((unsigned char *)data, (int)len);

#if SHOW_EXTRA_INFO
    if (verbose)
        debug_print("info: buf=0x%08lX\n", (unsigned long)bufptr);
#endif
}

void radioEventCb(WISE_RADIO_EVT_T evt)
{
    WISE_RX_META_T meta;
    uint32_t bufAddr = 0;
    bool isFrameOK = false;

    if (evt & WISE_RADIO_EVT_RX_FRAME || evt & WISE_RADIO_EVT_RX_ERR) {
        if (WISE_SUCCESS != wise_radio_get_rx_frame_info(defRadioIntf, &bufAddr, &meta)) {
            debug_print("rx read buffer fail\n");
            return;
        }

        isFrameOK = (evt & WISE_RADIO_EVT_RX_FRAME) && !(!!(evt & WISE_RADIO_EVT_RX_ERR));

        // uint8_t show_cnt = 0;
        // if (isFrameOK) {
        //     if (WISE_SUCCESS == _debug_tool_show_info(evt, (uint32_t *)bufAddr)) {
        //         show_cnt = 0;
        //     } else if (WISE_SUCCESS == _wmbus_test_show_info(evt, (uint32_t *)bufAddr)) {
        //         show_cnt = 1;
        //     } else {
        //         show_cnt = 1;
        //     }
        // }

        rxTotalCount++;
        
        if (isFrameOK) {
            rxGoodCount++;
#if CONFIG_DEBUG_TOOL && (ESMT_SOC_CHIP_ID == 0x9006)
            rx_info.rx_ok = 1;
            rx_info.rx_err = 0;
#endif
        } 
        else {
            rxErrCount++;
            
#if CONFIG_DEBUG_TOOL && (ESMT_SOC_CHIP_ID == 0x9006)
            rx_info.rx_ok = 0;
            rx_info.rx_err = 1;
#endif
        }

        rx_log_header(meta.timestamp, meta.data_len, meta.rssi, isFrameOK,
                      rxGoodCount, rxErrCount, rxTotalCount);

        if (rxLog == 1/* && (show_cnt || !isFrameOK)*/) {
            rx_log_data((const uint8_t *)bufAddr, meta.data_len, bufAddr, SHOW_EXTRA_INFO);
        }

        wise_radio_release_rx_frame(defRadioIntf);
#if CONFIG_DEBUG_TOOL
    wise_ctrl_evt_mac_interrupt();
#endif
    }

    _wise_radio_get_rx_info_output(defRadioIntf);
}
#endif

void radioRxOn(void)
{
    rxGoodCount = 0;
    rxErrCount  = 0;
    rxTotalCount = 0;
    wise_radio_set_rx_mode(defRadioIntf, RADIO_RX_CONTINUOUS);
    wise_radio_rx_start(defRadioIntf, actChannel[defRadioIntf]);
    rxStarted = 1;
}

void radioRxOff(void)
{
    wise_radio_rx_stop(defRadioIntf);
    rxStarted = 0;
}

void clearRadioConfigured(void)
{
    radioConfigured[defRadioIntf] = 0;
    radioInited[defRadioIntf] = 0;
    debug_print("To apply the updated parameter, please re-initialize RF.\n");
}

uint8_t isRadioConfigured(void)
{
    return radioConfigured[defRadioIntf];
}

uint8_t isRxOn(void)
{
    return rxStarted;
}

void periodicTxProc(uint32_t _testCount)
{
    uint32_t testNum           = _testCount;
    static uint32_t testCount  = 0;
    int testLen                = 0;
    static uint8_t testPattern = 0;
    uint8_t maxLen             = 240;

    while (testCount != testNum) {
        if (testCount == 0) {
            testPattern = 0;
        }

        debug_print("test cnt=%ld \n", testCount);

        if (radioMode[defRadioIntf] == 1) {
            maxLen = 120;
        }

        while (testLen == 0) {
            testLen = rand_cmwc() % maxLen;
        }

        radioTestTxFrame(testLen, testPattern);
        wise_tick_delay_us(20000);
        testPattern += testLen;
        testCount++;
    }

    testCount = 0;
}

uint8_t isTxProcEnd(void)
{
    return periodicTxSchId < 0 ? 1 : 0;
}

void radioTestSend(int testCount)
{
    if (testCount <= 1) {
        radioTestTxFrame(25, 0);
    } else {
        periodicTxProc(testCount);
    }
}

int radio_tx_set_fix_pattern(uint8_t mod_type)
{
    uint8_t *pattern;
    uint32_t pattern_len = 0;

    uint8_t data_cnt_gfsk_10bytes[] = {
#ifdef IEEE802154_LONG_ADDR
        0x41, 0xCC,
#else
        // 0x1B, // PHR, will be replaced by HW PHR if enabled
        0x41, 0xC8, // FCF: PANID Compression=1, Dest Addr mode=0x10(16 bit),
        // Sour Addr mode=0x11(64 bit)
#endif
        0x00,
        // Dest PAN ID, Dest Addr (0xffff is defined to be broadcast)
        radioPANID & 0xFF, (radioPANID >> 8) & 0xFF,
#ifdef IEEE802154_LONG_ADDR
        //        0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
        radioLonfAddr[0], radioLonfAddr[1], radioLonfAddr[2], radioLonfAddr[3],
        radioLonfAddr[4], radioLonfAddr[5], radioLonfAddr[6], radioLonfAddr[7],
#else
        radioShortAddr & 0xFF, (radioShortAddr >> 8) & 0xFF,
#endif
        0x02, 0x00, 0x00, 0xAB, 0xAA, 0x00, 0x00, 0x00, // Sour Addr
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
        // 0x62, 0xF2 // FCS, will be replaced by HW FCS if enabled
    };

    uint8_t data_cnt_bpsk_10bytes[] = {
        0x1b, 0x9f, 0x5c, 0xf2, 0x58, 0x5f, 0xbc, 0x80, 0x59, 0x1f, 0xc9,
        0xae, 0xc0, 0xbd, 0x4a, 0xf8, 0x6a, 0xec, 0xd5, 0x63, 0x58, 0x60,
        0x13, 0x46, 0xcf, 0x26, 0xb8, 0xb1, 0x00, 0x00, 0x00, 0x00
    };

    if (mod_type == E_MOD_TYPE_GFSK) {
        pattern     = &data_cnt_gfsk_10bytes[0];
        pattern_len = sizeof(data_cnt_gfsk_10bytes);
    }

    if (mod_type == E_MOD_TYPE_BPSK) {
        pattern     = &data_cnt_bpsk_10bytes[0];
        pattern_len = sizeof(data_cnt_bpsk_10bytes);
    }

    if (mod_type == E_MOD_TYPE_OOK) {
        // need to add OOK tx pattern
        pattern     = &data_cnt_gfsk_10bytes[0];
        pattern_len = sizeof(data_cnt_gfsk_10bytes);
    }

    memcpy(radioTestBuf, pattern, pattern_len);

    return pattern_len;
}

void radioTestTxFrame(int testTxLen, uint8_t patternStart)
{
    int i;

    if (radioMode[defRadioIntf] == 0) {
        radioTestBuf[0] = testTxLen; // bit_reverse(testTxLen & 0xff, 1);

        for (i = 0; i < testTxLen; i++) {
            radioTestBuf[i + 1] = patternStart++ & 0xff;
        }

#ifdef EN_TX_HW_CRC
        testTxLen++;
#else
        uint16_t crcCalc = 0;
        testTxLen++;
        crcCalc = crc16_kermit(&radioTestBuf[0], testTxLen, 0);

        radioTestBuf[testTxLen]      = crcCalc & 0xff;
        radioTestBuf[testTxLen + 1]  = (crcCalc >> 8) & 0xff;
        testTxLen                   += 2;
#endif

    }else if (radioMode[defRadioIntf] == 3) {
        if (wmbus_radio_info.role == ROLE_OTHER) {
           memcpy(radioTestBuf, wmbus_radio_info.o2m_pat, wmbus_radio_info.o2m_pat_len);
           testTxLen = wmbus_radio_info.o2m_pat_len;
        }

        if (wmbus_radio_info.role == ROLE_METER) {
           memcpy(radioTestBuf, wmbus_radio_info.m2o_pat, wmbus_radio_info.m2o_pat_len);
           testTxLen = wmbus_radio_info.m2o_pat_len;
        }

    } else {
        radioTestBuf[0] = 0;
        radioTestBuf[1] = 0;

        for (i = 2; i < testTxLen; i++) {
            radioTestBuf[i] = patternStart++ & 0xff;
        }
    }

    if (_wise_radio_get_sg_test_mode()) {
        //SG mode setting
        
        testTxLen = radio_tx_set_fix_pattern(radioCfg[defRadioIntf].modulation);
#if 0
        uint8_t testData[] = {0x0f, 0x16, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee};
        //uint8_t testData[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
        //uint8_t testData[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        // transparent mode
        //uint8_t testData[] = { 0xAA, 0xAA, 0xAA, 0xAA, 0x66, 0xBE, 0x0F, 0x5A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

        testTxLen = sizeof(testData);
        memcpy(radioTestBuf, testData, testTxLen);
#endif        
    } 

    debug_print("ch=%d Tx len = %d\n", actChannel[defRadioIntf], testTxLen);
    
    dump_buffer(radioTestBuf, testTxLen);
    wise_radio_tx_frame(defRadioIntf, actChannel[defRadioIntf], radioTestBuf, testTxLen);
}

void radioSetChannel(uint8_t channelIdx)
{
    uint32_t freq            = 0;
    actChannel[defRadioIntf] = channelIdx;
    wise_radio_get_channel_freq(defRadioIntf, actChannel[defRadioIntf], &freq);

    if (isRxOn()) {
        wise_radio_rx_stop(defRadioIntf);
        wise_radio_set_rx_mode(defRadioIntf, RADIO_RX_CONTINUOUS);
        wise_radio_rx_start(defRadioIntf, actChannel[defRadioIntf]);
    }
}

uint32_t radioGetFreq(void)
{
    uint32_t freq = 0;
    wise_radio_get_channel_freq(defRadioIntf, actChannel[defRadioIntf], &freq);
    return freq;
}

uint8_t radioGetChannel(void)
{
    uint32_t freq = 0;

    wise_radio_get_channel_freq(defRadioIntf, actChannel[defRadioIntf], &freq);
    return actChannel[defRadioIntf];
}

uint8_t radioTest(uint32_t val)
{
    _wise_radio_test();
    return 0;
}

uint8_t radioGetValidChannelNumber(void)
{
    uint8_t chNum = wise_radio_get_channel_num(defRadioIntf);
    return chNum;
}

uint8_t isInputPwrVaild(uint8_t _inputpwr)
{
    return (_inputpwr <= MAX_TX_PWR_LEVEL) ? 1 : 0;
}

void radioSetTxPwr(uint8_t _inputpwr)
{
    txPwrLevel[defRadioIntf] = _inputpwr;
    wise_radio_set_tx_pwr(defRadioIntf, txPwrLevel[defRadioIntf]);
}

uint8_t radioGetTxPwr(void)
{
    return txPwrLevel[defRadioIntf];
}

void radioSetRxLog(uint8_t _val)
{
    rxLog = _val;
}

void setPhyIntf(uint8_t _phy_intf)
{
    defRadioIntf = _phy_intf;
}

uint8_t getPhyIntf(void)
{
    return defRadioIntf;
}

void setPanId(uint16_t _panid)
{
    radioPANID = _panid;
    wise_radio_ieee802154_set_pan_id(defRadioIntf, _panid);
}

void setShortAddress(uint16_t _saddr)
{
    radioShortAddr = _saddr;
    wise_radio_ieee802154_set_short_address(defRadioIntf, _saddr);
}

void setLongAddress(uint8_t *_laddr)
{
    memcpy(radioLonfAddr, _laddr, sizeof(radioLonfAddr));
    wise_radio_ieee802154_set_long_address(defRadioIntf, radioLonfAddr);
}

int8_t radioGetDatarateIndex(uint32_t data_rate)
{
     uint32_t dr[] = 
    {
        4800,           //E_DATA_RATE_4P8K
        12500,          //E_DATA_RATE_12P5K
        32768,          //E_DATA_RATE_32P768K
        50000,          //E_DATA_RATE_50K
        100000,         //E_DATA_RATE_50K
        125000,         //E_DATA_RATE_125K
        200000,         //E_DATA_RATE_200K
        250000,         //E_DATA_RATE_250K
        500000,         //E_DATA_RATE_500K
        1000000,        //E_DATA_RATE_1M
        2000000,        //E_DATA_RATE_2M
    };

    for (uint32_t i = 0; i<sizeof(dr) / sizeof(uint32_t); i++) {
        if (dr[i] == data_rate)
            return i;
    }

    return -1;
}
void set_frame_fmt_msb_first(bool enable)
{

    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];
    fmt->frame_format &= ~FRAME_FMT_MSB_FIRST;
    fmt->crc.crc_config &= ~(CRC_INPUT_BIT_ENDIAN_MSB_FIRST|CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST|CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST); 

    if (enable){
        fmt->frame_format |= FRAME_FMT_MSB_FIRST;
        fmt->crc.crc_config |= (CRC_INPUT_BIT_ENDIAN_MSB_FIRST|CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST|CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST); 
    }
}

bool get_frame_fmt_msb_first(void)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];
    return (!!(fmt->frame_format & FRAME_FMT_MSB_FIRST));
}

void radio_set_pkt_type(WISE_RADIO_PKT_TYPE_T pkt_type)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    if (fmt) {
        fmt->pkt_type = pkt_type;
    }
}

void radio_set_crc_config(uint32_t cfg)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    if (fmt) {
        fmt->crc.crc_config = cfg;
    }
}

uint32_t radio_get_crc_config(void)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    return fmt ? fmt->crc.crc_config : 0u;
}

bool radio_set_crc_poly(WISE_CRC_POLY_SEL_T poly)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    if (!fmt || poly >= CRC_POLYNOMIAL_MAX) {
        return false;
    }

    fmt->crc.crc_poly_sel = poly;
    return true;
}

int32_t radio_get_crc_poly(void)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    return fmt ? fmt->crc.crc_poly_sel: -1;
}

void radio_set_crc_seed(uint16_t seed)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    if (fmt) {
        fmt->crc.crc_seed = seed;
    }
}

uint16_t radio_get_crc_seed(void)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    return fmt ? fmt->crc.crc_seed : 0u;
}

void radio_crc_set_flags(uint32_t mask)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];
    
    if (fmt) {
        fmt->crc.crc_config |= mask;
    }
}

void radio_crc_clr_flags(uint32_t mask)
{
    WISE_RADIO_PKT_FMT_T *fmt = &radioPktFmt[defRadioIntf];

    if (fmt) {
        fmt->crc.crc_config &= ~mask;
    }
}

void set_sg_setting(bool enable)
{
    WISE_RADIO_CFG_T *cfg     = &radioCfg[defRadioIntf];

    if (enable) {
        cfg->syncword = 0x667DF05A;
        //cfg->phy_mode = E_PHY_802154;
        _wise_radio_set_phy_mode(defRadioIntf, E_PHY_802154);
        _wise_radio_set_sg_test_mode(ENABLE);
    } else {
        cfg->syncword = 0xD8EC6B99;
        //cfg->phy_mode = E_PHY_TRANSPARENT;
        _wise_radio_set_phy_mode(defRadioIntf, E_PHY_TRANSPARENT);
        _wise_radio_set_sg_test_mode(DISABLE);
    }
}

void radio_clear_test_cnt(uint8_t enable)
{
    if (!enable) {
        rxGoodCount = 0;
        rxErrCount  = 0;
        rxTotalCount = 0;
    }
    
    debug_print("cnt info G/E/T:%lu/%lu/%lu\n", (unsigned long)rxGoodCount, (unsigned long)rxErrCount, (unsigned long)rxTotalCount);
}

void radio_set_ulpldo(uint8_t enable)
{
    _wise_radio_set_ulpldo(enable);
}

#endif //(SHELL_RADIO == ENABLE)

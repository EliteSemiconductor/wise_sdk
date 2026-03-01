#ifndef __INTERNAL_RADIO_CTRL_H 
#define __INTERNAL_RADIO_CTRL_H

//only internal used in radio function, do not include in public headers
#include "esmt_chip_specific.h"
#include <stdio.h>
#include "wise_radio_api.h"

#ifndef CHIP_RADIO_FLEXIBLE_FMT_VERSION
#define CHIP_RADIO_FLEXIBLE_FMT_VERSION         0
#endif

enum
{
    PHY_INIT_NONE = 0,
    PHY_INIT_TRANSPARENT,
    PHY_INIT_802154,
    PHY_INIT_WMBUS,
};

#define RADIO_MAX_RX_BUF_NUM                    8

//this is an internal used struct, do not move to header file
typedef struct {
    int8_t phy_idx;
    WISE_RADIO_CFG_T user_cfg;
    WISE_RADIO_PKT_FMT_T pkt_fmt;
    uint32_t phr_len_mask;
    uint8_t real802154;
    uint8_t phy_init_mode;
    uint8_t phy_mode; //moved from WISE_RADIO_CFG_T
    uint8_t wmbus_mode; //moved from WISE_RADIO_CFG_T
    
    int8_t ch_num;
    union
    {
        uint32_t *ch_map;
        uint32_t ch_freq;
    };
    WISE_RADIO_EVT_CB evt_callback;

    uint8_t intf_cfg_sts;
    WISE_RADIO_BUFFER_T buffer_pool;
    uint8_t *tx_buffer;
    uint16_t tx_buf_len;

    uint8_t *rx_buf_bank[RADIO_MAX_RX_BUF_NUM];
    uint16_t rx_buf_len;
    uint16_t rx_buf_num;
    uint32_t rx_meta_offset;

    uint16_t tx_len;
    int16_t tx_channel;
    CORE_IO_MODE_T tx_io_mode;
    uint8_t tx_pwr_level;
    int8_t tx_pwr_dbm;
    WISE_TX_TRANSITION_T tx_state_trans;

    WISE_RADIO_RX_MODE_T rx_mode;
    int16_t rx_channel;
    uint8_t rx_frame_count;
    uint8_t rx_w_index;
    uint8_t rx_r_index;

    uint32_t rx_byte_time_us;

    uint8_t crcWidth; //Notice: in byte unit
    uint8_t crcPolySel;
    uint8_t* crcTable;

    uint8_t radio_sts;
} RADIO_INTF_CFG_T;

typedef void (*RX_VALIDATE_PROC_T)(RADIO_INTF_CFG_T *intf_cfg, uint8_t *pframe, WISE_RX_META_T *rx_meta, uint32_t status);
typedef uint16_t (*RX_SYNC_CALLBACK_T)(RADIO_INTF_CFG_T *intf_cfg, uint8_t *pframe);

#endif

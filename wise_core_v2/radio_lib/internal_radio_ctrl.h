#ifndef __INTERNAL_RADIO_CTRL_H 
#define __INTERNAL_RADIO_CTRL_H

//only internal used in radio function, do not include in public headers
#include "esmt_chip_specific.h"
#include <stdio.h>
#include "wise_radio_api.h"

#ifndef CHIP_RADIO_FLEXIBLE_FMT_VERSION
#define CHIP_RADIO_FLEXIBLE_FMT_VERSION         2 //currently baseline is version 2
#endif

#ifndef ENABLE_RADIO_TRACE_LOG
#define ENABLE_RADIO_TRACE_LOG                 0
#endif

/**
 * @enum WISE_PHY_MODE_T
 * @brief High-level PHY operating modes.
 */
typedef enum 
{
    E_PHY_TRANSPARENT = 0x0, /**< Transparent (raw) PHY mode. */
    E_PHY_802154      = 0x1, /**< IEEE 802.15.4-compliant PHY mode. */
    E_PHY_CCA         = 0x2,
    E_PHY_BLE         = 0x3,
    E_PHY_BLE_LR      = 0x4,
    E_PHY_WMBUS       = 0x5, /**< Wireless M-Bus PHY mode. */
    E_PHY_NONE        = 0xff, 
} WISE_PHY_MODE_T;


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
    uint8_t crcInReflect;  //resolved input-bit reflection; drives the crc16 calc direction
    uint8_t crcOutReflect; //resolved output-bit reflection (custom poly only)
    uint32_t crcXorout;    //final XOR applied to the CRC (custom poly only)
    uint8_t* crcTable;

    uint8_t radio_sts;
} RADIO_INTF_CFG_T;

typedef void (*RX_VALIDATE_PROC_T)(RADIO_INTF_CFG_T *intf_cfg, uint8_t *pframe, WISE_RX_META_T *rx_meta, uint32_t status);
typedef uint16_t (*RX_SYNC_CALLBACK_T)(RADIO_INTF_CFG_T *intf_cfg, volatile uint8_t *pframe);

#endif

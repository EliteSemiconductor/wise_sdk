#ifndef __WISE_RADIO_BLE_API_H 
#define __WISE_RADIO_BLE_API_H

#include "types.h"
#include "wise_core.h"
#include "wise_radio_api.h"

WISE_STATUS wise_radio_ble_config(int8_t intf_idx, WISE_RADIO_CFG_T *cfg, WISE_RADIO_PKT_FMT_T *pkt_fmt);

#endif

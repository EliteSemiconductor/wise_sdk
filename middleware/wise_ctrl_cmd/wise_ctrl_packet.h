/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __WISE_CTRL_PACKET_H
#define __WISE_CTRL_PACKET_H

#ifndef TARGET_ARDUINO
#include "wise.h"
#endif

#include <stdint.h>

#ifndef PKT_NO_ERR
#define PKT_NO_ERR WISE_SUCCESS
#define PKT_ERR WISE_FAIL
#endif

//
//  |-----|-----|-----|-------|-------|-------------------------|-----|
//  |0xfc |0xfc |0xfc | len_h | len_l |    payload(1-255)       |crc  |
//

#define PKT_PREAMBLE                                        0xfc
#define PKT_PREAMBLE_LEN                                    3

#define PKT_OFFSET_LEN                                      (PKT_PREAMBLE_LEN)
#define PKT_OFFSET_PAYLOAD                                  (PKT_OFFSET_LEN + 2)

#define PKT_OFFSET_CRC_START                                PKT_OFFSET_LEN

#ifndef PKT_MAX_PAYLOAD_LEN
#define PKT_MAX_PAYLOAD_LEN                                 0x100 //kevinyang, 20251122, default use 0x100, assign new value in projects
#endif
#define PKT_ADD_ON                                          (PKT_PREAMBLE_LEN + 2 + 1) // preamble+len+crc
#define PKT_MAX_LEN                                         (PKT_MAX_PAYLOAD_LEN + PKT_ADD_ON)

#define PKT_OUTPUT_NO_WAIT                                  1
#define PKT_OUTPUT_BUFFERRED                                0

#define PKT_OUTPUT_STS_IDLE                                 0
#define PKT_OUTPUT_STS_BUSY                                 1

#define PKT_RX_STS_IDLE                                     0
#define PKT_RX_STS_BUSY                                     1

typedef int32_t (*WISE_PKT_RECV_CALLBACK)(uint8_t *buffer, int len);
typedef int32_t (*WISE_PKT_SEND_CALLBACK)(uint8_t *buffer, int len);

int32_t wise_pkt_init(WISE_PKT_SEND_CALLBACK sendCallback,
                      WISE_PKT_RECV_CALLBACK recvCallback);

int32_t wise_pkt_get_output_buf(uint32_t *bufAddr, uint32_t *bufLen);
int32_t wise_pkt_querry_output_buf();
int32_t wise_pkt_start_output(uint16_t length, uint8_t noWait);

void wise_pkt_proc(void);         // called from super loop
int8_t wise_pkt_input(uint8_t c); // called from uart input
int32_t wise_pkt_rx_state();
#endif

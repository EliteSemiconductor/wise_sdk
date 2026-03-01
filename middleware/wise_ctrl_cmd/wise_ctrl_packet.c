/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if MIDDLEWARE_WISE_CTRL_CMD
#include <stdlib.h>
#include <string.h>

#ifndef TARGET_ARDUINO
#include "wise.h"
#endif

#include "wise_ctrl_packet.h"

#include "radio_lib/util_crc.h"

#ifdef TARGET_HAL_CTRL
#include "util.h"
#include "util_debug_print.h"
#define CTRL_PKT_DEBUG util_debug_print
#else
#include "wise_core.h"
#define CTRL_PKT_DEBUG debug_print
#endif

#define PKT_STATE_SYNC          0
#define PKT_STATE_LEN           1
#define PKT_STATE_DATA          2
#define PKT_STATE_FIN           3

#define CTRL_PKT_CRC_POLY       CRC_TYPE_8_CCITT

typedef struct {
    uint8_t state;
    uint8_t syncCount;
    uint8_t lenCount;
    uint8_t checkcrc;
    uint16_t lenByte;
    uint16_t wIndex;
    uint8_t pktBuffer[PKT_MAX_LEN];
    uint32_t lastInputTime;
} WISE_PKT_COLLECTOR_T;

typedef struct {
    uint16_t pktLen;
    uint16_t pktWIndex;
    uint8_t pktBuf[PKT_MAX_LEN];
} WISE_PKT_SENDER_T;

static WISE_PKT_COLLECTOR_T pktCollector;
static WISE_PKT_RECV_CALLBACK pktRecvCb;
static WISE_PKT_SEND_CALLBACK pktSendCb;
static WISE_PKT_SENDER_T pktSender;
static uint8_t* ctrlPktCrcTable = NULL;

static void pktCollectorReset();

static void pktCollectorReset()
{
    pktCollector.syncCount = 0;
    pktCollector.lenCount  = 0;
    pktCollector.wIndex    = 0;
    pktCollector.checkcrc  = 0;
    pktCollector.state     = PKT_STATE_SYNC;
    pktCollector.lenByte   = 0;
}

int32_t wise_pkt_init(WISE_PKT_SEND_CALLBACK sendCallback,
                      WISE_PKT_RECV_CALLBACK recvCallback)
{
    memset(&pktCollector, 0, sizeof(WISE_PKT_COLLECTOR_T));
    memset(&pktSender, 0, sizeof(WISE_PKT_SENDER_T));

    if(!ctrlPktCrcTable)
    {
        ctrlPktCrcTable = malloc(CRC_8_TABLE_SIZE);
        if(!ctrlPktCrcTable)
        {
            CTRL_PKT_DEBUG("CTRL_PKT failed to allocate buffer\n");
            return PKT_ERR;
        }
        
        util_crc8_gen_table(CTRL_PKT_CRC_POLY, ctrlPktCrcTable);
    }

    pktRecvCb = recvCallback;
    pktSendCb = sendCallback;

    return PKT_NO_ERR;
}

// return value: 0=not handled, 1=handled, -1=packet fail
int8_t wise_pkt_input(uint8_t c)
{
    int8_t result = 0;

    switch (pktCollector.state) {
    case PKT_STATE_SYNC:
        if (pktCollector.syncCount < PKT_PREAMBLE_LEN - 1) {
            if (c == PKT_PREAMBLE) {
                pktCollector.syncCount++;
                result = 1;
            } else if (pktCollector.syncCount != 0) {
                pktCollectorReset();
            }
        } else {
            if (c == PKT_PREAMBLE) {
                pktCollector.state = PKT_STATE_LEN;
                // more preambles than normal, accept it
                result = 1;
            } else {
                pktCollectorReset();
            }
        }
        break;

    case PKT_STATE_LEN:
        pktCollector.checkcrc = util_crc8_calc(CTRL_PKT_CRC_POLY, ctrlPktCrcTable, &c, 1, pktCollector.checkcrc);
        pktCollector.lenByte = ((pktCollector.lenByte << (0x8 * pktCollector.lenCount++)) + c);

        if (pktCollector.lenCount <= 1) {
            pktCollector.state = PKT_STATE_LEN;
        } else {
            if (pktCollector.lenByte == 0) {
                CTRL_PKT_DEBUG("Invalid packet len\n");
                pktCollectorReset();
            } else {
                pktCollector.state = PKT_STATE_DATA;
            }
        }
        result = 1;
        break;

    case PKT_STATE_DATA:
        pktCollector.pktBuffer[pktCollector.wIndex] = c;
        pktCollector.wIndex++;
        pktCollector.checkcrc = util_crc8_calc(CTRL_PKT_CRC_POLY, ctrlPktCrcTable, &c, 1, pktCollector.checkcrc);
        
        if (pktCollector.wIndex == pktCollector.lenByte) {
            pktCollector.state   = PKT_STATE_FIN;
            pktCollector.lenByte = 0;
        }
        result = 1;
        break;

    case PKT_STATE_FIN:
        if (pktCollector.checkcrc == c) {
            if (pktRecvCb) {
                (pktRecvCb)(pktCollector.pktBuffer, pktCollector.wIndex);
            }
        } else {
            CTRL_PKT_DEBUG("uart pkt error, 0x%x, 0x%x\n", c, pktCollector.checkcrc);
            // dump_buffer(pktCollector.pktBuffer, pktCollector.lenByte);
        }

        pktCollectorReset();
        result = 1;
        break;
    }

    if (result == 0) {
        // filter non-text characters
        if (c >= 0x80) {
            result = 1;
        }
    }

    return result;
}

int32_t wise_pkt_rx_state()
{
    if((pktCollector.state == PKT_STATE_SYNC) && (pktCollector.syncCount == 0))
        return PKT_RX_STS_IDLE;
    else
        return PKT_RX_STS_BUSY;
}


int32_t wise_pkt_get_output_buf(uint32_t *bufAddr, uint32_t *bufLen)
{
    *bufAddr = (uint32_t)&pktSender.pktBuf[PKT_OFFSET_PAYLOAD];
    *bufLen  = PKT_MAX_PAYLOAD_LEN;

    return PKT_NO_ERR;
}

int32_t wise_pkt_querry_output_buf()
{
    if (pktSender.pktLen == 0) {
        return PKT_OUTPUT_STS_IDLE;
    } else {
        return PKT_OUTPUT_STS_BUSY;
    }
}

int32_t wise_pkt_start_output(uint16_t length, uint8_t noWait)
{
    uint8_t *pkt = &pktSender.pktBuf[0];
    int i;

    if (!length || (length > PKT_MAX_PAYLOAD_LEN)) {
        return PKT_ERR;
    }

    if (pktSender.pktLen) {
        CTRL_PKT_DEBUG("pkt abandoned\n");
    }

    pktSender.pktWIndex = 0;
    pktSender.pktLen    = length + PKT_ADD_ON;

    for (i = 0; i < PKT_PREAMBLE_LEN; i++) {
        pkt[i] = PKT_PREAMBLE;
    }

    PUT_BE_16(&pkt[PKT_OFFSET_LEN], length);
    
    pkt[pktSender.pktLen - 1] = util_crc8_calc(CTRL_PKT_CRC_POLY, 
                                                ctrlPktCrcTable, 
                                                &pkt[PKT_OFFSET_CRC_START], 
                                                length + 2, 
                                                0);

    if (noWait == PKT_OUTPUT_NO_WAIT) {
        if (pktSendCb) {
            (pktSendCb)(pktSender.pktBuf, pktSender.pktLen);
        }

        pktSender.pktLen = 0;
    } else {
    }

    return PKT_NO_ERR;
}

void wise_pkt_proc(void)
{
    if (pktSender.pktLen) {
        if (pktSendCb) {
            (pktSendCb)(&pktSender.pktBuf[pktSender.pktWIndex], 1);
        }

        pktSender.pktWIndex++;
        if (pktSender.pktWIndex == pktSender.pktLen) {
            // packet tx finished
            pktSender.pktLen    = 0;
            pktSender.pktWIndex = 0;
        }
    }
}
#endif /* MIDDLEWARE_WISE_CTRL_CMD */

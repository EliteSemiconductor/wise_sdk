/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_nfc_ctrl.c
 * @brief Example application: firmware update and data exchange over the NFC control channel.
 *
 * @ingroup WISE_EXAMPLE_APP_NFC
 *
 * This example shows how to use the NFC control channel so that a host application
 * can update the device firmware - or exchange arbitrary data - wirelessly over NFC,
 * without any wired (UART/SWD) connection.
 *
 * The host-side companion application is the ESMT NFC Tool, which is available in an
 * Android version and a PC version.
 *
 * Setup performed in main():
 * - wise_nfc_init()                 : initialize the NFC module.
 * - wise_nfc_crtl_tag_config("...") : register the NDEF text tag advertised to the reader.
 * - wise_nfc_ctrl_register_int_cb() : register the NFC control interrupt callback.
 *
 * Firmware update flow over NFC:
 * 1. The ESMT NFC Tool writes a control packet to the device over NFC. This raises
 *    the NFC control interrupt, and the callback reads the incoming bytes with
 *    wise_nfc_ctrl_read_data().
 * 2. The main loop feeds the received bytes into the WISE control-packet parser via
 *    wise_pkt_input(), which decodes and dispatches the command, including the
 *    firmware-update commands (image transfer and flashing).
 * 3. Any response the parser produces (for example an ACK or status) is fetched with
 *    wise_pkt_get_resp_data() and written back to the ESMT NFC Tool over NFC with
 *    wise_nfc_ctrl_write_data().
 *
 * Repeating this request/response exchange lets the ESMT NFC Tool drive a complete
 * firmware update purely over the NFC link. The UART shell ("NFC_CTRL> ") is a
 * transport/prompt convenience and is not required for the update.
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_sys_api.h"
#include "wise_nfc_api.h"
#include "wise_ctrl_packet.h"
#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

#define DEMO_APP_NAME       "DEMO NFC CTRL"
#define DEMO_APP_PROMPT     "NFC_CTRL> "

uint8_t nfcCtrlData[NFC_DATA_BLOCK_LEN];
volatile int nfcCtrlLen;

static void nfc_ctrl_isr_cb(void)
{
    nfcCtrlLen = wise_nfc_ctrl_read_data(nfcCtrlData);
}

void main(void)
{
    int idx = 0;
    uint8_t *respData = NULL;
    uint16_t respLen;

    demo_app_common_init(DEMO_APP_NAME);

    app_shell_init(DEMO_APP_PROMPT);

    wise_nfc_init();

    wise_nfc_crtl_tag_config("EMST_ER8130_SUBG_SOC");

    wise_nfc_ctrl_register_int_cb(nfc_ctrl_isr_cb);

    while (1) {
        // Feed received data byte-by-byte into the control packet handler
        while (nfcCtrlLen) {
            wise_pkt_input(nfcCtrlData[idx]);
            idx++;
            nfcCtrlLen--;
        }
        idx = 0;

        // Check for response data and write it back to the host via NFC
        respLen = wise_pkt_get_resp_data(&respData);
        if (respLen > 0 && respData != NULL) {
            wise_nfc_ctrl_write_data(respData, respLen);
        }
    }
}

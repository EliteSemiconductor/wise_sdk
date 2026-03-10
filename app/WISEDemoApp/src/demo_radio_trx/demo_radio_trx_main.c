/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_radio_shell_app.c
 * @brief Example application demonstrating WISE_RADIO basic TX/RX control via UART shell.
 *
 * @ingroup WISE_EXAMPLE_APP_RADIO
 *
 * This example application demonstrates:
 * - Initializing WISE radio interface and configuring PHY parameters
 * - Configuring packet format for fixed-length frames
 * - Setting TX power and TX/RX operation modes
 * - Receiving frames via event callback and dumping received payload
 * - Sending a fixed test packet repeatedly via shell command
 * - Selecting and listing radio channels via shell command
 *
 * The shell prompt is configured as "RADIO> " on the selected UART channel.
 *
 * Supported shell commands:
 * - recv on/off           : start/stop RX
 * - send [count] [ms]     : send test packet multiple times with interval
 * - channel               : show current channel and frequency
 * - channel list          : list all channels and frequencies
 * - channel [num]         : switch active channel (and restart RX if enabled)
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "util.h"
#include "wise_radio_api.h"
#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_RADIO Radio Shell Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: basic radio TX/RX and channel control via shell.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_RADIO for radio PHY configuration, TX/RX control, and event callback
 * - @ref WISE_UART  for shell UART transport
 * - @ref WISE_TICK  for delay between TX packets (via ::wise_tick_delay_ms)
 * - @ref WISE_CORE  for main processing loop
 *
 * @{
 */

#define DEMO_APP_PROMPT             "RADIO> "
#define RADIO_BUF_LEN               512 /**< RX buffer pool size in bytes. */

static void rfEventHandler(WISE_RADIO_EVT_T evt);

/** @brief Radio interface index used by this demo. */
static const int8_t rfIntf = 0;

/** @brief RX buffer pool storage for the radio driver. */
static uint8_t radioBufPool[RADIO_BUF_LEN];

/** @brief Radio buffer descriptor passed to the radio driver via ::wise_radio_set_buffer. */
static WISE_RADIO_BUFFER_T appRadioBuffer = {RADIO_BUF_LEN, (uint32_t)radioBufPool};

/** @brief Currently active channel index used for TX/RX. */
static uint8_t rfActiveCh = 0;

/** @brief RX state flag controlled by the "recv" shell command. */
static uint8_t radioReceiving = 0;

/**
 * @brief Radio PHY configuration used by this demo.
 *
 * This configuration selects:
 * - Modulation: GFSK
 * - Frequency range: 915MHz ~ 935MHz
 * - Channel spacing: 1MHz
 * - Data rate: 500kbps (driver enum)
 * - Preamble and syncword settings
 */
static WISE_RADIO_CFG_T rfConfig =
{
    .modulation    = E_MOD_TYPE_GFSK,
    .ch_freq_min   = 915000000,
    .ch_freq_max   = 935000000,
    .ch_spacing    = 1000000,
    .deviation     = 175000,
    .data_rate     = E_DATA_RATE_500K,
    .preamble      = 0xAA,
    .preamble_len  = 4,
    .sync_word_len = 4,
    .syncword      = 0xF68DF68D,
};

/**
 * @brief Packet format configuration used by this demo.
 *
 * The demo uses fixed-length packet type and enables preamble + syncword.
 * CRC/PHR fields are left as default (zero-initialized) for this example.
 */
static WISE_RADIO_PKT_FMT_T rfPktFmt =
{
    .pkt_type       = PKT_FIXED_LENGTH,
    .frame_format   = (FRAME_FMT_PREAMBLE_EN | FRAME_FMT_SYNCWORD_EN),
    .max_pkt_length = 16,
    .phr            = {0},
    .crc            = {0},
    .frame_codec    = E_FRAME_CODEC_NONE,
};

/** @brief Fixed test packet payload used by the "send" shell command. */
static const uint8_t TEST_PACKET[16] =
{
    0x0F, 0x16, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
    0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
};

/* ========================================================================== */
/* recv Command                                                               */
/* ========================================================================== */

/**
 * @brief Shell command: start or stop radio reception.
 *
 * Usage:
 * @code
 * recv on
 * recv off
 * @endcode
 *
 * - "on"  : calls ::wise_radio_rx_start() on current channel @ref rfActiveCh
 * - "off" : calls ::wise_radio_rx_stop()
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector (argv[1] is "on" or "off").
 *
 * @return 0 on success, negative value on invalid parameters.
 */
static int cmd_recv(int argc, char **argv)
{
    if (argc != 2) {
        printf("Invalid parameters:\r\n");
        printf("Usage: recv [on/off]\r\n");
        return -1;
    }

    if (strcmp(argv[1], "on") == 0) {
        wise_radio_rx_start(rfIntf, rfActiveCh);
        radioReceiving = 1;
        printf("start receiving on channel %d\r\n", rfActiveCh);
    } else if (strcmp(argv[1], "off") == 0) {
        wise_radio_rx_stop(rfIntf);
        radioReceiving = 0;
        printf("stop receiving\r\n");
    } else {
        printf("Invalid parameters:\r\n");
        printf("Usage: recv [on/off]\r\n");
        return -1;
    }

    return 0;
}

/** Register shell command "recv". */
SHELL_CMD_AUTO(recv, cmd_recv, "Switch receive on/off");

/* ========================================================================== */
/* send Command                                                               */
/* ========================================================================== */

/**
 * @brief Shell command: transmit the fixed test packet.
 *
 * Usage:
 * @code
 * send
 * send [count]
 * send [count] [interval_ms]
 * @endcode
 *
 * The command transmits @ref TEST_PACKET on current channel @ref rfActiveCh.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success, negative value on invalid parameters.
 */
static int cmd_send(int argc, char **argv)
{
    uint32_t testCount = 1;
    uint32_t interval = 100;
    int i;

    if (argc >= 2) {
        testCount = strtoul(argv[1], NULL, 10);
        if (errno != 0) {
            printf("Invalid parameters:\r\n");
            printf("Usage: send [count] [interval_ms]\r\n");
            return -1;
        }

        if (argc == 3) {
            interval = strtoul(argv[2], NULL, 10);
            if (errno != 0) {
                printf("Invalid parameters:\r\n");
                printf("Usage: send [count] [interval_ms]\r\n");
                return -1;
            }
        } else if (argc > 3) {
            printf("Invalid parameters:\r\n");
            printf("Usage: send [count] [interval_ms]\r\n");
            return -1;
        }
    }

    for (i = 0; i < (int)testCount; i++) {
        printf("send pkt %d\r\n", i);
        if (WISE_SUCCESS != wise_radio_tx_frame(rfIntf, rfActiveCh, TEST_PACKET, sizeof(TEST_PACKET))) {
            printf("tx failed\r\n");
        }
        wise_tick_delay_ms(interval);
    }

    return 0;
}

/** Register shell command "send". */
SHELL_CMD_AUTO(send, cmd_send, "Send a packet");

/* ========================================================================== */
/* channel Command                                                            */
/* ========================================================================== */

/**
 * @brief Shell command: show/list/set radio channel.
 *
 * Usage:
 * @code
 * channel
 * channel list
 * channel [channel_num]
 * @endcode
 *
 * - No argument: prints current channel and its frequency.
 * - "list"     : prints all channels and their frequencies.
 * - number     : selects active channel @ref rfActiveCh and restarts RX if enabled.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success, negative value on invalid parameters.
 */
static int cmd_channel(int argc, char **argv)
{
    uint8_t channelNum = 0;
    int i;
    uint32_t freq = 0;

    if (argc > 2) {
        printf("Invalid parameters:\r\n");
        printf("Usage: channel [channel_num] or channel list\r\n");
        return -1;
    }

    channelNum = wise_radio_get_channel_num(rfIntf);

    if (argc == 1) {
        wise_radio_get_channel_freq(rfIntf, rfActiveCh, &freq);
        printf("Current ch=%d freq=%lu\r\n", rfActiveCh, (unsigned long)freq);
    } else if (strcmp(argv[1], "list") == 0) {
        for (i = 0; i < channelNum; i++) {
            wise_radio_get_channel_freq(rfIntf, i, &freq);
            printf("ch=%d freq=%lu\r\n", i, (unsigned long)freq);
        }
    } else {
        uint8_t newChannel = 0;

        newChannel = strtoul(argv[1], NULL, 10);
        if (errno != 0) {
            printf("Invalid parameters:\r\n");
            printf("Usage: channel [channel_num] or channel list\r\n");
            return -1;
        }

        if (newChannel < channelNum) {
            rfActiveCh = newChannel;
            printf("set channel to %d\r\n", newChannel);

            if (radioReceiving) {
                wise_radio_rx_stop(rfIntf);
                wise_radio_rx_start(rfIntf, rfActiveCh);
                printf("restarted receiving on ch %d\r\n", rfActiveCh);
            }
        } else {
            printf("invalid channel\r\n");
        }
    }

    return 0;
}

/** Register shell command "channel". */
SHELL_CMD_AUTO(channel, cmd_channel, "Select radio channel");

/* ========================================================================== */
/* Radio Event Handler                                                        */
/* ========================================================================== */

/**
 * @brief Radio event callback registered via ::wise_radio_set_evt_callback().
 *
 * Handles events:
 * - @ref WISE_RADIO_EVT_RX_FRAME : fetch and dump all pending RX frames
 * - @ref WISE_RADIO_EVT_RX_ERR   : release RX frame and report invalid frame
 *
 * RX frame handling flow:
 * 1. Query number of pending frames by ::wise_radio_get_rx_frame_num()
 * 2. Query frame info and buffer address by ::wise_radio_get_rx_frame_info()
 * 3. Dump payload by ::dump_buffer()
 * 4. Release RX frame by ::wise_radio_release_rx_frame()
 *
 * @param[in] evt Bitmask of radio events.
 */
static void rfEventHandler(WISE_RADIO_EVT_T evt)
{
    if (evt & WISE_RADIO_EVT_RX_FRAME) {
        WISE_RX_META_T meta = {0};
        uint32_t bufAddr = 0;

        while (0 < wise_radio_get_rx_frame_num(rfIntf)) {
            if (WISE_SUCCESS == wise_radio_get_rx_frame_info(0, &bufAddr, &meta)) {
                printf("recv len %d buf=%08lx rssi=%d\r\n",
                       meta.data_len,
                       (unsigned long)bufAddr,
                       meta.rssi);
                dump_buffer(bufAddr, meta.data_len);
            } else {
                printf("rx read buffer fail\r\n");
            }
            wise_radio_release_rx_frame(rfIntf);
        }
    }

    if (evt & WISE_RADIO_EVT_RX_ERR) {
        wise_radio_release_rx_frame(rfIntf);
        printf("invalid frame received\r\n");
    }
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the radio shell example application.
 *
 * Initializes demo environment and shell, then initializes and configures radio:
 * - Initialize radio interface
 * - Register event callback
 * - Provide RX buffer pool
 * - Apply PHY and packet format configuration
 * - Set TX power and TX/RX operating modes
 *
 * Finally enters the main processing loop via ::wise_main_proc().
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    wise_radio_init(rfIntf);
    wise_radio_set_evt_callback(rfIntf, rfEventHandler);
    wise_radio_set_buffer(rfIntf, &appRadioBuffer);

    wise_radio_config(rfIntf, &rfConfig, &rfPktFmt);
    wise_radio_set_tx_pwr_dbm(rfIntf, 11);

    wise_radio_set_tx_io_mode(rfIntf, CORE_IO_BLOCKING);
    wise_radio_set_rx_mode(0, RADIO_RX_CONTINUOUS);

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_RADIO */

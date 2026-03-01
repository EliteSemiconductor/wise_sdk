/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_wmbus_phy_main.c
 * @brief Example application demonstrating WMBus PHY TX/RX using WISE Radio WMBus API.
 *
 * @ingroup WISE_EXAMPLE_APP_WMBUS
 *
 * This example demonstrates a Wireless M-Bus (WMBus) PHY demo based on:
 * - WISE Radio WMBus API (PHY control / TX / RX)
 * - WMBus crypto helper (key setup)
 * - WMBus frame helper (frame build and parsing)
 *
 * The demo supports two roles selected at build time:
 * - Meter role   (define WMBUS_DEMO_PHY_METER)
 * - Collector role (default if WMBUS_DEMO_PHY_METER is not defined)
 *
 * High-level behavior:
 * - Meter:
 *   - Periodically sends a report frame every @ref METER_REPORT_INTERVAL
 *   - After TX done, enters one-shot RX to wait for collector response
 *   - Times out after @ref METER_RESP_TIMEOUT if no valid response is received
 *
 * - Collector:
 *   - Continuously listens in one-shot RX mode
 *   - On receiving a valid meter report, waits @ref COLLECTOR_ACK_TIME then sends a response
 *   - Restarts RX for next frame
 *
 * Shell:
 * - A UART shell is initialized for consistency with other demos. No explicit
 *   shell commands are registered in this snippet.
 *
 * @note This demo uses non-blocking TX mode via ::wise_radio_set_tx_io_mode().
 * @note RX buffer pool must be provided to radio driver via ::wise_radio_set_buffer().
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "wise_sys_api.h"
#include "wise_radio_wmbus_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

#include "wise_wmbus_crypto.h"
#include "wmbus_helper.h"

/**
 * @defgroup WISE_EXAMPLE_APP_WMBUS WMBus PHY Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: WMBus PHY demo (meter/collector) using WISE Radio WMBus API.
 *
 * This demo uses the following modules:
 * - @ref WISE_RADIO_WMBUS_APIs for WMBus PHY TX/RX control
 * - @ref WISE_RADIO           for event callback and RX buffer handling
 * - WMBus crypto helper (wise_wmbus_crypto_*) for key initialization
 * - WMBus helper (wmbus_setup_* / wmbus_dump_frame_info) for frame build/parse
 * - @ref WISE_TICK for timing and timeout management
 * - @ref WISE_CORE for task and scheduler helpers (wise_create_task / wise_schlr_add_periodical)
 *
 * @{
 */

#define SHELL_UART_CH               0   /**< UART channel used by shell backend. */
#define WMBUS_RADIO_INTF            0   /**< Radio interface index used by this demo. */
#define WMBUS_RF_BUFFER_LEN         768 /**< Radio driver buffer pool size in bytes. */

#define METER_REPORT_INTERVAL       8000 /**< Meter periodic report interval in milliseconds. */
#define METER_RESP_TIMEOUT          1000 /**< Meter response wait timeout in milliseconds. */
#define COLLECTOR_ACK_TIME          5    /**< Collector ACK response delay in milliseconds. */

#define WMBUS_TX_BUF_LEN            256  /**< Local TX frame buffer length in bytes. */
#define WMBUS_RX_BUF_LEN            256  /**< Local RX frame buffer length in bytes. */

/**
 * @brief Meter state machine states.
 */
enum
{
    E_MTR_STAT_IDLE = 0,        /**< Idle state, waiting for report request. */
    E_MTR_STAT_REPORTING,       /**< TX started, waiting TX done event. */
    E_MTR_STAT_WAITING_RESP,    /**< RX started, waiting for response frame or timeout. */
};

/** @brief Radio buffer pool storage for WMBus PHY radio. */
uint8_t wmbusRFBufferPool[WMBUS_RF_BUFFER_LEN];

/** @brief Radio buffer descriptor passed to radio driver via ::wise_radio_set_buffer(). */
WISE_RADIO_BUFFER_T wmbusRFBuffer = {WMBUS_RF_BUFFER_LEN, (uint32_t)wmbusRFBufferPool};

/** @brief Local TX frame buffer (frame builder writes into this buffer). */
static uint8_t mbusTxBuffer[WMBUS_TX_BUF_LEN];

/** @brief Local RX frame buffer (copies from radio buffer). */
static uint8_t mbusRxBuffer[WMBUS_RX_BUF_LEN];

/**
 * @brief Build-time role selection.
 *
 * If WMBUS_DEMO_PHY_METER is defined, role is meter; otherwise collector.
 */
#ifdef WMBUS_DEMO_PHY_METER
const wmbus_role_t wmbusRole = WMBUS_ROLE_METER;
#else
const wmbus_role_t wmbusRole = WMBUS_ROLE_OTHER;
#endif

/** @brief WMBus PHY mode selected for this demo. */
const wmbus_mode_t wmbusMode = WMBUS_MODE_T2;

/** @brief Demo AES key used by WMBus crypto helper. */
static const uint8_t cryptoKey[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

/** @brief Human-readable mode names for logging. */
static const int8_t* WMBUS_MODE_STR[] = {"S1", "S1M", "S2", "T1", "T2", "C1", "C2"};

/** @brief Human-readable role names for logging. */
static const int8_t* WMBUS_ROLE_STR[] = {"COLLECTOR", "METER"};

/** @brief Meter state machine variable. */
static uint8_t meterState = E_MTR_STAT_IDLE;

/** @brief Meter response RX start tick (used for timeout check). */
static uint32_t meterRxStartTime = 0;

/** @brief Set by periodic scheduler to request a meter report TX. */
static uint8_t reportRequest = 0;

/** @brief Set when meter TX finishes (TX_DONE or TX_ERR). */
static uint8_t reportFinished = 0;

/** @brief Set when a frame is received (RX_FRAME or RX_ERR). */
static uint8_t frameReceived = 0;

/** @brief RX frame metadata returned by ::wise_radio_get_rx_frame_info(). */
static WISE_RX_META_T rxFrameMeta = {0};

/** @brief WMBus accessibility field used by helper frame builder. */
WMBUS_accessibility_t wmbusAccessbility = WMBUS_ACCESSIBILITY_LIMITED_ACCESS;

/** @brief WMBus access number (incremented each TX). */
uint8_t accessNumber = 0U;

/* Forward declarations */
static void _setup_wmbus_demo_meter(void);
static void _meter_report_timer(void *pData);
static void _meter_proc(void *pData);
static void _wmbus_meter_report(void);

static void _setup_wmbus_demo_collector(void);
static void _collector_proc(void);
static void _collector_response(void);

/* ========================================================================== */
/* Shell Backend                                                              */
/* ========================================================================== */

/**
 * @brief Read one character from UART for shell input.
 *
 * @param[out] ch Pointer to the variable that receives the character.
 *
 * @retval true  A character was read successfully.
 * @retval false No character available or read failed.
 */
static bool shell_uart_read_char(char *ch)
{
    uint8_t tmp;
    if (wise_uart_read_char(SHELL_UART_CH, &tmp) == WISE_SUCCESS) {
        *ch = (char)tmp;
        return true;
    }
    return false;
}

/**
 * @brief Write a null-terminated string to UART for shell output.
 *
 * @param[in] s Null-terminated string to transmit.
 */
static void shell_uart_write_str(const char *s)
{
    while (*s) {
        wise_uart_write_char(SHELL_UART_CH, (uint8_t)*s++);
    }
}

/**
 * @brief Initialize shell configuration and bind UART backend callbacks.
 *
 * Sets the shell prompt to "DEMO> ".
 */
static void app_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = "DEMO> ",
    };

    shell_init(&cfg);
}

/* ========================================================================== */
/* Radio Event Callback                                                       */
/* ========================================================================== */

/**
 * @brief Radio event callback for WMBus PHY demo.
 *
 * RX events:
 * - On RX_FRAME: read RX frame info and copy payload to @ref mbusRxBuffer
 * - On RX_ERR  : mark frameReceived and release RX frame
 *
 * TX events:
 * - On TX_DONE/TX_ERR: for meter role, sets @ref reportFinished
 *
 * @param[in] evt Bitmask of radio events.
 *
 * @note This callback always calls ::wise_radio_release_rx_frame() when RX_FRAME
 *       or RX_ERR occurs.
 * @note RX payload is copied only when @ref rxFrameMeta.valid is true.
 */
static void radioWMbusEventCb(WISE_RADIO_EVT_T evt)
{
    if ((evt & WISE_RADIO_EVT_RX_FRAME) || (evt & WISE_RADIO_EVT_RX_ERR))
    {
        uint32_t bufAddr = 0;

        if (evt & WISE_RADIO_EVT_RX_FRAME)
        {
            if (WISE_SUCCESS == wise_radio_get_rx_frame_info(WMBUS_RADIO_INTF, &bufAddr, &rxFrameMeta))
            {
                if (rxFrameMeta.valid) {
                    memcpy((void *)mbusRxBuffer, (void *)bufAddr, rxFrameMeta.data_len);
                }
            }
        }

        frameReceived = 1;
        wise_radio_release_rx_frame(WMBUS_RADIO_INTF);
    }

    if ((evt & WISE_RADIO_EVT_TX_DONE) || (evt & WISE_RADIO_EVT_TX_ERR))
    {
#ifdef WMBUS_DEMO_PHY_METER
        reportFinished = 1;
#endif
    }
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the WMBus PHY demo application.
 *
 * Flow:
 * 1. Initialize demo environment and shell
 * 2. Initialize WMBus radio interface and register event callback
 * 3. Provide RF buffer pool and set TX IO mode to non-blocking
 * 4. Initialize WMBus crypto module and set crypto key
 * 5. Configure WMBus PHY role/mode and set TX power
 * 6. Start meter or collector demo task(s) based on build role macro
 *
 * The application then runs the background processing loop ::wise_main_proc().
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init();

    wise_radio_wmbus_init(WMBUS_RADIO_INTF);
    wise_radio_set_evt_callback(WMBUS_RADIO_INTF, radioWMbusEventCb);
    wise_radio_set_buffer(WMBUS_RADIO_INTF, &wmbusRFBuffer);
    wise_radio_set_tx_io_mode(WMBUS_RADIO_INTF, CORE_IO_NONBLOCKING);

    wise_wmbus_crypto_init();
    wise_wmbus_crypto_set_key(cryptoKey);

    wise_radio_wmbus_set_mode(WMBUS_RADIO_INTF, wmbusRole, wmbusMode);
    wise_radio_set_tx_pwr(WMBUS_RADIO_INTF, 127);

    printf("Start WMbus %s-%s\n", WMBUS_MODE_STR[wmbusMode], WMBUS_ROLE_STR[wmbusRole]);

#ifdef WMBUS_DEMO_PHY_METER
    _setup_wmbus_demo_meter();
#else
    _setup_wmbus_demo_collector();
#endif

    while (1) {
        wise_main_proc();
    }
}

/* ======================= Meter related functions ======================= */

/**
 * @brief Build and transmit a WMBus meter report frame.
 *
 * Frame is built into @ref mbusTxBuffer by ::wmbus_setup_tx_frame() and then
 * transmitted by ::wise_radio_wmbus_tx_frame().
 *
 * Side effects:
 * - Clears @ref reportFinished before sending
 * - Increments @ref accessNumber after sending
 */
void _wmbus_meter_report(void)
{
    uint16_t frameLen = 0;

    memset(mbusTxBuffer, 0, WMBUS_TX_BUF_LEN);
    frameLen = wmbus_setup_tx_frame(mbusTxBuffer, accessNumber, wmbusAccessbility, true, true);

    reportFinished = 0;
    wise_radio_wmbus_tx_frame(WMBUS_RADIO_INTF, mbusTxBuffer, frameLen);

    accessNumber++;
}

/**
 * @brief Meter task procedure implementing a simple state machine.
 *
 * State behavior:
 * - IDLE:
 *   - When @ref reportRequest is set, send a report and switch to REPORTING
 * - REPORTING:
 *   - When @ref reportFinished is set, start one-shot RX and switch to WAITING_RESP
 * - WAITING_RESP:
 *   - When @ref frameReceived is set:
 *     - If frame valid, dump frame info and go back to IDLE
 *     - If invalid, clear @ref frameReceived and keep waiting (until timeout)
 *   - If timeout exceeds @ref METER_RESP_TIMEOUT, stop RX and return to IDLE
 *
 * @param[in] pData Task context pointer (unused).
 */
void _meter_proc(void *pData)
{
    (void)pData;

    switch (meterState)
    {
        case E_MTR_STAT_IDLE:
            if (reportRequest)
            {
                static uint32_t reportCount = 0;
                uint32_t now = wise_get_tu();

                meterState = E_MTR_STAT_REPORTING;
                reportRequest = 0;
                reportFinished = 0;

                _wmbus_meter_report();

                wise_log_time_info(now);
                debug_print("WMbus meter report %lu\n", ++reportCount);
            }
            break;

        case E_MTR_STAT_REPORTING:
            if (reportFinished)
            {
                meterState = E_MTR_STAT_WAITING_RESP;
                meterRxStartTime = wise_tick_get_counter();
                frameReceived = 0;
                wise_radio_mbus_rx_start(WMBUS_RADIO_INTF, RADIO_RX_ONE_SHOT);
            }
            break;

        case E_MTR_STAT_WAITING_RESP:
            if (frameReceived)
            {
                uint32_t logTime = wise_get_tu();

                if (rxFrameMeta.valid)
                {
                    wise_log_time_info(logTime);
                    debug_print("received response rssi=%d\n", rxFrameMeta.rssi);
                    wmbus_dump_frame_info(mbusRxBuffer, rxFrameMeta.data_len);

                    meterState = E_MTR_STAT_IDLE;
                }
                else {
                    frameReceived = 0;
                }
            }
            else
            {
                if ((wise_tick_get_counter() - meterRxStartTime) >= MS_TO_CLK(METER_RESP_TIMEOUT))
                {
                    uint32_t logTime = wise_get_tu();

                    wise_radio_rx_stop(WMBUS_RADIO_INTF);

                    wise_log_time_info(logTime);
                    debug_print("wait response timeout\n");

                    meterState = E_MTR_STAT_IDLE;
                }
            }
            break;

        default:
            meterState = E_MTR_STAT_IDLE;
            break;
    };
}

/**
 * @brief Periodic scheduler callback to request meter report TX.
 *
 * Sets @ref reportRequest for meter task to send a report frame.
 *
 * @param[in] pData Scheduler context pointer (unused).
 *
 * @note If the previous report has not been processed yet (reportRequest still set),
 *       it prints a "missed" message.
 */
static void _meter_report_timer(void *pData)
{
    (void)pData;

    if (reportRequest == 1) {
        debug_print("WMbus tx missed\n");
    }

    reportRequest = 1;
}

/**
 * @brief Setup meter demo: create meter task and periodic report scheduler.
 *
 * Creates a low-priority task running ::_meter_proc() and enables it.
 * Adds a periodic scheduler entry to trigger ::_meter_report_timer() every
 * @ref METER_REPORT_INTERVAL.
 */
static void _setup_wmbus_demo_meter(void)
{
    int meterTaskId = -1;

    reportRequest = 0;
    meterState = E_MTR_STAT_IDLE;

    meterTaskId = wise_create_task(_meter_proc, NULL, "mtrtsk", E_WISE_TASK_PRI_LOW);
    if (meterTaskId >= 0) {
        wise_set_task_active(meterTaskId);
    }

    wise_schlr_add_periodical(MS_TO_SCHLR_UNIT(METER_REPORT_INTERVAL), _meter_report_timer, NULL);
}

/* ======================= Collector related functions ======================= */

/**
 * @brief Build and transmit a collector response (null frame).
 *
 * Frame is built into @ref mbusTxBuffer by ::wmbus_setup_null_frame() and then
 * transmitted by ::wise_radio_wmbus_tx_frame().
 *
 * Side effects:
 * - Increments @ref accessNumber after sending
 */
void _collector_response(void)
{
    uint16_t frameLen = 0;

    memset(mbusTxBuffer, 0, WMBUS_TX_BUF_LEN);
    frameLen = wmbus_setup_null_frame(mbusTxBuffer, accessNumber, wmbusAccessbility, true, true);

    wise_radio_wmbus_tx_frame(WMBUS_RADIO_INTF, mbusTxBuffer, frameLen);

    accessNumber++;
}

/**
 * @brief Collector task procedure.
 *
 * Behavior:
 * - When @ref frameReceived is set:
 *   - If the received frame is valid:
 *     - Waits for a fixed ACK delay (@ref COLLECTOR_ACK_TIME) compensated by
 *       RX timestamp delta, then transmits response via ::_collector_response().
 *     - Dumps received meter report info via ::wmbus_dump_frame_info().
 *   - Clears flags/metadata and restarts one-shot RX.
 */
static void _collector_proc(void)
{
    if (frameReceived)
    {
        if (rxFrameMeta.valid)
        {
            uint32_t delayTick = 0;
            uint32_t logTime = wise_get_tu();
            uint32_t now = wise_tick_get_counter();

            delayTick = MS_TO_CLK(COLLECTOR_ACK_TIME) - (now - rxFrameMeta.timestamp);
            wise_tick_delay_us(CLK_TO_US(delayTick));

            _collector_response();

            wise_log_time_info(logTime);
            debug_print("received meter report rssi=%d\n", rxFrameMeta.rssi);
            wmbus_dump_frame_info(mbusRxBuffer, rxFrameMeta.data_len);
        }

        frameReceived = 0;
        memset(&rxFrameMeta, 0, sizeof(rxFrameMeta));
        wise_radio_mbus_rx_start(WMBUS_RADIO_INTF, RADIO_RX_ONE_SHOT);
    }
}

/**
 * @brief Setup collector demo: create collector task and start one-shot RX.
 *
 * Creates a low-priority task running ::_collector_proc() and enables it.
 * Starts one-shot RX immediately.
 */
static void _setup_wmbus_demo_collector(void)
{
    int collectorTaskId = -1;

    collectorTaskId = wise_create_task(_collector_proc, NULL, "clttsk", E_WISE_TASK_PRI_LOW);
    if (collectorTaskId >= 0) {
        wise_set_task_active(collectorTaskId);
    }

    frameReceived = 0;
    wise_radio_mbus_rx_start(WMBUS_RADIO_INTF, RADIO_RX_ONE_SHOT);
}

/** @} */ /* end of WISE_EXAMPLE_APP_WMBUS */

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
 * The demo supports two roles selected at build time; exactly one of these
 * must be defined:
 * - Meter role     (define WMBUS_DEMO_PHY_METER, or WMBUS_DEMO_PHY_METER_PD
 *                   for the power-saving meter variant)
 * - Collector role (define WMBUS_DEMO_PHY_OTHER)
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
 * Shell (meter and collector builds only):
 * - `demo start` / `demo stop` switches the automatic meter/collector demo
 *   on and off. The demo starts automatically at boot.
 * - `per [mode] tx [count] [interval_ms]` / `per [mode] rx [count]` runs a
 *   packet error rate test on the given WMBus PHY mode, where mode is the
 *   same index as `wmbus init` in wise_core_trunk (0=S2, 1=T2, 2=C2, 3=R2).
 *   The automatic demo is stopped while the test runs.
 *
 * @note This demo uses non-blocking TX mode via ::wise_radio_set_tx_io_mode().
 * @note RX buffer pool must be provided to radio driver via ::wise_radio_set_buffer().
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "wise_sys_api.h"
#include "wise_radio_wmbus_api.h"
#include "wise_wutmr_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

#include "wise_wmbus_crypto.h"
#include "wmbus_helper.h"

/* Demo banner name (was a -D define; now provided in the demo source). */
#define DEMO_APP_NAME "DEMO W-Mbus"

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
#define DEMO_APP_PROMPT             "WMBUS> "
#define WMBUS_RADIO_INTF            0   /**< Radio interface index used by this demo. */
#define WMBUS_RF_BUFFER_LEN         1536 /**< Radio driver buffer pool size in bytes. */

#ifdef WMBUS_DEMO_PHY_METER_PD
#define METER_REPORT_INTERVAL       15000
#else
#define METER_REPORT_INTERVAL       8000 /**< Meter periodic report interval in milliseconds. */
#endif
#define METER_RESP_TIMEOUT          1000 /**< Meter response wait timeout in milliseconds. */
#define COLLECTOR_ACK_TIME          5    /**< Collector ACK response delay in milliseconds. */

#define WMBUS_TX_BUF_LEN            256  /**< Local TX frame buffer length in bytes. */
#define WMBUS_RX_BUF_LEN            256  /**< Local RX frame buffer length in bytes. */

#define PER_RX_IDLE_TIMEOUT_MS      5000 /**< PER receiver gives up after this long without a frame. */

/** @brief TX I/O mode used by the automatic demo for this build. */
#if (defined WMBUS_DEMO_PHY_METER_PD) || (defined WMBUS_DEMO_PHY_OTHER)
#define DEMO_TX_IO_MODE             CORE_IO_BLOCKING
#else
#define DEMO_TX_IO_MODE             CORE_IO_NONBLOCKING
#endif

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
 * WMBUS_DEMO_PHY_METER / WMBUS_DEMO_PHY_METER_PD select meter role,
 * WMBUS_DEMO_PHY_OTHER selects collector role.
 */
#if (defined WMBUS_DEMO_PHY_METER) || (defined WMBUS_DEMO_PHY_METER_PD)
const wmbus_role_t wmbusRole = WMBUS_ROLE_METER;
#elif (defined WMBUS_DEMO_PHY_OTHER)
const wmbus_role_t wmbusRole = WMBUS_ROLE_OTHER;
#else
#error "Define one of WMBUS_DEMO_PHY_METER, WMBUS_DEMO_PHY_METER_PD or WMBUS_DEMO_PHY_OTHER"
#endif

/** @brief WMBus PHY mode selected for this demo. */
#ifdef WMBUS_DEMO_PHY_METER_PD
const wmbus_mode_t wmbusMode = WMBUS_MODE_T1;
#else
const wmbus_mode_t wmbusMode = WMBUS_MODE_T2;
#endif

/** @brief Demo AES key used by WMBus crypto helper. */
static const uint8_t cryptoKey[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

/** @brief Human-readable mode names for logging. */
static const int8_t* WMBUS_MODE_STR[] = {"S1", "S2", "T1", "T2", "C1", "C2", "R2"};

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

static uint8_t biDirectionMode = 0;

/** @brief WMBus access number (incremented each TX). */
uint8_t accessNumber = 0U;

/** @brief Task id of the meter/collector demo task, -1 until created. */
static int8_t demoTaskId = -1;

/** @brief Scheduler slot of the meter report timer, -1 when not armed. */
static int8_t reportTimerId = -1;

/** @brief Non-zero while the automatic demo is running. */
static uint8_t demoRunning = 0;

/* Forward declarations */
static void _setup_wmbus_demo_meter(void);
static void _stop_wmbus_demo_meter(void);
static void _meter_report_timer(void *pData);
static void _meter_proc(void *pData);
static void _wmbus_meter_report(void);
static void _meter_power_saving_proc();

static void _setup_wmbus_demo_collector(void);
static void _stop_wmbus_demo_collector(void);
static void _collector_proc(void *pData);
static void _collector_response(void);


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
    demo_app_common_init(DEMO_APP_NAME);
    app_shell_init(DEMO_APP_PROMPT);

    wise_radio_wmbus_init(WMBUS_RADIO_INTF);
    wise_radio_set_evt_callback(WMBUS_RADIO_INTF, radioWMbusEventCb);
    wise_radio_set_buffer(WMBUS_RADIO_INTF, &wmbusRFBuffer);
    wise_radio_set_tx_io_mode(WMBUS_RADIO_INTF, DEMO_TX_IO_MODE);

    wise_wmbus_crypto_init();
    wise_wmbus_crypto_set_key(cryptoKey);

    if((wmbusMode == WMBUS_MODE_S2) || (wmbusMode == WMBUS_MODE_T2) || (wmbusMode == WMBUS_MODE_C2) || (wmbusMode == WMBUS_MODE_R2))
        biDirectionMode = 1;

    wise_radio_wmbus_set_mode(WMBUS_RADIO_INTF, wmbusRole, wmbusMode);
    wise_radio_set_tx_pwr(WMBUS_RADIO_INTF, 127);

    printf("Start WMbus %s-%s\n", WMBUS_MODE_STR[wmbusMode], WMBUS_ROLE_STR[wmbusRole]);

#if (defined WMBUS_DEMO_PHY_METER) || (defined WMBUS_DEMO_PHY_OTHER)
#ifdef WMBUS_DEMO_PHY_METER
    printf("Auto demo: meter sends a report every %d ms and waits for the collector response\n", METER_REPORT_INTERVAL);
#else
    printf("Auto demo: collector listens for meter reports and replies to each one\n");
#endif
    printf("Commands:\n");
    printf("  demo start|stop                       switch the auto demo on/off\n");
    printf("  per [mode] tx [count] [interval_ms]   PER test sender\n");
    printf("  per [mode] rx [count]                 PER test receiver (prints PER)\n");
    printf("      [mode] 0=S2, 1=T2, 2=C2, 3=R2     (Ctrl+C aborts a running test)\n");
    printf("  help                                  list all commands\n");
#endif

#ifdef WMBUS_DEMO_PHY_METER
    _setup_wmbus_demo_meter();  
#elif defined WMBUS_DEMO_PHY_OTHER
    _setup_wmbus_demo_collector();
#endif

    while (1) {
        wise_main_proc();

#ifdef WMBUS_DEMO_PHY_METER_PD
        _meter_power_saving_proc();
#endif
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
                frameReceived = 0;

                if(biDirectionMode)
                {
                    meterState = E_MTR_STAT_WAITING_RESP;
                    meterRxStartTime = wise_tick_get_counter();
                    
                    wise_radio_mbus_rx_start(WMBUS_RADIO_INTF, RADIO_RX_ONE_SHOT);
                }
                else
                {
                    meterState = E_MTR_STAT_IDLE;
                }
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
                    //wmbus_dump_frame_info(mbusRxBuffer, rxFrameMeta.data_len);

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

                    wise_radio_wmbus_rx_stop(WMBUS_RADIO_INTF);

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

static void _meter_power_saving_proc()
{
    uint32_t tickNow = wise_wutmr_get_counter();
    static uint32_t prevRptTick = 0;
    static uint32_t reportCount = 0;

    if(((prevRptTick == 0) && (reportCount == 0)) || (tickNow - prevRptTick) >= wise_wutmr_ms_to_clk(METER_REPORT_INTERVAL))
    {
        uint32_t nextRptTick = tickNow + wise_wutmr_ms_to_clk(METER_REPORT_INTERVAL);
        uint32_t sleepTick = 0;
        
        _wmbus_meter_report();
        prevRptTick = tickNow;

        tickNow = wise_wutmr_clk_to_ms(tickNow);
        wise_log_time_info(tickNow * 10);
        debug_print("WMbus meter report %lu\n", ++reportCount);

        sleepTick = nextRptTick - wise_wutmr_get_counter();
        wise_system_sleep(wise_wutmr_clk_to_ms(sleepTick) - 5); //wakeup 5ms earlier
    }
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
 * @brief Start meter demo: activate meter task and arm periodic report scheduler.
 *
 * The low-priority task running ::_meter_proc() is created on first call and
 * re-activated on later calls. Adds a periodic scheduler entry to trigger
 * ::_meter_report_timer() every @ref METER_REPORT_INTERVAL.
 */
static void _setup_wmbus_demo_meter(void)
{
    if (demoRunning) {
        return;
    }

    reportRequest = 0;
    reportFinished = 0;
    frameReceived = 0;
    meterState = E_MTR_STAT_IDLE;

    if (demoTaskId < 0) {
        demoTaskId = wise_create_task(_meter_proc, NULL, "mtrtsk", E_WISE_TASK_PRI_LOW);
    }
    if (demoTaskId >= 0) {
        wise_set_task_active(demoTaskId);
    }

    reportTimerId = wise_schlr_add_periodical(MS_TO_SCHLR_UNIT(METER_REPORT_INTERVAL), _meter_report_timer, NULL);
    demoRunning = 1;
}

/**
 * @brief Stop meter demo: disarm report scheduler, idle meter task and stop RX.
 *
 * Any response wait in progress is abandoned. The task is kept so that
 * ::_setup_wmbus_demo_meter() can re-activate it.
 */
static void _stop_wmbus_demo_meter(void)
{
    if (!demoRunning) {
        return;
    }

    if (reportTimerId >= 0) {
        wise_schlr_remove(reportTimerId);
        reportTimerId = -1;
    }

    if (demoTaskId >= 0) {
        wise_set_task_idle(demoTaskId);
    }

    if (meterState == E_MTR_STAT_WAITING_RESP) {
        wise_radio_wmbus_rx_stop(WMBUS_RADIO_INTF);
    }

    reportRequest = 0;
    meterState = E_MTR_STAT_IDLE;
    demoRunning = 0;
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
 *       If that delay has already elapsed, responds immediately.
 *     - Dumps received meter report info via ::wmbus_dump_frame_info().
 *   - Clears flags/metadata and restarts one-shot RX.
 *
 * @param[in] pData Task context pointer (unused).
 */
static void _collector_proc(void *pData)
{
    (void)pData;

    if (frameReceived)
    {
        if (rxFrameMeta.valid)
        {
            uint32_t logTime = wise_get_tu();
            uint32_t elapsedTick = wise_tick_get_counter() - rxFrameMeta.timestamp;

            if (elapsedTick < MS_TO_CLK(COLLECTOR_ACK_TIME)) {
                wise_tick_delay_us(CLK_TO_US(MS_TO_CLK(COLLECTOR_ACK_TIME) - elapsedTick));
            }

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
 * @brief Start collector demo: activate collector task and start one-shot RX.
 *
 * The low-priority task running ::_collector_proc() is created on first call
 * and re-activated on later calls. Starts one-shot RX immediately.
 */
static void _setup_wmbus_demo_collector(void)
{
    if (demoRunning) {
        return;
    }

    if (demoTaskId < 0) {
        demoTaskId = wise_create_task(_collector_proc, NULL, "clttsk", E_WISE_TASK_PRI_LOW);
    }
    if (demoTaskId >= 0) {
        wise_set_task_active(demoTaskId);
    }

    frameReceived = 0;
    memset(&rxFrameMeta, 0, sizeof(rxFrameMeta));
    wise_radio_mbus_rx_start(WMBUS_RADIO_INTF, RADIO_RX_ONE_SHOT);
    demoRunning = 1;
}

/**
 * @brief Stop collector demo: stop RX and idle the collector task.
 *
 * The task is kept so that ::_setup_wmbus_demo_collector() can re-activate it.
 */
static void _stop_wmbus_demo_collector(void)
{
    if (!demoRunning) {
        return;
    }

    wise_radio_wmbus_rx_stop(WMBUS_RADIO_INTF);

    if (demoTaskId >= 0) {
        wise_set_task_idle(demoTaskId);
    }

    frameReceived = 0;
    memset(&rxFrameMeta, 0, sizeof(rxFrameMeta));
    demoRunning = 0;
}

/* ========================================================================== */
/* demo Command                                                               */
/* ========================================================================== */

#if (defined WMBUS_DEMO_PHY_METER) || (defined WMBUS_DEMO_PHY_OTHER)
/**
 * @brief Shell command: switch the automatic meter/collector demo on or off.
 *
 * Usage:
 * @code
 * demo start
 * demo stop
 * @endcode
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success, negative value on invalid parameters.
 */
static int cmd_demo(int argc, char **argv)
{
    if (argc != 2) {
        printf("Invalid parameters:\r\n");
        printf("Usage: demo [start/stop]\r\n");
        return -1;
    }

    if (strcmp(argv[1], "start") == 0) {
        if (demoRunning) {
            printf("demo already running\r\n");
            return 0;
        }
#ifdef WMBUS_DEMO_PHY_METER
        _setup_wmbus_demo_meter();
#else
        _setup_wmbus_demo_collector();
#endif
        printf("demo started\r\n");
    } else if (strcmp(argv[1], "stop") == 0) {
        if (!demoRunning) {
            printf("demo not running\r\n");
            return 0;
        }
#ifdef WMBUS_DEMO_PHY_METER
        _stop_wmbus_demo_meter();
#else
        _stop_wmbus_demo_collector();
#endif
        printf("demo stopped\r\n");
    } else {
        printf("Invalid parameters:\r\n");
        printf("Usage: demo [start/stop]\r\n");
        return -1;
    }

    return 0;
}

/** Register shell command "demo". */
SHELL_CMD_AUTO(demo, cmd_demo, "Switch automatic demo start/stop");

/* ========================================================================== */
/* per Command (packet error rate test)                                       */
/* ========================================================================== */

/** @brief Number of valid frames received by the PER receiver. */
static volatile uint32_t perRxGoodCount = 0;

/** @brief Number of RX_ERR / invalid frames seen by the PER receiver. */
static volatile uint32_t perRxErrCount = 0;

/** @brief Number of valid frames whose access number was not the expected one. */
static volatile uint32_t perSeqErrCount = 0;

/** @brief Sum of RSSI over valid frames, for the average in the report. */
static volatile int32_t perRssiSum = 0;

/** @brief Access number expected in the next frame, valid once a frame was seen. */
static volatile uint8_t perRxExpect = 0;
static volatile uint8_t perRxExpectValid = 0;

/** @brief Set by the PER RX callback when one-shot RX must be restarted. */
static volatile uint8_t perRxRestart = 0;

/**
 * @brief Re-initialise the WMBus radio with a new PHY mode.
 *
 * ::wise_radio_wmbus_set_mode() refuses to run twice on a live interface, so
 * the radio is torn down and brought up again the same way ::main() does it.
 * The role stays the build-time @ref wmbusRole.
 *
 * @param[in] mode     WMBus PHY mode to configure.
 * @param[in] txIoMode TX I/O mode to apply after re-init.
 * @param[in] evtCb    Radio event callback to register after re-init.
 *
 * @return WISE_SUCCESS on success, otherwise the failing status.
 */
static int8_t _wmbus_radio_setup(wmbus_mode_t mode, CORE_IO_MODE_T txIoMode, WISE_RADIO_EVT_CB evtCb)
{
    int8_t status;

    wise_radio_wmbus_rx_stop(WMBUS_RADIO_INTF);
    wise_radio_deinit(WMBUS_RADIO_INTF);
    wise_radio_wmbus_deinit(WMBUS_RADIO_INTF);

    status = wise_radio_wmbus_init(WMBUS_RADIO_INTF);
    if (status != WISE_SUCCESS) {
        return status;
    }

    wise_radio_set_evt_callback(WMBUS_RADIO_INTF, evtCb);
    wise_radio_set_buffer(WMBUS_RADIO_INTF, &wmbusRFBuffer);
    wise_radio_set_tx_io_mode(WMBUS_RADIO_INTF, txIoMode);

    status = wise_radio_wmbus_set_mode(WMBUS_RADIO_INTF, wmbusRole, mode);
    if (status != WISE_SUCCESS) {
        return status;
    }

    wise_radio_set_tx_pwr(WMBUS_RADIO_INTF, 127);

    return WISE_SUCCESS;
}

/**
 * @brief Radio event callback used while the PER receiver is running.
 *
 * Counts valid frames, RSSI, and access-number gaps. The frame is released
 * and @ref perRxRestart is raised so the command loop re-arms one-shot RX.
 *
 * @param[in] evt Bitmask of radio events.
 */
static void radioPEREventCb(WISE_RADIO_EVT_T evt)
{
    if ((evt & WISE_RADIO_EVT_RX_FRAME) || (evt & WISE_RADIO_EVT_RX_ERR))
    {
        WISE_RX_META_T meta;
        uint32_t bufAddr = 0;

        if ((evt & WISE_RADIO_EVT_RX_FRAME) &&
            (WISE_SUCCESS == wise_radio_get_rx_frame_info(WMBUS_RADIO_INTF, &bufAddr, &meta)) &&
            meta.valid &&
            (meta.data_len >= (sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t))))
        {
            const WMBUS_stl_header_t *stl = (const WMBUS_stl_header_t *)((uint8_t *)bufAddr + sizeof(WMBUS_dll_header_t));

            perRxGoodCount++;
            perRssiSum += meta.rssi;

            if (perRxExpectValid && (stl->accessNumber != perRxExpect)) {
                perSeqErrCount++;
                debug_print("X");
            } else {
                debug_print("<");
            }

            perRxExpect      = (uint8_t)(stl->accessNumber + 1);
            perRxExpectValid = 1;
        }
        else
        {
            perRxErrCount++;
        }

        wise_radio_release_rx_frame(WMBUS_RADIO_INTF);
        perRxRestart = 1;
    }
}

/**
 * @brief PER sender: transmit @p count unencrypted SND_NR frames.
 *
 * The STL access number carries the sequence (0, 1, 2, ...) so the receiver
 * can detect gaps. Ctrl+C aborts early.
 *
 * @param[in] count      Number of frames to send.
 * @param[in] intervalMs Delay between frames in milliseconds.
 */
static void _per_run_tx(uint32_t count, uint32_t intervalMs)
{
    uint32_t sent = 0;

    printf("PER tx: %lu frames, interval %lu ms, Ctrl+C to abort\r\n",
           (unsigned long)count, (unsigned long)intervalMs);

    for (sent = 0; sent < count; sent++) {
        uint16_t frameLen;

        if (shell_poll_break()) {
            printf("\r\naborted\r\n");
            break;
        }

        memset(mbusTxBuffer, 0, WMBUS_TX_BUF_LEN);
        frameLen = wmbus_setup_tx_frame(mbusTxBuffer, (uint8_t)sent, wmbusAccessbility, true, false);

        if (WISE_SUCCESS != wise_radio_wmbus_tx_frame(WMBUS_RADIO_INTF, mbusTxBuffer, frameLen)) {
            debug_print("!");
        } else {
            debug_print(">");
        }

        wise_tick_delay_ms(intervalMs);
    }

    printf("\r\nPER tx finished: %lu/%lu frames sent\r\n", (unsigned long)sent, (unsigned long)count);
}

/**
 * @brief PER receiver: listen until @p expected frames arrive, then report.
 *
 * Stops on: expected count reached, @ref PER_RX_IDLE_TIMEOUT_MS without a
 * frame after the first one, or Ctrl+C. Prints received/expected, average
 * RSSI, sequence gaps and PER.
 *
 * @param[in] expected Number of frames the sender is expected to transmit.
 */
static void _per_run_rx(uint32_t expected)
{
    uint32_t lastRxTu = wise_get_tu();
    uint32_t lastGoodCount = 0;
    uint32_t lost;
    uint32_t perX100;

    perRxGoodCount   = 0;
    perRxErrCount    = 0;
    perSeqErrCount   = 0;
    perRssiSum       = 0;
    perRxExpect      = 0;
    perRxExpectValid = 0;
    perRxRestart     = 0;

    printf("PER rx: expecting %lu frames, Ctrl+C to stop\r\n", (unsigned long)expected);

    wise_radio_mbus_rx_start(WMBUS_RADIO_INTF, RADIO_RX_ONE_SHOT);

    while (1) {
        if (shell_poll_break()) {
            printf("\r\nstopped by user\r\n");
            break;
        }

        if (perRxRestart) {
            perRxRestart = 0;
            wise_radio_mbus_rx_start(WMBUS_RADIO_INTF, RADIO_RX_ONE_SHOT);
        }

        if (perRxGoodCount >= expected) {
            break;
        }

        if (perRxGoodCount != lastGoodCount) {
            lastGoodCount = perRxGoodCount;
            lastRxTu = wise_get_tu();
        } else if ((lastGoodCount > 0) &&
                   ((wise_get_tu() - lastRxTu) >= MS_TO_SCHLR_UNIT(PER_RX_IDLE_TIMEOUT_MS))) {
            printf("\r\nrx timeout\r\n");
            break;
        }
    }

    wise_radio_wmbus_rx_stop(WMBUS_RADIO_INTF);

    lost = (perRxGoodCount >= expected) ? 0 : (expected - perRxGoodCount);
    perX100 = (expected > 0) ? (uint32_t)(((uint64_t)lost * 10000U) / expected) : 0;

    printf("\r\nPER test result:\r\n");
    printf("    %lu/%lu frames received\r\n", (unsigned long)perRxGoodCount, (unsigned long)expected);
    printf("    rx error frames: %lu, sequence gaps: %lu\r\n",
           (unsigned long)perRxErrCount, (unsigned long)perSeqErrCount);
    if (perRxGoodCount > 0) {
        printf("    average rssi: %d\r\n", (int)(perRssiSum / (int32_t)perRxGoodCount));
    }
    printf("    PER: %lu.%02lu%%\r\n", (unsigned long)(perX100 / 100), (unsigned long)(perX100 % 100));
}

/**
 * @brief Mode index table for the `per` command.
 *
 * Same numbering as `wmbus init [mode] [role]` in the wise_core_trunk shell:
 * 0=S2, 1=T2, 2=C2, 3=R2.
 */
static const wmbus_mode_t PER_MODE_TABLE[] = {WMBUS_MODE_S2, WMBUS_MODE_T2, WMBUS_MODE_C2, WMBUS_MODE_R2};

/**
 * @brief Parse the numeric mode index used by the `per` command.
 *
 * @param[in]  arg  Mode index string from the command line.
 * @param[out] mode Parsed mode on success.
 *
 * @return 0 on success, -1 if the index is out of range or not a number.
 */
static int _per_parse_mode(const char *arg, wmbus_mode_t *mode)
{
    char *end = NULL;
    unsigned long idx;

    if ((arg == NULL) || (*arg == '\0')) {
        return -1;
    }

    idx = strtoul(arg, &end, 10);
    if ((*end != '\0') || (idx >= (sizeof(PER_MODE_TABLE) / sizeof(PER_MODE_TABLE[0])))) {
        return -1;
    }

    *mode = PER_MODE_TABLE[idx];
    return 0;
}

/**
 * @brief Shell command: packet error rate test.
 *
 * Usage:
 * @code
 * per <mode> tx <count> <interval_ms>
 * per <mode> rx <count>
 *   mode: 0=S2, 1=T2, 2=C2, 3=R2
 * @endcode
 *
 * The automatic demo is stopped first. The radio is re-initialised with the
 * requested PHY mode for the test and restored to the demo mode afterwards;
 * the demo is restarted if it was running before.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success, negative value on invalid parameters or radio error.
 */
static int cmd_per(int argc, char **argv)
{
    wmbus_mode_t mode;
    uint32_t count;
    uint32_t interval = 0;
    uint8_t isTx;
    uint8_t demoWasRunning = demoRunning;
    int ret = 0;

    if ((argc < 4) || (_per_parse_mode(argv[1], &mode) != 0)) {
        goto usage;
    }

    if (strcmp(argv[2], "tx") == 0) {
        if (argc != 5) {
            goto usage;
        }
        isTx = 1;
        interval = strtoul(argv[4], NULL, 10);
    } else if (strcmp(argv[2], "rx") == 0) {
        if (argc != 4) {
            goto usage;
        }
        isTx = 0;
    } else {
        goto usage;
    }

    count = strtoul(argv[3], NULL, 10);
    if (count == 0) {
        goto usage;
    }

    if (demoWasRunning) {
#ifdef WMBUS_DEMO_PHY_METER
        _stop_wmbus_demo_meter();
#else
        _stop_wmbus_demo_collector();
#endif
    }

    /* Blocking TX keeps the send loop simple regardless of the build's demo I/O mode. */
    if (WISE_SUCCESS != _wmbus_radio_setup(mode, CORE_IO_BLOCKING, radioPEREventCb)) {
        printf("failed to configure WMbus mode %s\r\n", WMBUS_MODE_STR[mode]);
        ret = -1;
        goto restore;
    }

    printf("WMbus %s-%s\r\n", WMBUS_MODE_STR[mode], WMBUS_ROLE_STR[wmbusRole]);

    if (isTx) {
        _per_run_tx(count, interval);
    } else {
        _per_run_rx(count);
    }

restore:
    if (WISE_SUCCESS != _wmbus_radio_setup(wmbusMode, DEMO_TX_IO_MODE, radioWMbusEventCb)) {
        printf("failed to restore WMbus mode %s\r\n", WMBUS_MODE_STR[wmbusMode]);
        return -1;
    }

    if (demoWasRunning) {
#ifdef WMBUS_DEMO_PHY_METER
        _setup_wmbus_demo_meter();
#else
        _setup_wmbus_demo_collector();
#endif
    }

    return ret;

usage:
    printf("Invalid parameters:\r\n");
    printf("Usage: per [mode] tx [count] [interval_ms]\r\n");
    printf("       per [mode] rx [count]\r\n");
    printf("       [mode] 0=S2, 1=T2, 2=C2, 3=R2\r\n");
    return -1;
}

/** Register shell command "per". */
SHELL_CMD_AUTO(per, cmd_per, "Packet error rate test: per [mode] [tx/rx] ...");
#endif

/** @} */ /* end of WISE_EXAMPLE_APP_WMBUS */

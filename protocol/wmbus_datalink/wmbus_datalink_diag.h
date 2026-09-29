/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef WMBUS_DATALINK_DIAG_H
#define WMBUS_DATALINK_DIAG_H

#include <stdint.h>

/* Customer builds disable buffered logs unless explicitly enabled. */
#ifndef WMBUS_LINK_DIAGNOSTICS
#define WMBUS_LINK_DIAGNOSTICS 0
#endif

#ifndef WMBUS_LINK_LA_MEASUREMENT
#define WMBUS_LINK_LA_MEASUREMENT 0
#endif

#ifndef WMBUS_LINK_TIMING_TEST
#define WMBUS_LINK_TIMING_TEST 0
#endif

#if WMBUS_LINK_LA_MEASUREMENT && WMBUS_LINK_TIMING_TEST
#error "WMBUS_LINK_LA_MEASUREMENT requires WMBUS_LINK_TIMING_TEST = 0"
#endif

#if WMBUS_LINK_TIMING_TEST && !WMBUS_LINK_DIAGNOSTICS
#error "WMBUS_LINK_TIMING_TEST requires WMBUS_LINK_DIAGNOSTICS = 1"
#endif

#ifndef MCU_CLOCK_ESTIMATE_TEST
#define MCU_CLOCK_ESTIMATE_TEST 0
#endif

#if MCU_CLOCK_ESTIMATE_TEST
#define MCU_CLOCK_ESTIMATE_GPIO_PIN 12
#define MCU_CLOCK_ESTIMATE_TICK_DELTA 400000000UL
#endif

typedef enum {
    GW_RX2TX_LOG_END_NONE = 0,
    GW_RX2TX_LOG_END_SND_NKE
} GW_RX2TX_LOG_END_REASON_T;

#if !WMBUS_LINK_DIAGNOSTICS || WMBUS_LINK_LA_MEASUREMENT
#define wmbus_link_gw_rx2tx_log_begin(generation) ((void)(generation))
#define wmbus_link_gw_rx2tx_log_reset() ((void)0)
#define wmbus_link_gw_rx2tx_log_capture_rx(...) ((void)0)
#define wmbus_link_gw_rx2tx_log_mark_session_start(...) ((void)0)
#define wmbus_link_gw_rx2tx_log_mark_session_end(...) ((void)0)
#define wmbus_link_gw_rx2tx_log_mark_tx_in_flight() ((void)0)
#define wmbus_link_gw_rx2tx_log_flush_no_tx() (0U)
#define wmbus_link_gw_rx2tx_log_flush_tx_done(...) ((void)0)
#else
void wmbus_link_gw_rx2tx_log_begin(uint32_t generation);
void wmbus_link_gw_rx2tx_log_reset(void);
void wmbus_link_gw_rx2tx_log_capture_rx(uint32_t dev_id,
                                        uint8_t function_code,
                                        uint8_t ell_access_number,
                                        uint8_t stl_access_number,
                                        uint8_t fcv,
                                        uint8_t fcb);
void wmbus_link_gw_rx2tx_log_mark_session_start(uint8_t state);
void wmbus_link_gw_rx2tx_log_mark_session_end(uint8_t state,
                                              uint8_t reason);
void wmbus_link_gw_rx2tx_log_mark_tx_in_flight(void);
uint8_t wmbus_link_gw_rx2tx_log_flush_no_tx(void);
void wmbus_link_gw_rx2tx_log_flush_tx_done(uint8_t state,
                                           uint32_t dev_id,
                                           uint8_t tx_function_code,
                                           uint8_t tx_ell_access_number,
                                           uint8_t tx_ltl_access_number,
                                           uint8_t tx_fcv,
                                           uint8_t tx_fcb,
                                           uint8_t rx_function_code,
                                           uint8_t rx_ell_access_number,
                                           uint8_t rx_stl_access_number,
                                           uint8_t rx_fcv,
                                           uint8_t rx_fcb);
#endif

void wmbus_link_mcu_clock_estimate_test(void);

typedef enum {
    WMBUS_TIMING_SYNC_ISR = 0,
    WMBUS_TIMING_EARLY_ARM_DONE,
    WMBUS_TIMING_EARLY_SAMPLE_ENTER,
    WMBUS_TIMING_EARLY_SAMPLE_DONE,
    WMBUS_TIMING_EARLY_PREP_START,
    WMBUS_TIMING_EARLY_PREP_DONE,
    WMBUS_TIMING_EARLY_CANDIDATE_READY,
    WMBUS_TIMING_EARLY_TX_FRAME_READY,
    WMBUS_TIMING_EARLY_RF_PACK_START,
    WMBUS_TIMING_EARLY_RF_PACK_END,
    WMBUS_TIMING_RX_ISR_ENTER,
    WMBUS_TIMING_RX_EVENT_POSTED,
    WMBUS_TIMING_EVENT_DISPATCH,
    WMBUS_TIMING_GW_FSM_ENTER,
    WMBUS_TIMING_GW_PRESELECT_DONE,
    WMBUS_TIMING_GW_RX_CASE,
    WMBUS_TIMING_PARSE_START,
    WMBUS_TIMING_PARSE_END,
    WMBUS_TIMING_SCHED_ADD_START,
    WMBUS_TIMING_SCHED_ADD_END,
    WMBUS_TIMING_GEN_START,
    WMBUS_TIMING_GEN_END,
    WMBUS_TIMING_SYNC_TX_ENTRY,
    WMBUS_TIMING_TX_API_ENTER,
    WMBUS_TIMING_TX_PACK_START,
    WMBUS_TIMING_TX_PACK_END,
    WMBUS_TIMING_TX_CONFIG_START,
    WMBUS_TIMING_TX_CONFIG_END,
    WMBUS_TIMING_RADIO_TX_FRAME_ENTER,
    WMBUS_TIMING_RADIO_TX_FRAME_RETURN,
    WMBUS_TIMING_TX_API_RETURN,
    WMBUS_TIMING_TX_DONE_ISR,
    WMBUS_TIMING_COUNT
} WMBUS_TIMING_POINT_T;

typedef enum {
    WMBUS_TIMING_TX_SOURCE_NONE = 0,
    WMBUS_TIMING_TX_SOURCE_EARLY_COMMIT,
    WMBUS_TIMING_TX_SOURCE_PREENC_DATA,
    WMBUS_TIMING_TX_SOURCE_PREENC_NULL,
    WMBUS_TIMING_TX_SOURCE_PREENC_NKE,
    WMBUS_TIMING_TX_SOURCE_GEN_DATA,
    WMBUS_TIMING_TX_SOURCE_GEN_NULL,
    WMBUS_TIMING_TX_SOURCE_GEN_NKE,
    WMBUS_TIMING_TX_SOURCE_RETRY_LAST_TX
} WMBUS_TIMING_TX_SOURCE_T;

#if WMBUS_LINK_TIMING_TEST
void wmbus_link_timing_begin(uint32_t generation);
uint8_t wmbus_link_timing_get_capture_id(uint32_t *generation,
                                         uint32_t *sequence);
void wmbus_link_timing_mark(uint8_t timing_id);
void wmbus_link_timing_set_tx_source(uint8_t tx_source);
void wmbus_link_timing_set_enabled(uint8_t enabled);
uint8_t wmbus_link_timing_is_enabled(void);
uint8_t wmbus_link_timing_is_capture_active(void);
void wmbus_link_dump_timing_check(void);
#else
#define wmbus_link_timing_begin(generation) ((void)(generation))
#define wmbus_link_timing_get_capture_id(generation, sequence) (0U)
#define wmbus_link_timing_mark(timing_id) ((void)(timing_id))
#define wmbus_link_timing_set_tx_source(tx_source) ((void)(tx_source))
#define wmbus_link_timing_set_enabled(enabled) ((void)(enabled))
#define wmbus_link_timing_is_enabled() (0U)
#define wmbus_link_timing_is_capture_active() (0U)
#define wmbus_link_dump_timing_check() ((void)0)
#endif

#endif

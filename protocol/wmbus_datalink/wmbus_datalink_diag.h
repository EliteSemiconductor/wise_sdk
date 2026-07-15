/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef WMBUS_DATALINK_DIAG_H
#define WMBUS_DATALINK_DIAG_H

#include <stdint.h>

#ifndef MCU_CLOCK_ESTIMATE_TEST
#define MCU_CLOCK_ESTIMATE_TEST 0
#endif

#if MCU_CLOCK_ESTIMATE_TEST
#define MCU_CLOCK_ESTIMATE_GPIO_PIN 12
#define MCU_CLOCK_ESTIMATE_TICK_DELTA 400000000UL
#endif

#ifndef WMBUS_LINK_TIMING_TEST
#define WMBUS_LINK_TIMING_TEST 0
#endif

void wmbus_link_mcu_clock_estimate_test(void);

typedef enum {
    WMBUS_TIMING_RX_ISR_ENTER = 0,
    WMBUS_TIMING_RX_INFO_DONE,
    WMBUS_TIMING_RX_RELEASE_DONE,
    WMBUS_TIMING_RX_EVENT_POSTED,
    WMBUS_TIMING_EVENT_DISPATCH,
    WMBUS_TIMING_GW_FSM_ENTER,
    WMBUS_TIMING_GW_PRESELECT_DONE,
    WMBUS_TIMING_GW_RX_CASE,
    WMBUS_TIMING_PARSE_CALL,
    WMBUS_TIMING_PARSE_START,
    WMBUS_TIMING_PARSE_DLL_DONE,
    WMBUS_TIMING_PARSE_GW_ADMIT_DONE,
    WMBUS_TIMING_PARSE_CONN_DONE,
    WMBUS_TIMING_PARSE_CONTEXT_DONE,
    WMBUS_TIMING_PARSE_ELL_DONE,
    WMBUS_TIMING_PARSE_AFL_DONE,
    WMBUS_TIMING_PARSE_TPL_DONE,
    WMBUS_TIMING_PARSE_QUEUE_DONE,
    WMBUS_TIMING_PARSE_PAYLOAD_DONE,
    WMBUS_TIMING_PARSE_END,
    WMBUS_TIMING_PARSE_RETURN,
    WMBUS_TIMING_FAC_TIMER_REMOVED,
    WMBUS_TIMING_GEN_START,
    WMBUS_TIMING_GEN_END,
    WMBUS_TIMING_SCHED_ADD_START,
    WMBUS_TIMING_SCHED_ADD_END,
    WMBUS_TIMING_SYNC_TX_ENTRY,
    WMBUS_TIMING_TX_API_ENTER,
    WMBUS_TIMING_TX_API_RETURN,
    WMBUS_TIMING_TX_DONE_ISR,
    WMBUS_TIMING_COUNT
} WMBUS_TIMING_POINT_T;

#if WMBUS_LINK_TIMING_TEST || defined(WMBUS_DATALINK_DIAG_IMPL)
void wmbus_link_timing_mark(uint8_t timing_id);
void wmbus_link_timing_set_enabled(uint8_t enabled);
uint8_t wmbus_link_timing_is_enabled(void);
uint8_t wmbus_link_timing_is_capture_active(void);
void wmbus_link_dump_timing_check(void);
#else
#define wmbus_link_timing_mark(timing_id) ((void)0)
#define wmbus_link_timing_set_enabled(enabled) ((void)0)
#define wmbus_link_timing_is_enabled() (0U)
#define wmbus_link_timing_is_capture_active() (0U)
#define wmbus_link_dump_timing_check() ((void)0)
#endif

#endif

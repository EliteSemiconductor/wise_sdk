/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __PLATFORM_INTF_RF_H
#define __PLATFORM_INTF_RF_H

#include "es_platform_components.h"

#include <stdio.h>
#include "wise.h"
#include "util.h"
#include "wmbus_datalink_dll.h"

void radioDebug(void);
void platform_wmbus_rf_init(uint8_t role, uint8_t mode);
void platform_rf_deinit(void);
#ifdef WMBUS_FRAME_TEST
#define PLATFORM_FRAME_TEST_EVENT_TX_DONE  0x00000040UL
#define PLATFORM_FRAME_TEST_EVENT_TX_ERROR 0x00000080UL
#define PLATFORM_FRAME_TEST_EVENT_RX_DONE  0x00000100UL
#define PLATFORM_FRAME_TEST_EVENT_RX_ERROR 0x00000200UL

int8_t platform_frame_test_rf_init(uint8_t role, uint8_t mode);
void platform_frame_test_rf_deinit(void);
int8_t platform_frame_test_rf_tx_frame(const uint8_t *frame, uint16_t length);
void platform_frame_test_rf_stop_tx(void);
int8_t platform_frame_test_rf_start_rx(void);
void platform_frame_test_rf_stop_rx(void);
int8_t platform_frame_test_rf_get_rx_frame(uint8_t *frame, uint16_t capacity,
                                    uint16_t *length, int16_t *rssi);
#endif
void radioInit802154(void);
void platform_rf_start_rx(void);
void platform_rf_stop_rx(void);
void platform_rf_tx_frame(uint8_t* pFrame, uint16_t length);
int8_t platform_rf_test_link_loss(uint32_t duration_ms);
int8_t platform_rf_pack_frame(const uint8_t *dll_frame,
                              uint16_t dll_length,
                              uint8_t *packed_frame,
                              uint16_t packed_capacity,
                              uint16_t *packed_length);
int8_t platform_rf_finalize_wmbus_frame_a_first_block(
    uint8_t *packed_frame,
    uint16_t packed_length,
    const uint8_t *dll_header);
int8_t platform_rf_build_wmbus_frame_a_first_block_variants(
    const uint8_t *dll_header_fcb0,
    const uint8_t *dll_header_fcb1,
    uint8_t *variant_fcb0,
    uint8_t *variant_fcb1,
    uint16_t variant_capacity,
    uint16_t *variant_length);
int8_t platform_rf_stage_packed_frame(const uint8_t *packed_frame,
                                      uint16_t packed_length);
int8_t platform_rf_start_staged_tx(void);
void platform_rf_set_mode(uint8_t role, uint8_t mode);
int8_t platform_rf_set_max_frame_len(uint16_t maxLen);
void platform_rf_set_txpwr(uint8_t _inputpwr);
uint8_t platform_rf_get_txpwr(void);

uint8_t isRadioConfigured(void);
uint8_t isRxOn(void);
uint32_t platform_rf_get_frequence(void);
uint8_t isInputPwrVaild(uint8_t _inputpwr);

void radioSetRxLog(uint8_t _val);
void platform_rf_early_rx_sync_isr(void);
void platform_rf_early_rx_arm(void);
void platform_rf_early_rx_dump(void);
void platform_rf_early_rx_reset(void);
int8_t platform_rf_early_rx_set_delay(uint32_t delay_us);
uint32_t platform_rf_early_rx_get_delay(void);
uint8_t platform_rf_early_rx_get_snapshot(uint32_t *generation,
                                          WMBUS_dll_header_t *dll_header);
uint32_t platform_rf_early_rx_get_generation(void);
#endif

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

void radioDebug(void);
void platform_wmbus_rf_init(uint8_t role, uint8_t mode);
void platform_rf_deinit(void);
void platform_dtm_rf_init(uint8_t role, uint8_t mode);
void radioInit802154(void);
void platform_rf_start_rx(void);
void platform_rf_stop_rx(void);
void platform_rf_tx_frame(uint8_t* pFrame, uint8_t length);
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
void platform_rf_early_rx_prepare_validate(void);
void platform_rf_early_rx_capture_final(const uint8_t *frame, uint16_t length,
                                        uint8_t valid);
void platform_rf_early_rx_dump(void);
void platform_rf_early_rx_reset(void);
int8_t platform_rf_early_rx_set_delay(uint32_t delay_us);
uint32_t platform_rf_early_rx_get_delay(void);
uint8_t platform_rf_early_rx_get_header(uint32_t *generation,
                                        uint32_t *buffer_addr,
                                        uint8_t *header,
                                        uint16_t header_len);
uint32_t platform_rf_early_rx_get_generation(void);
#endif

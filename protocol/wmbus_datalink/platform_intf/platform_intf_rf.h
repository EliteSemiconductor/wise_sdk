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
#endif

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __FMT_PRINTF_H
#define __FMT_PRINTF_H

#include <stdio.h>
#include "wise_core.h"
#include "wise.h"
#include "wise_radio_api.h"

extern const char *const modString[];     // ex: "2-FSK","GFSK",...
extern const char *const drString[];      // ex: "50","100","200","500"
extern const char *const onOffString[];   // "OFF","ON"
extern const char *const paString[];      // ex: "Low voltage (10 dBm)",...
extern const char *const matchString[];   // ex: "868 MHz","915 MHz",...
extern const char *const operateString[]; // ex: "Transparent","802.15.4","BLE"
extern const char *const fmtLenTypeString[]; // ex: "Fixed","Variable"
extern const char *const wmbus_modeString[];
extern const char *const wmbus_roleString[]; // ex: "Other", "Meter"
extern const char *const wmbus_actString[]; // ex: RX", "TX
extern const char *gpio_pin_func_to_str(uint8_t func);

void dbg_box_begin(const char *title);
void dbg_box_end(void);
void dbg_section(const char *title);
void dbg_field(const char *label, const char *value);
const char *fmt_int(int v);
const char *fmt_hz_u32(uint32_t hz);
const char *fmt_bps_u32(uint32_t bps);
const char *fmt_bytes_u32(uint32_t n);
const char *fmt_hex8(uint8_t v);
const char *fmt_hex32(uint32_t v);
const char *fmt_hex16(uint16_t v);
const char *onOff(bool en);

void fmt_radio_debug(int8_t radioIntf, uint8_t actChIdx, uint8_t rxOn, const WISE_RADIO_CFG_T *cfg, const WISE_RADIO_PKT_FMT_T *fmt);

#endif //__FUNC_RF_H

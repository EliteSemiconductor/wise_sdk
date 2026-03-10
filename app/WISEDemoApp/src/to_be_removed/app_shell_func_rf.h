/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __APP_SHELL_FUNC_RF_H
#define __APP_SHELL_FUNC_RF_H

#include <stdio.h>
#include "wise.h"
#include "wise_core.h"

void radioDebug(void);
void radioInit(void);
void radioInit802154(void);
void radioRxOn(void);
void radioRxOff(void);
uint8_t isRadioConfigured(void);
uint8_t isRxOn(void);
void radioTestTxFrame(int testTxLen, uint8_t patternStart);
void radioTestSend(int testCount);
uint8_t isTxProcEnd(void);
void radioSetChannel(uint8_t channelIdx);
uint8_t radioGetChannel(void);
uint32_t radioGetFreq(void);
uint8_t radioGetValidChannelNumber(void);
uint8_t isInputPwrVaild(uint8_t _inputpwr);
void radioSetTxPwr(uint8_t _inputpwr);
uint8_t radioGetTxPwr(void);
void radioSetRxLog(uint8_t _val);
int8_t radioGetRssi();
void radioSingleToneOn(uint8_t pwrLevel);
void radioSingleToneOff();
void setPanId(uint16_t _panid);
void setShortAddress(uint16_t _saddr);
void setLongAddress(uint8_t *_laddr);

void startPERTest(uint8_t role, uint32_t testCount);
#endif

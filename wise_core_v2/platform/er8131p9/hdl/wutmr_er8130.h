/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __WUTMR_ER8130_H
#define __WUTMR_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

void wutmr_set_cnt_er8130(uint32_t wu_cnt);
uint32_t wutmr_get_cnt_er8130(void);
void wutmr_set_enable_er8130(uint8_t period_en);
void wutmr_set_disable_er8130(void);
uint32_t wutmr_get_int_status_er8130(void);
void wutmr_clear_int_status_er8130(void);
void wutmr_set_int_enable_er8130(void);
void wutmr_set_int_disable_er8130(void);

#endif

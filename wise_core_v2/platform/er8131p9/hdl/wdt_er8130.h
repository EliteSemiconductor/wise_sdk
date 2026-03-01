/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __WDT_ER8130_H
#define __WDT_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

void wdt_config_er8130(uint32_t period, uint32_t reset_en);
void wdt_start_er8130(void);
void wdt_stop_er8130(void);
uint32_t wdt_get_period_er8130(void);
void wdt_set_period_er8130(uint32_t period);
void wdt_clear_int_flag_er8130(void);
uint32_t wdt_get_status_er8130(void);

#endif /* __WDT_ER8130_H */

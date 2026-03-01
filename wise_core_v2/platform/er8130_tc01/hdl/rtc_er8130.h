/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __RTC_ER8130_H
#define __RTC_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

void rtc_set_time_er8130(uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
void rtc_get_time_er8130(uint8_t *day, uint8_t *hour, uint8_t *min,
                         uint8_t *sec);
void rtc_enable_er8130(void);
void rtc_disable_er8130(void);
uint8_t rtc_is_write_done_er8130(void);
void rtc_set_int_source_er8130(uint32_t int_sour);
uint32_t rtc_get_int_status_er8130(void);
void rtc_clear_int_status_er8130(uint8_t sour);
void rtc_set_alarm_er8130(uint8_t channel, uint8_t hour, uint8_t min,
                          uint8_t sec);

#endif /* __RTC_ER8130_H */

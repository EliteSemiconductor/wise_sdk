/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_RTC_H
#define __HAL_INTF_RTC_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} HAL_RTC_TIME_T;

typedef struct {
    uint8_t day;
    HAL_RTC_TIME_T time;
} HAL_RTC_CNT_T;

typedef struct {
    uint8_t channel;
    uint8_t enable;
    HAL_RTC_TIME_T time;
    uint8_t int_en;
} HAL_RTC_ALM_CFG_T;

typedef enum {
    RTC_EVENT_ALARM,
    RTC_EVENT_SECOND,
    RTC_EVENT_MINUTE,
    RTC_EVENT_HOUR,
    RTC_EVENT_DAY,
    RTC_MAX_EVENTS,
} RTC_CB_EVENT_T;

#define RTC_ALARM0_INT_MASK (1UL << 2)
#define RTC_ALARM1_INT_MASK (1UL << 13)
#define RTC_ALARM2_INT_MASK (1UL << 14)
#define RTC_ALARM3_INT_MASK (1UL << 15)
#define RTC_ALARM4_INT_MASK (1UL << 16)
#define RTC_ALARM5_INT_MASK (1UL << 17)

void hal_intf_rtc_probe(void);
void hal_intf_rtc_start(void);
void hal_intf_rtc_stop(void);
HAL_STATUS hal_intf_rtc_get_time(HAL_RTC_CNT_T *hal_time);
HAL_STATUS hal_intf_rtc_set_time(HAL_RTC_CNT_T *hal_time);
HAL_STATUS hal_intf_rtc_set_alarm(HAL_RTC_ALM_CFG_T *hal_alarm_cfg);
HAL_STATUS hal_intf_rtc_register_callback(RTC_CB_EVENT_T event, CALLBACK_T cb,
                                           void *context);
HAL_STATUS hal_intf_rtc_unregister_callback(RTC_CB_EVENT_T event);

#endif /* __HAL_INTF_RTC_H */

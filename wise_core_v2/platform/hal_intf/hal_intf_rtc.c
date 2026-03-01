/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_rtc.h"
#include "drv/hal_drv_rtc.h"

#define RTC_GET_ALM_INT_IDX(channel)                                           \
    ((channel) == 0   ? RTC_ALARM0_INT_MASK                                    \
     : (channel) == 1 ? RTC_ALARM1_INT_MASK                                    \
     : (channel) == 2 ? RTC_ALARM2_INT_MASK                                    \
     : (channel) == 3 ? RTC_ALARM3_INT_MASK                                    \
     : (channel) == 4 ? RTC_ALARM4_INT_MASK                                    \
                      : RTC_ALARM5_INT_MASK)

void hal_intf_rtc_start(void)
{
    hal_drv_rtc_enable();
}

void hal_intf_rtc_stop(void)
{
    hal_drv_rtc_disable();
}

HAL_STATUS hal_intf_rtc_get_time(HAL_RTC_CNT_T *hal_time)
{
    if (!hal_time) {
        return HAL_ERR;
    }

    uint8_t day, hour, min, sec;
    hal_drv_rtc_get_time(&day, &hour, &min, &sec);

    hal_time->day       = day;
    hal_time->time.hour = hour;
    hal_time->time.min  = min;
    hal_time->time.sec  = sec;

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_rtc_set_time(HAL_RTC_CNT_T *hal_time)
{
    if (!hal_time) {
        return HAL_ERR;
    }

    hal_drv_rtc_set_time(hal_time->day, hal_time->time.hour, hal_time->time.min,
                         hal_time->time.sec);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_rtc_set_alarm(HAL_RTC_ALM_CFG_T *hal_alarm_cfg)
{
    if (!hal_alarm_cfg) {
        return HAL_ERR;
    }

    if (hal_alarm_cfg->enable) {
        hal_drv_rtc_set_alarm(hal_alarm_cfg->channel, hal_alarm_cfg->time.hour,
                              hal_alarm_cfg->time.min, hal_alarm_cfg->time.sec);
        if (hal_alarm_cfg->int_en) {
            hal_drv_rtc_set_int_source(
                RTC_GET_ALM_INT_IDX(hal_alarm_cfg->channel));
        }
    } else {
        hal_drv_rtc_disable();
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_rtc_register_callback(RTC_CB_EVENT_T event, CALLBACK_T cb,
                                           void *context)
{
    return hal_drv_rtc_register_callback(event, cb, context);
}

HAL_STATUS hal_intf_rtc_unregister_callback(RTC_CB_EVENT_T event)
{
    return hal_drv_rtc_unregister_callback(event);
}

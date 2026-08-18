/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_rtc_api.h"
#include "hal_intf_sys.h"
#include "util_debug_log.h"

WISE_STATUS wise_rtc_init(void)
{
    /* Enable the RTC module clock only. The LFOSC source is a board-level
     * choice already applied at boot by _platform_init(); this function used to
     * override it with a fixed internal-16K config, which also powered down the
     * external 32K XO. Use wise_sys_lfosc_clk_src_config() to change it. */
    return hal_intf_module_clk_enable(RTC_MODULE);
}

WISE_STATUS wise_rtc_deinit(void)
{
    return hal_intf_module_clk_disable(RTC_MODULE);
}

WISE_STATUS wise_rtc_get_time(WISE_RTC_CNT_T *cur_time)
{
    if (!cur_time) {
        return WISE_FAIL;
    }

    HAL_RTC_CNT_T hal_time;
    if (hal_intf_rtc_get_time(&hal_time) == WISE_FAIL) {
        return WISE_FAIL;
    }

    cur_time->day       = hal_time.day;
    cur_time->time.hour = hal_time.time.hour;
    cur_time->time.min  = hal_time.time.min;
    cur_time->time.sec  = hal_time.time.sec;

    return WISE_SUCCESS;
}

WISE_STATUS wise_rtc_set_time(WISE_RTC_CNT_T *cur_time)
{
    if (!cur_time) {
        return WISE_FAIL;
    }

    /* Out-of-range values cause unexpected behavior (ATCRTC100 DS085 V1.2), so
     * reject them instead of letting the register mask truncate silently. */
    if ((cur_time->day > WISE_RTC_DAY_MAX) ||
        (cur_time->time.hour > WISE_RTC_HOUR_MAX) ||
        (cur_time->time.min > WISE_RTC_MIN_MAX) ||
        (cur_time->time.sec > WISE_RTC_SEC_MAX)) {
        WISE_LOG_ERR("rtc set_time out of range (day %u/%u, %u:%u:%u)\n",
                     cur_time->day, WISE_RTC_DAY_MAX, cur_time->time.hour,
                     cur_time->time.min, cur_time->time.sec);
        return WISE_FAIL;
    }

    HAL_RTC_CNT_T hal_time = {.day  = cur_time->day,
                              .time = {
                                  .hour = cur_time->time.hour,
                                  .min  = cur_time->time.min,
                                  .sec  = cur_time->time.sec,
                              }};

    return hal_intf_rtc_set_time(&hal_time);
}

WISE_STATUS wise_rtc_set_alarm(WISE_RTC_ALM_CFG_T *alarm_cfg)
{
    if (!alarm_cfg) {
        return WISE_FAIL;
    }

    /* Same range rule as wise_rtc_set_time(); the alarm has no Day field. */
    if ((alarm_cfg->time.hour > WISE_RTC_HOUR_MAX) ||
        (alarm_cfg->time.min > WISE_RTC_MIN_MAX) ||
        (alarm_cfg->time.sec > WISE_RTC_SEC_MAX)) {
        WISE_LOG_ERR("rtc set_alarm out of range (%u:%u:%u)\n",
                     alarm_cfg->time.hour, alarm_cfg->time.min, alarm_cfg->time.sec);
        return WISE_FAIL;
    }

    HAL_RTC_ALM_CFG_T hal_alarm_cfg = {.channel = alarm_cfg->channel,
                                       .enable  = alarm_cfg->enable,
                                       .int_en  = alarm_cfg->int_en,
                                       .time    = {
                                              .hour = alarm_cfg->time.hour,
                                              .min  = alarm_cfg->time.min,
                                              .sec  = alarm_cfg->time.sec,
                                       }};

    return hal_intf_rtc_set_alarm(&hal_alarm_cfg);
}

WISE_STATUS wise_rtc_start(void)
{
    hal_intf_rtc_start();
    return WISE_SUCCESS;
}

WISE_STATUS wise_rtc_stop(void)
{
    hal_intf_rtc_stop();
    return WISE_SUCCESS;
}

WISE_STATUS wise_rtc_register_callback(WISE_RTC_CB_EVENT_T event, CALLBACK_T cb, void *context)
{
    return hal_intf_rtc_register_callback(event, cb, context);
}

WISE_STATUS wise_rtc_unregister_callback(WISE_RTC_CB_EVENT_T event)
{
    return hal_intf_rtc_unregister_callback(event);
}

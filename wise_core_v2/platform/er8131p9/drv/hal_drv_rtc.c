/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_rtc.h"
#include "hdl/rtc_er8130.h"

static CALLBACK_ENTRY_T rtc_callbacks[RTC_MAX_EVENTS];

void hal_drv_rtc_set_time(uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    while (!hal_drv_rtc_is_write_done())
        ;
    rtc_set_time_er8130(day, hour, min, sec);
}

void hal_drv_rtc_get_time(uint8_t *day, uint8_t *hour, uint8_t *min,
                          uint8_t *sec)
{
    rtc_get_time_er8130(day, hour, min, sec);
}

void hal_drv_rtc_enable(void)
{
    rtc_enable_er8130();
}

void hal_drv_rtc_disable(void)
{
    rtc_disable_er8130();
}

uint8_t hal_drv_rtc_is_write_done(void)
{
    return rtc_is_write_done_er8130();
}

void hal_drv_rtc_set_int_source(uint32_t int_source)
{
    rtc_set_int_source_er8130(int_source);
}

uint32_t hal_drv_rtc_get_int_status(void)
{
    return rtc_get_int_status_er8130();
}

void hal_drv_rtc_clear_int_flag(uint8_t int_source)
{
    rtc_clear_int_status_er8130(int_source);
}

void hal_drv_rtc_set_alarm(uint8_t channel, uint8_t hour, uint8_t min,
                           uint8_t sec)
{
    while (!hal_drv_rtc_is_write_done())
        ;
    rtc_set_alarm_er8130(channel, hour, min, sec);
}

HAL_STATUS hal_drv_rtc_register_callback(RTC_CB_EVENT_T event, CALLBACK_T cb,
                                          void *context)
{
    if (event < RTC_MAX_EVENTS) {
        rtc_callbacks[event].callback = cb;
        rtc_callbacks[event].context  = context;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_rtc_unregister_callback(RTC_CB_EVENT_T event)
{
    if (event < RTC_MAX_EVENTS) {
        rtc_callbacks[event].callback = NULL;
        rtc_callbacks[event].context  = NULL;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

static void hal_drv_rtc_trigger_callback(RTC_CB_EVENT_T event, uint8_t idx)
{
    if (rtc_callbacks[event].callback) {
        rtc_callbacks[event].callback(rtc_callbacks[event].context, event);
    }
}

WEAK_ISR void RTC_IRQHandler(void)
{
    uint32_t status = hal_drv_rtc_get_int_status();
    hal_drv_rtc_clear_int_flag(status);

    if (status & RTC_STS_ALARM_INT_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_ALARM, 0);
    }

    if (status & RTC_STS_ALARM1_INT_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_ALARM, 1);
    }

    if (status & RTC_STS_ALARM2_INT_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_ALARM, 2);
    }

    if (status & RTC_STS_ALARM3_INT_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_ALARM, 3);
    }

    if (status & RTC_STS_ALARM4_INT_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_ALARM, 4);
    }

    if (status & RTC_STS_ALARM5_INT_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_ALARM, 5);
    }

    if (status & RTC_STS_DAY_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_DAY, 0);
    }

    if (status & RTC_STS_HOUR_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_HOUR, 0);
    }

    if (status & RTC_STS_MIN_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_MINUTE, 0);
    }

    if (status & RTC_STS_SEC_MASK) {
        hal_drv_rtc_trigger_callback(RTC_EVENT_SECOND, 0);
    }
}

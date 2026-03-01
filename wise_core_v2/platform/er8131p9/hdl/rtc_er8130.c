/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/rtc_er8130.h"
#include "util_debug_log.h"

void rtc_set_time_er8130(uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t reg_value = (((sec << RTC_CNTR_SEC_POS) & RTC_CNTR_SEC_MASK) |
                          ((min << RTC_CNTR_MIN_POS) & RTC_CNTR_MIN_MASK) |
                          ((hour << RTC_CNTR_HOUR_POS) & RTC_CNTR_HOUR_MASK) |
                          ((day << RTC_CNTR_DAY_POS) & RTC_CNTR_DAY_MASK));

    RTC->CNTR = reg_value;
}

void rtc_get_time_er8130(uint8_t *day, uint8_t *hour, uint8_t *min,
                         uint8_t *sec)
{
    *sec  = RTC->CNTR_b.SEC;
    *min  = RTC->CNTR_b.MIN;
    *hour = RTC->CNTR_b.HOUR;
    *day  = RTC->CNTR_b.DAY;
}

void rtc_enable_er8130(void)
{
    RTC->CTRL_b.RTC_EN = 1;
}

void rtc_disable_er8130(void)
{
    RTC->CTRL_b.RTC_EN = 0;
}

uint8_t rtc_is_write_done_er8130(void)
{
    return (uint8_t)(RTC->STATUS_b.WRITEDONE);
}

void rtc_set_int_source_er8130(uint32_t int_source)
{
    RTC->CTRL |= int_source;
    NVIC_EnableIRQ((IRQn_Type)RTC_IRQn);
}

uint32_t rtc_get_int_status_er8130(void)
{
    return (RTC->STATUS & ~RTC_WRITE_DONE_MASK);
}

void rtc_clear_int_status_er8130(uint8_t source)
{
    RTC->STATUS |= source;
}

void rtc_set_alarm_er8130(uint8_t channel, uint8_t hour, uint8_t min,
                          uint8_t sec)
{
    volatile uint32_t *alarm_reg = NULL;

    switch (channel) {
    case 0:
        alarm_reg = &RTC->ALARM0;
        break;
    case 1:
        alarm_reg = &RTC->ALARM1;
        break;
    case 2:
        alarm_reg = &RTC->ALARM2;
        break;
    case 3:
        alarm_reg = &RTC->ALARM3;
        break;
    case 4:
        alarm_reg = &RTC->ALARM4;
        break;
    case 5:
        alarm_reg = &RTC->ALARM5;
        break;
    default:
        debug_printf("Invalid channel: %d\n", channel);
        return;
    }
    uint32_t reg_value = ((sec << RTC_ALARM_SEC_POS) & RTC_ALARM_SEC_MASK) |
                         ((min << RTC_ALARM_MIN_POS) & RTC_ALARM_MIN_MASK) |
                         ((hour << RTC_ALARM_HOUR_POS) & RTC_ALARM_HOUR_MASK);

    *alarm_reg = reg_value;
}

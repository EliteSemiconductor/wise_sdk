/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_RTC_H__
#define __HAL_DRV_RTC_H__

#include "hal_intf_rtc.h"
#include "types.h"

void hal_drv_rtc_set_time(uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
void hal_drv_rtc_get_time(uint8_t *day, uint8_t *hour, uint8_t *min,
                          uint8_t *sec);
void hal_drv_rtc_enable(void);
void hal_drv_rtc_disable(void);
uint8_t hal_drv_rtc_is_write_done(void);
void hal_drv_rtc_set_int_source(uint32_t int_source);
uint32_t hal_drv_rtc_get_int_status(void);
void hal_drv_rtc_clear_int_flag(uint8_t int_source);
void hal_drv_rtc_set_alarm(uint8_t channel, uint8_t hour, uint8_t min,
                           uint8_t sec);
HAL_STATUS hal_drv_rtc_register_callback(RTC_CB_EVENT_T event, CALLBACK_T cb,
                                          void *context);
HAL_STATUS hal_drv_rtc_unregister_callback(RTC_CB_EVENT_T event);
#endif /* __HAL_DRV_RTC_H__ */

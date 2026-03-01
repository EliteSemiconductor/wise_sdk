/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_rtc_api.h
 * @brief RTC (Real-Time Clock) driver APIs.
 *
 * @ingroup WISE_RTC
 *
 * This header provides the interface for configuring and using the
 * on-chip Real-Time Clock, including current time read/write, alarm
 * configuration, start/stop control, and event callback registration.
 */

#ifndef _WISE_RTC_API_H_
#define _WISE_RTC_API_H_

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_pmu.h"
#include "hal_intf_rtc.h"
#include "types.h"

/**
 * @defgroup WISE_RTC WISE RTC APIs
 * @ingroup WISE_CORE_API
 * @brief Real-Time Clock timekeeping, alarm and event callbacks.
 * @{
 */

/**
 * @enum WISE_RTC_CB_EVENT_T
 * @brief RTC callback events.
 *
 * Event types that can be reported to a registered callback.
 */
typedef enum {
    WISE_RTC_EVENT_ALARM,  /**< Alarm event. */
    WISE_RTC_EVENT_SECOND, /**< Per-second tick event. */
    WISE_RTC_EVENT_MINUTE, /**< Per-minute tick event. */
    WISE_RTC_EVENT_HOUR,   /**< Per-hour tick event. */
    WISE_RTC_EVENT_DAY,    /**< Per-day tick event. */
    WISE_RTC_MAX_EVENTS,   /**< Total number of RTC events (sentinel). */
} WISE_RTC_CB_EVENT_T;

/**
 * @struct RTC_TIME_T
 * @brief Time-of-day representation.
 *
 * Represents hours, minutes, and seconds in a day.
 */
typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} RTC_TIME_T;

/**
 * @struct WISE_RTC_CNT_T
 * @brief RTC day counter and time.
 *
 * Combines a day counter with time-of-day information.
 */
typedef struct {
    uint8_t day;
    RTC_TIME_T time;
} WISE_RTC_CNT_T;

/**
 * @struct WISE_RTC_ALM_CFG_T
 * @brief RTC alarm configuration.
 *
 * Defines which alarm channel to use, whether it is enabled, the time it
 * should trigger at, and whether an interrupt is generated.
 */
typedef struct {
    uint8_t    channel; /**< Alarm channel index. */
    uint8_t    enable;  /**< Non-zero to enable this alarm, zero to disable. */
    RTC_TIME_T time;    /**< Alarm trigger time-of-day. */
    uint8_t    int_en;  /**< Non-zero to enable interrupt, zero to disable. */
} WISE_RTC_ALM_CFG_T;

/* ==== API functions ====================================================== */


/**
 * @brief Initialize the RTC subsystem.
 *
 * Must be called before using any RTC APIs.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_rtc_init(void);

/**
 * @brief Deinitialize the RTC subsystem.
 *
 * Stops the RTC and releases internal resources.
 *
 * @retval WISE_SUCCESS Deinitialization successful.
 * @retval WISE_FAIL    Deinitialization failed.
 */
WISE_STATUS wise_rtc_deinit(void);

/**
 * @brief Get the current RTC time and day count.
 *
 * @param[out] cur_time Pointer to ::WISE_RTC_CNT_T that receives
 *                      current day and time.
 *
 * @retval WISE_SUCCESS Time read successfully.
 * @retval WISE_FAIL    Operation failed (for example invalid argument).
 */
WISE_STATUS wise_rtc_get_time(WISE_RTC_CNT_T *cur_time);

/**
 * @brief Set the current RTC time and day count.
 *
 * @param[in] cur_time Pointer to ::WISE_RTC_CNT_T that contains the
 *                     desired day and time.
 *
 * @retval WISE_SUCCESS Time set successfully.
 * @retval WISE_FAIL    Operation failed (invalid value or HW error).
 */
WISE_STATUS wise_rtc_set_time(WISE_RTC_CNT_T *cur_time);

/**
 * @brief Configure an RTC alarm.
 *
 * Applies the configuration in @p alarm_cfg to the specified alarm channel.
 *
 * @param[in] alarm_cfg Pointer to ::WISE_RTC_ALM_CFG_T containing
 *                      alarm channel, enable flag, time, and interrupt
 *                      enable configuration.
 *
 * @retval WISE_SUCCESS Alarm configured successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_rtc_set_alarm(WISE_RTC_ALM_CFG_T *alarm_cfg);

/**
 * @brief Start RTC time counting.
 *
 * @retval WISE_SUCCESS RTC started successfully.
 * @retval WISE_FAIL    Start failed.
 */
WISE_STATUS wise_rtc_start(void);

/**
 * @brief Stop RTC time counting.
 *
 * @retval WISE_SUCCESS RTC stopped successfully.
 * @retval WISE_FAIL    Stop failed.
 */
WISE_STATUS wise_rtc_stop(void);

/**
 * @brief Register a callback for RTC events.
 *
 * The callback type @c CALLBACK_T is defined in the underlying HAL.
 *
 * @param[in] event   RTC event type, see ::WISE_RTC_CB_EVENT_T.
 * @param[in] cb      Callback function pointer.
 * @param[in] context User-defined context pointer passed to @p cb.
 *
 * @retval WISE_SUCCESS Callback registered successfully.
 * @retval WISE_FAIL    Registration failed.
 */
WISE_STATUS wise_rtc_register_callback(WISE_RTC_CB_EVENT_T event, CALLBACK_T cb, void *context);

/**
 * @brief Unregister a previously registered RTC callback.
 *
 * @param[in] event RTC event type to unregister, see ::WISE_RTC_CB_EVENT_T.
 *
 * @retval WISE_SUCCESS Callback unregistered successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_rtc_unregister_callback(WISE_RTC_CB_EVENT_T event);


/** @} */ /* end of WISE_RTC group */

#endif /* _WISE_RTC_API_H_ */

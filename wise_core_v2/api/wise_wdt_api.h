/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_wdt_api.h
 * @brief Watchdog Timer (WDT) driver APIs.
 *
 * @ingroup WISE_WDT
 *
 * This header provides the interface for configuring, starting, stopping,
 * refreshing, and monitoring the watchdog timer, including timeout callback
 * registration.
 */

#ifndef _WISE_WDT_API_H_
#define _WISE_WDT_API_H_

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_pmu.h"
#include "hal_intf_wdt.h"
#include "types.h"

#define WDT_RESET_DISABLE               0
#define WDT_RESET_ENABLE                1

/**
 * @defgroup WISE_WDT WISE Watchdog Timer APIs
 * @ingroup WISE_CORE_API
 * @brief WDT configuration, control, timeout handling, and callbacks.
 * @{
 */

/**
 * @enum WISE_WDT_CB_EVENT_T
 * @brief Watchdog timer events.
 */
typedef enum {
    WISE_WDT_EVENT_TIMEOUT,
    WISE_WDT_MAX_EVENTS = WDT_MAX_EVENTS,
} WISE_WDT_CB_EVENT_T;

/**
 * @brief Initialize the watchdog timer subsystem.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_wdt_init(void);

/**
 * @brief Deinitialize the watchdog timer subsystem.
 *
 * @retval WISE_SUCCESS Deinitialization successful.
 * @retval WISE_FAIL    Deinitialization failed.
 */
WISE_STATUS wise_wdt_deinit(void);

/**
 * @brief Configure watchdog reset behavior and timeout.
 *
 * @param[in] reset_en   Non-zero to enable system reset on timeout.
 * @param[in] timeout_ms Timeout period in milliseconds.
 */
void wise_wdt_config(uint8_t reset_en, uint32_t timeout_ms);

/**
 * @brief Start the watchdog timer.
 */
void wise_wdt_start(void);

/**
 * @brief Stop the watchdog timer.
 */
void wise_wdt_stop(void);

/**
 * @brief Update the watchdog timeout value.
 *
 * @param[in] timeout_ms Timeout period in milliseconds.
 */
void wise_wdt_set_timeout(uint32_t timeout_ms);

/**
 * @brief Refresh (kick) the watchdog timer.
 *
 * Must be called periodically before timeout expires to prevent a reset.
 */
void wise_wdt_set_refresh(void);

/**
 * @brief Get the configured watchdog timeout period.
 *
 * @return Timeout period in milliseconds.
 */
uint32_t wise_wdt_get_period(void);

/**
 * @brief Get watchdog timer status.
 *
 * @return Status flags (device-specific bitfield).
 */
uint32_t wise_wdt_get_status(void);

/**
 * @brief Register a WDT event callback.
 *
 * The callback is invoked when the corresponding WDT event occurs.
 *
 * @param[in] event   WDT event type, see ::WISE_WDT_CB_EVENT_T.
 * @param[in] cb      Callback function pointer.
 * @param[in] context User-defined context passed to the callback.
 *
 * @retval WISE_SUCCESS Callback registered.
 * @retval WISE_FAIL    Registration failed.
 */
WISE_STATUS wise_wdt_register_callback(WISE_WDT_CB_EVENT_T event, CALLBACK_T cb, void *context);

/**
 * @brief Unregister a previously registered WDT callback.
 *
 * @param[in] event WDT event type to unregister.
 *
 * @retval WISE_SUCCESS Callback unregistered.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_wdt_unregister_callback(WISE_WDT_CB_EVENT_T event);


/** @} */ /* end of WISE_WDT group */

#endif /* _WISE_WDT_API_H_ */

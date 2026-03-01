/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_gptmr_api.h
 * @brief General-Purpose Timer (GPTMR) driver APIs.
 *
 * @ingroup WISE_GPTMR
 *
 * This header provides interfaces for configuring and controlling general-purpose
 * timers, including periodic/one-shot modes, interval setup, callbacks, counter
 * queries, and interrupt control.
 */

#ifndef _WISE_GPTMR_API_H_
#define _WISE_GPTMR_API_H_

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_gptmr.h"
#include "hal_intf_pmu.h"
#include "types.h"

/**
 * @defgroup WISE_GPTMR WISE GPTMR APIs
 * @ingroup WISE_CORE_API
 * @brief Timer configuration, control, callbacks, and counter utilities.
 * @{
 */

/**
 * @brief Maximum supported timer interval (in microseconds).
 *
 * Derived from a 32-bit counter and system clock.
 */
#define WISE_TIMER_MAX_INTERVAL             ((uint32_t)(0xffffffff) / (__SYSTEM_CLOCK/1000000))

/**
 * @brief Special interval value indicating free-running mode.
 */
#define WISE_TIMER_COUNTER_MAX              (0xffffffff) //special interval value for free run timer

/**
 * @enum WISE_TIMER_TYPE_T
 * @brief Timer operation mode.
 */
typedef enum {
    WISE_TIMER_PERIODIC = 0, /**< Periodic timer (auto-reload). */
    WISE_TIMER_ONE_SHOT,     /**< One-shot timer (stops after first expiration). */
} WISE_TIMER_TYPE_T;

/**
 * @struct WISE_TIMER_CTRL_T
 * @brief Timer control configuration.
 *
 * Describes mode, timing interval, starting offset, and interrupt enable setting.
 */
typedef struct {
    WISE_TIMER_TYPE_T type; /**< Timer mode (periodic or one-shot). */
    uint32_t interval;      /**< Timer interval in microseconds. Max = ::WISE_TIMER_MAX_INTERVAL. */
    uint32_t start_offset;  /**< Initial offset in microseconds before counting begins. */
    bool interrupt_enable;  /**< Enable or disable timer interrupt. */
} WISE_TIMER_CTRL_T;

/**
 * @brief Initialize the GPTMR subsystem.
 *
 * Must be called before using any timer APIs.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_timer_init();

/**
 * @brief Deinitialize the GPTMR subsystem.
 *
 * Releases resources and returns the timer hardware to an uninitialized state.
 *
 * @retval WISE_SUCCESS Deinitialization successful.
 * @retval WISE_FAIL    Deinitialization failed.
 */
WISE_STATUS wise_timer_deinit();

/**
 * @brief Configure a timer channel.
 *
 * Applies the timer configuration including mode, interval, offset, and interrupt behavior.
 *
 * @param[in] timer_channel Timer channel index.
 * @param[in] config        Pointer to a ::WISE_TIMER_CTRL_T structure.
 *
 * @retval WISE_SUCCESS Configuration successful.
 * @retval WISE_FAIL    Configuration failed (invalid arguments or HW error).
 */
WISE_STATUS wise_timer_config(uint8_t timer_channel, WISE_TIMER_CTRL_T *config);

/**
 * @brief Start a timer channel.
 *
 * @param[in] timer_channel Timer channel index.
 *
 * @retval WISE_SUCCESS Timer started.
 * @retval WISE_FAIL    Start failed.
 */
WISE_STATUS wise_timer_start(uint8_t timer_channel);

/**
 * @brief Stop a timer channel.
 *
 * @param[in] timer_channel Timer channel index.
 *
 * @retval WISE_SUCCESS Timer stopped.
 * @retval WISE_FAIL    Stop failed.
 */
WISE_STATUS wise_timer_stop(uint8_t timer_channel);

/**
 * @brief Check if a timer channel is enabled.
 *
 * @param[in] timer_channel Timer channel index.
 *
 * @return true if the timer is running, false otherwise.
 */
bool wise_timer_is_enabled(uint8_t timer_channel);

/**
 * @brief Register a callback function for a timer channel.
 *
 * The callback is invoked on timer interrupt events.
 *
 * @param[in] timer_channel Timer channel index.
 * @param[in] cb            Callback function of type ::CALLBACK_T.
 * @param[in] context       User context pointer passed to callback.
 *
 * @retval WISE_SUCCESS Callback registered.
 * @retval WISE_FAIL    Registration failed.
 */
WISE_STATUS wise_timer_register_callback(uint8_t timer_channel, CALLBACK_T cb, void *context);

/**
 * @brief Close a timer channel and release associated resources.
 *
 * @param[in] timer_channel Timer channel index.
 *
 * @retval WISE_SUCCESS Close successful.
 * @retval WISE_FAIL    Close failed.
 */
WISE_STATUS wise_timer_close(uint8_t timer_channel);

/**
 * @brief Read the current counter value of a timer channel.
 *
 * @param[in] timer_channel Timer channel index.
 *
 * @return Current counter value (0 to 0xFFFFFFFF).
 */
uint32_t wise_timer_get_counter(uint32_t timer_channel);

/**
 * @brief Get the maximum counter value supported by a timer channel.
 *
 * @param[in] timer_channel Timer channel index.
 *
 * @return Upper bound of the counter (typically ::WISE_TIMER_COUNTER_MAX).
 */
uint32_t wise_timer_get_counter_max(uint32_t timer_channel);

/**
 * @brief Enable interrupt for a timer channel.
 *
 * @param[in] timer_channel Timer channel index.
 *
 * @retval WISE_SUCCESS Interrupt enabled.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_timer_enable_int(uint32_t timer_channel);

/**
 * @brief Disable interrupt for a timer channel.
 *
 * @param[in] timer_channel Timer channel index.
 *
 * @retval WISE_SUCCESS Interrupt disabled.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_timer_disable_int(uint32_t timer_channel);

/** @} */ /* end of WISE_GPTMR group */

#endif /* _WISE_GPTMR_API_H_ */

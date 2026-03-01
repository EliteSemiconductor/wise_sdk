/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_wutmr_api.h
 * @brief Wake-Up Timer (WUTMR) driver APIs.
 *
 * @ingroup WISE_WUTMR
 *
 * This header provides APIs for controlling the low-power wake-up timer,
 * including one-shot and periodic timers, calibration, interrupts,
 * counter queries, and callback registration.
 */

#ifndef _WISE_WUTMR_API_H_
#define _WISE_WUTMR_API_H_

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_pmu.h"
#include "hal_intf_wutmr.h"
#include "types.h"

/**
 * @defgroup WISE_WUTMR WISE Wake-Up Timer APIs
 * @ingroup WISE_CORE_API
 * @brief One-shot / periodic wake timer operations and callbacks.
 * @{
 */

/**
 * @brief Wake timer event bitmask.
 *
 * Only one event is currently defined:
 * - WISE_WUTMR_EVENT_EXPIRED — Timer reached expiry.
 */
#define WISE_WUTMR_EVENT_EXPIRED                (1 << 0)

/**
 * @brief Wake timer callback function type.
 *
 * @param[in] context User-supplied context pointer passed when the callback was registered.
 */
typedef void (*WISE_WUTMR_EVT_CALLBACK_T)(void*context);

/**
 * @enum WISE_WUTMR_TYPE_T
 * @brief Wake timer execution mode.
 */
typedef enum {
    WISE_WUTMR_ONE_SHOT = 0, /**< One-shot mode. */
    WISE_WUTMR_PERIODIC,     /**< Periodic mode. */
} WISE_WUTMR_TYPE_T;


/* ------------------------------------------------------------------------- */
/*                               API Functions                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Initialize the wake-up timer subsystem.
 */
void wise_wutmr_init(void);

/**
 * @brief Deinitialize the wake-up timer subsystem.
 */
void wise_wutmr_deinit(void);

/**
 * @brief Enable the wake-up timer hardware.
 */
void wise_wutmr_enable(void);

/**
 * @brief Disable the wake-up timer hardware.
 */
void wise_wutmr_disable(void);

/**
 * @brief Calibrate the wake timer clock against a known millisecond interval.
 *
 * @param[in] ms Calibration duration in milliseconds.
 */
void wise_wutmr_clk_calibrate(uint32_t ms);

/**
 * @brief Enable wake timer interrupt.
 */
void wise_wutmr_enable_int(void);

/**
 * @brief Disable wake timer interrupt.
 */
void wise_wutmr_disable_int(void);

/**
 * @brief Retrieve current timer counter value.
 *
 * @return Current WUTMR counter in ticks.
 */
uint32_t wise_wutmr_get_counter(void);

/**
 * @brief Start the wake timer with a timeout specified in milliseconds.
 *
 * @param[in] ms         Timeout duration in milliseconds.
 * @param[in] exec_type  Timer mode, see ::WISE_WUTMR_TYPE_T.
 *
 * @retval WISE_SUCCESS Start successful.
 * @retval WISE_FAIL    Invalid parameter or hardware failure.
 */
WISE_STATUS wise_wutmr_start(uint32_t ms, uint8_t exec_type);

/**
 * @brief Start the wake timer with a timeout specified in tick units.
 *
 * @param[in] tickCounter Timeout in raw tick count.
 * @param[in] exec_type   Timer mode, see ::WISE_WUTMR_TYPE_T.
 *
 * @retval WISE_SUCCESS Start successful.
 * @retval WISE_FAIL    Invalid parameter or hardware failure.
 */
WISE_STATUS wise_wutmr_start_tick(uint32_t tickCounter, uint8_t exec_type);

/**
 * @brief Stop the wake timer.
 */
void wise_wutmr_stop(void);

/**
 * @brief Register a callback for wake timer expiry events.
 *
 * @param[in] callback Function pointer to be invoked when timer expires.
 * @param[in] context  User-defined context pointer passed to the callback.
 *
 * @retval WISE_SUCCESS Callback registered.
 * @retval WISE_FAIL    Registration failed.
 */
WISE_STATUS wise_wutmr_register_callback(WISE_WUTMR_EVT_CALLBACK_T callback, void *context);

/* ------------------------------------------------------------------------- */
/*                          Clock Utility Functions                          */
/* ------------------------------------------------------------------------- */

/**
 * @brief Get the calibrated WUTMR clock rate in Hz.
 *
 * @return Clock frequency in Hertz.
 */
uint32_t wise_wutmr_get_clk_rate(void);

/**
 * @brief Get number of calibrated WUTMR clock ticks per millisecond.
 *
 * @return Ticks per millisecond.
 */
uint32_t wise_wutmr_clk_per_ms(void);

/**
 * @brief Convert milliseconds to WUTMR ticks.
 *
 * @param[in] ms Milliseconds.
 *
 * @return Equivalent tick count.
 */
uint32_t wise_wutmr_ms_to_clk(uint32_t ms);

/**
 * @brief Convert WUTMR ticks to milliseconds.
 *
 * @param[in] clk Tick count.
 *
 * @return Equivalent milliseconds.
 */
uint32_t wise_wutmr_clk_to_ms(uint32_t clk);


/** @} */ /* end of WISE_WUTMR group */

#endif /* _WISE_WUTMR_API_H_ */

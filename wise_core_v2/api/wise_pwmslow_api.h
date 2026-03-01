/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_pwmslow_api.h
 * @brief Slow PWM (low-frequency PWM) driver APIs.
 *
 * @ingroup WISE_PWMSLOW
 *
 * This header provides the interface for configuring and controlling
 * the slow PWM block, including duty cycle, period/frequency modes,
 * center alignment, one-shot mode, and trigger callback registration.
 */

#ifndef _WISE_PWMSLOW_API_H_
#define _WISE_PWMSLOW_API_H_

#include "wise_core.h"
#include "api/wise_gpio_api.h"
#include "api/wise_pmu_api.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_pmu.h"
#include "hal_intf_pwmslow.h"
#include "types.h"

/**
 * @defgroup WISE_PWMSLOW WISE Slow PWM APIs
 * @ingroup WISE_CORE_API
 * @brief Low-frequency PWM configuration, control and callbacks.
 *
 * These APIs are typically used to generate low-frequency PWM waveforms
 * on a single GPIO pin (for example LED dimming, low-speed blinking, etc.).
 * @{
 */

/**
 * @enum PWMSLOW_MODE_T
 * @brief Slow PWM operation mode.
 *
 * Period mode uses explicit @ref WISE_PWMSLOW_CONF_T::u::period.period and
 * @ref WISE_PWMSLOW_CONF_T::u::period.active_period.
 * Frequency mode uses @ref WISE_PWMSLOW_CONF_T::u::freq.frequency_Hz and
 * @ref WISE_PWMSLOW_CONF_T::u::freq.duty_percent.
 */
typedef enum {
    PWMSLOW_MODE_PERIOD,   /**< Define PWM using period and active-period (ticks). */
    PWMSLOW_MODE_FREQUENCY /**< Define PWM using frequency and duty cycle (%). */
} PWMSLOW_MODE_T;

/**
 * @enum WISE_PWMSLOW_CB_EVENT_T
 * @brief Slow PWM callback events.
 */
typedef enum {
    WISE_PWMSLOW_EVENT_TRIGGER,                     /**< Slow PWM trigger event. */
    WISE_PWMSLOW_MAX_EVENTS = PWMSLOW_MAX_EVENTS    /**< Total slow PWM event count (alias). */
} WISE_PWMSLOW_CB_EVENT_T;

/**
 * @struct WISE_PWMSLOW_COMMON_CONF_T
 * @brief Common slow PWM configuration fields.
 *
 * Contains parameters shared by both period mode and frequency mode.
 */
typedef struct {
    PWMSLOW_MODE_T mode;        /**< PWM mode selection, see ::PWMSLOW_MODE_T. */
    uint32_t idle_status;       /**< Output level when PWM is idle (0/1). */
    uint32_t low_active_en;     /**< Enable low-active output if non-zero. */
    uint32_t center_align_en;   /**< Enable center-aligned PWM if non-zero. */
    uint32_t oneshot_en;        /**< Enable one-shot mode if non-zero. */
    uint32_t oneshot_num;       /**< PWM pulses to output when one-shot mode is enabled. */
} WISE_PWMSLOW_COMMON_CONF_T;

/**
 * @struct WISE_PWMSLOW_PERIOD_CONF_T
 * @brief Slow PWM configuration for period mode.
 *
 * Defines period and active duration in timer ticks.
 */
typedef struct {
    uint32_t period;        /**< PWM period in ticks (for PERIOD mode). */
    uint32_t active_period; /**< Active-high duration in ticks (for PERIOD mode). */
} WISE_PWMSLOW_PERIOD_CONF_T;

/**
 * @struct WISE_PWMSLOW_FREQ_CONF_T
 * @brief Slow PWM configuration for frequency mode.
 *
 * Defines slow PWM output in terms of target frequency and duty cycle.
 */
typedef struct {
    uint32_t frequency_Hz;  /**< PWM frequency in Hz (for FREQUENCY mode). */
    uint32_t duty_percent;  /**< Duty cycle percentage (0–100) (for FREQUENCY mode). */
} WISE_PWMSLOW_FREQ_CONF_T;

/**
 * @struct WISE_PWMSLOW_CONF_T
 * @brief Slow PWM configuration structure.
 *
 * Defines all operational parameters for the slow PWM generator. Depending
 * on @ref WISE_PWMSLOW_COMMON_CONF_T::mode, either period/active_period or
 * frequency/duty_percent fields in the union are used.
 */
typedef struct {
    WISE_PWMSLOW_COMMON_CONF_T common;      /**< Common slow PWM configuration. */

    union {
        WISE_PWMSLOW_PERIOD_CONF_T period; /**< Period-mode configuration. */
        WISE_PWMSLOW_FREQ_CONF_T   freq;   /**< Frequency-mode configuration. */
    } u; /**< Mode-specific configuration. */
} WISE_PWMSLOW_CONF_T;

/* ==== API functions ====================================================== */

/**
 * @brief Initialize the slow PWM subsystem.
 *
 * Must be called before configuring or starting slow PWM output.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_pwmslow_init(void);

/**
 * @brief Deinitialize the slow PWM subsystem.
 *
 * Turns off slow PWM hardware resources and returns the subsystem to
 * an uninitialized state.
 *
 * @retval WISE_SUCCESS Deinitialization successful.
 * @retval WISE_FAIL    Deinitialization failed.
 */
WISE_STATUS wise_pwmslow_deinit(void);

/**
 * @brief Configure slow PWM output on a GPIO pin.
 *
 * Binds the slow PWM generator to a GPIO pin and applies configuration
 * parameters such as mode, period/frequency and duty cycle.
 *
 * @param[in] gpio_index    GPIO pin index used for slow PWM output.
 * @param[in] pwmslow_conf  Pointer to ::WISE_PWMSLOW_CONF_T configuration structure.
 *
 * @retval WISE_SUCCESS Configuration successful.
 * @retval WISE_FAIL    Configuration failed (invalid parameters or HW error).
 */
WISE_STATUS wise_pwmslow_configure(uint8_t gpio_index, WISE_PWMSLOW_CONF_T *pwmslow_conf);

/**
 * @brief Start slow PWM output.
 *
 * Starts the slow PWM waveform on the configured GPIO pin.
 */
void wise_pwmslow_start(void);

/**
 * @brief Stop slow PWM output.
 *
 * Stops the slow PWM waveform and leaves the output at @ref WISE_PWMSLOW_COMMON_CONF_T::idle_status.
 */
void wise_pwmslow_stop(void);

/**
 * @brief Register a callback for slow PWM events.
 *
 * The callback type @c CALLBACK_T is defined in the underlying HAL.
 *
 * @param[in] event   Slow PWM event, see ::WISE_PWMSLOW_CB_EVENT_T.
 * @param[in] cb      Callback function.
 * @param[in] context User context pointer passed to the callback.
 *
 * @retval WISE_SUCCESS Callback registered successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_pwmslow_register_callback(WISE_PWMSLOW_CB_EVENT_T event, CALLBACK_T cb, void *context);

/**
 * @brief Unregister a previously registered slow PWM callback.
 *
 * @param[in] event Slow PWM event, see ::WISE_PWMSLOW_CB_EVENT_T.
 *
 * @retval WISE_SUCCESS Callback unregistered successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_pwmslow_unregister_callback(WISE_PWMSLOW_CB_EVENT_T event);


/** @} */ /* end of WISE_PWMSLOW group */

#endif /* _WISE_PWMSLOW_API_H_ */

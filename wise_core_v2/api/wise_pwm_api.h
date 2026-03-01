/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_pwm_api.h
 * @brief PWM (Pulse-Width Modulation) driver APIs.
 *
 * @ingroup WISE_PWM
 *
 * This header provides the interface for configuring and controlling PWM
 * channels, including duty cycle, period/frequency modes, center alignment,
 * oneshot mode, and channel-level callbacks.
 */

#ifndef _WISE_PWM_API_H_
#define _WISE_PWM_API_H_

#include "wise_core.h"
#include "api/wise_gpio_api.h"
#include "api/wise_pmu_api.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_pmu.h"
#include "hal_intf_pwm.h"
#include "types.h"

/**
 * @defgroup WISE_PWM WISE PWM APIs
 * @ingroup WISE_CORE_API
 * @brief PWM configuration, start/stop control, and event callback registration.
 * @{
 */

#define PWM_CH_BIT(ch)              (1UL << ch)

/**
 * @enum PWM_MODE_T
 * @brief PWM operation mode.
 *
 * Period mode uses explicit @ref WISE_PWM_CONF_T::u::period.period and
 * @ref WISE_PWM_CONF_T::u::period.active_period.
 * Frequency mode instead uses @ref WISE_PWM_CONF_T::u::freq.frequency_Hz and
 * @ref WISE_PWM_CONF_T::u::freq.duty_percent.
 */
typedef enum { 
    PWM_MODE_PERIOD,          /**< Define PWM using period and active-period (ticks). */
    PWM_MODE_FREQUENCY        /**< Define PWM using frequency and duty cycle (%). */
} PWM_MODE_T;

/**
 * @enum WISE_PWM_CB_EVENT_T
 * @brief PWM callback events.
 *
 * Each PWM channel may generate a trigger event that can be reported
 * via a registered callback.
 */
typedef enum {
    WISE_PWM_EVENT_CH0_TRIGGER, /**< Channel 0 trigger event. */
    WISE_PWM_EVENT_CH1_TRIGGER, /**< Channel 1 trigger event. */
    WISE_PWM_EVENT_CH2_TRIGGER, /**< Channel 2 trigger event. */
    WISE_PWM_EVENT_CH3_TRIGGER, /**< Channel 3 trigger event. */
    WISE_PWM_EVENT_CH4_TRIGGER, /**< Channel 4 trigger event. */
    WISE_PWM_EVENT_CH5_TRIGGER, /**< Channel 5 trigger event. */
    WISE_PWM_MAX_EVENTS = PWM_MAX_EVENTS /**< Total PWM event count (alias). */
} WISE_PWM_CB_EVENT_T;

/**
 * @struct WISE_PWM_COMMON_CONF_T
 * @brief Common PWM configuration fields.
 *
 * Contains parameters shared by both period and frequency modes.
 */
typedef struct {
    PWM_MODE_T mode;          /**< PWM mode selection. */
    uint32_t idle_status;     /**< Output level when PWM is idle (0/1). */
    uint32_t low_active_en;   /**< Enable low-active output if non-zero. */
    uint32_t center_align_en; /**< Enable center-aligned PWM if non-zero. */
    uint32_t oneshot_en;      /**< Enable oneshot mode if non-zero. */
    uint32_t oneshot_num;     /**< PWM pulses to output when oneshot mode enabled. */
} WISE_PWM_COMMON_CONF_T;

/**
 * @struct WISE_PWM_PERIOD_CONF_T
 * @brief PWM configuration for period mode.
 *
 * Defines period and active duration in timer ticks.
 */
typedef struct {
    uint32_t period;          /**< PWM period in ticks (for PERIOD mode). */
    uint32_t active_period;   /**< Active-high duration in ticks (for PERIOD mode). */
} WISE_PWM_PERIOD_CONF_T;

/**
 * @struct WISE_PWM_FREQ_CONF_T
 * @brief PWM configuration for frequency mode.
 *
 * Defines PWM output in terms of target frequency and duty cycle.
 */
typedef struct {
    uint32_t frequency_Hz;    /**< PWM frequency in Hz (for FREQUENCY mode). */
    uint32_t duty_percent;    /**< Duty cycle percentage (0–100) (FREQUENCY mode). */
} WISE_PWM_FREQ_CONF_T;

/**
 * @struct WISE_PWM_CONF_T
 * @brief PWM configuration structure.
 *
 * Defines all operational parameters for a PWM channel. Depending on
 * @ref WISE_PWM_COMMON_CONF_T::mode, either period/active_period or
 * frequency/duty_percent is used.
 */
typedef struct {
    WISE_PWM_COMMON_CONF_T common;      /**< Common PWM configuration. */

    union {
        WISE_PWM_PERIOD_CONF_T period;  /**< Period-mode configuration. */
        WISE_PWM_FREQ_CONF_T freq;      /**< Frequency-mode configuration. */
    } u;                                /**< Mode-specific configuration. */
} WISE_PWM_CONF_T;


/* ==== API functions ====================================================== */

/**
 * @brief Initialize the PWM subsystem.
 *
 * Must be called before configuring or starting PWM channels.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_pwm_init(void);

/**
 * @brief Deinitialize the PWM subsystem.
 *
 * Turns off PWM hardware resources and returns subsystem to an
 * uninitialized state.
 *
 * @retval WISE_SUCCESS Deinitialization successful.
 * @retval WISE_FAIL    Deinitialization failed.
 */
WISE_STATUS wise_pwm_deinit(void);

/**
 * @brief Configure a PWM channel.
 *
 * Binds the PWM channel to a GPIO pin and applies PWM configuration parameters.
 *
 * @param[in] channel    PWM channel index.
 * @param[in] gpio_index GPIO pin index to output the PWM waveform.
 * @param[in] pwm_conf   Pointer to ::WISE_PWM_CONF_T configuration structure.
 *
 * @retval WISE_SUCCESS Configuration successful.
 * @retval WISE_FAIL    Configuration failed (invalid parameters or HW error).
 */
WISE_STATUS wise_pwm_configure(uint8_t channel, uint8_t gpio_index, WISE_PWM_CONF_T *pwm_conf);

/**
 * @brief Start one or multiple PWM channels.
 *
 * @param[in] channel_mask Bitmask of channels to start (bit0 = ch0, bit1 = ch1, …).
 */
void wise_pwm_start(uint32_t channel_mask);

/**
 * @brief Reconfigure a PWM channel.
 *
 * Modifies frequency configuration of a PWM channel after started. On an unconfigured PWM channel
 * an error code WISE_FAIL will be returned.
 *
 * @param[in] channel    PWM channel index.
 * @param[in] pwm_conf   Pointer to ::WISE_PWM_CONF_T configuration structure.
 *
 * @retval WISE_SUCCESS Configuration successful.
 * @retval WISE_FAIL    Configuration failed (invalid parameters or HW error).
 */
WISE_STATUS wise_pwm_reconf_channel(uint8_t channel, WISE_PWM_CONF_T* pwm_conf);

/**
 * @brief Stop one or multiple PWM channels.
 *
 * @param[in] channel_mask Bitmask of channels to stop.
 */
void wise_pwm_stop(uint32_t channel_mask);

/**
 * @brief Register a callback for PWM events.
 *
 * @param[in] event   PWM event, see ::WISE_PWM_CB_EVENT_T.
 * @param[in] cb      Callback function of type ::CALLBACK_T.
 * @param[in] context User context pointer passed to the callback.
 *
 * @retval WISE_SUCCESS Callback registered.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_pwm_register_callback(WISE_PWM_CB_EVENT_T event, CALLBACK_T cb, void *context);

/**
 * @brief Unregister a previously registered PWM callback.
 *
 * @param[in] event PWM event, see ::WISE_PWM_CB_EVENT_T.
 *
 * @retval WISE_SUCCESS Callback removed.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_pwm_unregister_callback(WISE_PWM_CB_EVENT_T event);

/** @} */ /* end of WISE_PWM group */

#endif /* _WISE_PWM_API_H_ */

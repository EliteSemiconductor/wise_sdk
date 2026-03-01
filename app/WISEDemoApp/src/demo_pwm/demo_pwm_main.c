/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_pwm_main.c
 * @brief Example application demonstrating WISE_PWM continuous and one-shot PWM output.
 *
 * @ingroup WISE_EXAMPLE_APP_PWM
 *
 * This demo demonstrates basic PWM output using the WISE PWM driver (Core API
 * @ref WISE_PWM) with two build-time modes:
 * - Continuous mode: generate PWM waveform continuously.
 * - One-shot mode  : generate N pulses then stop and report completion via callback.
 *
 * Build-time mode selection:
 * - Define @ref DEMO_PWM_MODE to one of:
 *   - @ref DEMO_PWM_MODE_CONTINUOUS
 *   - @ref DEMO_PWM_MODE_ONESHOT
 *
 * Hardware notes:
 * - Connect an oscilloscope or LED to the PWM output pin (@ref PWM_GPIO_PIN).
 * - Update pin mapping in the configuration section if needed.
 *
 * Functional notes:
 * - This demo configures PWM in @ref PWM_MODE_FREQUENCY mode.
 * - Default frequency: 1kHz (@ref PWM_FREQUENCY_HZ)
 * - Default duty cycle: 50% (@ref PWM_DUTY_PERCENT)
 * - One-shot mode generates @ref PWM_ONESHOT_COUNT pulses then stops.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "demo_app_common.h"
#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_pwm_api.h"
#include "wise_sys_api.h"

/**
 * @defgroup WISE_EXAMPLE_APP_PWM PWM Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: PWM continuous and one-shot output demo.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_PWM  for PWM configuration/start and completion callback
 * - @ref WISE_GPIO for PWM output pin function mapping (platform dependent)
 * - @ref WISE_SYS  for system support (via demo init)
 *
 * The demo supports two build modes selected by @ref DEMO_PWM_MODE:
 * - @ref DEMO_PWM_MODE_CONTINUOUS
 * - @ref DEMO_PWM_MODE_ONESHOT
 * @{
 */

/* ============================================================
 * Build-time mode select
 * ============================================================ */
#define DEMO_PWM_MODE_CONTINUOUS 1 /**< Build as continuous PWM mode. */
#define DEMO_PWM_MODE_ONESHOT    2 /**< Build as one-shot PWM mode. */

#ifndef DEMO_PWM_MODE
/** @brief Default PWM build mode when DEMO_PWM_MODE is not defined. */
#define DEMO_PWM_MODE DEMO_PWM_MODE_ONESHOT
#endif

/* ============================================================
 * Config
 * ============================================================ */
#define PWM_CHANNEL       0     /**< PWM channel index used by this demo. */
#define PWM_GPIO_PIN      2     /**< GPIO pin index used as PWM output. */
#define PWM_FREQUENCY_HZ  1000  /**< PWM frequency in Hz (default: 1kHz). */
#define PWM_DUTY_PERCENT  50    /**< PWM duty cycle in percent (0-100). */
#define PWM_ONESHOT_COUNT 10    /**< Number of pulses generated in one-shot mode. */

/* ============================================================
 * Globals
 * ============================================================ */

/** @brief Set to 1 when one-shot sequence is completed (set in callback). */
static volatile uint8_t g_pwm_complete = 0;

/* ============================================================
 * PWM callback
 * ============================================================ */

/**
 * @brief PWM completion callback for one-shot mode.
 *
 * This callback is registered via ::wise_pwm_register_callback() and is used
 * to notify the main loop that one-shot pulse generation has finished.
 *
 * @param[in] context User context pointer passed during registration.
 * @param[in] pwm_idx PWM channel index that triggered the callback.
 */
static void demo_pwm_callback(void *context, uint8_t pwm_idx)
{
    (void)context;
    (void)pwm_idx;

    g_pwm_complete = 1;
}

/* ============================================================
 * Main
 * ============================================================ */

/**
 * @brief Main entry of the PWM demo application.
 *
 * Common flow:
 * 1. Initialize demo environment
 * 2. Initialize PWM module
 * 3. Configure PWM channel/pin and parameters
 * 4. Start PWM
 * 5. In one-shot mode, wait for completion callback
 *
 * @return This demo does not return under normal operation.
 */
int main(void)
{
    WISE_STATUS st;

    printf("\n=== PWM Demo (%s) ===\n",
#if (DEMO_PWM_MODE == DEMO_PWM_MODE_CONTINUOUS)
           "CONTINUOUS"
#else
           "ONESHOT"
#endif
    );

    demo_app_common_init();

    /* Initialize PWM module */
    st = wise_pwm_init();
    if (st != WISE_SUCCESS) {
        printf("[ERR] wise_pwm_init failed\n");
        return 0;
    }
    printf("PWM initialized\n");

    /* Configure PWM */
    WISE_PWM_CONF_T pwm_conf;
    memset(&pwm_conf, 0, sizeof(pwm_conf));

#if (DEMO_PWM_MODE == DEMO_PWM_MODE_CONTINUOUS)

    /* ========== Continuous Mode ========== */
    pwm_conf.common.mode            = PWM_MODE_FREQUENCY;
    pwm_conf.u.freq.frequency_Hz    = PWM_FREQUENCY_HZ;
    pwm_conf.u.freq.duty_percent    = PWM_DUTY_PERCENT;
    pwm_conf.common.idle_status     = 0;
    pwm_conf.common.low_active_en   = DISABLE;
    pwm_conf.common.center_align_en = DISABLE;
    pwm_conf.common.oneshot_en      = DISABLE;
    pwm_conf.common.oneshot_num     = 0;

    st = wise_pwm_configure(PWM_CHANNEL, PWM_GPIO_PIN, &pwm_conf);
    if (st != WISE_SUCCESS) {
        printf("[ERR] PWM configure failed\n");
        return 0;
    }

    printf("PWM configured:\n");
    printf("  Channel: %u\n", PWM_CHANNEL);
    printf("  GPIO Pin: %u\n", PWM_GPIO_PIN);
    printf("  Frequency: %lu Hz\n", pwm_conf.u.freq.frequency_Hz);
    printf("  Duty Cycle: %lu%%\n", pwm_conf.u.freq.duty_percent);
    printf("  Mode: Continuous\n");

    /* Start PWM */
    wise_pwm_start(1 << PWM_CHANNEL);
    printf("\nPWM started - running continuously\n");
    printf("Connect oscilloscope to GPIO%u to observe waveform\n", PWM_GPIO_PIN);
    printf("Expected: %luHz square wave, %lu%% duty cycle\n\n",
           pwm_conf.u.freq.frequency_Hz,
           pwm_conf.u.freq.duty_percent);

#else /* ONESHOT */

    /* ========== One-shot Mode ========== */
    pwm_conf.common.mode            = PWM_MODE_FREQUENCY;
    pwm_conf.u.freq.frequency_Hz    = PWM_FREQUENCY_HZ;
    pwm_conf.u.freq.duty_percent    = PWM_DUTY_PERCENT;
    pwm_conf.common.idle_status     = 0;
    pwm_conf.common.low_active_en   = DISABLE;
    pwm_conf.common.center_align_en = DISABLE;
    pwm_conf.common.oneshot_en      = ENABLE;
    pwm_conf.common.oneshot_num     = PWM_ONESHOT_COUNT;

    st = wise_pwm_configure(PWM_CHANNEL, PWM_GPIO_PIN, &pwm_conf);
    if (st != WISE_SUCCESS) {
        printf("[ERR] PWM configure failed\n");
        return 0;
    }

    printf("PWM configured:\n");
    printf("  Channel: %u\n", PWM_CHANNEL);
    printf("  GPIO Pin: %u\n", PWM_GPIO_PIN);
    printf("  Frequency: %lu Hz\n", pwm_conf.u.freq.frequency_Hz);
    printf("  Duty Cycle: %lu%%\n", pwm_conf.u.freq.duty_percent);
    printf("  Mode: One-shot\n");
    printf("  Pulse Count: %u\n", pwm_conf.common.oneshot_num);

    /* Register callback for oneshot completion */
    st = wise_pwm_register_callback(PWM_CHANNEL, demo_pwm_callback, NULL);
    if (st != WISE_SUCCESS) {
        printf("[ERR] PWM callback registration failed\n");
        return 0;
    }
    printf("PWM interrupt callback registered\n");

    /* Start PWM */
    g_pwm_complete = 0;
    wise_pwm_start(1 << PWM_CHANNEL);
    printf("\nPWM started - generating %u pulses\n", PWM_ONESHOT_COUNT);
    printf("Connect oscilloscope to GPIO%u to observe waveform\n\n", PWM_GPIO_PIN);

    /* Wait for completion */
    printf("Waiting for PWM completion...\n");
    while (!g_pwm_complete) {
        __NOP();
    }

    printf("[PWM%u] ONESHOT_COMPLETE\n", PWM_CHANNEL);
    printf("\nPWM one-shot sequence complete!\n");
    printf("Generated %u pulses at %luHz\n", PWM_ONESHOT_COUNT, pwm_conf.u.freq.frequency_Hz);

#endif

    while (1) {
        __WFI();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_PWM */

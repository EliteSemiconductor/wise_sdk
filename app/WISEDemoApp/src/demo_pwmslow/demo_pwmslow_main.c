/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_pwmslow_main.c
 * @brief Example application demonstrating WISE_PWMSLOW low-frequency PWM output.
 *
 * @ingroup WISE_EXAMPLE_APP_PWMSLOW
 *
 * This demo demonstrates basic PWMSLOW output using the WISE PWMSLOW driver
 * (Core API @ref WISE_PWMSLOW) with two build-time modes:
 * - Continuous mode: generate a low-frequency PWM signal continuously.
 * - One-shot mode  : generate N pulses then stop and report completion via callback.
 *
 * PWMSLOW is designed for low-frequency waveforms and uses LFOSC as its clock source,
 * making it suitable for signals from sub-Hz up to a few Hz (platform dependent).
 *
 * Build-time mode selection:
 * - Define @ref DEMO_PWMSLOW_MODE to one of:
 *   - @ref DEMO_PWMSLOW_MODE_CONTINUOUS
 *   - @ref DEMO_PWMSLOW_MODE_ONESHOT
 *
 * Hardware notes:
 * - Connect an oscilloscope (or LED) to the PWMSLOW output pin (@ref PWMSLOW_GPIO_PIN).
 * - Update pin mapping in the configuration section if needed.
 *
 * Functional notes:
 * - This demo configures PWMSLOW in @ref PWMSLOW_MODE_FREQUENCY mode.
 * - Default frequency: 10Hz (@ref PWMSLOW_FREQUENCY_HZ)
 * - Default duty cycle: 50% (@ref PWMSLOW_DUTY_PERCENT)
 * - One-shot mode generates @ref PWMSLOW_ONESHOT_COUNT pulses then stops.
 *
 * @note LFOSC clock configuration and calibration are required before using PWMSLOW.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "demo_app_common.h"
#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_pwmslow_api.h"
#include "wise_sys_api.h"

/**
 * @defgroup WISE_EXAMPLE_APP_PWMSLOW PWMSLOW Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: PWMSLOW continuous and one-shot output demo.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_PWMSLOW for PWMSLOW configuration/start and completion callback
 * - @ref WISE_SYS    for LFOSC clock source configuration and calibration
 * - @ref WISE_GPIO   for PWMSLOW output pin function mapping (platform dependent)
 *
 * The demo supports two build modes selected by @ref DEMO_PWMSLOW_MODE:
 * - @ref DEMO_PWMSLOW_MODE_CONTINUOUS
 * - @ref DEMO_PWMSLOW_MODE_ONESHOT
 * @{
 */

/* ============================================================
 * Build-time mode select
 * ============================================================ */
#define DEMO_PWMSLOW_MODE_CONTINUOUS 1 /**< Build as continuous PWMSLOW mode. */
#define DEMO_PWMSLOW_MODE_ONESHOT    2 /**< Build as one-shot PWMSLOW mode. */

#ifndef DEMO_PWMSLOW_MODE
/** @brief Default PWMSLOW build mode when DEMO_PWMSLOW_MODE is not defined. */
#define DEMO_PWMSLOW_MODE DEMO_PWMSLOW_MODE_ONESHOT
#endif

/* ============================================================
 * Config
 * ============================================================ */
#define PWMSLOW_GPIO_PIN      2   /**< GPIO pin index used as PWMSLOW output. */
#define PWMSLOW_FREQUENCY_HZ  10  /**< PWMSLOW frequency in Hz (default: 10Hz). */
#define PWMSLOW_DUTY_PERCENT  50  /**< PWMSLOW duty cycle in percent (0-100). */
#define PWMSLOW_ONESHOT_COUNT 10  /**< Number of pulses generated in one-shot mode. */

/* ============================================================
 * Globals
 * ============================================================ */

/** @brief Set to 1 when one-shot sequence is completed (set in callback). */
static volatile uint8_t g_pwmslow_complete = 0;

/* ============================================================
 * PWMSLOW callback
 * ============================================================ */

/**
 * @brief PWMSLOW completion callback for one-shot mode.
 *
 * This callback is registered via ::wise_pwmslow_register_callback() and is used
 * to notify the main loop that one-shot pulse generation has finished.
 *
 * @param[in] context    User context pointer passed during registration.
 * @param[in] event_type Event type reported by the driver (driver-defined).
 */
static void demo_pwmslow_callback(void *context, uint8_t event_type)
{
    (void)context;
    (void)event_type;

    g_pwmslow_complete = 1;
}

/* ============================================================
 * LFOSC configuration
 * ============================================================ */

/**
 * @brief Configure and calibrate LFOSC clock source for PWMSLOW.
 *
 * PWMSLOW uses LFOSC as its clock source. This helper configures LFOSC clock
 * source and triggers calibration before PWMSLOW is initialized.
 *
 * Steps:
 * 1. Configure LFOSC clock source using ::wise_sys_lfosc_clk_src_config()
 * 2. Calibrate LFOSC using ::wise_sys_lfosc_clk_calibration()
 */
static void demo_lfosc_config(void)
{
    printf("Configuring LFOSC clock source...\n");

    WISE_LFOSC_SRC_T oscCfg = {
        .clk_src          = SYS_LFOSC_CLK_SRC_INTERNAL_16K,
        .mode.mode_select = LFOSC_16K_MODE_32K,
        .calFinish        = 0,
    };

    wise_sys_lfosc_clk_src_config(oscCfg);
    printf("LFOSC clock source configured\n");

    printf("Calibrating LFOSC...\n");
    wise_sys_lfosc_clk_calibration();
    printf("LFOSC calibration complete\n");
}

/* ============================================================
 * Main
 * ============================================================ */

/**
 * @brief Main entry of the PWMSLOW demo application.
 *
 * Common flow:
 * 1. Initialize demo environment
 * 2. Configure and calibrate LFOSC (required)
 * 3. Initialize PWMSLOW module
 * 4. Configure PWMSLOW output parameters
 * 5. Start PWMSLOW
 * 6. In one-shot mode, wait for completion callback
 *
 * @return This demo does not return under normal operation.
 */
int main(void)
{
    WISE_STATUS st;

    printf("\n=== PWMSLOW Demo (%s) ===\n",
#if (DEMO_PWMSLOW_MODE == DEMO_PWMSLOW_MODE_CONTINUOUS)
           "CONTINUOUS"
#else
           "ONESHOT"
#endif
    );

    demo_app_common_init();

    /* CRITICAL: Configure LFOSC before PWMSLOW initialization */
    demo_lfosc_config();
    printf("\n");

    /* Initialize PWMSLOW module */
    st = wise_pwmslow_init();
    if (st != WISE_SUCCESS) {
        printf("[ERR] wise_pwmslow_init failed\n");
        return 0;
    }
    printf("PWMSLOW initialized\n");

    /* Configure PWMSLOW */
    WISE_PWMSLOW_CONF_T pwmslow_conf;
    memset(&pwmslow_conf, 0, sizeof(pwmslow_conf));

#if (DEMO_PWMSLOW_MODE == DEMO_PWMSLOW_MODE_CONTINUOUS)

    /* ========== Continuous Mode ========== */
    pwmslow_conf.common.mode            = PWMSLOW_MODE_FREQUENCY;
    pwmslow_conf.u.freq.frequency_Hz    = PWMSLOW_FREQUENCY_HZ;
    pwmslow_conf.u.freq.duty_percent    = PWMSLOW_DUTY_PERCENT;
    pwmslow_conf.common.idle_status     = 0;
    pwmslow_conf.common.low_active_en   = DISABLE;
    pwmslow_conf.common.center_align_en = DISABLE;
    pwmslow_conf.common.oneshot_en      = DISABLE;
    pwmslow_conf.common.oneshot_num     = 0;

    st = wise_pwmslow_configure(PWMSLOW_GPIO_PIN, &pwmslow_conf);
    if (st != WISE_SUCCESS) {
        printf("[ERR] PWMSLOW configure failed\n");
        return 0;
    }

    printf("PWMSLOW configured:\n");
    printf("  GPIO Pin: %u\n", PWMSLOW_GPIO_PIN);
    printf("  Frequency: %lu Hz\n", pwmslow_conf.u.freq.frequency_Hz);
    printf("  Duty Cycle: %lu%%\n", pwmslow_conf.u.freq.duty_percent);
    printf("  Mode: Continuous\n");

    /* Start PWMSLOW */
    wise_pwmslow_start();
    printf("\nPWMSLOW started - running continuously\n");
    printf("Connect oscilloscope to GPIO%u to observe signal\n", PWMSLOW_GPIO_PIN);
    printf("Expected: %luHz signal, %lu%% duty cycle\n",
           pwmslow_conf.u.freq.frequency_Hz,
           pwmslow_conf.u.freq.duty_percent);
    printf("Note: output frequency depends on LFOSC configuration and calibration\n\n");

#else /* ONESHOT */

    /* ========== One-shot Mode ========== */
    pwmslow_conf.common.mode            = PWMSLOW_MODE_FREQUENCY;
    pwmslow_conf.u.freq.frequency_Hz    = PWMSLOW_FREQUENCY_HZ;
    pwmslow_conf.u.freq.duty_percent    = PWMSLOW_DUTY_PERCENT;
    pwmslow_conf.common.idle_status     = 0;
    pwmslow_conf.common.low_active_en   = DISABLE;
    pwmslow_conf.common.center_align_en = DISABLE;
    pwmslow_conf.common.oneshot_en      = ENABLE;
    pwmslow_conf.common.oneshot_num     = PWMSLOW_ONESHOT_COUNT;

    st = wise_pwmslow_configure(PWMSLOW_GPIO_PIN, &pwmslow_conf);
    if (st != WISE_SUCCESS) {
        printf("[ERR] PWMSLOW configure failed\n");
        return 0;
    }

    printf("PWMSLOW configured:\n");
    printf("  GPIO Pin: %u\n", PWMSLOW_GPIO_PIN);
    printf("  Frequency: %lu Hz\n", pwmslow_conf.u.freq.frequency_Hz);
    printf("  Duty Cycle: %lu%%\n", pwmslow_conf.u.freq.duty_percent);
    printf("  Mode: One-shot\n");
    printf("  Pulse Count: %u\n", pwmslow_conf.common.oneshot_num);

    /* Register callback for oneshot completion */
    st = wise_pwmslow_register_callback(PWMSLOW_EVENT_TRIGGER, demo_pwmslow_callback, NULL);
    if (st != WISE_SUCCESS) {
        printf("[ERR] PWMSLOW callback registration failed\n");
        return 0;
    }
    printf("PWMSLOW interrupt callback registered\n");

    /* Start PWMSLOW */
    g_pwmslow_complete = 0;
    wise_pwmslow_start();
    printf("\nPWMSLOW started - generating %u pulses\n", PWMSLOW_ONESHOT_COUNT);
    printf("Connect oscilloscope to GPIO%u to observe signal\n", PWMSLOW_GPIO_PIN);
    printf("Note: output frequency depends on LFOSC configuration and calibration\n\n");

    /* Wait for completion */
    printf("Waiting for PWMSLOW completion...\n");
    while (!g_pwmslow_complete) {
        __NOP();
    }

    printf("[PWMSLOW] ONESHOT_COMPLETE\n");
    printf("\nPWMSLOW one-shot sequence complete!\n");
    printf("Generated %u pulses at %luHz\n",
           PWMSLOW_ONESHOT_COUNT,
           pwmslow_conf.u.freq.frequency_Hz);

#endif

    while (1) {
        __WFI();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_PWMSLOW */

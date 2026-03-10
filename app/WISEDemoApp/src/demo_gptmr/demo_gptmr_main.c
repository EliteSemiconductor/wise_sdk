/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_gptmr_app.c
 * @brief Example application demonstrating WISE_GPTMR timer configuration and callbacks.
 *
 * @ingroup WISE_EXAMPLE_APP_GPTMR
 *
 * This example application demonstrates:
 * - Initializing the GPTMR (general-purpose timer) module
 * - Configuring a periodic timer channel and a one-shot timer channel
 * - Registering timer callbacks and starting timers
 * - Providing a simple UART shell prompt (transport only)
 *
 * The prompt is configured as "GPTMR> " on the selected UART channel.
 */

#include <stdio.h>
#include <stdbool.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "util.h"
#include "wise_gptmr_api.h"
#include "wise_shell_v2/src/shell.h"

#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_GPTMR GPTMR Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: GPTMR periodic and one-shot timers.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_GPTMR  for timer configuration, start/stop and callback
 * - @ref WISE_UART   for shell UART transport
 * - @ref WISE_CORE   for main processing loop
 *
 * Two channels are demonstrated:
 * - Channel 0: periodic timer with interval @ref GPTMR_0_INTERVAL
 * - Channel 1: one-shot timer with interval @ref GPTMR_1_INTERVAL
 * @{
 */

#define GPTMR_0_INTERVAL            2000000  /**< Channel 0 timer interval in microseconds. */
#define GPTMR_1_INTERVAL            12345678 /**< Channel 1 timer interval in microseconds. */
#define DEMO_APP_PROMPT             "GPTMR> "


static void demo_timer_callback(void *context, uint8_t idx);

/* ========================================================================== */
/* Timer Callback                                                             */
/* ========================================================================== */

/**
 * @brief GPTMR callback invoked when a timer channel expires.
 *
 * This callback is registered by ::wise_timer_register_callback() and is
 * triggered when the specified timer channel reaches its interval.
 *
 * @param[in] context User context pointer passed during registration.
 * @param[in] idx     Timer channel index that triggered the callback.
 */
static void demo_timer_callback(void *context, uint8_t idx)
{
    (void)context;
    printf("gptmr ch %d time up\r\n", idx);
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the GPTMR example application.
 *
 * Initializes the demo environment, configures two GPTMR channels (periodic
 * and one-shot), registers callbacks, starts timers, then enters the main
 * processing loop via ::wise_main_proc().
 */
void main(void)
{
    WISE_TIMER_CTRL_T timerCfg = {0};

    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    wise_timer_init();

    /* Channel 0: periodic timer */
    timerCfg.type = WISE_TIMER_PERIODIC;
    timerCfg.interval = GPTMR_0_INTERVAL;
    timerCfg.start_offset = 0;
    timerCfg.interrupt_enable = 1;

    wise_timer_config(0, &timerCfg);
    wise_timer_register_callback(0, demo_timer_callback, NULL);
    wise_timer_start(0);
    printf("GPTMR ch 0 started interval=%luus mode=PERIODIC\r\n",
           (unsigned long)GPTMR_0_INTERVAL);

    /* Channel 1: one-shot timer */
    timerCfg.type = WISE_TIMER_ONE_SHOT;
    timerCfg.interval = GPTMR_1_INTERVAL;
    timerCfg.start_offset = 0;
    timerCfg.interrupt_enable = 1;

    wise_timer_config(1, &timerCfg);
    wise_timer_register_callback(1, demo_timer_callback, NULL);
    wise_timer_start(1);
    printf("GPTMR ch 1 started interval=%luus mode=ONE_SHOT\r\n",
           (unsigned long)GPTMR_1_INTERVAL);

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_GPTMR */

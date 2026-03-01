/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_timer_hub_main.c
 * @brief Example application demonstrating WISE Timer Hub multi-channel scheduling.
 *
 * @ingroup WISE_EXAMPLE_APP_TIMER_HUB
 *
 * This example application demonstrates:
 * - Initializing Timer Hub module and allocating multiple timer channels
 * - Starting periodic timers on two channels
 * - Starting a one-shot timer on a third channel based on an event condition
 * - Using callbacks to signal timer expiration to the main loop
 * - Providing a UART shell prompt (for consistency with other demos)
 *
 * Timer behavior in this demo:
 * - Channel 0: periodic timer with @ref TIMER_HUB_0_INTERVAL
 * - Channel 1: periodic timer with @ref TIMER_HUB_1_INTERVAL (sets @ref timer1Up)
 * - Channel 2: one-shot timer started when channel 1 fires and channel 2 is not active
 *
 * The shell prompt is configured as "TMRHUB> " on the selected UART channel.
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "wise_sys_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

#include "wise_timer_hub.h"

/**
 * @defgroup WISE_EXAMPLE_APP_TIMER_HUB Timer Hub Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: Timer Hub multi-channel periodic/one-shot scheduling.
 *
 * This demo uses the following modules:
 * - Timer Hub module (middleware or service layer) for logical timer channels:
 *   - ::wise_timer_hub_init()
 *   - ::wise_timer_hub_alloc_ch()
 *   - ::wise_timer_hub_start_ch()
 * - @ref WISE_UART for shell UART transport
 * - @ref WISE_CORE for main processing loop
 *
 * @{
 */

#define SHELL_UART_CH           0     /**< UART channel used by shell backend. */
#define TIMER_HUB_0_INTERVAL    12345 /**< Timer Hub channel 0 periodic interval (ms/ticks per driver). */
#define TIMER_HUB_1_INTERVAL    4321  /**< Timer Hub channel 1 periodic interval (ms/ticks per driver). */
#define TIMER_HUB_2_INTERVAL    2120  /**< Timer Hub channel 2 one-shot interval (ms/ticks per driver). */

/** @brief Set when Timer Hub channel 1 callback fires. */
static uint8_t timer1Up = 0;

/** @brief Set when Timer Hub channel 2 callback fires. */
static uint8_t timer2Up = 0;

static void _timer_hub_cb_0(void *context);
static void _timer_hub_cb_1(void *context);
static void _timer_hub_cb_2(void *context);

/* ========================================================================== */
/* Shell Backend                                                              */
/* ========================================================================== */

/**
 * @brief Read one character from UART for shell input.
 *
 * @param[out] ch Pointer to the variable that receives the character.
 *
 * @retval true  A character was read successfully.
 * @retval false No character available or read failed.
 */
static bool shell_uart_read_char(char *ch)
{
    uint8_t tmp;
    if (wise_uart_read_char(SHELL_UART_CH, &tmp) == WISE_SUCCESS) {
        *ch = (char)tmp;
        return true;
    }
    return false;
}

/**
 * @brief Write a null-terminated string to UART for shell output.
 *
 * @param[in] s Null-terminated string to transmit.
 */
static void shell_uart_write_str(const char *s)
{
    while (*s) {
        wise_uart_write_char(SHELL_UART_CH, (uint8_t)*s++);
    }
}

/**
 * @brief Initialize shell configuration and bind UART backend callbacks.
 *
 * Sets the shell prompt to "TMRHUB> ".
 */
static void app_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = "TMRHUB> ",
    };

    shell_init(&cfg);
}

/* ========================================================================== */
/* Timer Hub Callbacks                                                        */
/* ========================================================================== */

/**
 * @brief Timer Hub callback for channel 0 expiration.
 *
 * @param[in] context User context pointer passed during channel allocation.
 */
static void _timer_hub_cb_0(void *context)
{
    (void)context;
    printf("ch 0 time up\r\n");
}

/**
 * @brief Timer Hub callback for channel 1 expiration.
 *
 * Sets @ref timer1Up flag for main-loop processing.
 *
 * @param[in] context User context pointer passed during channel allocation.
 */
static void _timer_hub_cb_1(void *context)
{
    (void)context;
    printf("ch 1 time up\r\n");
    timer1Up = 1;
}

/**
 * @brief Timer Hub callback for channel 2 expiration.
 *
 * Sets @ref timer2Up flag for main-loop processing.
 *
 * @param[in] context User context pointer passed during channel allocation.
 */
static void _timer_hub_cb_2(void *context)
{
    (void)context;
    printf("ch 2 time up\r\n");
    timer2Up = 1;
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the Timer Hub example application.
 *
 * Flow:
 * 1. Initialize demo environment and shell
 * 2. Initialize Timer Hub module
 * 3. Allocate three Timer Hub channels with callbacks
 * 4. Start periodic timers on channel 0 and channel 1
 * 5. In main loop:
 *    - When channel 1 fires, start a one-shot timer on channel 2 (if not already up)
 *    - Clear flags when handled
 *
 * @note The time unit of Timer Hub intervals depends on the implementation
 *       (e.g., milliseconds or WUTMR ticks). Refer to timer hub documentation
 *       for the exact unit.
 */
void main(void)
{
    int32_t timerHubHandle0 = WISE_TIMER_HUB_CH_INVALID;
    int32_t timerHubHandle1 = WISE_TIMER_HUB_CH_INVALID;
    int32_t timerHubHandle2 = WISE_TIMER_HUB_CH_INVALID;

    demo_app_common_init();
    app_shell_init();

    wise_timer_hub_init();

    timerHubHandle0 = wise_timer_hub_alloc_ch(_timer_hub_cb_0, NULL);
    timerHubHandle1 = wise_timer_hub_alloc_ch(_timer_hub_cb_1, NULL);
    timerHubHandle2 = wise_timer_hub_alloc_ch(_timer_hub_cb_2, NULL);

    printf("timer hub ch %ld start periodic\r\n", (long)timerHubHandle0);
    printf("timer hub ch %ld start periodic\r\n", (long)timerHubHandle1);

    timer1Up = 0;
    timer2Up = 0;

    wise_timer_hub_start_ch(timerHubHandle0, WISE_WUTMR_PERIODIC, TIMER_HUB_0_INTERVAL);
    wise_timer_hub_start_ch(timerHubHandle1, WISE_WUTMR_PERIODIC, TIMER_HUB_1_INTERVAL);

    while (1) {
        wise_main_proc();

        if (timer1Up) {
            timer1Up = 0;

            if (!timer2Up) {
                printf("timer hub ch %ld start one shot\r\n", (long)timerHubHandle2);
                wise_timer_hub_start_ch(timerHubHandle2, WISE_WUTMR_ONE_SHOT, TIMER_HUB_2_INTERVAL);
            }
        }

        if (timer2Up) {
            timer2Up = 0;
        }
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_TIMER_HUB */

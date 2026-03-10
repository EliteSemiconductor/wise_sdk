/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_wdt_main.c
 * @brief Example application demonstrating WISE_WDT watchdog configuration and refresh.
 *
 * @ingroup WISE_EXAMPLE_APP_WDT
 *
 * This example application demonstrates:
 * - Initializing and configuring the watchdog timer (WDT)
 * - Periodically refreshing the watchdog in the main loop
 * - Providing a shell command to intentionally block the main loop, causing WDT reset
 *
 * Supported shell commands:
 * - block : blocks the main loop forever to demonstrate watchdog timeout reset
 *
 * The shell prompt is configured as "WDT> " on the selected UART channel.
 */

#include <stdio.h>
#include <stdbool.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "util.h"
#include "wise_shell_v2/src/shell.h"
#include "wise_sys_api.h"
#include "wise_wdt_api.h"
#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_WDT WDT Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: watchdog timer init/config/start and refresh demo.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_WDT  for watchdog initialization/configuration/start and refresh
 * - @ref WISE_UART for shell UART transport
 * - @ref WISE_CORE for main processing loop
 *
 * @{
 */
#define DEMO_APP_PROMPT             "WDT> "
#define WDT_TIMEOUT_MS              4000 /**< Watchdog timeout in milliseconds. */

/* ========================================================================== */
/* block Command                                                              */
/* ========================================================================== */

/**
 * @brief Shell command: block the main loop to demonstrate watchdog reset.
 *
 * Usage:
 * @code
 * block
 * @endcode
 *
 * This command enters an infinite loop and stops watchdog refresh. If WDT reset
 * is enabled and timeout is configured, the system should reset after
 * @ref WDT_TIMEOUT_MS.
 *
 * @param[in] argc Argument count (unused).
 * @param[in] argv Argument vector (unused).
 *
 * @return This command does not return under normal operation.
 */
static int cmd_block(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("main loop blocked\r\n");
    while (1) {
        /* Intentionally block forever to trigger WDT timeout */
    }

    /* Unreachable */
    /* return 0; */
}

/** Register shell command "block". */
SHELL_CMD_AUTO(block, cmd_block, "Block main loop");

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the WDT example application.
 *
 * Flow:
 * 1. Initialize demo environment and shell
 * 2. Initialize watchdog module and configure timeout/reset behavior
 * 3. Start watchdog
 * 4. Periodically refresh watchdog in main loop
 *
 * The "block" command can be used to intentionally stop refreshing WDT, which
 * should cause a watchdog reset after timeout.
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    wise_wdt_init();
    wise_wdt_config(WDT_RESET_ENABLE, WDT_TIMEOUT_MS);
    wise_wdt_start();

    printf("WDT started timeout=%lums\r\n", (unsigned long)WDT_TIMEOUT_MS);

    while (1)
    {
        wise_wdt_set_refresh();
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_WDT */

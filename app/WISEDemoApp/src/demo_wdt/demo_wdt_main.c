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

#define SHELL_UART_CH       0    /**< UART channel used by shell backend. */
#define WDT_TIMEOUT_MS      4000 /**< Watchdog timeout in milliseconds. */

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
 * Sets the shell prompt to "WDT> ".
 */
static void app_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = "WDT> ",
    };

    shell_init(&cfg);
}

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
    app_shell_init();

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

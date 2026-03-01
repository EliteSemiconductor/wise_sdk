/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_wutmr_main.c
 * @brief Example application providing CLI to control WUTMR and power modes (idle/sleep).
 *
 * @ingroup WISE_EXAMPLE_APP_WUTMR
 *
 * This example application demonstrates:
 * - Using WUTMR (Wakeup Timer) counter and interrupt modes
 * - Starting periodic and one-shot WUTMR interrupts via CLI commands
 * - Entering CPU idle mode and system sleep mode for a specified duration
 * - Looping idle/sleep multiple times for current-consumption measurement
 * - Using the shell break detection (Ctrl+C) to stop/cancel operations
 *
 * Supported CLI commands:
 * - counter                 : display WUTMR counter value
 * - start <ms>              : start periodic WUTMR interrupt, Ctrl+C to stop
 * - oneshot <ms>            : start one-shot WUTMR interrupt, Ctrl+C to cancel
 * - idle <ms>               : enter CPU idle mode once
 * - sleep <ms>              : enter system sleep mode once
 * - loop_idle <ms> <n>      : enter CPU idle mode repeatedly
 * - loop_sleep <ms> <n>     : enter system sleep mode repeatedly
 *
 * Shell prompt is configured as "WUTMR> " on the selected UART channel.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "wise.h"
#include "wise_uart_api.h"
#include "wise_shell_v2/src/shell.h"
#include "wise_wutmr_api.h"
#include "wise_sys_api.h"
#include "esmt_chip_specific.h"
#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_WUTMR WUTMR Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: WUTMR control and power-mode (idle/sleep) measurement CLI.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_WUTMR for wakeup timer counter/interrupt/start/stop
 * - @ref WISE_SYS  for power management (idle/sleep)
 * - @ref WISE_UART for shell UART transport
 * - @ref WISE_CORE for main processing loop
 *
 * @{
 */

#define SHELL_UART_CH  0 /**< UART channel used by shell backend. */

/** @brief WUTMR interrupt counter (incremented in ISR callback). */
static uint32_t wutmrIntCount = 0;

static void wutmrIsrCallback(void *context);

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
 * Sets the shell prompt to "WUTMR> ".
 */
static void app_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = "WUTMR> ",
    };

    shell_init(&cfg);
}

/* ========================================================================== */
/* counter - Display WUTMR counter value                                      */
/* ========================================================================== */

/**
 * @brief Shell command: display current WUTMR counter value.
 *
 * Usage:
 * @code
 * counter
 * @endcode
 *
 * @param[in] argc Argument count (unused).
 * @param[in] argv Argument vector (unused).
 *
 * @retval 0 Always returns 0.
 */
static int cmd_counter(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("WUTMR counter: %lu\r\n", (unsigned long)wise_wutmr_get_counter());
    return 0;
}

/** Register shell command "counter". */
SHELL_CMD_AUTO(counter, cmd_counter, "Display WUTMR counter value");

/* ========================================================================== */
/* start - Periodic WUTMR interrupt (Ctrl+C to stop)                          */
/* ========================================================================== */

/**
 * @brief Shell command: start periodic WUTMR interrupt.
 *
 * Usage:
 * @code
 * start <interval_ms>
 * @endcode
 *
 * Starts a periodic WUTMR interrupt with the given interval and waits until
 * Ctrl+C is detected by ::shell_poll_break().
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector. argv[1] is interval in milliseconds.
 *
 * @retval 0  Command completed (stopped).
 * @retval -1 Invalid parameters.
 *
 * @note Minimum interval is clamped to 5 ms.
 */
static int cmd_start(int argc, char **argv)
{
    uint32_t intervalMs;

    if (argc != 2) {
        printf("Usage: start <interval_ms>\r\n");
        printf("  Start periodic WUTMR interrupt, press Ctrl+C to stop\r\n");
        return -1;
    }

    intervalMs = strtoul(argv[1], NULL, 10);
    if (intervalMs < 5) {
        intervalMs = 5;
    }

    wutmrIntCount = 0;  /* Reset interrupt counter */
    wise_wutmr_enable_int();
    wise_wutmr_start(intervalMs, WISE_WUTMR_PERIODIC);

    printf("Periodic WUTMR started (interval=%lums), press Ctrl+C to stop...\r\n",
           (unsigned long)intervalMs);

    while (!shell_poll_break()) {
        /* Wait for WUTMR interrupt or Ctrl+C */
    }

    wise_wutmr_stop();
    printf("Stopped\r\n");

    return 0;
}

/** Register shell command "start". */
SHELL_CMD_AUTO(start, cmd_start, "start <ms> - Periodic WUTMR (Ctrl+C stop)");

/* ========================================================================== */
/* oneshot - One-shot WUTMR interrupt (Ctrl+C to cancel)                      */
/* ========================================================================== */

/**
 * @brief Shell command: start one-shot WUTMR interrupt.
 *
 * Usage:
 * @code
 * oneshot <interval_ms>
 * @endcode
 *
 * Starts a one-shot WUTMR interrupt and waits until an interrupt occurs or
 * Ctrl+C is detected by ::shell_poll_break(). The timer is stopped afterward.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector. argv[1] is interval in milliseconds.
 *
 * @retval 0  Command completed.
 * @retval -1 Invalid parameters.
 *
 * @note Minimum interval is clamped to 5 ms.
 */
static int cmd_oneshot(int argc, char **argv)
{
    uint32_t intervalMs;

    if (argc != 2) {
        printf("Usage: oneshot <interval_ms>\r\n");
        printf("  Start one-shot WUTMR, press Ctrl+C to cancel\r\n");
        return -1;
    }

    intervalMs = strtoul(argv[1], NULL, 10);
    if (intervalMs < 5) {
        intervalMs = 5;
    }

    wise_wutmr_enable_int();
    wise_wutmr_start(intervalMs, WISE_WUTMR_ONE_SHOT);

    printf("One-shot WUTMR started (interval=%lums), waiting...\r\n",
           (unsigned long)intervalMs);

    while (!shell_poll_break()) {
        /* Wait for WUTMR interrupt or Ctrl+C */
    }

    wise_wutmr_stop();

    return 0;
}

/** Register shell command "oneshot". */
SHELL_CMD_AUTO(oneshot, cmd_oneshot, "oneshot <ms> - One-shot WUTMR (Ctrl+C cancel)");

/* ========================================================================== */
/* idle - Enter CPU idle mode once                                            */
/* ========================================================================== */

/**
 * @brief Shell command: enter CPU idle mode once for a specified duration.
 *
 * Usage:
 * @code
 * idle <interval_ms>
 * @endcode
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector. argv[1] is interval in milliseconds.
 *
 * @retval 0  Command completed.
 * @retval -1 Invalid parameters.
 *
 * @note Minimum interval is clamped to 5 ms.
 */
static int cmd_idle(int argc, char **argv)
{
    uint32_t intervalMs;

    if (argc != 2) {
        printf("Usage: idle <interval_ms>\r\n");
        printf("  Enter CPU idle mode for specified duration\r\n");
        return -1;
    }

    intervalMs = strtoul(argv[1], NULL, 10);
    if (intervalMs < 5) {
        intervalMs = 5;
    }

    printf("Entering CPU idle (%lums)...\r\n", (unsigned long)intervalMs);
    wise_system_idle(intervalMs);
    printf("CPU wakeup\r\n");

    return 0;
}

/** Register shell command "idle". */
SHELL_CMD_AUTO(idle, cmd_idle, "idle <ms> - Enter CPU idle once");

/* ========================================================================== */
/* sleep - Enter system sleep mode once                                       */
/* ========================================================================== */

/**
 * @brief Shell command: enter system sleep mode once for a specified duration.
 *
 * Usage:
 * @code
 * sleep <interval_ms>
 * @endcode
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector. argv[1] is interval in milliseconds.
 *
 * @retval 0  Command completed.
 * @retval -1 Invalid parameters.
 *
 * @note Minimum interval is clamped to 5 ms.
 */
static int cmd_sleep(int argc, char **argv)
{
    uint32_t intervalMs;

    if (argc != 2) {
        printf("Usage: sleep <interval_ms>\r\n");
        printf("  Enter system sleep mode for specified duration\r\n");
        return -1;
    }

    intervalMs = strtoul(argv[1], NULL, 10);
    if (intervalMs < 5) {
        intervalMs = 5;
    }

    printf("Entering system sleep (%lums)...\r\n", (unsigned long)intervalMs);
    wise_system_sleep(intervalMs);
    printf("System wakeup\r\n");

    return 0;
}

/** Register shell command "sleep". */
SHELL_CMD_AUTO(sleep, cmd_sleep, "sleep <ms> - Enter system sleep once");

/* ========================================================================== */
/* loop_idle - Enter CPU idle mode n times (for current measurement)          */
/* ========================================================================== */

/**
 * @brief Shell command: enter CPU idle mode repeatedly for current measurement.
 *
 * Usage:
 * @code
 * loop_idle <interval_ms> <count>
 * @endcode
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector:
 *                - argv[1] interval in milliseconds
 *                - argv[2] loop count
 *
 * @retval 0  Command completed.
 * @retval -1 Invalid parameters.
 *
 * @note Minimum interval is clamped to 5 ms. Minimum count is 1.
 */
static int cmd_loop_idle(int argc, char **argv)
{
    uint32_t intervalMs;
    uint32_t count;
    uint32_t i;

    if (argc != 3) {
        printf("Usage: loop_idle <interval_ms> <count>\r\n");
        printf("  Enter CPU idle mode repeatedly for current measurement\r\n");
        printf("  Example: loop_idle 1000 10  (idle 1s, repeat 10 times)\r\n");
        return -1;
    }

    intervalMs = strtoul(argv[1], NULL, 10);
    if (intervalMs < 5) {
        intervalMs = 5;
    }

    count = strtoul(argv[2], NULL, 10);
    if (count == 0) {
        count = 1;
    }

    printf("CPU idle loop started (interval=%lums, count=%lu)\r\n",
           (unsigned long)intervalMs, (unsigned long)count);

    for (i = 0; i < count; i++) {
        wise_system_idle(intervalMs);
        printf("[%lu/%lu] wakeup\r\n", (unsigned long)(i + 1), (unsigned long)count);
    }

    printf("CPU idle loop completed\r\n");

    return 0;
}

/** Register shell command "loop_idle". */
SHELL_CMD_AUTO(loop_idle, cmd_loop_idle, "loop_idle <ms> <n> - CPU idle n times");

/* ========================================================================== */
/* loop_sleep - Enter system sleep mode n times (for current measurement)     */
/* ========================================================================== */

/**
 * @brief Shell command: enter system sleep mode repeatedly for current measurement.
 *
 * Usage:
 * @code
 * loop_sleep <interval_ms> <count>
 * @endcode
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector:
 *                - argv[1] interval in milliseconds
 *                - argv[2] loop count
 *
 * @retval 0  Command completed.
 * @retval -1 Invalid parameters.
 *
 * @note Minimum interval is clamped to 5 ms. Minimum count is 1.
 */
static int cmd_loop_sleep(int argc, char **argv)
{
    uint32_t intervalMs;
    uint32_t count;
    uint32_t i;

    if (argc != 3) {
        printf("Usage: loop_sleep <interval_ms> <count>\r\n");
        printf("  Enter system sleep mode repeatedly for current measurement\r\n");
        printf("  Example: loop_sleep 1000 10  (sleep 1s, repeat 10 times)\r\n");
        return -1;
    }

    intervalMs = strtoul(argv[1], NULL, 10);
    if (intervalMs < 5) {
        intervalMs = 5;
    }

    count = strtoul(argv[2], NULL, 10);
    if (count == 0) {
        count = 1;
    }

    printf("System sleep loop started (interval=%lums, count=%lu)\r\n",
           (unsigned long)intervalMs, (unsigned long)count);

    for (i = 0; i < count; i++) {
        wise_system_sleep(intervalMs);
        printf("[%lu/%lu] wakeup\r\n", (unsigned long)(i + 1), (unsigned long)count);
    }

    printf("System sleep loop completed\r\n");

    return 0;
}

/** Register shell command "loop_sleep". */
SHELL_CMD_AUTO(loop_sleep, cmd_loop_sleep, "loop_sleep <ms> <n> - System sleep n times");

/* ========================================================================== */
/* WUTMR Callback                                                             */
/* ========================================================================== */

/**
 * @brief WUTMR interrupt callback.
 *
 * Increments @ref wutmrIntCount and prints an interrupt message.
 *
 * @param[in] context User context pointer passed during callback registration.
 */
static void wutmrIsrCallback(void *context)
{
    (void)context;
    wutmrIntCount++;
    printf("[%lu] WUTMR interrupt\r\n", (unsigned long)wutmrIntCount);
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the WUTMR CLI demo application.
 *
 * Flow:
 * 1. Initialize demo environment and shell
 * 2. Register WUTMR callback via ::wise_wutmr_register_callback()
 * 3. Print banner and wait for user commands
 * 4. Run ::wise_main_proc() in the main loop
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init();

    wise_wutmr_register_callback(wutmrIsrCallback, NULL);

    printf("\r\n=== WUTMR Demo ===\r\n");
    printf("Type 'help' for available commands\r\n\r\n");

    while (1)
    {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_WUTMR */

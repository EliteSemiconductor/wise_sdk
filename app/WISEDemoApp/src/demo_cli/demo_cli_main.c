/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_shell_app.c
 * @brief Example application demonstrating UART-based shell integration.
 *
 * @ingroup WISE_EXAMPLE_APP_SHELL_DEMO
 *
 * This example shows how to:
 * - Initialize the demo system environment
 * - Integrate wise_shell_v2 with WISE_UART_API as backend
 * - Implement basic shell read/write callbacks
 * - Run a simple interactive shell loop
 *
 * The shell prompt is configured as "DEMO> " and communicates through
 * the specified UART channel.
 */

#include <stdio.h>
#include <stdbool.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_uart_api.h"
#include "wise_radio_api.h"
#include "wise_sys_api.h"
#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_SHELL_DEMO Shell Demo App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: UART shell using WISE Core APIs.
 *
 * This demo utilizes the following Core API modules:
 * - @ref WISE_UART  for shell I/O backend
 * - @ref WISE_CORE  for system main processing loop
 * - @ref WISE_SYS   for system-level support (via demo init)
 *
 * It demonstrates how to bind a shell framework to UART transport
 * and run a continuous command-processing loop.
 * @{
 */

#define SHELL_UART_CH 0 /**< UART channel used as the shell communication interface. */

/**
 * @brief Read one character from the shell UART backend.
 *
 * This function is registered as the shell's read callback and is called
 * by the shell engine to fetch incoming characters.
 *
 * @param[out] ch Pointer to the variable that receives the character.
 *
 * @retval true  A character was successfully read.
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
 * @brief Write a string to the shell UART backend.
 *
 * This function is registered as the shell's write callback and is used
 * to output shell responses through UART.
 *
 * @param[in] s Null-terminated string to be transmitted.
 */
static void shell_uart_write_str(const char *s)
{
    while (*s) {
        wise_uart_write_char(SHELL_UART_CH, (uint8_t)*s++);
    }
}

/**
 * @brief Initialize the shell module and configure UART backend.
 *
 * This function sets up the shell configuration, including:
 * - UART read callback
 * - UART write callback
 * - Shell prompt string
 */
static void app_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = "DEMO> ",
    };

    shell_init(&cfg);
}

/**
 * @brief Main entry of the shell demo application.
 *
 * The application performs the following steps:
 * 1. Initialize common demo environment
 * 2. Initialize UART-based shell backend
 * 3. Enter infinite loop to process system tasks via ::wise_main_proc()
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init();

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_SHELL_DEMO */

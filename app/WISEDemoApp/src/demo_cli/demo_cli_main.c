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

#define DEMO_APP_PROMPT             "SHELL> "

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
    app_shell_init(DEMO_APP_PROMPT);

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_SHELL_DEMO */

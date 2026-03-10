/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_loader_shell_app.c
 * @brief Example loader application with UART shell, kermit update, and boot-to-app.
 *
 * @ingroup WISE_EXAMPLE_APP_LOADER
 *
 * This example application demonstrates:
 * - Building a simple shell backend over UART using WISE_UART_API
 * - Adding shell commands (boot / kermit)
 * - Updating an application image into flash via kermit protocol
 * - Booting to an application image by remapping system memory
 *
 * The application is intended to run as a small loader/monitor program.
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_sys_api.h"
#include "wise_uart_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"
#include "wise_kermit.h"


/**
 * @defgroup WISE_EXAMPLE_APP_LOADER Loader Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: shell-based loader with kermit update and boot.
 *
 * This example uses the following Core APIs:
 * - @ref WISE_UART (UART shell I/O)
 * - @ref WISE_SYS  (system remap / boot helper)
 * - @ref WISE_GPIO (deinit before remap)
 * - @ref WISE_PMU  (clock gating before remap)
 *
 * It also uses:
 * - wise_shell_v2 for command registration
 * - wise_kermit for receiving the application image
 * @{
 */

#define APP_BOOT_ADDR               0x6000 /**< Boot address of the target application image. */
#define DEMO_APP_PROMPT             "LOADER> "

static void bootToAPP(uint32_t addr);


/* ========================================================================== */
/* boot Command                                                               */
/* ========================================================================== */

/**
 * @brief Shell command: boot to application image.
 *
 * Boots to the application image located at @ref APP_BOOT_ADDR by calling
 * ::bootToAPP().
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success (command accepted).
 */
static int cmd_boot(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("booting to APP @%08lx\r\n", (unsigned long)APP_BOOT_ADDR);
    bootToAPP(APP_BOOT_ADDR);

    return 0;
}

/** Register "boot" command to shell. */
SHELL_CMD_AUTO(boot, cmd_boot, "Boot to APP");

/* ========================================================================== */
/* kermit Command                                                             */
/* ========================================================================== */

/**
 * @brief Shell command: update application image through kermit receiver.
 *
 * Starts the kermit receiver and writes received image into flash at
 * @ref APP_BOOT_ADDR. On success, prints received size and CRC.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success (command accepted).
 */
static int cmd_kermit(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    uint32_t rxDataLen = 0;
    uint16_t rxDataCrc = 0;

    if (WISE_SUCCESS == wise_kermit_init(E_KERMIT_TARGET_FLASH, APP_BOOT_ADDR, STDIO_UART_PORT)) {
        printf("Start kermit receiver\r\n");

        if (WISE_SUCCESS == wise_kermit_load(&rxDataLen, &rxDataCrc)) {
            printf("APP updated size=%08lx crc=%04x\r\n", (unsigned long)rxDataLen, rxDataCrc);
        } else {
            printf("aborted\r\n");
        }
    }

    return 0;
}

/** Register "kermit" command to shell. */
SHELL_CMD_AUTO(kermit, cmd_kermit, "Update APP through kermit");

/* ========================================================================== */
/* Boot Helper                                                                */
/* ========================================================================== */

/**
 * @brief Boot to an application image by remapping system memory.
 *
 * This routine performs a minimal "handoff" sequence:
 * - Disable STDIO UART (stop shell/printf traffic)
 * - Deinitialize GPIO and gate peripheral clocks
 * - Disable interrupts
 * - Insert a short delay (NOP loop) to allow HW to settle
 * - Remap system memory to @p addr
 *
 * @param[in] addr Boot address of the application image.
 *
 * @note The actual behavior after remap depends on the platform boot flow.
 *       Some systems will jump to reset vector automatically; others may
 *       require additional steps (e.g., setting MSP/PC and branching).
 */
static void bootToAPP(uint32_t addr)
{
    uint32_t wait = 20000;

    wise_uart_enable(STDIO_UART_PORT, WISE_UART_FLAG_NONE);
    wise_gpio_deinit();
    wise_pmu_module_clk_disable(0x1FFFFFFF);

    __disable_irq();
    while (wait--) {
        asm("nop");
    }

    wise_sys_remap(addr);
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the loader example application.
 *
 * Initializes common demo environment and starts the shell. The main loop
 * periodically calls ::wise_main_proc().
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_LOADER */

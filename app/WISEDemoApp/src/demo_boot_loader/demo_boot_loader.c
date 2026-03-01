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

#define SHELL_UART_CH       0      /**< UART channel used by shell backend. */
#define APP_BOOT_ADDR       0x6000 /**< Boot address of the target application image. */

static void bootToAPP(uint32_t addr);

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

/* ========================================================================== */
/* Shell Backend                                                              */
/* ========================================================================== */

/**
 * @brief Read one character from the shell UART channel.
 *
 * This function is used as the shell backend "read_char" callback.
 *
 * @param[out] ch Pointer to the output character.
 *
 * @retval true  A character was read successfully.
 * @retval false No character available / read failed.
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
 * @brief Write a null-terminated string to the shell UART channel.
 *
 * This function is used as the shell backend "write_str" callback.
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
 * @brief Initialize shell module and register UART backend callbacks.
 *
 * The shell prompt string is configured as "LOADER> ".
 */
static void app_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = "LOADER> ",
    };

    shell_init(&cfg);
}

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
    app_shell_init();

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_LOADER */

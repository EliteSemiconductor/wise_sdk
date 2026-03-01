/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_power_mgmt_app.c
 * @brief Example application demonstrating idle/sleep/shutdown power modes and wakeup sources.
 *
 * @ingroup WISE_EXAMPLE_APP_POWER_MGMT
 *
 * This example application demonstrates:
 * - Entering CPU idle mode via ::wise_system_idle()
 * - Entering system sleep mode via ::wise_system_sleep()
 * - Entering shutdown mode via ::wise_sys_enter_shutdown_mode()
 * - Configuring wakeup sources using WUTMR and GPIO interrupt
 * - Providing a UART shell prompt to trigger power commands interactively
 *
 * Wakeup sources used in this demo:
 * - WUTMR one-shot timer (timeout defined by @ref SLEEP_TIME)
 * - GPIO level trigger on @ref WAKEUP_GPIO_PIN
 * - Shutdown mode additionally enables NFC wakeup source (platform dependent)
 *
 * The shell prompt is configured as "PM> " on the selected UART channel.
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
#include "wise_wutmr_api.h"
#include "wise_sys_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_POWER_MGMT Power Management Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: idle/sleep/shutdown power modes with wakeup triggers.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_SYS   for entering low-power modes (idle/sleep/shutdown)
 * - @ref WISE_WUTMR for wakeup timer configuration
 * - @ref WISE_GPIO  for GPIO wakeup interrupt configuration
 * - @ref WISE_UART  for shell UART transport
 * - @ref WISE_CORE  for main processing loop
 *
 * Shell commands:
 * - idle     : enter CPU idle mode, wake by WUTMR or GPIO
 * - sleep    : enter system sleep mode, wake by WUTMR or GPIO
 * - shutdown : enter shutdown mode, reboot after wakeup timer or GPIO trigger
 * @{
 */

#define SHELL_UART_CH       0      /**< UART channel used by shell backend. */
#define SLEEP_TIME          20000  /**< Default wakeup timer duration in milliseconds. */
#define WAKEUP_GPIO_PIN     5      /**< GPIO pin index used as wakeup trigger. */
#define SHUTDOWN_TIME       60000  /**< Shutdown duration in milliseconds before auto wake/reboot. */

static void _setup_wakeup_trigger(void);
static void _wakeup_io_int_callback(void *context, uint8_t idx);

/** @brief Runtime-configurable sleep duration used by WUTMR start, in milliseconds. */
static uint32_t sleepMs = SLEEP_TIME;

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
 * Sets the shell prompt to "PM> ".
 */
static void app_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = "PM> ",
    };

    shell_init(&cfg);
}

/* ========================================================================== */
/* idle Command                                                               */
/* ========================================================================== */

/**
 * @brief Shell command: enter CPU idle mode (wake by WUTMR or GPIO).
 *
 * This command configures wakeup triggers via ::_setup_wakeup_trigger(),
 * then enters CPU idle mode using ::wise_system_idle() with
 * ::WISE_TIME_FOREVER timeout.
 *
 * @param[in] argc Argument count (unused).
 * @param[in] argv Argument vector (unused).
 *
 * @return 0 on success (command accepted).
 */
static int cmd_idle(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    _setup_wakeup_trigger();

    printf("Start CPU idle curTick=%08lx\r\n", (unsigned long)wise_wutmr_get_counter());
    wise_system_idle(WISE_TIME_FOREVER);
    printf("CPU wake up curTick=%08lx\r\n", (unsigned long)wise_wutmr_get_counter());

    return 0;
}

/** Register shell command "idle". */
SHELL_CMD_AUTO(idle, cmd_idle, "Enter CPU idle mode");

/* ========================================================================== */
/* sleep Command                                                              */
/* ========================================================================== */

/**
 * @brief Shell command: enter system sleep mode (wake by WUTMR or GPIO).
 *
 * This command configures wakeup triggers via ::_setup_wakeup_trigger(),
 * then enters system sleep mode using ::wise_system_sleep() with
 * ::WISE_TIME_FOREVER timeout.
 *
 * @param[in] argc Argument count (unused).
 * @param[in] argv Argument vector (unused).
 *
 * @return 0 on success (command accepted).
 */
static int cmd_sleep(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    _setup_wakeup_trigger();

    printf("Start system sleep curTick=%08lx\r\n", (unsigned long)wise_wutmr_get_counter());
    wise_system_sleep(WISE_TIME_FOREVER);
    printf("System wake up curTick=%08lx\r\n", (unsigned long)wise_wutmr_get_counter());

    return 0;
}

/** Register shell command "sleep". */
SHELL_CMD_AUTO(sleep, cmd_sleep, "Enter system sleep mode");

/* ========================================================================== */
/* shutdown Command                                                           */
/* ========================================================================== */

/**
 * @brief Shell command: enter shutdown mode (wake/reboot by timer, NFC, or GPIO).
 *
 * Configures shutdown wakeup sources and enters shutdown mode. The system
 * is expected to reboot or resume according to platform shutdown behavior.
 *
 * Wakeup sources configured in this demo:
 * - WUTMR wakeup timer
 * - NFC wakeup source
 * - GPIO wakeup source on @ref WAKEUP_GPIO_PIN
 *
 * @param[in] argc Argument count (unused).
 * @param[in] argv Argument vector (unused).
 *
 * @return 0 on success (command accepted).
 */
static int cmd_shutdown(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    SYS_SHUTDOWN_CFT_T shutdownCfg = {
        .wake_src     = SHUTDOWN_WAKE_SRC_WUTMR | SHUTDOWN_WAKE_SRC_NFC | SHUTDOWN_WAKE_SRC_GPIO,
        .shutdown_ms  = SHUTDOWN_TIME,
        .wake_io_idx  = WAKEUP_GPIO_PIN,
    };

    printf("Start shutdown mode. System will reboot after:\r\n");
    printf("   1. wakeup timer up\r\n");
    printf("   2. GPIO %d high trigger\r\n", WAKEUP_GPIO_PIN);

    wise_sys_enter_shutdown_mode(shutdownCfg);

    return 0;
}

/** Register shell command "shutdown". */
SHELL_CMD_AUTO(shutdown, cmd_shutdown, "Enter shutdown mode");

/* ========================================================================== */
/* Wakeup Helpers                                                             */
/* ========================================================================== */

/**
 * @brief GPIO interrupt callback used as wakeup trigger.
 *
 * Disables further GPIO interrupt for @ref WAKEUP_GPIO_PIN and clears any
 * pending GPIO IRQ to avoid repeated triggers.
 *
 * @param[in] context User context pointer passed during registration.
 * @param[in] idx     GPIO pin index that triggered the interrupt.
 */
static void _wakeup_io_int_callback(void *context, uint8_t idx)
{
    (void)context;
    (void)idx;

    WISE_GPIO_INT_CFG_T gpioIntConfig;

    printf("gpio %d int\r\n", WAKEUP_GPIO_PIN);

    gpioIntConfig.int_en   = GPIO_INT_DISABLE;
    gpioIntConfig.int_type = GPIO_INT_TYPE_LEVEL_LOW;
    wise_gpio_set_interrupt(WAKEUP_GPIO_PIN, &gpioIntConfig);

    NVIC_ClearPendingIRQ(GPIO_IRQn);
}

/**
 * @brief Configure wakeup triggers for idle/sleep modes.
 *
 * This helper enables two wakeup sources:
 * - GPIO interrupt on @ref WAKEUP_GPIO_PIN (level-low trigger)
 * - WUTMR one-shot timer with duration @ref sleepMs
 *
 * It configures GPIO input + interrupt, clears pending status, and registers
 * ::_wakeup_io_int_callback. It then starts WUTMR in one-shot mode and enables
 * WUTMR interrupt.
 */
static void _setup_wakeup_trigger(void)
{
    WISE_GPIO_INT_CFG_T gpioIntConfig;

    wise_gpio_set_mode(WAKEUP_GPIO_PIN, MODE_GPIO);
    wise_gpio_set_direction(WAKEUP_GPIO_PIN, GPIO_DIR_INPUT);
    wise_gpio_register_callback(WAKEUP_GPIO_PIN, _wakeup_io_int_callback, NULL);
    wise_gpio_clear_int_status(WAKEUP_GPIO_PIN);

    gpioIntConfig.int_en   = GPIO_INT_ENABLE;
    gpioIntConfig.int_type = GPIO_INT_TYPE_LEVEL_LOW;
    wise_gpio_set_interrupt(WAKEUP_GPIO_PIN, &gpioIntConfig);

    wise_wutmr_start(sleepMs, WISE_WUTMR_ONE_SHOT);
    wise_wutmr_enable_int();
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the power management example application.
 *
 * Initializes demo environment and shell, then enters the main processing loop
 * via ::wise_main_proc(). Power mode transitions are triggered by shell commands.
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init();

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_POWER_MGMT */

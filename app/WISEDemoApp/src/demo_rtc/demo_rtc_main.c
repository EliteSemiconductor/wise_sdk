/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_rtc_app.c
 * @brief Example application demonstrating WISE_RTC time read/set and alarm via UART shell.
 *
 * @ingroup WISE_EXAMPLE_APP_RTC
 *
 * This example application demonstrates:
 * - Initializing RTC and setting a start time
 * - Reading current time through RTC
 * - Updating current time through RTC
 * - Configuring an RTC alarm and receiving alarm callback
 * - Providing a UART shell prompt to trigger RTC commands interactively
 *
 * Supported shell commands:
 * - time                  : print current RTC time
 * - set [day] [h] [m] [s] : set current RTC time
 * - alarm [h] [m] [s]     : configure alarm time and enable alarm interrupt
 *
 * The shell prompt is configured as "RTC> " on the selected UART channel.
 *
 * @note The original code snippet contains a duplicated include line:
 *       `#include "wise_shell_v2/src/shell.h"#include "wise_shell_v2/src/shell.h"`
 *       This should be separated into a single include in real source code.
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>

#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_rtc_api.h"
#include "wise_uart_api.h"
#include "util.h"
#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_RTC RTC Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: RTC time read/set and alarm configuration via shell.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_RTC   for RTC timekeeping and alarm configuration
 * - @ref WISE_UART  for shell UART transport
 * - @ref WISE_CORE  for main processing loop
 *
 * Shell commands:
 * - time                  : display current RTC time
 * - set [day] [hour] [min] [sec]
 * - alarm [hour] [min] [sec]
 * @{
 */
#define DEMO_APP_PROMPT             "RTC> "

static void _rtc_alarm_callback(void *context, uint8_t idx);

/* ========================================================================== */
/* time Command                                                               */
/* ========================================================================== */

/**
 * @brief Shell command: display current RTC time.
 *
 * Usage:
 * @code
 * time
 * @endcode
 *
 * Reads current RTC counter via ::wise_rtc_get_time() and prints it.
 *
 * @param[in] argc Argument count (unused).
 * @param[in] argv Argument vector (unused).
 *
 * @return 0 on success.
 */
static int cmd_time(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    WISE_RTC_CNT_T now;

    wise_rtc_get_time(&now);
    printf("Current time: Day %d %02d:%02d:%02d\r\n",
           now.day, now.time.hour, now.time.min, now.time.sec);

    return 0;
}

/** Register shell command "time". */
SHELL_CMD_AUTO(time, cmd_time, "Display current time");

/* ========================================================================== */
/* set Command                                                                */
/* ========================================================================== */

/**
 * @brief Shell command: set current RTC time.
 *
 * Usage:
 * @code
 * set [day] [hour] [min] [sec]
 * @endcode
 *
 * Parses parameters as decimal integers and updates RTC counter via
 * ::wise_rtc_set_time().
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success, negative value on invalid parameters.
 */
static int cmd_set(int argc, char **argv)
{
    WISE_RTC_CNT_T newTime = {0};

    if (argc != 5) {
        printf("Invalid parameters:\r\n");
        printf("Usage: set [day] [hour] [min] [sec]\r\n");
        return -1;
    }

    newTime.day = (uint8_t)strtoul(argv[1], NULL, 10);
    if (errno != 0) goto invalid;
    newTime.time.hour = (uint8_t)strtoul(argv[2], NULL, 10);
    if (errno != 0) goto invalid;
    newTime.time.min = (uint8_t)strtoul(argv[3], NULL, 10);
    if (errno != 0) goto invalid;
    newTime.time.sec = (uint8_t)strtoul(argv[4], NULL, 10);
    if (errno != 0) goto invalid;

    wise_rtc_set_time(&newTime);
    printf("Set new time: day %d %02d:%02d:%02d\r\n",
           newTime.day, newTime.time.hour, newTime.time.min, newTime.time.sec);
    return 0;

invalid:
    printf("Invalid parameters:\r\n");
    printf("Usage: set [day] [hour] [min] [sec]\r\n");
    return -1;
}

/** Register shell command "set". */
SHELL_CMD_AUTO(set, cmd_set, "Set current time");

/* ========================================================================== */
/* alarm Command                                                              */
/* ========================================================================== */

/**
 * @brief RTC alarm callback invoked when alarm event is triggered.
 *
 * @param[in] context User context pointer passed during registration.
 * @param[in] idx     Event/channel index (driver-defined).
 */
static void _rtc_alarm_callback(void *context, uint8_t idx)
{
    (void)context;
    (void)idx;
    printf("Bi!!Bi!!Bi!!Bi!!Bi!!Bi!!\r\n");
}

/**
 * @brief Shell command: configure an RTC alarm time.
 *
 * Usage:
 * @code
 * alarm [hour] [min] [sec]
 * @endcode
 *
 * Configures alarm channel 0 and enables interrupt. The alarm callback is
 * registered via ::wise_rtc_register_callback() for @ref RTC_EVENT_ALARM.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success, negative value on invalid parameters.
 */
static int cmd_alarm(int argc, char **argv)
{
    RTC_TIME_T alarmTime = {0};
    WISE_RTC_ALM_CFG_T alarm_cfg;

    if (argc != 4) {
        printf("Invalid parameters:\r\n");
        printf("Usage: alarm [hour] [min] [sec]\r\n");
        return -1;
    }

    alarmTime.hour = (uint8_t)strtoul(argv[1], NULL, 10);
    if (errno != 0) goto invalid;
    alarmTime.min = (uint8_t)strtoul(argv[2], NULL, 10);
    if (errno != 0) goto invalid;
    alarmTime.sec = (uint8_t)strtoul(argv[3], NULL, 10);
    if (errno != 0) goto invalid;

    printf("Set alarm time: %02d:%02d:%02d\r\n",
           alarmTime.hour, alarmTime.min, alarmTime.sec);

    alarm_cfg.channel   = 0;
    alarm_cfg.enable    = 1;
    alarm_cfg.int_en    = 1;
    alarm_cfg.time.hour = alarmTime.hour;
    alarm_cfg.time.min  = alarmTime.min;
    alarm_cfg.time.sec  = alarmTime.sec;

    wise_rtc_register_callback(RTC_EVENT_ALARM, _rtc_alarm_callback, NULL);
    wise_rtc_set_alarm(&alarm_cfg);

    return 0;

invalid:
    printf("Invalid parameters:\r\n");
    printf("Usage: alarm [hour] [min] [sec]\r\n");
    return -1;
}

/** Register shell command "alarm". */
SHELL_CMD_AUTO(alarm, cmd_alarm, "Configure an alarm");

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the RTC example application.
 *
 * Initializes demo environment and shell, then initializes RTC:
 * - ::wise_rtc_init()
 * - ::wise_rtc_set_time() with a predefined start time
 * - ::wise_rtc_start()
 *
 * The main loop periodically refreshes watchdog via ::wise_wdt_set_refresh()
 * and processes background tasks via ::wise_main_proc().
 */
void main(void)
{
    WISE_RTC_CNT_T rtcStart =
    {
        .day = 10,
        .time.hour = 10,
        .time.min = 50,
        .time.sec = 0,
    };

    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    wise_rtc_init();
    wise_rtc_set_time(&rtcStart);
    wise_rtc_start();

    printf("RTC init time: Day %d %02d:%02d:%02d\r\n",
           rtcStart.day, rtcStart.time.hour, rtcStart.time.min, rtcStart.time.sec);

    while (1)
    {
        wise_wdt_set_refresh();
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_RTC */

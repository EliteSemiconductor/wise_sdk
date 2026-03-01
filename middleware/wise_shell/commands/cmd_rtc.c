/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_RTC
#include "cmd_rtc.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int _rtcInitCmd(int argc, char **argv);
static int _deinitCmd(int argc, char **argv);
static int _getTimeCmd(int argc, char **argv);
static int _setTimeCmd(int argc, char **argv);
static int _setAlarmCmd(int argc, char **argv);
static int _startCmd(int argc, char **argv);
static int _stopCmd(int argc, char **argv);
static int _demoCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"help", "RTC help command", _helpCmd},
    {"init", "Initialize RTC", _rtcInitCmd},
    {"deinit", "Deinitialize RTC", _deinitCmd},
    {"gettime", "Get current RTC time", _getTimeCmd},
    {"settime", "Set RTC time", _setTimeCmd},
    {"setalarm", "Set RTC alarm", _setAlarmCmd},
    {"start", "Start RTC", _startCmd},
    {"stop", "Stop RTC", _stopCmd},
    {"demo", "RTC demo function", _demoCmd},
};

void my_rtc_callback(void *context, uint8_t idx)
{
    printf("RTC alarm %d interrupt triggered! Context: %p\n", idx, context);
}

int RTCCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(
        argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static int _rtcInitCmd(int argc, char **argv)
{
    if (wise_rtc_init() == WISE_SUCCESS) {
        printf("RTC initialized successfully\n");
    } else {
        printf("RTC initialization failed\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _deinitCmd(int argc, char **argv)
{
    if (wise_rtc_deinit() == WISE_SUCCESS) {
        printf("RTC deinitialized successfully\n");
    } else {
        printf("RTC deinitialization failed\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _getTimeCmd(int argc, char **argv)
{
    WISE_RTC_CNT_T cur_time;
    if (wise_rtc_get_time(&cur_time) == WISE_SUCCESS) {
        printf("Current time: Day %d, %02d:%02d:%02d\n", cur_time.day,
               cur_time.time.hour, cur_time.time.min, cur_time.time.sec);
    } else {
        printf("Failed to get RTC time\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _setTimeCmd(int argc, char **argv)
{
    if (argc != 6) {
        printf("Usage: settime [day] [hour] [minute] [second]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_RTC_CNT_T cur_time = {
        .day       = (uint8_t)strtoul(argv[2], NULL, 10),
        .time.hour = (uint8_t)strtoul(argv[3], NULL, 10),
        .time.min  = (uint8_t)strtoul(argv[4], NULL, 10),
        .time.sec  = (uint8_t)strtoul(argv[5], NULL, 10),
    };

    if (wise_rtc_set_time(&cur_time) == WISE_SUCCESS) {
        printf("RTC time set successfully\n");
    } else {
        printf("Failed to set RTC time\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _setAlarmCmd(int argc, char **argv)
{
    if (argc != 7) {
        printf("Usage: setalarm [channel] [hour] [minute] [second] "
               "[interrupt_enable]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_RTC_ALM_CFG_T alarm_cfg = {
        .channel   = (uint8_t)strtoul(argv[2], NULL, 10),
        .enable    = 1,
        .time.hour = (uint8_t)strtoul(argv[3], NULL, 10),
        .time.min  = (uint8_t)strtoul(argv[4], NULL, 10),
        .time.sec  = (uint8_t)strtoul(argv[5], NULL, 10),
        .int_en    = (uint8_t)strtoul(argv[6], NULL, 10),
    };

    if (wise_rtc_set_alarm(&alarm_cfg) == WISE_SUCCESS) {
        printf("RTC alarm set successfully\n");
    } else {
        printf("Failed to set RTC alarm\n");
    }
    if (wise_rtc_register_callback(RTC_EVENT_ALARM, my_rtc_callback, NULL) ==
        WISE_SUCCESS) {
        printf("RTC callback registered\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _startCmd(int argc, char **argv)
{
    if (wise_rtc_start() == WISE_SUCCESS) {
        printf("RTC started successfully\n");
    } else {
        printf("Failed to start RTC\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _stopCmd(int argc, char **argv)
{
    if (wise_rtc_stop() == WISE_SUCCESS) {
        printf("RTC stopped successfully\n");
    } else {
        printf("Failed to stop RTC\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _demoCmd(int argc, char **argv)
{
    printf("Running RTC demo...\n");

    // Initialize RTC
    if (wise_rtc_init() == WISE_SUCCESS) {
        printf("RTC initialized\n");
    } else {
        printf("RTC initialization failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    // Set time
    WISE_RTC_CNT_T time = {
        .day       = 1,
        .time.hour = 12,
        .time.min  = 0,
        .time.sec  = 0,
    };

    if (wise_rtc_set_time(&time) == WISE_SUCCESS) {
        printf("RTC time set to Day %d, %02d:%02d:%02d\n", time.day,
               time.time.hour, time.time.min, time.time.sec);
    } else {
        printf("Failed to set RTC time\n");
    }

    // Set alarm
    WISE_RTC_ALM_CFG_T alarm = {
        .channel   = 0,
        .enable    = 1,
        .time.hour = 12,
        .time.min  = 1,
        .time.sec  = 0,
        .int_en    = 1,
    };

    if (wise_rtc_set_alarm(&alarm) == WISE_SUCCESS) {
        printf("RTC alarm %d set for Day %d, %02d:%02d:%02d\n", alarm.channel,
               time.day, alarm.time.hour, alarm.time.min, alarm.time.sec);
    } else {
        printf("Failed to set RTC alarm\n");
    }

    if (wise_rtc_register_callback(RTC_EVENT_ALARM, my_rtc_callback, NULL) ==
        WISE_SUCCESS) {
        printf("RTC callback registered\n");
    }
    printf("type 'rtc strat' to trigger rtc timer start\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}
#endif /* SHELL_RTC */

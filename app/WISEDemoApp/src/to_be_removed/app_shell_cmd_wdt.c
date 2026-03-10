/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include "app_shell_cmd_wdt.h"

static int _initCmd(int argc, char **argv);
static int _deinitCmd(int argc, char **argv);
static int _configCmd(int argc, char **argv);
static int _startCmd(int argc, char **argv);
static int _stopCmd(int argc, char **argv);
static int _setTimeoutCmd(int argc, char **argv);
static int _refreshCmd(int argc, char **argv);
static int _periodCmd(int argc, char **argv);
static int _statusCmd(int argc, char **argv);
static int _demoCmd(int argc, char **argv);
static int _cdmsCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"help", "WDT help command", _helpCmd},
    {"init", "Initialize WDT", _initCmd},
    {"deinit", "Deinitialize WDT", _deinitCmd},
    {"config", "Config WDT [reset_en] [ms]", _configCmd},
    {"start", "Start WDT", _startCmd},
    {"stop", "Stop WDT", _stopCmd},
    {"settimeout", "Set WDT timeout [ms]", _setTimeoutCmd},
    {"refresh", "Refresh (kick) WDT", _refreshCmd},
    {"period", "Get WDT period", _periodCmd},
    {"cdms", "Get WDT countdown ms", _cdmsCmd},
    {"status", "Get WDT status", _statusCmd},
    {"demo", "WDT demo function", _demoCmd},
};

int WDTCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(
        argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

void my_wdt_callback(void *context, uint8_t idx)
{
    printf("WDT event triggered!\n");
    wise_wdt_set_refresh();
    printf("WDT refresh\n");
}

static int _initCmd(int argc, char **argv)
{
    if (wise_wdt_init() == WISE_SUCCESS) {
        printf("WDT initialized successfully.\n");
    } else {
        printf("Failed to initialize WDT.\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _deinitCmd(int argc, char **argv)
{
    if (wise_wdt_deinit() == WISE_SUCCESS) {
        printf("WDT deinitialized successfully.\n");
    } else {
        printf("Failed to deinitialize WDT.\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _configCmd(int argc, char **argv)
{
    if (argc < 4) {
        printf("Usage: wdt config [reset_en] [timeout_ms]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint32_t reset_en   = (uint32_t)strtoul(argv[2], NULL, 10);
    uint32_t timeout_ms = (uint32_t)strtoul(argv[3], NULL, 10);

    wise_wdt_config(reset_en, timeout_ms);
    printf("WDT configured: reset_en=%u, timeout_ms=%u.\n", reset_en,
           timeout_ms);

    // e.g., wise_wdt_register_callback(WDT_EVENT_TIMEOUT, my_wdt_callback, NULL);

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _startCmd(int argc, char **argv)
{
    wise_wdt_start();
    printf("WDT started.\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _stopCmd(int argc, char **argv)
{
    wise_wdt_stop();
    printf("WDT stopped.\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _setTimeoutCmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: wdt settimeout [timeout_ms]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint32_t timeout_ms = (uint32_t)strtoul(argv[2], NULL, 10);
    wise_wdt_set_timeout(timeout_ms);
    printf("WDT timeout set to %u ms.\n", timeout_ms);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _refreshCmd(int argc, char **argv)
{
    wise_wdt_set_refresh();
    printf("WDT refreshed (kicked).\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _periodCmd(int argc, char **argv)
{
    uint32_t period = wise_wdt_get_period();
    printf("WDT period: %u \n", period);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _cdmsCmd(int argc, char **argv)
{
    uint32_t period  = wise_wdt_get_period();
    uint64_t tmp     = (uint64_t)period * 1000ULL;
    tmp             /= SystemCoreClock;
    if (tmp > 0xFFFFFFFFUL) {
        tmp = 0xFFFFFFFFUL;
    }
    printf("WDT countdown ms: %u\n", (uint32_t)tmp);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _statusCmd(int argc, char **argv)
{
    uint32_t status = wise_wdt_get_status();
    printf("WDT status: 0x%08X\n", status);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _demoCmd(int argc, char **argv)
{
    printf("Running WDT demo...\n");

    if (wise_wdt_init() == WISE_SUCCESS) {
        printf("WDT initialized.\n");
    } else {
        printf("WDT initialization failed.\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint32_t demo_timeout_ms = 10000;
    wise_wdt_set_timeout(demo_timeout_ms);
    printf("WDT timeout set to %u ms.\n", demo_timeout_ms);

    wise_wdt_config(1, demo_timeout_ms);

    if (wise_wdt_register_callback(WDT_EVENT_TIMEOUT, my_wdt_callback, NULL) ==
        WISE_SUCCESS) {
        printf("WDT callback registered.\n");
    }

    wise_wdt_start();
    printf("WDT started. Please observe system behavior or type 'wdt refresh' "
           "to kick.\n");
    printf("Type 'wdt status' or 'wdt period' to see info.\n");
    printf("Type 'wdt refresh' to prevent reset.\n");
    printf("Type 'wdt stop' to stop WDT.\n");

    return SHLCMD_HIST | SHLCMD_DONE;
}

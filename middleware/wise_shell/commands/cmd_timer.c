/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_TIMER
#include "api/wise_gptmr_api.h"
#include "cmd_timer.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int _stateCmd(int argc, char **argv);
static int _timerInitCmd(int argc, char **argv);
static int _deinitCmd(int argc, char **argv);
static int _configCmd(int argc, char **argv);
static int _startCmd(int argc, char **argv);
static int _stopCmd(int argc, char **argv);
static int _intCmd(int argc, char **argv);
static int _counterCmd(int argc, char **argv);
static int _demoCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"help", "timer function", _helpCmd},

    {"init", "timer function", _timerInitCmd},
    {"deinit", "timer function", _deinitCmd},
    {"state", "timer function", _stateCmd},
    {"config", "timer config", _configCmd},
    {"start", "timer start", _startCmd},
    {"stop", "timer stop", _stopCmd},
    {"int", "timer interrupt enable/disable", _intCmd},
    {"counter", "read counter of timer", _counterCmd},
    {"demo", "timer demo function", _demoCmd},
};

int TimerCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(
        argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static int _timerInitCmd(int argc, char **argv)
{
    if (argc != 2) {
        printf("timer init\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    //uint8_t channel = strtoul(argv[2], NULL, 10);
    if (wise_timer_init() == WISE_SUCCESS) {
        printf("timer inited\n");
    } else {
        printf("timer init failed\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _deinitCmd(int argc, char **argv)
{
    if (argc != 2) {
        printf("timer deinit\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    
    if (wise_timer_deinit() == WISE_SUCCESS) {
        printf("timer deinited\n");
    } else {
        printf("timer deinit failed\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _stateCmd(int argc, char **argv)
{
    printf("timer state:\n");
    for (uint8_t i = 0; i < CHIP_TIMER_CHANNEL_NUM; i++) {
        printf("timer %d %s \n", i,
               wise_timer_is_enabled(i) ? "enabled" : "disabled");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _configCmd(int argc, char **argv)
{
    if (argc != 7) {
        printf("timer config [ch_dec] [type_hex] [interval_hex] [start_offset] "
               "[interrupt_enable]\n");
        printf("    ch: timer channel select 0-5\n");
        printf("    type: 0=periodic 1=oneshot\n");
        printf("    interval: timer interval in us\n");
        printf("    start_offset: timer start_offset in us\n");
        printf("    interrupt_enable: 0=disable 1=enable\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_TIMER_CTRL_T config = {0};
    uint8_t timer_channel    = strtoul(argv[2], NULL, 10);
    config.type              = strtoul(argv[3], NULL, 10);
    config.interval          = strtoul(argv[4], NULL, 10);
    config.start_offset      = strtoul(argv[5], NULL, 10);
    config.interrupt_enable  = strtoul(argv[6], NULL, 10);

    if (wise_timer_config(timer_channel, &config) == WISE_SUCCESS) {
        printf("timer %d configured\n", timer_channel);
    } else {
        printf("timer config failed for channel %d\n", timer_channel);
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}
void my_timer_callback(void *context, uint8_t tmr_idx)
{
    printf("Timer %d interrupt occurred! context=%p\n", tmr_idx, context);
}
static int _startCmd(int argc, char **argv)
{
    if (argc != 3) {
        printf("timer start [ch_dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t timer_channel = strtoul(argv[2], NULL, 10);

    if (wise_timer_register_callback(timer_channel, my_timer_callback, NULL) == WISE_SUCCESS) 
    {
        printf("timer %d callback registered\n", timer_channel);
    } 
    else 
    {
        printf("timer callback register failed for channel %d\n",
               timer_channel);
    }
    
    if (wise_timer_start(timer_channel) == WISE_SUCCESS) {
        printf("timer %d started\n", timer_channel);
        printf("type '<C-g>' to abort \n");
    } else {
        printf("timer start failed for channel %d\n", timer_channel);
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _stopCmd(int argc, char **argv)
{
    if (argc != 3) {
        printf("timer stop [ch_dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t timer_channel = strtoul(argv[2], NULL, 10);

    if (wise_timer_stop(timer_channel) == WISE_SUCCESS) {
        printf("timer %d stopped\n", timer_channel);
    } else {
        printf("timer stop failed for channel %d\n", timer_channel);
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _intCmd(int argc, char **argv)
{
    uint8_t intEnable = 0;
    uint8_t timer_channel = 0;
    
    if (argc != 4) {
        printf("timer int [ch_dec] [0/1]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    timer_channel = strtoul(argv[2], NULL, 10);
    intEnable = strtoul(argv[3], NULL, 10);

    if(intEnable)
    {
        wise_timer_enable_int(timer_channel);
        printf("enable interupt on channel %d\n", timer_channel);
    }
    else
    {
        wise_timer_disable_int(timer_channel);
        printf("disable interrup on channel %d\n", timer_channel);
    }
    
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _counterCmd(int argc, char **argv)
{
    uint32_t timer_channel;
    uint32_t counter = 0;
    
    if (argc != 3) {
        printf("timer counter [ch_dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    timer_channel = strtoul(argv[2], NULL, 10);
    counter = wise_timer_get_counter(timer_channel);

    printf("Timer %ld counter=%08lx\n", timer_channel, counter);

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _demoCmd(int argc, char **argv)
{
    if (argc != 7) {
        printf("timer demo [ch_dec] [type_hex] [interval_hex] [start_offset] "
               "[interrupt_enable]\n");
        printf("    ch: timer channel select 0-5\n");
        printf("    type: 0=periodic 1=oneshot\n");
        printf("    interval: timer interval in us\n");
        printf("    start_offset: timer start_offset in us\n");
        printf("    interrupt_enable: 0=disable 1=enable\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_TIMER_CTRL_T config = {0};
    uint8_t timer_channel    = strtoul(argv[2], NULL, 10);
    config.type              = strtoul(argv[3], NULL, 10);
    config.interval          = strtoul(argv[4], NULL, 10);
    config.start_offset      = strtoul(argv[5], NULL, 10);
    config.interrupt_enable  = strtoul(argv[6], NULL, 10);

    if (wise_timer_init() == WISE_SUCCESS) {
        printf("timer inited\n");
    } else {
        printf("timer init failed for channel %d\n", timer_channel);
    }

    if (wise_timer_config(timer_channel, &config) == WISE_SUCCESS) {
        printf("timer %d configured\n", timer_channel);
    } else {
        printf("timer config failed for channel %d\n", timer_channel);
    }

    if (wise_timer_register_callback(timer_channel,
                                     my_timer_callback,
                                     NULL) == WISE_SUCCESS) {
        printf("timer %d callback registered\n", timer_channel);
    } else {
        printf("timer callback register failed for channel %d\n",
               timer_channel);
    }

    if (wise_timer_start(timer_channel) == WISE_SUCCESS) {
        printf("timer %d started\n", timer_channel);
        printf("type '<C-g>' to abort \n");
    } else {
        printf("timer start failed for channel %d\n", timer_channel);
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}
#endif /* SHELL_TIMER */

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_PWMSLOW
#include "cmd_pwmslow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _pwmslowInitCmd(int argc, char **argv);
static int _deinitCmd(int argc, char **argv);
static int _configPeriodCmd(int argc, char **argv);
static int _configFrequencyCmd(int argc, char **argv);
static int _startCmd(int argc, char **argv);
static int _stopCmd(int argc, char **argv);
static int _demoCmd(int argc, char **argv);
static void my_pwmslow_callback(void *context, uint8_t pwm_idx);

static const struct shellCommand m_CmdList[] = {
    {"init", "Initialize PWMSLOW", _pwmslowInitCmd},
    {"deinit", "Deinitialize PWMSLOW", _deinitCmd},
    {"cfgp", "Configure PWMSLOW with period control", _configPeriodCmd},
    {"cfgf", "Configure PWMSLOW with frequency control", _configFrequencyCmd},
    {"start", "Start PWMSLOW", _startCmd},
    {"stop", "Stop PWMSLOW", _stopCmd},
    {"demo", "PWMSLOW demo function", _demoCmd},
};

int PWMSLOWCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static void my_pwmslow_callback(void *context, uint8_t pwm_idx)
{
    printf("PWMSLOW interrupt triggered! Context: %p\n", context);
}

static int _pwmslowInitCmd(int argc, char **argv)
{
    if (wise_pwmslow_init() == WISE_SUCCESS) {
        printf("PWMSLOW initialized successfully\n");
    } else {
        printf("PWMSLOW initialization failed\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _deinitCmd(int argc, char **argv)
{
    if (wise_pwmslow_deinit() == WISE_SUCCESS) {
        printf("PWMSLOW deinitialized successfully\n");
    } else {
        printf("PWMSLOW deinitialization failed\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _configPeriodCmd(int argc, char **argv)
{
    if (argc < 8) {
        printf("Usage: pwmslow config_period <gpio_index> <period> "
               "<active_period> <center_align_en> <oneshot_en> <oneshot_num>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t gpio_index = atoi(argv[2]);
    WISE_PWMSLOW_CONF_T pwmslow_conf;
    memset(&pwmslow_conf, 0, sizeof(pwmslow_conf));

    pwmslow_conf.common.mode            = PWMSLOW_MODE_PERIOD;
    pwmslow_conf.u.period.period        = atoi(argv[3]);
    pwmslow_conf.u.period.active_period = atoi(argv[4]);
    pwmslow_conf.common.center_align_en = atoi(argv[5]);
    pwmslow_conf.common.oneshot_en      = atoi(argv[6]);
    pwmslow_conf.common.oneshot_num     = atoi(argv[7]);

    if (wise_pwmslow_configure(gpio_index, &pwmslow_conf) == WISE_SUCCESS) {
        printf("PWMSLOW configured successfully with period control.\n");
        if (pwmslow_conf.common.oneshot_en == ENABLE) {
            wise_pwmslow_register_callback(0, my_pwmslow_callback, NULL);
            printf("PWMSLOW interrupt enabled and callback registered.\n");
        }
    } else {
        printf("PWMSLOW configuration failed.\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _configFrequencyCmd(int argc, char **argv)
{
    if (argc < 9) {
        printf("Usage: pwmslow config_frequency <gpio_index> <frequency_Hz> "
               "<duty_percent> <idle_status> <low_active_en> <oneshot_en> "
               "<oneshot_num>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t gpio_index = atoi(argv[2]);
    WISE_PWMSLOW_CONF_T pwmslow_conf;
    memset(&pwmslow_conf, 0, sizeof(pwmslow_conf));

    pwmslow_conf.common.mode          = PWMSLOW_MODE_FREQUENCY;
    pwmslow_conf.u.freq.frequency_Hz  = atoi(argv[3]);
    pwmslow_conf.u.freq.duty_percent  = atoi(argv[4]);
    pwmslow_conf.common.idle_status   = atoi(argv[5]);
    pwmslow_conf.common.low_active_en = atoi(argv[6]);
    pwmslow_conf.common.oneshot_en    = atoi(argv[7]);
    pwmslow_conf.common.oneshot_num   = atoi(argv[8]);

    if (wise_pwmslow_configure(gpio_index, &pwmslow_conf) == WISE_SUCCESS) {
        printf("PWMSLOW configured successfully with frequency control.\n");
        if (pwmslow_conf.common.oneshot_en == ENABLE) {
            wise_pwmslow_register_callback(PWMSLOW_EVENT_TRIGGER, my_pwmslow_callback, NULL);
            printf("PWMSLOW interrupt enabled and callback registered.\n");
        }
    } else {
        printf("PWMSLOW configuration failed.\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _startCmd(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: pwmslow start \n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    wise_pwmslow_start();
    printf("PWMSLOW started \n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _stopCmd(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: pwmslow stop \n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    wise_pwmslow_stop();
    printf("PWMSLOW stopped \n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _demoCmd(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: pwmslow demo <gpio_index>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    printf("Running PWMSLOW demo...\n");
    uint8_t gpio_pin = atoi(argv[2]);
    if (wise_pwmslow_init() == WISE_SUCCESS) {
        printf("PWMSLOW initialized\n");
    } else {
        printf("PWMSLOW initialization failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_PWMSLOW_CONF_T pwmslow_conf = {
        .common.mode            = PWMSLOW_MODE_FREQUENCY,
        .u.freq.frequency_Hz    = 10,
        .u.freq.duty_percent    = 25,
        .common.center_align_en = DISABLE,
        .common.oneshot_en      = ENABLE,
        .common.oneshot_num     = 10,
    };

    if (wise_pwmslow_configure(gpio_pin, &pwmslow_conf) == WISE_SUCCESS) {
        printf("PWMSLOW configured with frequency %ldHz frequency duty percent "
               "%ld and interrupt enabled\n",
               pwmslow_conf.u.freq.frequency_Hz, pwmslow_conf.u.freq.duty_percent);
        wise_pwmslow_register_callback(PWMSLOW_EVENT_TRIGGER, my_pwmslow_callback, NULL);
    } else {
        printf("Failed to configure PWMSLOW channel\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    printf("PWMSLOW started on gpio%d \n", gpio_pin);
    wise_pwmslow_start();

    return SHLCMD_HIST | SHLCMD_DONE;
}
#endif /* SHELL_PWMSLOW */

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app_shell_cmd_pwm.h"

static int _initCmd(int argc, char **argv);
static int _deinitCmd(int argc, char **argv);
static int _configPeriodCmd(int argc, char **argv);
static int _configFrequencyCmd(int argc, char **argv);
static int _startCmd(int argc, char **argv);
static int _stopCmd(int argc, char **argv);
static int _demoCmd(int argc, char **argv);
static void my_pwm_callback(void *context, uint8_t pwm_idx);

static const struct shellCommand m_CmdList[] = {
    {"init", "Initialize PWM", _initCmd},
    {"deinit", "Deinitialize PWM", _deinitCmd},
    {"cfgp", "Configure PWM with period control", _configPeriodCmd},
    {"cfgf", "Configure PWM with frequency control", _configFrequencyCmd},
    {"start", "Start PWM", _startCmd},
    {"stop", "Stop PWM", _stopCmd},
    {"demo", "PWM demo function", _demoCmd},
};

int PWMCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(
        argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static void my_pwm_callback(void *context, uint8_t pwm_idx)
{
    printf("PWM interrupt triggered! Context: %p\n", context);
}

static int _initCmd(int argc, char **argv)
{
    if (wise_pwm_init() == WISE_SUCCESS) {
        printf("PWM initialized successfully\n");
    } else {
        printf("PWM initialization failed\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
    ;
}

static int _deinitCmd(int argc, char **argv)
{
    if (wise_pwm_deinit() == WISE_SUCCESS) {
        printf("PWM deinitialized successfully\n");
    } else {
        printf("PWM deinitialization failed\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _configPeriodCmd(int argc, char **argv)
{
    if (argc < 9) {
        printf(
            "Usage: pwm config_period <channel> <gpio_index> <period> "
            "<active_period> <center_align_en> <oneshot_en> <oneshot_num>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t channel    = atoi(argv[2]);
    uint8_t gpio_index = atoi(argv[3]);
    WISE_PWM_CONF_T pwm_conf;
    memset(&pwm_conf, 0, sizeof(pwm_conf));

    pwm_conf.mode            = PWM_MODE_PERIOD;
    pwm_conf.period          = atoi(argv[4]);
    pwm_conf.active_period   = atoi(argv[5]);
    pwm_conf.center_align_en = atoi(argv[6]);
    pwm_conf.oneshot_en      = atoi(argv[7]);
    pwm_conf.oneshot_num     = atoi(argv[8]);

    if (wise_pwm_configure(channel, gpio_index, &pwm_conf) == WISE_SUCCESS) {
        printf("PWM configured successfully with period control.\n");
        if (pwm_conf.oneshot_en == ENABLE) {
            wise_pwm_register_callback(channel, my_pwm_callback, NULL);
            printf("PWM interrupt enabled and callback registered.\n");
        }
    } else {
        printf("PWM configuration failed.\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _configFrequencyCmd(int argc, char **argv)
{
    if (argc < 10) {
        printf("Usage: pwm config_frequency <channel> <gpio_index> "
               "<frequency_Hz> <duty_percent> <idle_status> <low_active_en> "
               "<oneshot_en> <oneshot_num>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t channel    = atoi(argv[2]);
    uint8_t gpio_index = atoi(argv[3]);
    WISE_PWM_CONF_T pwm_conf;
    memset(&pwm_conf, 0, sizeof(pwm_conf));

    pwm_conf.mode          = PWM_MODE_FREQUENCY;
    pwm_conf.frequency_Hz  = atoi(argv[4]);
    pwm_conf.duty_percent  = atoi(argv[5]);
    pwm_conf.idle_status   = atoi(argv[6]);
    pwm_conf.low_active_en = atoi(argv[7]);
    pwm_conf.oneshot_en    = atoi(argv[8]);
    pwm_conf.oneshot_num   = atoi(argv[9]);

    if (wise_pwm_configure(channel, gpio_index, &pwm_conf) == WISE_SUCCESS) {
        printf("PWM configured successfully with frequency control.\n");
        if (pwm_conf.oneshot_en == ENABLE) {
            wise_pwm_register_callback(channel, my_pwm_callback, NULL);
            printf("PWM interrupt enabled and callback registered.\n");
        }
    } else {
        printf("PWM configuration failed.\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _startCmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: pwm start <channel_mask>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    uint32_t channel_mask = strtoul(argv[2], NULL, 0);
    wise_pwm_start(channel_mask);
    printf("PWM started on channel mask: 0x%X\n", channel_mask);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _stopCmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: pwm stop <channel_mask>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    uint32_t channel_mask = strtoul(argv[2], NULL, 0);
    wise_pwm_stop(channel_mask);
    printf("PWM stopped on channel mask: 0x%X\n", channel_mask);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _demoCmd(int argc, char **argv)
{
    printf("Running PWM demo...\n");
    uint8_t pwm_channel = 0;
    uint8_t gpio_pin    = 2;
    if (wise_pwm_init() == WISE_SUCCESS) {
        printf("PWM initialized\n");
    } else {
        printf("PWM initialization failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_PWM_CONF_T pwm_conf = {
        .mode            = PWM_MODE_FREQUENCY,
        .frequency_Hz    = 100,
        .duty_percent    = 75,
        .center_align_en = DISABLE,
        .oneshot_en      = ENABLE,
        .oneshot_num     = 10,
    };

    if (wise_pwm_configure(pwm_channel, gpio_pin, &pwm_conf) == WISE_SUCCESS) {
        printf("PWM channel %d configured with frequency %dHz duty percent "
               "%d%% and interrupt enabled\n",
               pwm_channel, pwm_conf.frequency_Hz, pwm_conf.duty_percent);
        wise_pwm_register_callback(0, my_pwm_callback, NULL);
    } else {
        printf("Failed to configure PWM channel\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    printf("PWM started channel %d on gpio%d \n", pwm_channel, gpio_pin);
    wise_pwm_start(1);

    return SHLCMD_HIST | SHLCMD_DONE;
}

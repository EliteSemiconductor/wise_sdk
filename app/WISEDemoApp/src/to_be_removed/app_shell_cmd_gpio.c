/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app_shell_cmd_gpio.h"
#include "api/wise_tick_api.h"
static int _initCmd(int argc, char **argv);
static int _deinitCmd(int argc, char **argv);
static int _configCmd(int argc, char **argv);
static int _readCmd(int argc, char **argv);
static int _writeCmd(int argc, char **argv);
static int _toggleCmd(int argc, char **argv);
static int _demoCmd(int argc, char **argv);
static int _dirCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"init", "Initialize GPIO", _initCmd},
    {"deinit", "Deinitialize GPIO", _deinitCmd},
    {"dir", "GPIO Direction status", _dirCmd},
    {"config", "Configure GPIO", _configCmd},
    {"read", "Read GPIO status", _readCmd},
    {"write", "Write GPIO status", _writeCmd},
    {"toggle", "Toggle GPIO state", _toggleCmd},
    {"demo", "GPIO demo function", _demoCmd},
};

int GPIOCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(
        argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static int _initCmd(int argc, char **argv)
{
    wise_gpio_init();
    printf("GPIO initialized successfully\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _deinitCmd(int argc, char **argv)
{
    wise_gpio_deinit();
    printf("GPIO deinitialized successfully\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _dirCmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: gpio dir <pin_idx>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t pin_idx          = atoi(argv[2]);
    GPIO_DIRECTION direction = wise_gpio_get_direction(pin_idx);
    printf("GPIO %d direction: %s\n", pin_idx,
           direction == GPIO_DIR_OUTPUT ? "OUTPUT" : "INPUT");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _configCmd(int argc, char **argv)
{
    if (argc < 7) {
        printf("Usage: gpio config <pin_idx> <mode> <direction> <int_en> "
               "<int_type>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_GPIO_CFG_T gpio_cfg;
    memset(&gpio_cfg, 0, sizeof(gpio_cfg));

    gpio_cfg.pin_idx   = atoi(argv[2]);
    gpio_cfg.mode      = atoi(argv[3]);
    gpio_cfg.direction = atoi(argv[4]);
    gpio_cfg.int_en    = atoi(argv[5]);
    gpio_cfg.int_type  = atoi(argv[6]);

    wise_gpio_cfg(&gpio_cfg);
    printf("GPIO configured successfully\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _readCmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: gpio read <pin_idx>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t pin_idx    = atoi(argv[2]);
    GPIO_STATUS status = wise_gpio_read(pin_idx);
    printf("GPIO %d status: %s\n", pin_idx,
           status == GPIO_HIGH ? "HIGH" : "LOW");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _writeCmd(int argc, char **argv)
{
    if (argc < 4) {
        printf("Usage: gpio write <pin_idx> <status>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t pin_idx    = atoi(argv[2]);
    GPIO_STATUS status = atoi(argv[3]);
    wise_gpio_write(pin_idx, status);
    printf("GPIO %d set to %s\n", pin_idx,
           status == GPIO_HIGH ? "HIGH" : "LOW");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _toggleCmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: gpio toggle <pin_idx>\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t pin_idx = atoi(argv[2]);
    wise_gpio_toggle(pin_idx);
    printf("GPIO %d toggled\n", pin_idx);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _demoCmd(int argc, char **argv)
{
    printf("Running GPIO demo...\n");

    uint8_t pin = 5;
    printf("Configuring GPIO %d as output...\n", pin);
    WISE_GPIO_CFG_T gpio_cfg = {.pin_idx   = pin,
                                .mode      = MODE_GPIO,
                                .direction = GPIO_DIR_OUTPUT,
                                .int_en    = 0,
                                .int_type  = 0};
    wise_gpio_init();
    wise_gpio_cfg(&gpio_cfg);

    printf("Toggling GPIO %d 5 times\n", pin);
    for (int i = 0; i < 5; i++) {
        wise_gpio_toggle(pin);
        wise_tick_delay_ms(1000);
    }

    printf("GPIO demo finished.\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

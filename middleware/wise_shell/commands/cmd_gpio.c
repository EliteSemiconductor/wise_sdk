/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_GPIO
#include "api/wise_tick_api.h"
#include "cmd_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int _gpioInitCmd(int argc, char **argv);
static int _deinitCmd(int argc, char **argv);
static int _configCmd(int argc, char **argv);
static int _readCmd(int argc, char **argv);
static int _writeCmd(int argc, char **argv);
static int _toggleCmd(int argc, char **argv);
static int _demoCmd(int argc, char **argv);
static int _dirCmd(int argc, char **argv);
static int _debugCmd(int argc, char **argv);
static int _testCmd(int argc, char **argv);



static const struct shellCommand m_CmdList[] = {
    {"init", "Initialize GPIO", _gpioInitCmd},
    {"deinit", "Deinitialize GPIO", _deinitCmd},
    {"dir", "GPIO Direction status", _dirCmd},
    {"config", "Configure GPIO", _configCmd},
    {"read", "Read GPIO status", _readCmd},
    {"write", "Write GPIO status", _writeCmd},
    {"toggle", "Toggle GPIO state", _toggleCmd},
    {"demo", "GPIO demo function", _demoCmd},
    {"dbg", "GPIO debug bus function", _debugCmd},
    {"test", "GPIO output test", _testCmd},
};

int GPIOCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(
        argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static int _gpioInitCmd(int argc, char **argv)
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

static int _debugCmd(int argc, char **argv)
{
    uint8_t gpio_pin_idx = 0;
    uint8_t dbg_bus_module_idx = 0;
    uint8_t dbg_bus_signal_idx = 0;
    uint8_t dbg_id = 0;

    if (argc < 4) {
        printf("Usage: gpio dbg [pin index] [debug bus module idx]\n");
        goto error;
    }

    gpio_pin_idx = strtoul(argv[2], NULL, 10);
    if (gpio_pin_idx > 15) {
        printf("pin index is between 0 to 15\n");
        goto error;
    }    
        
    dbg_bus_module_idx = strtoul(argv[3], NULL, 10);
    if (dbg_bus_module_idx == 17 && argc < 6) {
        printf("Usage: gpio dbg [%d] [%d] [dbg singnal sel(0-6)] [debug bus index(0-15)]\n", gpio_pin_idx, dbg_bus_module_idx);
        goto error;
    }
    
    if (dbg_bus_module_idx == 19 && argc < 6) {
        printf("Usage: gpio dbg [%d] [%d] [pmu debug sel(0-6)] [debug bus index(0-15)]\n", gpio_pin_idx, dbg_bus_module_idx);
        goto error;
    }

    if ((dbg_bus_module_idx == 17 && argc == 6) || (dbg_bus_module_idx == 19 && argc == 6)) {
        dbg_bus_signal_idx = strtoul(argv[4], NULL, 10);
        dbg_id = strtoul(argv[5], NULL, 10);
    }

    GPIO_DBG_CFG_INFO gpio_dbg_cfg = {  .gpio_pin_idx = gpio_pin_idx,
                                        .dbg_bus_module_idx = dbg_bus_module_idx,
                                        .dbg_bus_signal_idx = dbg_bus_signal_idx,
                                        .dbg_id = dbg_id};

    WISE_GPIO_CFG_T gpio_cfg = {.pin_idx   = gpio_pin_idx,
                                .mode      = MODE_DEBUG,
                                .direction = GPIO_DIR_OUTPUT,
                                .int_en    = 0,
                                .int_type  = 0};
    wise_gpio_init();
    wise_gpio_cfg(&gpio_cfg);

    wise_gpio_set_debug_bus(&gpio_dbg_cfg);
error:
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _testCmd(int argc, char **argv)
{
    uint8_t pin_idx;

    pin_idx = strtoul(argv[2], NULL, 10);

    printf("test: gpio %d set out\n", pin_idx);

    WISE_GPIO_CFG_T gpio_cfg = {.pin_idx   = pin_idx,
                                .mode      = MODE_GPIO,
                                .direction = GPIO_DIR_OUTPUT,
                                .int_en    = 0,
                                .int_type  = 0};

    wise_gpio_init();
    wise_gpio_cfg(&gpio_cfg);

    //for function test
    
    wise_gpio_write(pin_idx,1);
    wise_tick_delay_ms(100);
    wise_gpio_write(pin_idx,0);
    wise_tick_delay_ms(100);
    wise_gpio_write(pin_idx,1);
    wise_tick_delay_ms(100);
    wise_gpio_write(pin_idx,0);
    wise_tick_delay_ms(100);
    wise_gpio_write(pin_idx,1);
    

    return SHLCMD_HIST | SHLCMD_DONE;
}


#endif /* SHELL_GPIO */

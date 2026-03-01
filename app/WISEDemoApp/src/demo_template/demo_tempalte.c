/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
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
#include "wise_sys_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

#define SHELL_UART_CH  0

/* ========================================================================== */
/* Shell Backend                                                              */
/* ========================================================================== */

static bool shell_uart_read_char(char *ch)
{
    uint8_t tmp;
    if (wise_uart_read_char(SHELL_UART_CH, &tmp) == WISE_SUCCESS) {
        *ch = (char)tmp;
        return true;
    }
    return false;
}

static void shell_uart_write_str(const char *s)
{
    while (*s) {
        wise_uart_write_char(SHELL_UART_CH, (uint8_t)*s++);
    }
}

static void app_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = "DEMO> ",
    };

    shell_init(&cfg);
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

void main(void)
{
    demo_app_common_init();
    app_shell_init();

    while (1) {
    	wise_main_proc();
    }
}

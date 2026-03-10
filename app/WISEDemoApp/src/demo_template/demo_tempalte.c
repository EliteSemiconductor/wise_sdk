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

#define DEMO_APP_PROMPT             "DEMO> "


/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

void main(void)
{
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    while (1) {
    	wise_main_proc();
    }
}

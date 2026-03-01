/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include "wise.h"
#include "wise_core.h"
#include "api/wise_uart_api.h"

int main(void)
{
    if (WISE_SUCCESS != wise_init()) {
        while (1)
            ;
    }

    debug_print("Hello Arm World! %s %s\n", __DATE__, __TIME__);

    while (1) {
        uint8_t inputC;

        wise_main_proc();

        if (WISE_SUCCESS == wise_uart_read_char(STDIO_UART_PORT, &inputC)) {
            wise_uart_write_char(STDIO_UART_PORT, inputC);
        }
    }
    return 0;
}

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __LOADER_SHELL_CMDS_H
#define __LOADER_SHELL_CMDS_H

#include <stdint.h>

enum {
    E_LOADER_NORMAL_BOOT,
    E_LOADER_CONSOLE = 1,
};

void loader_shell_cmd_init();
int loader_shell_cmd_check();

#endif

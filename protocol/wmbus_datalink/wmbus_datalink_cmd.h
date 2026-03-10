/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __WMBUS_H_CMD
#define __WMBUS_H_CMD

#include "shell.h"
#include "shell_commands.h"

int32_t wmbus_link_shlcmds_init();

int WMbusCommandHandler(int argc, char **argv);
#endif //__WMBUS_H_CMD

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __CMD_SPI_H
#define __CMD_SPI_H

#include "wise_core.h"
#include "../shell.h"
#include "../shell_commands.h"
#include "api/wise_spi_api.h"
#include "api/wise_xcvr_api.h"
#include "api/wise_sys_api.h"

int SPICommandHandler(int argc, char **argv);
#endif //__CMD_SPI_H

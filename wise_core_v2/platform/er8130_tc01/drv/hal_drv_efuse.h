/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef _EFUSE_ER8130_H_
#define _EFUSE_ER8130_H_

#include "hal_intf_efuse.h"

HAL_STATUS hal_drv_efuse_read(EFUSE_CONF_S* efuse_conf);
HAL_STATUS hal_drv_efuse_write(EFUSE_CONF_S* efuse_conf);
void hal_drv_efuse_get_chip_unique(uint8_t* id_arr);


#endif

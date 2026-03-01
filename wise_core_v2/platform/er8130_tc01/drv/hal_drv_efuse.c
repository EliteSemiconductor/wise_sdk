/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_efuse.h"
#include "hdl/efuse_er8130.h"

HAL_STATUS hal_drv_efuse_read(EFUSE_CONF_S* efuse_conf)
{
    //9005 not support
    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_efuse_write(EFUSE_CONF_S* efuse_conf)
{
    //9005 not support
    return HAL_NO_ERR;
}

void hal_drv_efuse_get_chip_unique(uint8_t* id_arr)
{
    //9005 not support
}


/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_efuse.h"
#include "hdl/efuse_er8130.h"

HAL_STATUS hal_drv_efuse_read(EFUSE_CONF_S* efuse_conf)
{
    return efuse_read_er8130(efuse_conf->base_idx, efuse_conf->addr, efuse_conf->buf);
}

HAL_STATUS hal_drv_efuse_write(EFUSE_CONF_S* efuse_conf)
{
    return efuse_write_er8130(efuse_conf->base_idx, efuse_conf->addr, efuse_conf->buf);
}

void hal_drv_efuse_get_chip_unique(uint8_t* id_arr)
{
    efuse_get_chip_unique_er8130(id_arr);
}

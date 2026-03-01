/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_efuse.h"
#include "esmt_chip_specific.h"
#include "drv/hal_drv_efuse.h"
#include "hal_intf_pmu.h"
#include "hal_drv_radio.h"

void hal_intf_efuse_init(void)
{
    hal_intf_module_clk_enable(EFUSE_MODULE);
}

HAL_STATUS hal_intf_efuse_read(EFUSE_CONF_S *efuse_conf)
{
    return hal_drv_efuse_read(efuse_conf);
}

HAL_STATUS hal_intf_efuse_write(EFUSE_CONF_S *efuse_conf)
{
    HAL_STATUS status;

    hal_drv_radio_enable_efuse_write_data(ENABLE);
    status = hal_drv_efuse_write(efuse_conf);
    hal_drv_radio_enable_efuse_write_data(DISABLE);

    return status;
}

void hal_intf_efuse_get_chip_unique(uint8_t* id_arr)
{
    hal_drv_efuse_get_chip_unique(id_arr);
}

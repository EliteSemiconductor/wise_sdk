/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_trng.h"
#include "drv/hal_drv_trng.h"

void hal_intf_trng_sw_reset(void)
{
    hal_drv_trng_sw_reset();
}

uint8_t hal_intf_trng_is_busy(void)
{
    return hal_drv_trng_is_busy();
}

uint32_t hal_intf_trng_get_status(void)
{
    return hal_drv_trng_get_status();
}

void hal_intf_trng_clear_status(uint32_t status)
{
    hal_drv_trng_clear_status(status);
}

void hal_intf_trng_config(void)
{
    hal_drv_trng_config();
}

void hal_intf_trng_start(void)
{
    hal_drv_trng_start();
}

uint8_t hal_intf_trng_get_ehr_data(uint32_t *ehr_buf)
{
    return hal_drv_trng_get_ehr_data(ehr_buf);
}

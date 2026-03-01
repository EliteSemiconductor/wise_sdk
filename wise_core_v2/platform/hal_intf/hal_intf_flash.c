/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_flash.h"
#include "drv/hal_drv_flash.h"

void hal_intf_flash_init(void)
{
    hal_drv_flash_init();
}

void hal_intf_flash_deinit(void)
{
    hal_drv_flash_deinit();
}

HAL_STATUS hal_intf_flash_read(uint32_t addr, uint8_t *buf, uint32_t len)
{
    hal_drv_flash_read(addr, buf, len);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_flash_write(uint32_t addr, uint8_t *buf, uint32_t len)
{
    hal_drv_flash_write(addr, buf, len);
    return HAL_NO_ERR;
}

void hal_intf_flash_get_uid(uint8_t *uid)
{
    hal_drv_flash_get_uid(uid);
}

void hal_intf_flash_get_dev_id(uint32_t* id)
{
    hal_drv_flash_get_dev_id(id);    
}

HAL_STATUS hal_intf_flash_sector_erase(uint32_t addr)
{
    hal_drv_flash_sector_erase(addr);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_flash_block_erase(uint32_t addr)
{
    hal_drv_flash_block_erase(addr);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_otp_read(uint32_t addr, uint8_t *buf, uint32_t len)
{
	hal_drv_otp_read(addr, buf, len);
	return HAL_NO_ERR;
}

HAL_STATUS hal_intf_otp_write(uint32_t addr, uint8_t *buf, uint32_t len)
{
	hal_drv_otp_write(addr, buf, len);
	return HAL_NO_ERR;
}

HAL_STATUS hal_intf_otp_sector_erase(uint32_t addr)
{
	hal_drv_otp_sector_erase(addr);
	return HAL_NO_ERR;
}

HAL_STATUS hal_intf_xip_set_spi_mode(uint8_t mode)
{
#ifdef CHIP_XIP_SUPPORT_RUNTIME_CONFIG
    uint8_t halXipMode = XIP_SPI_MODE_NORMAL;

    switch(mode)
    {
        case XIP_SPI_MODE_SINGLE:
            halXipMode = XIP_SPI_MODE_NORMAL;
        break;
        
        case XIP_SPI_MODE_DUAL:
            halXipMode = XIP_SPI_MODE_DUAL_IO;
        break;
        
        case XIP_SPI_MODE_QUAD:
            halXipMode = XIP_SPI_MODE_QUAD_IO;
        break;

        default:
            return HAL_ERR;
    }

	hal_drv_xip_set_spi_mode(halXipMode);
#endif
	return HAL_NO_ERR;
}

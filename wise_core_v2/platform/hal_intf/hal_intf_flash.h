/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_FLASH_H
#define __HAL_INTF_FLASH_H

#include "esmt_chip_specific.h"
#include "types.h"

enum
{
    XIP_SPI_CLK_40M = 0,
    XIP_SPI_CLK_20M,
    XIP_SPI_CLK_10M,
    XIP_SPI_CLK_5M,
};

enum
{
    XIP_SPI_MODE_SINGLE = 0,
    XIP_SPI_MODE_DUAL,
    XIP_SPI_MODE_QUAD,
};

void hal_intf_flash_init(void);
void hal_intf_flash_get_info(uint32_t *id, uint32_t *flashSize, uint32_t* pageSize);
void hal_intf_flash_deinit(void);
HAL_STATUS hal_intf_flash_read(uint32_t addr, uint8_t *buf, uint32_t len);
HAL_STATUS hal_intf_flash_write(uint32_t addr, uint8_t *buf, uint32_t len);
void hal_intf_flash_get_uid(uint8_t *uid);
void hal_intf_flash_get_dev_id(uint32_t* id);
HAL_STATUS hal_intf_flash_sector_erase(uint32_t addr);
HAL_STATUS hal_intf_flash_block_erase(uint32_t addr);
HAL_STATUS hal_intf_otp_read(uint32_t addr, uint8_t *buf, uint32_t len);
HAL_STATUS hal_intf_otp_write(uint32_t addr, uint8_t *buf, uint32_t len);
HAL_STATUS hal_intf_otp_sector_erase(uint32_t addr);
HAL_STATUS hal_intf_xip_set_spi_mode(uint8_t mode);
#endif /* __HAL_INTF_FLASH_H */

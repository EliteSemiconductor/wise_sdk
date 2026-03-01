/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_FLCTL_H__
#define __HAL_DRV_FLCTL_H__

#include "hal_intf_flash.h"
#include "types.h"
#include "hdl/flctl_er8130.h"

#define hal_drv_flash_init                  flctl_init_er8130
#define hal_drv_flash_deinit                flctl_deinit_er8130
#define hal_drv_flash_read                  flctl_read_er8130
#define hal_drv_flash_write                 flctl_write_er8130
#define hal_drv_flash_get_uid               flctl_get_uid
#define hal_drv_flash_get_dev_id            flctl_get_device_id
#define hal_drv_flash_sector_erase          flctl_sector_erase_er8130
#define hal_drv_flash_block_erase           flctl_block_erase_er8130
#define hal_drv_otp_read                    flctl_otp_read_er8130
#define hal_drv_otp_write                   flctl_otp_write_er8130
#define hal_drv_otp_sector_erase            flctl_otp_sector_erase_er8130
#endif /* __HAL_DRV_FLCTL_H__ */

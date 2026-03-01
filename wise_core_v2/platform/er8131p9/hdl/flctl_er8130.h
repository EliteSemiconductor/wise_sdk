/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __FLCTL_ER8130_H
#define __FLCTL_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

#define XIP_SRAM_BASE 0x40012600
#define FLASH_PAGE_SIZE 256
#define FLASH_UID_OFS 0x80
#define FLASH_80B_UID_LEN 12
#define FLASH_16A_UID_LEN 16
#define FLASH_DEVID_LEN 4
#define FLASH_TYPE_80B 0x38
#define FLASH_TYPE_16A 0x48
#define FLASH_TYPE_25Q 0x60
#define FLASH_TYPE_25D 0x44

typedef struct flctl_info_t FLCTL_INFO_T;

#define XIP_AST_CMD_IDLE 0x00
#define XIP_AST_CMD_SECTOR_ERASE 0x01
#define XIP_AST_CMD_HALF_BLOCK_ERASE 0x02
#define XIP_AST_CMD_BLOCK_ERASE 0x03
#define XIP_AST_CMD_PROGRAM 0x04
#define XIP_AST_CMD_READ 0x05
#define XIP_AST_CMD_QUAD_IO_READ 0x06
#define XIP_AST_CMD_ENTER_OTP 0x07
#define XIP_AST_CMD_EXIT_OTP 0x08 // write disable
#define XIP_AST_CMD_READ_DEV_ID 0x09
#define XIP_AST_CMD_READ_ID 0x0A
#define XIP_AST_CMD_READ_SFDP 0x0B
#define XIP_AST_CMD_RDSR 0x0C
#define XIP_AST_CMD_WRSR 0x0D
#define XIP_AST_CMD_CHIP_ERASE 0x0E
#define XIP_AST_CMD_OTP_ERASE 0x0F
#define XIP_AST_CMD_OTP_PROGRAM 0x10
#define XIP_AST_CMD_OTP_READ 0x11
#define XIP_AST_CMD_OTP_RD_NO_CRPT 0x12
#define XIP_AST_CMD_RD_NO_CRPT 0x13
#define XIP_AST_CMD_QD_IO_RD_NO_CRPT 0x14
#define XIP_AST_CMD_OTP_ERASE2 0x15
#define XIP_AST_CMD_OTP_PROGRAM2 0x16
#define XIP_AST_CMD_OTP_READ2 0x17
#define XIP_AST_CMD_OTP_RD_NO_CRPT2 0x18
#define XIP_AST_CMD_WRSR_1 0x19
#define XIP_AST_CMD_READ_UID 0x1A

void flctl_get_uid(uint8_t *uid);
void flctl_get_device_id(uint32_t *dev_id);
void flctl_sector_erase_er8130(uint32_t addr);
void flctl_block_erase_er8130(uint32_t addr);
void flctl_write_er8130(uint32_t addr, uint8_t *buf, uint32_t len);
void flctl_read_er8130(uint32_t addr, uint8_t *buf, uint32_t len);
void flctl_init_er8130(void);
void flctl_deinit_er8130(void);

void flctl_otp_read_er8130(uint32_t addr, uint8_t *buf, uint32_t len);
void flctl_otp_write_er8130(uint32_t addr, uint8_t *buf, uint32_t len);
void flctl_otp_sector_erase_er8130(uint32_t addr);
void flctl_otp_init_er8130(void);
void flctl_otp_deinit_er8130(void);

// XIP SPI Mode definitions
#define XIP_SPI_MODE_NORMAL        0x0  // Normal SPI
#define XIP_SPI_MODE_DUAL_OUT      0x1  // Dual Output
#define XIP_SPI_MODE_QUAD_OUT      0x2  // Quad Output
#define XIP_SPI_MODE_DUAL_IO       0x3  // Dual I/O
#define XIP_SPI_MODE_QUAD_IO       0x4  // Quad I/O

void xip_set_spi_mode(uint8_t mode);

#endif /* __FLCTL_ER8130_H */

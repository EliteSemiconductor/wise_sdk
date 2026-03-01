/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/flctl_er8130.h"

static inline uint8_t xipast_wait_busy(void)
{
    return ((XIPAST->CTL_REG >> XIPAST_XIP_AST_RUN_POS) &
            XIPAST_XIP_AST_RUN_MASK);
}

static inline void xipast_set_addr(uint32_t addr)
{
    XIPAST->ADR_REG =
        ((addr << XIPAST_XIP_AST_ADR_POS) & XIPAST_XIP_AST_ADR_MASK);
}

static inline void xipast_start(void)
{
    XIPAST->CTL_REG = XIPAST_XIP_AST_RUN_MASK;
}

static void xipast_set_config(uint8_t cmd, uint32_t len)
{
    uint32_t reg;

    reg = ((len << XIPAST_XIP_AST_LEN_POS) & XIPAST_XIP_AST_LEN_MASK) |
          ((cmd << XIPAST_XIP_AST_CMD_POS) & XIPAST_XIP_AST_CMD_MASK);
    XIPAST->SET_REG = reg;
}

void flctl_get_device_id(uint32_t *dev_id)
{
    xipast_set_config(XIP_AST_CMD_READ_ID, 0);
    xipast_start();
    while (xipast_wait_busy())
        ;
    memcpy((char *)dev_id, (char *)XIP_SRAM_BASE, FLASH_DEVID_LEN);
}

void flctl_get_uid(uint8_t *uid)
{
	uint32_t dev_id = 0;
	uint8_t cmd = 0, addr = 0, len = 0;
	flctl_get_device_id(&dev_id);

	if(((dev_id >> 8) & 0xFF) == FLASH_TYPE_80B) {
		cmd = XIP_AST_CMD_READ_SFDP;
		addr = FLASH_UID_OFS;
		len = FLASH_80B_UID_LEN;
	}
	else {
		cmd = XIP_AST_CMD_READ_UID;
		addr = 0x0;
		len = FLASH_16A_UID_LEN;
	}

	xipast_set_config(cmd, len);
	xipast_set_addr(addr);
	xipast_start();
	while(xipast_wait_busy());
	memcpy((char *)uid, (char *)XIP_SRAM_BASE, len);
}

void flctl_sector_erase_er8130(uint32_t addr)
{
    xipast_set_config(XIP_AST_CMD_SECTOR_ERASE, 0);
    xipast_set_addr(addr);
    xipast_start();
    while (xipast_wait_busy())
        ;
}

void flctl_block_erase_er8130(uint32_t addr)
{
    xipast_set_config(XIP_AST_CMD_BLOCK_ERASE, 0);
    xipast_set_addr(addr);
    xipast_start();
    while (xipast_wait_busy())
        ;
}

void flctl_write_er8130(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint32_t rest_len   = len;
    uint32_t pgm_len    = 0;
    uint32_t data_cnt   = 0;
    uint32_t flash_addr = 0;

    while (rest_len) {
        pgm_len = (rest_len > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : rest_len;
        xipast_set_config(XIP_AST_CMD_PROGRAM, pgm_len);
        memcpy((char *)XIP_SRAM_BASE, (char *)(buf + data_cnt), pgm_len);
        xipast_set_addr(addr + flash_addr);
        xipast_start();
        while (xipast_wait_busy())
            ;
        rest_len   -= pgm_len;
        data_cnt   += pgm_len;
        flash_addr += pgm_len;
    }
}

void flctl_read_er8130(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint32_t rest_len   = len;
    uint32_t pgm_len    = 0;
    uint32_t data_cnt   = 0;
    uint32_t flash_addr = 0;

    while (rest_len) {
        pgm_len = (rest_len > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : rest_len;
        xipast_set_config(XIP_AST_CMD_READ, pgm_len);
        xipast_set_addr(addr + flash_addr);
        xipast_start();
        while (xipast_wait_busy())
            ;
        memcpy((char *)(buf + data_cnt), (char *)XIP_SRAM_BASE, pgm_len);
        rest_len   -= pgm_len;
        data_cnt   += pgm_len;
        flash_addr += pgm_len;
    }
}

void flctl_init_er8130(void)
{
}

void flctl_deinit_er8130(void)
{
}

void _flctl_sector_erase(uint8_t cmd, uint32_t addr)
{
	xipast_set_config(cmd, 0);
	xipast_set_addr(addr);
	xipast_start();
	while(xipast_wait_busy());
}

void _flctl_write(uint8_t cmd, uint32_t addr, uint8_t *buf, uint32_t len)
{
	uint32_t rest_len = len;
	uint32_t pgm_len = 0;
	uint32_t data_cnt = 0;
	uint32_t flash_addr = 0;

	while(rest_len)
	{
		pgm_len = (rest_len > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : rest_len;

		xipast_set_config(cmd, pgm_len);
		memcpy((char *)XIP_SRAM_BASE, (char *)(buf + data_cnt), pgm_len);
		xipast_set_addr(addr + flash_addr);
		xipast_start();
		while(xipast_wait_busy());

		rest_len -= pgm_len;
		data_cnt += pgm_len;
		flash_addr += pgm_len;
	}
}

void _flctl_read(uint8_t cmd, uint32_t addr, uint8_t *buf, uint32_t len)
{
	uint32_t rest_len = len;
	uint32_t pgm_len = 0;
	uint32_t data_cnt = 0;
	uint32_t flash_addr = 0;

	while(rest_len)
	{
		pgm_len = (rest_len > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : rest_len;
		xipast_set_config(cmd, pgm_len);
		xipast_set_addr(addr + flash_addr);
		xipast_start();
		while(xipast_wait_busy());
		memcpy((char *)(buf + data_cnt), (char *)XIP_SRAM_BASE, pgm_len);
		rest_len -= pgm_len;
		data_cnt += pgm_len;
		flash_addr += pgm_len;
	}
}

void flctl_otp_sector_erase_er8130(uint32_t addr)
{
	uint32_t dev_id = 0;

	flctl_get_device_id(&dev_id);
	dev_id = (dev_id >> 8) & 0xFF;

	_flctl_sector_erase((dev_id == FLASH_TYPE_80B) ? XIP_AST_CMD_OTP_ERASE : XIP_AST_CMD_OTP_ERASE2,
		                addr);
}

void flctl_otp_write_er8130(uint32_t addr, uint8_t *buf, uint32_t len)
{
	uint32_t dev_id = 0;

	flctl_get_device_id(&dev_id);
	dev_id = (dev_id >> 8) & 0xFF;

	_flctl_write((dev_id == FLASH_TYPE_80B) ? XIP_AST_CMD_OTP_PROGRAM : XIP_AST_CMD_OTP_PROGRAM2,
		         addr,
		         buf,
		         len);
}

void flctl_otp_read_er8130(uint32_t addr, uint8_t *buf, uint32_t len)
{
	uint32_t dev_id = 0;

	flctl_get_device_id(&dev_id);
	dev_id = (dev_id >> 8) & 0xFF;

	_flctl_read((dev_id == FLASH_TYPE_80B) ? XIP_AST_CMD_OTP_RD_NO_CRPT : XIP_AST_CMD_OTP_READ2,
		        addr,
		        buf,
		        len);
}


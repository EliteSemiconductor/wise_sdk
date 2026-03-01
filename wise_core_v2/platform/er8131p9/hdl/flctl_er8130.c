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

static void xipast_write_status(uint8_t status)
{
	uint32_t dev_id = 0;
	uint8_t cmd;
	
	flctl_get_device_id(&dev_id);
	dev_id = (dev_id >> 8) & 0xFF;
	cmd = (dev_id == FLASH_TYPE_80B) ? XIP_AST_CMD_WRSR : XIP_AST_CMD_WRSR_1;

	xipast_set_config(cmd, 4);
	xipast_set_addr(0x0);
	REG_W32(XIP_SRAM_BASE, status);
	xipast_start();
	while(xipast_wait_busy());
}

RAM_TEXT void xip_set_spi_mode(uint8_t mode)
{
	uint32_t spi_cmd;
	uint32_t mode_mask;
	uint32_t dummy_bytes;
	uint32_t address_bytes;

	switch(mode) {
		case XIP_SPI_MODE_NORMAL:  // Normal SPI
			spi_cmd = 0x03000000;      // XIP_CMD_READ
			mode_mask = 0x00000000;    // XIP_REG_014_MODE_NORMAL_SPI (bit[30:28] = 0x0)
			dummy_bytes = 0;           // READ: 0 dummy clocks
			address_bytes = 3;
			break;
		case XIP_SPI_MODE_DUAL_OUT:  // Dual Output
			spi_cmd = 0x3B000000;      // XIP_CMD_FAST_READ_DUAL_OUT
			mode_mask = 0x10000000;    // XIP_REG_014_MODE_DUAL_OUT (bit[30:28] = 0x1)
			dummy_bytes = 1;           // DREAD: 8 dummy clocks = 1 byte
			address_bytes = 3;
			break;
		case XIP_SPI_MODE_QUAD_OUT:  // Quad Output
			spi_cmd = 0x6B000000;      // XIP_CMD_FAST_READ_QUAD_OUT
			mode_mask = 0x20000000;    // XIP_REG_014_MODE_QUAD_OUT (bit[30:28] = 0x2)
			dummy_bytes = 1;           // QREAD: 8 dummy clocks = 1 byte
			address_bytes = 3;
			break;
		case XIP_SPI_MODE_DUAL_IO:  // Dual I/O
			spi_cmd = 0xBB000000;      // XIP_CMD_FAST_READ_DUAL_IO
			mode_mask = 0x30000000;    // XIP_REG_014_MODE_DUAL_IO (bit[30:28] = 0x3)
			dummy_bytes = 1;           // 2READ: 4 dummy clocks
			address_bytes = 3;
			break;
		case XIP_SPI_MODE_QUAD_IO:  // Quad I/O (default)
		default:
			spi_cmd = 0xEB000000;      // XIP_CMD_FAST_READ_QUAD_IO
			mode_mask = 0x40000000;    // XIP_REG_014_MODE_QUAD_IO (bit[30:28] = 0x4)
			dummy_bytes = 2;           // 4READ: 6 dummy clocks (at least 2 bytes)
			address_bytes = 4;         // Quad I/O
			break;
	}

	// XIP_REG_014 register configuration
	// bit[31]: QPI_MODE_EN, bit[30:28]: SPI_MODE, bit[27]: CMD_EN, bit[26]: ADR_EN
	// bit[25]: DMY_EN, bit[24]: DAT_EN, bit[19:17]: ADR_LEN, bit[16:12]: DMY_LEN
	uint32_t reg_val = mode_mask |
	                   0x08000000 |  // XIP_REG_014_CMD_EN_MASK (bit[27])
	                   0x04000000 |  // XIP_REG_014_ADR_EN_MASK (bit[26])
	                   0x01000000 |  // XIP_REG_014_DAT_EN_MASK (bit[24])
	                   (((address_bytes - 1) << 17) & 0x000E0000);  // ADR_LEN (bit[19:17])

	// Normal mode does not need dummy bytes
	if (mode != XIP_SPI_MODE_NORMAL) {
		reg_val |= 0x02000000 |  // XIP_REG_014_DMY_EN_MASK (bit[25])
		           (((dummy_bytes - 1) << 12) & 0x0001F000);  // DMY_LEN (bit[16:12])
	}

	REG_W32(0x40012014, reg_val);  // XIP_REG_014 address
	REG_W32(0x40012018, spi_cmd);  // XIP_REG_018 address

	// Flash status register setting (for Quad mode)
	if(mode == XIP_SPI_MODE_QUAD_OUT || mode == XIP_SPI_MODE_QUAD_IO) {  // Quad Output and Quad I/O should be set flash QE bit
		uint32_t dev_id = 0;
		flctl_get_device_id(&dev_id);
		dev_id = (dev_id >> 8) & 0xFF;
		if(dev_id != FLASH_TYPE_80B)
			xipast_write_status(0x02);
	}
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


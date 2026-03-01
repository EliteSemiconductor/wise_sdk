/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_xcvr_api.h"

#define OPCODE_NOP 0x00
#define OPCODE_CONFIG_TX 0x08
#define OPCODE_CONFIG_RX 0x09
#define OPCODE_GET_INT_STS 0x0A
#define OPCODE_SET_INT_EN 0x0B
#define OPCODE_CLEARSTATUS 0x0C
#define OPCODE_GET_MASKED_INT_STS 0x0D
#define OPCODE_CHANGE_MODE 0x0E
#define OPCODE_WR_TX_FIFO 0x10
#define OPCODE_RD_RX_FIFO 0x11
#define OPCODE_RST_FIFO 0x18
#define OPCODE_START_TX 0x20
#define OPCODE_START_RX 0x21
#define OPCODE_RD_PWR 0x31
#define OPCODE_WR_CSR 0x90
#define OPCODE_RD_CSR 0x91
#define OPCODE_WR_TX_FIFO2 0xC0
#define OPCODE_RD_RX_FIFO2 0xD0

WISE_STATUS wise_xcvr_nop(uint8_t spi_channel)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_NOP;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_config_tx(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_CONFIG_TX;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_config_rx(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_CONFIG_RX;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_get_int_sts(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_GET_INT_STS;
    trans_msg.trans_mode           = SPI_TM_DMY_READ;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.rx_data_buff         = (uint8_t *)value;
    trans_msg.rx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_set_int_en(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_SET_INT_EN;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_clear_int_sts(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_CLEARSTATUS;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_get_masked_int_sts(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_GET_MASKED_INT_STS;
    trans_msg.trans_mode           = SPI_TM_DMY_READ;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.rx_data_buff         = (uint8_t *)value;
    trans_msg.rx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_change_mode(uint8_t spi_channel, uint8_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_CHANGE_MODE;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_write_tx_fifo(uint8_t spi_channel, uint16_t nbytes_len, uint32_t *tx_ptr)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN | SPI_MSG_FMT_ADDR_EN);
    trans_msg.cmd                  = OPCODE_WR_TX_FIFO;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = nbytes_len;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)tx_ptr;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_read_rx_fifo(uint8_t spi_channel, uint16_t nbytes_len, uint32_t *rx_ptr)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN | SPI_MSG_FMT_ADDR_EN);
    trans_msg.cmd                  = OPCODE_RD_RX_FIFO;
    trans_msg.trans_mode           = SPI_TM_DMY_READ;
    trans_msg.address              = nbytes_len;
    trans_msg.dummy_len            = 1;
    trans_msg.rx_data_buff         = (uint8_t *)rx_ptr;
    trans_msg.rx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_write_tx_fifo2(uint8_t spi_channel, uint16_t nbytes_len, uint32_t *tx_ptr)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN | SPI_MSG_FMT_ADDR_EN);
    trans_msg.cmd                  = OPCODE_WR_TX_FIFO2;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = nbytes_len;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)tx_ptr;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_read_rx_fifo2(uint8_t spi_channel, uint16_t nbytes_len, uint32_t *rx_ptr)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_RD_RX_FIFO2;
    trans_msg.trans_mode           = SPI_TM_DMY_READ;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.rx_data_buff         = (uint8_t *)rx_ptr;
    trans_msg.rx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_reset_fifo(uint8_t spi_channel)
{
    uint32_t value                 = 0x100;
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_RST_FIFO;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)&value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_start_tx(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_START_TX;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_start_rx(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_START_RX;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_read_power(uint8_t spi_channel, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN);
    trans_msg.cmd                  = OPCODE_RD_PWR;
    trans_msg.trans_mode           = SPI_TM_DMY_READ;
    trans_msg.address              = 0;
    trans_msg.dummy_len            = 1;
    trans_msg.rx_data_buff         = (uint8_t *)value;
    trans_msg.rx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_register_write(uint8_t spi_channel, uint16_t address, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN | SPI_MSG_FMT_ADDR_EN);
    trans_msg.cmd                  = OPCODE_WR_CSR;
    trans_msg.trans_mode           = SPI_TM_WRITE_ONLY;
    trans_msg.address              = address;
    trans_msg.dummy_len            = 1;
    trans_msg.tx_data_buff         = (uint8_t *)value;
    trans_msg.tx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_xcvr_register_read(uint8_t spi_channel, uint16_t address, uint32_t *value)
{
    WISE_SPI_XFER_MSG_T trans_msg  = {0};
    trans_msg.msg_fmt             |= (SPI_MSG_FMT_CMD_EN | SPI_MSG_FMT_ADDR_EN);
    trans_msg.cmd                  = OPCODE_RD_CSR;
    trans_msg.trans_mode           = SPI_TM_DMY_READ;
    trans_msg.address              = address;
    trans_msg.dummy_len            = 1;
    trans_msg.rx_data_buff         = (uint8_t *)value;
    trans_msg.rx_unit_count        = 4;
    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

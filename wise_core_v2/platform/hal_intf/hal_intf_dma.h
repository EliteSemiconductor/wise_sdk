/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_DMA_H__
#define __HAL_INTF_DMA_H__

#include "esmt_chip_specific.h"
#include "types.h"
#include "hal_intf_pmu.h"
#include "drv/hal_drv_dma.h"
#include <stdint.h>

#define IS_DMA_EXT_RX_FUNC(f)                                                                                                                        \
    ((f) == HAL_DMA_SPI0_RX || (f) == HAL_DMA_SPI1_RX || (f) == HAL_DMA_UART0_RX || (f) == HAL_DMA_UART1_RX || (f) == HAL_DMA_UART2_RX ||            \
     (f) == HAL_DMA_I2C0_RX || (f) == HAL_DMA_I2C1_RX)

#define HAL_DMA_I2C_TX(idx) ((idx) == 1 ? HAL_DMA_I2C1_TX : HAL_DMA_I2C0_TX)
#define HAL_DMA_I2C_RX(idx) ((idx) == 1 ? HAL_DMA_I2C1_RX : HAL_DMA_I2C0_RX)
#define HAL_DMA_UART_TX(idx) ((idx) == 0 ? HAL_DMA_UART0_TX : ((idx) == 1 ? HAL_DMA_UART1_TX : HAL_DMA_UART2_TX))
#define HAL_DMA_UART_RX(idx) ((idx) == 0 ? HAL_DMA_UART0_RX : ((idx) == 1 ? HAL_DMA_UART1_RX : HAL_DMA_UART2_RX))
#define HAL_DMA_SPI_TX(idx) ((idx) == 1 ? HAL_DMA_SPI1_TX : HAL_DMA_SPI0_TX)
#define HAL_DMA_SPI_RX(idx) ((idx) == 1 ? HAL_DMA_SPI1_RX : HAL_DMA_SPI0_RX)

#define SPI_TX_CH(spi_index) ((spi_index == 0) ? hal_dma_func_map.spi0_tx : hal_dma_func_map.spi1_tx)
#define SPI_RX_CH(spi_index) ((spi_index == 0) ? hal_dma_func_map.spi0_rx : hal_dma_func_map.spi1_rx)

typedef enum {
    HAL_DMA_EXT_TX = 0x00,
    HAL_DMA_EXT_RX,
    HAL_DMA_EXT_NONE
} HAL_DMA_EXT_DIR_T;

typedef enum {
    HAL_DMA_UNSED = 0,
    HAL_DMA_MAC_TX,
    HAL_DMA_MAC_RX,
    HAL_DMA_AES_IN,
    HAL_DMA_AES_OUT,
    HAL_DMA_AES_AUTH_IN,
    HAL_DMA_SHA,
    HAL_DMA_SPI0_TX,
    HAL_DMA_SPI0_RX,
    HAL_DMA_SPI1_TX,
    HAL_DMA_SPI1_RX,
    HAL_DMA_UART0_TX,
    HAL_DMA_UART0_RX,
    HAL_DMA_UART1_TX,
    HAL_DMA_UART1_RX,
    HAL_DMA_UART2_TX,
    HAL_DMA_UART2_RX,
    HAL_DMA_I2C0_TX,
    HAL_DMA_I2C0_RX,
    HAL_DMA_I2C1_TX,
    HAL_DMA_I2C1_RX,
    HAL_DMA_USER,
    HAL_DMA_FUNC_MAX
} HAL_DMA_SRC_REQ_SEL_T;

typedef struct {
    uint8_t mac_tx;
    uint8_t mac_rx;
    uint8_t aes_in;
    uint8_t aes_out;
    uint8_t aes_auth_in;
    uint8_t sha;
    uint8_t spi0_tx;
    uint8_t spi0_rx;
    uint8_t spi1_tx;
    uint8_t spi1_rx;
    uint8_t uart0_tx;
    uint8_t uart0_rx;
    uint8_t uart1_tx;
    uint8_t uart1_rx;
    uint8_t uart2_tx;
    uint8_t uart2_rx;
    uint8_t i2c0_tx;
    uint8_t i2c0_rx;
    uint8_t i2c1_tx;
    uint8_t i2c1_rx;
    uint8_t user;
} HAL_DMA_FUNC_MAP_T;

typedef enum {
    HAL_DMA_P2M_MAC_TX = 0,
    HAL_DMA_P2M_MAC_RX,
    HAL_DMA_P2M_AES_IN,
    HAL_DMA_P2M_AES_OUT,
    HAL_DMA_P2M_AES_AUTH_IN,
    HAL_DMA_P2M_SHA,
} HAL_DMA_P2M_SRC_REQ_SEL_T;

typedef enum {
    HAL_DMA_M2M_SPI0_TX = 1,
    HAL_DMA_M2M_SPI0_RX,
    HAL_DMA_M2M_SPI1_TX,
    HAL_DMA_M2M_SPI1_RX,
    HAL_DMA_M2M_UART0_TX,
    HAL_DMA_M2M_UART0_RX,
    HAL_DMA_M2M_UART1_TX,
    HAL_DMA_M2M_UART1_RX,
    HAL_DMA_M2M_UART2_TX,
    HAL_DMA_M2M_UART2_RX,
    HAL_DMA_M2M_I2C0,
    HAL_DMA_M2M_I2C1,
} HAL_DMA_M2M_EXT_SRC_REQ_SEL_T;

typedef struct {
    HAL_DMA_SRC_REQ_SEL_T func;
    bool allocated;
    const void *src;
    void *dst;
    uint32_t data_size;
    uint32_t data_len;
    bool int_en;
} HAL_DMA_CHANNEL_STATE_T;

typedef struct {
    const void *input;
    void *output;
    uint32_t input_length;
    uint32_t output_length;
} HAL_DMA_DATA_CFG_T;

typedef struct {
    uint8_t ch_dma_aes_in;
    uint8_t ch_dma_aes_out;
    uint8_t ch_dma_aes_auth;
    HAL_DMA_DATA_CFG_T data_io;
    HAL_DMA_DATA_CFG_T *auth_in; // NULL if unused
} HAL_AES_DMA_SETUP_T;

#ifdef CHIP_DMA_SUPPORT_PERIPHERAL

void hal_intf_dma_init_channels(const uint8_t *ch_func);
void hal_intf_dma_func_map_export_printf(void);
HAL_STATUS hal_intf_aes_dma_init(void);
HAL_STATUS hal_intf_sha_dma_init(void);
HAL_STATUS hal_intf_crypto_dma_init(void);
HAL_STATUS hal_intf_dma_channel_setup(HAL_DMA_SRC_REQ_SEL_T func, const void *src_addr, void *dst_addr, uint32_t data_len, uint32_t data_size,
                                      bool int_en);
HAL_STATUS hal_intf_aes_dma_update(const HAL_AES_DMA_SETUP_T *cfg);
HAL_STATUS hal_intf_dma_channel_trigger(HAL_DMA_SRC_REQ_SEL_T func);
HAL_STATUS hal_intf_aes_dma_setup(const HAL_AES_DMA_SETUP_T *cfg);
HAL_STATUS hal_intf_sha_dma_setup(const HAL_DMA_DATA_CFG_T *cfg);
HAL_STATUS hal_intf_sha_dma_update(const HAL_DMA_DATA_CFG_T *cfg);
HAL_STATUS hal_intf_dma_aes_in_channel_trigger(void);
HAL_STATUS hal_intf_dma_enable_channel(uint8_t dma_channel);

HAL_STATUS hal_intf_dma_extsrc_config(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func);
HAL_STATUS hal_intf_dma_extsrc_update(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len);
HAL_STATUS hal_intf_dma_extsrc_setup(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len);
HAL_STATUS hal_intf_dma_extsrc_send(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len);
HAL_STATUS hal_intf_dma_extsrc_trigger(uint8_t channel, HAL_DMA_SRC_REQ_SEL_T func);

/* ========================================================================== */
/*  SPI DMA Helpers                                                           */
/* ========================================================================== */
HAL_STATUS hal_intf_spi_dma_init(void);
HAL_STATUS hal_intf_spi_dma_update(uint8_t spi_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count);
HAL_STATUS hal_intf_spi_dma_trigger(uint8_t spi_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count);
/* ========================================================================== */
/*  I2C DMA Helpers                                                           */
/* ========================================================================== */
HAL_STATUS hal_intf_i2c_dma_init(void);
HAL_STATUS hal_intf_i2c_dma_update(uint8_t i2c_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count);
HAL_STATUS hal_intf_i2c_dma_trigger(uint8_t i2c_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count);
#else
static inline void hal_intf_dma_init_channels(const uint8_t *ch_func)
{
    (void)ch_func;
}
static inline void hal_intf_dma_func_map_export_printf(void)
{
}

static inline HAL_STATUS hal_intf_aes_dma_init(void)
{
    return HAL_ERR;
}
static inline HAL_STATUS hal_intf_sha_dma_init(void)
{
    return HAL_ERR;
}
static inline HAL_STATUS hal_intf_crypto_dma_init(void)
{
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_dma_channel_setup(HAL_DMA_SRC_REQ_SEL_T func, const void *src_addr, void *dst_addr, uint32_t data_len,
                                                    uint32_t data_size, bool int_en)
{
    (void)func;
    (void)src_addr;
    (void)dst_addr;
    (void)data_len;
    (void)data_size;
    (void)int_en;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_aes_dma_update(const HAL_AES_DMA_SETUP_T *cfg)
{
    (void)cfg;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_dma_channel_trigger(HAL_DMA_SRC_REQ_SEL_T func)
{
    (void)func;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_aes_dma_setup(const HAL_AES_DMA_SETUP_T *cfg)
{
    (void)cfg;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_sha_dma_setup(const HAL_DMA_DATA_CFG_T *cfg)
{
    (void)cfg;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_sha_dma_update(const HAL_DMA_DATA_CFG_T *cfg)
{
    (void)cfg;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_dma_aes_in_channel_trigger(void)
{
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_dma_enable_channel(uint8_t dma_channel)
{
    (void)dma_channel;
    return HAL_ERR;
}

/* ExtSrc helpers */
static inline HAL_STATUS hal_intf_dma_extsrc_config(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func)
{
    (void)ch;
    (void)func;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_dma_extsrc_update(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len)
{
    (void)ch;
    (void)func;
    (void)data;
    (void)len;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_dma_extsrc_setup(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len)
{
    (void)ch;
    (void)func;
    (void)data;
    (void)len;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_dma_extsrc_send(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len)
{
    (void)ch;
    (void)func;
    (void)data;
    (void)len;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_dma_extsrc_trigger(uint8_t channel, HAL_DMA_SRC_REQ_SEL_T func)
{
    (void)channel;
    (void)func;
    return HAL_ERR;
}

/* SPI helpers */
static inline HAL_STATUS hal_intf_spi_dma_init(void)
{
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_spi_dma_update(uint8_t spi_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    (void)spi_index;
    (void)tx_fifo_ptr;
    (void)tx_count;
    (void)rx_fifo_ptr;
    (void)rx_count;
    return HAL_ERR;
}

static inline HAL_STATUS hal_intf_spi_dma_trigger(uint8_t spi_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    (void)spi_index;
    (void)tx_fifo_ptr;
    (void)tx_count;
    (void)rx_fifo_ptr;
    (void)rx_count;
    return HAL_ERR;
}
static inline HAL_STATUS hal_intf_i2c_dma_init(void)
{
    return HAL_ERR;
}
static inline HAL_STATUS hal_intf_i2c_dma_update(uint8_t i2c_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    (void)i2c_index;
    (void)tx_fifo_ptr;
    (void)tx_count;
    (void)rx_fifo_ptr;
    return HAL_ERR;
}
static inline HAL_STATUS hal_intf_i2c_dma_trigger(uint8_t i2c_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    (void)i2c_index;
    (void)tx_fifo_ptr;
    (void)tx_count;
    (void)rx_fifo_ptr;
    return HAL_ERR;
}
#endif
#endif // __HAL_INTF_DMA_H__

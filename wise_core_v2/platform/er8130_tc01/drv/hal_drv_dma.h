/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_DMA_H
#define __HAL_DRV_DMA_H

#include "hal_intf_dma.h"
#include <stdint.h>

#define DMA_BACKUP_LEN 2048
#define DMA_CSR_BKUP_ADDR 0x40040000

#define DMA_M2M_BKUP_CHANL 0
#define DMA_M2M_RCOV_CHANL 1

#define M2M_ADDR_INCRE_MODE 0x00
#define M2M_ADDR_FIXED_MODE 0x02
#define M2M_ADDR_INCRE_TYPE_DEFT 0
#define M2M_ADDR_INCRE_TYPE_32BIT 1

typedef enum {
    DMA_EXT_TX = 0x00,
    DMA_EXT_RX,
    DMA_EXT_NONE
} DMA_EXT_DIR_TYPE;

enum m2m_data_size {
    DATA_SIZE_8BITS = 0,
    DATA_SIZE_16BITS,
    DATA_SIZE_32BITS,
};

typedef enum {
    DMA_PERI_REQ_MAC_TX = 0,
    DMA_PERI_REQ_MAC_RX,
    DMA_PERI_REQ_AES_IN,
    DMA_PERI_REQ_AES_OUT,
    DMA_PERI_REQ_AES_AUTH_IN,
    DMA_PERI_REQ_SHA,
} DMA_PERI_REQ_TYPE_T;

enum dma_channel {
    DMA_CHANNEL_0 = 0,
    DMA_CHANNEL_1,
    DMA_CHANNEL_2,
    DMA_CHANNEL_3,
    DMA_CHANNEL_4,
    DMA_CHANNEL_5,
    DMA_CHANNEL_6,
    DMA_CHANNEL_7,
};

typedef enum {
    DMA_UNSED = 0,
    DMA_MAC_TX,
    DMA_MAC_RX,
    DMA_AES_IN,
    DMA_AES_OUT,
    DMA_AES_AUTH_IN,
    DMA_SHA,
    DMA_SPI0_TX,
    DMA_SPI0_RX,
    DMA_SPI1_TX,
    DMA_SPI1_RX,
    DMA_UART0_TX,
    DMA_UART0_RX,
    DMA_UART1_TX,
    DMA_UART1_RX,
    DMA_UART2_TX,
    DMA_UART2_RX,
    DMA_I2C0_TX,
    DMA_I2C0_RX,
    DMA_I2C1_TX,
    DMA_I2C1_RX,
    DMA_USER,
    DMA_FUNC_MAX
} DMA_SRC_REQ_SEL_T;

typedef enum {
    EXT_SRC_DMA_SPI0_TX = 1,
    EXT_SRC_DMA_SPI0_RX,
    EXT_SRC_DMA_SPI1_TX,
    EXT_SRC_DMA_SPI1_RX,
    EXT_SRC_DMA_UART0_TX,
    EXT_SRC_DMA_UART0_RX,
    EXT_SRC_DMA_UART1_TX,
    EXT_SRC_DMA_UART1_RX,
    EXT_SRC_DMA_UART2_TX,
    EXT_SRC_DMA_UART2_RX,
    EXT_SRC_DMA_I2C0,
    EXT_SRC_DMA_I2C1,
    EXT_SRC_DMA_FUNC_MAX
} DMA_EXT_SRC_REQ_SEL_T;

typedef enum {
    DMA_EVT_CMPLT_A    = (1U << 0),
    DMA_EVT_CMPLT_B    = (1U << 1),
    DMA_EVT_M2M_CMPLT  = (1U << 2),
    DMA_EVT_ERR_OVRN_A = (1U << 3),
    DMA_EVT_ERR_OVRN_B = (1U << 4),
    DMA_EVT_ERR_SIZE_A = (1U << 5),
    DMA_EVT_ERR_SIZE_B = (1U << 6),
} DMA_EVENT_FLAG_T;

typedef enum {
    DMA_DIR_MEM_TO_PERIPH = 0x00,
    DMA_DIR_PERIPH_TO_MEM,
} EXT_DIR_TYPE;

typedef struct {
    uint32_t periph_addr;
    uint8_t data_size;
    EXT_DIR_TYPE def_dir;
    uint8_t ext_src_sel;
} DMA_EXTSRC_INFO_T;

void hal_drv_dma_module_reset(void);
void hal_drv_dma_modem_backup(void);
HAL_STATUS hal_drv_dma_extsrc_config(uint8_t channel, DMA_SRC_REQ_SEL_T func);
HAL_STATUS hal_drv_dma_extsrc_update(uint8_t channel, DMA_SRC_REQ_SEL_T func, uint32_t mem_addr, uint32_t len);
void hal_drv_dma_extsrc_trigger(uint8_t channel, DMA_SRC_REQ_SEL_T func);
HAL_STATUS hal_drv_dma_p2m2p_update_transfer(uint8_t ch, uint32_t addr, uint32_t len);
void hal_drv_dma_buffer_trigger(uint8_t ch, uint32_t buf_op);
void hal_drv_dma_p2m2p_config(uint8_t ch, uint8_t req_idx);
void hal_drv_dma_p2m2p_trigger(uint8_t ch, uint32_t addr, uint32_t len);
void hal_drv_dma_p2m2p_config_and_trigger(uint8_t ch, uint8_t req_idx, uint32_t addr, uint32_t len);
void hal_drv_dma_m2m_config(uint8_t channel, uint32_t src_addr, uint32_t dst_addr, uint32_t data_size, uint32_t data_len);
void hal_drv_dma_m2m_dst_src_config(uint8_t channel, uint8_t src_type, uint8_t dst_type, uint8_t src_nxt_unit, uint8_t dst_nxt_unit);
void hal_drv_dma_src_m2m_trigger(uint8_t channel);
void hal_drv_dma_extsrc_m2m_config(uint8_t channel, uint8_t ext_src_type, DMA_EXT_DIR_TYPE trans_type, uint32_t data_addr, uint32_t data_size,
                                   uint32_t data_len);
void hal_drv_dma_extsrc_m2m_trigger(uint8_t channel, uint8_t req_idx);
uint32_t hal_drv_dma_interrupt_enable(uint8_t channel, bool enable);
uint32_t hal_drv_dma_buf_a_cmplt_status(void);
void hal_drv_dma_buf_a_cmplt_clear(uint32_t status);
uint32_t hal_drv_dma_buf_b_cmplt_status(void);
void hal_drv_dma_buf_b_cmplt_clear(uint32_t status);
uint32_t hal_drv_dma_m2m_cmplt_status(void);
void hal_drv_dma_m2m_cmplt_clear(uint32_t status);
uint32_t hal_drv_dma_get_err_ovrn_a_status(void);
void hal_drv_dma_clear_err_ovrn_a_status(uint32_t status);
uint32_t hal_drv_dma_get_err_ovrn_b_status(void);
void hal_drv_dma_clear_err_ovrn_b_status(uint32_t status);
uint32_t hal_drv_dma_get_err_ex_size_a_status(void);
void hal_drv_dma_clear_err_ex_size_a_status(uint32_t status);
uint32_t hal_drv_dma_get_err_ex_size_b_status(void);
void hal_drv_dma_clear_err_ex_size_b_status(uint32_t status);
HAL_STATUS hal_drv_dma_register_callback(uint8_t dma_ch_idx, EVT_CALLBACK_T cb, void *context);
HAL_STATUS hal_drv_dma_unregister_callback(uint8_t dma_ch_idx);

#endif /* __HAL_DRV_DMA_H */

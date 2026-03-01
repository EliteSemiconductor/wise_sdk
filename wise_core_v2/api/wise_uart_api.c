/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_uart_api.h"
#include "types.h"
#include <stdint.h>
#include "hal_intf_dma.h"

typedef struct {
    uint8_t uartChannel;
    uint8_t onFlag;
    uint32_t clockSel;
    WISE_FIFO_REF rxFifo;
    WISE_FIFO_REF txFifo;
    UART_RX_HANDLER rxCallback;
    uint8_t dma_enable;
} ST_WISE_UART_CTRL_T;

static ST_WISE_UART_CTRL_T uartCtrl[CHIP_UART_CHANNEL_NUM] = {
    {
        0,
        WISE_UART_FLAG_NONE,
        UART0_MODULE,
        NULL,
        NULL,
        NULL,
    },
    {
        1,
        WISE_UART_FLAG_NONE,
        UART1_MODULE,
        NULL,
        NULL,
        NULL,
    },
    {
        2,
        WISE_UART_FLAG_NONE,
        UART2_MODULE,
        NULL,
        NULL,
        NULL,
    },
};

uint8_t uartBusyFlag = 0;

#define UART_SET_BUSY(ch) (uartBusyFlag |= (1 << ch))
#define UART_CLR_BUSY(ch) (uartBusyFlag &= ~(1 << ch))
#define UART_IS_BUSY(ch) ((uartBusyFlag >> ch) & 1)

#if (defined CHIP_HAS_DMA) && (defined CHIP_DMA_SUPPORT_PERIPHERAL)
#ifdef CHIP_DMA_BUFFER_SECTION
STATIC_DMA uint8_t uart_dma_rx_byte[CHIP_UART_CHANNEL_NUM];
STATIC_DMA uint8_t uart_dma_tx_byte[CHIP_UART_CHANNEL_NUM];
#else
static uint8_t uart_dma_rx_byte[CHIP_UART_CHANNEL_NUM];
static uint8_t uart_dma_tx_byte[CHIP_UART_CHANNEL_NUM];
#endif

static int _wise_uart_dma_buf_in_pool(const void *buf, uint32_t len)
{
#ifdef CHIP_DMA_BUFFER_SECTION
    uintptr_t start = (uintptr_t)&__dma_pool_start__;
    uintptr_t end   = (uintptr_t)&__dma_pool_end__;
    uintptr_t addr  = (uintptr_t)buf;

    if ((buf == NULL) || (len == 0U)) {
        return 0;
    }

    return (addr >= start) && ((addr + len) <= end);
#else
    (void)buf;
    (void)len;
    return 1;
#endif
}
#endif

static void _wise_uart_rx_isr(void *context, uint8_t uart_idx);
static void _wise_uart_tx_isr(void *context, uint8_t uart_idx);
static void _wise_uart_process_tx_fifo(uint8_t channel);

static void _wise_uart_rx_isr(void *context, uint8_t uart_idx)
{
    ST_WISE_UART_CTRL_T *uCtrl = (ST_WISE_UART_CTRL_T *)&uartCtrl[uart_idx];
    uint8_t rByte              = 0;

    if (uCtrl->rxFifo) {
        if (uartCtrl[uart_idx].dma_enable != ENABLE) {
            while (WISE_SUCCESS == hal_intf_uart_poll_byte(uart_idx, &rByte)) {
                wise_fifo_push_char(uCtrl->rxFifo, rByte);
            }
        }
#if (defined CHIP_HAS_DMA) && (defined CHIP_DMA_SUPPORT_PERIPHERAL)
        else {
            uint8_t *dma_rx = &uart_dma_rx_byte[uart_idx];

            if (hal_intf_dma_channel_setup(HAL_DMA_UART_RX(uart_idx), NULL, dma_rx, 1, 0, 0) == HAL_ERR) {
                return;
            }
            hal_intf_dma_channel_trigger(HAL_DMA_UART_RX(uart_idx));
            wise_fifo_push_char(uCtrl->rxFifo, *dma_rx);
        }
#endif
    } else if (uCtrl->rxCallback) {
        while (WISE_SUCCESS == hal_intf_uart_poll_byte(uart_idx, &rByte)) {
            (uCtrl->rxCallback)(uart_idx, rByte);
        }
    }
}

static void _wise_uart_tx_isr(void *context, uint8_t uart_idx)
{
    UART_CLR_BUSY(uart_idx);
    _wise_uart_process_tx_fifo(uart_idx);
}

void wise_uart_init()
{
    hal_intf_uart_register_callback(HAL_UART_EVT_RX_FIN, _wise_uart_rx_isr, NULL);
    hal_intf_uart_register_callback(HAL_UART_EVT_TX_FIN, _wise_uart_tx_isr, NULL);
}

void wise_uart_reset_fifo(uint8_t uartIntf, uint8_t type)
{
    HAL_UART_RESET_TYPE resetFlag = 0;

    if(type & WISE_UART_FLAG_RX)
        resetFlag |= HAL_UART_RESET_RX;
    if(type & WISE_UART_FLAG_TX)
        resetFlag |= HAL_UART_RESET_TX;

    if(resetFlag)
        hal_intf_uart_reset_fifo(uartIntf, resetFlag);
}

void wise_uart_config(uint8_t uartIntf, WISE_UART_CFG_T *cfg)
{
    HAL_UART_CFG_T uart_cfg;

    if (uartIntf < CHIP_UART_CHANNEL_NUM) {
        hal_intf_module_clk_enable(uartCtrl[uartIntf].clockSel);

        uart_cfg.baudrate = cfg->baudrate;
        uart_cfg.data     = cfg->data;
        uart_cfg.parity   = cfg->parity;
        uart_cfg.stop     = cfg->stop;

#if (defined CHIP_HAS_DMA) && (defined CHIP_DMA_SUPPORT_PERIPHERAL)
        uart_cfg.dmaConfig            = cfg->dmaEnable;
        uartCtrl[uartIntf].dma_enable = cfg->dmaEnable;
#endif
        hal_intf_uart_set_config(uartIntf, &uart_cfg);

        UART_CLR_BUSY(uartIntf);
    }
}

void wise_uart_set_buffer(uint8_t uartIntf, WISE_BUFFER_T *rxBuffer, WISE_BUFFER_T *txBuffer)
{
    if (uartIntf >= CHIP_UART_CHANNEL_NUM) {
        return;
    }

    if (rxBuffer && (rxBuffer->length) && (rxBuffer->addr != 0)) {
        uartCtrl[uartIntf].rxFifo = wise_fifo_create((uint8_t *)rxBuffer->addr, rxBuffer->length);
    }

    if (txBuffer && (txBuffer->length) && (txBuffer->addr != 0)) {
        uartCtrl[uartIntf].txFifo = wise_fifo_create((uint8_t *)txBuffer->addr, txBuffer->length);
    }
}

void wise_uart_enable(uint8_t uartIntf, uint8_t opMode)
{
    if (uartIntf >= CHIP_UART_CHANNEL_NUM) {
        return;
    }

    if (opMode == WISE_UART_FLAG_NONE) {
        hal_intf_uart_disable_irq(uartIntf);
        uartCtrl[uartIntf].onFlag = opMode;

        hal_intf_module_clk_disable(uartCtrl[uartIntf].clockSel);

        return;
    }

    uartCtrl[uartIntf].onFlag = opMode;

    if (uartCtrl[uartIntf].rxFifo) {
        wise_fifo_reset(uartCtrl[uartIntf].rxFifo);
    }

    if (uartCtrl[uartIntf].txFifo) {
        wise_fifo_reset(uartCtrl[uartIntf].txFifo);
    }

    //reset uart HW fifo & int status
    hal_intf_uart_reset_fifo(uartIntf, HAL_UART_RESET_ALL);
    if (uartIntf == 0) {
        NVIC_ClearPendingIRQ(UART0_IRQn);
    } else if (uartIntf == 1) {
        NVIC_ClearPendingIRQ(UART1_IRQn);
    } else if (uartIntf == 2) {
        NVIC_ClearPendingIRQ(UART2_IRQn);
    }

    if ((uartCtrl[uartIntf].onFlag & WISE_UART_FLAG_RX) && (uartCtrl[uartIntf].rxFifo != NULL)) {
        hal_intf_uart_enable_irq(uartIntf, HAL_UART_EVT_RX_FIN);
    }

    if ((uartCtrl[uartIntf].onFlag & WISE_UART_FLAG_TX) && (uartCtrl[uartIntf].txFifo != NULL)) {
        hal_intf_uart_enable_irq(uartIntf, HAL_UART_EVT_TX_FIN);
    }

    // TODO: TX FIFO?
}

int32_t wise_uart_write(uint8_t uartIntf, uint8_t *buf, int reqLen)
{
    if (uartCtrl[uartIntf].dma_enable != ENABLE) {
        if (uartCtrl[uartIntf].onFlag & WISE_UART_FLAG_TX) {
            // if(uartCtrl[uartIntf].onFlag & UART_TX_FIFO_ON)
            if (uartCtrl[uartIntf].txFifo) {
            } else {
                hal_intf_uart_write_data(uartIntf, buf, reqLen);
            }
        }
    } else {
#if (defined CHIP_HAS_DMA) && (defined CHIP_DMA_SUPPORT_PERIPHERAL)
        if (!_wise_uart_dma_buf_in_pool(buf, (uint32_t)reqLen)) {
            return WISE_FAIL;
        }
        if (hal_intf_dma_channel_setup(HAL_DMA_UART_TX(uartIntf), buf, NULL, reqLen, 0, 1) == HAL_ERR) {
            return WISE_FAIL;
        }
        hal_intf_dma_channel_trigger(HAL_DMA_UART_TX(uartIntf));
#else
        return WISE_FAIL;
#endif
    }

    return WISE_SUCCESS;
}

int32_t wise_uart_write_char(uint8_t uartIntf, uint8_t dataByte)
{
    if (uartCtrl[uartIntf].onFlag & WISE_UART_FLAG_TX) {
        if (uartCtrl[uartIntf].txFifo) {
            wise_fifo_push_char(uartCtrl[uartIntf].txFifo, dataByte);
            _wise_uart_process_tx_fifo(uartIntf);
        } else {
            if (uartCtrl[uartIntf].dma_enable != ENABLE) {
                hal_intf_uart_write_data(uartIntf, &dataByte, 1);
            } else {
#if (defined CHIP_HAS_DMA) && (defined CHIP_DMA_SUPPORT_PERIPHERAL)
                uint8_t *dma_tx = &uart_dma_tx_byte[uartIntf];
                *dma_tx = dataByte;
                if (hal_intf_dma_channel_setup(HAL_DMA_UART_TX(uartIntf), dma_tx, NULL, 1, 0, 0) == HAL_ERR) {
                    return WISE_FAIL;
                }
                hal_intf_dma_channel_trigger(HAL_DMA_UART_TX(uartIntf));
#else
                return WISE_FAIL;
#endif
            }
        }
    }

    return WISE_SUCCESS;
}

int32_t wise_uart_read(uint8_t uartIntf, uint8_t *buf, int reqLen)
{
    // printf("%s: %d, %p, %d\n",__func__, uartIntf, buf, reqLen);

    return WISE_FAIL;
}

int32_t wise_uart_read_char(uint8_t uartIntf, uint8_t *outputC)
{
    uint8_t c;

    if (WISE_SUCCESS == wise_fifo_pop_char(uartCtrl[uartIntf].rxFifo, &c)) {
        *outputC = c;
        return WISE_SUCCESS;
    }
    return WISE_FAIL;
}

int32_t wise_uart_check_data_len(uint8_t uartIntf)
{
    return wise_fifo_get_data_len(uartCtrl[uartIntf].rxFifo);
}

void wise_uart_set_rx_int_handler(uint8_t uartIntf, UART_RX_HANDLER rxCallback)
{
    if (uartIntf < CHIP_UART_CHANNEL_NUM) {
        uartCtrl[uartIntf].rxCallback = rxCallback;
    }
}

void wise_uart_enable_interrupt(int uartIntf)
{
    hal_intf_uart_enable_irq(uartIntf, HAL_UART_EVT_RX_FIN);
}

void wise_uart_disable_interrupt(int uartIntf)
{
    hal_intf_uart_disable_irq(uartIntf);
}

static void _wise_uart_process_tx_fifo(uint8_t channel)
{
    CORE_DECLARE_IRQ_STATE;

    if (!(uartCtrl[channel].txFifo)) {
        return;
    }

    if (!CORE_IS_IN_ISR()) {
        CORE_ENTER_CRITICAL();
    }

    if (!UART_IS_BUSY(channel)) {
        uint8_t outputC;

        if (WISE_SUCCESS == wise_fifo_pop_char_no_crit(uartCtrl[channel].txFifo, &outputC)) {
            if (uartCtrl[channel].dma_enable != ENABLE) {
                hal_intf_uart_write_data(0, &outputC, 1);
                UART_SET_BUSY(channel);
            } else {
#if (defined CHIP_HAS_DMA) && (defined CHIP_DMA_SUPPORT_PERIPHERAL)
                uint8_t *dma_tx = &uart_dma_tx_byte[channel];
                *dma_tx = outputC;
                if (hal_intf_dma_channel_setup(HAL_DMA_UART_TX(channel), dma_tx, NULL, 1, 0, 1) == HAL_ERR) {
                    return; //FAIL
                }
                hal_intf_dma_channel_trigger(HAL_DMA_UART_TX(channel));
#endif
            }
        }
    }

    if (!CORE_IS_IN_ISR()) {
        CORE_EXIT_CRITICAL();
    }
}

void wise_uart_proc()
{
    _wise_uart_process_tx_fifo(0);
    _wise_uart_process_tx_fifo(1);
    _wise_uart_process_tx_fifo(2);
}

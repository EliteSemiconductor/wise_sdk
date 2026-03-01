/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_uart_api.h
 * @brief UART (Universal Asynchronous Receiver-Transmitter) driver APIs.
 *
 * @ingroup WISE_UART
 *
 * This header provides the interfaces for configuring and using UART
 * peripherals, including baud rate, data format, flow control, FIFO
 * buffers, and interrupt-based RX handling.
 */

#ifndef __WISE_UART_API_H
#define __WISE_UART_API_H

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "esmt_chip_specific.h"
#include "hal_intf_pmu.h"
#include "hal_intf_uart.h"
#include "types.h"

/**
 * @defgroup WISE_UART WISE UART APIs
 * @ingroup WISE_CORE_API
 * @brief UART configuration, data transfer and interrupt handling.
 * @{
 */

/**
 * @name UART operation flags
 * @brief Flags used in wise_uart_enable().
 * @{
 */
#define WISE_UART_FLAG_NONE             0
#define WISE_UART_FLAG_RX               0x01
#define WISE_UART_FLAG_TX               0x02
#define WISE_UART_FLAG_BI_DIR           (WISE_UART_FLAG_RX | WISE_UART_FLAG_TX)
/** @} */

/**
 * @typedef UART_RX_HANDLER
 * @brief RX callback function type.
 *
 * @param uartIntf UART interface index.
 * @param rxByte   Received byte.
 */
typedef void (*UART_RX_HANDLER)(uint8_t uartIntf, uint8_t rxByte);

/**
 * @name UART data bits options
 * @{
 */
enum {
    E_UART_DATA_5_BIT = 0, /**< 5 data bits. */
    E_UART_DATA_6_BIT,     /**< 6 data bits. */
    E_UART_DATA_7_BIT,     /**< 7 data bits. */
    E_UART_DATA_8_BIT,     /**< 8 data bits. */
};
/** @} */

/**
 * @name UART parity options
 * @{
 */
enum {
    E_UART_PARITY_NONE = 0, /**< No parity. */
    E_UART_PARITY_ODD,      /**< Odd parity. */
    E_UART_PARITY_EVEN,     /**< Even parity. */
};
/** @} */


/**
 * @name UART stop bit options
 * @{
 */
enum {
    E_UART_STOP_1_BIT = 0, /**< 1 stop bit. */
    E_UART_STOP_2_BIT,     /**< 2 stop bits. */
};
/** @} */

/**
 * @name UART flow control options
 * @{
 */
enum {
    E_UART_FLOW_NONE = 0, /**< No flow control. */
    E_UART_FLOW_CTS_RTS,  /**< CTS/RTS flow control. */
};
/** @} */

/**
 * @struct WISE_UART_CFG_T
 * @brief UART configuration parameters.
 */
typedef struct {
    uint32_t baudrate; /**< Baud rate for UART communication. */
    uint8_t  data;     /**< Data bits setting, see E_UART_DATA_* definitions. */
    uint8_t  parity;   /**< Parity setting, see E_UART_PARITY_* definitions. */
    uint8_t  stop;     /**< Stop bits setting, see E_UART_STOP_* definitions. */
#if (defined CHIP_HAS_DMA) && (defined CHIP_DMA_SUPPORT_PERIPHERAL)
    uint8_t  dmaEnable;/**< DMA enable setting (0 = disable, 1 = enable). */
#endif
} WISE_UART_CFG_T;

/* ==== API functions ====================================================== */

/**
 * @brief Initialize UART subsystem.
 *
 * Sets up UART hardware and internal resources. Must be called before
 * using other UART APIs.
 */
void wise_uart_init(void);

/**
 * @brief Configure a UART interface.
 *
 * Applies baud rate, data bits, parity, stop bits and optional DMA settings.
 *
 * @param[in] uartIntf UART interface index.
 * @param[in] cfg      Pointer to a ::WISE_UART_CFG_T structure.
 */
void wise_uart_config(uint8_t uartIntf, WISE_UART_CFG_T *cfg);

/**
 * @brief Set FIFO buffers for a UART interface.
 *
 * Associates RX and TX FIFO buffers with the UART interface to enable
 * buffered (FIFO) mode operation.
 *
 * @param[in] uartIntf UART interface index.
 * @param[in] rxBuffer Pointer to RX FIFO buffer descriptor.
 * @param[in] txBuffer Pointer to TX FIFO buffer descriptor.
 */
void wise_uart_set_buffer(uint8_t uartIntf, WISE_BUFFER_T *rxBuffer, WISE_BUFFER_T *txBuffer);

/**
 * @brief Enable or disable UART directions.
 *
 * Controls whether RX, TX or both are enabled on the specified interface.
 *
 * @param[in] uartIntf UART interface index.
 * @param[in] opMode   Operation flags, see WISE_UART_FLAG_* macros.
 */
void wise_uart_enable(uint8_t uartIntf, uint8_t opMode);

/**
 * @brief Write data to UART.
 *
 * Sends up to @p reqLen bytes from @p buf.
 *
 * @param[in] uartIntf UART interface index.
 * @param[in] buf      Pointer to data buffer.
 * @param[in] reqLen   Number of bytes to send.
 *
 * @return Number of bytes written, or a negative error code on failure.
 */
int32_t wise_uart_write(uint8_t uartIntf, uint8_t *buf, int reqLen);

/**
 * @brief Write a single byte to UART.
 *
 * @param[in] uartIntf UART interface index.
 * @param[in] dataByte Byte to transmit.
 *
 * @return Number of bytes written (1 on success), or a negative error code.
 */
int32_t wise_uart_write_char(uint8_t uartIntf, uint8_t dataByte);

/**
 * @brief Read data from UART.
 *
 * Attempts to read up to @p reqLen bytes into @p buf.
 *
 * @param[in]  uartIntf UART interface index.
 * @param[out] buf      Pointer to receive buffer.
 * @param[in]  reqLen   Maximum number of bytes to read.
 *
 * @return Number of bytes read, or a negative error code on failure.
 */
int32_t wise_uart_read(uint8_t uartIntf, uint8_t *buf, int reqLen);

/**
 * @brief Read a single byte from UART.
 *
 * @param[in]  uartIntf UART interface index.
 * @param[out] outputC  Pointer to variable that receives the byte.
 *
 * @return Number of bytes read (1 on success), or a negative error code.
 */
int32_t wise_uart_read_char(uint8_t uartIntf, uint8_t *outputC);

/**
 * @brief Check length of data available in UART RX buffer.
 *
 * @param[in] uartIntf UART interface index.
 *
 * @return Number of bytes available to read, or a negative error code.
 */
int32_t wise_uart_check_data_len(uint8_t uartIntf);

/**
 * @brief Set RX interrupt handler callback.
 *
 * The callback is invoked when RX data is available.
 *
 * @param[in] uartIntf    UART interface index.
 * @param[in] rxCallback  Callback function.
 */
void wise_uart_set_rx_int_handler(uint8_t uartIntf, UART_RX_HANDLER rxCallback);

/**
 * @brief Enable UART interrupt.
 *
 * @param[in] uartIntf UART interface index.
 */
void wise_uart_enable_interrupt(int uartIntf);

/**
 * @brief Disable UART interrupt.
 *
 * @param[in] uartIntf UART interface index.
 */
void wise_uart_disable_interrupt(int uartIntf);

/**
 * @brief Reset UART FIFO.
 *
 * @param[in] uartIntf UART interface index.
 * @param[in] type     Reset type (implementation-specific).
 */
void wise_uart_reset_fifo(uint8_t uartIntf, uint8_t type);

/**
 * @brief UART processing routine.
 *
 * Intended to be called periodically or from a main loop to handle
 * UART-related background tasks.
 */
void wise_uart_proc(void);


/** @} */ /* end of WISE_UART group */

#endif /* __WISE_UART_API_H */

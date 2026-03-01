/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_spi_api.h
 * @brief SPI (Serial Peripheral Interface) driver APIs.
 *
 * @ingroup WISE_SPI
 *
 * This header provides the interfaces for configuring and using SPI in
 * master/slave mode, including clock settings, data format, transfer
 * message structures, and event callback registration.
 */

#ifndef __WISE_SPI_API_H
#define __WISE_SPI_API_H

#include "wise_core.h"
#include "api/wise_gpio_api.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_pmu.h"
#include "hal_intf_spi.h"
#include "types.h"
#include <stdint.h>

/**
 * @defgroup WISE_SPI WISE SPI APIs
 * @ingroup WISE_CORE_API
 * @brief SPI configuration, transfer and event callback interfaces.
 * @{
 */

/**
 * @typedef SPI_EVT_CALLBACK_T
 * @brief SPI event callback function type.
 *
 * @param spi_channel SPI channel index.
 * @param spi_event   Event code (one of ::WISE_SPI_CB_EVENT_T or HAL-defined).
 * @param context     User-defined context pointer.
 */
typedef void (*SPI_EVT_CALLBACK_T)(uint8_t spi_channel, uint32_t spi_event, void *context);

/**
 * @enum WISE_SLAVE_CMD_T
 * @brief SPI slave command set for specific protocols.
 *
 * These commands are typically used when the device operates as an SPI slave.
 */
typedef enum {
    READ_STATUS_SINGLE_IO = 0x05, /**< Read status via single I/O. */
    READ_STATUS_DUAL_IO   = 0x15, /**< Read status via dual I/O. */
    READ_STATUS_QUAD_IO   = 0x25, /**< Read status via quad I/O. */
    READ_DATA_SINGLE_IO   = 0x0B, /**< Read data via single I/O. */
    READ_DATA_DUAL_IO     = 0x0C, /**< Read data via dual I/O. */
    READ_DATA_QUAD_IO     = 0x0E, /**< Read data via quad I/O. */
    WRITE_DATA_SINGLE_IO  = 0x51, /**< Write data via single I/O. */
    WRITE_DATA_DUAL_IO    = 0x52, /**< Write data via dual I/O. */
    WRITE_DATA_QUAD_IO    = 0x54, /**< Write data via quad I/O. */
    USER_CMD              = 0x0A, /**< Generic user-defined command. */
} WISE_SLAVE_CMD_T;

/**
 * @enum WISE_SPI_MASTER_CLOCK_SEL_T
 * @brief SPI master clock selection.
 *
 * Defines possible clock frequencies for the SPI master.
 */
typedef enum {
    E_SPI_CLOCK_SEL_100K = 0, /**< 100 kHz SPI clock. */
    E_SPI_CLOCK_SEL_200K,     /**< 200 kHz SPI clock. */
    E_SPI_CLOCK_SEL_400K,     /**< 400 kHz SPI clock. */
    E_SPI_CLOCK_SEL_500K,     /**< 500 kHz SPI clock. */
    E_SPI_CLOCK_SEL_800K,     /**< 800 kHz SPI clock. */
    E_SPI_CLOCK_SEL_1M,       /**< 1 MHz SPI clock. */
    E_SPI_CLOCK_SEL_2M,       /**< 2 MHz SPI clock. */
    E_SPI_CLOCK_SEL_4M,       /**< 4 MHz SPI clock. */
    E_SPI_CLOCK_SEL_5M,       /**< 5 MHz SPI clock. */
    E_SPI_CLOCK_SEL_10M,      /**< 10 MHz SPI clock. */
    E_SPI_CLOCK_SEL_MAX       /**< Sentinel value, not a valid selection. */
} WISE_SPI_MASTER_CLOCK_SEL_T;

/**
 * @enum WISE_SPI_ROLE
 * @brief SPI role selection.
 */
typedef enum {
    E_SPI_ROLE_MASTER = 0, /**< SPI master role. */
    E_SPI_ROLE_SLAVE,      /**< SPI slave role. */
} WISE_SPI_ROLE;

/**
 * @enum WISE_SPI_BLOCK_MODE_T
 * @brief SPI operation blocking mode.
 */
typedef enum {
    E_SPI_BLOCK_MODE = 0, /**< Blocking operations (API waits until done). */
    E_SPI_NONBLOCK_MODE,  /**< Non-blocking operations. */
} WISE_SPI_BLOCK_MODE_T;

/**
 * @enum SPI_STATE
 * @brief SPI interface state.
 */
typedef enum {
    E_SPI_STATE_OFF = 0, /**< SPI interface disabled. */
    E_SPI_STATE_ON,      /**< SPI interface enabled. */
} SPI_STATE;

/**
 * @enum WISE_SPI_CLOCK_MODE_T
 * @brief SPI clock polarity and phase.
 *
 * The combinations correspond to standard SPI modes 0–3.
 */
typedef enum {
    CLOCK_MODE0, /**< CPOL = 0, CPHA = 0. */
    CLOCK_MODE1, /**< CPOL = 0, CPHA = 1. */
    CLOCK_MODE2, /**< CPOL = 1, CPHA = 0. */
    CLOCK_MODE3  /**< CPOL = 1, CPHA = 1. */
} WISE_SPI_CLOCK_MODE_T;

/**
 * @enum WISE_SPI_MODE_T
 * @brief SPI transfer mode.
 */
typedef enum {
    SPI_MODE_REGULAR, /**< Regular mode (single I/O). */
    SPI_MODE_DUAL,    /**< Dual I/O mode. */
    SPI_MODE_QUAD,    /**< Quad I/O mode. */
    SPI_MODE_3WIRE    /**< 3-wire SPI mode. */
} WISE_SPI_MODE_T;

/**
 * @enum WISE_SPI_BIT_ORDER_T
 * @brief SPI bit order.
 */
typedef enum {
    SPI_MSB_FIRST = 0, /**< Most significant bit first. */
    SPI_LSB_FIRST,     /**< Least significant bit first. */
} WISE_SPI_BIT_ORDER_T;

/**
 * @enum WISE_SPI_RESET_TYPE
 * @brief SPI reset control.
 *
 * Bitmask specifying which parts of the SPI core to reset.
 */
typedef enum {
    WISE_SPI_RESET_CORE = (1 << 0),         /**< Reset SPI core logic. */
    WISE_SPI_RESET_RX   = (1 << 1),         /**< Reset RX path/FIFO. */
    WISE_SPI_RESET_TX   = (1 << 2),         /**< Reset TX path/FIFO. */
    WISE_SPI_RESET_ALL  = WISE_SPI_RESET_CORE | WISE_SPI_RESET_RX | WISE_SPI_RESET_TX   /**< Reset entire SPI. */
} WISE_SPI_RESET_TYPE;

/**
 * @enum WISE_SPI_CB_EVENT_T
 * @brief SPI callback events.
 */
typedef enum {
    WISE_SPI_EVENT_SLAVE_COMMAND_INTERRUPT, /**< Slave command interrupt event. */
    WISE_SPI_EVENT_TRANSFER_DONE,           /**< A transfer has completed. */
    WISE_SPI_MAX_EVENTS = SPI_MAX_EVENTS,   /**< Total number of SPI events (alias). */
} WISE_SPI_CB_EVENT_T;


/**
 * @defgroup WISE_SPI_MSG_FMT SPI Message Format Flags
 * @brief Flags used to compose SPI message format.
 * @{
 */
/**
 * @brief Enable command (CMD) field in SPI message.
 */
#define SPI_MSG_FMT_CMD_EN 0x01

/**
 * @brief Enable address (ADDR) field in SPI message.
 */
#define SPI_MSG_FMT_ADDR_EN 0x02
/** @} */ /* end of WISE_SPI_MSG_FMT group */


/**
 * @struct WISE_SPI_CONF_T
 * @brief SPI configuration.
 *
 * Holds basic configuration parameters for an SPI interface.
 */
typedef struct {
    WISE_SPI_MODE_T spi_mode;              /**< SPI mode (single/dual/quad/3-wire). */
    WISE_SPI_CLOCK_MODE_T clock_mode;      /**< Clock polarity and phase. */
    WISE_SPI_MASTER_CLOCK_SEL_T clock_sel; /**< Master clock selection. */
    WISE_SPI_BLOCK_MODE_T block_mode;      /**< Blocking or non-blocking mode. */
    uint8_t role;                          /**< Role, see ::WISE_SPI_ROLE. */
    uint8_t data_bit_width;                /**< Data bit width (bits per frame). */
    uint8_t addr_len;                      /**< Address length in bytes. */
    WISE_SPI_BIT_ORDER_T bit_order;        /**< Bit order on the wire. */
    uint8_t data_merge;                    /**< Data merge setting (implementation-specific). */
    uint8_t addr_fmt;                      /**< Address format (implementation-specific). */
    uint8_t dma_enable;                    /**< Non-zero to enable DMA. */
} WISE_SPI_CONF_T;

/**
 * @struct WISE_SPI_XFER_MSG_T
 * @brief SPI transfer message.
 *
 * Describes a single SPI transfer including command, address, dummy cycles
 * and data buffers.
 */
typedef struct {
    uint8_t msg_fmt;                        /**< Message format flags (SPI_MSG_FMT_*). */
    uint8_t cmd;                            /**< Command byte. */
    uint8_t dummy_len;                      /**< Number of dummy bytes/cycles. */
    uint8_t trans_mode;                     /**< Transfer mode (implementation-specific). */
    uint16_t address;                       /**< Address field. */
    void *tx_data_buff;                     /**< Pointer to TX data buffer. */
    uint16_t tx_unit_count;                 /**< Number of TX units (words/bytes). */
    void *rx_data_buff;                     /**< Pointer to RX data buffer. */
    uint16_t rx_unit_count;                 /**< Number of RX units (words/bytes). */
} WISE_SPI_XFER_MSG_T;

/* ==== API functions ====================================================== */

/**
 * @brief Initialize SPI subsystem.
 *
 * Must be called before using other SPI APIs.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_spi_init(void);

/**
 * @brief Deinitialize SPI subsystem.
 *
 * Releases SPI resources and returns the subsystem to an uninitialized state.
 *
 * @retval WISE_SUCCESS Deinitialization successful.
 * @retval WISE_FAIL    Deinitialization failed.
 */
WISE_STATUS wise_spi_deinit(void);

/**
 * @brief Open an SPI channel as master.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] spi_conf    Pointer to ::WISE_SPI_CONF_T configuration.
 *
 * @retval WISE_SUCCESS Channel opened successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_master_open(uint8_t spi_channel, WISE_SPI_CONF_T *spi_conf);

/**
 * @brief Open an SPI channel as slave.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] spi_conf    Pointer to ::WISE_SPI_CONF_T configuration.
 *
 * @retval WISE_SUCCESS Channel opened successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_slave_open(uint8_t spi_channel, WISE_SPI_CONF_T *spi_conf);

/**
 * @brief Indicate that SPI slave is ready to receive commands.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @retval WISE_SUCCESS Operation successful.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_slave_set_ready(uint8_t spi_channel);

/**
 * @brief Get the number of bytes received by the SPI slave.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @return Number of bytes available in slave receive buffer.
 */
uint16_t wise_spi_slave_get_recv_count(uint8_t spi_channel);

/**
 * @brief Close an SPI channel.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @retval WISE_SUCCESS Channel closed successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_close(uint8_t spi_channel);

/**
 * @brief Get the most recent SPI command received in slave mode.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @return Last command byte.
 */
uint8_t wise_spi_get_cmd(uint8_t spi_channel);

/**
 * @brief Master write (blocking) on SPI.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] data_ptr    Pointer to TX buffer.
 * @param[in] len         Number of bytes to transmit.
 *
 * @retval WISE_SUCCESS Transmission successful.
 * @retval WISE_FAIL    Transmission failed.
 */
WISE_STATUS wise_spi_master_write(uint8_t spi_channel, void *data_ptr, uint16_t len);

/**
 * @brief Master read (blocking) from SPI.
 *
 * @param[in]  spi_channel SPI channel index.
 * @param[out] data_ptr    Pointer to RX buffer.
 * @param[in]  len         Number of bytes to receive.
 *
 * @retval WISE_SUCCESS Reception successful.
 * @retval WISE_FAIL    Reception failed.
 */
WISE_STATUS wise_spi_master_read(uint8_t spi_channel, void *data_ptr, uint16_t len);

/**
 * @brief Master write single byte.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] in_byte     Byte to transmit.
 *
 * @retval WISE_SUCCESS Transmission successful.
 * @retval WISE_FAIL    Transmission failed.
 */
WISE_STATUS wise_spi_master_write_byte(uint8_t spi_channel, uint8_t in_byte);

/**
 * @brief Master read single byte.
 *
 * @param[in]  spi_channel SPI channel index.
 * @param[out] out_byte    Pointer to variable receiving the byte.
 *
 * @retval WISE_SUCCESS Reception successful.
 * @retval WISE_FAIL    Reception failed.
 */
WISE_STATUS wise_spi_master_read_byte(uint8_t spi_channel, uint8_t *out_byte);

/**
 * @brief Execute a formatted SPI transfer.
 *
 * Uses ::WISE_SPI_XFER_MSG_T to describe command, address, dummy cycles
 * and TX/RX buffers.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] msg         Pointer to transfer message.
 *
 * @retval WISE_SUCCESS Transfer successful.
 * @retval WISE_FAIL    Transfer failed.
 */
WISE_STATUS wise_spi_msg_xfer(uint8_t spi_channel, WISE_SPI_XFER_MSG_T *msg);

/**
 * @brief Register an SPI event callback.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] cb          Callback function.
 * @param[in] context     User context pointer.
 *
 * @retval WISE_SUCCESS Callback registered.
 * @retval WISE_FAIL    Registration failed.
 */
WISE_STATUS wise_spi_register_event_callback(uint8_t spi_channel, SPI_EVT_CALLBACK_T cb, void *context);

/**
 * @brief Unregister an SPI event callback.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @retval WISE_SUCCESS Callback unregistered.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_unregister_event_callback(uint8_t spi_channel);

/**
 * @brief Prepare TX data into SPI FIFO or internal buffer.
 *
 * Typically used before starting a non-blocking transfer.
 *
 * @param[in] spi_channel   SPI channel index.
 * @param[in] tx_fifo_ptr   Pointer to TX buffer.
 * @param[in] tx_unit_count Number of TX units (words/bytes).
 */
void wise_spi_data_prepare(uint8_t spi_channel, void *tx_fifo_ptr, uint16_t tx_unit_count);

/**
 * @brief Reset parts of the SPI controller.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] type        Reset type mask, see ::WISE_SPI_RESET_TYPE.
 */
void wise_spi_reset(uint8_t spi_channel, WISE_SPI_RESET_TYPE type);


/** @} */ /* end of WISE_SPI group */

#endif /* __WISE_SPI_API_H */

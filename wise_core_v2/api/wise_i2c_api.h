/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_i2c_api.h
 * @brief I2C (Inter-Integrated Circuit) driver APIs.
 *
 * @ingroup WISE_I2C
 *
 * This header provides the interface for configuring and using the I2C
 * controller in master/slave roles, including speed selection, addressing
 * mode, FIFO access, combined transfer, and memory read/write helpers.
 */

#ifndef __WISE_I2C_API_H_
#define __WISE_I2C_API_H_

#include "wise_core.h"
#include "api/wise_gpio_api.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_i2c.h"
#include "hal_intf_pmu.h"
#include "types.h"
#include <stdint.h>

/**
 * @defgroup WISE_I2C WISE I2C APIs
 * @ingroup WISE_CORE_API
 * @brief I2C controller configuration, transfers, and helper functions.
 * @{
 */

/**
 * @brief Disable the I2C controller.
 */
#define I2C_DISABLE (0x0U)

/**
 * @brief Enable the I2C controller.
 */
#define I2C_ENABLE (0x1U)

/**
 * @brief Configure I2C as a slave.
 */
#define I2C_SLAVE (0x0U)

/**
 * @brief Configure I2C as a master.
 */
#define I2C_MASTER (0x1U)

/**
 * @brief Use 7-bit I2C addressing mode.
 */
#define I2C_ADDR_7_BITS (0x0U)

/**
 * @brief Use 10-bit I2C addressing mode.
 */
#define I2C_ADDR_10_BITS (0x1U)

/* ===== Message flags for combined transfer ===== */

/* ---- Message flags ---- */

/**
 * @brief I2C message flag: read direction.
 *
 * When set, the message performs a read from the target.
 */
#define WISE_I2C_M_RD (1u << 0)

/**
 * @brief I2C message flag: 10-bit address.
 */
#define WISE_I2C_M_TEN (1u << 1)

/**
 * @brief I2C message flag: do not send STOP at the end.
 *
 * Used for combined transfers with a repeated START condition.
 */
#define WISE_I2C_M_NO_STOP (1u << 2)

/**
 * @typedef WISE_I2C_EVT_T
 * @brief Type for I2C event bitmask.
 */
typedef uint32_t WISE_I2C_EVT_T;

/**
 * @typedef I2C_EVT_CALLBACK_T
 * @brief I2C event callback type used by driver.
 *
 * @param i2c_channel I2C channel index.
 * @param i2c_event   Event code (driver-specific or @ref WISE_I2C_CB_EVENT_T).
 * @param context     User context pointer.
 */
typedef void (*I2C_EVT_CALLBACK_T)(uint8_t i2c_channel, uint32_t i2c_event, void *context);

/**
 * @enum WISE_I2C_DIR_T
 * @brief I2C transfer direction.
 */
typedef enum {
    WISE_I2C_DIR_TX = 0, /**< Transmit (write to slave). */
    WISE_I2C_DIR_RX = 1, /**< Receive (read from slave). */
} WISE_I2C_DIR_T;

/**
 * @enum I2C_STATUS_T
 * @brief I2C controller status.
 */
typedef enum {
    I2C_STS_INIT, /**< I2C has been initialized. */
    I2C_STS_CMPL, /**< I2C transfer(s) completed. */
} I2C_STATUS_T;

/**
 * @enum I2C_SPEED_SEL_T
 * @brief I2C clock speed selection.
 */
typedef enum {
    E_I2C_CLOCK_SEL_100K = 0,                /**< 100 kb/s I2C speed. */
    E_I2C_CLOCK_SEL_400K,                    /**< 400 kb/s I2C speed. */
    E_I2C_CLOCK_SEL_1M,                      /**< 1 Mb/s I2C speed. */
    E_I2C_CLOCK_SEL_MAX = E_I2C_CLOCK_SEL_1M /**< Maximum I2C speed selection. */
} I2C_SPEED_SEL_T;

/**
 * @enum WISE_I2C_CB_EVENT_T
 * @brief High-level I2C callback events.
 */
typedef enum {
    WISE_I2C_EVENT_TRANSFER_DONE, /**< A transfer has completed. */
    WISE_I2C_EVENT_ADDRESS_HIT,   /**< Slave address match event. */
    WISE_I2C_MAX_EVENTS = I2C_MAX_EVENTS, /**< Maximum number of supported events (alias). */
} WISE_I2C_CB_EVENT_T;

/**
 * @struct WISE_I2C_XFER_MSG_T
 * @brief I2C message structure for combined transfer.
 *
 * Describes a single segment (read or write) in a combined transaction.
 */
typedef struct {
    uint16_t addr;   /**< Target slave address. */
    uint16_t flags;  /**< Message flags (WISE_I2C_M_*). */
    uint8_t *buf;    /**< Pointer to data buffer. */
    uint16_t len;    /**< Number of bytes to transfer. */
} WISE_I2C_XFER_MSG_T;

/**
 * @struct WISE_I2C_CONF_T
 * @brief I2C configuration structure.
 *
 * Defines controller index, role, enable flags, speed, addressing mode,
 * direction and target address.
 */
typedef struct {
    uint8_t i2c_idx;          /**< I2C controller index. */
    uint8_t role;             /**< Master or slave role (I2C_MASTER / I2C_SLAVE). */
    uint8_t i2cEn;            /**< Enable flag for I2C controller (I2C_ENABLE / I2C_DISABLE). */
    uint8_t dmaEn;            /**< Enable DMA mode data transfer (non-zero to enable). */
    I2C_SPEED_SEL_T speedMode;/**< I2C bus speed selection. */
    uint8_t addressing;       /**< Addressing mode (I2C_ADDR_7_BITS / I2C_ADDR_10_BITS). */
    uint8_t dir;              /**< Transfer direction flag (implementation-specific). */
    uint16_t target_address;  /**< Target slave address. */
} WISE_I2C_CONF_T;

/**
 * @struct WISE_I2C_MSG_T
 * @brief I2C message configuration for low-level transfers.
 *
 * Used internally for staged transactions with explicit control
 * over START/address/data/STOP phases.
 */
typedef struct {
    WISE_I2C_DIR_T dir;       /**< Direction (TX/RX). */
    uint16_t target_address;  /**< Target slave address. */
    uint8_t *buf_ptr;         /**< Data buffer pointer. */
    uint32_t data_count;      /**< Number of bytes to transfer. */
    bool en_start;            /**< Enable START condition for this segment. */
    bool en_addr;             /**< Send address in this segment. */
    bool en_data;             /**< Transfer data in this segment. */
    bool en_stop;             /**< Send STOP at the end of this segment. */
} WISE_I2C_MSG_T;

/* ===== API functions ===== */

/**
 * @brief Initialize the I2C subsystem.
 *
 * Must be called before using any I2C APIs.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_i2c_init(void);

/**
 * @brief Deinitialize the I2C subsystem.
 *
 * Releases internal resources and returns the I2C subsystem to an
 * uninitialized state.
 *
 * @retval WISE_SUCCESS Deinitialization successful.
 * @retval WISE_FAIL    Deinitialization failed.
 */
WISE_STATUS wise_i2c_deinit(void);

/**
 * @brief Open (enable) an I2C channel.
 *
 * @param[in] i2c_channel I2C controller/channel index.
 *
 * @retval WISE_SUCCESS Channel opened successfully.
 * @retval WISE_FAIL    Open failed.
 */
WISE_STATUS wise_i2c_open(uint8_t i2c_channel);

/**
 * @brief Close (disable) an I2C channel.
 *
 * @param[in] i2c_channel I2C controller/channel index.
 *
 * @retval WISE_SUCCESS Channel closed successfully.
 * @retval WISE_FAIL    Close failed.
 */
WISE_STATUS wise_i2c_close(uint8_t i2c_channel);

/**
 * @brief Set I2C transfer direction.
 *
 * @param[in] i2c_channel I2C controller/channel index.
 * @param[in] dir         Direction flag (implementation-specific mapping to TX/RX).
 */
void wise_i2c_set_direction(uint8_t i2c_channel, bool dir);

/**
 * @brief Configure an I2C channel.
 *
 * Applies configuration such as role, speed, addressing mode and target address.
 *
 * @param[in] i2c_channel I2C controller/channel index.
 * @param[in] i2c_conf    Pointer to ::WISE_I2C_CONF_T configuration.
 *
 * @retval 0   Configuration successful.
 * @retval <0  Configuration failed.
 */
int32_t wise_i2c_config(uint8_t i2c_channel, WISE_I2C_CONF_T *i2c_conf);

/**
 * @brief Send multiple bytes (blocking) on an I2C interface.
 *
 * @param[in] i2cIntf    I2C controller/channel index.
 * @param[in] data       Pointer to data buffer to send.
 * @param[in] data_count Number of bytes to send.
 *
 * @retval >=0 Number of bytes actually sent.
 * @retval <0  Send failed.
 */
int32_t wise_i2c_send_nbyte(uint8_t i2cIntf, uint8_t *data, uint32_t data_count);

/**
 * @brief Receive multiple bytes (blocking) from an I2C interface.
 *
 * @param[in]  i2cIntf    I2C controller/channel index.
 * @param[out] data       Pointer to buffer to store received data.
 * @param[in]  data_count Number of bytes to receive.
 *
 * @retval >=0 Number of bytes actually received.
 * @retval <0  Receive failed.
 */
int32_t wise_i2c_recv_nbyte(uint8_t i2cIntf, uint8_t *data, uint32_t data_count);

/**
 * @brief Send bytes without issuing a STOP condition at the end.
 *
 * Often used as the first part of a combined transaction where a repeated
 * START is required (for example, register-write followed by read).
 *
 * @param[in] i2cIntf    I2C controller/channel index.
 * @param[in] address    Target slave address.
 * @param[in] bufptr     Pointer to data buffer to send.
 * @param[in] data_count Number of bytes to send.
 *
 * @retval WISE_SUCCESS Operation completed successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_i2c_send_nbyte_with_nonstop(uint8_t i2cIntf, uint16_t address, uint8_t *bufptr, uint32_t data_count);

/**
 * @brief Register an I2C event callback on a channel.
 *
 * @param[in] i2c_channel I2C controller/channel index.
 * @param[in] cb          Callback function.
 * @param[in] context     User context pointer passed to the callback.
 *
 * @retval WISE_SUCCESS Callback registered successfully.
 * @retval WISE_FAIL    Registration failed.
 */
WISE_STATUS wise_i2c_register_event_callback(uint8_t i2c_channel, I2C_EVT_CALLBACK_T cb, void *context);

/**
 * @brief Unregister a previously registered I2C event callback.
 *
 * @param[in] i2c_channel I2C controller/channel index.
 *
 * @retval WISE_SUCCESS Callback unregistered successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_i2c_unregister_event_callback(uint8_t i2c_channel);

/**
 * @brief Get the current data count in the I2C FIFO.
 *
 * @param[in] i2cIntf I2C controller/channel index.
 *
 * @return Number of bytes currently in FIFO.
 */
uint8_t wise_i2c_get_data_count(uint8_t i2cIntf);

/**
 * @brief Get one byte from I2C data register/FIFO.
 *
 * @param[in] i2cIntf I2C controller/channel index.
 *
 * @return Data byte read from the controller.
 */
uint8_t wise_i2c_get_data(uint8_t i2cIntf);

/**
 * @brief Clear I2C FIFO.
 *
 * Empties the internal FIFO for the given I2C interface.
 *
 * @param[in] i2cIntf I2C controller/channel index.
 */
void wise_i2c_clear_fifo(uint8_t i2cIntf);

/**
 * @brief Perform a combined I2C transfer.
 *
 * Executes @p nmsgs messages in a single transaction sequence, optionally
 * using repeated START conditions without releasing the bus in between.
 *
 * @param[in] intf        I2C controller/channel index.
 * @param[in] msgs        Pointer to an array of ::WISE_I2C_XFER_MSG_T.
 * @param[in] nmsgs       Number of messages in the array.
 * @param[in] interval_ms Interval between messages in milliseconds.
 *
 * @retval 0   Transfer completed successfully.
 * @retval <0  Transfer failed.
 */
int32_t wise_i2c_transfer(uint8_t intf, const WISE_I2C_XFER_MSG_T *msgs, size_t nmsgs, uint32_t interval_ms);

/**
 * @brief Read from an I2C memory-like device (for example EEPROM or registers).
 *
 * This helper sends a register address and then reads data from that address.
 *
 * @param[in]  intf       I2C controller/channel index.
 * @param[in]  addr       Target slave address.
 * @param[in]  reg        Register/memory address to read from.
 * @param[in]  reg_len    Length of @p reg in bytes.
 * @param[out] rx         Pointer to receive buffer.
 * @param[in]  count      Number of bytes to read.
 * @param[in]  timeout_ms Timeout in milliseconds.
 *
 * @retval 0   Read completed successfully.
 * @retval <0  Read failed or timed out.
 */
int32_t wise_i2c_mem_read(uint8_t intf, uint16_t addr, uint32_t reg, uint8_t reg_len, uint8_t *rx, uint16_t count, uint32_t timeout_ms);

/**
 * @brief Write to an I2C memory-like device (for example EEPROM or registers).
 *
 * This helper sends a register address followed by data bytes to write.
 *
 * @param[in] intf       I2C controller/channel index.
 * @param[in] addr       Target slave address.
 * @param[in] reg        Register/memory address to write to.
 * @param[in] reg_len    Length of @p reg in bytes.
 * @param[in] tx         Pointer to data buffer to write.
 * @param[in] count      Number of bytes to write.
 * @param[in] timeout_ms Timeout in milliseconds.
 *
 * @retval 0   Write completed successfully.
 * @retval <0  Write failed or timed out.
 */
int32_t wise_i2c_mem_write(uint8_t intf, uint16_t addr, uint32_t reg, uint8_t reg_len, const uint8_t *tx, uint16_t count, uint32_t timeout_ms);

#endif /* __WISE_I2C_API_H_ */

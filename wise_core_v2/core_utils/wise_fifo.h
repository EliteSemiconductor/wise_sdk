/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_fifo.h
 * @brief Simple software FIFO (ring buffer) interface.
 *
 * @ingroup WISE_FIFO
 *
 * This header defines a lightweight FIFO utility for buffering bytes,
 * typically used between ISR and task contexts.
 */

#ifndef __WISE_FIFO_H
#define __WISE_FIFO_H

#include <stdint.h>

/**
 * @defgroup WISE_FIFO WISE FIFO APIs
 * @ingroup WISE_CORE_API
 * @brief Simple byte-oriented FIFO creation and access.
 * @{
 */

/**
 * @typedef WISE_FIFO_REF
 * @brief Opaque reference to a FIFO instance.
 *
 * The actual FIFO structure is hidden from users. All operations are
 * performed through this handle.
 *
 * @ingroup WISE_FIFO
 */
typedef void *WISE_FIFO_REF;

/**
 * @brief Create and initialize a FIFO on top of a user-provided buffer.
 *
 * The caller supplies a buffer and its length; the FIFO implementation
 * uses this memory for queued data. The buffer must remain valid for
 * the lifetime of the FIFO.
 *
 * @param[in] buffer Pointer to memory used as FIFO storage.
 * @param[in] bufLen Size of @p buffer in bytes.
 *
 * @return A FIFO reference on success, or NULL on failure.
 *
 * @ingroup WISE_FIFO
 */
WISE_FIFO_REF wise_fifo_create(uint8_t *buffer, uint16_t bufLen);

/**
 * @brief Reset FIFO state.
 *
 * Clears stored data and resets read/write indices. The underlying
 * buffer content is considered invalid after reset.
 *
 * @param[in] fifoRef FIFO reference returned by ::wise_fifo_create().
 *
 * @ingroup WISE_FIFO
 */
void wise_fifo_reset(WISE_FIFO_REF fifoRef);

/**
 * @brief Push a single byte into FIFO.
 *
 * This variant is typically implemented with critical section protection.
 *
 * @param[in] fifoRef FIFO reference.
 * @param[in] c       Byte to push.
 *
 * @retval 0   Byte pushed successfully.
 * @retval <0  FIFO is full or @p fifoRef is invalid.
 *
 * @ingroup WISE_FIFO
 */
int8_t wise_fifo_push_char(WISE_FIFO_REF fifoRef, uint8_t c);

/**
 * @brief Push multiple bytes into FIFO.
 *
 * Attempts to write up to @p len bytes from @p buf into the FIFO.
 *
 * @param[in] fifoRef FIFO reference.
 * @param[in] buf     Pointer to data buffer.
 * @param[in] len     Number of bytes to push.
 *
 * @return Number of bytes actually pushed (may be less than @p len if FIFO is full).
 *
 * @ingroup WISE_FIFO
 */
uint16_t wise_fifo_push(WISE_FIFO_REF fifoRef, uint8_t *buf, uint16_t len);

/**
 * @brief Pop a single byte from FIFO.
 *
 * This variant is typically implemented with critical section protection.
 *
 * @param[in]  fifoRef FIFO reference.
 * @param[out] c       Pointer to variable receiving popped byte.
 *
 * @retval 0   Byte popped successfully.
 * @retval <0  FIFO is empty or @p fifoRef is invalid.
 *
 * @ingroup WISE_FIFO
 */
int8_t wise_fifo_pop_char(WISE_FIFO_REF fifoRef, uint8_t *c);

/**
 * @brief Push a single byte into FIFO without entering a critical section.
 *
 * This variant is intended for contexts where the caller already
 * handles concurrency or where interrupts are disabled.
 *
 * @param[in] fifoRef FIFO reference.
 * @param[in] c       Byte to push.
 *
 * @retval 0   Byte pushed successfully.
 * @retval <0  FIFO is full or @p fifoRef is invalid.
 *
 * @ingroup WISE_FIFO
 */
int8_t wise_fifo_push_char_no_crit(WISE_FIFO_REF fifoRef, uint8_t c);

/**
 * @brief Pop a single byte from FIFO without entering a critical section.
 *
 * This variant is intended for contexts where the caller already
 * handles concurrency or where interrupts are disabled.
 *
 * @param[in]  fifoRef FIFO reference.
 * @param[out] c       Pointer to variable receiving popped byte.
 *
 * @retval 0   Byte popped successfully.
 * @retval <0  FIFO is empty or @p fifoRef is invalid.
 *
 * @ingroup WISE_FIFO
 */
int8_t wise_fifo_pop_char_no_crit(WISE_FIFO_REF fifoRef, uint8_t *c);

/**
 * @brief Pop multiple bytes from FIFO.
 *
 * Attempts to read up to @p len bytes into @p buf.
 *
 * @param[in]  fifoRef FIFO reference.
 * @param[out] buf     Destination buffer.
 * @param[in]  len     Maximum number of bytes to pop.
 *
 * @return Number of bytes actually popped.
 *
 * @ingroup WISE_FIFO
 */
uint16_t wise_fifo_pop(WISE_FIFO_REF fifoRef, uint8_t *buf, uint16_t len);

/**
 * @brief Get number of bytes currently stored in FIFO.
 *
 * @param[in] fifoRef FIFO reference.
 *
 * @return The number of bytes available to read, or a negative value if @p fifoRef is invalid.
 *
 * @ingroup WISE_FIFO
 */
int16_t wise_fifo_get_data_len(WISE_FIFO_REF fifoRef);

/**
 * @brief Check whether the default FIFO (if any) is empty.
 *
 * @note Implementation-specific: this function may check a global or
 *       statically allocated FIFO instance.
 *
 * @retval 1 FIFO is empty.
 * @retval 0 FIFO is not empty.
 * @retval <0 Error or FIFO not initialized.
 *
 * @ingroup WISE_FIFO
 */
int8_t wise_fifo_check_empty(void);

/** @} */ /* end of WISE_FIFO group */

#endif /* __WISE_FIFO_H */

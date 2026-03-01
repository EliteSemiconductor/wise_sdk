/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_nvm_api.h
 * @brief wise_nvm_api.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */


#ifndef __WISE_NVM_API_H
#define __WISE_NVM_API_H

#include "wise_nvm.h"
#include "types.h"

/**
 * @brief Initialize the NVM module.
 *
 * This function initializes the underlying flash access and prepares
 * internal structures for NVM operations. Must be called before using
 * any other NVM API.
 */
void wise_nvm_init(void);

/**
 * @brief Read data from an NVM instance by object ID.
 *
 * @param[in]  handle  NVM instance handle.
 * @param[in]  id      Object ID to read (12-bit, range: 0x001 ~ 0xFFE).
 * @param[out] data    Pointer to the buffer that will store the read data.
 *
 * @retval >=0 Number of bytes successfully read.
 * @retval <0  Error code (e.g., object not found, invalid parameters).
 */
int32_t wise_nvm_read_data(nvm_handle_t handle, uint16_t id, uint8_t *data);

/**
 * @brief Write data to an NVM instance.
 *
 * @param[in] handle  NVM instance handle.
 * @param[in] id      Object ID to write (12-bit, range: 0x001 ~ 0xFFE).
 * @param[in] data    Pointer to the buffer containing the data to write.
 * @param[in] size    Data length in bytes (10-bit, range: 1 ~ 1023).
 *
 * @retval NVM_ACTION_OK   Write successful.
 * @retval NVM_ACTION_FAIL Write failed (e.g., insufficient space, invalid parameters).
 */
int32_t wise_nvm_write_data(nvm_handle_t handle, uint16_t id, uint8_t *data, int size);

/**
 * @brief Delete an object from an NVM instance.
 *
 * @param[in] handle  NVM instance handle.
 * @param[in] id      Object ID to delete (12-bit).
 *
 * @retval NVM_ACTION_OK   Deletion successful.
 * @retval NVM_ACTION_FAIL Deletion failed (e.g., object not found).
 */
int32_t wise_nvm_delete_data(nvm_handle_t handle, uint16_t id);

/**
 * @brief Delete an entire NVM instance.
 *
 * This function invalidates the NVM instance and frees its resources.
 *
 * @param[in] handle NVM instance handle.
 */
void wise_nvm_delete_instance(nvm_handle_t handle);

/**
 * @brief Create a new NVM instance.
 *
 * @param[in] start_addr Start address in flash memory for the instance.
 * @param[in] length     Length of the instance in bytes (must align with erase block size).
 *
 * @return NVM instance handle on success, NULL on failure.
 */
nvm_handle_t wise_nvm_create_instance(uint32_t start_addr, uint32_t length);

#endif

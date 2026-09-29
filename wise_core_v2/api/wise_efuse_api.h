/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */


/**
 * @file wise_efuse_api.h
 * @brief EFUSE APIs.
 *
 * @ingroup WISE_EFUSE
 */

#ifndef _WISE_EFUSE_API_H_
#define _WISE_EFUSE_API_H_

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "types.h"
#include "hal_intf_efuse.h"

/**
 * @defgroup WISE_EFUSE EFUSE
 * @ingroup WISE_CORE_API
 * @brief EFUSE APIs.
 * @{
 */


/**
 * @brief Initialize the eFuse subsystem.
 *
 * Sets up eFuse controller hardware and internal resources. Must be called
 * before using any other eFuse APIs.
 */
void wise_efuse_init(void);

/**
 * @brief Read data from eFuse memory.
 *
 * Reads data from the specified 4-byte aligned eFuse address.
 *
 * @param[in]  addr     eFuse memory address (must be 4-byte aligned).
 * @param[out] buf      Pointer to destination buffer to store read data.
 * @param[in]  byte_len Number of bytes to read (must be non-zero, 4-byte aligned, max 64 bytes).
 *
 * @retval WISE_SUCCESS Data read successfully.
 * @retval WISE_FAIL    Invalid parameters or read operation failed.
 */
WISE_STATUS wise_efuse_read(uint32_t addr, uint8_t *buf, uint32_t byte_len);

/**
 * @brief Write data to eFuse memory.
 *
 * Programs data into the specified 4-byte aligned eFuse address.
 *
 * @param[in] addr     eFuse memory address (must be 4-byte aligned).
 * @param[in] buf      Pointer to data buffer to program into eFuse.
 * @param[in] byte_len Number of bytes to write (must be non-zero, 4-byte aligned, max 64 bytes).
 *
 * @retval WISE_SUCCESS Data programmed successfully.
 * @retval WISE_FAIL    Invalid parameters or write operation failed.
 */
WISE_STATUS wise_efuse_write(uint32_t addr, uint8_t *buf, uint32_t byte_len);

/**
 * @brief Get the unique chip identification number from eFuse.
 *
 * Reads the unique chip ID stored in eFuse memory into the provided buffer.
 *
 * @param[out] id_arr Pointer to array buffer to store the unique chip ID.
 */
void wise_efuse_get_chip_unique(uint8_t* id_arr);


/** @} */ /* end of WISE_EFUSE group */

#endif /* _WISE_EFUSE_API_H_ */

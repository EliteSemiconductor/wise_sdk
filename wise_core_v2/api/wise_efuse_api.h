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


void wise_efuse_init(void);
WISE_STATUS wise_efuse_read(uint32_t addr, uint8_t *buf, uint32_t byte_len);
WISE_STATUS wise_efuse_write(uint32_t addr, uint8_t *buf, uint32_t byte_len);
void wise_efuse_get_chip_unique(uint8_t* id_arr);


/** @} */ /* end of WISE_EFUSE group */

#endif /* _WISE_EFUSE_API_H_ */

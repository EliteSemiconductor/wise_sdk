/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_TRNG_H
#define __HAL_DRV_TRNG_H

#include "esmt_chip_specific.h"
#include "hal_intf_trng.h"
#include <stdint.h>

void hal_drv_trng_sw_reset(void);
uint8_t hal_drv_trng_is_busy(void);
uint32_t hal_drv_trng_get_status(void);
void hal_drv_trng_clear_status(uint32_t status);
void hal_drv_trng_config(void);
void hal_drv_trng_start(void);
uint8_t hal_drv_trng_get_ehr_data(uint32_t *ehr_buf);
#endif /* __HAL_DRV_TRNG_H */

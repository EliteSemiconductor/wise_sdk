/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_EFUSE_H
#define __HAL_INTF_EFUSE_H

#include "esmt_chip_specific.h"
#include "types.h"

#define EFUSE_SN_ADDR 0x008 // for er8130 tc04

typedef enum {
    EFUSE_BASE_MCU = 0,
    EFUSE_BASE_NFC = 1
}EFUSE_BASE_IDX_E;

typedef struct {
    EFUSE_BASE_IDX_E base_idx;
    uint32_t addr;
    uint8_t *buf;
}EFUSE_CONF_S;


void hal_intf_efuse_init(void);
HAL_STATUS hal_intf_efuse_read(EFUSE_CONF_S *efuse_conf);
HAL_STATUS hal_intf_efuse_write(EFUSE_CONF_S *efuse_conf);
void hal_intf_efuse_get_chip_unique(uint8_t* id_arr);

#endif /* __HAL_INTF_EFUSE_H */

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_CACHE_H__
#define __HAL_INTF_CACHE_H__

#include "esmt_chip_specific.h"
#include "types.h"


#define CACHE_WAY0_MASK BIT0
#define CACHE_WAY1_MASK BIT1

typedef struct {
    bool cache_en;
    uint8_t way_mask;
    bool cache_cnt_en;
    uint32_t miss_cnt;
    uint32_t hit_cnt_msb;
    uint32_t hit_cnt_lsb;
} CACHE_CFG_T;

HAL_STATUS hal_intf_cache_config(CACHE_CFG_T *cache_cfg);
HAL_STATUS hal_intf_cache_set_way_enable_mask(uint8_t way_mask);
HAL_STATUS hal_intf_cache_set_enabled(bool enabled);
HAL_STATUS hal_intf_cache_set_counter_enabled(bool enabled);
HAL_STATUS hal_intf_cache_get_counter_info(CACHE_CFG_T *cache_cfg);
HAL_STATUS hal_intf_cache_reset(void);
HAL_STATUS hal_intf_cache_invalidate(void);
bool hal_intf_cache_is_enabled(void);

#endif /* __HAL_INTF_CACHE_H__ */

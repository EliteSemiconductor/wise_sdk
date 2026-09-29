/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_CACHE_H__
#define __HAL_DRV_CACHE_H__

#include "hdl/cache_er8130.h"
#include "hal_intf_cache.h"

HAL_STATUS hal_drv_cache_config(CACHE_CFG_T *cache_cfg);
HAL_STATUS hal_drv_cache_set_way_enable_mask(uint8_t way_mask);
HAL_STATUS hal_drv_cache_set_enabled(bool enabled);
HAL_STATUS hal_drv_cache_set_counter_enabled(bool enabled);
HAL_STATUS hal_drv_cache_reset(void);
HAL_STATUS hal_drv_cache_invalidate(void);
HAL_STATUS hal_drv_cache_get_counter_info(CACHE_CFG_T *cache_cfg);
bool hal_drv_cache_is_enabled(void);

#endif


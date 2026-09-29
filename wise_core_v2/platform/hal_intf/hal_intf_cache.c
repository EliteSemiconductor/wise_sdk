/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_cache.h"
#include "drv/hal_drv_cache.h"

HAL_STATUS hal_intf_cache_config(CACHE_CFG_T *cache_cfg)
{
    return hal_drv_cache_config(cache_cfg);
}

HAL_STATUS hal_intf_cache_set_way_enable_mask(uint8_t way_mask)
{
    return hal_drv_cache_set_way_enable_mask(way_mask);
}

HAL_STATUS hal_intf_cache_set_enabled(bool enabled)
{
    return hal_drv_cache_set_enabled(enabled);
}

HAL_STATUS hal_intf_cache_set_counter_enabled(bool enabled)
{
    return hal_drv_cache_set_counter_enabled(enabled);
}

HAL_STATUS hal_intf_cache_get_counter_info(CACHE_CFG_T *cache_cfg)
{
    return hal_drv_cache_get_counter_info(cache_cfg);
}

HAL_STATUS hal_intf_cache_reset(void)
{
    return hal_drv_cache_reset();
}

HAL_STATUS hal_intf_cache_invalidate(void)
{
    return hal_drv_cache_invalidate();
}

bool hal_intf_cache_is_enabled(void)
{
    return hal_drv_cache_is_enabled();
}

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_drv_cache.h"
#include "util_debug_log.h"

HAL_STATUS hal_drv_cache_config(CACHE_CFG_T *cache_cfg)
{
    HAL_STATUS status = HAL_NO_ERR;

    cache_set_enabled(DISABLE);   /* disable so the reconfig is safe from flash */
    cache_set_counter_enabled(cache_cfg->cache_cnt_en);
    cache_set_way_enable_mask(cache_cfg->way_mask);

    // cache_invalidate for avoiding cache crash
    if (cache_invalidate() == HAL_ERR) {
        WISE_LOG_ERR("cache invalidate occurs timeout\n");
        status = HAL_ERR;
    }

    cache_set_enabled(cache_cfg->cache_en); /* apply the caller's request */

    return status;
}

HAL_STATUS hal_drv_cache_set_way_enable_mask(uint8_t way_mask)
{
    HAL_STATUS status = HAL_NO_ERR;
    bool was_enabled = cache_is_enabled();

    cache_set_enabled(DISABLE);
    cache_set_way_enable_mask(way_mask);

    // cache_invalidate for avoiding cache crash
    if (cache_invalidate() == HAL_ERR) {
        WISE_LOG_ERR("cache invalidate occurs timeout\n");
        status = HAL_ERR;
    }

    cache_set_enabled(was_enabled);

    return status;
}

HAL_STATUS hal_drv_cache_set_enabled(bool enabled)
{
    HAL_STATUS status = HAL_NO_ERR;

    cache_set_enabled(enabled);

    // cache_invalidate for avoiding cache crash
    if (cache_invalidate() == HAL_ERR) {
        WISE_LOG_ERR("cache invalidate occurs timeout\n");
        status = HAL_ERR;
    }

    return status;
}

HAL_STATUS hal_drv_cache_set_counter_enabled(bool enabled)
{
    /* CNT_EN only gates the hit/miss statistics counters; it changes neither the
       cache geometry nor line validity, so no invalidate is required. */
    cache_set_counter_enabled(enabled);

    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_cache_reset(void)
{
    HAL_STATUS status = HAL_NO_ERR;

    bool was_enabled = cache_is_enabled();

    cache_set_enabled(DISABLE);
    cache_reset();

    // cache_invalidate for avoiding cache crash
    if (cache_invalidate() == HAL_ERR) {
        WISE_LOG_ERR("cache invalidate occurs timeout\n");
        status = HAL_ERR;
    }

    cache_set_enabled(was_enabled);

    return status;
}

HAL_STATUS hal_drv_cache_invalidate(void)
{
    if (cache_invalidate() == HAL_ERR) {
        WISE_LOG_ERR("cache invalidate occurs timeout\n");
        return HAL_ERR;
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_cache_get_counter_info(CACHE_CFG_T *cache_cfg)
{
    cache_cfg->hit_cnt_lsb = cache_get_hit_count_lsb();
    cache_cfg->hit_cnt_msb = cache_get_hit_count_msb();
    cache_cfg->miss_cnt = cache_get_miss_count();

    return HAL_NO_ERR;
}

bool hal_drv_cache_is_enabled(void)
{
    return cache_is_enabled();
}


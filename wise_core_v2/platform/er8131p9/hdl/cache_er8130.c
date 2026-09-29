#include "cache_er8130.h"
#include "util_debug_log.h"

RAM_TEXT static void _cache_disable_settle_delay(void)
{
    volatile uint32_t i;

    for (i=0; i<200; i++) {
        __NOP();
    }
}

RAM_TEXT void cache_set_way_enable_mask(uint8_t way_mask)
{
    /*
        bit0 = channel 0 (way0)
        bit1 = channel 1 (way1)
        bit2 = channel 2 (way2)
        bit3 = channel 3 (way3)

        [0] = enable this channel,
        [1] = disable this channel

        ER8130 support way0 and way1 only
    */

    uint32_t val;
    uint32_t mask = ~(way_mask);

    val = REG_R32(CACHE_CONFIG_ADDR);
    val = (val & ~(CACHE_WAY_DIS_MASK)) |
          ((mask << CACHE_WAY_DIS_POS) & CACHE_WAY_DIS_MASK);
    REG_W32(CACHE_CONFIG_ADDR, val);
}

RAM_TEXT void cache_set_enabled(bool enabled)
{
    uint32_t val;

    val = REG_R32(CACHE_CONFIG_ADDR);
    val = (val & ~(CACHE_CACHE_EN_MASK)) |
          ((enabled << CACHE_CACHE_EN_POS) & CACHE_CACHE_EN_MASK);
    REG_W32(CACHE_CONFIG_ADDR, val);
}

RAM_TEXT void cache_set_counter_enabled(bool enabled)
{
    uint32_t val;

    val = REG_R32(CACHE_CONFIG_ADDR);
    val = (val & ~(CACHE_CNT_EN_MASK)) |
          ((enabled << CACHE_CNT_EN_POS) & CACHE_CNT_EN_MASK);
    REG_W32(CACHE_CONFIG_ADDR, val);
}

RAM_TEXT void cache_reset(void)
{
    //this function will reset cache hardware
    uint32_t val;

    //toggle bit 14
    val = REG_R32(CACHE_CONFIG_ADDR);
    val = (val & ~(CACHE_RST_MASK)) |
          ((1 << CACHE_RST_POS) & CACHE_RST_MASK);
    REG_W32(CACHE_CONFIG_ADDR, val);

    val = REG_R32(CACHE_CONFIG_ADDR);
    val = (val & ~(CACHE_RST_MASK)) |
          ((0 << CACHE_RST_POS) & CACHE_RST_MASK);
    REG_W32(CACHE_CONFIG_ADDR, val);
}

RAM_TEXT HAL_STATUS cache_invalidate(void)
{
    HAL_STATUS status = HAL_NO_ERR;
    uint32_t val;
    uint32_t cnt = 0;
    bool was_enabled = cache_is_enabled();

    // step1: disable cache
    cache_set_enabled(DISABLE);

    //system need to wait a time for disabing cache
    _cache_disable_settle_delay();

    // step2: set invalid en
    val = REG_R32(CACHE_CONFIG_ADDR);
    val = (val & ~(CACHE_INVALID_EN_MASK)) |
          ((ENABLE << CACHE_INVALID_EN_POS) & CACHE_INVALID_EN_MASK);
    REG_W32(CACHE_CONFIG_ADDR, val);

    // step3: wait invalid done
    while (!(REG_R32(CACHE_CONFIG_ADDR) & CACHE_INVALID_DONE_MASK)) {
        if (cnt >= 10000) {
            status = HAL_ERR;
            break;
        }
        cnt++;
    }

    // step4: clear the invalid_en and invalid_done
    val = REG_R32(CACHE_CONFIG_ADDR);
    val &= ~(CACHE_INVALID_EN_MASK|CACHE_INVALID_DONE_MASK);
    REG_W32(CACHE_CONFIG_ADDR, val);

    // step5: restore the original cache status
    cache_set_enabled(was_enabled);

    return status;
}

uint32_t cache_get_hit_count_msb(void)
{
    return (REG_R32(CACHE_HIT_CNTR_MSB_ADDR));
}

uint32_t cache_get_hit_count_lsb(void)
{
    return (REG_R32(CACHE_HIT_CNTR_LSB_ADDR));
}

uint32_t cache_get_miss_count(void)
{
    return (REG_R32(CACHE_MISS_CNTR_ADDR));
}

RAM_TEXT bool cache_is_enabled(void)
{
    bool status;

    if (REG_R32(CACHE_CONFIG_ADDR) & (CACHE_CACHE_EN_MASK))
        status = ENABLE;
    else
        status = DISABLE;

    return status;
}


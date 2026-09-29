#ifndef __CACHE_ER8130_H__ 
#define __CACHE_ER8130_H__ 

#include "esmt_chip_specific.h"
#include "types.h"


void cache_set_way_enable_mask(uint8_t way_mask);
void cache_set_enabled(bool enabled);
void cache_set_counter_enabled(bool enabled);
uint32_t cache_get_hit_count_lsb(void);
uint32_t cache_get_hit_count_msb(void);
uint32_t cache_get_miss_count(void);
void cache_reset(void);
HAL_STATUS cache_invalidate(void);
bool cache_is_enabled(void);





#endif


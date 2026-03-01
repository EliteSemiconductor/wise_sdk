/**
 * @file wise_timer_hub.h
 * @brief wise_timer_hub.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */

#ifndef __WISE_TIMER_HUB_H 
#define __WISE_TIMER_HUB_H

#include <stdint.h>

#include "es_platform_components.h"

#include "wise_core.h"
#include "wise_wutmr_api.h"
#include "wise_gptmr_api.h"

#define WISE_TIMER_HUB_CH_INVALID       (-1)

#ifndef WISE_TIMER_HUB_CH_NUM
#define WISE_TIMER_HUB_CH_NUM           4
#endif


typedef WISE_WUTMR_EVT_CALLBACK_T TIMER_HUB_CB_T;
typedef WISE_WUTMR_TYPE_T TIMER_HUB_MODE_T;


int32_t wise_timer_hub_init();
int32_t wise_timer_hub_alloc_ch(TIMER_HUB_CB_T cb, void* userData);
void wise_timer_hub_free_ch(int ch);
int32_t wise_timer_hub_start_ch(int ch, TIMER_HUB_MODE_T mode, uint32_t periodMs);
void wise_timer_hub_stop_ch(int ch);

#endif
  

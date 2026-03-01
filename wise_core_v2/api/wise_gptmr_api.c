/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_gptmr_api.h"
#include "wise_core.h"

#define IS_VALID_TIMER_CHANNEL(channel)         ((channel) < CHIP_TIMER_CHANNEL_NUM)

/* kevinyang, 20250930, replaced with array index
#define GET_TIMER_MODULE(timer_channel)                                        \
    ((timer_channel) == 0   ? GPTMR0_MODULE                                    \
     : (timer_channel) == 1 ? GPTMR1_MODULE                                    \
     : (timer_channel) == 2 ? GPTMR2_MODULE                                    \
     : (timer_channel) == 3 ? GPTMR3_MODULE                                    \
     : (timer_channel) == 4 ? GPTMR4_MODULE                                    \
                            : GPTMR5_MODULE)
*/

typedef struct
{
    CALLBACK_T callback;
    void *pCtx;
} ST_TIMER_CALLBACKS_T;

static uint8_t timerInited = 0;
static ST_TIMER_CALLBACKS_T timerCallbacks[CHIP_TIMER_CHANNEL_NUM];
static const uint32_t TIMER_MODULES[] = {GPTMR0_MODULE, GPTMR1_MODULE, GPTMR2_MODULE, GPTMR3_MODULE, GPTMR4_MODULE, GPTMR5_MODULE};

static void _hal_timer_isr(void *context, uint8_t channelIdx);

static void _hal_timer_isr(void *context, uint8_t channelIdx)
{
    if(timerCallbacks[channelIdx].callback)
    {
        (timerCallbacks[channelIdx].callback)(timerCallbacks[channelIdx].pCtx, channelIdx);
    }
}

WISE_STATUS wise_timer_init()
{
    int i;
    
    if(timerInited)
    {
        return WISE_SUCCESS;
    }
    
    memset(timerCallbacks, 0, sizeof(timerCallbacks));
    timerInited = 1;

    for(i = 0; i < GPTMR_MAX_EVENTS; i++)
    {
#ifdef CHIP_TICK_TIMER_CHANNEL
        if(i == CHIP_TICK_TIMER_CHANNEL)
            continue;
#endif
        hal_intf_gptmr_register_callback(i, _hal_timer_isr, NULL);
    }
    
    return WISE_SUCCESS;
}

WISE_STATUS wise_timer_deinit()
{
    int i;

    for(i = 0; i < CHIP_TIMER_CHANNEL_NUM; i++)
    {
#ifdef CHIP_TICK_TIMER_CHANNEL
        if(i == CHIP_TICK_TIMER_CHANNEL)
            continue;
#endif

        hal_intf_module_clk_disable(TIMER_MODULES[i]);
    }

    for(i = 0; i < GPTMR_MAX_EVENTS; i++)
    {
#ifdef CHIP_TICK_TIMER_CHANNEL
        if(i == CHIP_TICK_TIMER_CHANNEL)
            continue;
#endif
        hal_intf_gptmr_unregister_callback(i);
    }
    
    timerInited = 0;
    return WISE_SUCCESS;
}

bool wise_timer_is_enabled(uint8_t timer_channel)
{
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) {
        return WISE_INVALID_INDEX;
    }
    return hal_intf_gptmr_is_enabled(timer_channel);
}

WISE_STATUS wise_timer_config(uint8_t timer_channel, WISE_TIMER_CTRL_T *config)
{
    TIMER_TYPE_T timerType;
    
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) {
        return WISE_FAIL;
    }

#ifdef CHIP_TICK_TIMER_CHANNEL
    if(timer_channel == CHIP_TICK_TIMER_CHANNEL)
    {
        debug_print("timer ch %d is internal used\n", timer_channel);
        return WISE_FAIL;
    }
#endif
    
    if (hal_intf_module_clk_is_enabled(TIMER_MODULES[timer_channel]) == DISABLE) {
        hal_intf_module_clk_enable(TIMER_MODULES[timer_channel]);
    }

    if(config->start_offset >= WISE_TIMER_MAX_INTERVAL)
        return WISE_FAIL;

    if(config->interval != WISE_TIMER_COUNTER_MAX)
    {
        if(config->interval >= WISE_TIMER_MAX_INTERVAL)
        {
            //out of range
            return WISE_FAIL;
        }
    }

    wise_timer_stop(timer_channel);

    if(config->type == WISE_TIMER_PERIODIC)
        timerType = TIMER_PERIODIC;
    else
        timerType = TIMER_ONE_SHOT;
    
    hal_intf_gptmr_config(timer_channel, 
                            timerType, 
                            config->interrupt_enable,
                            config->start_offset,
                            config->interval);
    return WISE_SUCCESS;
}

WISE_STATUS wise_timer_start(uint8_t timer_channel)
{
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) {
        return WISE_FAIL;
    }

#ifdef CHIP_TICK_TIMER_CHANNEL
    if(timer_channel == CHIP_TICK_TIMER_CHANNEL)
    {
        return WISE_FAIL;
    }
#endif

    hal_intf_gptmr_start(timer_channel);

    return WISE_SUCCESS;
}

uint32_t wise_timer_get_counter(uint32_t timer_channel)
{
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) 
    {
        return 0;
    }

    return hal_intf_gptmr_get_cnt(timer_channel);
}

WISE_STATUS wise_timer_enable_int(uint32_t timer_channel)
{
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) 
    {
        return WISE_FAIL;
    }

    hal_intf_gptmr_enable_int(timer_channel);
    
    return WISE_SUCCESS;
}

uint32_t wise_timer_get_counter_max(uint32_t timer_channel)
{
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) 
    {
        return 0;
    }

    return hal_intf_gptmr_get_cnt_max(timer_channel);
};

WISE_STATUS wise_timer_disable_int(uint32_t timer_channel)
{
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) 
    {
        return WISE_FAIL;
    }

    hal_intf_gptmr_disable_int(timer_channel);
    
    return WISE_SUCCESS;
}

WISE_STATUS wise_timer_stop(uint8_t timer_channel)
{
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) {
        return WISE_INVALID_INDEX;
    }

#ifdef CHIP_TICK_TIMER_CHANNEL
    if(timer_channel == CHIP_TICK_TIMER_CHANNEL)
    {
        return WISE_FAIL;
    }
#endif


    hal_intf_gptmr_stop(timer_channel);

    return WISE_SUCCESS;
}

WISE_STATUS wise_timer_register_callback(uint8_t timer_channel, CALLBACK_T cb, void *context)
{
    if (!IS_VALID_TIMER_CHANNEL(timer_channel)) {
        return WISE_INVALID_INDEX;
    }

    timerCallbacks[timer_channel].callback = cb;
    timerCallbacks[timer_channel].pCtx = context;
    
    return WISE_SUCCESS;
}

WISE_STATUS wise_timer_close(uint8_t timer_channel)
{
#ifdef CHIP_TICK_TIMER_CHANNEL
    if(timer_channel == CHIP_TICK_TIMER_CHANNEL)
    {
        return WISE_FAIL;
    }
#endif

    hal_intf_gptmr_stop(timer_channel);

    timerCallbacks[timer_channel].callback = NULL;
    timerCallbacks[timer_channel].pCtx = NULL;

    hal_intf_module_clk_disable(TIMER_MODULES[timer_channel]);

    return WISE_SUCCESS;
}

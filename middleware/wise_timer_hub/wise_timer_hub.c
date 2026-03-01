#include "es_platform_components.h"

#if (defined MIDDLEWARE_WISE_TIMER_HUB) && (MIDDLEWARE_WISE_TIMER_HUB == 1)
#include "wise_timer_hub.h"
#include "wise_wutmr_api.h"
#include "wise_gptmr_api.h"

enum
{
    E_TIMER_HUB_STATE_NONE = 0,
    E_TIMER_HUB_STATE_USED,
    E_TIMER_HUB_STATE_RUNNING,
    E_TIMER_HUB_STATE_ACTIVE,
};

typedef struct 
{
    uint8_t state;
    TIMER_HUB_MODE_T mode;
    uint32_t periodMs;
    uint32_t periodTick;
    uint32_t startTick;
    uint32_t nextExpire;
    TIMER_HUB_CB_T cb;
    void *user;
    volatile uint8_t pending;
} ST_TIMER_HUB_CH_T;

static uint8_t tmrHubInited = 0;
static ST_TIMER_HUB_CH_T timerBank[WISE_TIMER_HUB_CH_NUM];
static int wutmrActiveCh = -1;

static void _wutmr_int_handler(void* data);
static void _wutmr_reschedule_next();
static void _wutmr_ch_expired(int ch, uint32_t curTick);
static void _timer_hub_debug_info();

static inline uint32_t tick_diff(uint32_t from, uint32_t to) 
{
    return (uint32_t)(to - from);
}

static void _timer_hub_debug_info()
{
    uint32_t now = wise_wutmr_get_counter();
    int i;

    debug_print("tmrhub now=%08x\n", now);
    for(i = 0; i < WISE_TIMER_HUB_CH_NUM; i++)
    {
        debug_print(" ch=%d m=%d st=%d exp=%08x\n", i, timerBank[i].mode, timerBank[i].state, timerBank[i].nextExpire);
    }
}

static void _wutmr_ch_expired(int ch, uint32_t curTick)
{
    ST_TIMER_HUB_CH_T* pActCh = &timerBank[ch];
    
    if(pActCh->cb)
    {
        (pActCh->cb)(pActCh->user);
    }

    pActCh->startTick = curTick;
    pActCh->nextExpire = pActCh->startTick + pActCh->periodTick;

    if(pActCh->mode == WISE_WUTMR_ONE_SHOT)
    {
        pActCh->state = E_TIMER_HUB_STATE_USED;
    }
    else
    {        
        pActCh->state = E_TIMER_HUB_STATE_RUNNING;
    }
}

static void _wutmr_int_handler(void* data)
{
    uint32_t now = wise_wutmr_get_counter();

    //printf("-");
    
    if(wutmrActiveCh == -1)
    {
        debug_print("bug!! invalid act ch\n");
    }
    else
    {
        _wutmr_ch_expired(wutmrActiveCh, now);
    }
    
    _wutmr_reschedule_next();
}

static void _timer_ch_reset(int ch)
{
    timerBank[ch].state = E_TIMER_HUB_STATE_NONE;
    timerBank[ch].mode = WISE_WUTMR_ONE_SHOT;
    timerBank[ch].periodMs = 0;
    timerBank[ch].periodTick = 0;
    timerBank[ch].nextExpire = 0;
    timerBank[ch].cb = NULL;
    timerBank[ch].user = NULL;
    timerBank[ch].pending = 0;
}

int32_t wise_timer_hub_init()
{
    int i;
    CORE_DECLARE_IRQ_STATE;

    CORE_ENTER_CRITICAL();
    for(i = 0; i < WISE_TIMER_HUB_CH_NUM; i++)
    {
        _timer_ch_reset(i);
    }
    CORE_EXIT_CRITICAL();

    wise_wutmr_register_callback(_wutmr_int_handler, NULL);
    tmrHubInited = 1;

    wise_wutmr_enable_int();
    
    return WISE_SUCCESS;
}

int32_t wise_timer_hub_alloc_ch(TIMER_HUB_CB_T cb, void* userData)
{
    int32_t newCh = WISE_TIMER_HUB_CH_INVALID;
    CORE_DECLARE_IRQ_STATE;
    int i;

    if(!tmrHubInited)
        return WISE_TIMER_HUB_CH_INVALID;
    
    CORE_ENTER_CRITICAL();
    for(i = 0; i < WISE_TIMER_HUB_CH_NUM; i++)
    {
        if(timerBank[i].state == E_TIMER_HUB_STATE_NONE)
        {
            _timer_ch_reset(i);

            timerBank[i].cb = cb;
            timerBank[i].user = userData;
            timerBank[i].state = E_TIMER_HUB_STATE_USED;
            newCh = i;
            break;
        }
    }
    CORE_EXIT_CRITICAL();
    
    return newCh;
}

void wise_timer_hub_free_ch(int ch)
{
    CORE_DECLARE_IRQ_STATE;

    CORE_ENTER_CRITICAL();
    if(timerBank[ch].state == E_TIMER_HUB_STATE_ACTIVE)
    {
        wise_wutmr_stop();
        _wutmr_reschedule_next();
    }
    
    timerBank[ch].state = E_TIMER_HUB_STATE_NONE;
    CORE_EXIT_CRITICAL();
}

int32_t wise_timer_hub_start_ch(int ch, TIMER_HUB_MODE_T mode, uint32_t periodMs)
{
    int32_t result = WISE_FAIL;
    CORE_DECLARE_IRQ_STATE;

    if(!tmrHubInited)
        return WISE_FAIL;

    if(timerBank[ch].state == E_TIMER_HUB_STATE_NONE)
        return WISE_FAIL;
    
    CORE_ENTER_CRITICAL();
    
    //if(timerBank[ch].state == E_TIMER_HUB_STATE_ACTIVE)
    if(wutmrActiveCh != -1)
    {
        wise_wutmr_stop();
        timerBank[wutmrActiveCh].state = E_TIMER_HUB_STATE_RUNNING;
        wutmrActiveCh = -1;
    }

    timerBank[ch].mode = mode;
    timerBank[ch].periodMs = periodMs;
    timerBank[ch].periodTick = wise_wutmr_ms_to_clk(periodMs);
    timerBank[ch].startTick = wise_wutmr_get_counter();
    timerBank[ch].nextExpire = timerBank[ch].startTick + timerBank[ch].periodTick;

    timerBank[ch].state = E_TIMER_HUB_STATE_RUNNING;

    _wutmr_reschedule_next();
    
    CORE_EXIT_CRITICAL();

    return result;
}

void wise_timer_hub_stop_ch(int ch)
{
    CORE_DECLARE_IRQ_STATE;

    if(!tmrHubInited)
        return WISE_FAIL;

    if(timerBank[ch].state <= E_TIMER_HUB_STATE_USED)
        return WISE_FAIL;

    CORE_ENTER_CRITICAL();

    if(timerBank[ch].state == E_TIMER_HUB_STATE_ACTIVE)
    {
        wise_wutmr_stop();
    }

    timerBank[ch].state = E_TIMER_HUB_STATE_USED;

    _wutmr_reschedule_next();
    
    CORE_EXIT_CRITICAL();
}

static void _wutmr_reschedule_next()
{
    int i;
    uint32_t now = wise_wutmr_get_counter();
    int closestCh = -1;
    uint32_t closestTick = 0;
    
    for(i = 0; i < WISE_TIMER_HUB_CH_NUM; i++)
    {
        if(timerBank[i].state == E_TIMER_HUB_STATE_RUNNING)
        {
            uint32_t diff = tick_diff(now, timerBank[i].nextExpire);

            timerBank[i].state == E_TIMER_HUB_STATE_RUNNING; //set back to running here, pick up next active later

            if(diff >= 0xff000000)
            {
                _wutmr_ch_expired(i, now);
                diff = tick_diff(now, timerBank[i].nextExpire);

                if(diff >= 0xff000000)
                {
                    printf("WTF!!!\n");
                }
            }
            
            if(closestCh == -1)
            {
                closestCh = i;
                closestTick = diff;
            }
            else
            {
                if(diff <= closestTick)
                {
                    closestCh = i;
                    closestTick = diff;
                }
            }
        }
    }

    if(closestCh == -1)
    {
        //debug_print("[WTH] bug!!\n");
        wutmrActiveCh = -1;
    }
    else if(closestCh != -1)
    {
        wise_wutmr_stop();

        wutmrActiveCh = closestCh;
        timerBank[wutmrActiveCh].state = E_TIMER_HUB_STATE_ACTIVE;
        //wise_wutmr_start(timerBank[wutmrActiveCh].periodMs, WISE_WUTMR_ONE_SHOT); //always use one shot mode, sw has to reschedule next
        //printf("+%d", closestTick);
        wise_wutmr_start_tick(closestTick, WISE_WUTMR_ONE_SHOT);
    }
}

#endif

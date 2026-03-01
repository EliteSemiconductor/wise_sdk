/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if MIDDLEWARE_WISE_SYSTEM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wise_core.h"
#include "wise.h"

#include "wise_tick_api.h"

// #include "wise_timer_api.h"

#define SCHLR_MAX_EVENT_NUM 8 // max=127
#define SCHLR_MAX_POLLING_NUM 4

#define SCHLR_STS_INITIALIZED 0x01
#define SCHLR_STS_IDLING 0x02
#define SCHLR_STS_RELOAD_EVENT 0x04
#define SCHLR_STS_CLOCK_OVERFLOW 0x08

typedef struct {
    uint32_t round;
    uint32_t tu;
    uint32_t ms;
} SCHLR_TIME_T;

typedef struct {
    SCHLR_TIME_T targetTime;
    uint32_t periodTick;

    SCHEDULER_CALLBACK proc;
    void *pData;
} SCHLR_EVENT_T;

typedef struct {
    uint8_t status;

    // E_SCH_MODE_T mode;

    int8_t nextEventIdx;
    SCHLR_TIME_T nextEvtTime;
    SCHLR_TIME_T prevEvtTime;

    uint32_t idleStart;
    uint8_t awakenCount;

    SCHLR_EVENT_T eventBank[SCHLR_MAX_EVENT_NUM];
} SCHLR_T;

static SCHLR_T stSchCtrl       = {0};
static SCHLR_TIME_T systemTick = {1, 0, 0}; // round start from 1

static void updateTimeBase();
static void schlrGetTime(SCHLR_TIME_T *outTime);
static int isTimeup(SCHLR_TIME_T checkTime);
static void processEvent(int8_t index);
static void loadNextEvent();
static int8_t findEmptySlot();
static void timeIncrease(SCHLR_TIME_T *time, uint32_t increaseTU);

void _wise_schlr_proc();
uint32_t wiseSchlrNextEventTime();

void schlrGetTime(SCHLR_TIME_T *outTime)
{
    updateTimeBase();
    memcpy(outTime, &systemTick, sizeof(SCHLR_TIME_T));
}

static int8_t findEmptySlot()
{
    int i;
    int8_t foundIndex = WISE_INVALID_INDEX;

    for (i = 0; i < SCHLR_MAX_EVENT_NUM; i++) {
        SCHLR_EVENT_T *pEvent = &stSchCtrl.eventBank[i];

        if (pEvent->proc == NULL) {
            foundIndex = i;
            break;
        }
    }

    return foundIndex;
}

int isTimeup(SCHLR_TIME_T checkTime)
{
    if ((systemTick.round > checkTime.round) ||
        ((checkTime.round == systemTick.round) &&
         (checkTime.tu <= systemTick.tu))) {
        return 1;
    }

    return 0;
}

static void timeIncrease(SCHLR_TIME_T *time, uint32_t increaseTU)
{
    uint32_t resultTU;

    if (!time) {
        return;
    }

    resultTU = time->tu + increaseTU;
    if (resultTU < time->tu) {
        time->round++;
    }

    time->tu = resultTU;
}

static void processEvent(int8_t index)
{
    SCHLR_EVENT_T *pEvent = &stSchCtrl.eventBank[index];

    // APTX_INFO("p %d %p\n", index, pEvent->proc);

    (pEvent->proc)(pEvent->pData);

    stSchCtrl.nextEventIdx = WISE_INVALID_INDEX;

    memcpy(&stSchCtrl.prevEvtTime, &stSchCtrl.nextEvtTime,
           sizeof(SCHLR_TIME_T));
    memset(&stSchCtrl.nextEvtTime, 0, sizeof(SCHLR_TIME_T));

    if (pEvent->periodTick) {
        uint32_t nextTick = pEvent->targetTime.tu + pEvent->periodTick;

        if (pEvent->targetTime.tu > nextTick) {
            pEvent->targetTime.round++;
        }

        pEvent->targetTime.tu = nextTick;
    } else if (pEvent->proc) {
        memset(pEvent, 0, sizeof(SCHLR_EVENT_T));
    }

    loadNextEvent();
}

static void loadNextEvent()
{
    int i;
    int8_t closestIndex    = WISE_INVALID_INDEX;
    uint32_t closestTime   = 0;
    int8_t closestRBIdx    = WISE_INVALID_INDEX;
    uint32_t closestRBTime = 0;
    // ST_APTX_TIME_T nowTick = systemTick;

    stSchCtrl.status &= ~SCHLR_STS_RELOAD_EVENT;

    for (i = 0; i < SCHLR_MAX_EVENT_NUM; i++) {
        SCHLR_EVENT_T *pEvent = &stSchCtrl.eventBank[i];

        if (pEvent->proc) {
            if (pEvent->targetTime.round == stSchCtrl.prevEvtTime.round) {
                if (pEvent->targetTime.tu >= stSchCtrl.prevEvtTime.tu) {
                    if ((closestIndex == WISE_INVALID_INDEX) ||
                        (pEvent->targetTime.tu < closestTime)) {
                        closestIndex = i;
                        closestTime  = pEvent->targetTime.tu;
                    }
                }
            } else if (pEvent->targetTime.round > stSchCtrl.prevEvtTime.round) {
                if ((closestRBIdx == WISE_INVALID_INDEX) ||
                    (pEvent->targetTime.tu < closestRBTime)) {
                    closestRBIdx  = i;
                    closestRBTime = pEvent->targetTime.tu;
                }
            } else {
                debug_print("WTF\n");
            }
        }
    }

    if (closestIndex != WISE_INVALID_INDEX) {
        stSchCtrl.nextEventIdx      = closestIndex;
        stSchCtrl.nextEvtTime.round = stSchCtrl.prevEvtTime.round;
        stSchCtrl.nextEvtTime.tu    = closestTime;

        stSchCtrl.status    &= ~SCHLR_STS_IDLING;
        stSchCtrl.idleStart  = 0;
    } else if (closestRBIdx != WISE_INVALID_INDEX) {
        // printf("N2 %d %lu\n", closestRBIdx, closestRBTime);

        stSchCtrl.nextEventIdx      = closestRBIdx;
        stSchCtrl.nextEvtTime.round = stSchCtrl.prevEvtTime.round + 1;
        stSchCtrl.nextEvtTime.tu    = closestRBTime;

        stSchCtrl.status    &= ~SCHLR_STS_IDLING;
        stSchCtrl.idleStart  = 0;
    } else {
        schlrGetTime(&stSchCtrl.prevEvtTime);
    }
}

void updateTimeBase()
{
    static uint32_t lastSync    = 0;
    static uint32_t truncated   = 0;
    static uint32_t truncatedMs = 0;
    static int8_t entry         = 0;
    uint32_t now                = 0;
    uint32_t timelapsed         = 0;
    uint32_t nowTu              = 0;
    uint32_t nowMs              = 0;

    CORE_DECLARE_IRQ_STATE;

    if (!CORE_IS_IN_ISR()) {
        CORE_ENTER_CRITICAL();
    }
    if (entry) {
        if (!CORE_IS_IN_ISR()) {
            CORE_EXIT_CRITICAL();
        }
        return;
    }
    entry = 1;

    now        = wise_tick_get_counter();
    timelapsed = time_diff(lastSync, now);
    if (timelapsed >= 16000000) {
        //debug_print("tick glitch!! lastSyn=%08lx now=%08lx\n", lastSync, now);
    }

    // update tu
    nowTu      = systemTick.tu;
    nowTu     += (CLK_TO_US(timelapsed) / 100);
    truncated += (timelapsed % (CLK_PER_US * 100));
    if (truncated >= (CLK_PER_US * 100)) {
        nowTu     += (truncated / (CLK_PER_US * 100));
        truncated  = (truncated % (CLK_PER_US * 100));
    }

    // update ms
    nowMs        = systemTick.ms;
    nowMs       += CLK_TO_MS(timelapsed);
    truncatedMs += (timelapsed % CLK_PER_MS);
    if (truncatedMs >= CLK_PER_MS) {
        nowMs       += (truncatedMs / CLK_PER_MS);
        truncatedMs  = (truncatedMs % CLK_PER_MS);
    }

    if (nowTu < systemTick.tu) // TU overflow
    {
        uint32_t bkp = systemTick.round;

        systemTick.round++;

        if (bkp > systemTick.round) // round overflow
        {
            debug_print("Reborn!!\n");
        }
    }

    systemTick.tu = nowTu;
    systemTick.ms = nowMs;

    lastSync = now;

    entry = 0;

    if (!CORE_IS_IN_ISR()) {
        CORE_EXIT_CRITICAL();
    }
}

int8_t wise_schlr_init()
{
    memset(&stSchCtrl, 0, sizeof(stSchCtrl));

    updateTimeBase();

    stSchCtrl.nextEventIdx = WISE_INVALID_INDEX;
    schlrGetTime(&stSchCtrl.prevEvtTime);
    // stSchCtrl.mode = mode;
    // stSchCtrl.awakenCount = 1;
    stSchCtrl.status = SCHLR_STS_INITIALIZED;

    return WISE_SUCCESS;
}

uint32_t sysTimeDiff(SCHLR_TIME_T from, SCHLR_TIME_T to)
{
    if (from.round == to.round) {
        if (to.tu > from.tu) {
            return (from.tu - to.tu);
        } else {
            return 0;
        }
    } else if (from.round < to.round) {
        return (to.tu + (0xffffffff - from.tu));
    } else {
        return 0;
    }
}

uint32_t wiseSchlrNextEventTime() // unit: us
{
    uint32_t sleepInterval = 0xffffffff;

    if (stSchCtrl.nextEventIdx != WISE_INVALID_INDEX) {
#define MIN_SLEEP_TIME 200
#define SLEEP_WAKE_OVERHEAD 50

        if (isTimeup(stSchCtrl.nextEvtTime)) {
            processEvent(stSchCtrl.nextEventIdx);
            // goto finish;
        }

        sleepInterval = SCHLR_UNIT_TO_US(
            time_diff(systemTick.tu, stSchCtrl.nextEvtTime.tu));
    } else {
        // no jobs to do, sleep?
#define SCHEDULE_IDLE_SLEEP_THRESHOLD 3000  // ms
#define SCHEDULE_IDLE_SLEEP_INTERVAL 300000 // ms

        if (!(stSchCtrl.status & SCHLR_STS_IDLING)) {
            stSchCtrl.status    |= SCHLR_STS_IDLING;
            stSchCtrl.idleStart  = wise_get_tu();
        } else {
            uint32_t current = wise_get_tu();
            if (time_diff(stSchCtrl.idleStart, current) >
                MS_TO_SCHLR_UNIT(SCHEDULE_IDLE_SLEEP_THRESHOLD)) {
                sleepInterval = (SCHEDULE_IDLE_SLEEP_INTERVAL * 1000);
            }
        }
    }

    // finish:
    return sleepInterval;
}

void _wise_schlr_proc()
{
    updateTimeBase();

    if ((stSchCtrl.nextEventIdx != WISE_INVALID_INDEX) &&
        (!(stSchCtrl.status & SCHLR_STS_RELOAD_EVENT))) {
        // SCHLR_EVENT_T* pEvent = &stSchCtrl.eventBank[stSchCtrl.nextEventIdx];

        if (isTimeup(stSchCtrl.nextEvtTime)) {
            processEvent(stSchCtrl.nextEventIdx);
        }
    } else {
        loadNextEvent();
    }

#if 0
    if(stSchCtrl.mode == E_SCH_MODE_POWER_SAVING)
    {
        if(stSchCtrl.awakenCount == 0)
        {
            if(stSchCtrl.nextEventIdx != WISE_INVALID_INDEX)
            {
                uint32_t sleepInterval = 0;

#define MIN_SLEEP_TIME 200
#define SLEEP_WAKE_OVERHEAD 50

                if(isTimeup(stSchCtrl.nextEvtTime))
                {
                    debug_print("ouch\n");
                    processEvent(stSchCtrl.nextEventIdx);
                    return;
                }

                sleepInterval = SCHLR_UNIT_TO_US(time_diff(systemTick.tu, stSchCtrl.nextEvtTime.tu));
                if(sleepInterval >= MIN_SLEEP_TIME)
                {
#if 0
                    platform_enter_sleep(sleepInterval - SLEEP_WAKE_OVERHEAD);
#else
                    printf("s %lu\n", sleepInterval);
//                    platform_delay_us(sleepInterval - SLEEP_WAKE_OVERHEAD);

#endif
                }
            }
            else
            {
                //no jobs to do, sleep?
#define SCHEDULE_IDLE_SLEEP_THRESHOLD 3000  // ms
#define SCHEDULE_IDLE_SLEEP_INTERVAL 300000 // ms

                //debug_print(".");

                if(!(stSchCtrl.status & SCHLR_STS_IDLING))
                {
                    stSchCtrl.status |= SCHLR_STS_IDLING;
                    stSchCtrl.idleStart = wise_get_tu();
                }
                else
                {
                    uint32_t current = wise_get_tu();
                    if(time_diff(stSchCtrl.idleStart, current) > MS_TO_SCHLR_UNIT(SCHEDULE_IDLE_SLEEP_THRESHOLD))
                    {
                        //APTX_INFO("%lu %lu\n", stSchCtrl.idleStart, current);
                        debug_print("IDLE: %d ms\n", SCHEDULE_IDLE_SLEEP_INTERVAL);
                        wise_delay_us(10000);
//                        platform_enter_sleep(SCHEDULE_IDLE_SLEEP_INTERVAL * 1000);
                        stSchCtrl.idleStart = wise_get_tu();
                    }
                }
            }
        }
    }
#endif
}

int8_t wise_schlr_add_single(uint32_t afterTick, SCHEDULER_CALLBACK eventCb,
                             void *pData)
{
    int8_t emptySlot      = findEmptySlot();
    SCHLR_EVENT_T *pEvent = NULL;

    if (emptySlot == WISE_INVALID_INDEX) {
        debug_print("[SCH] event full\n");
        return WISE_INVALID_INDEX;
    }

    if (eventCb == NULL) {
        debug_print("[SCH] null single event\n");
        return WISE_INVALID_INDEX;
    }

    if (afterTick > 0xff000000) {
        debug_print("[SCH] invalid time %08lx %p\n", afterTick,
                    (void *)eventCb);
        return WISE_INVALID_INDEX;
    }

    pEvent             = &stSchCtrl.eventBank[emptySlot];
    pEvent->periodTick = 0; // should be 0 for single type.

    schlrGetTime(&pEvent->targetTime);
    if ((pEvent->targetTime.tu + afterTick) < pEvent->targetTime.tu) {
        pEvent->targetTime.round++;
    }
    pEvent->targetTime.tu += afterTick;

    pEvent->pData = pData;
    pEvent->proc  = eventCb;

    stSchCtrl.status |= SCHLR_STS_RELOAD_EVENT;

    return emptySlot;
}

int8_t wise_schlr_add_periodical(uint32_t periodTick,
                                 SCHEDULER_CALLBACK eventCb, void *pData)
{
    int8_t emptySlot      = findEmptySlot();
    SCHLR_EVENT_T *pEvent = NULL;

    if (emptySlot == WISE_INVALID_INDEX) {
        debug_print("[SCH] event full\n");
        return WISE_INVALID_INDEX;
    }

    if (eventCb == NULL) {
        debug_print("[SCH] null periodical event\n");
        return WISE_INVALID_INDEX;
    }

    pEvent             = &stSchCtrl.eventBank[emptySlot];
    pEvent->periodTick = periodTick;
    schlrGetTime(&pEvent->targetTime);
    timeIncrease(&pEvent->targetTime, periodTick);

    pEvent->pData = pData;
    pEvent->proc  = eventCb;

    stSchCtrl.status |= SCHLR_STS_RELOAD_EVENT;

    return emptySlot;
}

int8_t wise_schlr_remove(int8_t index)
{
    if (stSchCtrl.nextEventIdx == index) {
        memset(&stSchCtrl.nextEvtTime, 0, sizeof(SCHLR_TIME_T));
        stSchCtrl.nextEventIdx = WISE_INVALID_INDEX;
    }

    // APTX_DEBUG("r %d %p\n", index, stSchCtrl.eventBank[index].proc);

    if (stSchCtrl.eventBank[index].proc) {
        memset(&stSchCtrl.eventBank[index], 0, sizeof(SCHLR_EVENT_T));
    }

    return WISE_SUCCESS;
}

uint32_t wise_get_tu()
{
    updateTimeBase();
    return systemTick.tu;
}

uint32_t wise_get_cur_ms()
{
    updateTimeBase();
    return systemTick.ms;
}

void wise_log_time_info(uint32_t wiseTU)
{
    debug_print("[%d.%04d] ", (int)(wiseTU/10000), (int)(wiseTU%10000));
}


#endif /* MIDDLEWARE_WISE_SYSTEM*/

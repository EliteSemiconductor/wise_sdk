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

#define GET_EVENT_MODULE(e)                                                    \
    ((e >> WISE_EVENT_TYPE_SHIFT) & WISE_EVENT_TYPE_MASK)
#define GET_EVENT(e) (e & WISE_EVENT_MASK)

typedef struct {
    uint32_t events[WISE_MAX_EVENT_QUEUE];
    uint8_t eventNum;
    uint8_t wIndex;
    uint8_t rIndex;
} ST_WISE_EVT_Q_T;

typedef struct {
    uint32_t events;
    wise_event_handler handler;
} ST_WISE_EVT_HANDLER_T;

static ST_WISE_EVT_Q_T stEvtQueue;
static ST_WISE_EVT_HANDLER_T stEvtHandlers[WISE_MAX_EVENT_HANDLER];
static uint8_t evtHandlerNum = 0;

void _wise_process_event();
uint8_t _wise_get_evt_num();

static void dispatchEvent(uint32_t event);

static void dispatchEvent(uint32_t event)
{
    int i;
    uint8_t module = GET_EVENT_MODULE(event);

    event = GET_EVENT(event);

    for (i = 0; i < evtHandlerNum; i++) {
        if (!stEvtHandlers[i].handler) {
            debug_print("Bug!! N H\n");
            continue;
        }

        if ((GET_EVENT_MODULE(stEvtHandlers[i].events) == module) &&
            (event & GET_EVENT(stEvtHandlers[i].events))) {
            (stEvtHandlers[i].handler)(module, event);
        }
    }
}

static inline uint8_t evtValid(uint32_t event)
{
    int bits = 0;

    while (event) {
        if (event & 1) {
            bits++;
        }

        event = (event >> 1);
    }

    return ((bits == 1) ? 1 : 0);
}

void wise_event_init()
{
    memset(&stEvtQueue, 0, sizeof(stEvtQueue));
    memset((void *)stEvtHandlers, 0, sizeof(stEvtHandlers));
    evtHandlerNum = 0;
}

uint8_t _wise_get_evt_num()
{
    return stEvtQueue.eventNum;
}

void _wise_process_event()
{
    while (stEvtQueue.eventNum > 0) {
        uint32_t event;

        CORE_DECLARE_IRQ_STATE;
        CORE_ENTER_CRITICAL();

        event = stEvtQueue.events[stEvtQueue.rIndex];
        stEvtQueue.events[stEvtQueue.rIndex] = 0;
        stEvtQueue.rIndex++;
        if (stEvtQueue.rIndex == WISE_MAX_EVENT_QUEUE) {
            stEvtQueue.rIndex = 0;
        }
        stEvtQueue.eventNum--;
        CORE_EXIT_CRITICAL();

        dispatchEvent(event);
    }
}

void wise_event_post(uint8_t module, uint32_t event)
{
    uint32_t newEvt = PACK_EVENT(module, event);
    int i, rIndex;

    if (stEvtQueue.eventNum >= WISE_MAX_EVENT_QUEUE) {
        debug_print("EVT_Q drop %d-%lx\n", module, event);
        return;
    }

    if (!evtValid(event)) {
        debug_print("EVT_E %08lx\n", event);
        return;
    }

    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    rIndex = stEvtQueue.rIndex;
    for (i = 0; i < stEvtQueue.eventNum; i++) {
        if (newEvt == stEvtQueue.events[rIndex]) {
            goto finish;
        }

        rIndex++;
        if (rIndex == WISE_MAX_EVENT_QUEUE) {
            rIndex = 0;
        }
    }

    stEvtQueue.events[stEvtQueue.wIndex] = newEvt;
    stEvtQueue.wIndex++;
    if (stEvtQueue.wIndex == WISE_MAX_EVENT_QUEUE) {
        stEvtQueue.wIndex = 0;
    }
    stEvtQueue.eventNum++;

finish:
    CORE_EXIT_CRITICAL();
}

int8_t wise_event_register_handler(uint8_t module, uint32_t event,
                                   wise_event_handler evtHandler)
{
    if (!module || !event || !evtHandler) {
        return WISE_FAIL;
    }

    if (evtHandlerNum >= WISE_MAX_EVENT_HANDLER) {
        debug_print("EVT_H full\n");
        return WISE_FAIL;
    }

    // debug_print("%d %p\n", evtHandlerNum, evtHandler);

    stEvtHandlers[evtHandlerNum].events  = PACK_EVENT(module, event);
    stEvtHandlers[evtHandlerNum].handler = evtHandler;
    evtHandlerNum++;

    return WISE_SUCCESS;
}

#endif /* MIDDLEWARE_WISE_SYSTEM*/

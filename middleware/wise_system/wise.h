/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise.h
 * @brief wise.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */


#ifndef __WISE_H
#define __WISE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wise_core.h"
#include "es_platform_components.h"

// WISE task API
#ifndef WISE_MAX_TASK_NUM
#define WISE_MAX_TASK_NUM                               8
#endif
#define WISE_MAX_TASK_NAME_LEN                          8

#define WISE_TIME_FOREVER                               ((uint32_t)0xFFFFFFFF)

typedef enum {
    E_WISE_PM_DISABLE = 0,
    E_WISE_PM_ENABLE  = 1,
} E_PWR_MODE_EN_T;

typedef enum {
    E_WISE_PM_ALLOW_SLEEP = 0,
    E_WISE_PM_KEEP_AWAKE  = 1,
} E_PWR_MODE_T;

int32_t wise_init();
void wise_main_proc();

void wise_system_idle(uint32_t idleMs);
void wise_system_sleep(uint32_t sleepMs);
void wise_system_restore();

void wise_pm_enable(E_PWR_MODE_EN_T enable);
void wise_pm_set_mode(E_PWR_MODE_T mode);

#if 0
int8_t wise_pm_is_system_busy();
#endif

// WISE task priority
enum {
    E_WISE_TASK_PRI_LOW = 0, // 100ms
    E_WISE_TASK_PRI_NORMAL,  // 10ms
    E_WISE_TASK_PRI_HIGH,    // continueously
};

typedef void (*wise_task_proc)(void *pData);

int8_t wise_create_task(wise_task_proc taskProc, void *pData, int8_t *name, uint8_t priority);
int8_t wise_remove_task(int8_t taskId);
int8_t wise_set_task_active(int8_t taskId);
int8_t wise_set_task_idle(int8_t taskId);

int8_t wise_request_reboot(uint32_t afterMs);

// WISE Event API
// WISE event fields
//  [24:31]  1-byte event module
//  [0:23]   1-bit event flag

#define WISE_EVENT_TYPE_MASK                            0xff
#define WISE_EVENT_TYPE_SHIFT                           24

#define WISE_EVENT_MASK                                 0x00ffffff

#define PACK_EVENT(m, e)                                (((m & WISE_EVENT_TYPE_MASK) << WISE_EVENT_TYPE_SHIFT) | (e & WISE_EVENT_MASK))

#define WISE_MAX_EVENT_QUEUE                            16
#define WISE_MAX_EVENT_HANDLER                          32

typedef void (*wise_event_handler)(uint8_t module, uint32_t event);

void wise_event_init();
int8_t wise_event_register_handler(uint8_t module, uint32_t event, wise_event_handler evtHandler);
void wise_event_post(uint8_t module, uint32_t event);

// WISE Scheduler API
//  scheduler uses 100us as time unit.
#define US_TO_SCHLR_UNIT(u)                             ((u) / 100)
#define MS_TO_SCHLR_UNIT(m)                             ((m) * 10)
#define SCHLR_UNIT_TO_US(t)                             ((t) * 100)
#define SCHLR_UNIT_TO_MS(t)                             ((t) / 10)

typedef void (*SCHEDULER_CALLBACK)(void *pData);

typedef enum {
    E_SCH_EVT_TYPE_ONCE       = 0,
    E_SCH_EVT_TYPE_PERIODICAL = 1
} E_SCH_EVT_TYPE_T;

int8_t wise_schlr_init();
int8_t wise_schlr_add_single(uint32_t afterTick, SCHEDULER_CALLBACK eventCb, void *pData);
int8_t wise_schlr_add_periodical(uint32_t periodTick, SCHEDULER_CALLBACK eventCb, void *pData);
int8_t wise_schlr_remove(int8_t index);

uint32_t wise_get_tu();     // get current time unit, 1tu=100us
uint32_t wise_get_cur_ms(); // get current time miliscond
void wise_log_time_info(uint32_t wiseTU);

// wraper for critical section
#define WISE_DECLARE_CRITICAL                           CORE_DECLARE_IRQ_STATE
#define WISE_ENTER_CRITICAL                             CORE_ENTER_CRITICAL
#define WISE_EXIT_CRITICAL                              CORE_EXIT_CRITICAL
#define WISE_IS_IN_ISR                                  CORE_IS_IN_ISR

// APIs for Debug IO
void wise_debug_io_init(uint8_t *pinArray, uint8_t pinNum);
void wise_debug_io_high(uint8_t index);
void wise_debug_io_low(uint8_t index);
void wise_debug_io_toggle(uint8_t index);
#endif

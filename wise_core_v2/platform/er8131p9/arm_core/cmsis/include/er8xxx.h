/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/*****************************************************************************
 * @file     er8xxx.h
 * @brief    ER8xxx peripheral access layer header file.
 *           This file contains all the peripheral register's definitions,
 *           bits definitions and memory mapping for ESMT SOC MCU.
 *
 * @copyright (C) COPYRIGHT 2021 ESMT Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __ER8XXX_H__
#define __ER8XXX_H__
/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
typedef enum {
    /* -------------------  Cortex-M3 Processor Exceptions Numbers
       ---------------------------------------------- */
    Reset_IRQn          = -15, /*!<   1  Reset Vector, invoked on Power up and warm reset       */
    NonMaskableInt_IRQn = -14, /*!<   2  Non maskable Interrupt, cannot be stopped or preempted */
    HardFault_IRQn      = -13, /*!<   3  Hard Fault, all classes of Fault   */
    SVCall_IRQn         = -5,  /*!<  11  System Service Call via SVC instruction       */
    DebugMonitor_IRQn   = -4,  /*!<  12  Debug Monitor */
    PendSV_IRQn         = -2,  /*!<  14  Pendable request for system service       */
    SysTick_IRQn        = -1,  /*!<  15  System Tick Timer      */
} IRQn_Type;
/* ----------------Configuration of the Cortex-M3 Processor and Core
 * Peripherals------------------------------ */
#define __CM3_REV 0x0201UL         /*!< Core Revision r2p1 */
#define __NVIC_PRIO_BITS 4UL       /*!< Number of Bits used for Priority Levels                     */
#define __Vendor_SysTickConfig 0UL /*!< Set to 1 if different SysTick Config is used                */
#define __MPU_PRESENT 1UL          /*!< MPU present or not                                          */
/* --------------------------- System Configuration Headers
 * -------------------------------------------------- */
#include "cmsis/include/core_cm3.h"
#include "system_er81xx.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* --------------------------- Default Hardware Configuration Parameters
 * ------------------------------------- */
#define MAX_TIMEOUT 100000 /**< Specify Time out */
#define EXT_IRQ_COUNT 33   /**< Number of External (NVIC) interrupts */

#endif /* __ER8XXX_H__ */

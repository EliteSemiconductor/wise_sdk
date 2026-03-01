/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**************************************************************************/ /**
                                                                              * @file     system_ARMCM3.c
                                                                              * @brief    CMSIS Device System Source File for
                                                                              *           ARMCM3 Device
                                                                              * @version  V5.3.1
                                                                              * @date     09. July 2018
                                                                              ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "esmt_chip_specific.h"
#include "cmsis/include/er8xxx.h"

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/

#define __SYSTEM_CLOCK_DIVIDER (1 << PCRMU->MCU_CLK_FAC_REG)
#define SYSTEM_CLOCK (__SYSTEM_CLOCK / __SYSTEM_CLOCK_DIVIDER)
uint32_t SystemCoreClock =
    __SYSTEM_CLOCK; /*!< System Clock Frequency (Core Clock)*/
uint32_t CyclesPerUs = (__HXT / 1000000UL); /* Cycles per micro second */

/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/

#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
extern uint32_t __VECTOR_TABLE;
#else
#define TOTAL_INTERRUPTS (16 + EXT_IRQ_COUNT)
typedef void (*VECTOR_TABLE_Type)(void);
extern const VECTOR_TABLE_Type __Vectors[TOTAL_INTERRUPTS];
#endif

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate(void)
{
    SystemCoreClock = __SYSTEM_CLOCK / __SYSTEM_CLOCK_DIVIDER;
    CyclesPerUs     = SystemCoreClock / 1000000UL;
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit(void)
{
#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    SCB->VTOR = (uint32_t)&__VECTOR_TABLE;
#else
    SCB->VTOR = (uint32_t)&__Vectors;
#endif
    SystemCoreClock = SYSTEM_CLOCK;
}

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**************************************************************************/ /**
                                                                              * @file     system_er8xxx.h
                                                                              * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File for ESMT SOC series
                                                                              *
                                                                              * @copyright (C) COPYRIGHT 2020 ESMT Technology Corp. All rights reserved.
                                                                              *****************************************************************************/
#ifndef __SYSTEM_ER8XXX_H
#define __SYSTEM_ER8XXX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */
extern uint32_t CyclesPerUs;     /*!< Cycles per micro second             */
extern uint32_t PllClock;        /*!< PLL Output Clock Frequency          */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the micro controller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit(void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_ER8XXX_H__ */
/*** (C) COPYRIGHT 2020 ESMT Technology Corp. ***/

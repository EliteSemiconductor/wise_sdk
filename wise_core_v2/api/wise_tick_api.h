/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_tick_api.h
 * @brief System tick counter and delay utilities.
 *
 * @ingroup WISE_TICK
 *
 * This module provides a unified system tick source (ARM SysTick, GPTMR,
 * or WUTMR depending on device configuration) and simple APIs for
 * microsecond/millisecond delays and tick counter retrieval.
 */

#ifndef __WISE_TICK_API_H
#define __WISE_TICK_API_H

#include "esmt_chip_specific.h"
#include "wise_core.h"
#include "hal_intf_sys.h"
#include "types.h"
#include <stdint.h>

/**
 * @defgroup WISE_TICK WISE Tick APIs
 * @ingroup WISE_CORE_API
 * @brief Tick counter and delay-function utilities.
 *
 * Depending on the chip configuration, system tick may be sourced from:
 * - ARM SysTick
 * - General-purpose timer (GPTMR)
 * - Wake-up timer (WUTMR)
 * @{
 */

/* ------------------------------------------------------------------------- */
/*                        Tick Source Selection Macros                       */
/* ------------------------------------------------------------------------- */

/** @brief Tick source: ARM SysTick. */
#define WISE_SYS_TICK_SRC_ARMTICK                       0
/** @brief Tick source: General-purpose timer (GPTMR). */
#define WISE_SYS_TICK_SRC_GPTMR                         1
/** @brief Tick source: Wake-up timer (WUTMR). */
#define WISE_SYS_TICK_SRC_WUTMR                         2

#ifdef CHIP_TICK_TIMER_CHANNEL
/** @brief Default tick source when GPTMR is available. */
#define WISE_SYS_TICK_SRC                               WISE_SYS_TICK_SRC_GPTMR
#else
/** @brief Default tick source when no dedicated GPTMR tick is defined. */
#define WISE_SYS_TICK_SRC                               WISE_SYS_TICK_SRC_ARMTICK
#endif

#if (WISE_SYS_TICK_SRC == WISE_SYS_TICK_SRC_ARMTICK)
#define _TICK_SRC_HZ                                    __SYSTEM_CLOCK
#elif (WISE_SYS_TICK_SRC == WISE_SYS_TICK_SRC_GPTMR)
#define _TICK_SRC_HZ                                    __SYSTEM_CLOCK
#ifndef WISE_SYS_TICK_GPTMR_CH
/** @brief GPTMR channel used as tick source. */
#define WISE_SYS_TICK_GPTMR_CH                          (CHIP_TIMER_CHANNEL_NUM - 1)
#endif
#endif


#if ((WISE_SYS_TICK_SRC == WISE_SYS_TICK_SRC_ARMTICK) || (WISE_SYS_TICK_SRC == WISE_SYS_TICK_SRC_GPTMR))
/** @brief Tick clock cycles per microsecond. */
#define CLK_PER_US                                      (_TICK_SRC_HZ / 1000000)
/** @brief Tick clock cycles per millisecond. */
#define CLK_PER_MS                                      (_TICK_SRC_HZ / 1000)

/** @brief Convert tick cycles to microseconds. */
#define CLK_TO_US(c)                                    ((c) / CLK_PER_US)
/** @brief Convert tick cycles to milliseconds. */
#define CLK_TO_MS(c)                                    ((c) / CLK_PER_MS)
/** @brief Convert microseconds to tick cycles. */
#define US_TO_CLK(u)                                    ((u) * CLK_PER_US)
/** @brief Convert milliseconds to tick cycles. */
#define MS_TO_CLK(m)                                    ((m) * CLK_PER_MS)
#endif

/* ------------------------------------------------------------------------- */
/*                               API Functions                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Initialize the system tick source.
 *
 * Selects the appropriate tick provider (SysTick, GPTMR, or WUTMR) based on
 * build-time configuration and prepares it for use.
 *
 * @retval 0   Initialization successful.
 * @retval <0  Initialization failed.
 */
int32_t wise_tick_init(void);

/**
 * @brief Get current tick counter value.
 *
 * @return Tick counter value (wrap-around depends on tick source width).
 */
uint32_t wise_tick_get_counter(void);

/**
 * @brief Delay execution for a number of milliseconds.
 *
 * Busy-wait delay based on the current system tick source.
 *
 * @param[in] ms Number of milliseconds to block.
 */
void wise_tick_delay_ms(uint32_t ms);

/**
 * @brief Delay execution for a number of microseconds.
 *
 * Busy-wait delay based on the current system tick source.
 *
 * @param[in] us Number of microseconds to block.
 */
void wise_tick_delay_us(uint32_t us);


/** @} */ /* end of WISE_TICK group */

#endif

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __CACHE_REG_ER8130_H__
#define __CACHE_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"

/* ================================================================================ */
/* ================                      CACHE_T                     ================ */
/* ================================================================================ */

/**
  * @brief CACHE (CACHE_T)
  */

 typedef struct {                                          /*!< CACHE_T Structure                                                        */
  __IO uint32_t CACHE_CONFIG;                              /*!< Configuration Register (0x000)                                           */
  __I  uint32_t CACHE_HIT_CNTR_LSB;                        /*!< Hit Counter LSB Register (0x004)                                         */
  __I  uint32_t CACHE_HIT_CNTR_MSB;                        /*!< Hit Counter MSB Register (0x008)                                         */
  __I  uint32_t CACHE_MISS_CNTR;                           /*!< Hit Miss Counter Register (0x00C)                                        */
} CACHE_T;

#define CACHE_CONFIG_ADDR                                  (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_CACHE_EN_ADDR                                (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_CACHE_EN_POS                                 (0)      /*< bit[0]      */
#define CACHE_CACHE_EN_MASK                                (0x1ul << CACHE_CACHE_EN_POS)
#define CACHE_CNT_EN_ADDR                                  (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_CNT_EN_POS                                   (1)      /*< bit[1]      */
#define CACHE_CNT_EN_MASK                                  (0x1ul << CACHE_CNT_EN_POS)
#define CACHE_INVALID_EN_ADDR                              (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_INVALID_EN_POS                               (2)      /*< bit[2]      */
#define CACHE_INVALID_EN_MASK                              (0x1ul << CACHE_INVALID_EN_POS)
#define CACHE_INVALID_DONE_ADDR                            (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_INVALID_DONE_POS                             (3)      /*< bit[3]      */
#define CACHE_INVALID_DONE_MASK                            (0x1ul << CACHE_INVALID_DONE_POS)
#define CACHE_WAY_DIS_ADDR                                 (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_WAY_DIS_POS                                  (8)      /*< bit[11:8]   */
#define CACHE_WAY_DIS_MASK                                 (0xFul << CACHE_WAY_DIS_POS)
#define CACHE_RST_ADDR                                     (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_RST_POS                                      (14)     /*< bit[14]     */
#define CACHE_RST_MASK                                     (0x1ul << CACHE_RST_POS)
#define CACHE_EARLY_RSP_DIS_ADDR                           (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_EARLY_RSP_DIS_POS                            (25)     /*< bit[25]     */
#define CACHE_EARLY_RSP_DIS_MASK                           (0x1ul << CACHE_EARLY_RSP_DIS_POS)
#define CACHE_WRAP_DIS_ADDR                                (uint32_t)&(CACHE->CACHE_CONFIG)
#define CACHE_WRAP_DIS_POS                                 (26)     /*< bit[26]     */
#define CACHE_WRAP_DIS_MASK                                (0x1ul << CACHE_WRAP_DIS_POS)

#define CACHE_HIT_CNTR_LSB_ADDR                            (uint32_t)&(CACHE->CACHE_HIT_CNTR_LSB)

#define CACHE_HIT_CNTR_MSB_ADDR                            (uint32_t)&(CACHE->CACHE_HIT_CNTR_MSB)

#define CACHE_MISS_CNTR_ADDR                               (uint32_t)&(CACHE->CACHE_MISS_CNTR)

#define CACHE_BASE                                          0x40008000UL
#define CACHE                                               ((CACHE_T               *) CACHE_BASE)

#endif /* __CACHE_REG_ER8130_H__ */

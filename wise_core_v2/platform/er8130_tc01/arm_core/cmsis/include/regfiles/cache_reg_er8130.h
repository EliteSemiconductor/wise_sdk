/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_cache_soc_d1_20241111.xlsx */
/* ================================================================================
 */
/* ================                     CACHE_T ================ */
/* ================================================================================
 */
#ifndef __CACHE_REG_ER8130_H__
#define __CACHE_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief CACHE (CACHE_T)
 */

typedef struct {                   /*!< CACHE_T Structure */
    __IO uint32_t L1C_CONFIG;      /*!< L1C_CONFIG Register (0x000) */
    __IO uint32_t L1C_HIT_CNT_LSB; /*!< L1C_HIT_CNT_LSB Register (0x004) */
    __IO uint32_t L1C_HIT_CNT_MSB; /*!< L1C_HIT_CNT_MSB Register (0x008) */
    __IO uint32_t L1C_MISS_CNT;    /*!< L1C_MISS_CNT Register (0x00C) */
} CACHE_T;

#define CACHE_L1C_CONFIG_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_CACHEABLE_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_CACHEABLE_POS (0) /*< bit[0]      */
#define CACHE_CACHEABLE_MASK (0x1ul << CACHE_CACHEABLE_POS)
#define CACHE_CNT_EN_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_CNT_EN_POS (1) /*< bit[1]      */
#define CACHE_CNT_EN_MASK (0x1ul << CACHE_CNT_EN_POS)
#define CACHE_INVALID_EN_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_INVALID_EN_POS (2) /*< bit[2]      */
#define CACHE_INVALID_EN_MASK (0x1ul << CACHE_INVALID_EN_POS)
#define CACHE_INVALID_DONE_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_INVALID_DONE_POS (3) /*< bit[3]      */
#define CACHE_INVALID_DONE_MASK (0x1ul << CACHE_INVALID_DONE_POS)
#define CACHE_WAY_DIS_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_WAY_DIS_POS (8) /*< bit[11:8]   */
#define CACHE_WAY_DIS_MASK (0xFul << CACHE_WAY_DIS_POS)
#define CACHE_RESERVED0_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_RESERVED0_POS (12) /*< bit[13:12]  */
#define CACHE_RESERVED0_MASK (0x3ul << CACHE_RESERVED0_POS)
#define CACHE_ENB_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_ENB_POS (14) /*< bit[14]     */
#define CACHE_ENB_MASK (0x1ul << CACHE_ENB_POS)
#define CACHE_RESERVED1_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_RESERVED1_POS (15) /*< bit[24:15]  */
#define CACHE_RESERVED1_MASK (0x3FFul << CACHE_RESERVED1_POS)
#define CACHE_EARLY_RESP_DIS_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_EARLY_RESP_DIS_POS (25) /*< bit[25]     */
#define CACHE_EARLY_RESP_DIS_MASK (0x1ul << CACHE_EARLY_RESP_DIS_POS)
#define CACHE_WRAP_DIS_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_WRAP_DIS_POS (26) /*< bit[26]     */
#define CACHE_WRAP_DIS_MASK (0x1ul << CACHE_WRAP_DIS_POS)
#define CACHE_RESERVED2_ADDR (uint32_t)&(CACHE->L1C_CONFIG)
#define CACHE_RESERVED2_POS (27) /*< bit[31:27]  */
#define CACHE_RESERVED2_MASK (0x1Ful << CACHE_RESERVED2_POS)
#define CACHE_L1C_HIT_CNT_LSB_ADDR (uint32_t)&(CACHE->L1C_HIT_CNT_LSB)
#define CACHE_L1C_HIT_CNT_MSB_ADDR (uint32_t)&(CACHE->L1C_HIT_CNT_MSB)
#define CACHE_L1C_MISS_CNT_ADDR (uint32_t)&(CACHE->L1C_MISS_CNT)
#define CACHE_BASE 0x40008000UL
#define CACHE ((CACHE_T *)CACHE_BASE)

#endif /* __CACHE_REG_ER8130_H__ */

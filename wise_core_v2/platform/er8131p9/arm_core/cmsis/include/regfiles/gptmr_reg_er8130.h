/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_gptimer_soc_d1_20241017.xlsx */
/* ================================================================================
 */
/* ================                     GPTMR_T ================ */
/* ================================================================================
 */
#ifndef __GPTMR_REG_ER8130_H__
#define __GPTMR_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief GPTMR (GPTMR_T)
 */

typedef struct {
    __IO uint32_t CTRL;   /*!< CTRL Register (0x000)      */
    __IO uint32_t VALUE;  /*!< VALUE Register (0x004)     */
    __IO uint32_t RELOAD; /*!< RELOAD Register (0x008)    */
    union {
        __I uint32_t INTSTATUS; /*!< INTSTATUS Register (0x00C) */
        __O uint32_t INTCLEAR;  /*!< INTCLEAR Register (0x00C)  */
    };
} GPTMR_T;

#define GPTMR_CTRL_ADDR (uint32_t)&(GPTMR->CTRL)
#define GPTMR_EN_ADDR (uint32_t)&(GPTMR->CTRL)
#define GPTMR_EN_POS (0) /*< bit[0]      */
#define GPTMR_EN_MASK (0x1ul << GPTMR_EN_POS)
#define GPTMR_EXT_EN_ADDR (uint32_t)&(GPTMR->CTRL)
#define GPTMR_EXT_EN_POS (1) /*< bit[1]      */
#define GPTMR_EXT_EN_MASK (0x1ul << GPTMR_EXT_EN_POS)
#define GPTMR_EXT_CLK_ADDR (uint32_t)&(GPTMR->CTRL)
#define GPTMR_EXT_CLK_POS (2) /*< bit[2]      */
#define GPTMR_EXT_CLK_MASK (0x1ul << GPTMR_EXT_CLK_POS)
#define GPTMR_INT_EN_ADDR (uint32_t)&(GPTMR->CTRL)
#define GPTMR_INT_EN_POS (3) /*< bit[3]      */
#define GPTMR_INT_EN_MASK (0x1ul << GPTMR_INT_EN_POS)

#define GPTMR_VALUE_ADDR (uint32_t)&(GPTMR->VALUE)
#define GPTMR_RELOAD_ADDR (uint32_t)&(GPTMR->RELOAD)
#define GPTMR_INTSTATUS_ADDR (uint32_t)&(GPTMR->INTSTATUS)
#define GPTMR_INTSTATUS_ADDR (uint32_t)&(GPTMR->INTSTATUS)
#define GPTMR_INTSTATUS_POS (0) /*< bit[0]      */
#define GPTMR_INTSTATUS_MASK (0x1ul << GPTMR_INTSTATUS_POS)

#define GPTMR_INTCLEAR_ADDR (uint32_t)&(GPTMR->INTCLEAR)
#define GPTMR_INTCLEAR_ADDR (uint32_t)&(GPTMR->INTCLEAR)
#define GPTMR_INTCLEAR_POS (0) /*< bit[0]      */
#define GPTMR_INTCLEAR_MASK (0x1ul << GPTMR_INTCLEAR_POS)

#define GPTMR_BASE 0x4001A000UL
#define GPTMR0 ((GPTMR_T *)GPTMR_BASE)
#define GPTMR1 ((GPTMR_T *)(GPTMR_BASE + 0x1000))
#define GPTMR2 ((GPTMR_T *)(GPTMR_BASE + 0x2000))
#define GPTMR3 ((GPTMR_T *)(GPTMR_BASE + 0x3000))
#define GPTMR4 ((GPTMR_T *)(GPTMR_BASE + 0x4000))
#define GPTMR5 ((GPTMR_T *)(GPTMR_BASE + 0x5000))

#endif /* __GPTMR_REG_ER8130_H__ */

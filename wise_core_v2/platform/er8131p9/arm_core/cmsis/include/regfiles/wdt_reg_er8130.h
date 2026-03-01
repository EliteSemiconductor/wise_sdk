/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_wdt_soc_d1_20241204.xlsx */
/* ================================================================================
 */
/* ================                      WDT_T ================ */
/* ================================================================================
 */
#ifndef __WDT_REG_ER8130_H__
#define __WDT_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief WDT (WDT_T)
 */

typedef struct {
    __IO uint32_t LOAD;   /*!< LOAD Register (0x000)  */
    __IO uint32_t VALUE;  /*!< VALUE Register (0x004) */
    __IO uint32_t CTRL;   /*!< CTRL Register (0x008)  */
    __IO uint32_t INTCLR; /*!< INTCLR Register (0x00C)*/
    __I uint32_t RIS;     /*!< RIS Register (0x010)   */
    __I uint32_t MIS;     /*!< MIS Register (0x014)   */
    __I uint32_t RESERVED0[762];
    __IO uint32_t LOCK; /*!< LOCK Register (0xC00)   */
} WDT_T;

#define WDT_LOAD_ADDR (uint32_t)&(WDT->LOAD)
#define WDT_VALUE_ADDR (uint32_t)&(WDT->VALUE)
#define WDT_CTRL_ADDR (uint32_t)&(WDT->CTRL)
#define WDT_INTEN_ADDR (uint32_t)&(WDT->CTRL)
#define WDT_INTEN_POS (0) /*< bit[0]      */
#define WDT_INTEN_MASK (0x1ul << WDT_INTEN_POS)
#define WDT_RESEN_ADDR (uint32_t)&(WDT->CTRL)
#define WDT_RESEN_POS (1) /*< bit[1]      */
#define WDT_RESEN_MASK (0x1ul << WDT_RESEN_POS)

#define WDT_INTCLR_ADDR (uint32_t)&(WDT->INTCLR)
#define WDT_RIS_ADDR (uint32_t)&(WDT->RIS)
#define WDT_RAW_INTSTS_ADDR (uint32_t)&(WDT->RIS)
#define WDT_RAW_INTSTS_POS (0) /*< bit[0]      */
#define WDT_RAW_INTSTS_MASK (0x1ul << WDT_RAW_INTSTS_POS)

#define WDT_MIS_ADDR (uint32_t)&(WDT->MIS)
#define WDT_INTSTS_ADDR (uint32_t)&(WDT->MIS)
#define WDT_INTSTS_POS (0) /*< bit[0]      */
#define WDT_INTSTS_MASK (0x1ul << WDT_INTSTS_POS)

#define WDT_LOCK_ADDR (uint32_t)&(WDT->LOCK)
#define WDT_WEN_ADDR (uint32_t)&(WDT->LOCK)
#define WDT_WEN_POS (0) /*< bit[0]      */
#define WDT_WEN_MASK (0x1ul << WDT_WEN_POS)
#define WDT_WREG_EN_ADDR (uint32_t)&(WDT->LOCK)
#define WDT_WREG_EN_POS (1) /*< bit[31:1]   */
#define WDT_WREG_EN_MASK (0x7FFFFFFFul << WDT_WREG_EN_POS)

#define WDT_BASE 0x40019000UL
#define WDT ((WDT_T *)WDT_BASE)
#endif /* __WDT_REG_ER8130_H__ */

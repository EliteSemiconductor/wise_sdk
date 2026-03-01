/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : Reg_Table_wutmr_soc_20240819.xlsx */
/* ================================================================================
 */
/* ================                     WUTMR_T ================ */
/* ================================================================================
 */
#ifndef __WUTMR_REG_ER8130_H__
#define __WUTMR_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief WUTMR (WUTMR_T)
 */

typedef struct {
    union {
        __IO uint32_t CTL_REG; /*!< CTL_REG Register (0x000)       */
        struct {
            __IO uint32_t WUTMR_EN      : 1;
            __IO uint32_t WUTMR_PAWR_EN : 1;
            uint32_t                    : 2;
            __IO uint32_t WUTMR_REP_EN  : 1;
            uint32_t                    : 26;
            __IO uint32_t WUTMR_CTL_RST : 1;
        } CTL_REG_b;
    };
    union {
        __I uint32_t STS; /*!< STS Register (0x004)           */
        struct {
            __I uint32_t WUTMR_EVT_WAKEUP : 1;
            __I uint32_t WUTMR_RSP_WAKEUP : 1;
            uint32_t                      : 30;
        } STS_b;
    };
    __IO uint32_t PER_REG; /*!< PER_REG Register (0x008)       */
    __I uint32_t RESERVED0;
    __IO uint32_t CUR0; /*!< CUR0 Register (0x010)          */
    __I uint32_t CUR1;  /*!< CUR1 Register (0x014)          */
    union {
        __I uint32_t CUR2; /*!< CUR2 Register (0x018)          */
        struct {
            __I uint32_t WUTNR_SYNC : 1; /*!< CUR2 Register (0x018)          */
            uint32_t                : 31;
        } CUR2_b;
    };
    __I uint32_t RESERVED1;
    union {
        __IO uint32_t INT_ST; /*!< INT_ST Register (0x020)        */
        struct {
            __O uint32_t INT_ST_WUTMR : 1;
            uint32_t                  : 31;
        } INT_ST_b;
    };
    union {
        __IO uint32_t INT_EN; /*!< INT_EN Register (0x024)        */
        struct {
            __IO uint32_t INT_EN_WUTMR : 1;
            uint32_t                   : 31;
        } INT_EN_b;
    };
    union {
        __I uint32_t INT; /*!< INT Register (0x028)           */
        struct {
            __IO uint32_t INT_WUTMR : 1;
            uint32_t                : 31;
        } INT_b;
    };
    __IO uint32_t EVT_CNTR_INIT; /*!< EVT_CNTR_INIT Register (0x02C) */
    __I uint32_t EVT;            /*!< EVT Register (0x030)           */
    __I uint32_t PAWR_REG0;      /*!< PAWR_REG0 Register (0x034)     */
    __IO uint32_t PAWR_REG1;     /*!< PAWR_REG1 Register (0x038)     */
    __IO uint32_t PAWR_REG2;     /*!< PAWR_REG2 Register (0x03C)     */
    __IO uint32_t PAWR_REG3;     /*!< PAWR_REG3 Register (0x040)     */
} WUTMR_T;

#define WUTMR_CTL_REG_ADDR (uint32_t)&(WUTMR->CTL_REG)
#define WUTMR_WUTMR_EN_ADDR (uint32_t)&(WUTMR->CTL_REG)
#define WUTMR_WUTMR_EN_POS (0) /*< bit[0]      */
#define WUTMR_WUTMR_EN_MASK (0x1ul << WUTMR_WUTMR_EN_POS)
#define WUTMR_WUTMR_PAWR_EN_ADDR (uint32_t)&(WUTMR->CTL_REG)
#define WUTMR_WUTMR_PAWR_EN_POS (1) /*< bit[1]      */
#define WUTMR_WUTMR_PAWR_EN_MASK (0x1ul << WUTMR_WUTMR_PAWR_EN_POS)
#define WUTMR_WUTMR_REP_EN_ADDR (uint32_t)&(WUTMR->CTL_REG)
#define WUTMR_WUTMR_REP_EN_POS (4) /*< bit[4]      */
#define WUTMR_WUTMR_REP_EN_MASK (0x1ul << WUTMR_WUTMR_REP_EN_POS)
#define WUTMR_WUTMR_CTL_RST_ADDR (uint32_t)&(WUTMR->CTL_REG)
#define WUTMR_WUTMR_CTL_RST_POS (31) /*< bit[31]     */
#define WUTMR_WUTMR_CTL_RST_MASK (0x1ul << WUTMR_WUTMR_CTL_RST_POS)

#define WUTMR_STS_ADDR (uint32_t)&(WUTMR->STS)
#define WUTMR_WUTMR_EVT_WAKEUP_ADDR (uint32_t)&(WUTMR->STS)
#define WUTMR_WUTMR_EVT_WAKEUP_POS (0) /*< bit[0]      */
#define WUTMR_WUTMR_EVT_WAKEUP_MASK (0x1ul << WUTMR_WUTMR_EVT_WAKEUP_POS)
#define WUTMR_WUTMR_RSP_WAKEUP_ADDR (uint32_t)&(WUTMR->STS)
#define WUTMR_WUTMR_RSP_WAKEUP_POS (1) /*< bit[1]      */
#define WUTMR_WUTMR_RSP_WAKEUP_MASK (0x1ul << WUTMR_WUTMR_RSP_WAKEUP_POS)

#define WUTMR_PER_REG_ADDR (uint32_t)&(WUTMR->PER_REG)
#define WUTMR_CUR0_ADDR (uint32_t)&(WUTMR->CUR0)
#define WUTMR_CUR1_ADDR (uint32_t)&(WUTMR->CUR1)
#define WUTMR_WUTMR_CUR_H_ADDR (uint32_t)&(WUTMR->CUR1)
#define WUTMR_WUTMR_CUR_H_POS (0) /*< bit[15:0]   */
#define WUTMR_WUTMR_CUR_H_MASK (0xFFFFul << WUTMR_WUTMR_CUR_H_POS)

#define WUTMR_CUR2_ADDR (uint32_t)&(WUTMR->CUR2)
#define WUTMR_WUTMR_SYNC_ADDR (uint32_t)&(WUTMR->CUR2)
#define WUTMR_WUTMR_SYNC_POS (0) /*< bit[0]      */
#define WUTMR_WUTMR_SYNC_MASK (0x1ul << WUTMR_WUTMR_SYNC_POS)

#define WUTMR_INT_ST_ADDR (uint32_t)&(WUTMR->INT_ST)
#define WUTMR_INT_ST_WUTMR_ADDR (uint32_t)&(WUTMR->INT_ST)
#define WUTMR_INT_ST_WUTMR_POS (0) /*< bit[0]      */
#define WUTMR_INT_ST_WUTMR_MASK (0x1ul << WUTMR_INT_ST_WUTMR_POS)

#define WUTMR_INT_EN_ADDR (uint32_t)&(WUTMR->INT_EN)
#define WUTMR_INT_EN_WUTMR_ADDR (uint32_t)&(WUTMR->INT_EN)
#define WUTMR_INT_EN_WUTMR_POS (0) /*< bit[0]      */
#define WUTMR_INT_EN_WUTMR_MASK (0x1ul << WUTMR_INT_EN_WUTMR_POS)

#define WUTMR_INT_ADDR (uint32_t)&(WUTMR->INT)
#define WUTMR_INT_WUTMR_ADDR (uint32_t)&(WUTMR->INT)
#define WUTMR_INT_WUTMR_POS (0) /*< bit[0]      */
#define WUTMR_INT_WUTMR_MASK (0x1ul << WUTMR_INT_WUTMR_POS)

#define WUTMR_EVT_CNTR_INIT_ADDR (uint32_t)&(WUTMR->EVT_CNTR_INIT)
#define WUTMR_WUTMR_EVT_CNTR_INIT_ADDR (uint32_t)&(WUTMR->EVT_CNTR_INIT)
#define WUTMR_WUTMR_EVT_CNTR_INIT_POS (0) /*< bit[15:0]   */
#define WUTMR_WUTMR_EVT_CNTR_INIT_MASK                                         \
    (0xFFFFul << WUTMR_WUTMR_EVT_CNTR_INIT_POS)

#define WUTMR_EVT_ADDR (uint32_t)&(WUTMR->EVT)
#define WUTMR_WUTMR_EVT_CNTR_ADDR (uint32_t)&(WUTMR->EVT)
#define WUTMR_WUTMR_EVT_CNTR_POS (0) /*< bit[15:0]   */
#define WUTMR_WUTMR_EVT_CNTR_MASK (0xFFFFul << WUTMR_WUTMR_EVT_CNTR_POS)
#define WUTMR_WUTMR_EVT_ID_ADDR (uint32_t)&(WUTMR->EVT)
#define WUTMR_WUTMR_EVT_ID_POS (16) /*< bit[31:16]  */
#define WUTMR_WUTMR_EVT_ID_MASK (0xFFFFul << WUTMR_WUTMR_EVT_ID_POS)

#define WUTMR_PAWR_REG0_ADDR (uint32_t)&(WUTMR->PAWR_REG0)
#define WUTMR_WUTMR_EVT_SUB_CNTR_ADDR (uint32_t)&(WUTMR->PAWR_REG0)
#define WUTMR_WUTMR_EVT_SUB_CNTR_POS (0) /*< bit[7:0]    */
#define WUTMR_WUTMR_EVT_SUB_CNTR_MASK (0xFFul << WUTMR_WUTMR_EVT_SUB_CNTR_POS)
#define WUTMR_WUTMR_EVT_SUB_ID_ADDR (uint32_t)&(WUTMR->PAWR_REG0)
#define WUTMR_WUTMR_EVT_SUB_ID_POS (8) /*< bit[14:8]   */
#define WUTMR_WUTMR_EVT_SUB_ID_MASK (0x7Ful << WUTMR_WUTMR_EVT_SUB_ID_POS)
#define WUTMR_WUTMR_RSP_CNTR_ADDR (uint32_t)&(WUTMR->PAWR_REG0)
#define WUTMR_WUTMR_RSP_CNTR_POS (15) /*< bit[23:15]  */
#define WUTMR_WUTMR_RSP_CNTR_MASK (0x1FFul << WUTMR_WUTMR_RSP_CNTR_POS)
#define WUTMR_WUTMR_RSP_ID_ADDR (uint32_t)&(WUTMR->PAWR_REG0)
#define WUTMR_WUTMR_RSP_ID_POS (24) /*< bit[31:24]  */
#define WUTMR_WUTMR_RSP_ID_MASK (0xFFul << WUTMR_WUTMR_RSP_ID_POS)

#define WUTMR_PAWR_REG1_ADDR (uint32_t)&(WUTMR->PAWR_REG1)
#define WUTMR_WUTMR_EVT_SUB_NUM_ADDR (uint32_t)&(WUTMR->PAWR_REG1)
#define WUTMR_WUTMR_EVT_SUB_NUM_POS (0) /*< bit[7:0]    */
#define WUTMR_WUTMR_EVT_SUB_NUM_MASK (0xFFul << WUTMR_WUTMR_EVT_SUB_NUM_POS)
#define WUTMR_WUTMR_EVT_SUB_PER_ADDR (uint32_t)&(WUTMR->PAWR_REG1)
#define WUTMR_WUTMR_EVT_SUB_PER_POS (8) /*< bit[21:8]   */
#define WUTMR_WUTMR_EVT_SUB_PER_MASK (0x3FFFul << WUTMR_WUTMR_EVT_SUB_PER_POS)

#define WUTMR_PAWR_REG2_ADDR (uint32_t)&(WUTMR->PAWR_REG2)
#define WUTMR_WUTMR_RSP_DLY_ADDR (uint32_t)&(WUTMR->PAWR_REG2)
#define WUTMR_WUTMR_RSP_DLY_POS (0) /*< bit[13:0]   */
#define WUTMR_WUTMR_RSP_DLY_MASK (0x3FFFul << WUTMR_WUTMR_RSP_DLY_POS)
#define WUTMR_WUTMR_RSP_MODE_ADDR (uint32_t)&(WUTMR->PAWR_REG2)
#define WUTMR_WUTMR_RSP_MODE_POS (14) /*< bit[14]     */
#define WUTMR_WUTMR_RSP_MODE_MASK (0x1ul << WUTMR_WUTMR_RSP_MODE_POS)
#define WUTMR_WUTMR_EVT_SUB_ID_SET_ADDR (uint32_t)&(WUTMR->PAWR_REG2)
#define WUTMR_WUTMR_EVT_SUB_ID_SET_POS (15) /*< bit[21:15]  */
#define WUTMR_WUTMR_EVT_SUB_ID_SET_MASK                                        \
    (0x7Ful << WUTMR_WUTMR_EVT_SUB_ID_SET_POS)
#define WUTMR_WUTMR_RSP_ID_SET_ADDR (uint32_t)&(WUTMR->PAWR_REG2)
#define WUTMR_WUTMR_RSP_ID_SET_POS (22) /*< bit[29:22]  */
#define WUTMR_WUTMR_RSP_ID_SET_MASK (0xFFul << WUTMR_WUTMR_RSP_ID_SET_POS)

#define WUTMR_PAWR_REG3_ADDR (uint32_t)&(WUTMR->PAWR_REG3)
#define WUTMR_WUTMR_RSP_NUM_ADDR (uint32_t)&(WUTMR->PAWR_REG3)
#define WUTMR_WUTMR_RSP_NUM_POS (0) /*< bit[8:0]    */
#define WUTMR_WUTMR_RSP_NUM_MASK (0x1FFul << WUTMR_WUTMR_RSP_NUM_POS)
#define WUTMR_WUTMR_RSP_PER_ADDR (uint32_t)&(WUTMR->PAWR_REG3)
#define WUTMR_WUTMR_RSP_PER_POS (9) /*< bit[19:9]   */
#define WUTMR_WUTMR_RSP_PER_MASK (0x7FFul << WUTMR_WUTMR_RSP_PER_POS)

#define WUTMR_BASE 0x40018000UL
#define WUTMR ((WUTMR_T *)WUTMR_BASE)

#endif /* __WUTMR_REG_ER8130_H__ */

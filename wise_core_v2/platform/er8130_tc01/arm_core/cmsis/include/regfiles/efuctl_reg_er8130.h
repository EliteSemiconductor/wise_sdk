/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_efuctl_xs_b1_20231211.xlsx */
/* ================================================================================
 */
/* ================                    EFUCTL_T ================ */
/* ================================================================================
 */
#ifndef __EFUCTL_REG_ER8130_H__
#define __EFUCTL_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief EFUCTL (EFUCTL_T)
 */

typedef struct {        /*!< EFUCTL_T Structure */
    __IO uint32_t CTL0; /*!< CTL0 Register (0x000) */
    __IO uint32_t CTL1; /*!< CTL1 Register (0x004) */
    __IO uint32_t VQPS; /*!< VQPS Register (0x008) */
    __I uint32_t RESERVED0;
    __IO uint32_t INT_ST; /*!< INT_ST Register (0x010) */
    __IO uint32_t INT_EN; /*!< INT_EN Register (0x014) */
    __I uint32_t INT;     /*!< INT Register (0x018) */
    __I uint32_t RESERVED1;
    __IO uint32_t XIP_REG; /*!< XIP_REG Register (0x020) */
    __I uint32_t RESERVED2[1007];
    __I uint32_t ID0; /*!< ID0 Register (0xFE0) */
    __I uint32_t RESERVED3[3];
    __IO uint32_t ID1; /*!< ID1 Register (0xFF0) */
} EFUCTL_T;

#define EFUCTL_CTL0_ADDR (uint32_t)&(EFUCTL->CTL0)
#define EFUCTL_CMD_ADDR (uint32_t)&(EFUCTL->CTL0)
#define EFUCTL_CMD_POS (0) /*< bit[3:0]    */
#define EFUCTL_CMD_MASK (0xFul << EFUCTL_CMD_POS)
#define EFUCTL_ADR_ADDR (uint32_t)&(EFUCTL->CTL0)
#define EFUCTL_ADR_POS (16) /*< bit[24:16]  */
#define EFUCTL_ADR_MASK (0x1FFul << EFUCTL_ADR_POS)

#define EFUCTL_CTL1_ADDR (uint32_t)&(EFUCTL->CTL1)
#define EFUCTL_VQPS_ADDR (uint32_t)&(EFUCTL->VQPS)
#define EFUCTL_VQPS_EN_ADDR (uint32_t)&(EFUCTL->VQPS)
#define EFUCTL_VQPS_EN_POS (0) /*< bit[0]      */
#define EFUCTL_VQPS_EN_MASK (0x1ul << EFUCTL_VQPS_EN_POS)
#define EFUCTL_VQPS_STS_ADDR (uint32_t)&(EFUCTL->VQPS)
#define EFUCTL_VQPS_STS_POS (1) /*< bit[1]      */
#define EFUCTL_VQPS_STS_MASK (0x1ul << EFUCTL_VQPS_STS_POS)

#define EFUCTL_INT_ST_ADDR (uint32_t)&(EFUCTL->INT_ST)
#define EFUCTL_INT_ST_COMPL_ADDR (uint32_t)&(EFUCTL->INT_ST)
#define EFUCTL_INT_ST_COMPL_POS (0) /*< bit[0]      */
#define EFUCTL_INT_ST_COMPL_MASK (0x1ul << EFUCTL_INT_ST_COMPL_POS)
#define EFUCTL_INT_ST_CMDERR_ADDR (uint32_t)&(EFUCTL->INT_ST)
#define EFUCTL_INT_ST_CMDERR_POS (1) /*< bit[1]      */
#define EFUCTL_INT_ST_CMDERR_MASK (0x1ul << EFUCTL_INT_ST_CMDERR_POS)

#define EFUCTL_INT_EN_ADDR (uint32_t)&(EFUCTL->INT_EN)
#define EFUCTL_INT_EN_COMPL_ADDR (uint32_t)&(EFUCTL->INT_EN)
#define EFUCTL_INT_EN_COMPL_POS (0) /*< bit[0]      */
#define EFUCTL_INT_EN_COMPL_MASK (0x1ul << EFUCTL_INT_EN_COMPL_POS)
#define EFUCTL_INT_EN_CMDERR_ADDR (uint32_t)&(EFUCTL->INT_EN)
#define EFUCTL_INT_EN_CMDERR_POS (1) /*< bit[1]      */
#define EFUCTL_INT_EN_CMDERR_MASK (0x1ul << EFUCTL_INT_EN_CMDERR_POS)

#define EFUCTL_INT_ADDR (uint32_t)&(EFUCTL->INT)
#define EFUCTL_INT_COMPL_ADDR (uint32_t)&(EFUCTL->INT)
#define EFUCTL_INT_COMPL_POS (0) /*< bit[0]      */
#define EFUCTL_INT_COMPL_MASK (0x1ul << EFUCTL_INT_COMPL_POS)
#define EFUCTL_INT_CMDERR_ADDR (uint32_t)&(EFUCTL->INT)
#define EFUCTL_INT_CMDERR_POS (1) /*< bit[1]      */
#define EFUCTL_INT_CMDERR_MASK (0x1ul << EFUCTL_INT_CMDERR_POS)

#define EFUCTL_XIP_REG_ADDR (uint32_t)&(EFUCTL->XIP_REG)
#define EFUCTL_XIP_KEY_RELOAD_ADDR (uint32_t)&(EFUCTL->XIP_REG)
#define EFUCTL_XIP_KEY_RELOAD_POS (0) /*< bit[0]      */
#define EFUCTL_XIP_KEY_RELOAD_MASK (0x1ul << EFUCTL_XIP_KEY_RELOAD_POS)

#define EFUCTL_ID0_ADDR (uint32_t)&(EFUCTL->ID0)
#define EFUCTL_EFUCTL_ID_LEN_ADDR (uint32_t)&(EFUCTL->ID0)
#define EFUCTL_EFUCTL_ID_LEN_POS (0) /*< bit[3:0]    */
#define EFUCTL_EFUCTL_ID_LEN_MASK (0xFul << EFUCTL_EFUCTL_ID_LEN_POS)

#define EFUCTL_ID1_ADDR (uint32_t)&(EFUCTL->ID1)
#define EFUCTL_BASE 0x40013000UL
#define EFUCTL ((EFUCTL_T *)EFUCTL_BASE)

#endif /* __EFUCTL_REG_ER8130_H__ */

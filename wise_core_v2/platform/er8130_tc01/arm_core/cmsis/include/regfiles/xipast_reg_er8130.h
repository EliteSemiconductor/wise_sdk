/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_xip_ast_xs_c1_20231212.xlsx */
/* ================================================================================
 */
/* ================                    XIPAST_T ================ */
/* ================================================================================
 */
#ifndef __XIPAST_REG_ER8130_H__
#define __XIPAST_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief XIPAST (XIPAST_T)
 */

typedef struct {           /*!< XIPAST_T Structure           */
    __IO uint32_t CTL_REG; /*!< CTL_REG Register (0x000) */
    __IO uint32_t SET_REG; /*!< SET_REG Register (0x004) */
    __IO uint32_t ADR_REG; /*!< ADR_REG Register (0x008) */
} XIPAST_T;

#define XIPAST_CTL_REG_ADDR (uint32_t)&(XIPAST->CTL_REG)
#define XIPAST_XIP_AST_RUN_ADDR (uint32_t)&(XIPAST->CTL_REG)
#define XIPAST_XIP_AST_RUN_POS (0) /*< bit[0]      */
#define XIPAST_XIP_AST_RUN_MASK (0x1ul << XIPAST_XIP_AST_RUN_POS)

#define XIPAST_SET_REG_ADDR (uint32_t)&(XIPAST->SET_REG)
#define XIPAST_XIP_AST_LEN_ADDR (uint32_t)&(XIPAST->SET_REG)
#define XIPAST_XIP_AST_LEN_POS (0) /*< bit[8:0]    */
#define XIPAST_XIP_AST_LEN_MASK (0x1FFul << XIPAST_XIP_AST_LEN_POS)
#define XIPAST_XIP_AST_CMD_ADDR (uint32_t)&(XIPAST->SET_REG)
#define XIPAST_XIP_AST_CMD_POS (24) /*< bit[31:24]  */
#define XIPAST_XIP_AST_CMD_MASK (0xFFul << XIPAST_XIP_AST_CMD_POS)

#define XIPAST_ADR_REG_ADDR (uint32_t)&(XIPAST->ADR_REG)
#define XIPAST_XIP_AST_ADR_ADDR (uint32_t)&(XIPAST->ADR_REG)
#define XIPAST_XIP_AST_ADR_POS (0) /*< bit[23:0]   */
#define XIPAST_XIP_AST_ADR_MASK (0xFFFFFFul << XIPAST_XIP_AST_ADR_POS)

#define XIPAST_BASE 0x4003C000UL
#define XIPAST ((XIPAST_T *)XIPAST_BASE)

#endif /* __XIPAST_REG_ER8130_H__ */

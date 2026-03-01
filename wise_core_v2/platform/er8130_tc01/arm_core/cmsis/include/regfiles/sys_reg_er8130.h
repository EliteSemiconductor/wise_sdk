/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_sys_ctrl_xs_d1_20241022.xlsx */
/* ================================================================================
 */
/* ================                      SYS_T ================ */
/* ================================================================================
 */
#ifndef __SYS_REG_ER8130_H__
#define __SYS_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief SYS (SYS_T)
 */

typedef struct { /*!< SOC_SYS_T Structure */
    __I uint32_t RESERVED0[44];
    __O uint32_t SYS_REMAP_TRG_REG;  /*!< SYS_REMAP_TRG_REG Register (0x0B0) */
    __IO uint32_t SYS_REMAP_ADR_REG; /*!< SYS_REMAP_ADR_REG Register (0x0B4) */
    __IO uint32_t SYS_GPIO_REG;      /*!< SYS_GPIO_ADR_REG Register (0x0B8) */
    __I uint32_t RESERVED1[17];
    __IO uint32_t PRODUCT_ID_REG; /*!< PRODUCT_ID_REG Register (0x100) */
    __I uint32_t REVISION_ID_REG; /*!< REVISION_ID_REG Register (0x104) */
    __I uint32_t RESERVED2[830];
    __IO uint32_t SRAM_OP_REG; /*!< SRAM_OP_REG Register (0xE00) */
    __I uint32_t RESERVED3[115];
    __IO uint32_t LOCK; /*!< LOCK Register (0xFD0) */
} SOC_SYS_T;

#define SYS_REMAP_TRG_REG_ADDR (uint32_t)&(SOC_SYS->SYS_REMAP_TRG_REG)
#define SYS_W1T_REMAP_RST_ADDR (uint32_t)&(SOC_SYS->SYS_REMAP_TRG_REG)
#define SYS_W1T_REMAP_RST_POS (0) /*< bit[0]      */
#define SYS_W1T_REMAP_RST_MASK (0x1ul << SYS_W1T_REMAP_RST_POS)

#define SYS_REMAP_ADR_REG_ADDR (uint32_t)&(SOC_SYS->SYS_REMAP_ADR_REG)
#define SYS_REMAP_ADROFFSET_ADDR (uint32_t)&(SOC_SYS->SYS_REMAP_ADR_REG)
#define SYS_REMAP_ADROFFSET_POS (12) /*< bit[27:12]  */
#define SYS_REMAP_ADROFFSET_MASK (0xFFFFul << SYS_REMAP_ADROFFSET_POS)

#define SYS_GPIO_REG_ADDR (uint32_t)&(SOC_SYS->SYS_GPIO_REG)
#define SYS_SWD_GPIO_EN_ADDR (uint32_t)&(SOC_SYS->SYS_GPIO_REG)
#define SYS_SWD_GPIO_EN_POS (0) /*< bit[0]      */
#define SYS_SWD_GPIO_EN_MASK (0x1ul << SYS_SWD_GPIO_EN_POS)
#define SYS_SWD_GPIO_PU_ADDR (uint32_t)&(SOC_SYS->SYS_GPIO_REG)
#define SYS_SWD_GPIO_PU_POS (1) /*< bit[1]      */
#define SYS_SWD_GPIO_PU_MASK (0x1ul << SYS_SWD_GPIO_PU_POS)
#define SYS_TCXO_GPIO_EN_ADDR (uint32_t)&(SOC_SYS->SYS_GPIO_REG)
#define SYS_TCXO_GPIO_EN_POS (4) /*< bit[4]      */
#define SYS_TCXO_GPIO_EN_MASK (0x1ul << SYS_TCXO_GPIO_EN_POS)

#define SYS_PRODUCT_ID_REG_ADDR (uint32_t)&(SOC_SYS->PRODUCT_ID_REG)
#define SYS_REVISION_ID_REG_ADDR (uint32_t)&(SOC_SYS->REVISION_ID_REG)
#define SYS_REVISION_ID_ADDR (uint32_t)&(SOC_SYS->REVISION_ID_REG)
#define SYS_REVISION_ID_POS (0) /*< bit[1:0]    */
#define SYS_REVISION_ID_MASK (0x3ul << SYS_REVISION_ID_POS)

#define SYS_SRAM_OP_REG_ADDR (uint32_t)&(SOC_SYS->SRAM_OP_REG)
#define SYS_SRAM_BIST_MOD_ADDR (uint32_t)&(SOC_SYS->SRAM_OP_REG)
#define SYS_SRAM_BIST_MOD_POS (0) /*< bit[0]      */
#define SYS_SRAM_BIST_MOD_MASK (0x1ul << SYS_SRAM_BIST_MOD_POS)
#define SYS_SRAM_BIST_EN_ADDR (uint32_t)&(SOC_SYS->SRAM_OP_REG)
#define SYS_SRAM_BIST_EN_POS (1) /*< bit[1]      */
#define SYS_SRAM_BIST_EN_MASK (0x1ul << SYS_SRAM_BIST_EN_POS)
#define SYS_SRAM_BIST_FINISH_ADDR (uint32_t)&(SOC_SYS->SRAM_OP_REG)
#define SYS_SRAM_BIST_FINISH_POS (8) /*< bit[8]      */
#define SYS_SRAM_BIST_FINISH_MASK (0x1ul << SYS_SRAM_BIST_FINISH_POS)
#define SYS_SRAM_BIST_FAIL_ADDR (uint32_t)&(SOC_SYS->SRAM_OP_REG)
#define SYS_SRAM_BIST_FAIL_POS (9) /*< bit[9]      */
#define SYS_SRAM_BIST_FAIL_MASK (0x1ul << SYS_SRAM_BIST_FAIL_POS)

#define SOC_SYS_LOCK_ADDR (uint32_t)&(SOC_SYS->LOCK)

#define SOC_SYS_BASE 0x4000F000UL
#define SOC_SYS ((SOC_SYS_T *)SOC_SYS_BASE)

#endif

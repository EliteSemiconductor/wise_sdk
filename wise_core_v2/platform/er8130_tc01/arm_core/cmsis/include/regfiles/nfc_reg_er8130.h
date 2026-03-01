/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 *
 * All rights reserved.
 *
 */

#ifndef __NFC_REG_ER8130_H__
#define __NFC_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/* ================================================================================ */
/* ================                      NFC_T                     ================ */
/* ================================================================================ */

/**
  * @brief NFC (NFC_T)
  */

 typedef struct {                                          /*!< NFC_T Structure                                                            */
  __I  uint32_t RESERVED0[255];
  __IO uint32_t DPE;                                       /*!< DPE Register (0x3FC)                                                       */
  __I  uint32_t RESERVED1[64];
  __IO uint32_t INT_ST;                                    /*!< INT_ST Register (0x500)                                                    */
  __IO uint32_t INT_EN;                                    /*!< INT_EN Register (0x504)                                                    */
  __I  uint32_t INT;                                       /*!< INT Register (0x508)                                                       */
  __IO uint32_t FEA_CFG;                                   /*!< FEA_CFG Register (0x50C)                                                   */
  __IO uint32_t SRAM_CFG;                                  /*!< SRAM_CFG Register (0x510)                                                  */
  __IO uint32_t SRAM_CTRL;                                 /*!< SRAM_CTRL Register (0x514)                                                 */
  __I  uint32_t STS_REG;                                   /*!< STS_REG Register (0x518)                                                   */
  __I  uint32_t RESERVED2[121];
  __IO uint32_t CONFIG;                                    /*!< CONFIG Register (0x700)                                                    */
} NFC_T;

#define NFC_DPE_ADDR                                       (uint32_t)&(NFC->DPE)
#define NFC_DPE_POR_ADDR                                   (uint32_t)&(NFC->DPE)
#define NFC_DPE_POR_POS                                    (17)     /*< bit[17]     */
#define NFC_DPE_POR_MASK                                   (0x1ul << NFC_DPE_POR_POS)
#define NFC_DPE_RCVCMD_ADDR                                (uint32_t)&(NFC->DPE)
#define NFC_DPE_RCVCMD_POS                                 (18)     /*< bit[18]     */
#define NFC_DPE_RCVCMD_MASK                                (0x1ul << NFC_DPE_RCVCMD_POS)
#define NFC_DPE_TXEN_ADDR                                  (uint32_t)&(NFC->DPE)
#define NFC_DPE_TXEN_POS                                   (19)     /*< bit[19]     */
#define NFC_DPE_TXEN_MASK                                  (0x1ul << NFC_DPE_TXEN_POS)
#define NFC_DPE_STATUS_4_ADDR                              (uint32_t)&(NFC->DPE)
#define NFC_DPE_STATUS_4_POS                               (20)     /*< bit[20]     */
#define NFC_DPE_STATUS_4_MASK                              (0x1ul << NFC_DPE_STATUS_4_POS)
#define NFC_DPE_STATUS_5_ADDR                              (uint32_t)&(NFC->DPE)
#define NFC_DPE_STATUS_5_POS                               (21)     /*< bit[21]     */
#define NFC_DPE_STATUS_5_MASK                              (0x1ul << NFC_DPE_STATUS_5_POS)
#define NFC_SET_LOCK_OR_NFC_IS_LOCKED_ADDR                 (uint32_t)&(NFC->DPE)
#define NFC_SET_LOCK_OR_NFC_IS_LOCKED_POS                  (22)     /*< bit[22]     */
#define NFC_SET_LOCK_OR_NFC_IS_LOCKED_MASK                 (0x1ul << NFC_SET_LOCK_OR_NFC_IS_LOCKED_POS)
#define NFC_HOST_IS_LOCKED_OR_CLR_HOST_LOCK_ADDR           (uint32_t)&(NFC->DPE)
#define NFC_HOST_IS_LOCKED_OR_CLR_HOST_LOCK_POS            (23)     /*< bit[23]     */
#define NFC_HOST_IS_LOCKED_OR_CLR_HOST_LOCK_MASK           (0x1ul << NFC_HOST_IS_LOCKED_OR_CLR_HOST_LOCK_POS)
#define NFC_DPE_IRQ_SRC_ADDR                               (uint32_t)&(NFC->DPE)
#define NFC_DPE_IRQ_SRC_POS                                (24)     /*< bit[26:24]  */
#define NFC_DPE_IRQ_SRC_MASK                               (0x7ul << NFC_DPE_IRQ_SRC_POS)
#define NFC_DPE_IRQ_CLR_ADDR                               (uint32_t)&(NFC->DPE)
#define NFC_DPE_IRQ_CLR_POS                                (27)     /*< bit[27]     */
#define NFC_DPE_IRQ_CLR_MASK                               (0x1ul << NFC_DPE_IRQ_CLR_POS)
#define NFC_DEP_RESET_ADDR                                 (uint32_t)&(NFC->DPE)
#define NFC_DEP_RESET_POS                                  (28)     /*< bit[28]     */
#define NFC_DEP_RESET_MASK                                 (0x1ul << NFC_DEP_RESET_POS)
#define NFC_DPE_NFC_IS_ACCESSING_ADDR                      (uint32_t)&(NFC->DPE)
#define NFC_DPE_NFC_IS_ACCESSING_POS                       (29)     /*< bit[29]     */
#define NFC_DPE_NFC_IS_ACCESSING_MASK                      (0x1ul << NFC_DPE_NFC_IS_ACCESSING_POS)
#define NFC_DPE_HOST_IS_ACCESSING_ADDR                     (uint32_t)&(NFC->DPE)
#define NFC_DPE_HOST_IS_ACCESSING_POS                      (30)     /*< bit[30]     */
#define NFC_DPE_HOST_IS_ACCESSING_MASK                     (0x1ul << NFC_DPE_HOST_IS_ACCESSING_POS)

#define NFC_INT_ST_ADDR                                    (uint32_t)&(NFC->INT_ST)
#define NFC_INT_ST_NFC_EVT_ADDR                            (uint32_t)&(NFC->INT_ST)
#define NFC_INT_ST_NFC_EVT_POS                             (0)      /*< bit[0]      */
#define NFC_INT_ST_NFC_EVT_MASK                            (0x1ul << NFC_INT_ST_NFC_EVT_POS)

#define NFC_INT_EN_ADDR                                    (uint32_t)&(NFC->INT_EN)
#define NFC_INT_EN_NFC_EVT_ADDR                            (uint32_t)&(NFC->INT_EN)
#define NFC_INT_EN_NFC_EVT_POS                             (0)      /*< bit[0]      */
#define NFC_INT_EN_NFC_EVT_MASK                            (0x1ul << NFC_INT_EN_NFC_EVT_POS)

#define NFC_INT_ADDR                                       (uint32_t)&(NFC->INT)
#define NFC_INT_NFC_EVT_ADDR                               (uint32_t)&(NFC->INT)
#define NFC_INT_NFC_EVT_POS                                (0)      /*< bit[0]      */
#define NFC_INT_NFC_EVT_MASK                               (0x1ul << NFC_INT_NFC_EVT_POS)

#define NFC_FEA_CFG_ADDR                                   (uint32_t)&(NFC->FEA_CFG)
#define NFC_NFC_POR_RST_EN_ADDR                            (uint32_t)&(NFC->FEA_CFG)
#define NFC_NFC_POR_RST_EN_POS                             (0)      /*< bit[0]      */
#define NFC_NFC_POR_RST_EN_MASK                            (0x1ul << NFC_NFC_POR_RST_EN_POS)

#define NFC_SRAM_CFG_ADDR                                  (uint32_t)&(NFC->SRAM_CFG)
#define NFC_NFC_SRAM_FULL_EN_ADDR                          (uint32_t)&(NFC->SRAM_CFG)
#define NFC_NFC_SRAM_FULL_EN_POS                           (0)      /*< bit[0]      */
#define NFC_NFC_SRAM_FULL_EN_MASK                          (0x1ul << NFC_NFC_SRAM_FULL_EN_POS)
#define NFC_NFC_MEM_LCK_MOD_ADDR                           (uint32_t)&(NFC->SRAM_CFG)
#define NFC_NFC_MEM_LCK_MOD_POS                            (2)      /*< bit[2]      */
#define NFC_NFC_MEM_LCK_MOD_MASK                           (0x1ul << NFC_NFC_MEM_LCK_MOD_POS)
#define NFC_NFC_RPT_BSY_EN_ADDR                            (uint32_t)&(NFC->SRAM_CFG)
#define NFC_NFC_RPT_BSY_EN_POS                             (3)      /*< bit[3]      */
#define NFC_NFC_RPT_BSY_EN_MASK                            (0x1ul << NFC_NFC_RPT_BSY_EN_POS)

#define NFC_SRAM_CTRL_ADDR                                 (uint32_t)&(NFC->SRAM_CTRL)
#define NFC_CLR_NFC_LOCK_ADDR                              (uint32_t)&(NFC->SRAM_CTRL)
#define NFC_CLR_NFC_LOCK_POS                               (0)      /*< bit[0]      */
#define NFC_CLR_NFC_LOCK_MASK                              (0x1ul << NFC_CLR_NFC_LOCK_POS)
#define NFC_SET_MEM_LOCK_ADDR                              (uint32_t)&(NFC->SRAM_CTRL)
#define NFC_SET_MEM_LOCK_POS                               (4)      /*< bit[4]      */
#define NFC_SET_MEM_LOCK_MASK                              (0x1ul << NFC_SET_MEM_LOCK_POS)

#define NFC_STS_REG_ADDR                                   (uint32_t)&(NFC->STS_REG)
#define NFC_NFC_IS_LOCKED_ADDR                             (uint32_t)&(NFC->STS_REG)
#define NFC_NFC_IS_LOCKED_POS                              (0)      /*< bit[0]      */
#define NFC_NFC_IS_LOCKED_MASK                             (0x1ul << NFC_NFC_IS_LOCKED_POS)
#define NFC_NFC_IS_ACCESSING_ADDR                          (uint32_t)&(NFC->STS_REG)
#define NFC_NFC_IS_ACCESSING_POS                           (1)      /*< bit[1]      */
#define NFC_NFC_IS_ACCESSING_MASK                          (0x1ul << NFC_NFC_IS_ACCESSING_POS)
#define NFC_HOST_IS_LOCKED_ADDR                            (uint32_t)&(NFC->STS_REG)
#define NFC_HOST_IS_LOCKED_POS                             (4)      /*< bit[4]      */
#define NFC_HOST_IS_LOCKED_MASK                            (0x1ul << NFC_HOST_IS_LOCKED_POS)
#define NFC_HOST_IS_ACCESSING_ADDR                         (uint32_t)&(NFC->STS_REG)
#define NFC_HOST_IS_ACCESSING_POS                          (5)      /*< bit[5]      */
#define NFC_HOST_IS_ACCESSING_MASK                         (0x1ul << NFC_HOST_IS_ACCESSING_POS)

#define NFC_CONFIG_ADDR                                    (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_DEMO_LOAD_ADDR                             (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_DEMO_LOAD_POS                              (4)      /*< bit[5:4]    */
#define NFC_NFC_DEMO_LOAD_MASK                             (0x3ul << NFC_NFC_DEMO_LOAD_POS)
#define NFC_NFC_MOD_LEVEL_ADDR                             (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_MOD_LEVEL_POS                              (6)      /*< bit[8:6]    */
#define NFC_NFC_MOD_LEVEL_MASK                             (0x7ul << NFC_NFC_MOD_LEVEL_POS)
#define NFC_NFC_IMP_SEL_ADDR                               (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_IMP_SEL_POS                                (10)     /*< bit[12:10]  */
#define NFC_NFC_IMP_SEL_MASK                               (0x7ul << NFC_NFC_IMP_SEL_POS)
#define NFC_NFC_LDO_LV_ADDR                                (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_LDO_LV_POS                                 (14)     /*< bit[15:14]  */
#define NFC_NFC_LDO_LV_MASK                                (0x3ul << NFC_NFC_LDO_LV_POS)
#define NFC_NFC_EN_CLK_TST_ADDR                            (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_EN_CLK_TST_POS                             (16)     /*< bit[16]     */
#define NFC_NFC_EN_CLK_TST_MASK                            (0x1ul << NFC_NFC_EN_CLK_TST_POS)
#define NFC_NFC_EN_DEMO_TST_ADDR                           (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_EN_DEMO_TST_POS                            (17)     /*< bit[17]     */
#define NFC_NFC_EN_DEMO_TST_MASK                           (0x1ul << NFC_NFC_EN_DEMO_TST_POS)
#define NFC_NFC_DTST_SEL_ADDR                              (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_DTST_SEL_POS                               (21)     /*< bit[22:21]  */
#define NFC_NFC_DTST_SEL_MASK                              (0x3ul << NFC_NFC_DTST_SEL_POS)
#define NFC_NFC_POR_ADDR                                   (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_POR_POS                                    (23)     /*< bit[23]     */
#define NFC_NFC_POR_MASK                                   (0x1ul << NFC_NFC_POR_POS)
#define NFC_NFC_EN_ANTI_COLI_ADDR                          (uint32_t)&(NFC->CONFIG)
#define NFC_NFC_EN_ANTI_COLI_POS                           (24)     /*< bit[25:24]  */
#define NFC_NFC_EN_ANTI_COLI_MASK                          (0x3ul << NFC_NFC_EN_ANTI_COLI_POS)

#define NFC_BASE                                          0x40021000UL
#define NFC                                               ((NFC_T               *) NFC_BASE)



#endif /* __NFC_REG_ER8130_H__ */


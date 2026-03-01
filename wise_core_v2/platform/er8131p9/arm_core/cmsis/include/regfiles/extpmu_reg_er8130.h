/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_ext_pmu_xs_d1_20241024.xlsx */
/* ================================================================================
 */
/* ================                    EXTPMU_T ================ */
/* ================================================================================
 */
#ifndef __EXTPMU_REG_ER8130_H__
#define __EXTPMU_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief EXTPMU (EXTPMU_T)
 */

typedef struct {                                          /*!< EXTPMU_T Structure                                                         */
  __IO uint32_t EXT_PMU_GLO_REG;                           /*!< EXT_PMU_GLO_REG Register (0x000)                                           */
  __IO uint32_t EXT_PMU_CTL_REG;                           /*!< EXT_PMU_CTL_REG Register (0x004)                                           */
  __IO uint32_t EXT_PMU_CFG_REG;                           /*!< EXT_PMU_CFG_REG Register (0x008)                                           */
  __IO uint32_t EXT_PMU_TIM_REG;                           /*!< EXT_PMU_TIM_REG Register (0x00C)                                           */
  __I  uint32_t EXT_PMU_STS_REG;                           /*!< EXT_PMU_STS_REG Register (0x010)                                           */
  __IO uint32_t EXT_PMU_WU_REG;                            /*!< EXT_PMU_WU_REG Register (0x014)                                            */
  __IO uint32_t EXT_PMU_CFG2_REG;                          /*!< EXT_PMU_CFG2_REG Register (0x018)                                          */
  __I  uint32_t RESERVED0;
  __IO uint32_t EXT_PMU_TMR_PROD_REG;                      /*!< EXT_PMU_TMR_PROD_REG Register (0x020)                                      */
  __I  uint32_t RESERVED1[3];
  __IO uint32_t EXT_PMU_INT_ST;                            /*!< EXT_PMU_INT_ST Register (0x030)                                            */
  __IO uint32_t EXT_PMU_INT_EN;                            /*!< EXT_PMU_INT_EN Register (0x034)                                            */
  __I  uint32_t EXT_PMU_INT;                               /*!< EXT_PMU_INT Register (0x038)                                               */
  __I  uint32_t RESERVED2;
  __IO uint32_t EXT_PMU_STS2_REG;                          /*!< EXT_PMU_STS2_REG Register (0x040)                                          */
  __IO uint32_t EXT_PMU_STS3_REG;                          /*!< EXT_PMU_STS3_REG Register (0x044)                                          */
  __I  uint32_t RESERVED3[998];
  __I  uint32_t EXT_PMU_PERIPH_ID_LEN;                     /*!< EXT_PMU_PERIPH_ID_LEN Register (0xFE0)                                     */
  __I  uint32_t RESERVED4[3];
  __IO uint32_t EXT_PMU_PERIPH_ID_0;                       /*!< EXT_PMU_PERIPH_ID_0 Register (0xFF0)                                       */
} EXTPMU_T;

#define EXTPMU_EXT_PMU_GLO_REG_ADDR                        (uint32_t)&(EXTPMU->EXT_PMU_GLO_REG)
#define EXTPMU_PMU_EN_ADDR                                 (uint32_t)&(EXTPMU->EXT_PMU_GLO_REG)
#define EXTPMU_PMU_EN_POS                                  (0)      /*< bit[0]      */
#define EXTPMU_PMU_EN_MASK                                 (0x1ul << EXTPMU_PMU_EN_POS)

#define EXTPMU_EXT_PMU_CTL_REG_ADDR                        (uint32_t)&(EXTPMU->EXT_PMU_CTL_REG)
#define EXTPMU_SLEEP_EN_ADDR                               (uint32_t)&(EXTPMU->EXT_PMU_CTL_REG)
#define EXTPMU_SLEEP_EN_POS                                (0)      /*< bit[0]      */
#define EXTPMU_SLEEP_EN_MASK                               (0x1ul << EXTPMU_SLEEP_EN_POS)
#define EXTPMU_WUTMR_EN_ADDR                               (uint32_t)&(EXTPMU->EXT_PMU_CTL_REG)
#define EXTPMU_WUTMR_EN_POS                                (1)      /*< bit[1]      */
#define EXTPMU_WUTMR_EN_MASK                               (0x1ul << EXTPMU_WUTMR_EN_POS)

#define EXTPMU_EXT_PMU_CFG_REG_ADDR                        (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_XO_40M_PD_EN_ADDR                       (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_XO_40M_PD_EN_POS                        (0)      /*< bit[0]      */
#define EXTPMU_PMU_XO_40M_PD_EN_MASK                       (0x1ul << EXTPMU_PMU_XO_40M_PD_EN_POS)
#define EXTPMU_PMU_XO_40M_GAIN_EN_ADDR                     (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_XO_40M_GAIN_EN_POS                      (1)      /*< bit[1]      */
#define EXTPMU_PMU_XO_40M_GAIN_EN_MASK                     (0x1ul << EXTPMU_PMU_XO_40M_GAIN_EN_POS)
#define EXTPMU_PMU_BGR_PD_EN_ADDR                          (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_BGR_PD_EN_POS                           (2)      /*< bit[2]      */
#define EXTPMU_PMU_BGR_PD_EN_MASK                          (0x1ul << EXTPMU_PMU_BGR_PD_EN_POS)
#define EXTPMU_PMU_DIG_LDO_PD_EN_ADDR                      (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_DIG_LDO_PD_EN_POS                       (3)      /*< bit[3]      */
#define EXTPMU_PMU_DIG_LDO_PD_EN_MASK                      (0x1ul << EXTPMU_PMU_DIG_LDO_PD_EN_POS)
#define EXTPMU_PMU_DCDC_PD12_EN_ADDR                       (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_DCDC_PD12_EN_POS                        (4)      /*< bit[4]      */
#define EXTPMU_PMU_DCDC_PD12_EN_MASK                       (0x1ul << EXTPMU_PMU_DCDC_PD12_EN_POS)
#define EXTPMU_PMU_DIG_LDOVDD_PG_PD_EN_ADDR                (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_DIG_LDOVDD_PG_PD_EN_POS                 (5)      /*< bit[5]      */
#define EXTPMU_PMU_DIG_LDOVDD_PG_PD_EN_MASK                (0x1ul << EXTPMU_PMU_DIG_LDOVDD_PG_PD_EN_POS)
#define EXTPMU_PMU_DIG_ULPLDOVDD_LV_EN_ADDR                (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_DIG_ULPLDOVDD_LV_EN_POS                 (6)      /*< bit[6]      */
#define EXTPMU_PMU_DIG_ULPLDOVDD_LV_EN_MASK                (0x1ul << EXTPMU_PMU_DIG_ULPLDOVDD_LV_EN_POS)
#define EXTPMU_PMU_32K_SHDN_EN_ADDR                        (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_32K_SHDN_EN_POS                         (7)      /*< bit[7]      */
#define EXTPMU_PMU_32K_SHDN_EN_MASK                        (0x1ul << EXTPMU_PMU_32K_SHDN_EN_POS)
#define EXTPMU_PMU_16K_SHDN_EN_ADDR                        (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_16K_SHDN_EN_POS                         (8)      /*< bit[8]      */
#define EXTPMU_PMU_16K_SHDN_EN_MASK                        (0x1ul << EXTPMU_PMU_16K_SHDN_EN_POS)
#define EXTPMU_PMU_XO_40M_BYCAP_EN_ADDR                    (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_XO_40M_BYCAP_EN_POS                     (9)      /*< bit[9]      */
#define EXTPMU_PMU_XO_40M_BYCAP_EN_MASK                    (0x1ul << EXTPMU_PMU_XO_40M_BYCAP_EN_POS)
#define EXTPMU_PMU_DIG_ULPLDOVDD_LV_S_ADDR                 (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_DIG_ULPLDOVDD_LV_S_POS                  (24)     /*< bit[27:24]  */
#define EXTPMU_PMU_DIG_ULPLDOVDD_LV_S_MASK                 (0xFul << EXTPMU_PMU_DIG_ULPLDOVDD_LV_S_POS)
#define EXTPMU_PMU_DCDC_TAR_ADDR                           (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_DCDC_TAR_POS                            (28)     /*< bit[29:28]  */
#define EXTPMU_PMU_DCDC_TAR_MASK                           (0x3ul << EXTPMU_PMU_DCDC_TAR_POS)
#define EXTPMU_PMU_CLK_SRC_ADDR                            (uint32_t)&(EXTPMU->EXT_PMU_CFG_REG)
#define EXTPMU_PMU_CLK_SRC_POS                             (30)     /*< bit[30]     */
#define EXTPMU_PMU_CLK_SRC_MASK                            (0x1ul << EXTPMU_PMU_CLK_SRC_POS)

#define EXTPMU_EXT_PMU_TIM_REG_ADDR                        (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_S_LDO_ADDR                          (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_S_LDO_POS                           (0)      /*< bit[1:0]    */
#define EXTPMU_PMU_TIM_S_LDO_MASK                          (0x3ul << EXTPMU_PMU_TIM_S_LDO_POS)
#define EXTPMU_PMU_TIM_A_LDO_ADDR                          (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_A_LDO_POS                           (2)      /*< bit[3:2]    */
#define EXTPMU_PMU_TIM_A_LDO_MASK                          (0x3ul << EXTPMU_PMU_TIM_A_LDO_POS)
#define EXTPMU_PMU_TIM_S_DCDC_ADDR                         (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_S_DCDC_POS                          (4)      /*< bit[5:4]    */
#define EXTPMU_PMU_TIM_S_DCDC_MASK                         (0x3ul << EXTPMU_PMU_TIM_S_DCDC_POS)
#define EXTPMU_PMU_TIM_A_DCDC_ADDR                         (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_A_DCDC_POS                          (6)      /*< bit[9:6]    */
#define EXTPMU_PMU_TIM_A_DCDC_MASK                         (0xFul << EXTPMU_PMU_TIM_A_DCDC_POS)
#define EXTPMU_PMU_TIM_S_LDOVDD_LV_ADDR                    (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_S_LDOVDD_LV_POS                     (10)     /*< bit[11:10]  */
#define EXTPMU_PMU_TIM_S_LDOVDD_LV_MASK                    (0x3ul << EXTPMU_PMU_TIM_S_LDOVDD_LV_POS)
#define EXTPMU_PMU_TIM_A_LDOVDD_LV_ADDR                    (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_A_LDOVDD_LV_POS                     (12)     /*< bit[13:12]  */
#define EXTPMU_PMU_TIM_A_LDOVDD_LV_MASK                    (0x3ul << EXTPMU_PMU_TIM_A_LDOVDD_LV_POS)
#define EXTPMU_PMU_TIM_A_32K_ADDR                          (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_A_32K_POS                           (14)     /*< bit[15:14]  */
#define EXTPMU_PMU_TIM_A_32K_MASK                          (0x3ul << EXTPMU_PMU_TIM_A_32K_POS)
#define EXTPMU_PMU_TIM_A_40M_GAIN_ADDR                     (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_A_40M_GAIN_POS                      (16)     /*< bit[19:16]  */
#define EXTPMU_PMU_TIM_A_40M_GAIN_MASK                     (0xFul << EXTPMU_PMU_TIM_A_40M_GAIN_POS)
#define EXTPMU_PMU_TIM_A_DIG_ADDR                          (uint32_t)&(EXTPMU->EXT_PMU_TIM_REG)
#define EXTPMU_PMU_TIM_A_DIG_POS                           (20)     /*< bit[21:20]  */
#define EXTPMU_PMU_TIM_A_DIG_MASK                          (0x3ul << EXTPMU_PMU_TIM_A_DIG_POS)

#define EXTPMU_EXT_PMU_STS_REG_ADDR                        (uint32_t)&(EXTPMU->EXT_PMU_STS_REG)
#define EXTPMU_EXT_PMU_STS_ADDR                            (uint32_t)&(EXTPMU->EXT_PMU_STS_REG)
#define EXTPMU_EXT_PMU_STS_POS                             (0)      /*< bit[5:0]    */
#define EXTPMU_EXT_PMU_STS_MASK                            (0x3Ful << EXTPMU_EXT_PMU_STS_POS)

#define EXTPMU_EXT_PMU_WU_REG_ADDR                         (uint32_t)&(EXTPMU->EXT_PMU_WU_REG)
#define EXTPMU_PMU_WUBY_WUTMR_ADDR                         (uint32_t)&(EXTPMU->EXT_PMU_WU_REG)
#define EXTPMU_PMU_WUBY_WUTMR_POS                          (0)      /*< bit[0]      */
#define EXTPMU_PMU_WUBY_WUTMR_MASK                         (0x1ul << EXTPMU_PMU_WUBY_WUTMR_POS)
#define EXTPMU_PMU_WUBY_NFC_ADDR                           (uint32_t)&(EXTPMU->EXT_PMU_WU_REG)
#define EXTPMU_PMU_WUBY_NFC_POS                            (3)      /*< bit[3]      */
#define EXTPMU_PMU_WUBY_NFC_MASK                           (0x1ul << EXTPMU_PMU_WUBY_NFC_POS)
#define EXTPMU_PMU_WUBY_GPIO_ADDR                          (uint32_t)&(EXTPMU->EXT_PMU_WU_REG)
#define EXTPMU_PMU_WUBY_GPIO_POS                           (4)      /*< bit[4]      */
#define EXTPMU_PMU_WUBY_GPIO_MASK                          (0x1ul << EXTPMU_PMU_WUBY_GPIO_POS)

#define EXTPMU_EXT_PMU_CFG2_REG_ADDR                       (uint32_t)&(EXTPMU->EXT_PMU_CFG2_REG)
#define EXTPMU_PMU_TIM_DCDC_BYPASS_ADDR                    (uint32_t)&(EXTPMU->EXT_PMU_CFG2_REG)
#define EXTPMU_PMU_TIM_DCDC_BYPASS_POS                     (0)      /*< bit[1:0]    */
#define EXTPMU_PMU_TIM_DCDC_BYPASS_MASK                    (0x3ul << EXTPMU_PMU_TIM_DCDC_BYPASS_POS)
#define EXTPMU_PMU_DCDC_BYPASS_EN_ADDR                     (uint32_t)&(EXTPMU->EXT_PMU_CFG2_REG)
#define EXTPMU_PMU_DCDC_BYPASS_EN_POS                      (4)      /*< bit[7:4]    */
#define EXTPMU_PMU_DCDC_BYPASS_EN_MASK                     (0xFul << EXTPMU_PMU_DCDC_BYPASS_EN_POS)
#define EXTPMU_PMU_WUBY_GPIO_TAR_ADDR                      (uint32_t)&(EXTPMU->EXT_PMU_CFG2_REG)
#define EXTPMU_PMU_WUBY_GPIO_TAR_POS                       (8)      /*< bit[11:8]   */
#define EXTPMU_PMU_WUBY_GPIO_TAR_MASK                      (0xFul << EXTPMU_PMU_WUBY_GPIO_TAR_POS)

#define EXTPMU_EXT_PMU_TMR_PROD_REG_ADDR                   (uint32_t)&(EXTPMU->EXT_PMU_TMR_PROD_REG)
#define EXTPMU_EXT_PMU_INT_ST_ADDR                         (uint32_t)&(EXTPMU->EXT_PMU_INT_ST)
#define EXTPMU_INT_ST_EXT_WAKEUP_ADDR                      (uint32_t)&(EXTPMU->EXT_PMU_INT_ST)
#define EXTPMU_INT_ST_EXT_WAKEUP_POS                       (0)      /*< bit[0]      */
#define EXTPMU_INT_ST_EXT_WAKEUP_MASK                      (0x1ul << EXTPMU_INT_ST_EXT_WAKEUP_POS)

#define EXTPMU_EXT_PMU_INT_EN_ADDR                         (uint32_t)&(EXTPMU->EXT_PMU_INT_EN)
#define EXTPMU_INT_EN_EXT_WAKEUP_ADDR                      (uint32_t)&(EXTPMU->EXT_PMU_INT_EN)
#define EXTPMU_INT_EN_EXT_WAKEUP_POS                       (0)      /*< bit[0]      */
#define EXTPMU_INT_EN_EXT_WAKEUP_MASK                      (0x1ul << EXTPMU_INT_EN_EXT_WAKEUP_POS)

#define EXTPMU_EXT_PMU_INT_ADDR                            (uint32_t)&(EXTPMU->EXT_PMU_INT)
#define EXTPMU_INT_EXT_WAKEUP_ADDR                         (uint32_t)&(EXTPMU->EXT_PMU_INT)
#define EXTPMU_INT_EXT_WAKEUP_POS                          (0)      /*< bit[0]      */
#define EXTPMU_INT_EXT_WAKEUP_MASK                         (0x1ul << EXTPMU_INT_EXT_WAKEUP_POS)

#define EXTPMU_EXT_PMU_STS2_REG_ADDR                       (uint32_t)&(EXTPMU->EXT_PMU_STS2_REG)
#define EXTPMU_EXT_PMU_STS3_REG_ADDR                       (uint32_t)&(EXTPMU->EXT_PMU_STS3_REG)
#define EXTPMU_EXT_PMU_PERIPH_ID_LEN_ADDR                  (uint32_t)&(EXTPMU->EXT_PMU_PERIPH_ID_LEN)
#define EXTPMU_PERIPH_ID_LEN_ADDR                          (uint32_t)&(EXTPMU->EXT_PMU_PERIPH_ID_LEN)
#define EXTPMU_PERIPH_ID_LEN_POS                           (0)      /*< bit[3:0]    */
#define EXTPMU_PERIPH_ID_LEN_MASK                          (0xFul << EXTPMU_PERIPH_ID_LEN_POS)


#define EXTPMU_EXT_PMU_PERIPH_ID_0_ADDR                     (uint32_t)&(EXTPMU->EXT_PMU_PERIPH_ID_0)

#define EXTPMU_BASE 0x4000A000UL
#define EXTPMU ((EXTPMU_T *)EXTPMU_BASE)

#endif /* __EXTPMU_REG_ER8130_H__ */

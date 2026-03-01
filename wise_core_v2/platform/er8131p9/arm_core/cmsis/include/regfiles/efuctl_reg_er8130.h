/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __EFUCTL_REG_ER8130_H__
#define __EFUCTL_REG_ER8130_H__

 typedef struct {                                          /*!< EFUSE_MCU_T Structure                                                      */
  __IO uint32_t EFUCTL_CTL0;                               /*!< EFUCTL_CTL0 Register (0x000)                                               */
  __IO uint32_t EFUCTL_CTL1;                               /*!< EFUCTL_CTL1 Register (0x004)                                               */
  __IO uint32_t EFUCTL_VQPS;                               /*!< EFUCTL_VQPS Register (0x008)                                               */
  __I  uint32_t RESERVED0;
  __IO uint32_t EFUCTL_INT_ST;                             /*!< EFUCTL_INT_ST Register (0x010)                                             */
  __IO uint32_t EFUCTL_INT_EN;                             /*!< EFUCTL_INT_EN Register (0x014)                                             */
  __I  uint32_t EFUCTL_INT;                                /*!< EFUCTL_INT Register (0x018)                                                */
  __I  uint32_t RESERVED1;
  __IO uint32_t EFUCTL_XIP_REG;                            /*!< EFUCTL_XIP_REG Register (0x020)                                            */
  __I  uint32_t RESERVED2[1007];
  __I  uint32_t EFUCTL_ID_REG0;                            /*!< EFUCTL_ID_REG0 Register (0xFE0)                                            */
  __I  uint32_t RESERVED3[3];
  __IO uint32_t EFUCTL_ID_REG1;                            /*!< EFUCTL_ID_REG1 Register (0xFF0)                                            */
  __IO uint32_t EFUCTL_ID_REG2;                            /*!< EFUCTL_ID_REG2 Register (0xFF4)                                            */
} EFUSE_MCU_T, EFUSE_NFC_T;

/* ================================================================================ */
/* ================                   EFUSE_MCU_T                  ================ */
/* ================================================================================ */

/**
  * @brief EFUSE_MCU (EFUSE_MCU_T)
  */

#define EFUSE_MCU_EFUCTL_CTL0_ADDR                         (uint32_t)&(EFUSE_MCU->EFUCTL_CTL0)
#define EFUSE_MCU_EFUCTL_CMD_ADDR                          (uint32_t)&(EFUSE_MCU->EFUCTL_CTL0)
#define EFUSE_MCU_EFUCTL_CMD_POS                           (0)      /*< bit[3:0]    */
#define EFUSE_MCU_EFUCTL_CMD_MASK                          (0xFul << EFUSE_MCU_EFUCTL_CMD_POS)
#define EFUSE_MCU_EFUCTL_ADR_ADDR                          (uint32_t)&(EFUSE_MCU->EFUCTL_CTL0)
#define EFUSE_MCU_EFUCTL_ADR_POS                           (16)     /*< bit[21:16]  */
#define EFUSE_MCU_EFUCTL_ADR_MASK                          (0x3Ful << EFUSE_MCU_EFUCTL_ADR_POS)

#define EFUSE_MCU_EFUCTL_CTL1_ADDR                         (uint32_t)&(EFUSE_MCU->EFUCTL_CTL1)
#define EFUSE_MCU_EFUCTL_VQPS_ADDR                         (uint32_t)&(EFUSE_MCU->EFUCTL_VQPS)
#define EFUSE_MCU_EFUCTL_VQPS_EN_ADDR                      (uint32_t)&(EFUSE_MCU->EFUCTL_VQPS)
#define EFUSE_MCU_EFUCTL_VQPS_EN_POS                       (0)      /*< bit[0]      */
#define EFUSE_MCU_EFUCTL_VQPS_EN_MASK                      (0x1ul << EFUSE_MCU_EFUCTL_VQPS_EN_POS)
#define EFUSE_MCU_EFUCTL_VQPS_STS_ADDR                     (uint32_t)&(EFUSE_MCU->EFUCTL_VQPS)
#define EFUSE_MCU_EFUCTL_VQPS_STS_POS                      (1)      /*< bit[1]      */
#define EFUSE_MCU_EFUCTL_VQPS_STS_MASK                     (0x1ul << EFUSE_MCU_EFUCTL_VQPS_STS_POS)

#define EFUSE_MCU_EFUCTL_INT_ST_ADDR                       (uint32_t)&(EFUSE_MCU->EFUCTL_INT_ST)
#define EFUSE_MCU_INT_ST_COMPL_ADDR                        (uint32_t)&(EFUSE_MCU->EFUCTL_INT_ST)
#define EFUSE_MCU_INT_ST_COMPL_POS                         (0)      /*< bit[0]      */
#define EFUSE_MCU_INT_ST_COMPL_MASK                        (0x1ul << EFUSE_MCU_INT_ST_COMPL_POS)
#define EFUSE_MCU_INT_ST_CMDERR_ADDR                       (uint32_t)&(EFUSE_MCU->EFUCTL_INT_ST)
#define EFUSE_MCU_INT_ST_CMDERR_POS                        (1)      /*< bit[1]      */
#define EFUSE_MCU_INT_ST_CMDERR_MASK                       (0x1ul << EFUSE_MCU_INT_ST_CMDERR_POS)

#define EFUSE_MCU_EFUCTL_INT_EN_ADDR                       (uint32_t)&(EFUSE_MCU->EFUCTL_INT_EN)
#define EFUSE_MCU_INT_EN_COMPL_ADDR                        (uint32_t)&(EFUSE_MCU->EFUCTL_INT_EN)
#define EFUSE_MCU_INT_EN_COMPL_POS                         (0)      /*< bit[0]      */
#define EFUSE_MCU_INT_EN_COMPL_MASK                        (0x1ul << EFUSE_MCU_INT_EN_COMPL_POS)
#define EFUSE_MCU_INT_EN_CMDERR_ADDR                       (uint32_t)&(EFUSE_MCU->EFUCTL_INT_EN)
#define EFUSE_MCU_INT_EN_CMDERR_POS                        (1)      /*< bit[1]      */
#define EFUSE_MCU_INT_EN_CMDERR_MASK                       (0x1ul << EFUSE_MCU_INT_EN_CMDERR_POS)

#define EFUSE_MCU_EFUCTL_INT_ADDR                          (uint32_t)&(EFUSE_MCU->EFUCTL_INT)
#define EFUSE_MCU_INT_COMPL_ADDR                           (uint32_t)&(EFUSE_MCU->EFUCTL_INT)
#define EFUSE_MCU_INT_COMPL_POS                            (0)      /*< bit[0]      */
#define EFUSE_MCU_INT_COMPL_MASK                           (0x1ul << EFUSE_MCU_INT_COMPL_POS)
#define EFUSE_MCU_INT_CMDERR_ADDR                          (uint32_t)&(EFUSE_MCU->EFUCTL_INT)
#define EFUSE_MCU_INT_CMDERR_POS                           (1)      /*< bit[1]      */
#define EFUSE_MCU_INT_CMDERR_MASK                          (0x1ul << EFUSE_MCU_INT_CMDERR_POS)

#define EFUSE_MCU_EFUCTL_XIP_REG_ADDR                      (uint32_t)&(EFUSE_MCU->EFUCTL_XIP_REG)
#define EFUSE_MCU_XIP_KEY_RELOAD_ADDR                      (uint32_t)&(EFUSE_MCU->EFUCTL_XIP_REG)
#define EFUSE_MCU_XIP_KEY_RELOAD_POS                       (0)      /*< bit[0]      */
#define EFUSE_MCU_XIP_KEY_RELOAD_MASK                      (0x1ul << EFUSE_MCU_XIP_KEY_RELOAD_POS)

#define EFUSE_MCU_EFUCTL_ID_REG0_ADDR                      (uint32_t)&(EFUSE_MCU->EFUCTL_ID_REG0)
#define EFUSE_MCU_EFUCTL_ID_LEN_ADDR                       (uint32_t)&(EFUSE_MCU->EFUCTL_ID_REG0)
#define EFUSE_MCU_EFUCTL_ID_LEN_POS                        (0)      /*< bit[3:0]    */
#define EFUSE_MCU_EFUCTL_ID_LEN_MASK                       (0xFul << EFUSE_MCU_EFUCTL_ID_LEN_POS)

#define EFUSE_MCU_EFUCTL_ID_REG1_ADDR                      (uint32_t)&(EFUSE_MCU->EFUCTL_ID_REG1)
#define EFUSE_MCU_EFUCTL_ID_REG2_ADDR                      (uint32_t)&(EFUSE_MCU->EFUCTL_ID_REG2)
#define EFUSE_MCU_BASE                                    0x40013000UL
#define EFUSE_MCU                                         ((EFUSE_MCU_T         *) EFUSE_MCU_BASE)

/* ================================================================================ */
/* ================                   EFUSE_NFC_T                  ================ */
/* ================================================================================ */

/**
  * @brief EFUSE_NFC (EFUSE_NFC_T)
  */

#define EFUSE_NFC_EFUCTL_CTL0_ADDR                         (uint32_t)&(EFUSE_NFC->EFUCTL_CTL0)
#define EFUSE_NFC_EFUCTL_CMD_ADDR                          (uint32_t)&(EFUSE_NFC->EFUCTL_CTL0)
#define EFUSE_NFC_EFUCTL_CMD_POS                           (0)      /*< bit[3:0]    */
#define EFUSE_NFC_EFUCTL_CMD_MASK                          (0xFul << EFUSE_NFC_EFUCTL_CMD_POS)
#define EFUSE_NFC_EFUCTL_ADR_ADDR                          (uint32_t)&(EFUSE_NFC->EFUCTL_CTL0)
#define EFUSE_NFC_EFUCTL_ADR_POS                           (16)     /*< bit[21:16]  */
#define EFUSE_NFC_EFUCTL_ADR_MASK                          (0x3Ful << EFUSE_NFC_EFUCTL_ADR_POS)

#define EFUSE_NFC_EFUCTL_CTL1_ADDR                         (uint32_t)&(EFUSE_NFC->EFUCTL_CTL1)
#define EFUSE_NFC_EFUCTL_VQPS_ADDR                         (uint32_t)&(EFUSE_NFC->EFUCTL_VQPS)
#define EFUSE_NFC_EFUCTL_VQPS_EN_ADDR                      (uint32_t)&(EFUSE_NFC->EFUCTL_VQPS)
#define EFUSE_NFC_EFUCTL_VQPS_EN_POS                       (0)      /*< bit[0]      */
#define EFUSE_NFC_EFUCTL_VQPS_EN_MASK                      (0x1ul << EFUSE_NFC_EFUCTL_VQPS_EN_POS)
#define EFUSE_NFC_EFUCTL_VQPS_STS_ADDR                     (uint32_t)&(EFUSE_NFC->EFUCTL_VQPS)
#define EFUSE_NFC_EFUCTL_VQPS_STS_POS                      (1)      /*< bit[1]      */
#define EFUSE_NFC_EFUCTL_VQPS_STS_MASK                     (0x1ul << EFUSE_NFC_EFUCTL_VQPS_STS_POS)

#define EFUSE_NFC_EFUCTL_INT_ST_ADDR                       (uint32_t)&(EFUSE_NFC->EFUCTL_INT_ST)
#define EFUSE_NFC_INT_ST_COMPL_ADDR                        (uint32_t)&(EFUSE_NFC->EFUCTL_INT_ST)
#define EFUSE_NFC_INT_ST_COMPL_POS                         (0)      /*< bit[0]      */
#define EFUSE_NFC_INT_ST_COMPL_MASK                        (0x1ul << EFUSE_NFC_INT_ST_COMPL_POS)
#define EFUSE_NFC_INT_ST_CMDERR_ADDR                       (uint32_t)&(EFUSE_NFC->EFUCTL_INT_ST)
#define EFUSE_NFC_INT_ST_CMDERR_POS                        (1)      /*< bit[1]      */
#define EFUSE_NFC_INT_ST_CMDERR_MASK                       (0x1ul << EFUSE_NFC_INT_ST_CMDERR_POS)

#define EFUSE_NFC_EFUCTL_INT_EN_ADDR                       (uint32_t)&(EFUSE_NFC->EFUCTL_INT_EN)
#define EFUSE_NFC_INT_EN_COMPL_ADDR                        (uint32_t)&(EFUSE_NFC->EFUCTL_INT_EN)
#define EFUSE_NFC_INT_EN_COMPL_POS                         (0)      /*< bit[0]      */
#define EFUSE_NFC_INT_EN_COMPL_MASK                        (0x1ul << EFUSE_NFC_INT_EN_COMPL_POS)
#define EFUSE_NFC_INT_EN_CMDERR_ADDR                       (uint32_t)&(EFUSE_NFC->EFUCTL_INT_EN)
#define EFUSE_NFC_INT_EN_CMDERR_POS                        (1)      /*< bit[1]      */
#define EFUSE_NFC_INT_EN_CMDERR_MASK                       (0x1ul << EFUSE_NFC_INT_EN_CMDERR_POS)

#define EFUSE_NFC_EFUCTL_INT_ADDR                          (uint32_t)&(EFUSE_NFC->EFUCTL_INT)
#define EFUSE_NFC_INT_COMPL_ADDR                           (uint32_t)&(EFUSE_NFC->EFUCTL_INT)
#define EFUSE_NFC_INT_COMPL_POS                            (0)      /*< bit[0]      */
#define EFUSE_NFC_INT_COMPL_MASK                           (0x1ul << EFUSE_NFC_INT_COMPL_POS)
#define EFUSE_NFC_INT_CMDERR_ADDR                          (uint32_t)&(EFUSE_NFC->EFUCTL_INT)
#define EFUSE_NFC_INT_CMDERR_POS                           (1)      /*< bit[1]      */
#define EFUSE_NFC_INT_CMDERR_MASK                          (0x1ul << EFUSE_NFC_INT_CMDERR_POS)

#define EFUSE_NFC_EFUCTL_OFFSET_REG_ADDR                   (uint32_t)&(EFUSE_NFC->EFUCTL_OFFSET_REG)
#define EFUSE_NFC_EFUCTL_NFC_ADR_OFFSET_ADDR               (uint32_t)&(EFUSE_NFC->EFUCTL_OFFSET_REG)
#define EFUSE_NFC_EFUCTL_NFC_ADR_OFFSET_POS                (0)      /*< bit[5:0]    */
#define EFUSE_NFC_EFUCTL_NFC_ADR_OFFSET_MASK               (0x3Ful << EFUSE_NFC_EFUCTL_NFC_ADR_OFFSET_POS)

#define EFUSE_NFC_EFUCTL_ID_REG0_ADDR                      (uint32_t)&(EFUSE_NFC->EFUCTL_ID_REG0)
#define EFUSE_NFC_EFUCTL_ID_LEN_ADDR                       (uint32_t)&(EFUSE_NFC->EFUCTL_ID_REG0)
#define EFUSE_NFC_EFUCTL_ID_LEN_POS                        (0)      /*< bit[3:0]    */
#define EFUSE_NFC_EFUCTL_ID_LEN_MASK                       (0xFul << EFUSE_NFC_EFUCTL_ID_LEN_POS)

#define EFUSE_NFC_EFUCTL_ID_REG1_ADDR                      (uint32_t)&(EFUSE_NFC->EFUCTL_ID_REG1)
#define EFUSE_NFC_EFUCTL_ID_REG2_ADDR                      (uint32_t)&(EFUSE_NFC->EFUCTL_ID_REG2)
#define EFUSE_NFC_BASE                                    0x40022000UL
#define EFUSE_NFC                                         ((EFUSE_NFC_T         *) EFUSE_NFC_BASE)

#endif /* __EFUCTL_REG_ER8130_H__ */

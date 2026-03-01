/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_sha_xs_b1_20231212.xlsx */
/* ================================================================================
 */
/* ================                      SHA_T ================ */
/* ================================================================================
 */
#ifndef __SHA_REG_ER8130_H__
#define __SHA_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief SHA (SHA_T)
 */
typedef struct { /*!< SHA_T Structure */
    __I uint32_t RESERVED0[8];
    __I uint32_t DIGEST[8]; /*!< SHA DIGEST OUT Word 0~7 Register */

    union {
        __IO uint32_t CTRL; /*!< SHA Control Register */

        struct {
            __IO uint32_t MODE    : 1; /*!< 0: SHA224, 1:SHA256 */
            __IO uint32_t IN_SWAP : 1; /*!< 0: no swap1: in swap */
        } CTRL_b;                      /*!< BitSize */
    };
    __IO uint32_t DATA_LEN0; /*!< DATA_LEN Register */
    __IO uint32_t DATA_LEN1; /*!< DATA_LEN Register */

    union {
        __O uint32_t SHA_DMA; /*!< SHA DMA Register */

        struct {
            __O uint32_t
                SHAEN : 1; /*!< trigger DMA sha_in channel sending data
                                      to SHA module */
        } SHA_DMA_b;       /*!< BitSize */
    };

    union {
        __IO uint32_t STATUS; /*!< SHA Status Register */

        struct {
            __IO uint32_t FSH : 1; /*!< set when SHA operation finish */
        } STATUS_b;                /*!< BitSize */
    };

    union {
        __IO uint32_t INTEN; /*!< Interrupt Enable Register */

        struct {
            __IO uint32_t FSH : 1; /*!< Set to enable SHA operation finish */
        } INTEN_b;                 /*!< BitSize */
    };

    union {
        __I uint32_t STATUS_MSK; /*!< SHA Masked Status Register */

        struct {
            __I uint32_t FSH : 1; /*!< SHA operation finish interrupt mask
                                    status */
        } STATUS_MSK_b;           /*!< BitSize */
    };

    union {
        __I uint32_t SHA_STATUS; /*!< SHA_STATUS Register */

        struct {
            __I uint32_t BUSY : 1; /*!< sha engine is busy */
        } SHA_STATUS_b;            /*!< BitSize */
    };
} SHA_T;

#define SHA_DIGEST_OUT_0_ADDR                           (uint32_t)&(SHA->DIGEST[0])
#define SHA_DIGEST_OUT_1_ADDR                           (uint32_t)&(SHA->DIGEST[1])
#define SHA_DIGEST_OUT_2_ADDR                           (uint32_t)&(SHA->DIGEST[2])
#define SHA_DIGEST_OUT_3_ADDR                           (uint32_t)&(SHA->DIGEST[3])
#define SHA_DIGEST_OUT_4_ADDR                           (uint32_t)&(SHA->DIGEST[4])
#define SHA_DIGEST_OUT_5_ADDR                           (uint32_t)&(SHA->DIGEST[5])
#define SHA_DIGEST_OUT_6_ADDR                           (uint32_t)&(SHA->DIGEST[6])
#define SHA_DIGEST_OUT_7_ADDR                           (uint32_t)&(SHA->DIGEST[7])
#define SHA_CTL_ADDR                                    (uint32_t)&(SHA->CTRL)
#define SHA_SHA_MODE_ADDR                               (uint32_t)&(SHA->CTRL)
#define SHA_SHA_MODE_POS                                (0) /*< bit[0]      */
#define SHA_SHA_MODE_MASK                               (0x1ul << SHA_SHA_MODE_POS)
#define SHA_SHA_IN_SWAP_ADDR                            (uint32_t)&(SHA->CTRL)
#define SHA_SHA_IN_SWAP_POS                             (1) /*< bit[1]      */
#define SHA_SHA_IN_SWAP_MASK                            (0x1ul << SHA_SHA_IN_SWAP_POS)

#define SHA_IN_LEN_0_ADDR                               (uint32_t)&(SHA->DATA_LEN0)
#define SHA_IN_LEN_1_ADDR                               (uint32_t)&(SHA->DATA_LEN1)
#define SHA_DMA_ADDR                                    (uint32_t)&(SHA->SHA_DMA)
#define SHA_W1T_SHA_DMA_DATA_ADDR                       (uint32_t)&(SHA->SHA_DMA)
#define SHA_W1T_SHA_DMA_DATA_POS                        (0) /*< bit[0]      */
#define SHA_W1T_SHA_DMA_DATA_MASK                       (0x1ul << SHA_W1T_SHA_DMA_DATA_POS)

#define SHA_INT_ST_ADDR                                 (uint32_t)&(SHA->STATUS)
#define SHA_INT_ST_FSH_ADDR                             (uint32_t)&(SHA->STATUS)
#define SHA_INT_ST_FSH_POS                              (0) /*< bit[0]      */
#define SHA_INT_ST_FSH_MASK                             (0x1ul << SHA_INT_ST_FSH_POS)

#define SHA_INT_EN_ADDR                                 (uint32_t)&(SHA->INTEN)
#define SHA_INT_EN_FSH_ADDR                             (uint32_t)&(SHA->INTEN)
#define SHA_INT_EN_FSH_POS                              (0) /*< bit[0]      */
#define SHA_INT_EN_FSH_MASK                             (0x1ul << SHA_INT_EN_FSH_POS)

#define SHA_INT_ADDR                                    (uint32_t)&(SHA->STATUS_MSK)
#define SHA_INT_FSH_ADDR                                (uint32_t)&(SHA->STATUS_MSK)
#define SHA_INT_FSH_POS                                 (0) /*< bit[0]      */
#define SHA_INT_FSH_MASK                                (0x1ul << SHA_INT_FSH_POS)

#define SHA_STATUS_ADDR                                 (uint32_t)&(SHA->SHA_STATUS)
#define SHA_SHA_BUSY_ADDR                               (uint32_t)&(SHA->SHA_STATUS)
#define SHA_SHA_BUSY_POS                                (0) /*< bit[0]      */
#define SHA_SHA_BUSY_MASK                               (0x1ul << SHA_SHA_BUSY_POS)

#define SHA_PERIPH_ID_LEN_REG_ADDR                      (uint32_t)&(SHA->PERIPH_ID_LEN_REG)
#define SHA_PERIPH_ID_LEN_ADDR                          (uint32_t)&(SHA->PERIPH_ID_LEN_REG)
#define SHA_PERIPH_ID_LEN_POS                           (0) /*< bit[3:0]    */
#define SHA_PERIPH_ID_LEN_MASK                          (0xFul << SHA_PERIPH_ID_LEN_POS)

#define SHA_PERIPH_ID_0_REG_ADDR                        (uint32_t)&(SHA->PERIPH_ID_0_REG)
#define SHA_BASE                                        0x40038000UL
#define SHA                                             ((SHA_T *)SHA_BASE)

#endif /* __SHA_REG_ER8130_H__ */

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_trng_soc_b1_20240720 */
/* ================================================================================
 */
/* ================                     TRNG_T ================ */
/* ================================================================================
 */
#ifndef __TRNG_REG_ER8130_H__
#define __TRNG_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief TRNG (TRNG_T)
 */

typedef struct { /*!< TRNG_T Structure              */
    __I uint32_t RESERVED0[64];
    __IO uint32_t IMR;           /*!< IMR Register (0x100)          */
    __I uint32_t ISR;            /*!< ISR Register (0x104)          */
    __O uint32_t ICR;            /*!< ICR Register (0x108)          */
    __IO uint32_t CONFIG;        /*!< CONFIG Register (0x10C)       */
    __I uint32_t VALID;          /*!< VALID Register (0x110)        */
    __IO uint32_t EHR_DATA0;     /*!< EHR_DATA0 Register (0x114)    */
    __IO uint32_t EHR_DATA1;     /*!< EHR_DATA1 Register (0x118)    */
    __IO uint32_t EHR_DATA2;     /*!< EHR_DATA2 Register (0x11C)    */
    __IO uint32_t EHR_DATA3;     /*!< EHR_DATA3 Register (0x120)    */
    __IO uint32_t EHR_DATA4;     /*!< EHR_DATA4 Register (0x124)    */
    __IO uint32_t EHR_DATA5;     /*!< EHR_DATA5 Register (0x128)    */
    __IO uint32_t RND_SRC_EN;    /*!< RND_SRC_EN Register (0x12C)   */
    __IO uint32_t SAMPLE_CNT1;   /*!< SAMPLE_CNT1 Register (0x130)  */
    __IO uint32_t AUTOCORR_STAT; /*!< AUTOCORR_STAT Register (0x134)*/
    __IO uint32_t DBG_CTRL;      /*!< DBG_CTRL Register (0x138)     */
    __I uint32_t RESERVED1;
    __O uint32_t SW_RST; /*!< SW_RST Register (0x140)       */
    __I uint32_t RESERVED2[28];
    __IO uint32_t DBG_EN_INPUT; /*!< DBG_EN_INPUT Register (0x1B4) */
    __I uint32_t BUSY;          /*!< BUSY Register (0x1B8)         */
    __O uint32_t RST_BITS_CNTR; /*!< RST_BITS_CNTR Register (0x1BC)*/
    __I uint32_t VER;           /*!< VER Register (0x1C0)          */
    __I uint32_t RESERVED3[7];
    __I uint32_t BIST_CNTR0; /*!< BIST_CNTR0 Register (0x1E0)   */
    __I uint32_t BIST_CNTR1; /*!< BIST_CNTR1 Register (0x1E4)   */
    __I uint32_t BIST_CNTR2; /*!< BIST_CNTR2 Register (0x1E8)   */
} TRNG_T;

#define TRNG_IMR_ADDR (uint32_t)&(TRNG->IMR)
#define TRNG_EHR_VALID_INT_MASK_ADDR (uint32_t)&(TRNG->IMR)
#define TRNG_EHR_VALID_INT_MASK_POS (0) /*< bit[0]      */
#define TRNG_EHR_VALID_INT_MASK_MASK (0x1ul << TRNG_EHR_VALID_INT_MASK_POS)
#define TRNG_AUTOCORR_ERR_INT_MASK_ADDR (uint32_t)&(TRNG->IMR)
#define TRNG_AUTOCORR_ERR_INT_MASK_POS (1) /*< bit[1]      */
#define TRNG_AUTOCORR_ERR_INT_MASK_MASK                                        \
    (0x1ul << TRNG_AUTOCORR_ERR_INT_MASK_POS)
#define TRNG_CRNGT_ERR_INT_MASK_ADDR (uint32_t)&(TRNG->IMR)
#define TRNG_CRNGT_ERR_INT_MASK_POS (2) /*< bit[2]      */
#define TRNG_CRNGT_ERR_INT_MASK_MASK (0x1ul << TRNG_CRNGT_ERR_INT_MASK_POS)
#define TRNG_VN_ERR_INT_MASK_ADDR (uint32_t)&(TRNG->IMR)
#define TRNG_VN_ERR_INT_MASK_POS (3) /*< bit[3]      */
#define TRNG_VN_ERR_INT_MASK_MASK (0x1ul << TRNG_VN_ERR_INT_MASK_POS)

#define TRNG_ISR_ADDR (uint32_t)&(TRNG->ISR)
#define TRNG_ISR_EHR_VALID_ADDR (uint32_t)&(TRNG->ISR)
#define TRNG_ISR_EHR_VALID_POS (0) /*< bit[0]      */
#define TRNG_ISR_EHR_VALID_MASK (0x1ul << TRNG_ISR_EHR_VALID_POS)
#define TRNG_ISR_AUTOCORR_ERR_ADDR (uint32_t)&(TRNG->ISR)
#define TRNG_ISR_AUTOCORR_ERR_POS (1) /*< bit[1]      */
#define TRNG_ISR_AUTOCORR_ERR_MASK (0x1ul << TRNG_ISR_AUTOCORR_ERR_POS)
#define TRNG_ISR_CRNGT_ERR_ADDR (uint32_t)&(TRNG->ISR)
#define TRNG_ISR_CRNGT_ERR_POS (2) /*< bit[2]      */
#define TRNG_ISR_CRNGT_ERR_MASK (0x1ul << TRNG_ISR_CRNGT_ERR_POS)
#define TRNG_ISR_VN_ERR_ADDR (uint32_t)&(TRNG->ISR)
#define TRNG_ISR_VN_ERR_POS (3) /*< bit[3]      */
#define TRNG_ISR_VN_ERR_MASK (0x1ul << TRNG_ISR_VN_ERR_POS)

#define TRNG_ICR_ADDR (uint32_t)&(TRNG->ICR)
#define TRNG_ICR_EHR_VALID_ADDR (uint32_t)&(TRNG->ICR)
#define TRNG_ICR_EHR_VALID_POS (0) /*< bit[0]      */
#define TRNG_ICR_EHR_VALID_MASK (0x1ul << TRNG_ICR_EHR_VALID_POS)
#define TRNG_ICR_AUTOCORR_ERR_ADDR (uint32_t)&(TRNG->ICR)
#define TRNG_ICR_AUTOCORR_ERR_POS (1) /*< bit[1]      */
#define TRNG_ICR_AUTOCORR_ERR_MASK (0x1ul << TRNG_ICR_AUTOCORR_ERR_POS)
#define TRNG_ICR_CRNGT_ERR_ADDR (uint32_t)&(TRNG->ICR)
#define TRNG_ICR_CRNGT_ERR_POS (2) /*< bit[2]      */
#define TRNG_ICR_CRNGT_ERR_MASK (0x1ul << TRNG_ICR_CRNGT_ERR_POS)
#define TRNG_ICR_VN_ERR_ADDR (uint32_t)&(TRNG->ICR)
#define TRNG_ICR_VN_ERR_POS (3) /*< bit[3]      */
#define TRNG_ICR_VN_ERR_MASK (0x1ul << TRNG_ICR_VN_ERR_POS)

#define TRNG_CONFIG_ADDR (uint32_t)&(TRNG->CONFIG)
#define TRNG_RND_SRC_SEL_ADDR (uint32_t)&(TRNG->CONFIG)
#define TRNG_RND_SRC_SEL_POS (0) /*< bit[1:0]    */
#define TRNG_RND_SRC_SEL_MASK (0x3ul << TRNG_RND_SRC_SEL_POS)

#define TRNG_VALID_ADDR (uint32_t)&(TRNG->VALID)
#define TRNG_VALID_EHR_VLD_ADDR (uint32_t)&(TRNG->VALID)
#define TRNG_VALID_EHR_VLD_POS (0) /*< bit[1:0]    */
#define TRNG_VALID_EHR_VLD_MASK (0x3ul << TRNG_VALID_EHR_VLD_POS)

#define TRNG_EHR_DATA0_ADDR (uint32_t)&(TRNG->EHR_DATA0)
#define TRNG_EHR_DATA1_ADDR (uint32_t)&(TRNG->EHR_DATA1)
#define TRNG_EHR_DATA2_ADDR (uint32_t)&(TRNG->EHR_DATA2)
#define TRNG_EHR_DATA3_ADDR (uint32_t)&(TRNG->EHR_DATA3)
#define TRNG_EHR_DATA4_ADDR (uint32_t)&(TRNG->EHR_DATA4)
#define TRNG_EHR_DATA5_ADDR (uint32_t)&(TRNG->EHR_DATA5)
#define TRNG_RND_SRC_EN_ADDR (uint32_t)&(TRNG->RND_SRC_EN)
#define TRNG_RND_SRC_EN_ADDR (uint32_t)&(TRNG->RND_SRC_EN)
#define TRNG_RND_SRC_EN_POS (0) /*< bit[0]      */
#define TRNG_RND_SRC_EN_MASK (0x1ul << TRNG_RND_SRC_EN_POS)

#define TRNG_SAMPLE_CNT1_ADDR (uint32_t)&(TRNG->SAMPLE_CNT1)
#define TRNG_AUTOCORR_STAT_ADDR (uint32_t)&(TRNG->AUTOCORR_STAT)
#define TRNG_AUTOCORR_TRYS_ADDR (uint32_t)&(TRNG->AUTOCORR_STAT)
#define TRNG_AUTOCORR_TRYS_POS (0) /*< bit[13:0]   */
#define TRNG_AUTOCORR_TRYS_MASK (0x3FFFul << TRNG_AUTOCORR_TRYS_POS)
#define TRNG_AUTOCORR_FAILS_ADDR (uint32_t)&(TRNG->AUTOCORR_STAT)
#define TRNG_AUTOCORR_FAILS_POS (14) /*< bit[21:14]  */
#define TRNG_AUTOCORR_FAILS_MASK (0xFFul << TRNG_AUTOCORR_FAILS_POS)

#define TRNG_DBG_CTRL_ADDR (uint32_t)&(TRNG->DBG_CTRL)
#define TRNG_VNC_BYPASS_ADDR (uint32_t)&(TRNG->DBG_CTRL)
#define TRNG_VNC_BYPASS_POS (1) /*< bit[1]      */
#define TRNG_VNC_BYPASS_MASK (0x1ul << TRNG_VNC_BYPASS_POS)
#define TRNG_TRNG_CRNGT_BYPASS_ADDR (uint32_t)&(TRNG->DBG_CTRL)
#define TRNG_TRNG_CRNGT_BYPASS_POS (2) /*< bit[2]      */
#define TRNG_TRNG_CRNGT_BYPASS_MASK (0x1ul << TRNG_TRNG_CRNGT_BYPASS_POS)
#define TRNG_AUTO_CORRELATE_BYPASS_ADDR (uint32_t)&(TRNG->DBG_CTRL)
#define TRNG_AUTO_CORRELATE_BYPASS_POS (3) /*< bit[3]      */
#define TRNG_AUTO_CORRELATE_BYPASS_MASK                                        \
    (0x1ul << TRNG_AUTO_CORRELATE_BYPASS_POS)

#define TRNG_SW_RST_ADDR (uint32_t)&(TRNG->SW_RST)
#define TRNG_RNG_SW_RESET_ADDR (uint32_t)&(TRNG->SW_RST)
#define TRNG_RNG_SW_RESET_POS (0) /*< bit[0]      */
#define TRNG_RNG_SW_RESET_MASK (0x1ul << TRNG_RNG_SW_RESET_POS)

#define TRNG_DBG_EN_INPUT_ADDR (uint32_t)&(TRNG->DBG_EN_INPUT)
#define TRNG_RNG_DEBUG_EN_ADDR (uint32_t)&(TRNG->DBG_EN_INPUT)
#define TRNG_RNG_DEBUG_EN_POS (0) /*< bit[0]      */
#define TRNG_RNG_DEBUG_EN_MASK (0x1ul << TRNG_RNG_DEBUG_EN_POS)

#define TRNG_BUSY_ADDR (uint32_t)&(TRNG->BUSY)
#define TRNG_BSY_ADDR (uint32_t)&(TRNG->BUSY)
#define TRNG_BSY_POS (0) /*< bit[0]      */
#define TRNG_BSY_MASK (0x1ul << TRNG_BSY_POS)

#define TRNG_RST_BITS_CNTR_ADDR (uint32_t)&(TRNG->RST_BITS_CNTR)
#define TRNG_RST_BITS_CNTR_ADDR (uint32_t)&(TRNG->RST_BITS_CNTR)
#define TRNG_RST_BITS_CNTR_POS (0) /*< bit[0]      */
#define TRNG_RST_BITS_CNTR_MASK (0x1ul << TRNG_RST_BITS_CNTR_POS)

#define TRNG_VER_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_EHR_WIDTH_192_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_EHR_WIDTH_192_POS (0) /*< bit[0]      */
#define TRNG_EHR_WIDTH_192_MASK (0x1ul << TRNG_EHR_WIDTH_192_POS)
#define TRNG_CRNGT_EXISTS_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_CRNGT_EXISTS_POS (1) /*< bit[1]      */
#define TRNG_CRNGT_EXISTS_MASK (0x1ul << TRNG_CRNGT_EXISTS_POS)
#define TRNG_AUTOCORR_EXISTS_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_AUTOCORR_EXISTS_POS (2) /*< bit[2]      */
#define TRNG_AUTOCORR_EXISTS_MASK (0x1ul << TRNG_AUTOCORR_EXISTS_POS)
#define TRNG_TRNG_TESTS_BYPASS_EN_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_TRNG_TESTS_BYPASS_EN_POS (3) /*< bit[3]      */
#define TRNG_TRNG_TESTS_BYPASS_EN_MASK (0x1ul << TRNG_TRNG_TESTS_BYPASS_EN_POS)
#define TRNG_PRNG_EXISTS_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_PRNG_EXISTS_POS (4) /*< bit[4]      */
#define TRNG_PRNG_EXISTS_MASK (0x1ul << TRNG_PRNG_EXISTS_POS)
#define TRNG_KAT_EXISTS_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_KAT_EXISTS_POS (5) /*< bit[5]      */
#define TRNG_KAT_EXISTS_MASK (0x1ul << TRNG_KAT_EXISTS_POS)
#define TRNG_RESEEDING_EXISTS_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_RESEEDING_EXISTS_POS (6) /*< bit[6]      */
#define TRNG_RESEEDING_EXISTS_MASK (0x1ul << TRNG_RESEEDING_EXISTS_POS)
#define TRNG_RNG_USE_5_SBOXES_ADDR (uint32_t)&(TRNG->VER)
#define TRNG_RNG_USE_5_SBOXES_POS (7) /*< bit[7]      */
#define TRNG_RNG_USE_5_SBOXES_MASK (0x1ul << TRNG_RNG_USE_5_SBOXES_POS)

#define TRNG_BIST_CNTR0_ADDR (uint32_t)&(TRNG->BIST_CNTR0)
#define TRNG_ROSC_CNTR0_VAL_ADDR (uint32_t)&(TRNG->BIST_CNTR0)
#define TRNG_ROSC_CNTR0_VAL_POS (0) /*< bit[21:0]   */
#define TRNG_ROSC_CNTR0_VAL_MASK (0x3FFFFFul << TRNG_ROSC_CNTR0_VAL_POS)

#define TRNG_BIST_CNTR1_ADDR (uint32_t)&(TRNG->BIST_CNTR1)
#define TRNG_ROSC_CNTR1_VAL_ADDR (uint32_t)&(TRNG->BIST_CNTR1)
#define TRNG_ROSC_CNTR1_VAL_POS (0) /*< bit[21:0]   */
#define TRNG_ROSC_CNTR1_VAL_MASK (0x3FFFFFul << TRNG_ROSC_CNTR1_VAL_POS)

#define TRNG_BIST_CNTR2_ADDR (uint32_t)&(TRNG->BIST_CNTR2)
#define TRNG_ROSC_CNTR2_VAL_ADDR (uint32_t)&(TRNG->BIST_CNTR2)
#define TRNG_ROSC_CNTR2_VAL_POS (0) /*< bit[21:0]   */
#define TRNG_ROSC_CNTR2_VAL_MASK (0x3FFFFFul << TRNG_ROSC_CNTR2_VAL_POS)

#define TRNG_BASE 0x40039000UL
#define TRNG ((TRNG_T *)TRNG_BASE)

#endif /* __TRNG_REG_ER8130_H__ */

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_aes_xs_b1_20231212.xlsx */
/* ================================================================================
 */
/* ================                      AES_T ================ */
/* ================================================================================
 */
#ifndef __AES_REG_ER8130_H__
#define __AES_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief AES (AES_T)
 */

typedef struct {          /*!< AES_T Structure */
    __IO uint32_t KEY[8]; /*!< AES Key Word 0~7 Register */
    __IO uint32_t IV[4];  /*!< AES Initial Vector Word 0~3 Register */
    __I uint32_t RESERVED0[4];

    union {
        __IO uint32_t CTRL; /*!< AES Control Register */

        struct {
            __IO uint32_t DIR      : 1; /*!< 0:encryption 1:decryption */
            __IO uint32_t KEY_SIZE : 2; /*!< 0:128 1:192 2:256 bits */
            __IO uint32_t MODE     : 3; /*!< 0:ECB 1:CBC 2:CTR 3:CBC_MAC 4:CCM */
            __IO uint32_t SOURCE   : 1; /*!< 0:from keystore 1:from CSR */
            __IO uint32_t CCM_L    : 3; /*!< Width of the length field for CCM */
            __IO uint32_t CCM_M    : 3; /*!< Width of the authentication field */
            __IO uint32_t IN_SWAP  : 1; /*!< Swap in */
            __IO uint32_t OUT_SWAP : 1; /*!< Swap out */
            uint32_t               : 17;
        } CTRL_b;
    };

    __IO uint32_t DATA_LEN0; /*!< DATA_LEN Register */
    __IO uint32_t DATA_LEN1; /*!< DATA_LEN Register */
    __IO uint32_t AUTH_LEN;  /*!< AUTH_LEN Register */
    __I uint32_t TAG[4];     /*!< AES TAG Word 0~3 Register */
    __I uint32_t RESERVED1;

    union {
        __O uint32_t AES_DMA; /*!< AES DMA Register */

        struct {
            __O uint32_t AESEN  : 1; /*!< Trigger DMA aes_in channel */
            __O uint32_t AUTHEN : 1; /*!< Trigger DMA auth_in channel */
            __O uint32_t WKEYEN : 1; /*!< Trigger writing key to keystore */
            __O uint32_t RKEYEN : 1; /*!< Trigger loading key from keystore */
            uint32_t            : 28;
        } AES_DMA_b;
    };

    union {
        __IO uint32_t STATUS; /*!< AES Status Register */

        struct {
            __IO uint32_t CCM_INVALID : 1; /*!< CCM decryption is invalid */
            __IO uint32_t TAG         : 1; /*!< TAG finish */
            __IO uint32_t FSH         : 1; /*!< AES operation finish */
            __IO uint32_t KEY_WR_ERR  : 1; /*!< Key write error */
            __IO uint32_t KEY_RD_ERR  : 1; /*!< Key read error */
            uint32_t                  : 27;
        } STATUS_b;
    };

    union {
        __IO uint32_t INTEN; /*!< Interrupt Enable Register */

        struct {
            __IO uint32_t CCM_INVALID : 1; /*!< Enable CCM decryption invalid */
            __IO uint32_t TAG         : 1; /*!< Enable TAG finish */
            __IO uint32_t FSH         : 1; /*!< Enable AES operation finish */
            __IO uint32_t KEY_WR_ERR  : 1; /*!< Enable key write error */
            __IO uint32_t KEY_RD_ERR  : 1; /*!< Enable key read error */
            uint32_t                  : 27;
        } INTEN_b;
    };

    union {
        __I uint32_t STATUS_MSK; /*!< Masked Status Register */

        struct {
            __I uint32_t CCM_INVALID : 1; /*!< CCM decryption invalid mask */
            __I uint32_t TAG         : 1; /*!< TAG finish mask */
            __I uint32_t FSH         : 1; /*!< AES operation finish mask */
            __I uint32_t KEY_WR_ERR  : 1; /*!< Key write error mask */
            __I uint32_t KEY_RD_ERR  : 1; /*!< Key read error mask */
            uint32_t                 : 27;
        } STATUS_MSK_b;
    };

    __I uint32_t RESERVED2;

    union {
        __IO uint32_t KEY_WAREA; /*!< KEY_WRITE_AREA Register */

        struct {
            __IO uint32_t AREA : 3; /*!< Address for writing */
            uint32_t           : 29;
        } KEY_WAREA_b;
    };

    union {
        __IO uint32_t KEY_STORE_ST; /*!< KEY_WRITTEN_AREA Register */

        struct {
            __IO uint32_t AREA0 : 1; /*!< Key store area 0 written or not */
            __IO uint32_t AREA1 : 1;
            __IO uint32_t AREA2 : 1;
            __IO uint32_t AREA3 : 1;
            __IO uint32_t AREA4 : 1;
            __IO uint32_t AREA5 : 1;
            __IO uint32_t AREA6 : 1;
            __IO uint32_t AREA7 : 1;
            uint32_t            : 24;
        } KEY_STORE_ST_b;
    };

    union {
        __IO uint32_t KEY_STORE_SIZE; /*!< KEY_STORE_SIZE Register */

        struct {
            __IO uint32_t SIZE : 2; /*!< Key size: 0:128 1:192 2:256 bits */
            uint32_t           : 30;
        } KEY_STORE_SIZE_b;
    };

    union {
        __IO uint32_t KEY_RAREA; /*!< KEY_READ_AREA Register */

        struct {
            __IO uint32_t AREA : 3; /*!< Address for reading */
            uint32_t           : 29;
        } KEY_RAREA_b;
    };

    __I uint32_t RESERVED3;

    union {
        __I uint32_t AES_STATUS; /*!< AES_STATUS Register */

        struct {
            __I uint32_t BUSY           : 1; /*!< AES engine is busy */
            __I uint32_t KEY_READY      : 1; /*!< Key is ready or not */
            __I uint32_t KEY_WRITE_BUSY : 1; /*!< Key store module write status */
            uint32_t                    : 29;
        } AES_STATUS_b;
    };
} AES_T;

#define AES_KEY_0_ADDR (uint32_t)&(AES->KEY_0)
#define AES_KEY_1_ADDR (uint32_t)&(AES->KEY_1)
#define AES_KEY_2_ADDR (uint32_t)&(AES->KEY_2)
#define AES_KEY_3_ADDR (uint32_t)&(AES->KEY_3)
#define AES_KEY_4_ADDR (uint32_t)&(AES->KEY_4)
#define AES_KEY_5_ADDR (uint32_t)&(AES->KEY_5)
#define AES_KEY_6_ADDR (uint32_t)&(AES->KEY_6)
#define AES_KEY_7_ADDR (uint32_t)&(AES->KEY_7)
#define AES_IV_0_ADDR (uint32_t)&(AES->IV[0])
#define AES_IV_1_ADDR (uint32_t)&(AES->IV[1])
#define AES_IV_2_ADDR (uint32_t)&(AES->IV[2])
#define AES_IV_3_ADDR (uint32_t)&(AES->IV[3])
#define AES_CTL_ADDR (uint32_t)&(AES->CTRL)
#define AES_DIR_ADDR (uint32_t)&(AES->CTRL)
#define AES_DIR_POS (0) /*< bit[0]      */
#define AES_DIR_MASK (0x1ul << AES_DIR_POS)
#define AES_KEY_SIZE_ADDR (uint32_t)&(AES->CTRL)
#define AES_KEY_SIZE_POS (1) /*< bit[2:1]    */
#define AES_KEY_SIZE_MASK (0x3ul << AES_KEY_SIZE_POS)
#define AES_MODE_ADDR (uint32_t)&(AES->CTRL)
#define AES_MODE_POS (3) /*< bit[5:3]    */
#define AES_MODE_MASK (0x7ul << AES_MODE_POS)
#define AES_KEY_SOURCE_ADDR (uint32_t)&(AES->CTRL)
#define AES_KEY_SOURCE_POS (6) /*< bit[6]      */
#define AES_KEY_SOURCE_MASK (0x1ul << AES_KEY_SOURCE_POS)
#define AES_CCM_L_ADDR (uint32_t)&(AES->CTRL)
#define AES_CCM_L_POS (7) /*< bit[9:7]    */
#define AES_CCM_L_MASK (0x7ul << AES_CCM_L_POS)
#define AES_CCM_M_ADDR (uint32_t)&(AES->CTRL)
#define AES_CCM_M_POS (10) /*< bit[12:10]  */
#define AES_CCM_M_MASK (0x7ul << AES_CCM_M_POS)
#define AES_IN_SWAP_ADDR (uint32_t)&(AES->CTRL)
#define AES_IN_SWAP_POS (13) /*< bit[13]     */
#define AES_IN_SWAP_MASK (0x1ul << AES_IN_SWAP_POS)
#define AES_OUT_SWAP_ADDR (uint32_t)&(AES->CTRL)
#define AES_OUT_SWAP_POS (14) /*< bit[14]     */
#define AES_OUT_SWAP_MASK (0x1ul << AES_OUT_SWAP_POS)

#define AES_DATA_LEN_0_ADDR (uint32_t)&(AES->DATA_LEN0)
#define AES_DATA_LEN_1_ADDR (uint32_t)&(AES->DATA_LEN1)
#define AES_AUTH_LEN_ADDR (uint32_t)&(AES->AUTH_LEN)
#define AES_TAG_0_ADDR (uint32_t)&(AES->TAG[0])
#define AES_TAG_1_ADDR (uint32_t)&(AES->TAG[1])
#define AES_TAG_2_ADDR (uint32_t)&(AES->TAG[2])
#define AES_TAG_3_ADDR (uint32_t)&(AES->TAG[3])
#define AES_DMA_ADDR (uint32_t)&(AES->AES_DMA)
#define AES_W1T_AES_DMA_DATA_ADDR (uint32_t)&(AES->AES_DMA)
#define AES_W1T_AES_DMA_DATA_POS (0) /*< bit[0]      */
#define AES_W1T_AES_DMA_DATA_MASK (0x1ul << AES_W1T_AES_DMA_DATA_POS)
#define AES_W1T_AES_DMA_AUTH_ADDR (uint32_t)&(AES->AES_DMA)
#define AES_W1T_AES_DMA_AUTH_POS (1) /*< bit[1]      */
#define AES_W1T_AES_DMA_AUTH_MASK (0x1ul << AES_W1T_AES_DMA_AUTH_POS)
#define AES_W1T_AES_KEY_WRITE_ADDR (uint32_t)&(AES->AES_DMA)
#define AES_W1T_AES_KEY_WRITE_POS (2) /*< bit[2]      */
#define AES_W1T_AES_KEY_WRITE_MASK (0x1ul << AES_W1T_AES_KEY_WRITE_POS)
#define AES_W1T_AES_KEY_READ_ADDR (uint32_t)&(AES->AES_DMA)
#define AES_W1T_AES_KEY_READ_POS (3) /*< bit[3]      */
#define AES_W1T_AES_KEY_READ_MASK (0x1ul << AES_W1T_AES_KEY_READ_POS)

#define AES_INT_ST_ADDR (uint32_t)&(AES->STATUS)
#define AES_INT_ST_CCM_INVALID_ADDR (uint32_t)&(AES->STATUS)
#define AES_INT_ST_CCM_INVALID_POS (0) /*< bit[0]      */
#define AES_INT_ST_CCM_INVALID_MASK (0x1ul << AES_INT_ST_CCM_INVALID_POS)
#define AES_INT_ST_TAG_ADDR (uint32_t)&(AES->STATUS)
#define AES_INT_ST_TAG_POS (1) /*< bit[1]      */
#define AES_INT_ST_TAG_MASK (0x1ul << AES_INT_ST_TAG_POS)
#define AES_INT_ST_FSH_ADDR (uint32_t)&(AES->STATUS)
#define AES_INT_ST_FSH_POS (2) /*< bit[2]      */
#define AES_INT_ST_FSH_MASK (0x1ul << AES_INT_ST_FSH_POS)
#define AES_INT_ST_KEY_WR_ERR_ADDR (uint32_t)&(AES->STATUS)
#define AES_INT_ST_KEY_WR_ERR_POS (3) /*< bit[3]      */
#define AES_INT_ST_KEY_WR_ERR_MASK (0x1ul << AES_INT_ST_KEY_WR_ERR_POS)
#define AES_INT_ST_KEY_RD_ERR_ADDR (uint32_t)&(AES->STATUS)
#define AES_INT_ST_KEY_RD_ERR_POS (4) /*< bit[4]      */
#define AES_INT_ST_KEY_RD_ERR_MASK (0x1ul << AES_INT_ST_KEY_RD_ERR_POS)

#define AES_INT_EN_ADDR (uint32_t)&(AES->INTEN)
#define AES_INT_EN_CCM_INVALID_ADDR (uint32_t)&(AES->INTEN)
#define AES_INT_EN_CCM_INVALID_POS (0) /*< bit[0]      */
#define AES_INT_EN_CCM_INVALID_MASK (0x1ul << AES_INT_EN_CCM_INVALID_POS)
#define AES_INT_EN_TAG_ADDR (uint32_t)&(AES->INTEN)
#define AES_INT_EN_TAG_POS (1) /*< bit[1]      */
#define AES_INT_EN_TAG_MASK (0x1ul << AES_INT_EN_TAG_POS)
#define AES_INT_EN_FSH_ADDR (uint32_t)&(AES->INTEN)
#define AES_INT_EN_FSH_POS (2) /*< bit[2]      */
#define AES_INT_EN_FSH_MASK (0x1ul << AES_INT_EN_FSH_POS)
#define AES_INT_EN_KEY_WR_ERR_ADDR (uint32_t)&(AES->INTEN)
#define AES_INT_EN_KEY_WR_ERR_POS (3) /*< bit[3]      */
#define AES_INT_EN_KEY_WR_ERR_MASK (0x1ul << AES_INT_EN_KEY_WR_ERR_POS)
#define AES_INT_EN_KEY_RD_ERR_ADDR (uint32_t)&(AES->INTEN)
#define AES_INT_EN_KEY_RD_ERR_POS (4) /*< bit[4]      */
#define AES_INT_EN_KEY_RD_ERR_MASK (0x1ul << AES_INT_EN_KEY_RD_ERR_POS)

#define AES_INT_ADDR (uint32_t)&(AES->INT)
#define AES_INT_CCM_INVALID_ADDR (uint32_t)&(AES->INT)
#define AES_INT_CCM_INVALID_POS (0) /*< bit[0]      */
#define AES_INT_CCM_INVALID_MASK (0x1ul << AES_INT_CCM_INVALID_POS)
#define AES_INT_TAG_ADDR (uint32_t)&(AES->INT)
#define AES_INT_TAG_POS (1) /*< bit[1]      */
#define AES_INT_TAG_MASK (0x1ul << AES_INT_TAG_POS)
#define AES_INT_FSH_ADDR (uint32_t)&(AES->INT)
#define AES_INT_FSH_POS (2) /*< bit[2]      */
#define AES_INT_FSH_MASK (0x1ul << AES_INT_FSH_POS)
#define AES_INT_KEY_WR_ERR_ADDR (uint32_t)&(AES->INT)
#define AES_INT_KEY_WR_ERR_POS (3) /*< bit[3]      */
#define AES_INT_KEY_WR_ERR_MASK (0x1ul << AES_INT_KEY_WR_ERR_POS)
#define AES_INT_KEY_RD_ERR_ADDR (uint32_t)&(AES->INT)
#define AES_INT_KEY_RD_ERR_POS (4) /*< bit[4]      */
#define AES_INT_KEY_RD_ERR_MASK (0x1ul << AES_INT_KEY_RD_ERR_POS)

#define AES_KEY_WRITE_AREA_ADDR (uint32_t)&(AES->KEY_WAREA)
#define AES_KEY_WRITE_AREA_ADDR (uint32_t)&(AES->KEY_WAREA)
#define AES_KEY_WRITE_AREA_POS (0) /*< bit[2:0]    */
#define AES_KEY_WRITE_AREA_MASK (0x7ul << AES_KEY_WRITE_AREA_POS)

#define AES_KEY_WRITTEN_AREA_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_0_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_0_POS (0) /*< bit[0]      */
#define AES_KEY_WRITTEN_AREA_0_MASK (0x1ul << AES_KEY_WRITTEN_AREA_0_POS)
#define AES_KEY_WRITTEN_AREA_1_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_1_POS (1) /*< bit[1]      */
#define AES_KEY_WRITTEN_AREA_1_MASK (0x1ul << AES_KEY_WRITTEN_AREA_1_POS)
#define AES_KEY_WRITTEN_AREA_2_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_2_POS (2) /*< bit[2]      */
#define AES_KEY_WRITTEN_AREA_2_MASK (0x1ul << AES_KEY_WRITTEN_AREA_2_POS)
#define AES_KEY_WRITTEN_AREA_3_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_3_POS (3) /*< bit[3]      */
#define AES_KEY_WRITTEN_AREA_3_MASK (0x1ul << AES_KEY_WRITTEN_AREA_3_POS)
#define AES_KEY_WRITTEN_AREA_4_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_4_POS (4) /*< bit[4]      */
#define AES_KEY_WRITTEN_AREA_4_MASK (0x1ul << AES_KEY_WRITTEN_AREA_4_POS)
#define AES_KEY_WRITTEN_AREA_5_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_5_POS (5) /*< bit[5]      */
#define AES_KEY_WRITTEN_AREA_5_MASK (0x1ul << AES_KEY_WRITTEN_AREA_5_POS)
#define AES_KEY_WRITTEN_AREA_6_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_6_POS (6) /*< bit[6]      */
#define AES_KEY_WRITTEN_AREA_6_MASK (0x1ul << AES_KEY_WRITTEN_AREA_6_POS)
#define AES_KEY_WRITTEN_AREA_7_ADDR (uint32_t)&(AES->KEY_STORE_ST)
#define AES_KEY_WRITTEN_AREA_7_POS (7) /*< bit[7]      */
#define AES_KEY_WRITTEN_AREA_7_MASK (0x1ul << AES_KEY_WRITTEN_AREA_7_POS)

#define AES_KEY_STORE_SIZE_ADDR (uint32_t)&(AES->KEY_STORE_SIZE)
#define AES_KEY_STORE_SIZE_ADDR (uint32_t)&(AES->KEY_STORE_SIZE)
#define AES_KEY_STORE_SIZE_POS (0) /*< bit[1:0]    */
#define AES_KEY_STORE_SIZE_MASK (0x3ul << AES_KEY_STORE_SIZE_POS)

#define AES_KEY_READ_AREA_ADDR (uint32_t)&(AES->KEY_RAREA)
#define AES_KEY_READ_AREA_ADDR (uint32_t)&(AES->KEY_RAREA)
#define AES_KEY_READ_AREA_POS (0) /*< bit[2:0]    */
#define AES_KEY_READ_AREA_MASK (0x7ul << AES_KEY_READ_AREA_POS)

#define AES_STATUS_ADDR (uint32_t)&(AES->AES_STATUS)
#define AES_BUSY_ADDR (uint32_t)&(AES->AES_STATUS)
#define AES_BUSY_POS (0) /*< bit[0]      */
#define AES_BUSY_MASK (0x1ul << AES_BUSY_POS)
#define AES_KEY_READY_ADDR (uint32_t)&(AES->AES_STATUS)
#define AES_KEY_READY_POS (1) /*< bit[1]      */
#define AES_KEY_READY_MASK (0x1ul << AES_KEY_READY_POS)
#define AES_KEY_WRITE_BUSY_ADDR (uint32_t)&(AES->AES_STATUS)
#define AES_KEY_WRITE_BUSY_POS (2) /*< bit[2]      */
#define AES_KEY_WRITE_BUSY_MASK (0x1ul << AES_KEY_WRITE_BUSY_POS)

#define AES_PERIPH_ID_LEN_REG_ADDR (uint32_t)&(AES->PERIPH_ID_LEN_REG)
#define AES_PERIPH_ID_LEN_ADDR (uint32_t)&(AES->PERIPH_ID_LEN_REG)
#define AES_PERIPH_ID_LEN_POS (0) /*< bit[3:0]    */
#define AES_PERIPH_ID_LEN_MASK (0xFul << AES_PERIPH_ID_LEN_POS)

#define AES_PERIPH_ID_0_REG_ADDR (uint32_t)&(AES->PERIPH_ID_0_REG)
#define AES_BASE 0x40037000UL
#define AES ((AES_T *)AES_BASE)

#endif /* __AES_REG_ER8130_H__ */

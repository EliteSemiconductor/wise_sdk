/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __DMA_REG_ER81XX_H__
#define __DMA_REG_ER81XX_H__

/* Version : csr_table_dma_xs_d1_20240926 */
/* ================================================================================
 */
/* ================                     DMA_T ================ */
/* ================================================================================
 */
#include "cmsis/include/er8xxx.h"
/**
 * @brief DMA (DMA_T)
 */

typedef struct {                  /*!< DMA_T Structure                                                            */
    __IO uint32_t CH0_SWRST_REG;  /*!< CH0_SWRST_REG Register (0x000)                                             */
    __IO uint32_t CH0_CONFIG_REG; /*!< CH0_CONFIG_REG Register (0x004)                                            */
    __IO uint32_t CH0_BUF_EN_REG; /*!< CH0_BUF_EN_REG Register (0x008)                                            */
    __I uint32_t RESERVED0;
    __IO uint32_t CH0_BUF_WORD_SIZE_REG;   /*!< CH0_BUF_WORD_SIZE_REG Register (0x010)                                     */
    __IO uint32_t CH0_M2M_SRC_ADDR_REG;    /*!< CH0_M2M_SRC_ADDR_REG Register (0x014)                                      */
    __IO uint32_t CH0_M2M_DEST_ADDR_REG;   /*!< CH0_M2M_DEST_ADDR_REG Register (0x018)                                     */
    __IO uint32_t CH0_M2M_LEN_REG;         /*!< CH0_M2M_LEN_REG Register (0x01C)                                           */
    __I uint32_t CH0_CNT_A_REG;            /*!< CH0_CNT_A_REG Register (0x020)                                             */
    __I uint32_t CH0_CNT_B_REG;            /*!< CH0_CNT_B_REG Register (0x024)                                             */
    __IO uint32_t CH0_TRIG_REG;            /*!< CH0_TRIG_REG Register (0x028)                                              */
    __I uint32_t CH0_PING_PONG_PTR_REG;    /*!< CH0_PING_PONG_PTR_REG Register (0x02C)                                     */
    __IO uint32_t CH0_BUF_BASE_ADDR_A_REG; /*!< CH0_BUF_BASE_ADDR_A_REG Register (0x030)                                   */
    __IO uint32_t CH0_BUF_BASE_ADDR_B_REG; /*!< CH0_BUF_BASE_ADDR_B_REG Register (0x034)                                   */
    __I uint32_t RESERVED1[2];
    __IO uint32_t CH1_SWRST_REG;  /*!< CH1_SWRST_REG Register (0x040)                                             */
    __IO uint32_t CH1_CONFIG_REG; /*!< CH1_CONFIG_REG Register (0x044)                                            */
    __IO uint32_t CH1_BUF_EN_REG; /*!< CH1_BUF_EN_REG Register (0x048)                                            */
    __I uint32_t RESERVED2;
    __IO uint32_t CH1_BUF_WORD_SIZE_REG;   /*!< CH1_BUF_WORD_SIZE_REG Register (0x050)                                     */
    __IO uint32_t CH1_M2M_SRC_ADDR_REG;    /*!< CH1_M2M_SRC_ADDR_REG Register (0x054)                                      */
    __IO uint32_t CH1_M2M_DEST_ADDR_REG;   /*!< CH1_M2M_DEST_ADDR_REG Register (0x058)                                     */
    __IO uint32_t CH1_M2M_LEN_REG;         /*!< CH1_M2M_LEN_REG Register (0x05C)                                           */
    __I uint32_t CH1_CNT_A_REG;            /*!< CH1_CNT_A_REG Register (0x060)                                             */
    __I uint32_t CH1_CNT_B_REG;            /*!< CH1_CNT_B_REG Register (0x064)                                             */
    __IO uint32_t CH1_TRIG_REG;            /*!< CH1_TRIG_REG Register (0x068)                                              */
    __I uint32_t CH1_PING_PONG_PTR_REG;    /*!< CH1_PING_PONG_PTR_REG Register (0x06C)                                     */
    __IO uint32_t CH1_BUF_BASE_ADDR_A_REG; /*!< CH1_BUF_BASE_ADDR_A_REG Register (0x070)                                   */
    __IO uint32_t CH1_BUF_BASE_ADDR_B_REG; /*!< CH1_BUF_BASE_ADDR_B_REG Register (0x074)                                   */
    __I uint32_t RESERVED3[2];
    __IO uint32_t CH2_SWRST_REG;  /*!< CH2_SWRST_REG Register (0x080)                                             */
    __IO uint32_t CH2_CONFIG_REG; /*!< CH2_CONFIG_REG Register (0x084)                                            */
    __IO uint32_t CH2_BUF_EN_REG; /*!< CH2_BUF_EN_REG Register (0x088)                                            */
    __I uint32_t RESERVED4;
    __IO uint32_t CH2_BUF_WORD_SIZE_REG;   /*!< CH2_BUF_WORD_SIZE_REG Register (0x090)                                     */
    __IO uint32_t CH2_M2M_SRC_ADDR_REG;    /*!< CH2_M2M_SRC_ADDR_REG Register (0x094)                                      */
    __IO uint32_t CH2_M2M_DEST_ADDR_REG;   /*!< CH2_M2M_DEST_ADDR_REG Register (0x098)                                     */
    __IO uint32_t CH2_M2M_LEN_REG;         /*!< CH2_M2M_LEN_REG Register (0x09C)                                           */
    __I uint32_t CH2_CNT_A_REG;            /*!< CH2_CNT_A_REG Register (0x0A0)                                             */
    __I uint32_t CH2_CNT_B_REG;            /*!< CH2_CNT_B_REG Register (0x0A4)                                             */
    __IO uint32_t CH2_TRIG_REG;            /*!< CH2_TRIG_REG Register (0x0A8)                                              */
    __I uint32_t CH2_PING_PONG_PTR_REG;    /*!< CH2_PING_PONG_PTR_REG Register (0x0AC)                                     */
    __IO uint32_t CH2_BUF_BASE_ADDR_A_REG; /*!< CH2_BUF_BASE_ADDR_A_REG Register (0x0B0)                                   */
    __IO uint32_t CH2_BUF_BASE_ADDR_B_REG; /*!< CH2_BUF_BASE_ADDR_B_REG Register (0x0B4)                                   */
    __I uint32_t RESERVED5[2];
    __IO uint32_t CH3_SWRST_REG;  /*!< CH3_SWRST_REG Register (0x0C0)                                             */
    __IO uint32_t CH3_CONFIG_REG; /*!< CH3_CONFIG_REG Register (0x0C4)                                            */
    __IO uint32_t CH3_BUF_EN_REG; /*!< CH3_BUF_EN_REG Register (0x0C8)                                            */
    __I uint32_t RESERVED6;
    __IO uint32_t CH3_BUF_WORD_SIZE_REG;   /*!< CH3_BUF_WORD_SIZE_REG Register (0x0D0)                                     */
    __IO uint32_t CH3_M2M_SRC_ADDR_REG;    /*!< CH3_M2M_SRC_ADDR_REG Register (0x0D4)                                      */
    __IO uint32_t CH3_M2M_DEST_ADDR_REG;   /*!< CH3_M2M_DEST_ADDR_REG Register (0x0D8)                                     */
    __IO uint32_t CH3_M2M_LEN_REG;         /*!< CH3_M2M_LEN_REG Register (0x0DC)                                           */
    __I uint32_t CH3_CNT_A_REG;            /*!< CH3_CNT_A_REG Register (0x0E0)                                             */
    __I uint32_t CH3_CNT_B_REG;            /*!< CH3_CNT_B_REG Register (0x0E4)                                             */
    __IO uint32_t CH3_TRIG_REG;            /*!< CH3_TRIG_REG Register (0x0E8)                                              */
    __I uint32_t CH3_PING_PONG_PTR_REG;    /*!< CH3_PING_PONG_PTR_REG Register (0x0EC)                                     */
    __IO uint32_t CH3_BUF_BASE_ADDR_A_REG; /*!< CH3_BUF_BASE_ADDR_A_REG Register (0x0F0)                                   */
    __IO uint32_t CH3_BUF_BASE_ADDR_B_REG; /*!< CH3_BUF_BASE_ADDR_B_REG Register (0x0F4)                                   */
    __I uint32_t RESERVED7[2];
    __IO uint32_t CH4_SWRST_REG;  /*!< CH4_SWRST_REG Register (0x100)                                             */
    __IO uint32_t CH4_CONFIG_REG; /*!< CH4_CONFIG_REG Register (0x104)                                            */
    __IO uint32_t CH4_BUF_EN_REG; /*!< CH4_BUF_EN_REG Register (0x108)                                            */
    __I uint32_t RESERVED8;
    __IO uint32_t CH4_BUF_WORD_SIZE_REG;   /*!< CH4_BUF_WORD_SIZE_REG Register (0x110)                                     */
    __IO uint32_t CH4_M2M_SRC_ADDR_REG;    /*!< CH4_M2M_SRC_ADDR_REG Register (0x114)                                      */
    __IO uint32_t CH4_M2M_DEST_ADDR_REG;   /*!< CH4_M2M_DEST_ADDR_REG Register (0x118)                                     */
    __IO uint32_t CH4_M2M_LEN_REG;         /*!< CH4_M2M_LEN_REG Register (0x11C)                                           */
    __I uint32_t CH4_CNT_A_REG;            /*!< CH4_CNT_A_REG Register (0x120)                                             */
    __I uint32_t CH4_CNT_B_REG;            /*!< CH4_CNT_B_REG Register (0x124)                                             */
    __IO uint32_t CH4_TRIG_REG;            /*!< CH4_TRIG_REG Register (0x128)                                              */
    __I uint32_t CH4_PING_PONG_PTR_REG;    /*!< CH4_PING_PONG_PTR_REG Register (0x12C)                                     */
    __IO uint32_t CH4_BUF_BASE_ADDR_A_REG; /*!< CH4_BUF_BASE_ADDR_A_REG Register (0x130)                                   */
    __IO uint32_t CH4_BUF_BASE_ADDR_B_REG; /*!< CH4_BUF_BASE_ADDR_B_REG Register (0x134)                                   */
    __I uint32_t RESERVED9[2];
    __IO uint32_t CH5_SWRST_REG;  /*!< CH5_SWRST_REG Register (0x140)                                             */
    __IO uint32_t CH5_CONFIG_REG; /*!< CH5_CONFIG_REG Register (0x144)                                            */
    __IO uint32_t CH5_BUF_EN_REG; /*!< CH5_BUF_EN_REG Register (0x148)                                            */
    __I uint32_t RESERVED10;
    __IO uint32_t CH5_BUF_WORD_SIZE_REG;   /*!< CH5_BUF_WORD_SIZE_REG Register (0x150)                                     */
    __IO uint32_t CH5_M2M_SRC_ADDR_REG;    /*!< CH5_M2M_SRC_ADDR_REG Register (0x154)                                      */
    __IO uint32_t CH5_M2M_DEST_ADDR_REG;   /*!< CH5_M2M_DEST_ADDR_REG Register (0x158)                                     */
    __IO uint32_t CH5_M2M_LEN_REG;         /*!< CH5_M2M_LEN_REG Register (0x15C)                                           */
    __I uint32_t CH5_CNT_A_REG;            /*!< CH5_CNT_A_REG Register (0x160)                                             */
    __I uint32_t CH5_CNT_B_REG;            /*!< CH5_CNT_B_REG Register (0x164)                                             */
    __IO uint32_t CH5_TRIG_REG;            /*!< CH5_TRIG_REG Register (0x168)                                              */
    __I uint32_t CH5_PING_PONG_PTR_REG;    /*!< CH5_PING_PONG_PTR_REG Register (0x16C)                                     */
    __IO uint32_t CH5_BUF_BASE_ADDR_A_REG; /*!< CH5_BUF_BASE_ADDR_A_REG Register (0x170)                                   */
    __IO uint32_t CH5_BUF_BASE_ADDR_B_REG; /*!< CH5_BUF_BASE_ADDR_B_REG Register (0x174)                                   */
    __I uint32_t RESERVED11[2];
    __IO uint32_t CH6_SWRST_REG;  /*!< CH6_SWRST_REG Register (0x180)                                             */
    __IO uint32_t CH6_CONFIG_REG; /*!< CH6_CONFIG_REG Register (0x184)                                            */
    __IO uint32_t CH6_BUF_EN_REG; /*!< CH6_BUF_EN_REG Register (0x188)                                            */
    __I uint32_t RESERVED12;
    __IO uint32_t CH6_BUF_WORD_SIZE_REG;   /*!< CH6_BUF_WORD_SIZE_REG Register (0x190)                                     */
    __IO uint32_t CH6_M2M_SRC_ADDR_REG;    /*!< CH6_M2M_SRC_ADDR_REG Register (0x194)                                      */
    __IO uint32_t CH6_M2M_DEST_ADDR_REG;   /*!< CH6_M2M_DEST_ADDR_REG Register (0x198)                                     */
    __IO uint32_t CH6_M2M_LEN_REG;         /*!< CH6_M2M_LEN_REG Register (0x19C)                                           */
    __I uint32_t CH6_CNT_A_REG;            /*!< CH6_CNT_A_REG Register (0x1A0)                                             */
    __I uint32_t CH6_CNT_B_REG;            /*!< CH6_CNT_B_REG Register (0x1A4)                                             */
    __IO uint32_t CH6_TRIG_REG;            /*!< CH6_TRIG_REG Register (0x1A8)                                              */
    __I uint32_t CH6_PING_PONG_PTR_REG;    /*!< CH6_PING_PONG_PTR_REG Register (0x1AC)                                     */
    __IO uint32_t CH6_BUF_BASE_ADDR_A_REG; /*!< CH6_BUF_BASE_ADDR_A_REG Register (0x1B0)                                   */
    __IO uint32_t CH6_BUF_BASE_ADDR_B_REG; /*!< CH6_BUF_BASE_ADDR_B_REG Register (0x1B4)                                   */
    __I uint32_t RESERVED13[2];
    __IO uint32_t CH7_SWRST_REG;  /*!< CH7_SWRST_REG Register (0x1C0)                                             */
    __IO uint32_t CH7_CONFIG_REG; /*!< CH7_CONFIG_REG Register (0x1C4)                                            */
    __IO uint32_t CH7_BUF_EN_REG; /*!< CH7_BUF_EN_REG Register (0x1C8)                                            */
    __I uint32_t RESERVED14;
    __IO uint32_t CH7_BUF_WORD_SIZE_REG;   /*!< CH7_BUF_WORD_SIZE_REG Register (0x1D0)                                     */
    __IO uint32_t CH7_M2M_SRC_ADDR_REG;    /*!< CH7_M2M_SRC_ADDR_REG Register (0x1D4)                                      */
    __IO uint32_t CH7_M2M_DEST_ADDR_REG;   /*!< CH7_M2M_DEST_ADDR_REG Register (0x1D8)                                     */
    __IO uint32_t CH7_M2M_LEN_REG;         /*!< CH7_M2M_LEN_REG Register (0x1DC)                                           */
    __I uint32_t CH7_CNT_A_REG;            /*!< CH7_CNT_A_REG Register (0x1E0)                                             */
    __I uint32_t CH7_CNT_B_REG;            /*!< CH7_CNT_B_REG Register (0x1E4)                                             */
    __IO uint32_t CH7_TRIG_REG;            /*!< CH7_TRIG_REG Register (0x1E8)                                              */
    __I uint32_t CH7_PING_PONG_PTR_REG;    /*!< CH7_PING_PONG_PTR_REG Register (0x1EC)                                     */
    __IO uint32_t CH7_BUF_BASE_ADDR_A_REG; /*!< CH7_BUF_BASE_ADDR_A_REG Register (0x1F0)                                   */
    __IO uint32_t CH7_BUF_BASE_ADDR_B_REG; /*!< CH7_BUF_BASE_ADDR_B_REG Register (0x1F4)                                   */
    __I uint32_t RESERVED15[514];
    __IO uint32_t INT_ST_CH_CMPLT_A_REG;       /*!< INT_ST_CH_CMPLT_A_REG Register (0xA00)                                     */
    __IO uint32_t INT_ST_CH_CMPLT_B_REG;       /*!< INT_ST_CH_CMPLT_B_REG Register (0xA04)                                     */
    __IO uint32_t INT_ST_CH_ERR_EX_SIZE_A_REG; /*!< INT_ST_CH_ERR_EX_SIZE_A_REG Register (0xA08)                               */
    __IO uint32_t INT_ST_CH_ERR_EX_SIZE_B_REG; /*!< INT_ST_CH_ERR_EX_SIZE_B_REG Register (0xA0C)                               */
    __IO uint32_t INT_ST_CH_ERR_OVRN_A_REG;    /*!< INT_ST_CH_ERR_OVRN_A_REG Register (0xA10)                                  */
    __IO uint32_t INT_ST_CH_ERR_OVRN_B_REG;    /*!< INT_ST_CH_ERR_OVRN_B_REG Register (0xA14)                                  */
    __IO uint32_t INT_ST_CH_M2M_CMPLT_REG;     /*!< INT_ST_CH_M2M_CMPLT_REG Register (0xA18)                                   */
    __I uint32_t RESERVED16[9];
    __IO uint32_t INT_EN_CH_CMPLT_A_REG;       /*!< INT_EN_CH_CMPLT_A_REG Register (0xA40)                                     */
    __IO uint32_t INT_EN_CH_CMPLT_B_REG;       /*!< INT_EN_CH_CMPLT_B_REG Register (0xA44)                                     */
    __IO uint32_t INT_EN_CH_ERR_EX_SIZE_A_REG; /*!< INT_EN_CH_ERR_EX_SIZE_A_REG Register (0xA48)                               */
    __IO uint32_t INT_EN_CH_ERR_EX_SIZE_B_REG; /*!< INT_EN_CH_ERR_EX_SIZE_B_REG Register (0xA4C)                               */
    __IO uint32_t INT_EN_CH_ERR_OVRN_A_REG;    /*!< INT_EN_CH_ERR_OVRN_A_REG Register (0xA50)                                  */
    __IO uint32_t INT_EN_CH_ERR_OVRN_B_REG;    /*!< INT_EN_CH_ERR_OVRN_B_REG Register (0xA54)                                  */
    __IO uint32_t INT_EN_CH_M2M_CMPLT_REG;     /*!< INT_EN_CH_M2M_CMPLT_REG Register (0xA58)                                   */
    __I uint32_t RESERVED17[9];
    __I uint32_t MASKED_INT_ST_CH_CMPLT_A_REG;       /*!< MASKED_INT_ST_CH_CMPLT_A_REG Register (0xA80)                              */
    __I uint32_t MASKED_INT_ST_CH_CMPLT_B_REG;       /*!< MASKED_INT_ST_CH_CMPLT_B_REG Register (0xA84)                              */
    __I uint32_t MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG; /*!< MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG Register (0xA88)                        */
    __I uint32_t MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG; /*!< MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG Register (0xA8C)                        */
    __I uint32_t MASKED_INT_ST_CH_ERR_OVRN_A_REG;    /*!< MASKED_INT_ST_CH_ERR_OVRN_A_REG Register (0xA90)                           */
    __I uint32_t MASKED_INT_ST_CH_ERR_OVRN_B_REG;    /*!< MASKED_INT_ST_CH_ERR_OVRN_B_REG Register (0xA94)                           */
    __I uint32_t MASKED_INT_ST_CH_M2M_CMPLT_REG;     /*!< MASKED_INT_ST_CH_M2M_CMPLT_REG Register (0xA98)                            */
    __I uint32_t RESERVED18[337];
    __I uint32_t PERIPH_ID_LEN_REG; /*!< PERIPH_ID_LEN_REG Register (0xFE0)                                         */
    __I uint32_t RESERVED19[3];
    __IO uint32_t PERIPH_ID_0_REG; /*!< PERIPH_ID_0_REG Register (0xFF0)                                           */
    __IO uint32_t PERIPH_ID_1_REG; /*!< PERIPH_ID_1_REG Register (0xFF4)                                           */
    __IO uint32_t PERIPH_ID_2_REG; /*!< PERIPH_ID_2_REG Register (0xFF8)                                           */
} DMA_T;

#define DMA_CH0_SWRST_REG_ADDR (uint32_t)&(DMA->CH0_SWRST_REG)
#define DMA_CH0_SWRST_ADDR (uint32_t)&(DMA->CH0_SWRST_REG)
#define DMA_CH0_SWRST_POS (0) /*< bit[0]      */
#define DMA_CH0_SWRST_MASK (0x1ul << DMA_CH0_SWRST_POS)

#define DMA_CH0_CONFIG_REG_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_EN_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_EN_POS (0) /*< bit[0]      */
#define DMA_CH0_EN_MASK (0x1ul << DMA_CH0_EN_POS)
#define DMA_CH0_M2M_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_M2M_POS (1) /*< bit[1]      */
#define DMA_CH0_M2M_MASK (0x1ul << DMA_CH0_M2M_POS)
#define DMA_CH0_PING_PONG_EN_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_PING_PONG_EN_POS (2) /*< bit[2]      */
#define DMA_CH0_PING_PONG_EN_MASK (0x1ul << DMA_CH0_PING_PONG_EN_POS)
#define DMA_CH0_M2M_DAT_SIZE_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_M2M_DAT_SIZE_POS (4) /*< bit[5:4]    */
#define DMA_CH0_M2M_DAT_SIZE_MASK (0x3ul << DMA_CH0_M2M_DAT_SIZE_POS)
#define DMA_CH0_M2M_SRC_TYP_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_M2M_SRC_TYP_POS (8) /*< bit[9:8]    */
#define DMA_CH0_M2M_SRC_TYP_MASK (0x3ul << DMA_CH0_M2M_SRC_TYP_POS)
#define DMA_CH0_M2M_DEST_TYP_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_M2M_DEST_TYP_POS (10) /*< bit[11:10]  */
#define DMA_CH0_M2M_DEST_TYP_MASK (0x3ul << DMA_CH0_M2M_DEST_TYP_POS)
#define DMA_CH0_M2M_SRC_NXT_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_M2M_SRC_NXT_POS (12) /*< bit[12]     */
#define DMA_CH0_M2M_SRC_NXT_MASK (0x1ul << DMA_CH0_M2M_SRC_NXT_POS)
#define DMA_CH0_M2M_DEST_NXT_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_M2M_DEST_NXT_POS (13) /*< bit[13]     */
#define DMA_CH0_M2M_DEST_NXT_MASK (0x1ul << DMA_CH0_M2M_DEST_NXT_POS)
#define DMA_CH0_PERI_REQ_IDX_ADDR (uint32_t)&(DMA->CH0_CONFIG_REG)
#define DMA_CH0_PERI_REQ_IDX_POS (24) /*< bit[27:24]  */
#define DMA_CH0_PERI_REQ_IDX_MASK (0xFul << DMA_CH0_PERI_REQ_IDX_POS)

#define DMA_CH0_BUF_EN_REG_ADDR (uint32_t)&(DMA->CH0_BUF_EN_REG)
#define DMA_CH0_BUF_EN_A_ADDR (uint32_t)&(DMA->CH0_BUF_EN_REG)
#define DMA_CH0_BUF_EN_A_POS (0) /*< bit[0]      */
#define DMA_CH0_BUF_EN_A_MASK (0x1ul << DMA_CH0_BUF_EN_A_POS)
#define DMA_CH0_BUF_EN_B_ADDR (uint32_t)&(DMA->CH0_BUF_EN_REG)
#define DMA_CH0_BUF_EN_B_POS (1) /*< bit[1]      */
#define DMA_CH0_BUF_EN_B_MASK (0x1ul << DMA_CH0_BUF_EN_B_POS)

#define DMA_CH0_BUF_WORD_SIZE_REG_ADDR (uint32_t)&(DMA->CH0_BUF_WORD_SIZE_REG)
#define DMA_CH0_BUF_WORD_SIZE_ADDR (uint32_t)&(DMA->CH0_BUF_WORD_SIZE_REG)
#define DMA_CH0_BUF_WORD_SIZE_POS (0) /*< bit[11:0]   */
#define DMA_CH0_BUF_WORD_SIZE_MASK (0xFFFul << DMA_CH0_BUF_WORD_SIZE_POS)
#define DMA_CH0_BUF_END_BY_PERI_ADDR (uint32_t)&(DMA->CH0_BUF_WORD_SIZE_REG)
#define DMA_CH0_BUF_END_BY_PERI_POS (31) /*< bit[31]     */
#define DMA_CH0_BUF_END_BY_PERI_MASK (0x1ul << DMA_CH0_BUF_END_BY_PERI_POS)

#define DMA_CH0_M2M_SRC_ADDR_REG_ADDR (uint32_t)&(DMA->CH0_M2M_SRC_ADDR_REG)
#define DMA_CH0_M2M_DEST_ADDR_REG_ADDR (uint32_t)&(DMA->CH0_M2M_DEST_ADDR_REG)
#define DMA_CH0_M2M_LEN_REG_ADDR (uint32_t)&(DMA->CH0_M2M_LEN_REG)
#define DMA_CH0_M2M_LEN_ADDR (uint32_t)&(DMA->CH0_M2M_LEN_REG)
#define DMA_CH0_M2M_LEN_POS (0) /*< bit[13:0]   */
#define DMA_CH0_M2M_LEN_MASK (0x3FFFul << DMA_CH0_M2M_LEN_POS)

#define DMA_CH0_CNT_A_REG_ADDR (uint32_t)&(DMA->CH0_CNT_A_REG)
#define DMA_CH0_CNT_A_ADDR (uint32_t)&(DMA->CH0_CNT_A_REG)
#define DMA_CH0_CNT_A_POS (0) /*< bit[13:0]   */
#define DMA_CH0_CNT_A_MASK (0x3FFFul << DMA_CH0_CNT_A_POS)

#define DMA_CH0_CNT_B_REG_ADDR (uint32_t)&(DMA->CH0_CNT_B_REG)
#define DMA_CH0_CNT_B_ADDR (uint32_t)&(DMA->CH0_CNT_B_REG)
#define DMA_CH0_CNT_B_POS (0) /*< bit[13:0]   */
#define DMA_CH0_CNT_B_MASK (0x3FFFul << DMA_CH0_CNT_B_POS)

#define DMA_CH0_TRIG_REG_ADDR (uint32_t)&(DMA->CH0_TRIG_REG)
#define DMA_W1T_CH0_M2M_TRIG_ADDR (uint32_t)&(DMA->CH0_TRIG_REG)
#define DMA_W1T_CH0_M2M_TRIG_POS (0) /*< bit[0]      */
#define DMA_W1T_CH0_M2M_TRIG_MASK (0x1ul << DMA_W1T_CH0_M2M_TRIG_POS)
#define DMA_CH0_M2M_EXT_RUN_ADDR (uint32_t)&(DMA->CH0_TRIG_REG)
#define DMA_CH0_M2M_EXT_RUN_POS (8) /*< bit[8]      */
#define DMA_CH0_M2M_EXT_RUN_MASK (0x1ul << DMA_CH0_M2M_EXT_RUN_POS)
#define DMA_CH0_M2M_EXT_SRC_ADDR (uint32_t)&(DMA->CH0_TRIG_REG)
#define DMA_CH0_M2M_EXT_SRC_POS (12) /*< bit[15:12]  */
#define DMA_CH0_M2M_EXT_SRC_MASK (0xFul << DMA_CH0_M2M_EXT_SRC_POS)

#define DMA_CH0_PING_PONG_PTR_REG_ADDR (uint32_t)&(DMA->CH0_PING_PONG_PTR_REG)
#define DMA_CH0_PING_PONG_PTR_ABB_ADDR (uint32_t)&(DMA->CH0_PING_PONG_PTR_REG)
#define DMA_CH0_PING_PONG_PTR_ABB_POS (0) /*< bit[0]      */
#define DMA_CH0_PING_PONG_PTR_ABB_MASK (0x1ul << DMA_CH0_PING_PONG_PTR_ABB_POS)

#define DMA_CH0_BUF_BASE_ADDR_A_REG_ADDR (uint32_t)&(DMA->CH0_BUF_BASE_ADDR_A_REG)
#define DMA_CH0_BUF_BASE_ADDR_B_REG_ADDR (uint32_t)&(DMA->CH0_BUF_BASE_ADDR_B_REG)
#define DMA_CH1_SWRST_REG_ADDR (uint32_t)&(DMA->CH1_SWRST_REG)
#define DMA_CH1_SWRST_ADDR (uint32_t)&(DMA->CH1_SWRST_REG)
#define DMA_CH1_SWRST_POS (0) /*< bit[0]      */
#define DMA_CH1_SWRST_MASK (0x1ul << DMA_CH1_SWRST_POS)

#define DMA_CH1_CONFIG_REG_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_EN_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_EN_POS (0) /*< bit[0]      */
#define DMA_CH1_EN_MASK (0x1ul << DMA_CH1_EN_POS)
#define DMA_CH1_M2M_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_M2M_POS (1) /*< bit[1]      */
#define DMA_CH1_M2M_MASK (0x1ul << DMA_CH1_M2M_POS)
#define DMA_CH1_PING_PONG_EN_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_PING_PONG_EN_POS (2) /*< bit[2]      */
#define DMA_CH1_PING_PONG_EN_MASK (0x1ul << DMA_CH1_PING_PONG_EN_POS)
#define DMA_CH1_M2M_DAT_SIZE_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_M2M_DAT_SIZE_POS (4) /*< bit[5:4]    */
#define DMA_CH1_M2M_DAT_SIZE_MASK (0x3ul << DMA_CH1_M2M_DAT_SIZE_POS)
#define DMA_CH1_M2M_SRC_TYP_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_M2M_SRC_TYP_POS (8) /*< bit[9:8]    */
#define DMA_CH1_M2M_SRC_TYP_MASK (0x3ul << DMA_CH1_M2M_SRC_TYP_POS)
#define DMA_CH1_M2M_DEST_TYP_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_M2M_DEST_TYP_POS (10) /*< bit[11:10]  */
#define DMA_CH1_M2M_DEST_TYP_MASK (0x3ul << DMA_CH1_M2M_DEST_TYP_POS)
#define DMA_CH1_M2M_SRC_NXT_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_M2M_SRC_NXT_POS (12) /*< bit[12]     */
#define DMA_CH1_M2M_SRC_NXT_MASK (0x1ul << DMA_CH1_M2M_SRC_NXT_POS)
#define DMA_CH1_M2M_DEST_NXT_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_M2M_DEST_NXT_POS (13) /*< bit[13]     */
#define DMA_CH1_M2M_DEST_NXT_MASK (0x1ul << DMA_CH1_M2M_DEST_NXT_POS)
#define DMA_CH1_PERI_REQ_IDX_ADDR (uint32_t)&(DMA->CH1_CONFIG_REG)
#define DMA_CH1_PERI_REQ_IDX_POS (24) /*< bit[27:24]  */
#define DMA_CH1_PERI_REQ_IDX_MASK (0xFul << DMA_CH1_PERI_REQ_IDX_POS)

#define DMA_CH1_BUF_EN_REG_ADDR (uint32_t)&(DMA->CH1_BUF_EN_REG)
#define DMA_CH1_BUF_EN_A_ADDR (uint32_t)&(DMA->CH1_BUF_EN_REG)
#define DMA_CH1_BUF_EN_A_POS (0) /*< bit[0]      */
#define DMA_CH1_BUF_EN_A_MASK (0x1ul << DMA_CH1_BUF_EN_A_POS)
#define DMA_CH1_BUF_EN_B_ADDR (uint32_t)&(DMA->CH1_BUF_EN_REG)
#define DMA_CH1_BUF_EN_B_POS (1) /*< bit[1]      */
#define DMA_CH1_BUF_EN_B_MASK (0x1ul << DMA_CH1_BUF_EN_B_POS)

#define DMA_CH1_BUF_WORD_SIZE_REG_ADDR (uint32_t)&(DMA->CH1_BUF_WORD_SIZE_REG)
#define DMA_CH1_BUF_WORD_SIZE_ADDR (uint32_t)&(DMA->CH1_BUF_WORD_SIZE_REG)
#define DMA_CH1_BUF_WORD_SIZE_POS (0) /*< bit[11:0]   */
#define DMA_CH1_BUF_WORD_SIZE_MASK (0xFFFul << DMA_CH1_BUF_WORD_SIZE_POS)
#define DMA_CH1_BUF_END_BY_PERI_ADDR (uint32_t)&(DMA->CH1_BUF_WORD_SIZE_REG)
#define DMA_CH1_BUF_END_BY_PERI_POS (31) /*< bit[31]     */
#define DMA_CH1_BUF_END_BY_PERI_MASK (0x1ul << DMA_CH1_BUF_END_BY_PERI_POS)

#define DMA_CH1_M2M_SRC_ADDR_REG_ADDR (uint32_t)&(DMA->CH1_M2M_SRC_ADDR_REG)
#define DMA_CH1_M2M_DEST_ADDR_REG_ADDR (uint32_t)&(DMA->CH1_M2M_DEST_ADDR_REG)
#define DMA_CH1_M2M_LEN_REG_ADDR (uint32_t)&(DMA->CH1_M2M_LEN_REG)
#define DMA_CH1_M2M_LEN_ADDR (uint32_t)&(DMA->CH1_M2M_LEN_REG)
#define DMA_CH1_M2M_LEN_POS (0) /*< bit[13:0]   */
#define DMA_CH1_M2M_LEN_MASK (0x3FFFul << DMA_CH1_M2M_LEN_POS)

#define DMA_CH1_CNT_A_REG_ADDR (uint32_t)&(DMA->CH1_CNT_A_REG)
#define DMA_CH1_CNT_A_ADDR (uint32_t)&(DMA->CH1_CNT_A_REG)
#define DMA_CH1_CNT_A_POS (0) /*< bit[13:0]   */
#define DMA_CH1_CNT_A_MASK (0x3FFFul << DMA_CH1_CNT_A_POS)

#define DMA_CH1_CNT_B_REG_ADDR (uint32_t)&(DMA->CH1_CNT_B_REG)
#define DMA_CH1_CNT_B_ADDR (uint32_t)&(DMA->CH1_CNT_B_REG)
#define DMA_CH1_CNT_B_POS (0) /*< bit[13:0]   */
#define DMA_CH1_CNT_B_MASK (0x3FFFul << DMA_CH1_CNT_B_POS)

#define DMA_CH1_TRIG_REG_ADDR (uint32_t)&(DMA->CH1_TRIG_REG)
#define DMA_W1T_CH1_M2M_TRIG_ADDR (uint32_t)&(DMA->CH1_TRIG_REG)
#define DMA_W1T_CH1_M2M_TRIG_POS (0) /*< bit[0]      */
#define DMA_W1T_CH1_M2M_TRIG_MASK (0x1ul << DMA_W1T_CH1_M2M_TRIG_POS)
#define DMA_CH1_M2M_EXT_RUN_ADDR (uint32_t)&(DMA->CH1_TRIG_REG)
#define DMA_CH1_M2M_EXT_RUN_POS (8) /*< bit[8]      */
#define DMA_CH1_M2M_EXT_RUN_MASK (0x1ul << DMA_CH1_M2M_EXT_RUN_POS)
#define DMA_CH1_M2M_EXT_SRC_ADDR (uint32_t)&(DMA->CH1_TRIG_REG)
#define DMA_CH1_M2M_EXT_SRC_POS (12) /*< bit[15:12]  */
#define DMA_CH1_M2M_EXT_SRC_MASK (0xFul << DMA_CH1_M2M_EXT_SRC_POS)
#define DMA_CH1_TX_THD_ADDR (uint32_t)&(DMA->CH1_TRIG_REG)
#define DMA_CH1_TX_THD_POS (16) /*< bit[29:16]  */
#define DMA_CH1_TX_THD_MASK (0x3FFFul << DMA_CH1_TX_THD_POS)

#define DMA_CH1_PING_PONG_PTR_REG_ADDR (uint32_t)&(DMA->CH1_PING_PONG_PTR_REG)
#define DMA_CH1_PING_PONG_PTR_ABB_ADDR (uint32_t)&(DMA->CH1_PING_PONG_PTR_REG)
#define DMA_CH1_PING_PONG_PTR_ABB_POS (0) /*< bit[0]      */
#define DMA_CH1_PING_PONG_PTR_ABB_MASK (0x1ul << DMA_CH1_PING_PONG_PTR_ABB_POS)

#define DMA_CH1_BUF_BASE_ADDR_A_REG_ADDR (uint32_t)&(DMA->CH1_BUF_BASE_ADDR_A_REG)
#define DMA_CH1_BUF_BASE_ADDR_B_REG_ADDR (uint32_t)&(DMA->CH1_BUF_BASE_ADDR_B_REG)
#define DMA_CH2_SWRST_REG_ADDR (uint32_t)&(DMA->CH2_SWRST_REG)
#define DMA_CH2_SWRST_ADDR (uint32_t)&(DMA->CH2_SWRST_REG)
#define DMA_CH2_SWRST_POS (0) /*< bit[0]      */
#define DMA_CH2_SWRST_MASK (0x1ul << DMA_CH2_SWRST_POS)

#define DMA_CH2_CONFIG_REG_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_EN_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_EN_POS (0) /*< bit[0]      */
#define DMA_CH2_EN_MASK (0x1ul << DMA_CH2_EN_POS)
#define DMA_CH2_M2M_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_M2M_POS (1) /*< bit[1]      */
#define DMA_CH2_M2M_MASK (0x1ul << DMA_CH2_M2M_POS)
#define DMA_CH2_PING_PONG_EN_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_PING_PONG_EN_POS (2) /*< bit[2]      */
#define DMA_CH2_PING_PONG_EN_MASK (0x1ul << DMA_CH2_PING_PONG_EN_POS)
#define DMA_CH2_M2M_DAT_SIZE_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_M2M_DAT_SIZE_POS (4) /*< bit[5:4]    */
#define DMA_CH2_M2M_DAT_SIZE_MASK (0x3ul << DMA_CH2_M2M_DAT_SIZE_POS)
#define DMA_CH2_M2M_SRC_TYP_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_M2M_SRC_TYP_POS (8) /*< bit[9:8]    */
#define DMA_CH2_M2M_SRC_TYP_MASK (0x3ul << DMA_CH2_M2M_SRC_TYP_POS)
#define DMA_CH2_M2M_DEST_TYP_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_M2M_DEST_TYP_POS (10) /*< bit[11:10]  */
#define DMA_CH2_M2M_DEST_TYP_MASK (0x3ul << DMA_CH2_M2M_DEST_TYP_POS)
#define DMA_CH2_M2M_SRC_NXT_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_M2M_SRC_NXT_POS (12) /*< bit[12]     */
#define DMA_CH2_M2M_SRC_NXT_MASK (0x1ul << DMA_CH2_M2M_SRC_NXT_POS)
#define DMA_CH2_M2M_DEST_NXT_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_M2M_DEST_NXT_POS (13) /*< bit[13]     */
#define DMA_CH2_M2M_DEST_NXT_MASK (0x1ul << DMA_CH2_M2M_DEST_NXT_POS)
#define DMA_CH2_PERI_REQ_IDX_ADDR (uint32_t)&(DMA->CH2_CONFIG_REG)
#define DMA_CH2_PERI_REQ_IDX_POS (24) /*< bit[27:24]  */
#define DMA_CH2_PERI_REQ_IDX_MASK (0xFul << DMA_CH2_PERI_REQ_IDX_POS)

#define DMA_CH2_BUF_EN_REG_ADDR (uint32_t)&(DMA->CH2_BUF_EN_REG)
#define DMA_CH2_BUF_EN_A_ADDR (uint32_t)&(DMA->CH2_BUF_EN_REG)
#define DMA_CH2_BUF_EN_A_POS (0) /*< bit[0]      */
#define DMA_CH2_BUF_EN_A_MASK (0x1ul << DMA_CH2_BUF_EN_A_POS)
#define DMA_CH2_BUF_EN_B_ADDR (uint32_t)&(DMA->CH2_BUF_EN_REG)
#define DMA_CH2_BUF_EN_B_POS (1) /*< bit[1]      */
#define DMA_CH2_BUF_EN_B_MASK (0x1ul << DMA_CH2_BUF_EN_B_POS)

#define DMA_CH2_BUF_WORD_SIZE_REG_ADDR (uint32_t)&(DMA->CH2_BUF_WORD_SIZE_REG)
#define DMA_CH2_BUF_WORD_SIZE_ADDR (uint32_t)&(DMA->CH2_BUF_WORD_SIZE_REG)
#define DMA_CH2_BUF_WORD_SIZE_POS (0) /*< bit[11:0]   */
#define DMA_CH2_BUF_WORD_SIZE_MASK (0xFFFul << DMA_CH2_BUF_WORD_SIZE_POS)
#define DMA_CH2_BUF_END_BY_PERI_ADDR (uint32_t)&(DMA->CH2_BUF_WORD_SIZE_REG)
#define DMA_CH2_BUF_END_BY_PERI_POS (31) /*< bit[31]     */
#define DMA_CH2_BUF_END_BY_PERI_MASK (0x1ul << DMA_CH2_BUF_END_BY_PERI_POS)

#define DMA_CH2_M2M_SRC_ADDR_REG_ADDR (uint32_t)&(DMA->CH2_M2M_SRC_ADDR_REG)
#define DMA_CH2_M2M_DEST_ADDR_REG_ADDR (uint32_t)&(DMA->CH2_M2M_DEST_ADDR_REG)
#define DMA_CH2_M2M_LEN_REG_ADDR (uint32_t)&(DMA->CH2_M2M_LEN_REG)
#define DMA_CH2_M2M_LEN_ADDR (uint32_t)&(DMA->CH2_M2M_LEN_REG)
#define DMA_CH2_M2M_LEN_POS (0) /*< bit[13:0]   */
#define DMA_CH2_M2M_LEN_MASK (0x3FFFul << DMA_CH2_M2M_LEN_POS)

#define DMA_CH2_CNT_A_REG_ADDR (uint32_t)&(DMA->CH2_CNT_A_REG)
#define DMA_CH2_CNT_A_ADDR (uint32_t)&(DMA->CH2_CNT_A_REG)
#define DMA_CH2_CNT_A_POS (0) /*< bit[13:0]   */
#define DMA_CH2_CNT_A_MASK (0x3FFFul << DMA_CH2_CNT_A_POS)

#define DMA_CH2_CNT_B_REG_ADDR (uint32_t)&(DMA->CH2_CNT_B_REG)
#define DMA_CH2_CNT_B_ADDR (uint32_t)&(DMA->CH2_CNT_B_REG)
#define DMA_CH2_CNT_B_POS (0) /*< bit[13:0]   */
#define DMA_CH2_CNT_B_MASK (0x3FFFul << DMA_CH2_CNT_B_POS)

#define DMA_CH2_TRIG_REG_ADDR (uint32_t)&(DMA->CH2_TRIG_REG)
#define DMA_W1T_CH2_M2M_TRIG_ADDR (uint32_t)&(DMA->CH2_TRIG_REG)
#define DMA_W1T_CH2_M2M_TRIG_POS (0) /*< bit[0]      */
#define DMA_W1T_CH2_M2M_TRIG_MASK (0x1ul << DMA_W1T_CH2_M2M_TRIG_POS)
#define DMA_CH2_M2M_EXT_RUN_ADDR (uint32_t)&(DMA->CH2_TRIG_REG)
#define DMA_CH2_M2M_EXT_RUN_POS (8) /*< bit[8]      */
#define DMA_CH2_M2M_EXT_RUN_MASK (0x1ul << DMA_CH2_M2M_EXT_RUN_POS)
#define DMA_CH2_M2M_EXT_SRC_ADDR (uint32_t)&(DMA->CH2_TRIG_REG)
#define DMA_CH2_M2M_EXT_SRC_POS (12) /*< bit[15:12]  */
#define DMA_CH2_M2M_EXT_SRC_MASK (0xFul << DMA_CH2_M2M_EXT_SRC_POS)

#define DMA_CH2_PING_PONG_PTR_REG_ADDR (uint32_t)&(DMA->CH2_PING_PONG_PTR_REG)
#define DMA_CH2_PING_PONG_PTR_ABB_ADDR (uint32_t)&(DMA->CH2_PING_PONG_PTR_REG)
#define DMA_CH2_PING_PONG_PTR_ABB_POS (0) /*< bit[0]      */
#define DMA_CH2_PING_PONG_PTR_ABB_MASK (0x1ul << DMA_CH2_PING_PONG_PTR_ABB_POS)

#define DMA_CH2_BUF_BASE_ADDR_A_REG_ADDR (uint32_t)&(DMA->CH2_BUF_BASE_ADDR_A_REG)
#define DMA_CH2_BUF_BASE_ADDR_B_REG_ADDR (uint32_t)&(DMA->CH2_BUF_BASE_ADDR_B_REG)
#define DMA_CH3_SWRST_REG_ADDR (uint32_t)&(DMA->CH3_SWRST_REG)
#define DMA_CH3_SWRST_ADDR (uint32_t)&(DMA->CH3_SWRST_REG)
#define DMA_CH3_SWRST_POS (0) /*< bit[0]      */
#define DMA_CH3_SWRST_MASK (0x1ul << DMA_CH3_SWRST_POS)

#define DMA_CH3_CONFIG_REG_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_EN_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_EN_POS (0) /*< bit[0]      */
#define DMA_CH3_EN_MASK (0x1ul << DMA_CH3_EN_POS)
#define DMA_CH3_M2M_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_M2M_POS (1) /*< bit[1]      */
#define DMA_CH3_M2M_MASK (0x1ul << DMA_CH3_M2M_POS)
#define DMA_CH3_PING_PONG_EN_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_PING_PONG_EN_POS (2) /*< bit[2]      */
#define DMA_CH3_PING_PONG_EN_MASK (0x1ul << DMA_CH3_PING_PONG_EN_POS)
#define DMA_CH3_M2M_DAT_SIZE_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_M2M_DAT_SIZE_POS (4) /*< bit[5:4]    */
#define DMA_CH3_M2M_DAT_SIZE_MASK (0x3ul << DMA_CH3_M2M_DAT_SIZE_POS)
#define DMA_CH3_M2M_SRC_TYP_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_M2M_SRC_TYP_POS (8) /*< bit[9:8]    */
#define DMA_CH3_M2M_SRC_TYP_MASK (0x3ul << DMA_CH3_M2M_SRC_TYP_POS)
#define DMA_CH3_M2M_DEST_TYP_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_M2M_DEST_TYP_POS (10) /*< bit[11:10]  */
#define DMA_CH3_M2M_DEST_TYP_MASK (0x3ul << DMA_CH3_M2M_DEST_TYP_POS)
#define DMA_CH3_M2M_SRC_NXT_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_M2M_SRC_NXT_POS (12) /*< bit[12]     */
#define DMA_CH3_M2M_SRC_NXT_MASK (0x1ul << DMA_CH3_M2M_SRC_NXT_POS)
#define DMA_CH3_M2M_DEST_NXT_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_M2M_DEST_NXT_POS (13) /*< bit[13]     */
#define DMA_CH3_M2M_DEST_NXT_MASK (0x1ul << DMA_CH3_M2M_DEST_NXT_POS)
#define DMA_CH3_PERI_REQ_IDX_ADDR (uint32_t)&(DMA->CH3_CONFIG_REG)
#define DMA_CH3_PERI_REQ_IDX_POS (24) /*< bit[27:24]  */
#define DMA_CH3_PERI_REQ_IDX_MASK (0xFul << DMA_CH3_PERI_REQ_IDX_POS)

#define DMA_CH3_BUF_EN_REG_ADDR (uint32_t)&(DMA->CH3_BUF_EN_REG)
#define DMA_CH3_BUF_EN_A_ADDR (uint32_t)&(DMA->CH3_BUF_EN_REG)
#define DMA_CH3_BUF_EN_A_POS (0) /*< bit[0]      */
#define DMA_CH3_BUF_EN_A_MASK (0x1ul << DMA_CH3_BUF_EN_A_POS)
#define DMA_CH3_BUF_EN_B_ADDR (uint32_t)&(DMA->CH3_BUF_EN_REG)
#define DMA_CH3_BUF_EN_B_POS (1) /*< bit[1]      */
#define DMA_CH3_BUF_EN_B_MASK (0x1ul << DMA_CH3_BUF_EN_B_POS)

#define DMA_CH3_BUF_WORD_SIZE_REG_ADDR (uint32_t)&(DMA->CH3_BUF_WORD_SIZE_REG)
#define DMA_CH3_BUF_WORD_SIZE_ADDR (uint32_t)&(DMA->CH3_BUF_WORD_SIZE_REG)
#define DMA_CH3_BUF_WORD_SIZE_POS (0) /*< bit[11:0]   */
#define DMA_CH3_BUF_WORD_SIZE_MASK (0xFFFul << DMA_CH3_BUF_WORD_SIZE_POS)
#define DMA_CH3_BUF_END_BY_PERI_ADDR (uint32_t)&(DMA->CH3_BUF_WORD_SIZE_REG)
#define DMA_CH3_BUF_END_BY_PERI_POS (31) /*< bit[31]     */
#define DMA_CH3_BUF_END_BY_PERI_MASK (0x1ul << DMA_CH3_BUF_END_BY_PERI_POS)

#define DMA_CH3_M2M_SRC_ADDR_REG_ADDR (uint32_t)&(DMA->CH3_M2M_SRC_ADDR_REG)
#define DMA_CH3_M2M_DEST_ADDR_REG_ADDR (uint32_t)&(DMA->CH3_M2M_DEST_ADDR_REG)
#define DMA_CH3_M2M_LEN_REG_ADDR (uint32_t)&(DMA->CH3_M2M_LEN_REG)
#define DMA_CH3_M2M_LEN_ADDR (uint32_t)&(DMA->CH3_M2M_LEN_REG)
#define DMA_CH3_M2M_LEN_POS (0) /*< bit[13:0]   */
#define DMA_CH3_M2M_LEN_MASK (0x3FFFul << DMA_CH3_M2M_LEN_POS)

#define DMA_CH3_CNT_A_REG_ADDR (uint32_t)&(DMA->CH3_CNT_A_REG)
#define DMA_CH3_CNT_A_ADDR (uint32_t)&(DMA->CH3_CNT_A_REG)
#define DMA_CH3_CNT_A_POS (0) /*< bit[13:0]   */
#define DMA_CH3_CNT_A_MASK (0x3FFFul << DMA_CH3_CNT_A_POS)

#define DMA_CH3_CNT_B_REG_ADDR (uint32_t)&(DMA->CH3_CNT_B_REG)
#define DMA_CH3_CNT_B_ADDR (uint32_t)&(DMA->CH3_CNT_B_REG)
#define DMA_CH3_CNT_B_POS (0) /*< bit[13:0]   */
#define DMA_CH3_CNT_B_MASK (0x3FFFul << DMA_CH3_CNT_B_POS)

#define DMA_CH3_TRIG_REG_ADDR (uint32_t)&(DMA->CH3_TRIG_REG)
#define DMA_W1T_CH3_M2M_TRIG_ADDR (uint32_t)&(DMA->CH3_TRIG_REG)
#define DMA_W1T_CH3_M2M_TRIG_POS (0) /*< bit[0]      */
#define DMA_W1T_CH3_M2M_TRIG_MASK (0x1ul << DMA_W1T_CH3_M2M_TRIG_POS)
#define DMA_CH3_M2M_EXT_RUN_ADDR (uint32_t)&(DMA->CH3_TRIG_REG)
#define DMA_CH3_M2M_EXT_RUN_POS (8) /*< bit[8]      */
#define DMA_CH3_M2M_EXT_RUN_MASK (0x1ul << DMA_CH3_M2M_EXT_RUN_POS)
#define DMA_CH3_M2M_EXT_SRC_ADDR (uint32_t)&(DMA->CH3_TRIG_REG)
#define DMA_CH3_M2M_EXT_SRC_POS (12) /*< bit[15:12]  */
#define DMA_CH3_M2M_EXT_SRC_MASK (0xFul << DMA_CH3_M2M_EXT_SRC_POS)

#define DMA_CH3_PING_PONG_PTR_REG_ADDR (uint32_t)&(DMA->CH3_PING_PONG_PTR_REG)
#define DMA_CH3_PING_PONG_PTR_ABB_ADDR (uint32_t)&(DMA->CH3_PING_PONG_PTR_REG)
#define DMA_CH3_PING_PONG_PTR_ABB_POS (0) /*< bit[0]      */
#define DMA_CH3_PING_PONG_PTR_ABB_MASK (0x1ul << DMA_CH3_PING_PONG_PTR_ABB_POS)

#define DMA_CH3_BUF_BASE_ADDR_A_REG_ADDR (uint32_t)&(DMA->CH3_BUF_BASE_ADDR_A_REG)
#define DMA_CH3_BUF_BASE_ADDR_B_REG_ADDR (uint32_t)&(DMA->CH3_BUF_BASE_ADDR_B_REG)
#define DMA_CH4_SWRST_REG_ADDR (uint32_t)&(DMA->CH4_SWRST_REG)
#define DMA_CH4_SWRST_ADDR (uint32_t)&(DMA->CH4_SWRST_REG)
#define DMA_CH4_SWRST_POS (0) /*< bit[0]      */
#define DMA_CH4_SWRST_MASK (0x1ul << DMA_CH4_SWRST_POS)

#define DMA_CH4_CONFIG_REG_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_EN_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_EN_POS (0) /*< bit[0]      */
#define DMA_CH4_EN_MASK (0x1ul << DMA_CH4_EN_POS)
#define DMA_CH4_M2M_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_M2M_POS (1) /*< bit[1]      */
#define DMA_CH4_M2M_MASK (0x1ul << DMA_CH4_M2M_POS)
#define DMA_CH4_PING_PONG_EN_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_PING_PONG_EN_POS (2) /*< bit[2]      */
#define DMA_CH4_PING_PONG_EN_MASK (0x1ul << DMA_CH4_PING_PONG_EN_POS)
#define DMA_CH4_M2M_DAT_SIZE_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_M2M_DAT_SIZE_POS (4) /*< bit[5:4]    */
#define DMA_CH4_M2M_DAT_SIZE_MASK (0x3ul << DMA_CH4_M2M_DAT_SIZE_POS)
#define DMA_CH4_M2M_SRC_TYP_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_M2M_SRC_TYP_POS (8) /*< bit[9:8]    */
#define DMA_CH4_M2M_SRC_TYP_MASK (0x3ul << DMA_CH4_M2M_SRC_TYP_POS)
#define DMA_CH4_M2M_DEST_TYP_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_M2M_DEST_TYP_POS (10) /*< bit[11:10]  */
#define DMA_CH4_M2M_DEST_TYP_MASK (0x3ul << DMA_CH4_M2M_DEST_TYP_POS)
#define DMA_CH4_M2M_SRC_NXT_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_M2M_SRC_NXT_POS (12) /*< bit[12]     */
#define DMA_CH4_M2M_SRC_NXT_MASK (0x1ul << DMA_CH4_M2M_SRC_NXT_POS)
#define DMA_CH4_M2M_DEST_NXT_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_M2M_DEST_NXT_POS (13) /*< bit[13]     */
#define DMA_CH4_M2M_DEST_NXT_MASK (0x1ul << DMA_CH4_M2M_DEST_NXT_POS)
#define DMA_CH4_PERI_REQ_IDX_ADDR (uint32_t)&(DMA->CH4_CONFIG_REG)
#define DMA_CH4_PERI_REQ_IDX_POS (24) /*< bit[27:24]  */
#define DMA_CH4_PERI_REQ_IDX_MASK (0xFul << DMA_CH4_PERI_REQ_IDX_POS)

#define DMA_CH4_BUF_EN_REG_ADDR (uint32_t)&(DMA->CH4_BUF_EN_REG)
#define DMA_CH4_BUF_EN_A_ADDR (uint32_t)&(DMA->CH4_BUF_EN_REG)
#define DMA_CH4_BUF_EN_A_POS (0) /*< bit[0]      */
#define DMA_CH4_BUF_EN_A_MASK (0x1ul << DMA_CH4_BUF_EN_A_POS)
#define DMA_CH4_BUF_EN_B_ADDR (uint32_t)&(DMA->CH4_BUF_EN_REG)
#define DMA_CH4_BUF_EN_B_POS (1) /*< bit[1]      */
#define DMA_CH4_BUF_EN_B_MASK (0x1ul << DMA_CH4_BUF_EN_B_POS)

#define DMA_CH4_BUF_WORD_SIZE_REG_ADDR (uint32_t)&(DMA->CH4_BUF_WORD_SIZE_REG)
#define DMA_CH4_BUF_WORD_SIZE_ADDR (uint32_t)&(DMA->CH4_BUF_WORD_SIZE_REG)
#define DMA_CH4_BUF_WORD_SIZE_POS (0) /*< bit[11:0]   */
#define DMA_CH4_BUF_WORD_SIZE_MASK (0xFFFul << DMA_CH4_BUF_WORD_SIZE_POS)
#define DMA_CH4_BUF_END_BY_PERI_ADDR (uint32_t)&(DMA->CH4_BUF_WORD_SIZE_REG)
#define DMA_CH4_BUF_END_BY_PERI_POS (31) /*< bit[31]     */
#define DMA_CH4_BUF_END_BY_PERI_MASK (0x1ul << DMA_CH4_BUF_END_BY_PERI_POS)

#define DMA_CH4_M2M_SRC_ADDR_REG_ADDR (uint32_t)&(DMA->CH4_M2M_SRC_ADDR_REG)
#define DMA_CH4_M2M_DEST_ADDR_REG_ADDR (uint32_t)&(DMA->CH4_M2M_DEST_ADDR_REG)
#define DMA_CH4_M2M_LEN_REG_ADDR (uint32_t)&(DMA->CH4_M2M_LEN_REG)
#define DMA_CH4_M2M_LEN_ADDR (uint32_t)&(DMA->CH4_M2M_LEN_REG)
#define DMA_CH4_M2M_LEN_POS (0) /*< bit[13:0]   */
#define DMA_CH4_M2M_LEN_MASK (0x3FFFul << DMA_CH4_M2M_LEN_POS)

#define DMA_CH4_CNT_A_REG_ADDR (uint32_t)&(DMA->CH4_CNT_A_REG)
#define DMA_CH4_CNT_A_ADDR (uint32_t)&(DMA->CH4_CNT_A_REG)
#define DMA_CH4_CNT_A_POS (0) /*< bit[13:0]   */
#define DMA_CH4_CNT_A_MASK (0x3FFFul << DMA_CH4_CNT_A_POS)

#define DMA_CH4_CNT_B_REG_ADDR (uint32_t)&(DMA->CH4_CNT_B_REG)
#define DMA_CH4_CNT_B_ADDR (uint32_t)&(DMA->CH4_CNT_B_REG)
#define DMA_CH4_CNT_B_POS (0) /*< bit[13:0]   */
#define DMA_CH4_CNT_B_MASK (0x3FFFul << DMA_CH4_CNT_B_POS)

#define DMA_CH4_TRIG_REG_ADDR (uint32_t)&(DMA->CH4_TRIG_REG)
#define DMA_W1T_CH4_M2M_TRIG_ADDR (uint32_t)&(DMA->CH4_TRIG_REG)
#define DMA_W1T_CH4_M2M_TRIG_POS (0) /*< bit[0]      */
#define DMA_W1T_CH4_M2M_TRIG_MASK (0x1ul << DMA_W1T_CH4_M2M_TRIG_POS)
#define DMA_CH4_M2M_EXT_RUN_ADDR (uint32_t)&(DMA->CH4_TRIG_REG)
#define DMA_CH4_M2M_EXT_RUN_POS (8) /*< bit[8]      */
#define DMA_CH4_M2M_EXT_RUN_MASK (0x1ul << DMA_CH4_M2M_EXT_RUN_POS)
#define DMA_CH4_M2M_EXT_SRC_ADDR (uint32_t)&(DMA->CH4_TRIG_REG)
#define DMA_CH4_M2M_EXT_SRC_POS (12) /*< bit[15:12]  */
#define DMA_CH4_M2M_EXT_SRC_MASK (0xFul << DMA_CH4_M2M_EXT_SRC_POS)

#define DMA_CH4_PING_PONG_PTR_REG_ADDR (uint32_t)&(DMA->CH4_PING_PONG_PTR_REG)
#define DMA_CH4_PING_PONG_PTR_ABB_ADDR (uint32_t)&(DMA->CH4_PING_PONG_PTR_REG)
#define DMA_CH4_PING_PONG_PTR_ABB_POS (0) /*< bit[0]      */
#define DMA_CH4_PING_PONG_PTR_ABB_MASK (0x1ul << DMA_CH4_PING_PONG_PTR_ABB_POS)

#define DMA_CH4_BUF_BASE_ADDR_A_REG_ADDR (uint32_t)&(DMA->CH4_BUF_BASE_ADDR_A_REG)
#define DMA_CH4_BUF_BASE_ADDR_B_REG_ADDR (uint32_t)&(DMA->CH4_BUF_BASE_ADDR_B_REG)
#define DMA_CH5_SWRST_REG_ADDR (uint32_t)&(DMA->CH5_SWRST_REG)
#define DMA_CH5_SWRST_ADDR (uint32_t)&(DMA->CH5_SWRST_REG)
#define DMA_CH5_SWRST_POS (0) /*< bit[0]      */
#define DMA_CH5_SWRST_MASK (0x1ul << DMA_CH5_SWRST_POS)

#define DMA_CH5_CONFIG_REG_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_EN_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_EN_POS (0) /*< bit[0]      */
#define DMA_CH5_EN_MASK (0x1ul << DMA_CH5_EN_POS)
#define DMA_CH5_M2M_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_M2M_POS (1) /*< bit[1]      */
#define DMA_CH5_M2M_MASK (0x1ul << DMA_CH5_M2M_POS)
#define DMA_CH5_PING_PONG_EN_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_PING_PONG_EN_POS (2) /*< bit[2]      */
#define DMA_CH5_PING_PONG_EN_MASK (0x1ul << DMA_CH5_PING_PONG_EN_POS)
#define DMA_CH5_M2M_DAT_SIZE_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_M2M_DAT_SIZE_POS (4) /*< bit[5:4]    */
#define DMA_CH5_M2M_DAT_SIZE_MASK (0x3ul << DMA_CH5_M2M_DAT_SIZE_POS)
#define DMA_CH5_M2M_SRC_TYP_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_M2M_SRC_TYP_POS (8) /*< bit[9:8]    */
#define DMA_CH5_M2M_SRC_TYP_MASK (0x3ul << DMA_CH5_M2M_SRC_TYP_POS)
#define DMA_CH5_M2M_DEST_TYP_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_M2M_DEST_TYP_POS (10) /*< bit[11:10]  */
#define DMA_CH5_M2M_DEST_TYP_MASK (0x3ul << DMA_CH5_M2M_DEST_TYP_POS)
#define DMA_CH5_M2M_SRC_NXT_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_M2M_SRC_NXT_POS (12) /*< bit[12]     */
#define DMA_CH5_M2M_SRC_NXT_MASK (0x1ul << DMA_CH5_M2M_SRC_NXT_POS)
#define DMA_CH5_M2M_DEST_NXT_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_M2M_DEST_NXT_POS (13) /*< bit[13]     */
#define DMA_CH5_M2M_DEST_NXT_MASK (0x1ul << DMA_CH5_M2M_DEST_NXT_POS)
#define DMA_CH5_PERI_REQ_IDX_ADDR (uint32_t)&(DMA->CH5_CONFIG_REG)
#define DMA_CH5_PERI_REQ_IDX_POS (24) /*< bit[27:24]  */
#define DMA_CH5_PERI_REQ_IDX_MASK (0xFul << DMA_CH5_PERI_REQ_IDX_POS)

#define DMA_CH5_BUF_EN_REG_ADDR (uint32_t)&(DMA->CH5_BUF_EN_REG)
#define DMA_CH5_BUF_EN_A_ADDR (uint32_t)&(DMA->CH5_BUF_EN_REG)
#define DMA_CH5_BUF_EN_A_POS (0) /*< bit[0]      */
#define DMA_CH5_BUF_EN_A_MASK (0x1ul << DMA_CH5_BUF_EN_A_POS)
#define DMA_CH5_BUF_EN_B_ADDR (uint32_t)&(DMA->CH5_BUF_EN_REG)
#define DMA_CH5_BUF_EN_B_POS (1) /*< bit[1]      */
#define DMA_CH5_BUF_EN_B_MASK (0x1ul << DMA_CH5_BUF_EN_B_POS)

#define DMA_CH5_BUF_WORD_SIZE_REG_ADDR (uint32_t)&(DMA->CH5_BUF_WORD_SIZE_REG)
#define DMA_CH5_BUF_WORD_SIZE_ADDR (uint32_t)&(DMA->CH5_BUF_WORD_SIZE_REG)
#define DMA_CH5_BUF_WORD_SIZE_POS (0) /*< bit[11:0]   */
#define DMA_CH5_BUF_WORD_SIZE_MASK (0xFFFul << DMA_CH5_BUF_WORD_SIZE_POS)
#define DMA_CH5_BUF_END_BY_PERI_ADDR (uint32_t)&(DMA->CH5_BUF_WORD_SIZE_REG)
#define DMA_CH5_BUF_END_BY_PERI_POS (31) /*< bit[31]     */
#define DMA_CH5_BUF_END_BY_PERI_MASK (0x1ul << DMA_CH5_BUF_END_BY_PERI_POS)

#define DMA_CH5_M2M_SRC_ADDR_REG_ADDR (uint32_t)&(DMA->CH5_M2M_SRC_ADDR_REG)
#define DMA_CH5_M2M_DEST_ADDR_REG_ADDR (uint32_t)&(DMA->CH5_M2M_DEST_ADDR_REG)
#define DMA_CH5_M2M_LEN_REG_ADDR (uint32_t)&(DMA->CH5_M2M_LEN_REG)
#define DMA_CH5_M2M_LEN_ADDR (uint32_t)&(DMA->CH5_M2M_LEN_REG)
#define DMA_CH5_M2M_LEN_POS (0) /*< bit[13:0]   */
#define DMA_CH5_M2M_LEN_MASK (0x3FFFul << DMA_CH5_M2M_LEN_POS)

#define DMA_CH5_CNT_A_REG_ADDR (uint32_t)&(DMA->CH5_CNT_A_REG)
#define DMA_CH5_CNT_A_ADDR (uint32_t)&(DMA->CH5_CNT_A_REG)
#define DMA_CH5_CNT_A_POS (0) /*< bit[13:0]   */
#define DMA_CH5_CNT_A_MASK (0x3FFFul << DMA_CH5_CNT_A_POS)

#define DMA_CH5_CNT_B_REG_ADDR (uint32_t)&(DMA->CH5_CNT_B_REG)
#define DMA_CH5_CNT_B_ADDR (uint32_t)&(DMA->CH5_CNT_B_REG)
#define DMA_CH5_CNT_B_POS (0) /*< bit[13:0]   */
#define DMA_CH5_CNT_B_MASK (0x3FFFul << DMA_CH5_CNT_B_POS)

#define DMA_CH5_TRIG_REG_ADDR (uint32_t)&(DMA->CH5_TRIG_REG)
#define DMA_W1T_CH5_M2M_TRIG_ADDR (uint32_t)&(DMA->CH5_TRIG_REG)
#define DMA_W1T_CH5_M2M_TRIG_POS (0) /*< bit[0]      */
#define DMA_W1T_CH5_M2M_TRIG_MASK (0x1ul << DMA_W1T_CH5_M2M_TRIG_POS)
#define DMA_CH5_M2M_EXT_RUN_ADDR (uint32_t)&(DMA->CH5_TRIG_REG)
#define DMA_CH5_M2M_EXT_RUN_POS (8) /*< bit[8]      */
#define DMA_CH5_M2M_EXT_RUN_MASK (0x1ul << DMA_CH5_M2M_EXT_RUN_POS)
#define DMA_CH5_M2M_EXT_SRC_ADDR (uint32_t)&(DMA->CH5_TRIG_REG)
#define DMA_CH5_M2M_EXT_SRC_POS (12) /*< bit[15:12]  */
#define DMA_CH5_M2M_EXT_SRC_MASK (0xFul << DMA_CH5_M2M_EXT_SRC_POS)

#define DMA_CH5_PING_PONG_PTR_REG_ADDR (uint32_t)&(DMA->CH5_PING_PONG_PTR_REG)
#define DMA_CH5_PING_PONG_PTR_ABB_ADDR (uint32_t)&(DMA->CH5_PING_PONG_PTR_REG)
#define DMA_CH5_PING_PONG_PTR_ABB_POS (0) /*< bit[0]      */
#define DMA_CH5_PING_PONG_PTR_ABB_MASK (0x1ul << DMA_CH5_PING_PONG_PTR_ABB_POS)

#define DMA_CH5_BUF_BASE_ADDR_A_REG_ADDR (uint32_t)&(DMA->CH5_BUF_BASE_ADDR_A_REG)
#define DMA_CH5_BUF_BASE_ADDR_B_REG_ADDR (uint32_t)&(DMA->CH5_BUF_BASE_ADDR_B_REG)
#define DMA_CH6_SWRST_REG_ADDR (uint32_t)&(DMA->CH6_SWRST_REG)
#define DMA_CH6_SWRST_ADDR (uint32_t)&(DMA->CH6_SWRST_REG)
#define DMA_CH6_SWRST_POS (0) /*< bit[0]      */
#define DMA_CH6_SWRST_MASK (0x1ul << DMA_CH6_SWRST_POS)

#define DMA_CH6_CONFIG_REG_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_EN_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_EN_POS (0) /*< bit[0]      */
#define DMA_CH6_EN_MASK (0x1ul << DMA_CH6_EN_POS)
#define DMA_CH6_M2M_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_M2M_POS (1) /*< bit[1]      */
#define DMA_CH6_M2M_MASK (0x1ul << DMA_CH6_M2M_POS)
#define DMA_CH6_PING_PONG_EN_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_PING_PONG_EN_POS (2) /*< bit[2]      */
#define DMA_CH6_PING_PONG_EN_MASK (0x1ul << DMA_CH6_PING_PONG_EN_POS)
#define DMA_CH6_M2M_DAT_SIZE_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_M2M_DAT_SIZE_POS (4) /*< bit[5:4]    */
#define DMA_CH6_M2M_DAT_SIZE_MASK (0x3ul << DMA_CH6_M2M_DAT_SIZE_POS)
#define DMA_CH6_M2M_SRC_TYP_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_M2M_SRC_TYP_POS (8) /*< bit[9:8]    */
#define DMA_CH6_M2M_SRC_TYP_MASK (0x3ul << DMA_CH6_M2M_SRC_TYP_POS)
#define DMA_CH6_M2M_DEST_TYP_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_M2M_DEST_TYP_POS (10) /*< bit[11:10]  */
#define DMA_CH6_M2M_DEST_TYP_MASK (0x3ul << DMA_CH6_M2M_DEST_TYP_POS)
#define DMA_CH6_M2M_SRC_NXT_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_M2M_SRC_NXT_POS (12) /*< bit[12]     */
#define DMA_CH6_M2M_SRC_NXT_MASK (0x1ul << DMA_CH6_M2M_SRC_NXT_POS)
#define DMA_CH6_M2M_DEST_NXT_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_M2M_DEST_NXT_POS (13) /*< bit[13]     */
#define DMA_CH6_M2M_DEST_NXT_MASK (0x1ul << DMA_CH6_M2M_DEST_NXT_POS)
#define DMA_CH6_PERI_REQ_IDX_ADDR (uint32_t)&(DMA->CH6_CONFIG_REG)
#define DMA_CH6_PERI_REQ_IDX_POS (24) /*< bit[27:24]  */
#define DMA_CH6_PERI_REQ_IDX_MASK (0xFul << DMA_CH6_PERI_REQ_IDX_POS)

#define DMA_CH6_BUF_EN_REG_ADDR (uint32_t)&(DMA->CH6_BUF_EN_REG)
#define DMA_CH6_BUF_EN_A_ADDR (uint32_t)&(DMA->CH6_BUF_EN_REG)
#define DMA_CH6_BUF_EN_A_POS (0) /*< bit[0]      */
#define DMA_CH6_BUF_EN_A_MASK (0x1ul << DMA_CH6_BUF_EN_A_POS)
#define DMA_CH6_BUF_EN_B_ADDR (uint32_t)&(DMA->CH6_BUF_EN_REG)
#define DMA_CH6_BUF_EN_B_POS (1) /*< bit[1]      */
#define DMA_CH6_BUF_EN_B_MASK (0x1ul << DMA_CH6_BUF_EN_B_POS)

#define DMA_CH6_BUF_WORD_SIZE_REG_ADDR (uint32_t)&(DMA->CH6_BUF_WORD_SIZE_REG)
#define DMA_CH6_BUF_WORD_SIZE_ADDR (uint32_t)&(DMA->CH6_BUF_WORD_SIZE_REG)
#define DMA_CH6_BUF_WORD_SIZE_POS (0) /*< bit[11:0]   */
#define DMA_CH6_BUF_WORD_SIZE_MASK (0xFFFul << DMA_CH6_BUF_WORD_SIZE_POS)
#define DMA_CH6_BUF_END_BY_PERI_ADDR (uint32_t)&(DMA->CH6_BUF_WORD_SIZE_REG)
#define DMA_CH6_BUF_END_BY_PERI_POS (31) /*< bit[31]     */
#define DMA_CH6_BUF_END_BY_PERI_MASK (0x1ul << DMA_CH6_BUF_END_BY_PERI_POS)

#define DMA_CH6_M2M_SRC_ADDR_REG_ADDR (uint32_t)&(DMA->CH6_M2M_SRC_ADDR_REG)
#define DMA_CH6_M2M_DEST_ADDR_REG_ADDR (uint32_t)&(DMA->CH6_M2M_DEST_ADDR_REG)
#define DMA_CH6_M2M_LEN_REG_ADDR (uint32_t)&(DMA->CH6_M2M_LEN_REG)
#define DMA_CH6_M2M_LEN_ADDR (uint32_t)&(DMA->CH6_M2M_LEN_REG)
#define DMA_CH6_M2M_LEN_POS (0) /*< bit[13:0]   */
#define DMA_CH6_M2M_LEN_MASK (0x3FFFul << DMA_CH6_M2M_LEN_POS)

#define DMA_CH6_CNT_A_REG_ADDR (uint32_t)&(DMA->CH6_CNT_A_REG)
#define DMA_CH6_CNT_A_ADDR (uint32_t)&(DMA->CH6_CNT_A_REG)
#define DMA_CH6_CNT_A_POS (0) /*< bit[13:0]   */
#define DMA_CH6_CNT_A_MASK (0x3FFFul << DMA_CH6_CNT_A_POS)

#define DMA_CH6_CNT_B_REG_ADDR (uint32_t)&(DMA->CH6_CNT_B_REG)
#define DMA_CH6_CNT_B_ADDR (uint32_t)&(DMA->CH6_CNT_B_REG)
#define DMA_CH6_CNT_B_POS (0) /*< bit[13:0]   */
#define DMA_CH6_CNT_B_MASK (0x3FFFul << DMA_CH6_CNT_B_POS)

#define DMA_CH6_TRIG_REG_ADDR (uint32_t)&(DMA->CH6_TRIG_REG)
#define DMA_W1T_CH6_M2M_TRIG_ADDR (uint32_t)&(DMA->CH6_TRIG_REG)
#define DMA_W1T_CH6_M2M_TRIG_POS (0) /*< bit[0]      */
#define DMA_W1T_CH6_M2M_TRIG_MASK (0x1ul << DMA_W1T_CH6_M2M_TRIG_POS)
#define DMA_CH6_M2M_EXT_RUN_ADDR (uint32_t)&(DMA->CH6_TRIG_REG)
#define DMA_CH6_M2M_EXT_RUN_POS (8) /*< bit[8]      */
#define DMA_CH6_M2M_EXT_RUN_MASK (0x1ul << DMA_CH6_M2M_EXT_RUN_POS)
#define DMA_CH6_M2M_EXT_SRC_ADDR (uint32_t)&(DMA->CH6_TRIG_REG)
#define DMA_CH6_M2M_EXT_SRC_POS (12) /*< bit[15:12]  */
#define DMA_CH6_M2M_EXT_SRC_MASK (0xFul << DMA_CH6_M2M_EXT_SRC_POS)

#define DMA_CH6_PING_PONG_PTR_REG_ADDR (uint32_t)&(DMA->CH6_PING_PONG_PTR_REG)
#define DMA_CH6_PING_PONG_PTR_ABB_ADDR (uint32_t)&(DMA->CH6_PING_PONG_PTR_REG)
#define DMA_CH6_PING_PONG_PTR_ABB_POS (0) /*< bit[0]      */
#define DMA_CH6_PING_PONG_PTR_ABB_MASK (0x1ul << DMA_CH6_PING_PONG_PTR_ABB_POS)

#define DMA_CH6_BUF_BASE_ADDR_A_REG_ADDR (uint32_t)&(DMA->CH6_BUF_BASE_ADDR_A_REG)
#define DMA_CH6_BUF_BASE_ADDR_B_REG_ADDR (uint32_t)&(DMA->CH6_BUF_BASE_ADDR_B_REG)
#define DMA_CH7_SWRST_REG_ADDR (uint32_t)&(DMA->CH7_SWRST_REG)
#define DMA_CH7_SWRST_ADDR (uint32_t)&(DMA->CH7_SWRST_REG)
#define DMA_CH7_SWRST_POS (0) /*< bit[0]      */
#define DMA_CH7_SWRST_MASK (0x1ul << DMA_CH7_SWRST_POS)

#define DMA_CH7_CONFIG_REG_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_EN_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_EN_POS (0) /*< bit[0]      */
#define DMA_CH7_EN_MASK (0x1ul << DMA_CH7_EN_POS)
#define DMA_CH7_M2M_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_M2M_POS (1) /*< bit[1]      */
#define DMA_CH7_M2M_MASK (0x1ul << DMA_CH7_M2M_POS)
#define DMA_CH7_PING_PONG_EN_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_PING_PONG_EN_POS (2) /*< bit[2]      */
#define DMA_CH7_PING_PONG_EN_MASK (0x1ul << DMA_CH7_PING_PONG_EN_POS)
#define DMA_CH7_M2M_DAT_SIZE_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_M2M_DAT_SIZE_POS (4) /*< bit[5:4]    */
#define DMA_CH7_M2M_DAT_SIZE_MASK (0x3ul << DMA_CH7_M2M_DAT_SIZE_POS)
#define DMA_CH7_M2M_SRC_TYP_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_M2M_SRC_TYP_POS (8) /*< bit[9:8]    */
#define DMA_CH7_M2M_SRC_TYP_MASK (0x3ul << DMA_CH7_M2M_SRC_TYP_POS)
#define DMA_CH7_M2M_DEST_TYP_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_M2M_DEST_TYP_POS (10) /*< bit[11:10]  */
#define DMA_CH7_M2M_DEST_TYP_MASK (0x3ul << DMA_CH7_M2M_DEST_TYP_POS)
#define DMA_CH7_M2M_SRC_NXT_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_M2M_SRC_NXT_POS (12) /*< bit[12]     */
#define DMA_CH7_M2M_SRC_NXT_MASK (0x1ul << DMA_CH7_M2M_SRC_NXT_POS)
#define DMA_CH7_M2M_DEST_NXT_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_M2M_DEST_NXT_POS (13) /*< bit[13]     */
#define DMA_CH7_M2M_DEST_NXT_MASK (0x1ul << DMA_CH7_M2M_DEST_NXT_POS)
#define DMA_CH7_PERI_REQ_IDX_ADDR (uint32_t)&(DMA->CH7_CONFIG_REG)
#define DMA_CH7_PERI_REQ_IDX_POS (24) /*< bit[27:24]  */
#define DMA_CH7_PERI_REQ_IDX_MASK (0xFul << DMA_CH7_PERI_REQ_IDX_POS)

#define DMA_CH7_BUF_EN_REG_ADDR (uint32_t)&(DMA->CH7_BUF_EN_REG)
#define DMA_CH7_BUF_EN_A_ADDR (uint32_t)&(DMA->CH7_BUF_EN_REG)
#define DMA_CH7_BUF_EN_A_POS (0) /*< bit[0]      */
#define DMA_CH7_BUF_EN_A_MASK (0x1ul << DMA_CH7_BUF_EN_A_POS)
#define DMA_CH7_BUF_EN_B_ADDR (uint32_t)&(DMA->CH7_BUF_EN_REG)
#define DMA_CH7_BUF_EN_B_POS (1) /*< bit[1]      */
#define DMA_CH7_BUF_EN_B_MASK (0x1ul << DMA_CH7_BUF_EN_B_POS)

#define DMA_CH7_BUF_WORD_SIZE_REG_ADDR (uint32_t)&(DMA->CH7_BUF_WORD_SIZE_REG)
#define DMA_CH7_BUF_WORD_SIZE_ADDR (uint32_t)&(DMA->CH7_BUF_WORD_SIZE_REG)
#define DMA_CH7_BUF_WORD_SIZE_POS (0) /*< bit[11:0]   */
#define DMA_CH7_BUF_WORD_SIZE_MASK (0xFFFul << DMA_CH7_BUF_WORD_SIZE_POS)
#define DMA_CH7_BUF_END_BY_PERI_ADDR (uint32_t)&(DMA->CH7_BUF_WORD_SIZE_REG)
#define DMA_CH7_BUF_END_BY_PERI_POS (31) /*< bit[31]     */
#define DMA_CH7_BUF_END_BY_PERI_MASK (0x1ul << DMA_CH7_BUF_END_BY_PERI_POS)

#define DMA_CH7_M2M_SRC_ADDR_REG_ADDR (uint32_t)&(DMA->CH7_M2M_SRC_ADDR_REG)
#define DMA_CH7_M2M_DEST_ADDR_REG_ADDR (uint32_t)&(DMA->CH7_M2M_DEST_ADDR_REG)
#define DMA_CH7_M2M_LEN_REG_ADDR (uint32_t)&(DMA->CH7_M2M_LEN_REG)
#define DMA_CH7_M2M_LEN_ADDR (uint32_t)&(DMA->CH7_M2M_LEN_REG)
#define DMA_CH7_M2M_LEN_POS (0) /*< bit[13:0]   */
#define DMA_CH7_M2M_LEN_MASK (0x3FFFul << DMA_CH7_M2M_LEN_POS)

#define DMA_CH7_CNT_A_REG_ADDR (uint32_t)&(DMA->CH7_CNT_A_REG)
#define DMA_CH7_CNT_A_ADDR (uint32_t)&(DMA->CH7_CNT_A_REG)
#define DMA_CH7_CNT_A_POS (0) /*< bit[13:0]   */
#define DMA_CH7_CNT_A_MASK (0x3FFFul << DMA_CH7_CNT_A_POS)

#define DMA_CH7_CNT_B_REG_ADDR (uint32_t)&(DMA->CH7_CNT_B_REG)
#define DMA_CH7_CNT_B_ADDR (uint32_t)&(DMA->CH7_CNT_B_REG)
#define DMA_CH7_CNT_B_POS (0) /*< bit[13:0]   */
#define DMA_CH7_CNT_B_MASK (0x3FFFul << DMA_CH7_CNT_B_POS)

#define DMA_CH7_TRIG_REG_ADDR (uint32_t)&(DMA->CH7_TRIG_REG)
#define DMA_W1T_CH7_M2M_TRIG_ADDR (uint32_t)&(DMA->CH7_TRIG_REG)
#define DMA_W1T_CH7_M2M_TRIG_POS (0) /*< bit[0]      */
#define DMA_W1T_CH7_M2M_TRIG_MASK (0x1ul << DMA_W1T_CH7_M2M_TRIG_POS)
#define DMA_CH7_M2M_EXT_RUN_ADDR (uint32_t)&(DMA->CH7_TRIG_REG)
#define DMA_CH7_M2M_EXT_RUN_POS (8) /*< bit[8]      */
#define DMA_CH7_M2M_EXT_RUN_MASK (0x1ul << DMA_CH7_M2M_EXT_RUN_POS)
#define DMA_CH7_M2M_EXT_SRC_ADDR (uint32_t)&(DMA->CH7_TRIG_REG)
#define DMA_CH7_M2M_EXT_SRC_POS (12) /*< bit[15:12]  */
#define DMA_CH7_M2M_EXT_SRC_MASK (0xFul << DMA_CH7_M2M_EXT_SRC_POS)

#define DMA_CH7_PING_PONG_PTR_REG_ADDR (uint32_t)&(DMA->CH7_PING_PONG_PTR_REG)
#define DMA_CH7_PING_PONG_PTR_ABB_ADDR (uint32_t)&(DMA->CH7_PING_PONG_PTR_REG)
#define DMA_CH7_PING_PONG_PTR_ABB_POS (0) /*< bit[0]      */
#define DMA_CH7_PING_PONG_PTR_ABB_MASK (0x1ul << DMA_CH7_PING_PONG_PTR_ABB_POS)

#define DMA_CH7_BUF_BASE_ADDR_A_REG_ADDR (uint32_t)&(DMA->CH7_BUF_BASE_ADDR_A_REG)
#define DMA_CH7_BUF_BASE_ADDR_B_REG_ADDR (uint32_t)&(DMA->CH7_BUF_BASE_ADDR_B_REG)
#define DMA_INT_ST_CH_CMPLT_A_REG_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH0_CMPLT_A_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH0_CMPLT_A_POS (0) /*< bit[0]      */
#define DMA_INT_ST_CH0_CMPLT_A_MASK (0x1ul << DMA_INT_ST_CH0_CMPLT_A_POS)
#define DMA_INT_ST_CH1_CMPLT_A_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH1_CMPLT_A_POS (1) /*< bit[1]      */
#define DMA_INT_ST_CH1_CMPLT_A_MASK (0x1ul << DMA_INT_ST_CH1_CMPLT_A_POS)
#define DMA_INT_ST_CH2_CMPLT_A_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH2_CMPLT_A_POS (2) /*< bit[2]      */
#define DMA_INT_ST_CH2_CMPLT_A_MASK (0x1ul << DMA_INT_ST_CH2_CMPLT_A_POS)
#define DMA_INT_ST_CH3_CMPLT_A_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH3_CMPLT_A_POS (3) /*< bit[3]      */
#define DMA_INT_ST_CH3_CMPLT_A_MASK (0x1ul << DMA_INT_ST_CH3_CMPLT_A_POS)
#define DMA_INT_ST_CH4_CMPLT_A_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH4_CMPLT_A_POS (4) /*< bit[4]      */
#define DMA_INT_ST_CH4_CMPLT_A_MASK (0x1ul << DMA_INT_ST_CH4_CMPLT_A_POS)
#define DMA_INT_ST_CH5_CMPLT_A_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH5_CMPLT_A_POS (5) /*< bit[5]      */
#define DMA_INT_ST_CH5_CMPLT_A_MASK (0x1ul << DMA_INT_ST_CH5_CMPLT_A_POS)
#define DMA_INT_ST_CH6_CMPLT_A_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH6_CMPLT_A_POS (6) /*< bit[6]      */
#define DMA_INT_ST_CH6_CMPLT_A_MASK (0x1ul << DMA_INT_ST_CH6_CMPLT_A_POS)
#define DMA_INT_ST_CH7_CMPLT_A_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_ST_CH7_CMPLT_A_POS (7) /*< bit[7]      */
#define DMA_INT_ST_CH7_CMPLT_A_MASK (0x1ul << DMA_INT_ST_CH7_CMPLT_A_POS)

#define DMA_INT_ST_CH_CMPLT_B_REG_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH0_CMPLT_B_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH0_CMPLT_B_POS (0) /*< bit[0]      */
#define DMA_INT_ST_CH0_CMPLT_B_MASK (0x1ul << DMA_INT_ST_CH0_CMPLT_B_POS)
#define DMA_INT_ST_CH1_CMPLT_B_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH1_CMPLT_B_POS (1) /*< bit[1]      */
#define DMA_INT_ST_CH1_CMPLT_B_MASK (0x1ul << DMA_INT_ST_CH1_CMPLT_B_POS)
#define DMA_INT_ST_CH2_CMPLT_B_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH2_CMPLT_B_POS (2) /*< bit[2]      */
#define DMA_INT_ST_CH2_CMPLT_B_MASK (0x1ul << DMA_INT_ST_CH2_CMPLT_B_POS)
#define DMA_INT_ST_CH3_CMPLT_B_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH3_CMPLT_B_POS (3) /*< bit[3]      */
#define DMA_INT_ST_CH3_CMPLT_B_MASK (0x1ul << DMA_INT_ST_CH3_CMPLT_B_POS)
#define DMA_INT_ST_CH4_CMPLT_B_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH4_CMPLT_B_POS (4) /*< bit[4]      */
#define DMA_INT_ST_CH4_CMPLT_B_MASK (0x1ul << DMA_INT_ST_CH4_CMPLT_B_POS)
#define DMA_INT_ST_CH5_CMPLT_B_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH5_CMPLT_B_POS (5) /*< bit[5]      */
#define DMA_INT_ST_CH5_CMPLT_B_MASK (0x1ul << DMA_INT_ST_CH5_CMPLT_B_POS)
#define DMA_INT_ST_CH6_CMPLT_B_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH6_CMPLT_B_POS (6) /*< bit[6]      */
#define DMA_INT_ST_CH6_CMPLT_B_MASK (0x1ul << DMA_INT_ST_CH6_CMPLT_B_POS)
#define DMA_INT_ST_CH7_CMPLT_B_ADDR (uint32_t)&(DMA->INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_ST_CH7_CMPLT_B_POS (7) /*< bit[7]      */
#define DMA_INT_ST_CH7_CMPLT_B_MASK (0x1ul << DMA_INT_ST_CH7_CMPLT_B_POS)

#define DMA_INT_ST_CH_ERR_EX_SIZE_A_REG_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH0_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH0_ERR_EX_SIZE_A_POS (0) /*< bit[0]      */
#define DMA_INT_ST_CH0_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_ST_CH0_ERR_EX_SIZE_A_POS)
#define DMA_INT_ST_CH1_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH1_ERR_EX_SIZE_A_POS (1) /*< bit[1]      */
#define DMA_INT_ST_CH1_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_ST_CH1_ERR_EX_SIZE_A_POS)
#define DMA_INT_ST_CH2_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH2_ERR_EX_SIZE_A_POS (2) /*< bit[2]      */
#define DMA_INT_ST_CH2_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_ST_CH2_ERR_EX_SIZE_A_POS)
#define DMA_INT_ST_CH3_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH3_ERR_EX_SIZE_A_POS (3) /*< bit[3]      */
#define DMA_INT_ST_CH3_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_ST_CH3_ERR_EX_SIZE_A_POS)
#define DMA_INT_ST_CH4_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH4_ERR_EX_SIZE_A_POS (4) /*< bit[4]      */
#define DMA_INT_ST_CH4_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_ST_CH4_ERR_EX_SIZE_A_POS)
#define DMA_INT_ST_CH5_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH5_ERR_EX_SIZE_A_POS (5) /*< bit[5]      */
#define DMA_INT_ST_CH5_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_ST_CH5_ERR_EX_SIZE_A_POS)
#define DMA_INT_ST_CH6_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH6_ERR_EX_SIZE_A_POS (6) /*< bit[6]      */
#define DMA_INT_ST_CH6_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_ST_CH6_ERR_EX_SIZE_A_POS)
#define DMA_INT_ST_CH7_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_ST_CH7_ERR_EX_SIZE_A_POS (7) /*< bit[7]      */
#define DMA_INT_ST_CH7_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_ST_CH7_ERR_EX_SIZE_A_POS)

#define DMA_INT_ST_CH_ERR_EX_SIZE_B_REG_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH0_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH0_ERR_EX_SIZE_B_POS (0) /*< bit[0]      */
#define DMA_INT_ST_CH0_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_ST_CH0_ERR_EX_SIZE_B_POS)
#define DMA_INT_ST_CH1_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH1_ERR_EX_SIZE_B_POS (1) /*< bit[1]      */
#define DMA_INT_ST_CH1_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_ST_CH1_ERR_EX_SIZE_B_POS)
#define DMA_INT_ST_CH2_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH2_ERR_EX_SIZE_B_POS (2) /*< bit[2]      */
#define DMA_INT_ST_CH2_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_ST_CH2_ERR_EX_SIZE_B_POS)
#define DMA_INT_ST_CH3_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH3_ERR_EX_SIZE_B_POS (3) /*< bit[3]      */
#define DMA_INT_ST_CH3_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_ST_CH3_ERR_EX_SIZE_B_POS)
#define DMA_INT_ST_CH4_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH4_ERR_EX_SIZE_B_POS (4) /*< bit[4]      */
#define DMA_INT_ST_CH4_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_ST_CH4_ERR_EX_SIZE_B_POS)
#define DMA_INT_ST_CH5_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH5_ERR_EX_SIZE_B_POS (5) /*< bit[5]      */
#define DMA_INT_ST_CH5_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_ST_CH5_ERR_EX_SIZE_B_POS)
#define DMA_INT_ST_CH6_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH6_ERR_EX_SIZE_B_POS (6) /*< bit[6]      */
#define DMA_INT_ST_CH6_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_ST_CH6_ERR_EX_SIZE_B_POS)
#define DMA_INT_ST_CH7_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_ST_CH7_ERR_EX_SIZE_B_POS (7) /*< bit[7]      */
#define DMA_INT_ST_CH7_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_ST_CH7_ERR_EX_SIZE_B_POS)

#define DMA_INT_ST_CH_ERR_OVRN_A_REG_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH0_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH0_ERR_OVRN_A_POS (0) /*< bit[0]      */
#define DMA_INT_ST_CH0_ERR_OVRN_A_MASK (0x1ul << DMA_INT_ST_CH0_ERR_OVRN_A_POS)
#define DMA_INT_ST_CH1_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH1_ERR_OVRN_A_POS (1) /*< bit[1]      */
#define DMA_INT_ST_CH1_ERR_OVRN_A_MASK (0x1ul << DMA_INT_ST_CH1_ERR_OVRN_A_POS)
#define DMA_INT_ST_CH2_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH2_ERR_OVRN_A_POS (2) /*< bit[2]      */
#define DMA_INT_ST_CH2_ERR_OVRN_A_MASK (0x1ul << DMA_INT_ST_CH2_ERR_OVRN_A_POS)
#define DMA_INT_ST_CH3_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH3_ERR_OVRN_A_POS (3) /*< bit[3]      */
#define DMA_INT_ST_CH3_ERR_OVRN_A_MASK (0x1ul << DMA_INT_ST_CH3_ERR_OVRN_A_POS)
#define DMA_INT_ST_CH4_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH4_ERR_OVRN_A_POS (4) /*< bit[4]      */
#define DMA_INT_ST_CH4_ERR_OVRN_A_MASK (0x1ul << DMA_INT_ST_CH4_ERR_OVRN_A_POS)
#define DMA_INT_ST_CH5_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH5_ERR_OVRN_A_POS (5) /*< bit[5]      */
#define DMA_INT_ST_CH5_ERR_OVRN_A_MASK (0x1ul << DMA_INT_ST_CH5_ERR_OVRN_A_POS)
#define DMA_INT_ST_CH6_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH6_ERR_OVRN_A_POS (6) /*< bit[6]      */
#define DMA_INT_ST_CH6_ERR_OVRN_A_MASK (0x1ul << DMA_INT_ST_CH6_ERR_OVRN_A_POS)
#define DMA_INT_ST_CH7_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_ST_CH7_ERR_OVRN_A_POS (7) /*< bit[7]      */
#define DMA_INT_ST_CH7_ERR_OVRN_A_MASK (0x1ul << DMA_INT_ST_CH7_ERR_OVRN_A_POS)

#define DMA_INT_ST_CH_ERR_OVRN_B_REG_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH0_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH0_ERR_OVRN_B_POS (0) /*< bit[0]      */
#define DMA_INT_ST_CH0_ERR_OVRN_B_MASK (0x1ul << DMA_INT_ST_CH0_ERR_OVRN_B_POS)
#define DMA_INT_ST_CH1_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH1_ERR_OVRN_B_POS (1) /*< bit[1]      */
#define DMA_INT_ST_CH1_ERR_OVRN_B_MASK (0x1ul << DMA_INT_ST_CH1_ERR_OVRN_B_POS)
#define DMA_INT_ST_CH2_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH2_ERR_OVRN_B_POS (2) /*< bit[2]      */
#define DMA_INT_ST_CH2_ERR_OVRN_B_MASK (0x1ul << DMA_INT_ST_CH2_ERR_OVRN_B_POS)
#define DMA_INT_ST_CH3_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH3_ERR_OVRN_B_POS (3) /*< bit[3]      */
#define DMA_INT_ST_CH3_ERR_OVRN_B_MASK (0x1ul << DMA_INT_ST_CH3_ERR_OVRN_B_POS)
#define DMA_INT_ST_CH4_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH4_ERR_OVRN_B_POS (4) /*< bit[4]      */
#define DMA_INT_ST_CH4_ERR_OVRN_B_MASK (0x1ul << DMA_INT_ST_CH4_ERR_OVRN_B_POS)
#define DMA_INT_ST_CH5_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH5_ERR_OVRN_B_POS (5) /*< bit[5]      */
#define DMA_INT_ST_CH5_ERR_OVRN_B_MASK (0x1ul << DMA_INT_ST_CH5_ERR_OVRN_B_POS)
#define DMA_INT_ST_CH6_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH6_ERR_OVRN_B_POS (6) /*< bit[6]      */
#define DMA_INT_ST_CH6_ERR_OVRN_B_MASK (0x1ul << DMA_INT_ST_CH6_ERR_OVRN_B_POS)
#define DMA_INT_ST_CH7_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_ST_CH7_ERR_OVRN_B_POS (7) /*< bit[7]      */
#define DMA_INT_ST_CH7_ERR_OVRN_B_MASK (0x1ul << DMA_INT_ST_CH7_ERR_OVRN_B_POS)

#define DMA_INT_ST_CH_M2M_CMPLT_REG_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH0_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH0_M2M_CMPLT_POS (0) /*< bit[0]      */
#define DMA_INT_ST_CH0_M2M_CMPLT_MASK (0x1ul << DMA_INT_ST_CH0_M2M_CMPLT_POS)
#define DMA_INT_ST_CH1_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH1_M2M_CMPLT_POS (1) /*< bit[1]      */
#define DMA_INT_ST_CH1_M2M_CMPLT_MASK (0x1ul << DMA_INT_ST_CH1_M2M_CMPLT_POS)
#define DMA_INT_ST_CH2_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH2_M2M_CMPLT_POS (2) /*< bit[2]      */
#define DMA_INT_ST_CH2_M2M_CMPLT_MASK (0x1ul << DMA_INT_ST_CH2_M2M_CMPLT_POS)
#define DMA_INT_ST_CH3_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH3_M2M_CMPLT_POS (3) /*< bit[3]      */
#define DMA_INT_ST_CH3_M2M_CMPLT_MASK (0x1ul << DMA_INT_ST_CH3_M2M_CMPLT_POS)
#define DMA_INT_ST_CH4_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH4_M2M_CMPLT_POS (4) /*< bit[4]      */
#define DMA_INT_ST_CH4_M2M_CMPLT_MASK (0x1ul << DMA_INT_ST_CH4_M2M_CMPLT_POS)
#define DMA_INT_ST_CH5_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH5_M2M_CMPLT_POS (5) /*< bit[5]      */
#define DMA_INT_ST_CH5_M2M_CMPLT_MASK (0x1ul << DMA_INT_ST_CH5_M2M_CMPLT_POS)
#define DMA_INT_ST_CH6_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH6_M2M_CMPLT_POS (6) /*< bit[6]      */
#define DMA_INT_ST_CH6_M2M_CMPLT_MASK (0x1ul << DMA_INT_ST_CH6_M2M_CMPLT_POS)
#define DMA_INT_ST_CH7_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_ST_CH7_M2M_CMPLT_POS (7) /*< bit[7]      */
#define DMA_INT_ST_CH7_M2M_CMPLT_MASK (0x1ul << DMA_INT_ST_CH7_M2M_CMPLT_POS)

#define DMA_INT_EN_CH_CMPLT_A_REG_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH0_CMPLT_A_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH0_CMPLT_A_POS (0) /*< bit[0]      */
#define DMA_INT_EN_CH0_CMPLT_A_MASK (0x1ul << DMA_INT_EN_CH0_CMPLT_A_POS)
#define DMA_INT_EN_CH1_CMPLT_A_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH1_CMPLT_A_POS (1) /*< bit[1]      */
#define DMA_INT_EN_CH1_CMPLT_A_MASK (0x1ul << DMA_INT_EN_CH1_CMPLT_A_POS)
#define DMA_INT_EN_CH2_CMPLT_A_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH2_CMPLT_A_POS (2) /*< bit[2]      */
#define DMA_INT_EN_CH2_CMPLT_A_MASK (0x1ul << DMA_INT_EN_CH2_CMPLT_A_POS)
#define DMA_INT_EN_CH3_CMPLT_A_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH3_CMPLT_A_POS (3) /*< bit[3]      */
#define DMA_INT_EN_CH3_CMPLT_A_MASK (0x1ul << DMA_INT_EN_CH3_CMPLT_A_POS)
#define DMA_INT_EN_CH4_CMPLT_A_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH4_CMPLT_A_POS (4) /*< bit[4]      */
#define DMA_INT_EN_CH4_CMPLT_A_MASK (0x1ul << DMA_INT_EN_CH4_CMPLT_A_POS)
#define DMA_INT_EN_CH5_CMPLT_A_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH5_CMPLT_A_POS (5) /*< bit[5]      */
#define DMA_INT_EN_CH5_CMPLT_A_MASK (0x1ul << DMA_INT_EN_CH5_CMPLT_A_POS)
#define DMA_INT_EN_CH6_CMPLT_A_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH6_CMPLT_A_POS (6) /*< bit[6]      */
#define DMA_INT_EN_CH6_CMPLT_A_MASK (0x1ul << DMA_INT_EN_CH6_CMPLT_A_POS)
#define DMA_INT_EN_CH7_CMPLT_A_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_A_REG)
#define DMA_INT_EN_CH7_CMPLT_A_POS (7) /*< bit[7]      */
#define DMA_INT_EN_CH7_CMPLT_A_MASK (0x1ul << DMA_INT_EN_CH7_CMPLT_A_POS)

#define DMA_INT_EN_CH_CMPLT_B_REG_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH0_CMPLT_B_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH0_CMPLT_B_POS (0) /*< bit[0]      */
#define DMA_INT_EN_CH0_CMPLT_B_MASK (0x1ul << DMA_INT_EN_CH0_CMPLT_B_POS)
#define DMA_INT_EN_CH1_CMPLT_B_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH1_CMPLT_B_POS (1) /*< bit[1]      */
#define DMA_INT_EN_CH1_CMPLT_B_MASK (0x1ul << DMA_INT_EN_CH1_CMPLT_B_POS)
#define DMA_INT_EN_CH2_CMPLT_B_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH2_CMPLT_B_POS (2) /*< bit[2]      */
#define DMA_INT_EN_CH2_CMPLT_B_MASK (0x1ul << DMA_INT_EN_CH2_CMPLT_B_POS)
#define DMA_INT_EN_CH3_CMPLT_B_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH3_CMPLT_B_POS (3) /*< bit[3]      */
#define DMA_INT_EN_CH3_CMPLT_B_MASK (0x1ul << DMA_INT_EN_CH3_CMPLT_B_POS)
#define DMA_INT_EN_CH4_CMPLT_B_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH4_CMPLT_B_POS (4) /*< bit[4]      */
#define DMA_INT_EN_CH4_CMPLT_B_MASK (0x1ul << DMA_INT_EN_CH4_CMPLT_B_POS)
#define DMA_INT_EN_CH5_CMPLT_B_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH5_CMPLT_B_POS (5) /*< bit[5]      */
#define DMA_INT_EN_CH5_CMPLT_B_MASK (0x1ul << DMA_INT_EN_CH5_CMPLT_B_POS)
#define DMA_INT_EN_CH6_CMPLT_B_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH6_CMPLT_B_POS (6) /*< bit[6]      */
#define DMA_INT_EN_CH6_CMPLT_B_MASK (0x1ul << DMA_INT_EN_CH6_CMPLT_B_POS)
#define DMA_INT_EN_CH7_CMPLT_B_ADDR (uint32_t)&(DMA->INT_EN_CH_CMPLT_B_REG)
#define DMA_INT_EN_CH7_CMPLT_B_POS (7) /*< bit[7]      */
#define DMA_INT_EN_CH7_CMPLT_B_MASK (0x1ul << DMA_INT_EN_CH7_CMPLT_B_POS)

#define DMA_INT_EN_CH_ERR_EX_SIZE_A_REG_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH0_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH0_ERR_EX_SIZE_A_POS (0) /*< bit[0]      */
#define DMA_INT_EN_CH0_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_EN_CH0_ERR_EX_SIZE_A_POS)
#define DMA_INT_EN_CH1_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH1_ERR_EX_SIZE_A_POS (1) /*< bit[1]      */
#define DMA_INT_EN_CH1_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_EN_CH1_ERR_EX_SIZE_A_POS)
#define DMA_INT_EN_CH2_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH2_ERR_EX_SIZE_A_POS (2) /*< bit[2]      */
#define DMA_INT_EN_CH2_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_EN_CH2_ERR_EX_SIZE_A_POS)
#define DMA_INT_EN_CH3_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH3_ERR_EX_SIZE_A_POS (3) /*< bit[3]      */
#define DMA_INT_EN_CH3_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_EN_CH3_ERR_EX_SIZE_A_POS)
#define DMA_INT_EN_CH4_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH4_ERR_EX_SIZE_A_POS (4) /*< bit[4]      */
#define DMA_INT_EN_CH4_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_EN_CH4_ERR_EX_SIZE_A_POS)
#define DMA_INT_EN_CH5_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH5_ERR_EX_SIZE_A_POS (5) /*< bit[5]      */
#define DMA_INT_EN_CH5_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_EN_CH5_ERR_EX_SIZE_A_POS)
#define DMA_INT_EN_CH6_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH6_ERR_EX_SIZE_A_POS (6) /*< bit[6]      */
#define DMA_INT_EN_CH6_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_EN_CH6_ERR_EX_SIZE_A_POS)
#define DMA_INT_EN_CH7_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_EN_CH7_ERR_EX_SIZE_A_POS (7) /*< bit[7]      */
#define DMA_INT_EN_CH7_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_EN_CH7_ERR_EX_SIZE_A_POS)

#define DMA_INT_EN_CH_ERR_EX_SIZE_B_REG_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH0_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH0_ERR_EX_SIZE_B_POS (0) /*< bit[0]      */
#define DMA_INT_EN_CH0_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_EN_CH0_ERR_EX_SIZE_B_POS)
#define DMA_INT_EN_CH1_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH1_ERR_EX_SIZE_B_POS (1) /*< bit[1]      */
#define DMA_INT_EN_CH1_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_EN_CH1_ERR_EX_SIZE_B_POS)
#define DMA_INT_EN_CH2_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH2_ERR_EX_SIZE_B_POS (2) /*< bit[2]      */
#define DMA_INT_EN_CH2_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_EN_CH2_ERR_EX_SIZE_B_POS)
#define DMA_INT_EN_CH3_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH3_ERR_EX_SIZE_B_POS (3) /*< bit[3]      */
#define DMA_INT_EN_CH3_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_EN_CH3_ERR_EX_SIZE_B_POS)
#define DMA_INT_EN_CH4_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH4_ERR_EX_SIZE_B_POS (4) /*< bit[4]      */
#define DMA_INT_EN_CH4_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_EN_CH4_ERR_EX_SIZE_B_POS)
#define DMA_INT_EN_CH5_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH5_ERR_EX_SIZE_B_POS (5) /*< bit[5]      */
#define DMA_INT_EN_CH5_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_EN_CH5_ERR_EX_SIZE_B_POS)
#define DMA_INT_EN_CH6_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH6_ERR_EX_SIZE_B_POS (6) /*< bit[6]      */
#define DMA_INT_EN_CH6_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_EN_CH6_ERR_EX_SIZE_B_POS)
#define DMA_INT_EN_CH7_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_EN_CH7_ERR_EX_SIZE_B_POS (7) /*< bit[7]      */
#define DMA_INT_EN_CH7_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_EN_CH7_ERR_EX_SIZE_B_POS)

#define DMA_INT_EN_CH_ERR_OVRN_A_REG_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH0_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH0_ERR_OVRN_A_POS (0) /*< bit[0]      */
#define DMA_INT_EN_CH0_ERR_OVRN_A_MASK (0x1ul << DMA_INT_EN_CH0_ERR_OVRN_A_POS)
#define DMA_INT_EN_CH1_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH1_ERR_OVRN_A_POS (1) /*< bit[1]      */
#define DMA_INT_EN_CH1_ERR_OVRN_A_MASK (0x1ul << DMA_INT_EN_CH1_ERR_OVRN_A_POS)
#define DMA_INT_EN_CH2_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH2_ERR_OVRN_A_POS (2) /*< bit[2]      */
#define DMA_INT_EN_CH2_ERR_OVRN_A_MASK (0x1ul << DMA_INT_EN_CH2_ERR_OVRN_A_POS)
#define DMA_INT_EN_CH3_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH3_ERR_OVRN_A_POS (3) /*< bit[3]      */
#define DMA_INT_EN_CH3_ERR_OVRN_A_MASK (0x1ul << DMA_INT_EN_CH3_ERR_OVRN_A_POS)
#define DMA_INT_EN_CH4_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH4_ERR_OVRN_A_POS (4) /*< bit[4]      */
#define DMA_INT_EN_CH4_ERR_OVRN_A_MASK (0x1ul << DMA_INT_EN_CH4_ERR_OVRN_A_POS)
#define DMA_INT_EN_CH5_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH5_ERR_OVRN_A_POS (5) /*< bit[5]      */
#define DMA_INT_EN_CH5_ERR_OVRN_A_MASK (0x1ul << DMA_INT_EN_CH5_ERR_OVRN_A_POS)
#define DMA_INT_EN_CH6_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH6_ERR_OVRN_A_POS (6) /*< bit[6]      */
#define DMA_INT_EN_CH6_ERR_OVRN_A_MASK (0x1ul << DMA_INT_EN_CH6_ERR_OVRN_A_POS)
#define DMA_INT_EN_CH7_ERR_OVRN_A_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_A_REG)
#define DMA_INT_EN_CH7_ERR_OVRN_A_POS (7) /*< bit[7]      */
#define DMA_INT_EN_CH7_ERR_OVRN_A_MASK (0x1ul << DMA_INT_EN_CH7_ERR_OVRN_A_POS)

#define DMA_INT_EN_CH_ERR_OVRN_B_REG_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH0_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH0_ERR_OVRN_B_POS (0) /*< bit[0]      */
#define DMA_INT_EN_CH0_ERR_OVRN_B_MASK (0x1ul << DMA_INT_EN_CH0_ERR_OVRN_B_POS)
#define DMA_INT_EN_CH1_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH1_ERR_OVRN_B_POS (1) /*< bit[1]      */
#define DMA_INT_EN_CH1_ERR_OVRN_B_MASK (0x1ul << DMA_INT_EN_CH1_ERR_OVRN_B_POS)
#define DMA_INT_EN_CH2_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH2_ERR_OVRN_B_POS (2) /*< bit[2]      */
#define DMA_INT_EN_CH2_ERR_OVRN_B_MASK (0x1ul << DMA_INT_EN_CH2_ERR_OVRN_B_POS)
#define DMA_INT_EN_CH3_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH3_ERR_OVRN_B_POS (3) /*< bit[3]      */
#define DMA_INT_EN_CH3_ERR_OVRN_B_MASK (0x1ul << DMA_INT_EN_CH3_ERR_OVRN_B_POS)
#define DMA_INT_EN_CH4_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH4_ERR_OVRN_B_POS (4) /*< bit[4]      */
#define DMA_INT_EN_CH4_ERR_OVRN_B_MASK (0x1ul << DMA_INT_EN_CH4_ERR_OVRN_B_POS)
#define DMA_INT_EN_CH5_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH5_ERR_OVRN_B_POS (5) /*< bit[5]      */
#define DMA_INT_EN_CH5_ERR_OVRN_B_MASK (0x1ul << DMA_INT_EN_CH5_ERR_OVRN_B_POS)
#define DMA_INT_EN_CH6_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH6_ERR_OVRN_B_POS (6) /*< bit[6]      */
#define DMA_INT_EN_CH6_ERR_OVRN_B_MASK (0x1ul << DMA_INT_EN_CH6_ERR_OVRN_B_POS)
#define DMA_INT_EN_CH7_ERR_OVRN_B_ADDR (uint32_t)&(DMA->INT_EN_CH_ERR_OVRN_B_REG)
#define DMA_INT_EN_CH7_ERR_OVRN_B_POS (7) /*< bit[7]      */
#define DMA_INT_EN_CH7_ERR_OVRN_B_MASK (0x1ul << DMA_INT_EN_CH7_ERR_OVRN_B_POS)

#define DMA_INT_EN_CH_M2M_CMPLT_REG_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH0_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH0_M2M_CMPLT_POS (0) /*< bit[0]      */
#define DMA_INT_EN_CH0_M2M_CMPLT_MASK (0x1ul << DMA_INT_EN_CH0_M2M_CMPLT_POS)
#define DMA_INT_EN_CH1_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH1_M2M_CMPLT_POS (1) /*< bit[1]      */
#define DMA_INT_EN_CH1_M2M_CMPLT_MASK (0x1ul << DMA_INT_EN_CH1_M2M_CMPLT_POS)
#define DMA_INT_EN_CH2_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH2_M2M_CMPLT_POS (2) /*< bit[2]      */
#define DMA_INT_EN_CH2_M2M_CMPLT_MASK (0x1ul << DMA_INT_EN_CH2_M2M_CMPLT_POS)
#define DMA_INT_EN_CH3_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH3_M2M_CMPLT_POS (3) /*< bit[3]      */
#define DMA_INT_EN_CH3_M2M_CMPLT_MASK (0x1ul << DMA_INT_EN_CH3_M2M_CMPLT_POS)
#define DMA_INT_EN_CH4_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH4_M2M_CMPLT_POS (4) /*< bit[4]      */
#define DMA_INT_EN_CH4_M2M_CMPLT_MASK (0x1ul << DMA_INT_EN_CH4_M2M_CMPLT_POS)
#define DMA_INT_EN_CH5_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH5_M2M_CMPLT_POS (5) /*< bit[5]      */
#define DMA_INT_EN_CH5_M2M_CMPLT_MASK (0x1ul << DMA_INT_EN_CH5_M2M_CMPLT_POS)
#define DMA_INT_EN_CH6_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH6_M2M_CMPLT_POS (6) /*< bit[6]      */
#define DMA_INT_EN_CH6_M2M_CMPLT_MASK (0x1ul << DMA_INT_EN_CH6_M2M_CMPLT_POS)
#define DMA_INT_EN_CH7_M2M_CMPLT_ADDR (uint32_t)&(DMA->INT_EN_CH_M2M_CMPLT_REG)
#define DMA_INT_EN_CH7_M2M_CMPLT_POS (7) /*< bit[7]      */
#define DMA_INT_EN_CH7_M2M_CMPLT_MASK (0x1ul << DMA_INT_EN_CH7_M2M_CMPLT_POS)

#define DMA_MASKED_INT_ST_CH_CMPLT_A_REG_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH0_CMPLT_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH0_CMPLT_A_POS (0) /*< bit[0]      */
#define DMA_INT_CH0_CMPLT_A_MASK (0x1ul << DMA_INT_CH0_CMPLT_A_POS)
#define DMA_INT_CH1_CMPLT_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH1_CMPLT_A_POS (1) /*< bit[1]      */
#define DMA_INT_CH1_CMPLT_A_MASK (0x1ul << DMA_INT_CH1_CMPLT_A_POS)
#define DMA_INT_CH2_CMPLT_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH2_CMPLT_A_POS (2) /*< bit[2]      */
#define DMA_INT_CH2_CMPLT_A_MASK (0x1ul << DMA_INT_CH2_CMPLT_A_POS)
#define DMA_INT_CH3_CMPLT_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH3_CMPLT_A_POS (3) /*< bit[3]      */
#define DMA_INT_CH3_CMPLT_A_MASK (0x1ul << DMA_INT_CH3_CMPLT_A_POS)
#define DMA_INT_CH4_CMPLT_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH4_CMPLT_A_POS (4) /*< bit[4]      */
#define DMA_INT_CH4_CMPLT_A_MASK (0x1ul << DMA_INT_CH4_CMPLT_A_POS)
#define DMA_INT_CH5_CMPLT_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH5_CMPLT_A_POS (5) /*< bit[5]      */
#define DMA_INT_CH5_CMPLT_A_MASK (0x1ul << DMA_INT_CH5_CMPLT_A_POS)
#define DMA_INT_CH6_CMPLT_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH6_CMPLT_A_POS (6) /*< bit[6]      */
#define DMA_INT_CH6_CMPLT_A_MASK (0x1ul << DMA_INT_CH6_CMPLT_A_POS)
#define DMA_INT_CH7_CMPLT_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_A_REG)
#define DMA_INT_CH7_CMPLT_A_POS (7) /*< bit[7]      */
#define DMA_INT_CH7_CMPLT_A_MASK (0x1ul << DMA_INT_CH7_CMPLT_A_POS)

#define DMA_MASKED_INT_ST_CH_CMPLT_B_REG_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH0_CMPLT_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH0_CMPLT_B_POS (0) /*< bit[0]      */
#define DMA_INT_CH0_CMPLT_B_MASK (0x1ul << DMA_INT_CH0_CMPLT_B_POS)
#define DMA_INT_CH1_CMPLT_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH1_CMPLT_B_POS (1) /*< bit[1]      */
#define DMA_INT_CH1_CMPLT_B_MASK (0x1ul << DMA_INT_CH1_CMPLT_B_POS)
#define DMA_INT_CH2_CMPLT_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH2_CMPLT_B_POS (2) /*< bit[2]      */
#define DMA_INT_CH2_CMPLT_B_MASK (0x1ul << DMA_INT_CH2_CMPLT_B_POS)
#define DMA_INT_CH3_CMPLT_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH3_CMPLT_B_POS (3) /*< bit[3]      */
#define DMA_INT_CH3_CMPLT_B_MASK (0x1ul << DMA_INT_CH3_CMPLT_B_POS)
#define DMA_INT_CH4_CMPLT_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH4_CMPLT_B_POS (4) /*< bit[4]      */
#define DMA_INT_CH4_CMPLT_B_MASK (0x1ul << DMA_INT_CH4_CMPLT_B_POS)
#define DMA_INT_CH5_CMPLT_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH5_CMPLT_B_POS (5) /*< bit[5]      */
#define DMA_INT_CH5_CMPLT_B_MASK (0x1ul << DMA_INT_CH5_CMPLT_B_POS)
#define DMA_INT_CH6_CMPLT_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH6_CMPLT_B_POS (6) /*< bit[6]      */
#define DMA_INT_CH6_CMPLT_B_MASK (0x1ul << DMA_INT_CH6_CMPLT_B_POS)
#define DMA_INT_CH7_CMPLT_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_CMPLT_B_REG)
#define DMA_INT_CH7_CMPLT_B_POS (7) /*< bit[7]      */
#define DMA_INT_CH7_CMPLT_B_MASK (0x1ul << DMA_INT_CH7_CMPLT_B_POS)

#define DMA_MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH0_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH0_ERR_EX_SIZE_A_POS (0) /*< bit[0]      */
#define DMA_INT_CH0_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_CH0_ERR_EX_SIZE_A_POS)
#define DMA_INT_CH1_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH1_ERR_EX_SIZE_A_POS (1) /*< bit[1]      */
#define DMA_INT_CH1_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_CH1_ERR_EX_SIZE_A_POS)
#define DMA_INT_CH2_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH2_ERR_EX_SIZE_A_POS (2) /*< bit[2]      */
#define DMA_INT_CH2_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_CH2_ERR_EX_SIZE_A_POS)
#define DMA_INT_CH3_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH3_ERR_EX_SIZE_A_POS (3) /*< bit[3]      */
#define DMA_INT_CH3_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_CH3_ERR_EX_SIZE_A_POS)
#define DMA_INT_CH4_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH4_ERR_EX_SIZE_A_POS (4) /*< bit[4]      */
#define DMA_INT_CH4_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_CH4_ERR_EX_SIZE_A_POS)
#define DMA_INT_CH5_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH5_ERR_EX_SIZE_A_POS (5) /*< bit[5]      */
#define DMA_INT_CH5_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_CH5_ERR_EX_SIZE_A_POS)
#define DMA_INT_CH6_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH6_ERR_EX_SIZE_A_POS (6) /*< bit[6]      */
#define DMA_INT_CH6_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_CH6_ERR_EX_SIZE_A_POS)
#define DMA_INT_CH7_ERR_EX_SIZE_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_A_REG)
#define DMA_INT_CH7_ERR_EX_SIZE_A_POS (7) /*< bit[7]      */
#define DMA_INT_CH7_ERR_EX_SIZE_A_MASK (0x1ul << DMA_INT_CH7_ERR_EX_SIZE_A_POS)

#define DMA_MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH0_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH0_ERR_EX_SIZE_B_POS (0) /*< bit[0]      */
#define DMA_INT_CH0_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_CH0_ERR_EX_SIZE_B_POS)
#define DMA_INT_CH1_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH1_ERR_EX_SIZE_B_POS (1) /*< bit[1]      */
#define DMA_INT_CH1_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_CH1_ERR_EX_SIZE_B_POS)
#define DMA_INT_CH2_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH2_ERR_EX_SIZE_B_POS (2) /*< bit[2]      */
#define DMA_INT_CH2_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_CH2_ERR_EX_SIZE_B_POS)
#define DMA_INT_CH3_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH3_ERR_EX_SIZE_B_POS (3) /*< bit[3]      */
#define DMA_INT_CH3_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_CH3_ERR_EX_SIZE_B_POS)
#define DMA_INT_CH4_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH4_ERR_EX_SIZE_B_POS (4) /*< bit[4]      */
#define DMA_INT_CH4_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_CH4_ERR_EX_SIZE_B_POS)
#define DMA_INT_CH5_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH5_ERR_EX_SIZE_B_POS (5) /*< bit[5]      */
#define DMA_INT_CH5_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_CH5_ERR_EX_SIZE_B_POS)
#define DMA_INT_CH6_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH6_ERR_EX_SIZE_B_POS (6) /*< bit[6]      */
#define DMA_INT_CH6_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_CH6_ERR_EX_SIZE_B_POS)
#define DMA_INT_CH7_ERR_EX_SIZE_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_EX_SIZE_B_REG)
#define DMA_INT_CH7_ERR_EX_SIZE_B_POS (7) /*< bit[7]      */
#define DMA_INT_CH7_ERR_EX_SIZE_B_MASK (0x1ul << DMA_INT_CH7_ERR_EX_SIZE_B_POS)

#define DMA_MASKED_INT_ST_CH_ERR_OVRN_A_REG_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH0_ERR_OVRN_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH0_ERR_OVRN_A_POS (0) /*< bit[0]      */
#define DMA_INT_CH0_ERR_OVRN_A_MASK (0x1ul << DMA_INT_CH0_ERR_OVRN_A_POS)
#define DMA_INT_CH1_ERR_OVRN_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH1_ERR_OVRN_A_POS (1) /*< bit[1]      */
#define DMA_INT_CH1_ERR_OVRN_A_MASK (0x1ul << DMA_INT_CH1_ERR_OVRN_A_POS)
#define DMA_INT_CH2_ERR_OVRN_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH2_ERR_OVRN_A_POS (2) /*< bit[2]      */
#define DMA_INT_CH2_ERR_OVRN_A_MASK (0x1ul << DMA_INT_CH2_ERR_OVRN_A_POS)
#define DMA_INT_CH3_ERR_OVRN_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH3_ERR_OVRN_A_POS (3) /*< bit[3]      */
#define DMA_INT_CH3_ERR_OVRN_A_MASK (0x1ul << DMA_INT_CH3_ERR_OVRN_A_POS)
#define DMA_INT_CH4_ERR_OVRN_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH4_ERR_OVRN_A_POS (4) /*< bit[4]      */
#define DMA_INT_CH4_ERR_OVRN_A_MASK (0x1ul << DMA_INT_CH4_ERR_OVRN_A_POS)
#define DMA_INT_CH5_ERR_OVRN_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH5_ERR_OVRN_A_POS (5) /*< bit[5]      */
#define DMA_INT_CH5_ERR_OVRN_A_MASK (0x1ul << DMA_INT_CH5_ERR_OVRN_A_POS)
#define DMA_INT_CH6_ERR_OVRN_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH6_ERR_OVRN_A_POS (6) /*< bit[6]      */
#define DMA_INT_CH6_ERR_OVRN_A_MASK (0x1ul << DMA_INT_CH6_ERR_OVRN_A_POS)
#define DMA_INT_CH7_ERR_OVRN_A_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_A_REG)
#define DMA_INT_CH7_ERR_OVRN_A_POS (7) /*< bit[7]      */
#define DMA_INT_CH7_ERR_OVRN_A_MASK (0x1ul << DMA_INT_CH7_ERR_OVRN_A_POS)

#define DMA_MASKED_INT_ST_CH_ERR_OVRN_B_REG_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH0_ERR_OVRN_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH0_ERR_OVRN_B_POS (0) /*< bit[0]      */
#define DMA_INT_CH0_ERR_OVRN_B_MASK (0x1ul << DMA_INT_CH0_ERR_OVRN_B_POS)
#define DMA_INT_CH1_ERR_OVRN_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH1_ERR_OVRN_B_POS (1) /*< bit[1]      */
#define DMA_INT_CH1_ERR_OVRN_B_MASK (0x1ul << DMA_INT_CH1_ERR_OVRN_B_POS)
#define DMA_INT_CH2_ERR_OVRN_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH2_ERR_OVRN_B_POS (2) /*< bit[2]      */
#define DMA_INT_CH2_ERR_OVRN_B_MASK (0x1ul << DMA_INT_CH2_ERR_OVRN_B_POS)
#define DMA_INT_CH3_ERR_OVRN_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH3_ERR_OVRN_B_POS (3) /*< bit[3]      */
#define DMA_INT_CH3_ERR_OVRN_B_MASK (0x1ul << DMA_INT_CH3_ERR_OVRN_B_POS)
#define DMA_INT_CH4_ERR_OVRN_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH4_ERR_OVRN_B_POS (4) /*< bit[4]      */
#define DMA_INT_CH4_ERR_OVRN_B_MASK (0x1ul << DMA_INT_CH4_ERR_OVRN_B_POS)
#define DMA_INT_CH5_ERR_OVRN_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH5_ERR_OVRN_B_POS (5) /*< bit[5]      */
#define DMA_INT_CH5_ERR_OVRN_B_MASK (0x1ul << DMA_INT_CH5_ERR_OVRN_B_POS)
#define DMA_INT_CH6_ERR_OVRN_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH6_ERR_OVRN_B_POS (6) /*< bit[6]      */
#define DMA_INT_CH6_ERR_OVRN_B_MASK (0x1ul << DMA_INT_CH6_ERR_OVRN_B_POS)
#define DMA_INT_CH7_ERR_OVRN_B_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_ERR_OVRN_B_REG)
#define DMA_INT_CH7_ERR_OVRN_B_POS (7) /*< bit[7]      */
#define DMA_INT_CH7_ERR_OVRN_B_MASK (0x1ul << DMA_INT_CH7_ERR_OVRN_B_POS)

#define DMA_MASKED_INT_ST_CH_M2M_CMPLT_REG_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH0_M2M_CMPLT_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH0_M2M_CMPLT_POS (0) /*< bit[0]      */
#define DMA_INT_CH0_M2M_CMPLT_MASK (0x1ul << DMA_INT_CH0_M2M_CMPLT_POS)
#define DMA_INT_CH1_M2M_CMPLT_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH1_M2M_CMPLT_POS (1) /*< bit[1]      */
#define DMA_INT_CH1_M2M_CMPLT_MASK (0x1ul << DMA_INT_CH1_M2M_CMPLT_POS)
#define DMA_INT_CH2_M2M_CMPLT_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH2_M2M_CMPLT_POS (2) /*< bit[2]      */
#define DMA_INT_CH2_M2M_CMPLT_MASK (0x1ul << DMA_INT_CH2_M2M_CMPLT_POS)
#define DMA_INT_CH3_M2M_CMPLT_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH3_M2M_CMPLT_POS (3) /*< bit[3]      */
#define DMA_INT_CH3_M2M_CMPLT_MASK (0x1ul << DMA_INT_CH3_M2M_CMPLT_POS)
#define DMA_INT_CH4_M2M_CMPLT_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH4_M2M_CMPLT_POS (4) /*< bit[4]      */
#define DMA_INT_CH4_M2M_CMPLT_MASK (0x1ul << DMA_INT_CH4_M2M_CMPLT_POS)
#define DMA_INT_CH5_M2M_CMPLT_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH5_M2M_CMPLT_POS (5) /*< bit[5]      */
#define DMA_INT_CH5_M2M_CMPLT_MASK (0x1ul << DMA_INT_CH5_M2M_CMPLT_POS)
#define DMA_INT_CH6_M2M_CMPLT_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH6_M2M_CMPLT_POS (6) /*< bit[6]      */
#define DMA_INT_CH6_M2M_CMPLT_MASK (0x1ul << DMA_INT_CH6_M2M_CMPLT_POS)
#define DMA_INT_CH7_M2M_CMPLT_ADDR (uint32_t)&(DMA->MASKED_INT_ST_CH_M2M_CMPLT_REG)
#define DMA_INT_CH7_M2M_CMPLT_POS (7) /*< bit[7]      */
#define DMA_INT_CH7_M2M_CMPLT_MASK (0x1ul << DMA_INT_CH7_M2M_CMPLT_POS)

#define DMA_PERIPH_ID_LEN_REG_ADDR (uint32_t)&(DMA->PERIPH_ID_LEN_REG)
#define DMA_PERIPH_ID_LEN_ADDR (uint32_t)&(DMA->PERIPH_ID_LEN_REG)
#define DMA_PERIPH_ID_LEN_POS (0) /*< bit[3:0]    */
#define DMA_PERIPH_ID_LEN_MASK (0xFul << DMA_PERIPH_ID_LEN_POS)

#define DMA_PERIPH_ID_0_REG_ADDR (uint32_t)&(DMA->PERIPH_ID_0_REG)
#define DMA_PERIPH_ID_1_REG_ADDR (uint32_t)&(DMA->PERIPH_ID_1_REG)
#define DMA_PERIPH_ID_2_REG_ADDR (uint32_t)&(DMA->PERIPH_ID_2_REG)

#define DMA_BASE 0x4003B000UL
#define DMA ((DMA_T *)DMA_BASE)

#endif

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __BBP_REG_ER8130_H__
#define __BBP_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"

/* ================================================================================ */
/* ================                      BBP_T                     ================ */
/* ================================================================================ */

/**
 * @brief BBP (BBP_T)
 */

typedef struct {                                          /*!< BBP_T Structure                                                            */
  __IO uint32_t RXIQCAL;                                   /*!< RXIQCAL Register (0x000)                                                   */
  __I  uint32_t RESERVED0[31];
  __IO uint32_t RX_FEC_CNTRL;                              /*!< RX_FEC_CNTRL Register (0x080)                                              */
  __IO uint32_t MDM_CNTRL;                                 /*!< MDM_CNTRL Register (0x084)                                                 */
  __I  uint32_t RESERVED1[8];
  __IO uint32_t RXGFSK_CNTL;                               /*!< RXGFSK_CNTL Register (0x0A8)                                               */
  __IO uint32_t GFO_PPTHR_BLE;                             /*!< GFO_PPTHR_BLE Register (0x0AC)                                             */
  __IO uint32_t GFO_GFSKDETECT;                            /*!< GFO_GFSKDETECT Register (0x0B0)                                            */
  __I  uint32_t RESERVED2[2];
  __IO uint32_t GFO_PEAKTIMETHR;                           /*!< GFO_PEAKTIMETHR Register (0x0BC)                                           */
  __I  uint32_t RESERVED3[4];
  __IO uint32_t PE_POWERTHR;                               /*!< PE_POWERTHR Register (0x0D0)                                               */
  __I  uint32_t RESERVED4[7];
  __IO uint32_t DDC_CIC;                                   /*!< DDC_CIC Register (0x0F0)                                                   */
  __I  uint32_t RESERVED5[3];
  __IO uint32_t CORR_CODE_BT_BLE;                          /*!< CORR_CODE_BT_BLE Register (0x100)                                          */
  __IO uint32_t CORR_ERR_CNT;                              /*!< CORR_ERR_CNT Register (0x104)                                              */
  __IO uint32_t RXLPF_0;                                   /*!< RXLPF_0 Register (0x108)                                                   */
  __IO uint32_t RXLPF_1;                                   /*!< RXLPF_1 Register (0x10C)                                                   */
  __IO uint32_t RXLPF_2;                                   /*!< RXLPF_2 Register (0x110)                                                   */
  __IO uint32_t RXLPF_3;                                   /*!< RXLPF_3 Register (0x114)                                                   */
  __IO uint32_t RXLPF_4;                                   /*!< RXLPF_4 Register (0x118)                                                   */
  __I  uint32_t RESERVED6[15];
  __IO uint32_t AGC1;                                      /*!< AGC1 Register (0x158)                                                      */
  __IO uint32_t AGC2;                                      /*!< AGC2 Register (0x15C)                                                      */
  __I  uint32_t RESERVED7[15];
  __IO uint32_t AGC18;                                     /*!< AGC18 Register (0x19C)                                                     */
  __I  uint32_t RESERVED8;
  __IO uint32_t AGC20;                                     /*!< AGC20 Register (0x1A4)                                                     */
  __IO uint32_t AGC21;                                     /*!< AGC21 Register (0x1A8)                                                     */
  __I  uint32_t RESERVED9;
  __IO uint32_t AGC23;                                     /*!< AGC23 Register (0x1B0)                                                     */
  __IO uint32_t AGC24;                                     /*!< AGC24 Register (0x1B4)                                                     */
  __I  uint32_t RESERVED10;
  __IO uint32_t AGC26;                                     /*!< AGC26 Register (0x1BC)                                                     */
  __IO uint32_t AGC27;                                     /*!< AGC27 Register (0x1C0)                                                     */
  __IO uint32_t AGC28;                                     /*!< AGC28 Register (0x1C4)                                                     */
  __I  uint32_t RESERVED11[13];
  __IO uint32_t AGC42;                                     /*!< AGC42 Register (0x1FC)                                                     */
  __IO uint32_t AGC43;                                     /*!< AGC43 Register (0x200)                                                     */
  __IO uint32_t AGC44;                                     /*!< AGC44 Register (0x204)                                                     */
  __IO uint32_t AGC45;                                     /*!< AGC45 Register (0x208)                                                     */
  __IO uint32_t DEWHITEN;                                  /*!< DEWHITEN Register (0x20C)                                                  */
  __IO uint32_t RXIQCAL2;                                  /*!< RXIQCAL2 Register (0x210)                                                  */
  __I  uint32_t RESERVED12[6];
  __IO uint32_t GFO_EST;                                   /*!< GFO_EST Register (0x22C)                                                   */
  __I  uint32_t RESERVED13[6];
  __IO uint32_t AGC_MODE;                                  /*!< AGC_MODE Register (0x248)                                                  */
  __I  uint32_t RESERVED14;
  __IO uint32_t AGC48;                                     /*!< AGC48 Register (0x250)                                                     */
  __I  uint32_t RESERVED15[2];
  __IO uint32_t IFEST1;                                    /*!< IFEST1 Register (0x25C)                                                    */
  __IO uint32_t IFEST2;                                    /*!< IFEST2 Register (0x260)                                                    */
  __IO uint32_t IFEST3;                                    /*!< IFEST3 Register (0x264)                                                    */
  __IO uint32_t IFEST4;                                    /*!< IFEST4 Register (0x268)                                                    */
  __IO uint32_t IFEST5;                                    /*!< IFEST5 Register (0x26C)                                                    */
  __IO uint32_t IFEST6;                                    /*!< IFEST6 Register (0x270)                                                    */
  __IO uint32_t IFEST7;                                    /*!< IFEST7 Register (0x274)                                                    */
  __IO uint32_t IFEST8;                                    /*!< IFEST8 Register (0x278)                                                    */
  __IO uint32_t FSC;                                       /*!< FSC Register (0x27C)                                                       */
  __I  uint32_t RESERVED16[33];
  __IO uint32_t CORE52;                                    /*!< CORE52 Register (0x304)                                                    */
  __I  uint32_t RESERVED17;
  __IO uint32_t POP_CLIPPING;                              /*!< POP_CLIPPING Register (0x30C)                                              */
  __IO uint32_t MBUS1;                                     /*!< MBUS1 Register (0x310)                                                     */
  __IO uint32_t MBUS2;                                     /*!< MBUS2 Register (0x314)                                                     */
  __IO uint32_t MBUS3;                                     /*!< MBUS3 Register (0x318)                                                     */
  __IO uint32_t MBUS4;                                     /*!< MBUS4 Register (0x31C)                                                     */
  __IO uint32_t MBUS5;                                     /*!< MBUS5 Register (0x320)                                                     */
  __I  uint32_t RESERVED18[55];
  __IO uint32_t DIGIT_GAIN;                                /*!< DIGIT_GAIN Register (0x400)                                                */
  __I  uint32_t RESERVED19[11];
  __IO uint32_t SCALE;                                     /*!< SCALE Register (0x430)                                                     */
  __IO uint32_t OOK;                                       /*!< OOK Register (0x434)                                                       */
} BBP_T;

#define BBP_RXIQCAL_ADDR                                   (uint32_t)&(BBP->RXIQCAL)
#define BBP_RG_BB_RXIQ_CAL_ADDR                            (uint32_t)&(BBP->RXIQCAL)
#define BBP_RG_BB_RXIQ_CAL_POS                             (0)      /*< bit[0]      */
#define BBP_RG_BB_RXIQ_CAL_MASK                            (0x1ul << BBP_RG_BB_RXIQ_CAL_POS)
#define BBP_RG_BB_RXIQ_NUM_ADDR                            (uint32_t)&(BBP->RXIQCAL)
#define BBP_RG_BB_RXIQ_NUM_POS                             (1)      /*< bit[2:1]    */
#define BBP_RG_BB_RXIQ_NUM_MASK                            (0x3ul << BBP_RG_BB_RXIQ_NUM_POS)
#define BBP_RG_BB_RXIQ_DELAY_ADDR                          (uint32_t)&(BBP->RXIQCAL)
#define BBP_RG_BB_RXIQ_DELAY_POS                           (3)      /*< bit[5:3]    */
#define BBP_RG_BB_RXIQ_DELAY_MASK                          (0x7ul << BBP_RG_BB_RXIQ_DELAY_POS)
#define BBP_RG_BB_RXIQ_VAL_MAN_ADDR                        (uint32_t)&(BBP->RXIQCAL)
#define BBP_RG_BB_RXIQ_VAL_MAN_POS                         (6)      /*< bit[6]      */
#define BBP_RG_BB_RXIQ_VAL_MAN_MASK                        (0x1ul << BBP_RG_BB_RXIQ_VAL_MAN_POS)
#define BBP_RG_PHY_TONE_PHASE_ADDR                         (uint32_t)&(BBP->RXIQCAL)
#define BBP_RG_PHY_TONE_PHASE_POS                          (9)      /*< bit[10:9]   */
#define BBP_RG_PHY_TONE_PHASE_MASK                         (0x3ul << BBP_RG_PHY_TONE_PHASE_POS)
#define BBP_RG_PHY_TONE_MAN_ADDR                           (uint32_t)&(BBP->RXIQCAL)
#define BBP_RG_PHY_TONE_MAN_POS                            (11)     /*< bit[11]     */
#define BBP_RG_PHY_TONE_MAN_MASK                           (0x1ul << BBP_RG_PHY_TONE_MAN_POS)
#define BBP_RG_PHY_TONE_SEL_ADDR                           (uint32_t)&(BBP->RXIQCAL)
#define BBP_RG_PHY_TONE_SEL_POS                            (12)     /*< bit[14:12]  */
#define BBP_RG_PHY_TONE_SEL_MASK                           (0x7ul << BBP_RG_PHY_TONE_SEL_POS)

#define BBP_RX_FEC_CNTRL_ADDR                              (uint32_t)&(BBP->RX_FEC_CNTRL)
#define BBP_RX_FEC_INFO_LEN_ADDR                           (uint32_t)&(BBP->RX_FEC_CNTRL)
#define BBP_RX_FEC_INFO_LEN_POS                            (0)      /*< bit[10:0]   */
#define BBP_RX_FEC_INFO_LEN_MASK                           (0x7FFul << BBP_RX_FEC_INFO_LEN_POS)
#define BBP_RX_FEC_LEN_SOURCE_ADDR                         (uint32_t)&(BBP->RX_FEC_CNTRL)
#define BBP_RX_FEC_LEN_SOURCE_POS                          (12)     /*< bit[13:12]  */
#define BBP_RX_FEC_LEN_SOURCE_MASK                         (0x3ul << BBP_RX_FEC_LEN_SOURCE_POS)
#define BBP_RX_FEC_BYPASS_ADDR                             (uint32_t)&(BBP->RX_FEC_CNTRL)
#define BBP_RX_FEC_BYPASS_POS                              (16)     /*< bit[16]     */
#define BBP_RX_FEC_BYPASS_MASK                             (0x1ul << BBP_RX_FEC_BYPASS_POS)
#define BBP_RX_INTRL_BYPASS_ADDR                           (uint32_t)&(BBP->RX_FEC_CNTRL)
#define BBP_RX_INTRL_BYPASS_POS                            (17)     /*< bit[17]     */
#define BBP_RX_INTRL_BYPASS_MASK                           (0x1ul << BBP_RX_INTRL_BYPASS_POS)
#define BBP_RX_DFE_MSB_SEL_ADDR                            (uint32_t)&(BBP->RX_FEC_CNTRL)
#define BBP_RX_DFE_MSB_SEL_POS                             (28)     /*< bit[31:28]  */
#define BBP_RX_DFE_MSB_SEL_MASK                            (0xFul << BBP_RX_DFE_MSB_SEL_POS)

#define BBP_MDM_CNTRL_ADDR                                 (uint32_t)&(BBP->MDM_CNTRL)
#define BBP_INT_RXFE_BYPASS_ADDR                           (uint32_t)&(BBP->MDM_CNTRL)
#define BBP_INT_RXFE_BYPASS_POS                            (0)      /*< bit[0]      */
#define BBP_INT_RXFE_BYPASS_MASK                           (0x1ul << BBP_INT_RXFE_BYPASS_POS)
#define BBP_INT_RX_CFG_SEL_ADDR                            (uint32_t)&(BBP->MDM_CNTRL)
#define BBP_INT_RX_CFG_SEL_POS                             (2)      /*< bit[2]      */
#define BBP_INT_RX_CFG_SEL_MASK                            (0x1ul << BBP_INT_RX_CFG_SEL_POS)
#define BBP_INT_RX_CFG_SEL_EXT_ADDR                        (uint32_t)&(BBP->MDM_CNTRL)
#define BBP_INT_RX_CFG_SEL_EXT_POS                         (3)      /*< bit[3]      */
#define BBP_INT_RX_CFG_SEL_EXT_MASK                        (0x1ul << BBP_INT_RX_CFG_SEL_EXT_POS)
#define BBP_INT_DUAL_MODE_EN_ADDR                          (uint32_t)&(BBP->MDM_CNTRL)
#define BBP_INT_DUAL_MODE_EN_POS                           (5)      /*< bit[5]      */
#define BBP_INT_DUAL_MODE_EN_MASK                          (0x1ul << BBP_INT_DUAL_MODE_EN_POS)

#define BBP_RXGFSK_CNTL_ADDR                               (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_GFO_ENSW_ADDR                              (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_GFO_ENSW_POS                               (1)      /*< bit[1]      */
#define BBP_INT_GFO_ENSW_MASK                              (0x1ul << BBP_INT_GFO_ENSW_POS)
#define BBP_INT_GFO_ENPL_BLE_ADDR                          (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_GFO_ENPL_BLE_POS                           (2)      /*< bit[2]      */
#define BBP_INT_GFO_ENPL_BLE_MASK                          (0x1ul << BBP_INT_GFO_ENPL_BLE_POS)
#define BBP_INT_GFO_ENPL_BT_ADDR                           (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_GFO_ENPL_BT_POS                            (3)      /*< bit[3]      */
#define BBP_INT_GFO_ENPL_BT_MASK                           (0x1ul << BBP_INT_GFO_ENPL_BT_POS)
#define BBP_INT_DFE_FORCERAW_BLE1M_ADDR                    (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_DFE_FORCERAW_BLE1M_POS                     (4)      /*< bit[4]      */
#define BBP_INT_DFE_FORCERAW_BLE1M_MASK                    (0x1ul << BBP_INT_DFE_FORCERAW_BLE1M_POS)
#define BBP_INT_DFE_FORCERAW_BLE2M_ADDR                    (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_DFE_FORCERAW_BLE2M_POS                     (5)      /*< bit[5]      */
#define BBP_INT_DFE_FORCERAW_BLE2M_MASK                    (0x1ul << BBP_INT_DFE_FORCERAW_BLE2M_POS)
#define BBP_INT_DFE_FORCERAW_BT_ADDR                       (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_DFE_FORCERAW_BT_POS                        (6)      /*< bit[6]      */
#define BBP_INT_DFE_FORCERAW_BT_MASK                       (0x1ul << BBP_INT_DFE_FORCERAW_BT_POS)
#define BBP_INT_STO_DISABLE_BLE1M_ADDR                     (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_STO_DISABLE_BLE1M_POS                      (8)      /*< bit[8]      */
#define BBP_INT_STO_DISABLE_BLE1M_MASK                     (0x1ul << BBP_INT_STO_DISABLE_BLE1M_POS)
#define BBP_INT_STO_DISABLE_BLE2M_ADDR                     (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_STO_DISABLE_BLE2M_POS                      (9)      /*< bit[9]      */
#define BBP_INT_STO_DISABLE_BLE2M_MASK                     (0x1ul << BBP_INT_STO_DISABLE_BLE2M_POS)
#define BBP_INT_STO_DISABLE_BT_ADDR                        (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_STO_DISABLE_BT_POS                         (10)     /*< bit[10]     */
#define BBP_INT_STO_DISABLE_BT_MASK                        (0x1ul << BBP_INT_STO_DISABLE_BT_POS)
#define BBP_INT_STO_QUALITY_EN_BLE1M_ADDR                  (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_STO_QUALITY_EN_BLE1M_POS                   (12)     /*< bit[12]     */
#define BBP_INT_STO_QUALITY_EN_BLE1M_MASK                  (0x1ul << BBP_INT_STO_QUALITY_EN_BLE1M_POS)
#define BBP_INT_STO_QUALITY_EN_BLE2M_ADDR                  (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_STO_QUALITY_EN_BLE2M_POS                   (13)     /*< bit[13]     */
#define BBP_INT_STO_QUALITY_EN_BLE2M_MASK                  (0x1ul << BBP_INT_STO_QUALITY_EN_BLE2M_POS)
#define BBP_INT_PSD_OMEGA_DIV2_EN_ADDR                     (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_PSD_OMEGA_DIV2_EN_POS                      (23)     /*< bit[23]     */
#define BBP_INT_PSD_OMEGA_DIV2_EN_MASK                     (0x1ul << BBP_INT_PSD_OMEGA_DIV2_EN_POS)
#define BBP_INT_PSD_SAT_EN_BLE1M_ADDR                      (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_PSD_SAT_EN_BLE1M_POS                       (24)     /*< bit[24]     */
#define BBP_INT_PSD_SAT_EN_BLE1M_MASK                      (0x1ul << BBP_INT_PSD_SAT_EN_BLE1M_POS)
#define BBP_INT_PSD_SAT_EN_BLE2M_ADDR                      (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_PSD_SAT_EN_BLE2M_POS                       (25)     /*< bit[25]     */
#define BBP_INT_PSD_SAT_EN_BLE2M_MASK                      (0x1ul << BBP_INT_PSD_SAT_EN_BLE2M_POS)
#define BBP_INT_PSD_SAT_EN_BT_ADDR                         (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_PSD_SAT_EN_BT_POS                          (26)     /*< bit[26]     */
#define BBP_INT_PSD_SAT_EN_BT_MASK                         (0x1ul << BBP_INT_PSD_SAT_EN_BT_POS)
#define BBP_INT_FO_COMP_NSYNC_DET_BLE1M_ADDR               (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_FO_COMP_NSYNC_DET_BLE1M_POS                (27)     /*< bit[28:27]  */
#define BBP_INT_FO_COMP_NSYNC_DET_BLE1M_MASK               (0x3ul << BBP_INT_FO_COMP_NSYNC_DET_BLE1M_POS)
#define BBP_INT_FO_COMP_NSYNC_DET_BLE2M_ADDR               (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_FO_COMP_NSYNC_DET_BLE2M_POS                (29)     /*< bit[29]     */
#define BBP_INT_FO_COMP_NSYNC_DET_BLE2M_MASK               (0x1ul << BBP_INT_FO_COMP_NSYNC_DET_BLE2M_POS)
#define BBP_INT_FO_COMP_NSYNC_DET_BT_ADDR                  (uint32_t)&(BBP->RXGFSK_CNTL)
#define BBP_INT_FO_COMP_NSYNC_DET_BT_POS                   (30)     /*< bit[30]     */
#define BBP_INT_FO_COMP_NSYNC_DET_BT_MASK                  (0x1ul << BBP_INT_FO_COMP_NSYNC_DET_BT_POS)

#define BBP_GFO_PPTHR_BLE_ADDR                             (uint32_t)&(BBP->GFO_PPTHR_BLE)
#define BBP_INT_GFO_P21PTHR_BLE_ADDR                       (uint32_t)&(BBP->GFO_PPTHR_BLE)
#define BBP_INT_GFO_P21PTHR_BLE_POS                        (0)      /*< bit[10:0]   */
#define BBP_INT_GFO_P21PTHR_BLE_MASK                       (0x7FFul << BBP_INT_GFO_P21PTHR_BLE_POS)
#define BBP_INT_GFO_P22PTHR_BLE_ADDR                       (uint32_t)&(BBP->GFO_PPTHR_BLE)
#define BBP_INT_GFO_P22PTHR_BLE_POS                        (16)     /*< bit[26:16]  */
#define BBP_INT_GFO_P22PTHR_BLE_MASK                       (0x7FFul << BBP_INT_GFO_P22PTHR_BLE_POS)

#define BBP_GFO_GFSKDETECT_ADDR                            (uint32_t)&(BBP->GFO_GFSKDETECT)
#define BBP_INT_GFO_GFSKDETECT_BT_ADDR                     (uint32_t)&(BBP->GFO_GFSKDETECT)
#define BBP_INT_GFO_GFSKDETECT_BT_POS                      (0)      /*< bit[10:0]   */
#define BBP_INT_GFO_GFSKDETECT_BT_MASK                     (0x7FFul << BBP_INT_GFO_GFSKDETECT_BT_POS)
#define BBP_INT_GFO_GFSKDETECT_BLE_ADDR                    (uint32_t)&(BBP->GFO_GFSKDETECT)
#define BBP_INT_GFO_GFSKDETECT_BLE_POS                     (12)     /*< bit[22:12]  */
#define BBP_INT_GFO_GFSKDETECT_BLE_MASK                    (0x7FFul << BBP_INT_GFO_GFSKDETECT_BLE_POS)
#define BBP_INT_GFO_DETECTLOCK_ADDR                        (uint32_t)&(BBP->GFO_GFSKDETECT)
#define BBP_INT_GFO_DETECTLOCK_POS                         (24)     /*< bit[28:24]  */
#define BBP_INT_GFO_DETECTLOCK_MASK                        (0x1Ful << BBP_INT_GFO_DETECTLOCK_POS)

#define BBP_GFO_PEAKTIMETHR_ADDR                           (uint32_t)&(BBP->GFO_PEAKTIMETHR)
#define BBP_INT_GFO_PEAKTIMETHR_1M_ADDR                    (uint32_t)&(BBP->GFO_PEAKTIMETHR)
#define BBP_INT_GFO_PEAKTIMETHR_1M_POS                     (0)      /*< bit[4:0]    */
#define BBP_INT_GFO_PEAKTIMETHR_1M_MASK                    (0x1Ful << BBP_INT_GFO_PEAKTIMETHR_1M_POS)
#define BBP_INT_GFO_PEAKTIMETHR_2M_ADDR                    (uint32_t)&(BBP->GFO_PEAKTIMETHR)
#define BBP_INT_GFO_PEAKTIMETHR_2M_POS                     (8)      /*< bit[12:8]   */
#define BBP_INT_GFO_PEAKTIMETHR_2M_MASK                    (0x1Ful << BBP_INT_GFO_PEAKTIMETHR_2M_POS)
#define BBP_INT_GFO_PEAKTIMETHRL_1M_ADDR                   (uint32_t)&(BBP->GFO_PEAKTIMETHR)
#define BBP_INT_GFO_PEAKTIMETHRL_1M_POS                    (16)     /*< bit[20:16]  */
#define BBP_INT_GFO_PEAKTIMETHRL_1M_MASK                   (0x1Ful << BBP_INT_GFO_PEAKTIMETHRL_1M_POS)
#define BBP_INT_GFO_PEAKTIMETHRL_2M_ADDR                   (uint32_t)&(BBP->GFO_PEAKTIMETHR)
#define BBP_INT_GFO_PEAKTIMETHRL_2M_POS                    (24)     /*< bit[28:24]  */
#define BBP_INT_GFO_PEAKTIMETHRL_2M_MASK                   (0x1Ful << BBP_INT_GFO_PEAKTIMETHRL_2M_POS)

#define BBP_PE_POWERTHR_ADDR                               (uint32_t)&(BBP->PE_POWERTHR)
#define BBP_INT_PE_PKTPOWERTHR_ADDR                        (uint32_t)&(BBP->PE_POWERTHR)
#define BBP_INT_PE_PKTPOWERTHR_POS                         (0)      /*< bit[5:0]    */
#define BBP_INT_PE_PKTPOWERTHR_MASK                        (0x3Ful << BBP_INT_PE_PKTPOWERTHR_POS)
#define BBP_INT_PE_POWERRAISETH_ADDR                       (uint32_t)&(BBP->PE_POWERTHR)
#define BBP_INT_PE_POWERRAISETH_POS                        (8)      /*< bit[13:8]   */
#define BBP_INT_PE_POWERRAISETH_MASK                       (0x3Ful << BBP_INT_PE_POWERRAISETH_POS)
#define BBP_INT_PE_EDPOWERTHR_ADDR                         (uint32_t)&(BBP->PE_POWERTHR)
#define BBP_INT_PE_EDPOWERTHR_POS                          (16)     /*< bit[21:16]  */
#define BBP_INT_PE_EDPOWERTHR_MASK                         (0x3Ful << BBP_INT_PE_EDPOWERTHR_POS)
#define BBP_INT_PE_PWRRAISE_THRES_POS_EN_ADDR              (uint32_t)&(BBP->PE_POWERTHR)
#define BBP_INT_PE_PWRRAISE_THRES_POS_EN_POS               (22)     /*< bit[22]     */
#define BBP_INT_PE_PWRRAISE_THRES_POS_EN_MASK              (0x1ul << BBP_INT_PE_PWRRAISE_THRES_POS_EN_POS)

#define BBP_DDC_CIC_ADDR                                   (uint32_t)&(BBP->DDC_CIC)
#define BBP_DDC_IF_PHI_ADDR                                (uint32_t)&(BBP->DDC_CIC)
#define BBP_DDC_IF_PHI_POS                                 (0)      /*< bit[15:0]   */
#define BBP_DDC_IF_PHI_MASK                                (0xFFFFul << BBP_DDC_IF_PHI_POS)
#define BBP_DDC_OUT_FLIP_EN_ADDR                           (uint32_t)&(BBP->DDC_CIC)
#define BBP_DDC_OUT_FLIP_EN_POS                            (16)     /*< bit[16]     */
#define BBP_DDC_OUT_FLIP_EN_MASK                           (0x1ul << BBP_DDC_OUT_FLIP_EN_POS)
#define BBP_DCRMVL_GEAR_SHIFT_ADDR                         (uint32_t)&(BBP->DDC_CIC)
#define BBP_DCRMVL_GEAR_SHIFT_POS                          (18)     /*< bit[27:18]  */
#define BBP_DCRMVL_GEAR_SHIFT_MASK                         (0x3FFul << BBP_DCRMVL_GEAR_SHIFT_POS)
#define BBP_DCRMVL_ENABLE_ADDR                             (uint32_t)&(BBP->DDC_CIC)
#define BBP_DCRMVL_ENABLE_POS                              (28)     /*< bit[28]     */
#define BBP_DCRMVL_ENABLE_MASK                             (0x1ul << BBP_DCRMVL_ENABLE_POS)
#define BBP_SAR_ADC_FORMAT_ADDR                            (uint32_t)&(BBP->DDC_CIC)
#define BBP_SAR_ADC_FORMAT_POS                             (29)     /*< bit[29]     */
#define BBP_SAR_ADC_FORMAT_MASK                            (0x1ul << BBP_SAR_ADC_FORMAT_POS)
#define BBP_AGC_INSEL_ADDR                                 (uint32_t)&(BBP->DDC_CIC)
#define BBP_AGC_INSEL_POS                                  (30)     /*< bit[30]     */
#define BBP_AGC_INSEL_MASK                                 (0x1ul << BBP_AGC_INSEL_POS)

#define BBP_CORR_CODE_BT_BLE_ADDR                          (uint32_t)&(BBP->CORR_CODE_BT_BLE)
#define BBP_CORR_ERR_CNT_ADDR                              (uint32_t)&(BBP->CORR_ERR_CNT)
#define BBP_INT_SYNC_ERR_BLE_ADDR                          (uint32_t)&(BBP->CORR_ERR_CNT)
#define BBP_INT_SYNC_ERR_BLE_POS                           (0)      /*< bit[2:0]    */
#define BBP_INT_SYNC_ERR_BLE_MASK                          (0x7ul << BBP_INT_SYNC_ERR_BLE_POS)
#define BBP_INT_SYNC_SAMP_ADJ_BLE1M_ADDR                   (uint32_t)&(BBP->CORR_ERR_CNT)
#define BBP_INT_SYNC_SAMP_ADJ_BLE1M_POS                    (3)      /*< bit[5:3]    */
#define BBP_INT_SYNC_SAMP_ADJ_BLE1M_MASK                   (0x7ul << BBP_INT_SYNC_SAMP_ADJ_BLE1M_POS)
#define BBP_INT_SYNC_SAMP_ADJ_BLE2M_ADDR                   (uint32_t)&(BBP->CORR_ERR_CNT)
#define BBP_INT_SYNC_SAMP_ADJ_BLE2M_POS                    (6)      /*< bit[8:6]    */
#define BBP_INT_SYNC_SAMP_ADJ_BLE2M_MASK                   (0x7ul << BBP_INT_SYNC_SAMP_ADJ_BLE2M_POS)
#define BBP_INT_EXT_CORRCODE_EN_ADDR                       (uint32_t)&(BBP->CORR_ERR_CNT)
#define BBP_INT_EXT_CORRCODE_EN_POS                        (9)      /*< bit[9]      */
#define BBP_INT_EXT_CORRCODE_EN_MASK                       (0x1ul << BBP_INT_EXT_CORRCODE_EN_POS)
#define BBP_INT_SYNC_ERR_BT_ADDR                           (uint32_t)&(BBP->CORR_ERR_CNT)
#define BBP_INT_SYNC_ERR_BT_POS                            (10)     /*< bit[13:10]  */
#define BBP_INT_SYNC_ERR_BT_MASK                           (0xFul << BBP_INT_SYNC_ERR_BT_POS)
#define BBP_INT_SYNC_SAMP_ADJ_BT_ADDR                      (uint32_t)&(BBP->CORR_ERR_CNT)
#define BBP_INT_SYNC_SAMP_ADJ_BT_POS                       (14)     /*< bit[16:14]  */
#define BBP_INT_SYNC_SAMP_ADJ_BT_MASK                      (0x7ul << BBP_INT_SYNC_SAMP_ADJ_BT_POS)
#define BBP_INT_CORRCODE_BYTE_NUM_ADDR                     (uint32_t)&(BBP->CORR_ERR_CNT)
#define BBP_INT_CORRCODE_BYTE_NUM_POS                      (17)     /*< bit[19:17]  */
#define BBP_INT_CORRCODE_BYTE_NUM_MASK                     (0x7ul << BBP_INT_CORRCODE_BYTE_NUM_POS)

#define BBP_RXLPF_0_ADDR                                   (uint32_t)&(BBP->RXLPF_0)
#define BBP_INT_RXLPF_COE0_ADDR                            (uint32_t)&(BBP->RXLPF_0)
#define BBP_INT_RXLPF_COE0_POS                             (0)      /*< bit[9:0]    */
#define BBP_INT_RXLPF_COE0_MASK                            (0x3FFul << BBP_INT_RXLPF_COE0_POS)
#define BBP_INT_RXLPF_COE1_ADDR                            (uint32_t)&(BBP->RXLPF_0)
#define BBP_INT_RXLPF_COE1_POS                             (11)     /*< bit[20:11]  */
#define BBP_INT_RXLPF_COE1_MASK                            (0x3FFul << BBP_INT_RXLPF_COE1_POS)
#define BBP_INT_RXLPF_COE2_ADDR                            (uint32_t)&(BBP->RXLPF_0)
#define BBP_INT_RXLPF_COE2_POS                             (22)     /*< bit[31:22]  */
#define BBP_INT_RXLPF_COE2_MASK                            (0x3FFul << BBP_INT_RXLPF_COE2_POS)

#define BBP_RXLPF_1_ADDR                                   (uint32_t)&(BBP->RXLPF_1)
#define BBP_INT_RXLPF_COE3_ADDR                            (uint32_t)&(BBP->RXLPF_1)
#define BBP_INT_RXLPF_COE3_POS                             (0)      /*< bit[9:0]    */
#define BBP_INT_RXLPF_COE3_MASK                            (0x3FFul << BBP_INT_RXLPF_COE3_POS)
#define BBP_INT_RXLPF_COE4_ADDR                            (uint32_t)&(BBP->RXLPF_1)
#define BBP_INT_RXLPF_COE4_POS                             (11)     /*< bit[20:11]  */
#define BBP_INT_RXLPF_COE4_MASK                            (0x3FFul << BBP_INT_RXLPF_COE4_POS)
#define BBP_INT_RXLPF_COE5_ADDR                            (uint32_t)&(BBP->RXLPF_1)
#define BBP_INT_RXLPF_COE5_POS                             (22)     /*< bit[31:22]  */
#define BBP_INT_RXLPF_COE5_MASK                            (0x3FFul << BBP_INT_RXLPF_COE5_POS)

#define BBP_RXLPF_2_ADDR                                   (uint32_t)&(BBP->RXLPF_2)
#define BBP_INT_RXLPF_COE6_ADDR                            (uint32_t)&(BBP->RXLPF_2)
#define BBP_INT_RXLPF_COE6_POS                             (0)      /*< bit[9:0]    */
#define BBP_INT_RXLPF_COE6_MASK                            (0x3FFul << BBP_INT_RXLPF_COE6_POS)
#define BBP_INT_RXLPF_COE7_ADDR                            (uint32_t)&(BBP->RXLPF_2)
#define BBP_INT_RXLPF_COE7_POS                             (11)     /*< bit[20:11]  */
#define BBP_INT_RXLPF_COE7_MASK                            (0x3FFul << BBP_INT_RXLPF_COE7_POS)
#define BBP_INT_RXLPF_COE8_ADDR                            (uint32_t)&(BBP->RXLPF_2)
#define BBP_INT_RXLPF_COE8_POS                             (22)     /*< bit[31:22]  */
#define BBP_INT_RXLPF_COE8_MASK                            (0x3FFul << BBP_INT_RXLPF_COE8_POS)

#define BBP_RXLPF_3_ADDR                                   (uint32_t)&(BBP->RXLPF_3)
#define BBP_INT_RXLPF_COE9_ADDR                            (uint32_t)&(BBP->RXLPF_3)
#define BBP_INT_RXLPF_COE9_POS                             (0)      /*< bit[9:0]    */
#define BBP_INT_RXLPF_COE9_MASK                            (0x3FFul << BBP_INT_RXLPF_COE9_POS)
#define BBP_INT_RXLPF_COE10_ADDR                           (uint32_t)&(BBP->RXLPF_3)
#define BBP_INT_RXLPF_COE10_POS                            (11)     /*< bit[20:11]  */
#define BBP_INT_RXLPF_COE10_MASK                           (0x3FFul << BBP_INT_RXLPF_COE10_POS)
#define BBP_INT_RXLPF_COE11_ADDR                           (uint32_t)&(BBP->RXLPF_3)
#define BBP_INT_RXLPF_COE11_POS                            (22)     /*< bit[31:22]  */
#define BBP_INT_RXLPF_COE11_MASK                           (0x3FFul << BBP_INT_RXLPF_COE11_POS)

#define BBP_RXLPF_4_ADDR                                   (uint32_t)&(BBP->RXLPF_4)
#define BBP_INT_RXLPF_COE12_ADDR                           (uint32_t)&(BBP->RXLPF_4)
#define BBP_INT_RXLPF_COE12_POS                            (0)      /*< bit[9:0]    */
#define BBP_INT_RXLPF_COE12_MASK                           (0x3FFul << BBP_INT_RXLPF_COE12_POS)
#define BBP_INT_RXLPF_COE13_ADDR                           (uint32_t)&(BBP->RXLPF_4)
#define BBP_INT_RXLPF_COE13_POS                            (11)     /*< bit[20:11]  */
#define BBP_INT_RXLPF_COE13_MASK                           (0x3FFul << BBP_INT_RXLPF_COE13_POS)
#define BBP_INT_RXLPF_GAIN_ADDR                            (uint32_t)&(BBP->RXLPF_4)
#define BBP_INT_RXLPF_GAIN_POS                             (28)     /*< bit[31:28]  */
#define BBP_INT_RXLPF_GAIN_MASK                            (0xFul << BBP_INT_RXLPF_GAIN_POS)

#define BBP_AGC1_ADDR                                      (uint32_t)&(BBP->AGC1)
#define BBP_AGC_TIME_OUT_MAX_ADDR                          (uint32_t)&(BBP->AGC1)
#define BBP_AGC_TIME_OUT_MAX_POS                           (0)      /*< bit[23:0]   */
#define BBP_AGC_TIME_OUT_MAX_MASK                          (0xFFFFFFul << BBP_AGC_TIME_OUT_MAX_POS)

#define BBP_AGC2_ADDR                                      (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_0_ADDR                (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_0_POS                 (0)      /*< bit[3:0]    */
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_0_MASK                (0xFul << BBP_AGC_GAIN_TABLE_MIX_INDEX_0_POS)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_1_ADDR                (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_1_POS                 (4)      /*< bit[7:4]    */
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_1_MASK                (0xFul << BBP_AGC_GAIN_TABLE_MIX_INDEX_1_POS)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_2_ADDR                (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_2_POS                 (8)      /*< bit[11:8]   */
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_2_MASK                (0xFul << BBP_AGC_GAIN_TABLE_MIX_INDEX_2_POS)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_3_ADDR                (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_3_POS                 (12)     /*< bit[15:12]  */
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_3_MASK                (0xFul << BBP_AGC_GAIN_TABLE_MIX_INDEX_3_POS)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_4_ADDR                (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_4_POS                 (16)     /*< bit[19:16]  */
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_4_MASK                (0xFul << BBP_AGC_GAIN_TABLE_MIX_INDEX_4_POS)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_5_ADDR                (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_5_POS                 (20)     /*< bit[23:20]  */
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_5_MASK                (0xFul << BBP_AGC_GAIN_TABLE_MIX_INDEX_5_POS)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_6_ADDR                (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_6_POS                 (24)     /*< bit[27:24]  */
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_6_MASK                (0xFul << BBP_AGC_GAIN_TABLE_MIX_INDEX_6_POS)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_7_ADDR                (uint32_t)&(BBP->AGC2)
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_7_POS                 (28)     /*< bit[31:28]  */
#define BBP_AGC_GAIN_TABLE_MIX_INDEX_7_MASK                (0xFul << BBP_AGC_GAIN_TABLE_MIX_INDEX_7_POS)

#define BBP_AGC18_ADDR                                     (uint32_t)&(BBP->AGC18)
#define BBP_AGC_GAIN_UPT_LOOP_MAX_ADDR                     (uint32_t)&(BBP->AGC18)
#define BBP_AGC_GAIN_UPT_LOOP_MAX_POS                      (0)      /*< bit[4:0]    */
#define BBP_AGC_GAIN_UPT_LOOP_MAX_MASK                     (0x1Ful << BBP_AGC_GAIN_UPT_LOOP_MAX_POS)
#define BBP_AGC_EG_SRC_SEL_ADDR                            (uint32_t)&(BBP->AGC18)
#define BBP_AGC_EG_SRC_SEL_POS                             (5)      /*< bit[5]      */
#define BBP_AGC_EG_SRC_SEL_MASK                            (0x1ul << BBP_AGC_EG_SRC_SEL_POS)
#define BBP_AGC_RF_SATURATE_FLAG_ADDR                      (uint32_t)&(BBP->AGC18)
#define BBP_AGC_RF_SATURATE_FLAG_POS                       (6)      /*< bit[6]      */
#define BBP_AGC_RF_SATURATE_FLAG_MASK                      (0x1ul << BBP_AGC_RF_SATURATE_FLAG_POS)
#define BBP_AGC_RF_UNDERFLW_FLAG_ADDR                      (uint32_t)&(BBP->AGC18)
#define BBP_AGC_RF_UNDERFLW_FLAG_POS                       (7)      /*< bit[7]      */
#define BBP_AGC_RF_UNDERFLW_FLAG_MASK                      (0x1ul << BBP_AGC_RF_UNDERFLW_FLAG_POS)
#define BBP_AGC_RF_SAT_PROTECT_ON_ADDR                     (uint32_t)&(BBP->AGC18)
#define BBP_AGC_RF_SAT_PROTECT_ON_POS                      (8)      /*< bit[8]      */
#define BBP_AGC_RF_SAT_PROTECT_ON_MASK                     (0x1ul << BBP_AGC_RF_SAT_PROTECT_ON_POS)
#define BBP_AGC_UNDERFLOW_ADV_STEP_ADDR                    (uint32_t)&(BBP->AGC18)
#define BBP_AGC_UNDERFLOW_ADV_STEP_POS                     (9)      /*< bit[12:9]   */
#define BBP_AGC_UNDERFLOW_ADV_STEP_MASK                    (0xFul << BBP_AGC_UNDERFLOW_ADV_STEP_POS)
#define BBP_AGC_SAT_BACKOFF_STEP_ADDR                      (uint32_t)&(BBP->AGC18)
#define BBP_AGC_SAT_BACKOFF_STEP_POS                       (13)     /*< bit[16:13]  */
#define BBP_AGC_SAT_BACKOFF_STEP_MASK                      (0xFul << BBP_AGC_SAT_BACKOFF_STEP_POS)
#define BBP_AGC_CLIP_UPDATE_NUM_MAX_ADDR                   (uint32_t)&(BBP->AGC18)
#define BBP_AGC_CLIP_UPDATE_NUM_MAX_POS                    (17)     /*< bit[21:17]  */
#define BBP_AGC_CLIP_UPDATE_NUM_MAX_MASK                   (0x1Ful << BBP_AGC_CLIP_UPDATE_NUM_MAX_POS)

#define BBP_AGC20_ADDR                                     (uint32_t)&(BBP->AGC20)
#define BBP_AGC_EG_PWR_THR_ADDR                            (uint32_t)&(BBP->AGC20)
#define BBP_AGC_EG_PWR_THR_POS                             (0)      /*< bit[15:0]   */
#define BBP_AGC_EG_PWR_THR_MASK                            (0xFFFFul << BBP_AGC_EG_PWR_THR_POS)
#define BBP_AGC_ED_OVER_THR_PERIOD_ADDR                    (uint32_t)&(BBP->AGC20)
#define BBP_AGC_ED_OVER_THR_PERIOD_POS                     (16)     /*< bit[18:16]  */
#define BBP_AGC_ED_OVER_THR_PERIOD_MASK                    (0x7ul << BBP_AGC_ED_OVER_THR_PERIOD_POS)

#define BBP_AGC21_ADDR                                     (uint32_t)&(BBP->AGC21)
#define BBP_AGC_RF_SETTLE_TIME_ADDR                        (uint32_t)&(BBP->AGC21)
#define BBP_AGC_RF_SETTLE_TIME_POS                         (0)      /*< bit[15:0]   */
#define BBP_AGC_RF_SETTLE_TIME_MASK                        (0xFFFFul << BBP_AGC_RF_SETTLE_TIME_POS)

#define BBP_AGC23_ADDR                                     (uint32_t)&(BBP->AGC23)
#define BBP_AGC_TOTPWR_DBM_ADDR                            (uint32_t)&(BBP->AGC23)
#define BBP_AGC_TOTPWR_DBM_POS                             (0)      /*< bit[9:0]    */
#define BBP_AGC_TOTPWR_DBM_MASK                            (0x3FFul << BBP_AGC_TOTPWR_DBM_POS)
#define BBP_AGC_INBANDPWR_DBM_ADDR                         (uint32_t)&(BBP->AGC23)
#define BBP_AGC_INBANDPWR_DBM_POS                          (10)     /*< bit[19:10]  */
#define BBP_AGC_INBANDPWR_DBM_MASK                         (0x3FFul << BBP_AGC_INBANDPWR_DBM_POS)

#define BBP_AGC24_ADDR                                     (uint32_t)&(BBP->AGC24)
#define BBP_AGC_INBAND_TARGET_ADDR                         (uint32_t)&(BBP->AGC24)
#define BBP_AGC_INBAND_TARGET_POS                          (0)      /*< bit[15:0]   */
#define BBP_AGC_INBAND_TARGET_MASK                         (0xFFFFul << BBP_AGC_INBAND_TARGET_POS)

#define BBP_AGC26_ADDR                                     (uint32_t)&(BBP->AGC26)
#define BBP_AGC_GAIN_TABLE_PWR_DB_0_ADDR                   (uint32_t)&(BBP->AGC26)
#define BBP_AGC_GAIN_TABLE_PWR_DB_0_POS                    (0)      /*< bit[15:0]   */
#define BBP_AGC_GAIN_TABLE_PWR_DB_0_MASK                   (0xFFFFul << BBP_AGC_GAIN_TABLE_PWR_DB_0_POS)
#define BBP_AGC_GAIN_TABLE_PWR_DB_1_ADDR                   (uint32_t)&(BBP->AGC26)
#define BBP_AGC_GAIN_TABLE_PWR_DB_1_POS                    (16)     /*< bit[31:16]  */
#define BBP_AGC_GAIN_TABLE_PWR_DB_1_MASK                   (0xFFFFul << BBP_AGC_GAIN_TABLE_PWR_DB_1_POS)

#define BBP_AGC27_ADDR                                     (uint32_t)&(BBP->AGC27)
#define BBP_AGC_GAIN_TABLE_PWR_DB_2_ADDR                   (uint32_t)&(BBP->AGC27)
#define BBP_AGC_GAIN_TABLE_PWR_DB_2_POS                    (0)      /*< bit[15:0]   */
#define BBP_AGC_GAIN_TABLE_PWR_DB_2_MASK                   (0xFFFFul << BBP_AGC_GAIN_TABLE_PWR_DB_2_POS)
#define BBP_AGC_GAIN_TABLE_PWR_DB_3_ADDR                   (uint32_t)&(BBP->AGC27)
#define BBP_AGC_GAIN_TABLE_PWR_DB_3_POS                    (16)     /*< bit[31:16]  */
#define BBP_AGC_GAIN_TABLE_PWR_DB_3_MASK                   (0xFFFFul << BBP_AGC_GAIN_TABLE_PWR_DB_3_POS)

#define BBP_AGC28_ADDR                                     (uint32_t)&(BBP->AGC28)
#define BBP_AGC_GAIN_TABLE_PWR_DB_4_ADDR                   (uint32_t)&(BBP->AGC28)
#define BBP_AGC_GAIN_TABLE_PWR_DB_4_POS                    (0)      /*< bit[15:0]   */
#define BBP_AGC_GAIN_TABLE_PWR_DB_4_MASK                   (0xFFFFul << BBP_AGC_GAIN_TABLE_PWR_DB_4_POS)
#define BBP_AGC_GAIN_TABLE_PWR_DB_5_ADDR                   (uint32_t)&(BBP->AGC28)
#define BBP_AGC_GAIN_TABLE_PWR_DB_5_POS                    (16)     /*< bit[31:16]  */
#define BBP_AGC_GAIN_TABLE_PWR_DB_5_MASK                   (0xFFFFul << BBP_AGC_GAIN_TABLE_PWR_DB_5_POS)

#define BBP_AGC42_ADDR                                     (uint32_t)&(BBP->AGC42)
#define BBP_AGC_INBAND_RSSI_ADDR                           (uint32_t)&(BBP->AGC42)
#define BBP_AGC_INBAND_RSSI_POS                            (0)      /*< bit[11:0]   */
#define BBP_AGC_INBAND_RSSI_MASK                           (0xFFFul << BBP_AGC_INBAND_RSSI_POS)
#define BBP_AGC_INBAND_RSSI_RDY_N_ADDR                     (uint32_t)&(BBP->AGC42)
#define BBP_AGC_INBAND_RSSI_RDY_N_POS                      (31)     /*< bit[31]     */
#define BBP_AGC_INBAND_RSSI_RDY_N_MASK                     (0x1ul << BBP_AGC_INBAND_RSSI_RDY_N_POS)

#define BBP_AGC43_ADDR                                     (uint32_t)&(BBP->AGC43)
#define BBP_AGC_GAIN_TABLE_INITI_IDX_ADDR                  (uint32_t)&(BBP->AGC43)
#define BBP_AGC_GAIN_TABLE_INITI_IDX_POS                   (0)      /*< bit[4:0]    */
#define BBP_AGC_GAIN_TABLE_INITI_IDX_MASK                  (0x1Ful << BBP_AGC_GAIN_TABLE_INITI_IDX_POS)
#define BBP_AGC_PWR_OFFSET_TOT_ADDR                        (uint32_t)&(BBP->AGC43)
#define BBP_AGC_PWR_OFFSET_TOT_POS                         (5)      /*< bit[14:5]   */
#define BBP_AGC_PWR_OFFSET_TOT_MASK                        (0x3FFul << BBP_AGC_PWR_OFFSET_TOT_POS)
#define BBP_AGC_PWR_OFFSET_INBAND_ADDR                     (uint32_t)&(BBP->AGC43)
#define BBP_AGC_PWR_OFFSET_INBAND_POS                      (15)     /*< bit[24:15]  */
#define BBP_AGC_PWR_OFFSET_INBAND_MASK                     (0x3FFul << BBP_AGC_PWR_OFFSET_INBAND_POS)
#define BBP_AGC_DONE_RST_GFO_EN_ADDR                       (uint32_t)&(BBP->AGC43)
#define BBP_AGC_DONE_RST_GFO_EN_POS                        (28)     /*< bit[28]     */
#define BBP_AGC_DONE_RST_GFO_EN_MASK                       (0x1ul << BBP_AGC_DONE_RST_GFO_EN_POS)

#define BBP_AGC44_ADDR                                     (uint32_t)&(BBP->AGC44)
#define BBP_AGC_RAMP_UP_ED_THR_ADDR                        (uint32_t)&(BBP->AGC44)
#define BBP_AGC_RAMP_UP_ED_THR_POS                         (0)      /*< bit[11:0]   */
#define BBP_AGC_RAMP_UP_ED_THR_MASK                        (0xFFFul << BBP_AGC_RAMP_UP_ED_THR_POS)
#define BBP_AGC_RAMP_UP_ED_AVG_SIZE_ADDR                   (uint32_t)&(BBP->AGC44)
#define BBP_AGC_RAMP_UP_ED_AVG_SIZE_POS                    (12)     /*< bit[13:12]  */
#define BBP_AGC_RAMP_UP_ED_AVG_SIZE_MASK                   (0x3ul << BBP_AGC_RAMP_UP_ED_AVG_SIZE_POS)
#define BBP_AGC_MULTI_ED_MODE_ADDR                         (uint32_t)&(BBP->AGC44)
#define BBP_AGC_MULTI_ED_MODE_POS                          (14)     /*< bit[14]     */
#define BBP_AGC_MULTI_ED_MODE_MASK                         (0x1ul << BBP_AGC_MULTI_ED_MODE_POS)
#define BBP_AGC_DONE_RST_BBP_EN_ADDR                       (uint32_t)&(BBP->AGC44)
#define BBP_AGC_DONE_RST_BBP_EN_POS                        (15)     /*< bit[15]     */
#define BBP_AGC_DONE_RST_BBP_EN_MASK                       (0x1ul << BBP_AGC_DONE_RST_BBP_EN_POS)
#define BBP_AGC_INIT_SETTLE_TIME_ADDR                      (uint32_t)&(BBP->AGC44)
#define BBP_AGC_INIT_SETTLE_TIME_POS                       (16)     /*< bit[31:16]  */
#define BBP_AGC_INIT_SETTLE_TIME_MASK                      (0xFFFFul << BBP_AGC_INIT_SETTLE_TIME_POS)

#define BBP_AGC45_ADDR                                     (uint32_t)&(BBP->AGC45)
#define BBP_AGC_TOTPWR_WINSIZE_ADDR                        (uint32_t)&(BBP->AGC45)
#define BBP_AGC_TOTPWR_WINSIZE_POS                         (0)      /*< bit[3:0]    */
#define BBP_AGC_TOTPWR_WINSIZE_MASK                        (0xFul << BBP_AGC_TOTPWR_WINSIZE_POS)
#define BBP_AGC_INBANDPWR_WINSIZE_ADDR                     (uint32_t)&(BBP->AGC45)
#define BBP_AGC_INBANDPWR_WINSIZE_POS                      (4)      /*< bit[7:4]    */
#define BBP_AGC_INBANDPWR_WINSIZE_MASK                     (0xFul << BBP_AGC_INBANDPWR_WINSIZE_POS)
#define BBP_AGC_INBANDPWR_ED_WINSIZE_ADDR                  (uint32_t)&(BBP->AGC45)
#define BBP_AGC_INBANDPWR_ED_WINSIZE_POS                   (8)      /*< bit[11:8]   */
#define BBP_AGC_INBANDPWR_ED_WINSIZE_MASK                  (0xFul << BBP_AGC_INBANDPWR_ED_WINSIZE_POS)
#define BBP_AGC_INBANDPWR_RSSI_WINSIZE_ADDR                (uint32_t)&(BBP->AGC45)
#define BBP_AGC_INBANDPWR_RSSI_WINSIZE_POS                 (12)     /*< bit[15:12]  */
#define BBP_AGC_INBANDPWR_RSSI_WINSIZE_MASK                (0xFul << BBP_AGC_INBANDPWR_RSSI_WINSIZE_POS)
#define BBP_AGC_TOTPWR_RSSI_WINSIZE_ADDR                   (uint32_t)&(BBP->AGC45)
#define BBP_AGC_TOTPWR_RSSI_WINSIZE_POS                    (16)     /*< bit[19:16]  */
#define BBP_AGC_TOTPWR_RSSI_WINSIZE_MASK                   (0xFul << BBP_AGC_TOTPWR_RSSI_WINSIZE_POS)
#define BBP_AGC_SHORT_CCA_MODE_ADDR                        (uint32_t)&(BBP->AGC45)
#define BBP_AGC_SHORT_CCA_MODE_POS                         (20)     /*< bit[20]     */
#define BBP_AGC_SHORT_CCA_MODE_MASK                        (0x1ul << BBP_AGC_SHORT_CCA_MODE_POS)
#define BBP_AGC_SHORT_CCA_WINSIZE_ADDR                     (uint32_t)&(BBP->AGC45)
#define BBP_AGC_SHORT_CCA_WINSIZE_POS                      (24)     /*< bit[27:24]  */
#define BBP_AGC_SHORT_CCA_WINSIZE_MASK                     (0xFul << BBP_AGC_SHORT_CCA_WINSIZE_POS)

#define BBP_DEWHITEN_ADDR                                  (uint32_t)&(BBP->DEWHITEN)
#define BBP_DEWHITEN_INIT_VAL_ADDR                         (uint32_t)&(BBP->DEWHITEN)
#define BBP_DEWHITEN_INIT_VAL_POS                          (0)      /*< bit[8:0]    */
#define BBP_DEWHITEN_INIT_VAL_MASK                         (0x1FFul << BBP_DEWHITEN_INIT_VAL_POS)
#define BBP_DEWHITEN_EN_ADDR                               (uint32_t)&(BBP->DEWHITEN)
#define BBP_DEWHITEN_EN_POS                                (9)      /*< bit[9]      */
#define BBP_DEWHITEN_EN_MASK                               (0x1ul << BBP_DEWHITEN_EN_POS)
#define BBP_DEWHITEN_MODE_ADDR                             (uint32_t)&(BBP->DEWHITEN)
#define BBP_DEWHITEN_MODE_POS                              (16)     /*< bit[16]     */
#define BBP_DEWHITEN_MODE_MASK                             (0x1ul << BBP_DEWHITEN_MODE_POS)

#define BBP_RXIQCAL2_ADDR                                  (uint32_t)&(BBP->RXIQCAL2)
#define BBP_RX_IQ_P_ADDR                                   (uint32_t)&(BBP->RXIQCAL2)
#define BBP_RX_IQ_P_POS                                    (0)      /*< bit[15:0]   */
#define BBP_RX_IQ_P_MASK                                   (0xFFFFul << BBP_RX_IQ_P_POS)
#define BBP_RX_IQ_A_ADDR                                   (uint32_t)&(BBP->RXIQCAL2)
#define BBP_RX_IQ_A_POS                                    (16)     /*< bit[31:16]  */
#define BBP_RX_IQ_A_MASK                                   (0xFFFFul << BBP_RX_IQ_A_POS)

#define BBP_GFO_EST_ADDR                                   (uint32_t)&(BBP->GFO_EST)
#define BBP_INT_GFO_ESTSW_ADDR                             (uint32_t)&(BBP->GFO_EST)
#define BBP_INT_GFO_ESTSW_POS                              (0)      /*< bit[11:0]   */
#define BBP_INT_GFO_ESTSW_MASK                             (0xFFFul << BBP_INT_GFO_ESTSW_POS)
#define BBP_INT_GFO_ESTPL_ADDR                             (uint32_t)&(BBP->GFO_EST)
#define BBP_INT_GFO_ESTPL_POS                              (16)     /*< bit[27:16]  */
#define BBP_INT_GFO_ESTPL_MASK                             (0xFFFul << BBP_INT_GFO_ESTPL_POS)

#define BBP_AGC_MODE_ADDR                                  (uint32_t)&(BBP->AGC_MODE)
#define BBP_AGC_MODE_ADDR                                  (uint32_t)&(BBP->AGC_MODE)
#define BBP_AGC_MODE_POS                                   (0)      /*< bit[1:0]    */
#define BBP_AGC_MODE_MASK                                  (0x3ul << BBP_AGC_MODE_POS)
#define BBP_AGC_IDX_CUR_DEBUG_ADDR                         (uint32_t)&(BBP->AGC_MODE)
#define BBP_AGC_IDX_CUR_DEBUG_POS                          (16)     /*< bit[20:16]  */
#define BBP_AGC_IDX_CUR_DEBUG_MASK                         (0x1Ful << BBP_AGC_IDX_CUR_DEBUG_POS)

#define BBP_AGC48_ADDR                                     (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_0_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_0_POS            (0)      /*< bit[1:0]    */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_0_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_0_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_1_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_1_POS            (2)      /*< bit[3:2]    */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_1_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_1_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_2_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_2_POS            (4)      /*< bit[5:4]    */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_2_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_2_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_3_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_3_POS            (6)      /*< bit[7:6]    */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_3_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_3_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_4_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_4_POS            (8)      /*< bit[9:8]    */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_4_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_4_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_5_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_5_POS            (10)     /*< bit[11:10]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_5_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_5_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_6_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_6_POS            (12)     /*< bit[13:12]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_6_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_6_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_7_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_7_POS            (14)     /*< bit[15:14]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_7_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_7_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_8_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_8_POS            (16)     /*< bit[17:16]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_8_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_8_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_9_ADDR           (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_9_POS            (18)     /*< bit[19:18]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_9_MASK           (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_9_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_10_ADDR          (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_10_POS           (20)     /*< bit[21:20]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_10_MASK          (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_10_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_11_ADDR          (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_11_POS           (22)     /*< bit[23:22]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_11_MASK          (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_11_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_12_ADDR          (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_12_POS           (24)     /*< bit[25:24]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_12_MASK          (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_12_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_13_ADDR          (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_13_POS           (26)     /*< bit[27:26]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_13_MASK          (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_13_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_14_ADDR          (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_14_POS           (28)     /*< bit[29:28]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_14_MASK          (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_14_POS)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_15_ADDR          (uint32_t)&(BBP->AGC48)
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_15_POS           (30)     /*< bit[31:30]  */
#define BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_15_MASK          (0x3ul << BBP_AGC_GAIN_TABLE_BBBUFNXT_INDEX_15_POS)

#define BBP_IFEST1_ADDR                                    (uint32_t)&(BBP->IFEST1)
#define BBP_IFEST_ENABLE_ADDR                              (uint32_t)&(BBP->IFEST1)
#define BBP_IFEST_ENABLE_POS                               (0)      /*< bit[0]      */
#define BBP_IFEST_ENABLE_MASK                              (0x1ul << BBP_IFEST_ENABLE_POS)
#define BBP_OMEGA_POS_THRD_1_ADDR                          (uint32_t)&(BBP->IFEST1)
#define BBP_OMEGA_POS_THRD_1_POS                           (1)      /*< bit[12:1]   */
#define BBP_OMEGA_POS_THRD_1_MASK                          (0xFFFul << BBP_OMEGA_POS_THRD_1_POS)
#define BBP_OMEGA_POS_THRD_2_ADDR                          (uint32_t)&(BBP->IFEST1)
#define BBP_OMEGA_POS_THRD_2_POS                           (13)     /*< bit[24:13]  */
#define BBP_OMEGA_POS_THRD_2_MASK                          (0xFFFul << BBP_OMEGA_POS_THRD_2_POS)

#define BBP_IFEST2_ADDR                                    (uint32_t)&(BBP->IFEST2)
#define BBP_OMEGA_POS_THRD_3_ADDR                          (uint32_t)&(BBP->IFEST2)
#define BBP_OMEGA_POS_THRD_3_POS                           (0)      /*< bit[11:0]   */
#define BBP_OMEGA_POS_THRD_3_MASK                          (0xFFFul << BBP_OMEGA_POS_THRD_3_POS)
#define BBP_OMEGA_POS_THRD_4_ADDR                          (uint32_t)&(BBP->IFEST2)
#define BBP_OMEGA_POS_THRD_4_POS                           (12)     /*< bit[23:12]  */
#define BBP_OMEGA_POS_THRD_4_MASK                          (0xFFFul << BBP_OMEGA_POS_THRD_4_POS)

#define BBP_IFEST3_ADDR                                    (uint32_t)&(BBP->IFEST3)
#define BBP_OMEGA_NEG_THRD_1_ADDR                          (uint32_t)&(BBP->IFEST3)
#define BBP_OMEGA_NEG_THRD_1_POS                           (0)      /*< bit[11:0]   */
#define BBP_OMEGA_NEG_THRD_1_MASK                          (0xFFFul << BBP_OMEGA_NEG_THRD_1_POS)
#define BBP_OMEGA_NEG_THRD_2_ADDR                          (uint32_t)&(BBP->IFEST3)
#define BBP_OMEGA_NEG_THRD_2_POS                           (12)     /*< bit[23:12]  */
#define BBP_OMEGA_NEG_THRD_2_MASK                          (0xFFFul << BBP_OMEGA_NEG_THRD_2_POS)

#define BBP_IFEST4_ADDR                                    (uint32_t)&(BBP->IFEST4)
#define BBP_OMEGA_NEG_THRD_3_ADDR                          (uint32_t)&(BBP->IFEST4)
#define BBP_OMEGA_NEG_THRD_3_POS                           (0)      /*< bit[11:0]   */
#define BBP_OMEGA_NEG_THRD_3_MASK                          (0xFFFul << BBP_OMEGA_NEG_THRD_3_POS)
#define BBP_OMEGA_NEG_THRD_4_ADDR                          (uint32_t)&(BBP->IFEST4)
#define BBP_OMEGA_NEG_THRD_4_POS                           (12)     /*< bit[23:12]  */
#define BBP_OMEGA_NEG_THRD_4_MASK                          (0xFFFul << BBP_OMEGA_NEG_THRD_4_POS)

#define BBP_IFEST5_ADDR                                    (uint32_t)&(BBP->IFEST5)
#define BBP_REG_IFSHIFT_OV1_ADDR                           (uint32_t)&(BBP->IFEST5)
#define BBP_REG_IFSHIFT_OV1_POS                            (0)      /*< bit[12:0]   */
#define BBP_REG_IFSHIFT_OV1_MASK                           (0x1FFFul << BBP_REG_IFSHIFT_OV1_POS)
#define BBP_REG_IFSHIFT_OV2_ADDR                           (uint32_t)&(BBP->IFEST5)
#define BBP_REG_IFSHIFT_OV2_POS                            (13)     /*< bit[25:13]  */
#define BBP_REG_IFSHIFT_OV2_MASK                           (0x1FFFul << BBP_REG_IFSHIFT_OV2_POS)

#define BBP_IFEST6_ADDR                                    (uint32_t)&(BBP->IFEST6)
#define BBP_REG_IFSHIFT_OV3_ADDR                           (uint32_t)&(BBP->IFEST6)
#define BBP_REG_IFSHIFT_OV3_POS                            (0)      /*< bit[12:0]   */
#define BBP_REG_IFSHIFT_OV3_MASK                           (0x1FFFul << BBP_REG_IFSHIFT_OV3_POS)
#define BBP_REG_IFSHIFT_OV4_ADDR                           (uint32_t)&(BBP->IFEST6)
#define BBP_REG_IFSHIFT_OV4_POS                            (13)     /*< bit[25:13]  */
#define BBP_REG_IFSHIFT_OV4_MASK                           (0x1FFFul << BBP_REG_IFSHIFT_OV4_POS)

#define BBP_IFEST7_ADDR                                    (uint32_t)&(BBP->IFEST7)
#define BBP_REG_IFSHIFT_UD1_ADDR                           (uint32_t)&(BBP->IFEST7)
#define BBP_REG_IFSHIFT_UD1_POS                            (0)      /*< bit[12:0]   */
#define BBP_REG_IFSHIFT_UD1_MASK                           (0x1FFFul << BBP_REG_IFSHIFT_UD1_POS)
#define BBP_REG_IFSHIFT_UD2_ADDR                           (uint32_t)&(BBP->IFEST7)
#define BBP_REG_IFSHIFT_UD2_POS                            (13)     /*< bit[25:13]  */
#define BBP_REG_IFSHIFT_UD2_MASK                           (0x1FFFul << BBP_REG_IFSHIFT_UD2_POS)

#define BBP_IFEST8_ADDR                                    (uint32_t)&(BBP->IFEST8)
#define BBP_REG_IFSHIFT_UD3_ADDR                           (uint32_t)&(BBP->IFEST8)
#define BBP_REG_IFSHIFT_UD3_POS                            (0)      /*< bit[12:0]   */
#define BBP_REG_IFSHIFT_UD3_MASK                           (0x1FFFul << BBP_REG_IFSHIFT_UD3_POS)
#define BBP_REG_IFSHIFT_UD4_ADDR                           (uint32_t)&(BBP->IFEST8)
#define BBP_REG_IFSHIFT_UD4_POS                            (13)     /*< bit[25:13]  */
#define BBP_REG_IFSHIFT_UD4_MASK                           (0x1FFFul << BBP_REG_IFSHIFT_UD4_POS)

#define BBP_FSC_ADDR                                       (uint32_t)&(BBP->FSC)
#define BBP_FO_ESTIM_SCALER_EN_ADDR                        (uint32_t)&(BBP->FSC)
#define BBP_FO_ESTIM_SCALER_EN_POS                         (0)      /*< bit[0]      */
#define BBP_FO_ESTIM_SCALER_EN_MASK                        (0x1ul << BBP_FO_ESTIM_SCALER_EN_POS)
#define BBP_FO_ESTIM_SCALER_MANUAL_ADDR                    (uint32_t)&(BBP->FSC)
#define BBP_FO_ESTIM_SCALER_MANUAL_POS                     (1)      /*< bit[11:1]   */
#define BBP_FO_ESTIM_SCALER_MANUAL_MASK                    (0x7FFul << BBP_FO_ESTIM_SCALER_MANUAL_POS)
#define BBP_IFEST_ASSERT_DELAY_ADDR                        (uint32_t)&(BBP->FSC)
#define BBP_IFEST_ASSERT_DELAY_POS                         (12)     /*< bit[19:12]  */
#define BBP_IFEST_ASSERT_DELAY_MASK                        (0xFFul << BBP_IFEST_ASSERT_DELAY_POS)

#define BBP_CORE52_ADDR                                    (uint32_t)&(BBP->CORE52)
#define BBP_RX_FLUSH_ADDR                                  (uint32_t)&(BBP->CORE52)
#define BBP_RX_FLUSH_POS                                   (0)      /*< bit[0]      */
#define BBP_RX_FLUSH_MASK                                  (0x1ul << BBP_RX_FLUSH_POS)
#define BBP_RX_CFG_SEL_ADDR                                (uint32_t)&(BBP->CORE52)
#define BBP_RX_CFG_SEL_POS                                 (1)      /*< bit[1]      */
#define BBP_RX_CFG_SEL_MASK                                (0x1ul << BBP_RX_CFG_SEL_POS)
#define BBP_BLE_MODE_ADDR                                  (uint32_t)&(BBP->CORE52)
#define BBP_BLE_MODE_POS                                   (2)      /*< bit[2]      */
#define BBP_BLE_MODE_MASK                                  (0x1ul << BBP_BLE_MODE_POS)
#define BBP_TX_FORCE_IQ_ADDR                               (uint32_t)&(BBP->CORE52)
#define BBP_TX_FORCE_IQ_POS                                (3)      /*< bit[3]      */
#define BBP_TX_FORCE_IQ_MASK                               (0x1ul << BBP_TX_FORCE_IQ_POS)
#define BBP_RATE_ADDR                                      (uint32_t)&(BBP->CORE52)
#define BBP_RATE_POS                                       (4)      /*< bit[5:4]    */
#define BBP_RATE_MASK                                      (0x3ul << BBP_RATE_POS)
#define BBP_RX_READY_ADDR                                  (uint32_t)&(BBP->CORE52)
#define BBP_RX_READY_POS                                   (6)      /*< bit[6]      */
#define BBP_RX_READY_MASK                                  (0x1ul << BBP_RX_READY_POS)

#define BBP_POP_CLIPPING_ADDR                              (uint32_t)&(BBP->POP_CLIPPING)
#define BBP_POP_ENABLE_DEMOD_ADDR                          (uint32_t)&(BBP->POP_CLIPPING)
#define BBP_POP_ENABLE_DEMOD_POS                           (1)      /*< bit[1]      */
#define BBP_POP_ENABLE_DEMOD_MASK                          (0x1ul << BBP_POP_ENABLE_DEMOD_POS)
#define BBP_AGC_CLR_CLIP_ALARM_DLY_TIME_ADDR               (uint32_t)&(BBP->POP_CLIPPING)
#define BBP_AGC_CLR_CLIP_ALARM_DLY_TIME_POS                (4)      /*< bit[11:4]   */
#define BBP_AGC_CLR_CLIP_ALARM_DLY_TIME_MASK               (0xFFul << BBP_AGC_CLR_CLIP_ALARM_DLY_TIME_POS)
#define BBP_POP_RESET_EN_ADDR                              (uint32_t)&(BBP->POP_CLIPPING)
#define BBP_POP_RESET_EN_POS                               (12)     /*< bit[12]     */
#define BBP_POP_RESET_EN_MASK                              (0x1ul << BBP_POP_RESET_EN_POS)
#define BBP_AGC_DIG_CLIP_ALARM_ENABLE_ADDR                 (uint32_t)&(BBP->POP_CLIPPING)
#define BBP_AGC_DIG_CLIP_ALARM_ENABLE_POS                  (13)     /*< bit[13]     */
#define BBP_AGC_DIG_CLIP_ALARM_ENABLE_MASK                 (0x1ul << BBP_AGC_DIG_CLIP_ALARM_ENABLE_POS)
#define BBP_AGC_ANA_CLIP_ALARM_ENABLE_ADDR                 (uint32_t)&(BBP->POP_CLIPPING)
#define BBP_AGC_ANA_CLIP_ALARM_ENABLE_POS                  (14)     /*< bit[14]     */
#define BBP_AGC_ANA_CLIP_ALARM_ENABLE_MASK                 (0x1ul << BBP_AGC_ANA_CLIP_ALARM_ENABLE_POS)
#define BBP_AGC_RF_OVERFLOW_FLAG_ENABLE_ADDR               (uint32_t)&(BBP->POP_CLIPPING)
#define BBP_AGC_RF_OVERFLOW_FLAG_ENABLE_POS                (15)     /*< bit[15]     */
#define BBP_AGC_RF_OVERFLOW_FLAG_ENABLE_MASK               (0x1ul << BBP_AGC_RF_OVERFLOW_FLAG_ENABLE_POS)
#define BBP_TO_RESET_EN_ADDR                               (uint32_t)&(BBP->POP_CLIPPING)
#define BBP_TO_RESET_EN_POS                                (16)     /*< bit[16]     */
#define BBP_TO_RESET_EN_MASK                               (0x1ul << BBP_TO_RESET_EN_POS)

#define BBP_MBUS1_ADDR                                     (uint32_t)&(BBP->MBUS1)
#define BBP_COUNT_CLK_NUM_ADDR                             (uint32_t)&(BBP->MBUS1)
#define BBP_COUNT_CLK_NUM_POS                              (0)      /*< bit[12:0]   */
#define BBP_COUNT_CLK_NUM_MASK                             (0x1FFFul << BBP_COUNT_CLK_NUM_POS)
#define BBP_COARSE_EST_ENABLE_ADDR                         (uint32_t)&(BBP->MBUS1)
#define BBP_COARSE_EST_ENABLE_POS                          (26)     /*< bit[26]     */
#define BBP_COARSE_EST_ENABLE_MASK                         (0x1ul << BBP_COARSE_EST_ENABLE_POS)
#define BBP_TRACKING_ENABLE_ADDR                           (uint32_t)&(BBP->MBUS1)
#define BBP_TRACKING_ENABLE_POS                            (27)     /*< bit[27]     */
#define BBP_TRACKING_ENABLE_MASK                           (0x1ul << BBP_TRACKING_ENABLE_POS)
#define BBP_SAMPLE_ALIGN_ENABLE_ADDR                       (uint32_t)&(BBP->MBUS1)
#define BBP_SAMPLE_ALIGN_ENABLE_POS                        (28)     /*< bit[28]     */
#define BBP_SAMPLE_ALIGN_ENABLE_MASK                       (0x1ul << BBP_SAMPLE_ALIGN_ENABLE_POS)
#define BBP_MBUS_MODE_ADDR                                 (uint32_t)&(BBP->MBUS1)
#define BBP_MBUS_MODE_POS                                  (29)     /*< bit[30:29]  */
#define BBP_MBUS_MODE_MASK                                 (0x3ul << BBP_MBUS_MODE_POS)
#define BBP_CDR_START_SWITCH_ADDR                          (uint32_t)&(BBP->MBUS1)
#define BBP_CDR_START_SWITCH_POS                           (31)     /*< bit[31]     */
#define BBP_CDR_START_SWITCH_MASK                          (0x1ul << BBP_CDR_START_SWITCH_POS)

#define BBP_MBUS2_ADDR                                     (uint32_t)&(BBP->MBUS2)
#define BBP_NUM_OF_EDGE_FOR_COARSE_ADDR                    (uint32_t)&(BBP->MBUS2)
#define BBP_NUM_OF_EDGE_FOR_COARSE_POS                     (0)      /*< bit[1:0]    */
#define BBP_NUM_OF_EDGE_FOR_COARSE_MASK                    (0x3ul << BBP_NUM_OF_EDGE_FOR_COARSE_POS)
#define BBP_NUM_OF_SYMBOL_FOR_TRACKING_ADDR                (uint32_t)&(BBP->MBUS2)
#define BBP_NUM_OF_SYMBOL_FOR_TRACKING_POS                 (4)      /*< bit[6:4]    */
#define BBP_NUM_OF_SYMBOL_FOR_TRACKING_MASK                (0x7ul << BBP_NUM_OF_SYMBOL_FOR_TRACKING_POS)

#define BBP_MBUS3_ADDR                                     (uint32_t)&(BBP->MBUS3)
#define BBP_ALPHA_ADDR                                     (uint32_t)&(BBP->MBUS3)
#define BBP_ALPHA_POS                                      (0)      /*< bit[15:0]   */
#define BBP_ALPHA_MASK                                     (0xFFFFul << BBP_ALPHA_POS)
#define BBP_BETA_ADDR                                      (uint32_t)&(BBP->MBUS3)
#define BBP_BETA_POS                                       (16)     /*< bit[25:16]  */
#define BBP_BETA_MASK                                      (0x3FFul << BBP_BETA_POS)

#define BBP_MBUS4_ADDR                                     (uint32_t)&(BBP->MBUS4)
#define BBP_COARSE_LATENCY_ADDR                            (uint32_t)&(BBP->MBUS4)
#define BBP_COARSE_LATENCY_POS                             (0)      /*< bit[7:0]    */
#define BBP_COARSE_LATENCY_MASK                            (0xFFul << BBP_COARSE_LATENCY_POS)
#define BBP_FINE_LATENCY_ADDR                              (uint32_t)&(BBP->MBUS4)
#define BBP_FINE_LATENCY_POS                               (8)      /*< bit[15:8]   */
#define BBP_FINE_LATENCY_MASK                              (0xFFul << BBP_FINE_LATENCY_POS)

#define BBP_MBUS5_ADDR                                     (uint32_t)&(BBP->MBUS5)
#define BBP_NCO_MAPPING_LOWER_BOUND_ADDR                   (uint32_t)&(BBP->MBUS5)
#define BBP_NCO_MAPPING_LOWER_BOUND_POS                    (0)      /*< bit[11:0]   */
#define BBP_NCO_MAPPING_LOWER_BOUND_MASK                   (0xFFFul << BBP_NCO_MAPPING_LOWER_BOUND_POS)
#define BBP_NCO_MAPPING_UPPER_BOUND_ADDR                   (uint32_t)&(BBP->MBUS5)
#define BBP_NCO_MAPPING_UPPER_BOUND_POS                    (16)     /*< bit[27:16]  */
#define BBP_NCO_MAPPING_UPPER_BOUND_MASK                   (0xFFFul << BBP_NCO_MAPPING_UPPER_BOUND_POS)

#define BBP_DIGIT_GAIN_ADDR                                (uint32_t)&(BBP->DIGIT_GAIN)
#define BBP_DIGIT_GAIN_SEL_ADDR                            (uint32_t)&(BBP->DIGIT_GAIN)
#define BBP_DIGIT_GAIN_SEL_POS                             (8)      /*< bit[10:8]   */
#define BBP_DIGIT_GAIN_SEL_MASK                            (0x7ul << BBP_DIGIT_GAIN_SEL_POS)

#define BBP_SCALE_ADDR                                     (uint32_t)&(BBP->SCALE)
#define BBP_OMEGA_RAW_SCALE_ADDR                           (uint32_t)&(BBP->SCALE)
#define BBP_OMEGA_RAW_SCALE_POS                            (0)      /*< bit[1:0]    */
#define BBP_OMEGA_RAW_SCALE_MASK                           (0x3ul << BBP_OMEGA_RAW_SCALE_POS)

#define BBP_OOK_ADDR                                       (uint32_t)&(BBP->OOK)
#define BBP_OOK_ED_THR_ADDR                                (uint32_t)&(BBP->OOK)
#define BBP_OOK_ED_THR_POS                                 (0)      /*< bit[15:0]   */
#define BBP_OOK_ED_THR_MASK                                (0xFFFFul << BBP_OOK_ED_THR_POS)
#define BBP_OOK_SELECT_ADDR                                (uint32_t)&(BBP->OOK)
#define BBP_OOK_SELECT_POS                                 (16)     /*< bit[16]     */
#define BBP_OOK_SELECT_MASK                                (0x1ul << BBP_OOK_SELECT_POS)
#define BBP_OOK_IQ_SEL_ADDR                                (uint32_t)&(BBP->OOK)
#define BBP_OOK_IQ_SEL_POS                                 (20)     /*< bit[20]     */
#define BBP_OOK_IQ_SEL_MASK                                (0x1ul << BBP_OOK_IQ_SEL_POS)


#define BBP_BASE                                           0x40028000UL
#define BBP                                                ((BBP_T *)BBP_BASE)

/* ================================================================================
 */
/* ================                   BBP_BPSK_T ================ */
/* ================================================================================
 */

/**
 * @brief BBP_BPSK (BBP_BPSK_T)
 */

typedef struct { /*!< BBP_BPSK_T Structure */
    __I uint32_t RESERVED0;
    __IO uint32_t
        BPSK_TX_FILTER_CFG0; /*!< BPSK_TX_FILTER_CFG0 Register (0x004) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG1; /*!< BPSK_TX_FILTER_CFG1 Register (0x008) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG2; /*!< BPSK_TX_FILTER_CFG2 Register (0x00C) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG3; /*!< BPSK_TX_FILTER_CFG3 Register (0x010) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG4; /*!< BPSK_TX_FILTER_CFG4 Register (0x014) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG5; /*!< BPSK_TX_FILTER_CFG5 Register (0x018) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG6; /*!< BPSK_TX_FILTER_CFG6 Register (0x01C) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG7; /*!< BPSK_TX_FILTER_CFG7 Register (0x020) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG8; /*!< BPSK_TX_FILTER_CFG8 Register (0x024) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG9; /*!< BPSK_TX_FILTER_CFG9 Register (0x028) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG10; /*!< BPSK_TX_FILTER_CFG10 Register (0x02C) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG11; /*!< BPSK_TX_FILTER_CFG11 Register (0x030) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG12; /*!< BPSK_TX_FILTER_CFG12 Register (0x034) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG13; /*!< BPSK_TX_FILTER_CFG13 Register (0x038) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG14; /*!< BPSK_TX_FILTER_CFG14 Register (0x03C) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG15; /*!< BPSK_TX_FILTER_CFG15 Register (0x040) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG16; /*!< BPSK_TX_FILTER_CFG16 Register (0x044) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG17; /*!< BPSK_TX_FILTER_CFG17 Register (0x048) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG18; /*!< BPSK_TX_FILTER_CFG18 Register (0x04C) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG19; /*!< BPSK_TX_FILTER_CFG19 Register (0x050) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG20; /*!< BPSK_TX_FILTER_CFG20 Register (0x054) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG21; /*!< BPSK_TX_FILTER_CFG21 Register (0x058) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG22; /*!< BPSK_TX_FILTER_CFG22 Register (0x05C) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG23; /*!< BPSK_TX_FILTER_CFG23 Register (0x060) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG24; /*!< BPSK_TX_FILTER_CFG24 Register (0x064) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG25; /*!< BPSK_TX_FILTER_CFG25 Register (0x068) */
    __IO uint32_t
        BPSK_TX_FILTER_CFG26; /*!< BPSK_TX_FILTER_CFG26 Register (0x06C) */
    __I uint32_t RESERVED1[100];
    __IO uint32_t BPSK_RX_CTRL1;     /*!< BPSK_RX_CTRL1 Register (0x200)     */
    __IO uint32_t BPSK_RX_CTRL2;     /*!< BPSK_RX_CTRL2 Register (0x204)     */
    __IO uint32_t BPSK_RX_FCFO;      /*!< BPSK_RX_FCFO Register (0x208)      */
    __IO uint32_t BPSK_RX_TR;        /*!< BPSK_RX_TR Register (0x20C)        */
    __IO uint32_t BPSK_RX_STATUS1;   /*!< BPSK_RX_STATUS1 Register (0x210)   */
    __IO uint32_t BPSK_RX_STATUS2;   /*!< BPSK_RX_STATUS2 Register (0x214)   */
    __IO uint32_t BPSK_RX_TR_SETTLE; /*!< BPSK_RX_TR_SETTLE Register (0x218) */
    __I uint32_t RESERVED2[881];
    __I uint32_t BPSK_ID_LEN_REG; /*!< BPSK_ID_LEN_REG Register (0xFE0) */
    __I uint32_t RESERVED3[3];
    __IO uint32_t BPSK_ID0_REG; /*!< BPSK_ID0_REG Register (0xFF0) */
} BBP_BPSK_T;

#define BBP_BPSK_BPSK_TX_FILTER_CFG0_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG0)
#define BBP_BPSK_TX_SRRC_SCALE_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG0)
#define BBP_BPSK_TX_SRRC_SCALE_POS (0) /*< bit[2:0]    */
#define BBP_BPSK_TX_SRRC_SCALE_MASK (0x7ul << BBP_BPSK_TX_SRRC_SCALE_POS)
#define BBP_BPSK_TX_CIC_SCALE_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG0)
#define BBP_BPSK_TX_CIC_SCALE_POS (4) /*< bit[7:4]    */
#define BBP_BPSK_TX_CIC_SCALE_MASK (0xFul << BBP_BPSK_TX_CIC_SCALE_POS)
#define BBP_BPSK_TX_CIC_MUL_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG0)
#define BBP_BPSK_TX_CIC_MUL_POS (8) /*< bit[15:8]   */
#define BBP_BPSK_TX_CIC_MUL_MASK (0xFFul << BBP_BPSK_TX_CIC_MUL_POS)
#define BBP_BPSK_TX_CIC_RATE_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG0)
#define BBP_BPSK_TX_CIC_RATE_POS (16) /*< bit[21:16]  */
#define BBP_BPSK_TX_CIC_RATE_MASK (0x3Ful << BBP_BPSK_TX_CIC_RATE_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG1_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG1)
#define BBP_BPSK_TX_SRRC_COEF0_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG1)
#define BBP_BPSK_TX_SRRC_COEF0_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF0_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF0_POS)
#define BBP_BPSK_TX_SRRC_COEF1_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG1)
#define BBP_BPSK_TX_SRRC_COEF1_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF1_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF1_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG2_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG2)
#define BBP_BPSK_TX_SRRC_COEF2_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG2)
#define BBP_BPSK_TX_SRRC_COEF2_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF2_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF2_POS)
#define BBP_BPSK_TX_SRRC_COEF3_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG2)
#define BBP_BPSK_TX_SRRC_COEF3_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF3_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF3_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG3_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG3)
#define BBP_BPSK_TX_SRRC_COEF4_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG3)
#define BBP_BPSK_TX_SRRC_COEF4_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF4_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF4_POS)
#define BBP_BPSK_TX_SRRC_COEF5_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG3)
#define BBP_BPSK_TX_SRRC_COEF5_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF5_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF5_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG4_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG4)
#define BBP_BPSK_TX_SRRC_COEF6_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG4)
#define BBP_BPSK_TX_SRRC_COEF6_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF6_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF6_POS)
#define BBP_BPSK_TX_SRRC_COEF7_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG4)
#define BBP_BPSK_TX_SRRC_COEF7_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF7_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF7_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG5_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG5)
#define BBP_BPSK_TX_SRRC_COEF8_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG5)
#define BBP_BPSK_TX_SRRC_COEF8_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF8_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF8_POS)
#define BBP_BPSK_TX_SRRC_COEF9_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG5)
#define BBP_BPSK_TX_SRRC_COEF9_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF9_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF9_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG6_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG6)
#define BBP_BPSK_TX_SRRC_COEF10_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG6)
#define BBP_BPSK_TX_SRRC_COEF10_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF10_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF10_POS)
#define BBP_BPSK_TX_SRRC_COEF11_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG6)
#define BBP_BPSK_TX_SRRC_COEF11_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF11_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF11_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG7_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG7)
#define BBP_BPSK_TX_SRRC_COEF12_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG7)
#define BBP_BPSK_TX_SRRC_COEF12_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF12_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF12_POS)
#define BBP_BPSK_TX_SRRC_COEF13_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG7)
#define BBP_BPSK_TX_SRRC_COEF13_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF13_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF13_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG8_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG8)
#define BBP_BPSK_TX_SRRC_COEF14_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG8)
#define BBP_BPSK_TX_SRRC_COEF14_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF14_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF14_POS)
#define BBP_BPSK_TX_SRRC_COEF15_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG8)
#define BBP_BPSK_TX_SRRC_COEF15_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF15_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF15_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG9_ADDR                                      \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG9)
#define BBP_BPSK_TX_SRRC_COEF16_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG9)
#define BBP_BPSK_TX_SRRC_COEF16_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF16_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF16_POS)
#define BBP_BPSK_TX_SRRC_COEF17_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG9)
#define BBP_BPSK_TX_SRRC_COEF17_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF17_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF17_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG10_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG10)
#define BBP_BPSK_TX_SRRC_COEF18_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG10)
#define BBP_BPSK_TX_SRRC_COEF18_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF18_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF18_POS)
#define BBP_BPSK_TX_SRRC_COEF19_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG10)
#define BBP_BPSK_TX_SRRC_COEF19_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF19_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF19_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG11_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG11)
#define BBP_BPSK_TX_SRRC_COEF20_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG11)
#define BBP_BPSK_TX_SRRC_COEF20_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF20_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF20_POS)
#define BBP_BPSK_TX_SRRC_COEF21_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG11)
#define BBP_BPSK_TX_SRRC_COEF21_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF21_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF21_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG12_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG12)
#define BBP_BPSK_TX_SRRC_COEF22_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG12)
#define BBP_BPSK_TX_SRRC_COEF22_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF22_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF22_POS)
#define BBP_BPSK_TX_SRRC_COEF23_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG12)
#define BBP_BPSK_TX_SRRC_COEF23_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF23_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF23_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG13_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG13)
#define BBP_BPSK_TX_SRRC_COEF24_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG13)
#define BBP_BPSK_TX_SRRC_COEF24_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF24_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF24_POS)
#define BBP_BPSK_TX_SRRC_COEF25_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG13)
#define BBP_BPSK_TX_SRRC_COEF25_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF25_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF25_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG14_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG14)
#define BBP_BPSK_TX_SRRC_COEF26_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG14)
#define BBP_BPSK_TX_SRRC_COEF26_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF26_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF26_POS)
#define BBP_BPSK_TX_SRRC_COEF27_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG14)
#define BBP_BPSK_TX_SRRC_COEF27_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF27_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF27_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG15_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG15)
#define BBP_BPSK_TX_SRRC_COEF28_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG15)
#define BBP_BPSK_TX_SRRC_COEF28_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF28_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF28_POS)
#define BBP_BPSK_TX_SRRC_COEF29_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG15)
#define BBP_BPSK_TX_SRRC_COEF29_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF29_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF29_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG16_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG16)
#define BBP_BPSK_TX_SRRC_COEF30_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG16)
#define BBP_BPSK_TX_SRRC_COEF30_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF30_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF30_POS)
#define BBP_BPSK_TX_SRRC_COEF31_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG16)
#define BBP_BPSK_TX_SRRC_COEF31_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF31_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF31_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG17_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG17)
#define BBP_BPSK_TX_SRRC_COEF32_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG17)
#define BBP_BPSK_TX_SRRC_COEF32_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF32_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF32_POS)
#define BBP_BPSK_TX_SRRC_COEF33_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG17)
#define BBP_BPSK_TX_SRRC_COEF33_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF33_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF33_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG18_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG18)
#define BBP_BPSK_TX_SRRC_COEF34_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG18)
#define BBP_BPSK_TX_SRRC_COEF34_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF34_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF34_POS)
#define BBP_BPSK_TX_SRRC_COEF35_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG18)
#define BBP_BPSK_TX_SRRC_COEF35_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF35_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF35_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG19_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG19)
#define BBP_BPSK_TX_SRRC_COEF36_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG19)
#define BBP_BPSK_TX_SRRC_COEF36_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF36_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF36_POS)
#define BBP_BPSK_TX_SRRC_COEF37_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG19)
#define BBP_BPSK_TX_SRRC_COEF37_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF37_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF37_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG20_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG20)
#define BBP_BPSK_TX_SRRC_COEF38_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG20)
#define BBP_BPSK_TX_SRRC_COEF38_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF38_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF38_POS)
#define BBP_BPSK_TX_SRRC_COEF39_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG20)
#define BBP_BPSK_TX_SRRC_COEF39_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF39_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF39_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG21_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG21)
#define BBP_BPSK_TX_SRRC_COEF40_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG21)
#define BBP_BPSK_TX_SRRC_COEF40_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF40_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF40_POS)
#define BBP_BPSK_TX_SRRC_COEF41_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG21)
#define BBP_BPSK_TX_SRRC_COEF41_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF41_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF41_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG22_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG22)
#define BBP_BPSK_TX_SRRC_COEF42_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG22)
#define BBP_BPSK_TX_SRRC_COEF42_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF42_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF42_POS)
#define BBP_BPSK_TX_SRRC_COEF43_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG22)
#define BBP_BPSK_TX_SRRC_COEF43_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF43_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF43_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG23_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG23)
#define BBP_BPSK_TX_SRRC_COEF44_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG23)
#define BBP_BPSK_TX_SRRC_COEF44_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF44_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF44_POS)
#define BBP_BPSK_TX_SRRC_COEF45_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG23)
#define BBP_BPSK_TX_SRRC_COEF45_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF45_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF45_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG24_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG24)
#define BBP_BPSK_TX_SRRC_COEF46_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG24)
#define BBP_BPSK_TX_SRRC_COEF46_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF46_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF46_POS)
#define BBP_BPSK_TX_SRRC_COEF47_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG24)
#define BBP_BPSK_TX_SRRC_COEF47_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF47_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF47_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG25_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG25)
#define BBP_BPSK_TX_SRRC_COEF48_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG25)
#define BBP_BPSK_TX_SRRC_COEF48_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF48_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF48_POS)
#define BBP_BPSK_TX_SRRC_COEF49_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG25)
#define BBP_BPSK_TX_SRRC_COEF49_POS (16) /*< bit[27:16]  */
#define BBP_BPSK_TX_SRRC_COEF49_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF49_POS)

#define BBP_BPSK_BPSK_TX_FILTER_CFG26_ADDR                                     \
    (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG26)
#define BBP_BPSK_TX_SRRC_COEF50_ADDR (uint32_t)&(BBP_BPSK->BPSK_TX_FILTER_CFG26)
#define BBP_BPSK_TX_SRRC_COEF50_POS (0) /*< bit[11:0]   */
#define BBP_BPSK_TX_SRRC_COEF50_MASK (0xFFFul << BBP_BPSK_TX_SRRC_COEF50_POS)

#define BBP_BPSK_BPSK_RX_CTRL1_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL1)
#define BBP_BPSK_TIMEOUT_THD_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL1)
#define BBP_BPSK_TIMEOUT_THD_POS (0) /*< bit[12:0]   */
#define BBP_BPSK_TIMEOUT_THD_MASK (0x1FFFul << BBP_BPSK_TIMEOUT_THD_POS)
#define BBP_BPSK_FILTER_LATENCY_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL1)
#define BBP_BPSK_FILTER_LATENCY_POS (13) /*< bit[17:13]  */
#define BBP_BPSK_FILTER_LATENCY_MASK (0x1Ful << BBP_BPSK_FILTER_LATENCY_POS)
#define BBP_BPSK_RSSI_MODE_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL1)
#define BBP_BPSK_RSSI_MODE_POS (18) /*< bit[18]     */
#define BBP_BPSK_RSSI_MODE_MASK (0x1ul << BBP_BPSK_RSSI_MODE_POS)
#define BBP_BPSK_FILTER_BYPASS_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL1)
#define BBP_BPSK_FILTER_BYPASS_POS (19) /*< bit[19]     */
#define BBP_BPSK_FILTER_BYPASS_MASK (0x1ul << BBP_BPSK_FILTER_BYPASS_POS)
#define BBP_BPSK_DAGC_BYPASS_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL1)
#define BBP_BPSK_DAGC_BYPASS_POS (20) /*< bit[20]     */
#define BBP_BPSK_DAGC_BYPASS_MASK (0x1ul << BBP_BPSK_DAGC_BYPASS_POS)
#define BBP_BPSK_PAYLOAD2MAC_DONE_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL1)
#define BBP_BPSK_PAYLOAD2MAC_DONE_POS (21) /*< bit[21]     */
#define BBP_BPSK_PAYLOAD2MAC_DONE_MASK (0x1ul << BBP_BPSK_PAYLOAD2MAC_DONE_POS)
#define BBP_BPSK_RSSI_MEAS_DONE_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL1)
#define BBP_BPSK_RSSI_MEAS_DONE_POS (22) /*< bit[22]     */
#define BBP_BPSK_RSSI_MEAS_DONE_MASK (0x1ul << BBP_BPSK_RSSI_MEAS_DONE_POS)

#define BBP_BPSK_BPSK_RX_CTRL2_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL2)
#define BBP_BPSK_DIG_GAIN_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL2)
#define BBP_BPSK_DIG_GAIN_POS (0) /*< bit[1:0]    */
#define BBP_BPSK_DIG_GAIN_MASK (0x3ul << BBP_BPSK_DIG_GAIN_POS)
#define BBP_BPSK_TARGET_PWR_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL2)
#define BBP_BPSK_TARGET_PWR_POS (2) /*< bit[8:2]    */
#define BBP_BPSK_TARGET_PWR_MASK (0x7Ful << BBP_BPSK_TARGET_PWR_POS)
#define BBP_BPSK_TR_THRESHOLD_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_CTRL2)
#define BBP_BPSK_TR_THRESHOLD_POS (9) /*< bit[22:9]   */
#define BBP_BPSK_TR_THRESHOLD_MASK (0x3FFFul << BBP_BPSK_TR_THRESHOLD_POS)

#define BBP_BPSK_BPSK_RX_FCFO_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_FCFO)
#define BBP_BPSK_K1_FCFO_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_FCFO)
#define BBP_BPSK_K1_FCFO_POS (0) /*< bit[13:0]   */
#define BBP_BPSK_K1_FCFO_MASK (0x3FFFul << BBP_BPSK_K1_FCFO_POS)
#define BBP_BPSK_K2_FCFO_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_FCFO)
#define BBP_BPSK_K2_FCFO_POS (14) /*< bit[27:14]  */
#define BBP_BPSK_K2_FCFO_MASK (0x3FFFul << BBP_BPSK_K2_FCFO_POS)

#define BBP_BPSK_BPSK_RX_TR_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_TR)
#define BBP_BPSK_TR_K1_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_TR)
#define BBP_BPSK_TR_K1_POS (0) /*< bit[13:0]   */
#define BBP_BPSK_TR_K1_MASK (0x3FFFul << BBP_BPSK_TR_K1_POS)
#define BBP_BPSK_TR_K2_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_TR)
#define BBP_BPSK_TR_K2_POS (14) /*< bit[27:14]  */
#define BBP_BPSK_TR_K2_MASK (0x3FFFul << BBP_BPSK_TR_K2_POS)

#define BBP_BPSK_BPSK_RX_STATUS1_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_STATUS1)
#define BBP_BPSK_BPSK_RX_AGC_LOKED_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_STATUS1)
#define BBP_BPSK_BPSK_RX_AGC_LOKED_POS (0) /*< bit[0]      */
#define BBP_BPSK_BPSK_RX_AGC_LOKED_MASK                                        \
    (0x1ul << BBP_BPSK_BPSK_RX_AGC_LOKED_POS)
#define BBP_BPSK_BPSK_RX_SFD_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_STATUS1)
#define BBP_BPSK_BPSK_RX_SFD_POS (1) /*< bit[1]      */
#define BBP_BPSK_BPSK_RX_SFD_MASK (0x1ul << BBP_BPSK_BPSK_RX_SFD_POS)
#define BBP_BPSK_BPSK_RX_CCFO_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_STATUS1)
#define BBP_BPSK_BPSK_RX_CCFO_POS (2) /*< bit[15:2]   */
#define BBP_BPSK_BPSK_RX_CCFO_MASK (0x3FFFul << BBP_BPSK_BPSK_RX_CCFO_POS)

#define BBP_BPSK_BPSK_RX_STATUS2_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_STATUS2)
#define BBP_BPSK_DAGC_GAIN_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_STATUS2)
#define BBP_BPSK_DAGC_GAIN_POS (0) /*< bit[6:0]    */
#define BBP_BPSK_DAGC_GAIN_MASK (0x7Ful << BBP_BPSK_DAGC_GAIN_POS)
#define BBP_BPSK_PWR_STRENGTH_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_STATUS2)
#define BBP_BPSK_PWR_STRENGTH_POS (7) /*< bit[13:7]   */
#define BBP_BPSK_PWR_STRENGTH_MASK (0x7Ful << BBP_BPSK_PWR_STRENGTH_POS)

#define BBP_BPSK_BPSK_RX_TR_SETTLE_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_TR_SETTLE)
#define BBP_BPSK_TR_SETTLE_TIME_ADDR (uint32_t)&(BBP_BPSK->BPSK_RX_TR_SETTLE)
#define BBP_BPSK_TR_SETTLE_TIME_POS (0) /*< bit[8:0]    */
#define BBP_BPSK_TR_SETTLE_TIME_MASK (0x1FFul << BBP_BPSK_TR_SETTLE_TIME_POS)

#define BBP_BPSK_BASE 0x40029000UL
#define BBP_BPSK ((BBP_BPSK_T *)BBP_BPSK_BASE)

#endif

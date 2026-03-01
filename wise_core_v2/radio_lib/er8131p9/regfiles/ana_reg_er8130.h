/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __ANA_REG_ER8130_H__
#define __ANA_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"

/* ================================================================================ */
/* ================                      ANA_T                     ================ */
/* ================================================================================ */

/**
  * @brief ANA (ANA_T)
  */

 typedef struct {                                          /*!< ANA_T Structure                                                            */
  __I  uint32_t RESERVED0[32];
  __IO uint32_t ASARADC_CONFIG_1;                          /*!< ASARADC_CONFIG_1 Register (0x080)                                          */
  __I  uint32_t ASARADC_CONFIG_2;                          /*!< ASARADC_CONFIG_2 Register (0x084)                                          */
  __I  uint32_t ASARADC_CONFIG_3;                          /*!< ASARADC_CONFIG_3 Register (0x088)                                          */
  __IO uint32_t ASARADC_CONFIG_4;                          /*!< ASARADC_CONFIG_4 Register (0x08C)                                          */
  __IO uint32_t ASARADC_CONFIG_5;                          /*!< ASARADC_CONFIG_5 Register (0x090)                                          */
  __IO uint32_t ASARADC_CONFIG_6;                          /*!< ASARADC_CONFIG_6 Register (0x094)                                          */
  __IO uint32_t ASARADC_CONFIG_7;                          /*!< ASARADC_CONFIG_7 Register (0x098)                                          */
  __IO uint32_t ASARADC_CONFIG_8;                          /*!< ASARADC_CONFIG_8 Register (0x09C)                                          */
  __IO uint32_t ASARADC_CONFIG_9;                          /*!< ASARADC_CONFIG_9 Register (0x0A0)                                          */
  __IO uint32_t ASARADC_CONFIG_10;                         /*!< ASARADC_CONFIG_10 Register (0x0A4)                                         */
  __IO uint32_t ASARADC_CONFIG_11;                         /*!< ASARADC_CONFIG_11 Register (0x0A8)                                         */
  __IO uint32_t ASARADC_CONFIG_12;                         /*!< ASARADC_CONFIG_12 Register (0x0AC)                                         */
  __IO uint32_t ASARADC_CONFIG_13;                         /*!< ASARADC_CONFIG_13 Register (0x0B0)                                         */
  __I  uint32_t ASARADC_CONFIG_14;                         /*!< ASARADC_CONFIG_14 Register (0x0B4)                                         */
  __I  uint32_t ASARADC_CONFIG_15;                         /*!< ASARADC_CONFIG_15 Register (0x0B8)                                         */
  __IO uint32_t ASARADC_CONFIG_16;                         /*!< ASARADC_CONFIG_16 Register (0x0BC)                                         */
  __IO uint32_t ASARADC_INT_ST;                            /*!< ASARADC_INT_ST Register (0x0C0)                                            */
  __IO uint32_t ASARADC_INT_EN;                            /*!< ASARADC_INT_EN Register (0x0C4)                                            */
  __I  uint32_t ASARADC_INT;                               /*!< ASARADC_INT Register (0x0C8)                                               */
  __I  uint32_t RESERVED1;
  __IO uint32_t ASARADC_CONFIG_17;                         /*!< ASARADC_CONFIG_17 Register (0x0D0)                                         */
  __I  uint32_t RESERVED2[11];
  __IO uint32_t BIAS_CONFIG1;                              /*!< BIAS_CONFIG1 Register (0x100)                                              */
  __IO uint32_t BIAS_CONFIG2;                              /*!< BIAS_CONFIG2 Register (0x104)                                              */
  __I  uint32_t RESERVED3[30];
  __IO uint32_t CTQBPSD_CONFIG_1;                          /*!< CTQBPSD_CONFIG_1 Register (0x180)                                          */
  __I  uint32_t CTQBPSD_CONFIG_2;                          /*!< CTQBPSD_CONFIG_2 Register (0x184)                                          */
  __I  uint32_t RESERVED4[30];
  __IO uint32_t DCDC_CONFIG_0;                             /*!< DCDC_CONFIG_0 Register (0x200)                                             */
  __IO uint32_t DCDC_CONFIG_1;                             /*!< DCDC_CONFIG_1 Register (0x204)                                             */
  __I  uint32_t RESERVED5[30];
  __IO uint32_t IRC_CPPLL_CONFIG_1;                        /*!< IRC_CPPLL_CONFIG_1 Register (0x280)                                        */
  __IO uint32_t IRC_CPPLL_CONFIG_2;                        /*!< IRC_CPPLL_CONFIG_2 Register (0x284)                                        */
  __IO uint32_t IRC_CPPLL_CONFIG_3;                        /*!< IRC_CPPLL_CONFIG_3 Register (0x288)                                        */
  __IO uint32_t IRC_CPPLL_CONFIG_4;                        /*!< IRC_CPPLL_CONFIG_4 Register (0x28C)                                        */
  __I  uint32_t IRC_CPPLL_CONFIG_5;                        /*!< IRC_CPPLL_CONFIG_5 Register (0x290)                                        */
  __IO uint32_t IRC_CPPLL_CONFIG_6;                        /*!< IRC_CPPLL_CONFIG_6 Register (0x294)                                        */
  __IO uint32_t IRC_CPPLL_CAL_TRIG;                        /*!< IRC_CPPLL_CAL_TRIG Register (0x298)                                        */
  __I  uint32_t RESERVED6[57];
  __IO uint32_t FREQSYNTH_CONFIG_1;                        /*!< FREQSYNTH_CONFIG_1 Register (0x380)                                        */
  __IO uint32_t FREQSYNTH_CONFIG_2;                        /*!< FREQSYNTH_CONFIG_2 Register (0x384)                                        */
  __IO uint32_t FREQSYNTH_CONFIG_3;                        /*!< FREQSYNTH_CONFIG_3 Register (0x388)                                        */
  __IO uint32_t FREQSYNTH_CONFIG_4;                        /*!< FREQSYNTH_CONFIG_4 Register (0x38C)                                        */
  __IO uint32_t FREQSYNTH_CONFIG_5;                        /*!< FREQSYNTH_CONFIG_5 Register (0x390)                                        */
  __IO uint32_t FREQSYNTH_CONFIG_6;                        /*!< FREQSYNTH_CONFIG_6 Register (0x394)                                        */
  __I  uint32_t FREQSYNTH_CONFIG_7;                        /*!< FREQSYNTH_CONFIG_7 Register (0x398)                                        */
  __IO uint32_t FREQSYNTH_TPMCAL_CONFIG_1;                 /*!< FREQSYNTH_TPMCAL_CONFIG_1 Register (0x39C)                                 */
  __IO uint32_t FREQSYNTH_TPMCAL_CONFIG_2;                 /*!< FREQSYNTH_TPMCAL_CONFIG_2 Register (0x3A0)                                 */
  __IO uint32_t FREQSYNTH_TPMCAL_CONFIG_3;                 /*!< FREQSYNTH_TPMCAL_CONFIG_3 Register (0x3A4)                                 */
  __IO uint32_t FREQSYNTH_TPMCAL_CONFIG_4;                 /*!< FREQSYNTH_TPMCAL_CONFIG_4 Register (0x3A8)                                 */
  __I  uint32_t FREQSYNTH_TPMCAL_CONFIG_5;                 /*!< FREQSYNTH_TPMCAL_CONFIG_5 Register (0x3AC)                                 */
  __IO uint32_t TPM_DMI_CONFIG_1;                          /*!< TPM_DMI_CONFIG_1 Register (0x3B0)                                          */
  __IO uint32_t TPM_DMI_CONFIG_2;                          /*!< TPM_DMI_CONFIG_2 Register (0x3B4)                                          */
  __IO uint32_t TPM_DMI_CONFIG_3;                          /*!< TPM_DMI_CONFIG_3 Register (0x3B8)                                          */
  __IO uint32_t TPM_DMI_CONFIG_4;                          /*!< TPM_DMI_CONFIG_4 Register (0x3BC)                                          */
  __I  uint32_t RESERVED7[48];
  __IO uint32_t PA1G_CONFIG_0;                             /*!< PA1G_CONFIG_0 Register (0x480)                                             */
  __IO uint32_t PA1G_CONFIG_1;                             /*!< PA1G_CONFIG_1 Register (0x484)                                             */
  __I  uint32_t RESERVED8[62];
  __IO uint32_t XO_CONFIG_1;                               /*!< XO_CONFIG_1 Register (0x580)                                               */
  __IO uint32_t XO_CONFIG_2;                               /*!< XO_CONFIG_2 Register (0x584)                                               */
  __IO uint32_t XO_CONFIG_3;                               /*!< XO_CONFIG_3 Register (0x588)                                               */
  __I  uint32_t XO_CONFIG_4;                               /*!< XO_CONFIG_4 Register (0x58C)                                               */
  __IO uint32_t GPIO_CONFIG_1;                             /*!< GPIO_CONFIG_1 Register (0x590)                                             */
  __IO uint32_t GPREG_CONFIG_1;                            /*!< GPREG_CONFIG_1 Register (0x594)                                            */
  __IO uint32_t GPREG_CONFIG_2;                            /*!< GPREG_CONFIG_2 Register (0x598)                                            */
  __IO uint32_t GPREG_CONFIG_3;                            /*!< GPREG_CONFIG_3 Register (0x59C)                                            */
  __I  uint32_t RESERVED9[24];
  __IO uint32_t RX_POWSET;                                 /*!< RX_POWSET Register (0x600)                                                 */
  __IO uint32_t RX_GAIN_FILTSET;                           /*!< RX_GAIN_FILTSET Register (0x604)                                           */
  __I  uint32_t RESERVED10[5];
  __I  uint32_t RX_STATUS;                                 /*!< RX_STATUS Register (0x61C)                                                 */
  __IO uint32_t RX_CONFIG_5;                               /*!< RX_CONFIG_5 Register (0x620)                                               */
  __IO uint32_t RX_CONFIG_6;                               /*!< RX_CONFIG_6 Register (0x624)                                               */
  __IO uint32_t RX_CONFIG_7;                               /*!< RX_CONFIG_7 Register (0x628)                                               */
  __IO uint32_t RX_CONFIG_8;                               /*!< RX_CONFIG_8 Register (0x62C)                                               */
  __I  uint32_t RESERVED11[148];
  __IO uint32_t DIG_LF_OSC16K_CONFIG1;                     /*!< DIG_LF_OSC16K_CONFIG1 Register (0x880)                                     */
  __I  uint32_t DIG_LF_OSC16K_CONFIG2;                     /*!< DIG_LF_OSC16K_CONFIG2 Register (0x884)                                     */
  __I  uint32_t RESERVED12[62];
  __IO uint32_t SYNTH_CAL_CONFIG_0;                        /*!< SYNTH_CAL_CONFIG_0 Register (0x980)                                        */
  __IO uint32_t SYNTH_CAL_CONFIG_1;                        /*!< SYNTH_CAL_CONFIG_1 Register (0x984)                                        */
  __IO uint32_t SYNTH_CAL_CONFIG_2;                        /*!< SYNTH_CAL_CONFIG_2 Register (0x988)                                        */
  __IO uint32_t SYNTH_CAL_CONFIG_3;                        /*!< SYNTH_CAL_CONFIG_3 Register (0x98C)                                        */
  __IO uint32_t SYNTH_CAL_TRIG;                            /*!< SYNTH_CAL_TRIG Register (0x990)                                            */
  __IO uint32_t SYNTH_CAL_CONFIG_4;                        /*!< SYNTH_CAL_CONFIG_4 Register (0x994)                                        */
  __IO uint32_t SYNTH_CAL_CONFIG_5;                        /*!< SYNTH_CAL_CONFIG_5 Register (0x998)                                        */
  __IO uint32_t SYNTH_CAL_CONFIG_6;                        /*!< SYNTH_CAL_CONFIG_6 Register (0x99C)                                        */
  __IO uint32_t SYNTH_CAL_CONFIG_7;                        /*!< SYNTH_CAL_CONFIG_7 Register (0x9A0)                                        */
  __IO uint32_t SYNTH_CAL_CONFIG_8;                        /*!< SYNTH_CAL_CONFIG_8 Register (0x9A4)                                        */
  __I  uint32_t RESERVED13[22];
  __IO uint32_t PMU_CTL;                                   /*!< PMU_CTL Register (0xA00)                                                   */
  __I  uint32_t PMU_STS;                                   /*!< PMU_STS Register (0xA04)                                                   */
  __IO uint32_t PMU_CONFIG_0;                              /*!< PMU_CONFIG_0 Register (0xA08)                                              */
  __IO uint32_t PMU_CONFIG_1;                              /*!< PMU_CONFIG_1 Register (0xA0C)                                              */
  __IO uint32_t PMU_CONFIG_2;                              /*!< PMU_CONFIG_2 Register (0xA10)                                              */
  __IO uint32_t PMU_CONFIG_3;                              /*!< PMU_CONFIG_3 Register (0xA14)                                              */
  __IO uint32_t PMU_DBG_REG;                               /*!< PMU_DBG_REG Register (0xA18)                                               */
  __I  uint32_t RESERVED14[249];
  __IO uint32_t ANCTL_CONFIG_0;                            /*!< ANCTL_CONFIG_0 Register (0xE00)                                            */
  __IO uint32_t ANCTL_CONFIG_1;                            /*!< ANCTL_CONFIG_1 Register (0xE04)                                            */
  __IO uint32_t ANCTL_CONFIG_2;                            /*!< ANCTL_CONFIG_2 Register (0xE08)                                            */
  __IO uint32_t ANCTL_CONFIG_3;                            /*!< ANCTL_CONFIG_3 Register (0xE0C)                                            */
  __IO uint32_t ANCTL_CONFIG_4;                            /*!< ANCTL_CONFIG_4 Register (0xE10)                                            */
  __IO uint32_t ANCTL_CONFIG_5;                            /*!< ANCTL_CONFIG_5 Register (0xE14)                                            */
  __IO uint32_t ANCTL_CONFIG_6;                            /*!< ANCTL_CONFIG_6 Register (0xE18)                                            */
  __IO uint32_t ANCTL_CONFIG_7;                            /*!< ANCTL_CONFIG_7 Register (0xE1C)                                            */
  __IO uint32_t ANCTL_CONFIG_7_2;                          /*!< ANCTL_CONFIG_7_2 Register (0xE20)                                          */
  __IO uint32_t ANCTL_CONFIG_7_3;                          /*!< ANCTL_CONFIG_7_3 Register (0xE24)                                          */
  __IO uint32_t ANCTL_CONFIG_7_4;                          /*!< ANCTL_CONFIG_7_4 Register (0xE28)                                          */
  __IO uint32_t ANCTL_CONFIG_VOL_DET;                      /*!< ANCTL_CONFIG_VOL_DET Register (0xE2C)                                      */
  __IO uint32_t ANCTL_CONFIG_7_5;                          /*!< ANCTL_CONFIG_7_5 Register (0xE30)                                          */
  __IO uint32_t ANCTL_CONFIG_7_6;                          /*!< ANCTL_CONFIG_7_6 Register (0xE34)                                          */
  __I  uint32_t RESERVED15;
  __IO uint32_t ANCTL_CONFIG_BBP;                          /*!< ANCTL_CONFIG_BBP Register (0xE3C)                                          */
  __IO uint32_t ANCTL_CONFIG_8;                            /*!< ANCTL_CONFIG_8 Register (0xE40)                                            */
  __IO uint32_t ANCTL_CONFIG_9;                            /*!< ANCTL_CONFIG_9 Register (0xE44)                                            */
  __IO uint32_t ANCTL_CONFIG_10;                           /*!< ANCTL_CONFIG_10 Register (0xE48)                                           */
  __IO uint32_t ANCTL_CONFIG_11;                           /*!< ANCTL_CONFIG_11 Register (0xE4C)                                           */
  __IO uint32_t ANCTL_DCDC_REG0;                           /*!< ANCTL_DCDC_REG0 Register (0xE50)                                           */
  __IO uint32_t ANCTL_DCDC_REG1;                           /*!< ANCTL_DCDC_REG1 Register (0xE54)                                           */
  __IO uint32_t ANCTL_DCDC_REG2;                           /*!< ANCTL_DCDC_REG2 Register (0xE58)                                           */
  __IO uint32_t ANCTL_DCDC_REG3;                           /*!< ANCTL_DCDC_REG3 Register (0xE5C)                                           */
  __IO uint32_t ANCTL_PA1G_REG0;                           /*!< ANCTL_PA1G_REG0 Register (0xE60)                                           */
  __IO uint32_t ANCTL_FS_REG0;                             /*!< ANCTL_FS_REG0 Register (0xE64)                                             */
  __IO uint32_t ANCTL_FS_REG1;                             /*!< ANCTL_FS_REG1 Register (0xE68)                                             */
  __I  uint32_t RESERVED16;
  __IO uint32_t ANCTL_WB_REG0;                             /*!< ANCTL_WB_REG0 Register (0xE70)                                             */
  __I  uint32_t RESERVED17;
  __IO uint32_t ANCTL_OSC16K_REG0;                         /*!< ANCTL_OSC16K_REG0 Register (0xE78)                                         */
  __IO uint32_t ANCTL_OSC16K_REG1;                         /*!< ANCTL_OSC16K_REG1 Register (0xE7C)                                         */
  __IO uint32_t ANCTL_OSC16K_REG2;                         /*!< ANCTL_OSC16K_REG2 Register (0xE80)                                         */
  __I  uint32_t ANCTL_OSC16K_REG3;                         /*!< ANCTL_OSC16K_REG3 Register (0xE84)                                         */
  __I  uint32_t RESERVED18[28];
  __IO uint32_t ANCTL_TEST_REG;                            /*!< ANCTL_TEST_REG Register (0xEF8)                                            */
  __I  uint32_t ANCTL_ST_REG;                              /*!< ANCTL_ST_REG Register (0xEFC)                                              */
  __IO uint32_t ANCTL_CONFIG_40;                           /*!< ANCTL_CONFIG_40 Register (0xF00)                                           */
} ANA_T;

#define ANA_ASARADC_CONFIG_1_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_VIN_MUX_SEL_ADDR                       (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_VIN_MUX_SEL_POS                        (0)      /*< bit[3:0]    */
#define ANA_ASARADC_VIN_MUX_SEL_MASK                       (0xFul << ANA_ASARADC_VIN_MUX_SEL_POS)
#define ANA_ASARADC_ASYNC_CLK_PULWD_ADDR                   (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_ASYNC_CLK_PULWD_POS                    (4)      /*< bit[6:4]    */
#define ANA_ASARADC_ASYNC_CLK_PULWD_MASK                   (0x7ul << ANA_ASARADC_ASYNC_CLK_PULWD_POS)
#define ANA_ASARADC_CKS_PULWD_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_CKS_PULWD_POS                          (7)      /*< bit[9:7]    */
#define ANA_ASARADC_CKS_PULWD_MASK                         (0x7ul << ANA_ASARADC_CKS_PULWD_POS)
#define ANA_ASARADC_CKS_PULSEMODE_EN_ADDR                  (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_CKS_PULSEMODE_EN_POS                   (10)     /*< bit[10]     */
#define ANA_ASARADC_CKS_PULSEMODE_EN_MASK                  (0x1ul << ANA_ASARADC_CKS_PULSEMODE_EN_POS)
#define ANA_ASARADC_RST_N_ADDR                             (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_RST_N_POS                              (11)     /*< bit[11]     */
#define ANA_ASARADC_RST_N_MASK                             (0x1ul << ANA_ASARADC_RST_N_POS)
#define ANA_ASARADC_PD_ADDR                                (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_PD_POS                                 (12)     /*< bit[12]     */
#define ANA_ASARADC_PD_MASK                                (0x1ul << ANA_ASARADC_PD_POS)
#define ANA_ASARADC_LDO_BYP_EN_ADDR                        (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_LDO_BYP_EN_POS                         (13)     /*< bit[13]     */
#define ANA_ASARADC_LDO_BYP_EN_MASK                        (0x1ul << ANA_ASARADC_LDO_BYP_EN_POS)
#define ANA_ASARADC_BUF_EN_ADDR                            (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_BUF_EN_POS                             (14)     /*< bit[14]     */
#define ANA_ASARADC_BUF_EN_MASK                            (0x1ul << ANA_ASARADC_BUF_EN_POS)
#define ANA_ASARADC_LDOVDD_LV_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_LDOVDD_LV_POS                          (15)     /*< bit[15]     */
#define ANA_ASARADC_LDOVDD_LV_MASK                         (0x1ul << ANA_ASARADC_LDOVDD_LV_POS)
#define ANA_ASARADC_STARTUP_EN_ADDR                        (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_STARTUP_EN_POS                         (16)     /*< bit[16]     */
#define ANA_ASARADC_STARTUP_EN_MASK                        (0x1ul << ANA_ASARADC_STARTUP_EN_POS)
#define ANA_ASARADC_AAF_EN_ADDR                            (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_AAF_EN_POS                             (17)     /*< bit[17]     */
#define ANA_ASARADC_AAF_EN_MASK                            (0x1ul << ANA_ASARADC_AAF_EN_POS)
#define ANA_ASARADC_AAF_BW_ADDR                            (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_AAF_BW_POS                             (18)     /*< bit[19:18]  */
#define ANA_ASARADC_AAF_BW_MASK                            (0x3ul << ANA_ASARADC_AAF_BW_POS)
#define ANA_ASARADC_BUF_LDOVDD_EN_ADDR                     (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_BUF_LDOVDD_EN_POS                      (20)     /*< bit[20]     */
#define ANA_ASARADC_BUF_LDOVDD_EN_MASK                     (0x1ul << ANA_ASARADC_BUF_LDOVDD_EN_POS)
#define ANA_ASARADC_LDO_BYPCAP_EN_ADDR                     (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_LDO_BYPCAP_EN_POS                      (21)     /*< bit[21]     */
#define ANA_ASARADC_LDO_BYPCAP_EN_MASK                     (0x1ul << ANA_ASARADC_LDO_BYPCAP_EN_POS)
#define ANA_ASARADC_VIN_MUX_EN_ADDR                        (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_VIN_MUX_EN_POS                         (22)     /*< bit[22]     */
#define ANA_ASARADC_VIN_MUX_EN_MASK                        (0x1ul << ANA_ASARADC_VIN_MUX_EN_POS)
#define ANA_ASARADC_DAF_EN_ADDR                            (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_DAF_EN_POS                             (23)     /*< bit[23]     */
#define ANA_ASARADC_DAF_EN_MASK                            (0x1ul << ANA_ASARADC_DAF_EN_POS)
#define ANA_ASARADC_DAF_AVG_SHIFT_N_ADDR                   (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_DAF_AVG_SHIFT_N_POS                    (24)     /*< bit[27:24]  */
#define ANA_ASARADC_DAF_AVG_SHIFT_N_MASK                   (0xFul << ANA_ASARADC_DAF_AVG_SHIFT_N_POS)
#define ANA_ASARADC_VDDDET_VSLV_SEL_ADDR                   (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_VDDDET_VSLV_SEL_POS                    (28)     /*< bit[30:28]  */
#define ANA_ASARADC_VDDDET_VSLV_SEL_MASK                   (0x7ul << ANA_ASARADC_VDDDET_VSLV_SEL_POS)
#define ANA_ASARADC_VDDDET_EN_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_1)
#define ANA_ASARADC_VDDDET_EN_POS                          (31)     /*< bit[31]     */
#define ANA_ASARADC_VDDDET_EN_MASK                         (0x1ul << ANA_ASARADC_VDDDET_EN_POS)

#define ANA_ASARADC_CONFIG_2_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_2)
#define ANA_ASARADC_DOUT_ADDR                              (uint32_t)&(ANA->ASARADC_CONFIG_2)
#define ANA_ASARADC_DOUT_POS                               (0)      /*< bit[11:0]   */
#define ANA_ASARADC_DOUT_MASK                              (0xFFFul << ANA_ASARADC_DOUT_POS)
#define ANA_ASARADC_TIMING_GOOD_ADDR                       (uint32_t)&(ANA->ASARADC_CONFIG_2)
#define ANA_ASARADC_TIMING_GOOD_POS                        (12)     /*< bit[12]     */
#define ANA_ASARADC_TIMING_GOOD_MASK                       (0x1ul << ANA_ASARADC_TIMING_GOOD_POS)

#define ANA_ASARADC_CONFIG_3_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_3)
#define ANA_ASARADC_HR_RDOUT_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_3)
#define ANA_ASARADC_HR_RDOUT_POS                           (0)      /*< bit[26:0]   */
#define ANA_ASARADC_HR_RDOUT_MASK                          (0x7FFFFFFul << ANA_ASARADC_HR_RDOUT_POS)

#define ANA_ASARADC_CONFIG_4_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_4)
#define ANA_ASARADC_CONFIG_5_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_5)
#define ANA_ASARADC_CONFIG_6_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_6)
#define ANA_ASARADC_CONFIG_7_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_7)
#define ANA_ASARADC_CONFIG_8_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_8)
#define ANA_ASARADC_CONFIG_9_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_9)
#define ANA_ASARADC_CONFIG_10_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_10)
#define ANA_ASARADC_CONFIG_11_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_11)
#define ANA_ASARADC_CONFIG_12_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_12)
#define ANA_ASARADC_DAF_MODE_ADDR                          (uint32_t)&(ANA->ASARADC_CONFIG_12)
#define ANA_ASARADC_DAF_MODE_POS                           (0)      /*< bit[1:0]    */
#define ANA_ASARADC_DAF_MODE_MASK                          (0x3ul << ANA_ASARADC_DAF_MODE_POS)
#define ANA_ASARADC_DAF_AUTOEN_MODE_ADDR                   (uint32_t)&(ANA->ASARADC_CONFIG_12)
#define ANA_ASARADC_DAF_AUTOEN_MODE_POS                    (2)      /*< bit[2]      */
#define ANA_ASARADC_DAF_AUTOEN_MODE_MASK                   (0x1ul << ANA_ASARADC_DAF_AUTOEN_MODE_POS)

#define ANA_ASARADC_CONFIG_13_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_13)
#define ANA_ASARADC_START_ADDR                             (uint32_t)&(ANA->ASARADC_CONFIG_13)
#define ANA_ASARADC_START_POS                              (0)      /*< bit[0]      */
#define ANA_ASARADC_START_MASK                             (0x1ul << ANA_ASARADC_START_POS)
#define ANA_ASARADC_READY_ADDR                             (uint32_t)&(ANA->ASARADC_CONFIG_13)
#define ANA_ASARADC_READY_POS                              (1)      /*< bit[1]      */
#define ANA_ASARADC_READY_MASK                             (0x1ul << ANA_ASARADC_READY_POS)

#define ANA_ASARADC_CONFIG_14_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_14)
#define ANA_ASARADC_DATA_ADDR                              (uint32_t)&(ANA->ASARADC_CONFIG_14)
#define ANA_ASARADC_DATA_POS                               (0)      /*< bit[11:0]   */
#define ANA_ASARADC_DATA_MASK                              (0xFFFul << ANA_ASARADC_DATA_POS)

#define ANA_ASARADC_CONFIG_15_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_15)
#define ANA_ASARADC_HR_DATA_ADDR                           (uint32_t)&(ANA->ASARADC_CONFIG_15)
#define ANA_ASARADC_HR_DATA_POS                            (0)      /*< bit[26:0]   */
#define ANA_ASARADC_HR_DATA_MASK                           (0x7FFFFFFul << ANA_ASARADC_HR_DATA_POS)

#define ANA_ASARADC_CONFIG_16_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_16)
#define ANA_ASARADC_VIN_MUX_SEL_SET_ADDR                   (uint32_t)&(ANA->ASARADC_CONFIG_16)
#define ANA_ASARADC_VIN_MUX_SEL_SET_POS                    (0)      /*< bit[3:0]    */
#define ANA_ASARADC_VIN_MUX_SEL_SET_MASK                   (0xFul << ANA_ASARADC_VIN_MUX_SEL_SET_POS)

#define ANA_ASARADC_INT_ST_ADDR                            (uint32_t)&(ANA->ASARADC_INT_ST)
#define ANA_INT_ST_READY_ADDR                              (uint32_t)&(ANA->ASARADC_INT_ST)
#define ANA_INT_ST_READY_POS                               (0)      /*< bit[0]      */
#define ANA_INT_ST_READY_MASK                              (0x1ul << ANA_INT_ST_READY_POS)

#define ANA_ASARADC_INT_EN_ADDR                            (uint32_t)&(ANA->ASARADC_INT_EN)
#define ANA_INT_EN_READY_ADDR                              (uint32_t)&(ANA->ASARADC_INT_EN)
#define ANA_INT_EN_READY_POS                               (0)      /*< bit[0]      */
#define ANA_INT_EN_READY_MASK                              (0x1ul << ANA_INT_EN_READY_POS)

#define ANA_ASARADC_INT_ADDR                               (uint32_t)&(ANA->ASARADC_INT)
#define ANA_INT_READY_ADDR                                 (uint32_t)&(ANA->ASARADC_INT)
#define ANA_INT_READY_POS                                  (0)      /*< bit[0]      */
#define ANA_INT_READY_MASK                                 (0x1ul << ANA_INT_READY_POS)

#define ANA_ASARADC_CONFIG_17_ADDR                         (uint32_t)&(ANA->ASARADC_CONFIG_17)
#define ANA_FETCH_MODE_ADDR                                (uint32_t)&(ANA->ASARADC_CONFIG_17)
#define ANA_FETCH_MODE_POS                                 (0)      /*< bit[0]      */
#define ANA_FETCH_MODE_MASK                                (0x1ul << ANA_FETCH_MODE_POS)
#define ANA_TIMER_MODE_1_ADDR                              (uint32_t)&(ANA->ASARADC_CONFIG_17)
#define ANA_TIMER_MODE_1_POS                               (1)      /*< bit[2:1]    */
#define ANA_TIMER_MODE_1_MASK                              (0x3ul << ANA_TIMER_MODE_1_POS)
#define ANA_TIMER_MODE_2_ADDR                              (uint32_t)&(ANA->ASARADC_CONFIG_17)
#define ANA_TIMER_MODE_2_POS                               (3)      /*< bit[4:3]    */
#define ANA_TIMER_MODE_2_MASK                              (0x3ul << ANA_TIMER_MODE_2_POS)

#define ANA_BIAS_CONFIG1_ADDR                              (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BGR_OTA_STARTUP_EN_ADDR                        (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BGR_OTA_STARTUP_EN_POS                         (0)      /*< bit[0]      */
#define ANA_BGR_OTA_STARTUP_EN_MASK                        (0x1ul << ANA_BGR_OTA_STARTUP_EN_POS)
#define ANA_BGR_PD_ADDR                                    (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BGR_PD_POS                                     (1)      /*< bit[1]      */
#define ANA_BGR_PD_MASK                                    (0x1ul << ANA_BGR_PD_POS)
#define ANA_BGR_STARTER_ZEROI_EN_ADDR                      (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BGR_STARTER_ZEROI_EN_POS                       (2)      /*< bit[2]      */
#define ANA_BGR_STARTER_ZEROI_EN_MASK                      (0x1ul << ANA_BGR_STARTER_ZEROI_EN_POS)
#define ANA_BOD_LV_ADDR                                    (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BOD_LV_POS                                     (3)      /*< bit[5:3]    */
#define ANA_BOD_LV_MASK                                    (0x7ul << ANA_BOD_LV_POS)
#define ANA_BOD_DISABLE_ADDR                               (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BOD_DISABLE_POS                                (6)      /*< bit[6]      */
#define ANA_BOD_DISABLE_MASK                               (0x1ul << ANA_BOD_DISABLE_POS)
#define ANA_BOD_STARTER_ADDR                               (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BOD_STARTER_POS                                (7)      /*< bit[7]      */
#define ANA_BOD_STARTER_MASK                               (0x1ul << ANA_BOD_STARTER_POS)
#define ANA_DIG_LDO_ILIMIT_EN_ADDR                         (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_LDO_ILIMIT_EN_POS                          (8)      /*< bit[8]      */
#define ANA_DIG_LDO_ILIMIT_EN_MASK                         (0x1ul << ANA_DIG_LDO_ILIMIT_EN_POS)
#define ANA_DIG_LDO_ILIMIT_SEL_ADDR                        (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_LDO_ILIMIT_SEL_POS                         (9)      /*< bit[10:9]   */
#define ANA_DIG_LDO_ILIMIT_SEL_MASK                        (0x3ul << ANA_DIG_LDO_ILIMIT_SEL_POS)
#define ANA_BIAS_ULP_VREFBUF_STARTUP_EN_ADDR               (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BIAS_ULP_VREFBUF_STARTUP_EN_POS                (11)     /*< bit[11]     */
#define ANA_BIAS_ULP_VREFBUF_STARTUP_EN_MASK               (0x1ul << ANA_BIAS_ULP_VREFBUF_STARTUP_EN_POS)
#define ANA_BIAS_ULP_VREFBUF_PD_ADDR                       (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_BIAS_ULP_VREFBUF_PD_POS                        (12)     /*< bit[12]     */
#define ANA_BIAS_ULP_VREFBUF_PD_MASK                       (0x1ul << ANA_BIAS_ULP_VREFBUF_PD_POS)
#define ANA_DIG_LDOVDD_LV_ADDR                             (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_LDOVDD_LV_POS                              (13)     /*< bit[14:13]  */
#define ANA_DIG_LDOVDD_LV_MASK                             (0x3ul << ANA_DIG_LDOVDD_LV_POS)
#define ANA_DIG_LDO_PD_ADDR                                (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_LDO_PD_POS                                 (15)     /*< bit[15]     */
#define ANA_DIG_LDO_PD_MASK                                (0x1ul << ANA_DIG_LDO_PD_POS)
#define ANA_DIG_ULPLDOVDD_LV_ADDR                          (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_ULPLDOVDD_LV_POS                           (16)     /*< bit[19:16]  */
#define ANA_DIG_ULPLDOVDD_LV_MASK                          (0xFul << ANA_DIG_ULPLDOVDD_LV_POS)
#define ANA_DIG_EXT_FLASH_LDOVDD_LV_ADDR                   (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_EXT_FLASH_LDOVDD_LV_POS                    (20)     /*< bit[22:20]  */
#define ANA_DIG_EXT_FLASH_LDOVDD_LV_MASK                   (0x7ul << ANA_DIG_EXT_FLASH_LDOVDD_LV_POS)
#define ANA_DIG_EXT_FLASH_LDO_PD_ADDR                      (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_EXT_FLASH_LDO_PD_POS                       (23)     /*< bit[23]     */
#define ANA_DIG_EXT_FLASH_LDO_PD_MASK                      (0x1ul << ANA_DIG_EXT_FLASH_LDO_PD_POS)
#define ANA_DIG_EXT_FLASH_LDO_SUPEN_ADDR                   (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_EXT_FLASH_LDO_SUPEN_POS                    (24)     /*< bit[24]     */
#define ANA_DIG_EXT_FLASH_LDO_SUPEN_MASK                   (0x1ul << ANA_DIG_EXT_FLASH_LDO_SUPEN_POS)
#define ANA_DIG_ULPLDO_STARTER_N_ADDR                      (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_ULPLDO_STARTER_N_POS                       (25)     /*< bit[25]     */
#define ANA_DIG_ULPLDO_STARTER_N_MASK                      (0x1ul << ANA_DIG_ULPLDO_STARTER_N_POS)
#define ANA_DIG_ULPLDO_VREF_SEL_ADDR                       (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_ULPLDO_VREF_SEL_POS                        (26)     /*< bit[26]     */
#define ANA_DIG_ULPLDO_VREF_SEL_MASK                       (0x1ul << ANA_DIG_ULPLDO_VREF_SEL_POS)
#define ANA_DIG_ULPLDO_0P9V_EN_REG_ADDR                    (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_ULPLDO_0P9V_EN_REG_POS                     (27)     /*< bit[27]     */
#define ANA_DIG_ULPLDO_0P9V_EN_REG_MASK                    (0x1ul << ANA_DIG_ULPLDO_0P9V_EN_REG_POS)
#define ANA_DIG_ULPLDO_0P6V_EN_REG_ADDR                    (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_ULPLDO_0P6V_EN_REG_POS                     (28)     /*< bit[28]     */
#define ANA_DIG_ULPLDO_0P6V_EN_REG_MASK                    (0x1ul << ANA_DIG_ULPLDO_0P6V_EN_REG_POS)
#define ANA_DIG_ULPLDOVDD_EN_MODE_ADDR                     (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_ULPLDOVDD_EN_MODE_POS                      (29)     /*< bit[29]     */
#define ANA_DIG_ULPLDOVDD_EN_MODE_MASK                     (0x1ul << ANA_DIG_ULPLDOVDD_EN_MODE_POS)
#define ANA_DIG_LDOVDD_PG_PD_ADDR                          (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_LDOVDD_PG_PD_POS                           (30)     /*< bit[30]     */
#define ANA_DIG_LDOVDD_PG_PD_MASK                          (0x1ul << ANA_DIG_LDOVDD_PG_PD_POS)
#define ANA_DIG_ULPLDO_0P6V_RLOAD_EN_ADDR                  (uint32_t)&(ANA->BIAS_CONFIG1)
#define ANA_DIG_ULPLDO_0P6V_RLOAD_EN_POS                   (31)     /*< bit[31]     */
#define ANA_DIG_ULPLDO_0P6V_RLOAD_EN_MASK                  (0x1ul << ANA_DIG_ULPLDO_0P6V_RLOAD_EN_POS)

#define ANA_BIAS_CONFIG2_ADDR                              (uint32_t)&(ANA->BIAS_CONFIG2)
#define ANA_DIG_EXT_FLASH_LDO_ILIMIT_EN_ADDR               (uint32_t)&(ANA->BIAS_CONFIG2)
#define ANA_DIG_EXT_FLASH_LDO_ILIMIT_EN_POS                (0)      /*< bit[0]      */
#define ANA_DIG_EXT_FLASH_LDO_ILIMIT_EN_MASK               (0x1ul << ANA_DIG_EXT_FLASH_LDO_ILIMIT_EN_POS)
#define ANA_DIG_EXT_FLASH_LDO_ILIMIT_SEL_ADDR              (uint32_t)&(ANA->BIAS_CONFIG2)
#define ANA_DIG_EXT_FLASH_LDO_ILIMIT_SEL_POS               (1)      /*< bit[2:1]    */
#define ANA_DIG_EXT_FLASH_LDO_ILIMIT_SEL_MASK              (0x3ul << ANA_DIG_EXT_FLASH_LDO_ILIMIT_SEL_POS)

#define ANA_CTQBPSD_CONFIG_1_ADDR                          (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_OTA_SUPEN_ADDR                        (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_OTA_SUPEN_POS                         (0)      /*< bit[0]      */
#define ANA_DASARADC_OTA_SUPEN_MASK                        (0x1ul << ANA_DASARADC_OTA_SUPEN_POS)
#define ANA_DASARADC_LDO_BYP_EN_ADDR                       (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_LDO_BYP_EN_POS                        (1)      /*< bit[1]      */
#define ANA_DASARADC_LDO_BYP_EN_MASK                       (0x1ul << ANA_DASARADC_LDO_BYP_EN_POS)
#define ANA_DASARADC_ILIMIT_EN_ADDR                        (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_ILIMIT_EN_POS                         (2)      /*< bit[2]      */
#define ANA_DASARADC_ILIMIT_EN_MASK                        (0x1ul << ANA_DASARADC_ILIMIT_EN_POS)
#define ANA_DASARADC_ILIMIT_SEL_ADDR                       (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_ILIMIT_SEL_POS                        (3)      /*< bit[4:3]    */
#define ANA_DASARADC_ILIMIT_SEL_MASK                       (0x3ul << ANA_DASARADC_ILIMIT_SEL_POS)
#define ANA_DASARADC_DIV_N_ADDR                            (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_DIV_N_POS                             (5)      /*< bit[9:5]    */
#define ANA_DASARADC_DIV_N_MASK                            (0x1Ful << ANA_DASARADC_DIV_N_POS)
#define ANA_DASARADC_ASYNC_CLK_PULWD_ADDR                  (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_ASYNC_CLK_PULWD_POS                   (10)     /*< bit[12:10]  */
#define ANA_DASARADC_ASYNC_CLK_PULWD_MASK                  (0x7ul << ANA_DASARADC_ASYNC_CLK_PULWD_POS)
#define ANA_DASARADC_CKS_PULWD_ADDR                        (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_CKS_PULWD_POS                         (13)     /*< bit[15:13]  */
#define ANA_DASARADC_CKS_PULWD_MASK                        (0x7ul << ANA_DASARADC_CKS_PULWD_POS)
#define ANA_DASARADC_PD_ADDR                               (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_PD_POS                                (16)     /*< bit[16]     */
#define ANA_DASARADC_PD_MASK                               (0x1ul << ANA_DASARADC_PD_POS)
#define ANA_DASARADC_RST_N_ADDR                            (uint32_t)&(ANA->CTQBPSD_CONFIG_1)
#define ANA_DASARADC_RST_N_POS                             (17)     /*< bit[17]     */
#define ANA_DASARADC_RST_N_MASK                            (0x1ul << ANA_DASARADC_RST_N_POS)

#define ANA_CTQBPSD_CONFIG_2_ADDR                          (uint32_t)&(ANA->CTQBPSD_CONFIG_2)
#define ANA_DASARADC_TIMING_GOOD_I_ADDR                    (uint32_t)&(ANA->CTQBPSD_CONFIG_2)
#define ANA_DASARADC_TIMING_GOOD_I_POS                     (0)      /*< bit[0]      */
#define ANA_DASARADC_TIMING_GOOD_I_MASK                    (0x1ul << ANA_DASARADC_TIMING_GOOD_I_POS)
#define ANA_DASARADC_TIMING_GOOD_Q_ADDR                    (uint32_t)&(ANA->CTQBPSD_CONFIG_2)
#define ANA_DASARADC_TIMING_GOOD_Q_POS                     (1)      /*< bit[1]      */
#define ANA_DASARADC_TIMING_GOOD_Q_MASK                    (0x1ul << ANA_DASARADC_TIMING_GOOD_Q_POS)

#define ANA_DCDC_CONFIG_0_ADDR                             (uint32_t)&(ANA->DCDC_CONFIG_0)
#define ANA_DCDC_PD12_ADDR                                 (uint32_t)&(ANA->DCDC_CONFIG_0)
#define ANA_DCDC_PD12_POS                                  (0)      /*< bit[0]      */
#define ANA_DCDC_PD12_MASK                                 (0x1ul << ANA_DCDC_PD12_POS)
#define ANA_DCDC_BYPASS12_ADDR                             (uint32_t)&(ANA->DCDC_CONFIG_0)
#define ANA_DCDC_BYPASS12_POS                              (1)      /*< bit[1]      */
#define ANA_DCDC_BYPASS12_MASK                             (0x1ul << ANA_DCDC_BYPASS12_POS)
#define ANA_DCDC_EXTRA12_CP_ADDR                           (uint32_t)&(ANA->DCDC_CONFIG_0)
#define ANA_DCDC_EXTRA12_CP_POS                            (7)      /*< bit[8:7]    */
#define ANA_DCDC_EXTRA12_CP_MASK                           (0x3ul << ANA_DCDC_EXTRA12_CP_POS)
#define ANA_DCDC_COR_CAL_EN12_ADDR                         (uint32_t)&(ANA->DCDC_CONFIG_0)
#define ANA_DCDC_COR_CAL_EN12_POS                          (25)     /*< bit[25]     */
#define ANA_DCDC_COR_CAL_EN12_MASK                         (0x1ul << ANA_DCDC_COR_CAL_EN12_POS)
#define ANA_DCDC_COR_MAN12_ADDR                            (uint32_t)&(ANA->DCDC_CONFIG_0)
#define ANA_DCDC_COR_MAN12_POS                             (26)     /*< bit[28:26]  */
#define ANA_DCDC_COR_MAN12_MASK                            (0x7ul << ANA_DCDC_COR_MAN12_POS)
#define ANA_DCDC_EN1P5_ADDR                                (uint32_t)&(ANA->DCDC_CONFIG_0)
#define ANA_DCDC_EN1P5_POS                                 (31)     /*< bit[31]     */
#define ANA_DCDC_EN1P5_MASK                                (0x1ul << ANA_DCDC_EN1P5_POS)

#define ANA_DCDC_CONFIG_1_ADDR                             (uint32_t)&(ANA->DCDC_CONFIG_1)
#define ANA_DCDC_SW12_F_ADDR                               (uint32_t)&(ANA->DCDC_CONFIG_1)
#define ANA_DCDC_SW12_F_POS                                (0)      /*< bit[2:0]    */
#define ANA_DCDC_SW12_F_MASK                               (0x7ul << ANA_DCDC_SW12_F_POS)
#define ANA_DCDC_EN_RANDOM12_ADDR                          (uint32_t)&(ANA->DCDC_CONFIG_1)
#define ANA_DCDC_EN_RANDOM12_POS                           (3)      /*< bit[3]      */
#define ANA_DCDC_EN_RANDOM12_MASK                          (0x1ul << ANA_DCDC_EN_RANDOM12_POS)
#define ANA_DCDC_PMOS_ON_BYP_ADDR                          (uint32_t)&(ANA->DCDC_CONFIG_1)
#define ANA_DCDC_PMOS_ON_BYP_POS                           (13)     /*< bit[13]     */
#define ANA_DCDC_PMOS_ON_BYP_MASK                          (0x1ul << ANA_DCDC_PMOS_ON_BYP_POS)
#define ANA_DCDC_VSEL_ADDR                                 (uint32_t)&(ANA->DCDC_CONFIG_1)
#define ANA_DCDC_VSEL_POS                                  (20)     /*< bit[22:20]  */
#define ANA_DCDC_VSEL_MASK                                 (0x7ul << ANA_DCDC_VSEL_POS)
#define ANA_DCDC_EN_IPN_ADDR                               (uint32_t)&(ANA->DCDC_CONFIG_1)
#define ANA_DCDC_EN_IPN_POS                                (23)     /*< bit[24:23]  */
#define ANA_DCDC_EN_IPN_MASK                               (0x3ul << ANA_DCDC_EN_IPN_POS)

#define ANA_IRC_CPPLL_CONFIG_1_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_VCOLDO_BYP_EN_ADDR                   (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_VCOLDO_BYP_EN_POS                    (0)      /*< bit[0]      */
#define ANA_IRC_CPPLL_VCOLDO_BYP_EN_MASK                   (0x1ul << ANA_IRC_CPPLL_VCOLDO_BYP_EN_POS)
#define ANA_IRC_CPPLL_ANGLDO_BYP_EN_ADDR                   (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_ANGLDO_BYP_EN_POS                    (1)      /*< bit[1]      */
#define ANA_IRC_CPPLL_ANGLDO_BYP_EN_MASK                   (0x1ul << ANA_IRC_CPPLL_ANGLDO_BYP_EN_POS)
#define ANA_IRC_CPPLL_PFD_RST_DLY_ADDR                     (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_PFD_RST_DLY_POS                      (2)      /*< bit[3:2]    */
#define ANA_IRC_CPPLL_PFD_RST_DLY_MASK                     (0x3ul << ANA_IRC_CPPLL_PFD_RST_DLY_POS)
#define ANA_IRC_CPPLL_CP_ICTRL_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_CP_ICTRL_POS                         (4)      /*< bit[7:4]    */
#define ANA_IRC_CPPLL_CP_ICTRL_MASK                        (0xFul << ANA_IRC_CPPLL_CP_ICTRL_POS)
#define ANA_IRC_CPPLL_LDOVDD_LV_ADDR                       (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_LDOVDD_LV_POS                        (8)      /*< bit[9:8]    */
#define ANA_IRC_CPPLL_LDOVDD_LV_MASK                       (0x3ul << ANA_IRC_CPPLL_LDOVDD_LV_POS)
#define ANA_IRC_CPPLL_GAINBOOST_ADDR                       (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_GAINBOOST_POS                        (10)     /*< bit[10]     */
#define ANA_IRC_CPPLL_GAINBOOST_MASK                       (0x1ul << ANA_IRC_CPPLL_GAINBOOST_POS)
#define ANA_IRC_CPPLL_RFBUF_ICTRL_ADDR                     (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_RFBUF_ICTRL_POS                      (11)     /*< bit[13:11]  */
#define ANA_IRC_CPPLL_RFBUF_ICTRL_MASK                     (0x7ul << ANA_IRC_CPPLL_RFBUF_ICTRL_POS)
#define ANA_IRC_CPPLL_RFBUF_EN_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_RFBUF_EN_POS                         (14)     /*< bit[14]     */
#define ANA_IRC_CPPLL_RFBUF_EN_MASK                        (0x1ul << ANA_IRC_CPPLL_RFBUF_EN_POS)
#define ANA_IRC_CPPLL_BANDSEL_ADDR                         (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_BANDSEL_POS                          (15)     /*< bit[17:15]  */
#define ANA_IRC_CPPLL_BANDSEL_MASK                         (0x7ul << ANA_IRC_CPPLL_BANDSEL_POS)
#define ANA_IRC_CPPLL_HFB_SEL_ADDR                         (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_HFB_SEL_POS                          (18)     /*< bit[19:18]  */
#define ANA_IRC_CPPLL_HFB_SEL_MASK                         (0x3ul << ANA_IRC_CPPLL_HFB_SEL_POS)
#define ANA_IRC_CPPLL_DIVMM_EXTRANGE_EN_ADDR               (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_DIVMM_EXTRANGE_EN_POS                (20)     /*< bit[20]     */
#define ANA_IRC_CPPLL_DIVMM_EXTRANGE_EN_MASK               (0x1ul << ANA_IRC_CPPLL_DIVMM_EXTRANGE_EN_POS)
#define ANA_IRC_CPPLL_VTUNEDET_VH_CTRL_ADDR                (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_VTUNEDET_VH_CTRL_POS                 (21)     /*< bit[23:21]  */
#define ANA_IRC_CPPLL_VTUNEDET_VH_CTRL_MASK                (0x7ul << ANA_IRC_CPPLL_VTUNEDET_VH_CTRL_POS)
#define ANA_IRC_CPPLL_VTUNEDET_VL_CTRL_ADDR                (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_VTUNEDET_VL_CTRL_POS                 (24)     /*< bit[26:24]  */
#define ANA_IRC_CPPLL_VTUNEDET_VL_CTRL_MASK                (0x7ul << ANA_IRC_CPPLL_VTUNEDET_VL_CTRL_POS)
#define ANA_IRC_CPPLL_KVC_DATA_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_1)
#define ANA_IRC_CPPLL_KVC_DATA_POS                         (27)     /*< bit[31:27]  */
#define ANA_IRC_CPPLL_KVC_DATA_MASK                        (0x1Ful << ANA_IRC_CPPLL_KVC_DATA_POS)

#define ANA_IRC_CPPLL_CONFIG_2_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_AFC_SETTLING_CYCLE_ADDR              (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_AFC_SETTLING_CYCLE_POS               (0)      /*< bit[9:0]    */
#define ANA_IRC_CPPLL_AFC_SETTLING_CYCLE_MASK              (0x3FFul << ANA_IRC_CPPLL_AFC_SETTLING_CYCLE_POS)
#define ANA_IRC_CPPLL_AFC_BSMODE_EN_ADDR                   (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_AFC_BSMODE_EN_POS                    (10)     /*< bit[10]     */
#define ANA_IRC_CPPLL_AFC_BSMODE_EN_MASK                   (0x1ul << ANA_IRC_CPPLL_AFC_BSMODE_EN_POS)
#define ANA_IRC_CPPLL_AFC_EN_ADDR                          (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_AFC_EN_POS                           (11)     /*< bit[11]     */
#define ANA_IRC_CPPLL_AFC_EN_MASK                          (0x1ul << ANA_IRC_CPPLL_AFC_EN_POS)
#define ANA_IRC_CPPLL_DSM_DITHERING_ADDR                   (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_DSM_DITHERING_POS                    (12)     /*< bit[12]     */
#define ANA_IRC_CPPLL_DSM_DITHERING_MASK                   (0x1ul << ANA_IRC_CPPLL_DSM_DITHERING_POS)
#define ANA_IRC_CPPLL_FRACMODE_EN_ADDR                     (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_FRACMODE_EN_POS                      (13)     /*< bit[13]     */
#define ANA_IRC_CPPLL_FRACMODE_EN_MASK                     (0x1ul << ANA_IRC_CPPLL_FRACMODE_EN_POS)
#define ANA_IRC_CPPLL_DSM_DITHERING_BIT_DVALUE_ADDR        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_DSM_DITHERING_BIT_DVALUE_POS         (14)     /*< bit[14]     */
#define ANA_IRC_CPPLL_DSM_DITHERING_BIT_DVALUE_MASK        (0x1ul << ANA_IRC_CPPLL_DSM_DITHERING_BIT_DVALUE_POS)
#define ANA_IRC_CPPLL_DSM_DITHERING_FS_PARM_ADDR           (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_DSM_DITHERING_FS_PARM_POS            (15)     /*< bit[26:15]  */
#define ANA_IRC_CPPLL_DSM_DITHERING_FS_PARM_MASK           (0xFFFul << ANA_IRC_CPPLL_DSM_DITHERING_FS_PARM_POS)
#define ANA_IRC_CPPLL_RST_N_ADDR                           (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_RST_N_POS                            (27)     /*< bit[27]     */
#define ANA_IRC_CPPLL_RST_N_MASK                           (0x1ul << ANA_IRC_CPPLL_RST_N_POS)
#define ANA_IRC_CPPLL_PD_ADDR                              (uint32_t)&(ANA->IRC_CPPLL_CONFIG_2)
#define ANA_IRC_CPPLL_PD_POS                               (28)     /*< bit[28]     */
#define ANA_IRC_CPPLL_PD_MASK                              (0x1ul << ANA_IRC_CPPLL_PD_POS)

#define ANA_IRC_CPPLL_CONFIG_3_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_3)
#define ANA_IRC_CPPLL_DIV_INT_ADDR                         (uint32_t)&(ANA->IRC_CPPLL_CONFIG_3)
#define ANA_IRC_CPPLL_DIV_INT_POS                          (0)      /*< bit[4:0]    */
#define ANA_IRC_CPPLL_DIV_INT_MASK                         (0x1Ful << ANA_IRC_CPPLL_DIV_INT_POS)

#define ANA_IRC_CPPLL_CONFIG_4_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_4)
#define ANA_IRC_CPPLL_DIV_FRAC_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_4)
#define ANA_IRC_CPPLL_DIV_FRAC_POS                         (0)      /*< bit[17:0]   */
#define ANA_IRC_CPPLL_DIV_FRAC_MASK                        (0x3FFFFul << ANA_IRC_CPPLL_DIV_FRAC_POS)

#define ANA_IRC_CPPLL_CONFIG_5_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_5)
#define ANA_IRC_CPPLL_KVC_ROUT_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_5)
#define ANA_IRC_CPPLL_KVC_ROUT_POS                         (0)      /*< bit[4:0]    */
#define ANA_IRC_CPPLL_KVC_ROUT_MASK                        (0x1Ful << ANA_IRC_CPPLL_KVC_ROUT_POS)
#define ANA_IRC_CPPLL_LOCKED_SIG_ADDR                      (uint32_t)&(ANA->IRC_CPPLL_CONFIG_5)
#define ANA_IRC_CPPLL_LOCKED_SIG_POS                       (5)      /*< bit[5]      */
#define ANA_IRC_CPPLL_LOCKED_SIG_MASK                      (0x1ul << ANA_IRC_CPPLL_LOCKED_SIG_POS)

#define ANA_IRC_CPPLL_CONFIG_6_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CONFIG_6)
#define ANA_IRC_CPPLL_CAL_TRIG_ADDR                        (uint32_t)&(ANA->IRC_CPPLL_CAL_TRIG)
#define ANA_IRC_CPPLL_CAL_TRIGGER_ADDR                     (uint32_t)&(ANA->IRC_CPPLL_CAL_TRIG)
#define ANA_IRC_CPPLL_CAL_TRIGGER_POS                      (0)      /*< bit[0]      */
#define ANA_IRC_CPPLL_CAL_TRIGGER_MASK                     (0x1ul << ANA_IRC_CPPLL_CAL_TRIGGER_POS)

#define ANA_FREQSYNTH_CONFIG_1_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_FREQSYNTH_PD_ADDR                           (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_FREQSYNTH_PD_POS                            (0)      /*< bit[0]      */
#define ANA_FS_FREQSYNTH_PD_MASK                           (0x1ul << ANA_FS_FREQSYNTH_PD_POS)
#define ANA_FS_IREFGEN_PD_ADDR                             (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_IREFGEN_PD_POS                              (1)      /*< bit[1]      */
#define ANA_FS_IREFGEN_PD_MASK                             (0x1ul << ANA_FS_IREFGEN_PD_POS)
#define ANA_FS_IREF_ICTRL_ADDR                             (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_IREF_ICTRL_POS                              (2)      /*< bit[3:2]    */
#define ANA_FS_IREF_ICTRL_MASK                             (0x3ul << ANA_FS_IREF_ICTRL_POS)
#define ANA_FS_VCODIV_LDO_BYP_EN_ADDR                      (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_VCODIV_LDO_BYP_EN_POS                       (4)      /*< bit[4]      */
#define ANA_FS_VCODIV_LDO_BYP_EN_MASK                      (0x1ul << ANA_FS_VCODIV_LDO_BYP_EN_POS)
#define ANA_FS_VCODIV_LDO_PD_ADDR                          (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_VCODIV_LDO_PD_POS                           (5)      /*< bit[5]      */
#define ANA_FS_VCODIV_LDO_PD_MASK                          (0x1ul << ANA_FS_VCODIV_LDO_PD_POS)
#define ANA_FS_TPM_LPF_BW_ADDR                             (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_TPM_LPF_BW_POS                              (6)      /*< bit[8:6]    */
#define ANA_FS_TPM_LPF_BW_MASK                             (0x7ul << ANA_FS_TPM_LPF_BW_POS)
#define ANA_FS_TPM_LPF_RES_EN_ADDR                         (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_TPM_LPF_RES_EN_POS                          (9)      /*< bit[9]      */
#define ANA_FS_TPM_LPF_RES_EN_MASK                         (0x1ul << ANA_FS_TPM_LPF_RES_EN_POS)
#define ANA_FS_TPM_LPF_CAP_EN_ADDR                         (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_TPM_LPF_CAP_EN_POS                          (10)     /*< bit[10]     */
#define ANA_FS_TPM_LPF_CAP_EN_MASK                         (0x1ul << ANA_FS_TPM_LPF_CAP_EN_POS)
#define ANA_FS_VCOLDO_LV_ADDR                              (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_VCOLDO_LV_POS                               (11)     /*< bit[12:11]  */
#define ANA_FS_VCOLDO_LV_MASK                              (0x3ul << ANA_FS_VCOLDO_LV_POS)
#define ANA_FS_TPM_CAPARRAY_CTRL_ADDR                      (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_TPM_CAPARRAY_CTRL_POS                       (13)     /*< bit[15:13]  */
#define ANA_FS_TPM_CAPARRAY_CTRL_MASK                      (0x7ul << ANA_FS_TPM_CAPARRAY_CTRL_POS)
#define ANA_FS_VCO_LDO_PD_ADDR                             (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_VCO_LDO_PD_POS                              (16)     /*< bit[16]     */
#define ANA_FS_VCO_LDO_PD_MASK                             (0x1ul << ANA_FS_VCO_LDO_PD_POS)
#define ANA_FS_OPT_VTUNE_SEL_ADDR                          (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_OPT_VTUNE_SEL_POS                           (17)     /*< bit[19:17]  */
#define ANA_FS_OPT_VTUNE_SEL_MASK                          (0x7ul << ANA_FS_OPT_VTUNE_SEL_POS)
#define ANA_FS_LFVCO_EN_ADDR                               (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_LFVCO_EN_POS                                (20)     /*< bit[20]     */
#define ANA_FS_LFVCO_EN_MASK                               (0x1ul << ANA_FS_LFVCO_EN_POS)
#define ANA_FS_IQBUF_ICTRL_ADDR                            (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_IQBUF_ICTRL_POS                             (21)     /*< bit[23:21]  */
#define ANA_FS_IQBUF_ICTRL_MASK                            (0x7ul << ANA_FS_IQBUF_ICTRL_POS)
#define ANA_FS_TX_EN_ADDR                                  (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_TX_EN_POS                                   (24)     /*< bit[24]     */
#define ANA_FS_TX_EN_MASK                                  (0x1ul << ANA_FS_TX_EN_POS)
#define ANA_FS_RX_EN_ADDR                                  (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_RX_EN_POS                                   (25)     /*< bit[25]     */
#define ANA_FS_RX_EN_MASK                                  (0x1ul << ANA_FS_RX_EN_POS)
#define ANA_FS_HF_LDO_BYP_EN_ADDR                          (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_HF_LDO_BYP_EN_POS                           (26)     /*< bit[26]     */
#define ANA_FS_HF_LDO_BYP_EN_MASK                          (0x1ul << ANA_FS_HF_LDO_BYP_EN_POS)
#define ANA_FS_HF_LDO_PD_ADDR                              (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_HF_LDO_PD_POS                               (27)     /*< bit[27]     */
#define ANA_FS_HF_LDO_PD_MASK                              (0x1ul << ANA_FS_HF_LDO_PD_POS)
#define ANA_FS_PFD_RST_DLY_ADDR                            (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_PFD_RST_DLY_POS                             (28)     /*< bit[29:28]  */
#define ANA_FS_PFD_RST_DLY_MASK                            (0x3ul << ANA_FS_PFD_RST_DLY_POS)
#define ANA_FS_ANG_LDO_BYP_EN_ADDR                         (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_ANG_LDO_BYP_EN_POS                          (30)     /*< bit[30]     */
#define ANA_FS_ANG_LDO_BYP_EN_MASK                         (0x1ul << ANA_FS_ANG_LDO_BYP_EN_POS)
#define ANA_FS_ANG_LDO_PD_ADDR                             (uint32_t)&(ANA->FREQSYNTH_CONFIG_1)
#define ANA_FS_ANG_LDO_PD_POS                              (31)     /*< bit[31]     */
#define ANA_FS_ANG_LDO_PD_MASK                             (0x1ul << ANA_FS_ANG_LDO_PD_POS)

#define ANA_FREQSYNTH_CONFIG_2_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_IREFGEN_STARTUP_EN_ADDR                     (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_IREFGEN_STARTUP_EN_POS                      (0)      /*< bit[0]      */
#define ANA_FS_IREFGEN_STARTUP_EN_MASK                     (0x1ul << ANA_FS_IREFGEN_STARTUP_EN_POS)
#define ANA_FS_VCODIV_GBOOST_ADDR                          (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_VCODIV_GBOOST_POS                           (1)      /*< bit[1]      */
#define ANA_FS_VCODIV_GBOOST_MASK                          (0x1ul << ANA_FS_VCODIV_GBOOST_POS)
#define ANA_FS_CP_ICTRL_ADDR                               (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_CP_ICTRL_POS                                (2)      /*< bit[4:2]    */
#define ANA_FS_CP_ICTRL_MASK                               (0x7ul << ANA_FS_CP_ICTRL_POS)
#define ANA_FS_VTUNEDET_VH_ADDR                            (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_VTUNEDET_VH_POS                             (5)      /*< bit[7:5]    */
#define ANA_FS_VTUNEDET_VH_MASK                            (0x7ul << ANA_FS_VTUNEDET_VH_POS)
#define ANA_FS_VTUNEDET_VL_ADDR                            (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_VTUNEDET_VL_POS                             (8)      /*< bit[10:8]   */
#define ANA_FS_VTUNEDET_VL_MASK                            (0x7ul << ANA_FS_VTUNEDET_VL_POS)
#define ANA_FS_VTUNEPROBE_EN_ADDR                          (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_VTUNEPROBE_EN_POS                           (11)     /*< bit[11]     */
#define ANA_FS_VTUNEPROBE_EN_MASK                          (0x1ul << ANA_FS_VTUNEPROBE_EN_POS)
#define ANA_FS_LF_BW_MODE_ADDR                             (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_LF_BW_MODE_POS                              (12)     /*< bit[12]     */
#define ANA_FS_LF_BW_MODE_MASK                             (0x1ul << ANA_FS_LF_BW_MODE_POS)
#define ANA_FS_KVC_IN_ADDR                                 (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_KVC_IN_POS                                  (13)     /*< bit[17:13]  */
#define ANA_FS_KVC_IN_MASK                                 (0x1Ful << ANA_FS_KVC_IN_POS)
#define ANA_FS_AFC_INIT_BSMODE_EN_ADDR                     (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_AFC_INIT_BSMODE_EN_POS                      (18)     /*< bit[18]     */
#define ANA_FS_AFC_INIT_BSMODE_EN_MASK                     (0x1ul << ANA_FS_AFC_INIT_BSMODE_EN_POS)
#define ANA_FS_AFC_EN_ADDR                                 (uint32_t)&(ANA->FREQSYNTH_CONFIG_2)
#define ANA_FS_AFC_EN_POS                                  (19)     /*< bit[19]     */
#define ANA_FS_AFC_EN_MASK                                 (0x1ul << ANA_FS_AFC_EN_POS)

#define ANA_FREQSYNTH_CONFIG_3_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_3)
#define ANA_FS_AFC_SETTLING_CYCLE_ADDR                     (uint32_t)&(ANA->FREQSYNTH_CONFIG_3)
#define ANA_FS_AFC_SETTLING_CYCLE_POS                      (0)      /*< bit[15:0]   */
#define ANA_FS_AFC_SETTLING_CYCLE_MASK                     (0xFFFFul << ANA_FS_AFC_SETTLING_CYCLE_POS)

#define ANA_FREQSYNTH_CONFIG_4_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_DSM_DITHERING_ADDR                          (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_DSM_DITHERING_POS                           (0)      /*< bit[0]      */
#define ANA_FS_DSM_DITHERING_MASK                          (0x1ul << ANA_FS_DSM_DITHERING_POS)
#define ANA_FS_FRACMODE_EN_ADDR                            (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_FRACMODE_EN_POS                             (1)      /*< bit[1]      */
#define ANA_FS_FRACMODE_EN_MASK                            (0x1ul << ANA_FS_FRACMODE_EN_POS)
#define ANA_FS_TPM_DSM_DITHERING_ADDR                      (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_TPM_DSM_DITHERING_POS                       (2)      /*< bit[2]      */
#define ANA_FS_TPM_DSM_DITHERING_MASK                      (0x1ul << ANA_FS_TPM_DSM_DITHERING_POS)
#define ANA_FS_TPM_EN_ADDR                                 (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_TPM_EN_POS                                  (3)      /*< bit[3]      */
#define ANA_FS_TPM_EN_MASK                                 (0x1ul << ANA_FS_TPM_EN_POS)
#define ANA_FS_TPM_CLK_LF_MODE_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_TPM_CLK_LF_MODE_POS                         (4)      /*< bit[4]      */
#define ANA_FS_TPM_CLK_LF_MODE_MASK                        (0x1ul << ANA_FS_TPM_CLK_LF_MODE_POS)
#define ANA_FS_TPM_CLK_INVMODE_EN_ADDR                     (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_TPM_CLK_INVMODE_EN_POS                      (5)      /*< bit[5]      */
#define ANA_FS_TPM_CLK_INVMODE_EN_MASK                     (0x1ul << ANA_FS_TPM_CLK_INVMODE_EN_POS)
#define ANA_FS_TPM_FBSRC_SEL_ADDR                          (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_TPM_FBSRC_SEL_POS                           (6)      /*< bit[6]      */
#define ANA_FS_TPM_FBSRC_SEL_MASK                          (0x1ul << ANA_FS_TPM_FBSRC_SEL_POS)
#define ANA_FS_TPM_FBSRC_INVMODE_EN_ADDR                   (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_TPM_FBSRC_INVMODE_EN_POS                    (7)      /*< bit[7]      */
#define ANA_FS_TPM_FBSRC_INVMODE_EN_MASK                   (0x1ul << ANA_FS_TPM_FBSRC_INVMODE_EN_POS)
#define ANA_FS_TPM_FBSRC_DLY_ADDR                          (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_TPM_FBSRC_DLY_POS                           (8)      /*< bit[9:8]    */
#define ANA_FS_TPM_FBSRC_DLY_MASK                          (0x3ul << ANA_FS_TPM_FBSRC_DLY_POS)
#define ANA_FS_FREQSYNTH_RST_N_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_FREQSYNTH_RST_N_POS                         (10)     /*< bit[10]     */
#define ANA_FS_FREQSYNTH_RST_N_MASK                        (0x1ul << ANA_FS_FREQSYNTH_RST_N_POS)
#define ANA_FS_FREF_INVMODE_EN_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_FREF_INVMODE_EN_POS                         (11)     /*< bit[11]     */
#define ANA_FS_FREF_INVMODE_EN_MASK                        (0x1ul << ANA_FS_FREF_INVMODE_EN_POS)
#define ANA_FS_DIVMM_GBOOST_ADDR                           (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_DIVMM_GBOOST_POS                            (12)     /*< bit[12]     */
#define ANA_FS_DIVMM_GBOOST_MASK                           (0x1ul << ANA_FS_DIVMM_GBOOST_POS)
#define ANA_FS_TPM_CLK_DIV_MODE_ADDR                       (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_TPM_CLK_DIV_MODE_POS                        (13)     /*< bit[13]     */
#define ANA_FS_TPM_CLK_DIV_MODE_MASK                       (0x1ul << ANA_FS_TPM_CLK_DIV_MODE_POS)
#define ANA_FS_LDO_SUPEN_ADDR                              (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_LDO_SUPEN_POS                               (14)     /*< bit[14]     */
#define ANA_FS_LDO_SUPEN_MASK                              (0x1ul << ANA_FS_LDO_SUPEN_POS)
#define ANA_FS_DSM_DITHERING_BIT_DVALUE_ADDR               (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_DSM_DITHERING_BIT_DVALUE_POS                (15)     /*< bit[15]     */
#define ANA_FS_DSM_DITHERING_BIT_DVALUE_MASK               (0x1ul << ANA_FS_DSM_DITHERING_BIT_DVALUE_POS)
#define ANA_FS_DSM_DITHERING_FS_PARM_ADDR                  (uint32_t)&(ANA->FREQSYNTH_CONFIG_4)
#define ANA_FS_DSM_DITHERING_FS_PARM_POS                   (16)     /*< bit[27:16]  */
#define ANA_FS_DSM_DITHERING_FS_PARM_MASK                  (0xFFFul << ANA_FS_DSM_DITHERING_FS_PARM_POS)

#define ANA_FREQSYNTH_CONFIG_5_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_5)
#define ANA_FS_DIV_INT_ADDR                                (uint32_t)&(ANA->FREQSYNTH_CONFIG_5)
#define ANA_FS_DIV_INT_POS                                 (0)      /*< bit[4:0]    */
#define ANA_FS_DIV_INT_MASK                                (0x1Ful << ANA_FS_DIV_INT_POS)
#define ANA_FS_DIV_FRAC_ADDR                               (uint32_t)&(ANA->FREQSYNTH_CONFIG_5)
#define ANA_FS_DIV_FRAC_POS                                (5)      /*< bit[22:5]   */
#define ANA_FS_DIV_FRAC_MASK                               (0x3FFFFul << ANA_FS_DIV_FRAC_POS)

#define ANA_FREQSYNTH_CONFIG_6_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_6)
#define ANA_FS_TPM_DIN_ADDR                                (uint32_t)&(ANA->FREQSYNTH_CONFIG_6)
#define ANA_FS_TPM_DIN_POS                                 (0)      /*< bit[11:0]   */
#define ANA_FS_TPM_DIN_MASK                                (0xFFFul << ANA_FS_TPM_DIN_POS)
#define ANA_FS_VCO_STARTER_CTRL_ADDR                       (uint32_t)&(ANA->FREQSYNTH_CONFIG_6)
#define ANA_FS_VCO_STARTER_CTRL_POS                        (12)     /*< bit[12]     */
#define ANA_FS_VCO_STARTER_CTRL_MASK                       (0x1ul << ANA_FS_VCO_STARTER_CTRL_POS)
#define ANA_FS_AUTOLBW_EN_ADDR                             (uint32_t)&(ANA->FREQSYNTH_CONFIG_6)
#define ANA_FS_AUTOLBW_EN_POS                              (13)     /*< bit[13]     */
#define ANA_FS_AUTOLBW_EN_MASK                             (0x1ul << ANA_FS_AUTOLBW_EN_POS)
#define ANA_FS_DSM_MODE_ADDR                               (uint32_t)&(ANA->FREQSYNTH_CONFIG_6)
#define ANA_FS_DSM_MODE_POS                                (14)     /*< bit[14]     */
#define ANA_FS_DSM_MODE_MASK                               (0x1ul << ANA_FS_DSM_MODE_POS)
#define ANA_FS_BANDSEL_ADDR                                (uint32_t)&(ANA->FREQSYNTH_CONFIG_6)
#define ANA_FS_BANDSEL_POS                                 (15)     /*< bit[15]     */
#define ANA_FS_BANDSEL_MASK                                (0x1ul << ANA_FS_BANDSEL_POS)

#define ANA_FREQSYNTH_CONFIG_7_ADDR                        (uint32_t)&(ANA->FREQSYNTH_CONFIG_7)
#define ANA_FS_PLL_LOCKED_SIG_ADDR                         (uint32_t)&(ANA->FREQSYNTH_CONFIG_7)
#define ANA_FS_PLL_LOCKED_SIG_POS                          (0)      /*< bit[0]      */
#define ANA_FS_PLL_LOCKED_SIG_MASK                         (0x1ul << ANA_FS_PLL_LOCKED_SIG_POS)
#define ANA_FS_KVC_ROUT_ADDR                               (uint32_t)&(ANA->FREQSYNTH_CONFIG_7)
#define ANA_FS_KVC_ROUT_POS                                (1)      /*< bit[5:1]    */
#define ANA_FS_KVC_ROUT_MASK                               (0x1Ful << ANA_FS_KVC_ROUT_POS)

#define ANA_FREQSYNTH_TPMCAL_CONFIG_1_ADDR                 (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_1)
#define ANA_FS_TPMCAL_DIV_FRAC_HB_ADDR                     (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_1)
#define ANA_FS_TPMCAL_DIV_FRAC_HB_POS                      (0)      /*< bit[17:0]   */
#define ANA_FS_TPMCAL_DIV_FRAC_HB_MASK                     (0x3FFFFul << ANA_FS_TPMCAL_DIV_FRAC_HB_POS)
#define ANA_FS_TPMCAL_DIV_INT_HB_ADDR                      (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_1)
#define ANA_FS_TPMCAL_DIV_INT_HB_POS                       (18)     /*< bit[22:18]  */
#define ANA_FS_TPMCAL_DIV_INT_HB_MASK                      (0x1Ful << ANA_FS_TPMCAL_DIV_INT_HB_POS)

#define ANA_FREQSYNTH_TPMCAL_CONFIG_2_ADDR                 (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_2)
#define ANA_FS_TPMCAL_DIV_FRAC_LB_ADDR                     (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_2)
#define ANA_FS_TPMCAL_DIV_FRAC_LB_POS                      (0)      /*< bit[17:0]   */
#define ANA_FS_TPMCAL_DIV_FRAC_LB_MASK                     (0x3FFFFul << ANA_FS_TPMCAL_DIV_FRAC_LB_POS)
#define ANA_FS_TPMCAL_DIV_INT_LB_ADDR                      (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_2)
#define ANA_FS_TPMCAL_DIV_INT_LB_POS                       (18)     /*< bit[22:18]  */
#define ANA_FS_TPMCAL_DIV_INT_LB_MASK                      (0x1Ful << ANA_FS_TPMCAL_DIV_INT_LB_POS)

#define ANA_FREQSYNTH_TPMCAL_CONFIG_3_ADDR                 (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_3)
#define ANA_FS_TPMCAL_TPM_DIN_HB_ADDR                      (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_3)
#define ANA_FS_TPMCAL_TPM_DIN_HB_POS                       (0)      /*< bit[11:0]   */
#define ANA_FS_TPMCAL_TPM_DIN_HB_MASK                      (0xFFFul << ANA_FS_TPMCAL_TPM_DIN_HB_POS)
#define ANA_FS_TPMCAL_TPM_DIN_LB_ADDR                      (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_3)
#define ANA_FS_TPMCAL_TPM_DIN_LB_POS                       (12)     /*< bit[23:12]  */
#define ANA_FS_TPMCAL_TPM_DIN_LB_MASK                      (0xFFFul << ANA_FS_TPMCAL_TPM_DIN_LB_POS)

#define ANA_FREQSYNTH_TPMCAL_CONFIG_4_ADDR                 (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_4)
#define ANA_FS_TPMCAL_SETTLING_CYCLES_ADDR                 (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_4)
#define ANA_FS_TPMCAL_SETTLING_CYCLES_POS                  (0)      /*< bit[6:0]    */
#define ANA_FS_TPMCAL_SETTLING_CYCLES_MASK                 (0x7Ful << ANA_FS_TPMCAL_SETTLING_CYCLES_POS)
#define ANA_FS_TPMCAL_SEQUENTIAL_COUNTS_ADDR               (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_4)
#define ANA_FS_TPMCAL_SEQUENTIAL_COUNTS_POS                (7)      /*< bit[18:7]   */
#define ANA_FS_TPMCAL_SEQUENTIAL_COUNTS_MASK               (0xFFFul << ANA_FS_TPMCAL_SEQUENTIAL_COUNTS_POS)
#define ANA_FS_TPMCAL_AVG_SAMPLES_SEL_ADDR                 (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_4)
#define ANA_FS_TPMCAL_AVG_SAMPLES_SEL_POS                  (19)     /*< bit[20:19]  */
#define ANA_FS_TPMCAL_AVG_SAMPLES_SEL_MASK                 (0x3ul << ANA_FS_TPMCAL_AVG_SAMPLES_SEL_POS)
#define ANA_FS_TPMCAL_EN_ADDR                              (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_4)
#define ANA_FS_TPMCAL_EN_POS                               (21)     /*< bit[21]     */
#define ANA_FS_TPMCAL_EN_MASK                              (0x1ul << ANA_FS_TPMCAL_EN_POS)
#define ANA_FS_TPMCAL_RST_N_ADDR                           (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_4)
#define ANA_FS_TPMCAL_RST_N_POS                            (22)     /*< bit[22]     */
#define ANA_FS_TPMCAL_RST_N_MASK                           (0x1ul << ANA_FS_TPMCAL_RST_N_POS)

#define ANA_FREQSYNTH_TPMCAL_CONFIG_5_ADDR                 (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_5)
#define ANA_FS_TPMCAL_TPM_DIN_HB_OUT_ADDR                  (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_5)
#define ANA_FS_TPMCAL_TPM_DIN_HB_OUT_POS                   (0)      /*< bit[11:0]   */
#define ANA_FS_TPMCAL_TPM_DIN_HB_OUT_MASK                  (0xFFFul << ANA_FS_TPMCAL_TPM_DIN_HB_OUT_POS)
#define ANA_FS_TPMCAL_DONE_ADDR                            (uint32_t)&(ANA->FREQSYNTH_TPMCAL_CONFIG_5)
#define ANA_FS_TPMCAL_DONE_POS                             (12)     /*< bit[12]     */
#define ANA_FS_TPMCAL_DONE_MASK                            (0x1ul << ANA_FS_TPMCAL_DONE_POS)

#define ANA_TPM_DMI_CONFIG_1_ADDR                          (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_FS_PARM_ADDR                           (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_FS_PARM_POS                            (0)      /*< bit[19:0]   */
#define ANA_TPM_DMI_FS_PARM_MASK                           (0xFFFFFul << ANA_TPM_DMI_FS_PARM_POS)
#define ANA_TPM_DMI_GSF_EN_ADDR                            (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_GSF_EN_POS                             (20)     /*< bit[20]     */
#define ANA_TPM_DMI_GSF_EN_MASK                            (0x1ul << ANA_TPM_DMI_GSF_EN_POS)
#define ANA_TPM_DMI_EN_ADDR                                (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_EN_POS                                 (21)     /*< bit[21]     */
#define ANA_TPM_DMI_EN_MASK                                (0x1ul << ANA_TPM_DMI_EN_POS)
#define ANA_TPM_DMI_TEST_MODE_ADDR                         (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_TEST_MODE_POS                          (22)     /*< bit[25:22]  */
#define ANA_TPM_DMI_TEST_MODE_MASK                         (0xFul << ANA_TPM_DMI_TEST_MODE_POS)
#define ANA_TPM_DMI_DPM_EN_ADDR                            (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_DPM_EN_POS                             (26)     /*< bit[26]     */
#define ANA_TPM_DMI_DPM_EN_MASK                            (0x1ul << ANA_TPM_DMI_DPM_EN_POS)
#define ANA_TPM_DMI_DPM_MODE_ADDR                          (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_DPM_MODE_POS                           (27)     /*< bit[27]     */
#define ANA_TPM_DMI_DPM_MODE_MASK                          (0x1ul << ANA_TPM_DMI_DPM_MODE_POS)
#define ANA_TPM_DMI_SINGLE_BIT_MODE_ADDR                   (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_SINGLE_BIT_MODE_POS                    (28)     /*< bit[28]     */
#define ANA_TPM_DMI_SINGLE_BIT_MODE_MASK                   (0x1ul << ANA_TPM_DMI_SINGLE_BIT_MODE_POS)
#define ANA_TPM_DMI_DPM_PWRCTL_PHASE_SENSING_ADDR          (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_DPM_PWRCTL_PHASE_SENSING_POS           (29)     /*< bit[29]     */
#define ANA_TPM_DMI_DPM_PWRCTL_PHASE_SENSING_MASK          (0x1ul << ANA_TPM_DMI_DPM_PWRCTL_PHASE_SENSING_POS)
#define ANA_TPM_DMI_DPM_PWRCTL_EN_ADDR                     (uint32_t)&(ANA->TPM_DMI_CONFIG_1)
#define ANA_TPM_DMI_DPM_PWRCTL_EN_POS                      (30)     /*< bit[30]     */
#define ANA_TPM_DMI_DPM_PWRCTL_EN_MASK                     (0x1ul << ANA_TPM_DMI_DPM_PWRCTL_EN_POS)

#define ANA_TPM_DMI_CONFIG_2_ADDR                          (uint32_t)&(ANA->TPM_DMI_CONFIG_2)
#define ANA_FS_TPMCAL_DIV_FRAC_HB_M_ADDR                   (uint32_t)&(ANA->TPM_DMI_CONFIG_2)
#define ANA_FS_TPMCAL_DIV_FRAC_HB_M_POS                    (0)      /*< bit[17:0]   */
#define ANA_FS_TPMCAL_DIV_FRAC_HB_M_MASK                   (0x3FFFFul << ANA_FS_TPMCAL_DIV_FRAC_HB_M_POS)
#define ANA_FS_TPMCAL_DIV_INT_HB_M_ADDR                    (uint32_t)&(ANA->TPM_DMI_CONFIG_2)
#define ANA_FS_TPMCAL_DIV_INT_HB_M_POS                     (18)     /*< bit[22:18]  */
#define ANA_FS_TPMCAL_DIV_INT_HB_M_MASK                    (0x1Ful << ANA_FS_TPMCAL_DIV_INT_HB_M_POS)

#define ANA_TPM_DMI_CONFIG_3_ADDR                          (uint32_t)&(ANA->TPM_DMI_CONFIG_3)
#define ANA_FS_TPMCAL_DIV_FRAC_LB_M_ADDR                   (uint32_t)&(ANA->TPM_DMI_CONFIG_3)
#define ANA_FS_TPMCAL_DIV_FRAC_LB_M_POS                    (0)      /*< bit[17:0]   */
#define ANA_FS_TPMCAL_DIV_FRAC_LB_M_MASK                   (0x3FFFFul << ANA_FS_TPMCAL_DIV_FRAC_LB_M_POS)
#define ANA_FS_TPMCAL_DIV_INT_LB_M_ADDR                    (uint32_t)&(ANA->TPM_DMI_CONFIG_3)
#define ANA_FS_TPMCAL_DIV_INT_LB_M_POS                     (18)     /*< bit[22:18]  */
#define ANA_FS_TPMCAL_DIV_INT_LB_M_MASK                    (0x1Ful << ANA_FS_TPMCAL_DIV_INT_LB_M_POS)

#define ANA_TPM_DMI_CONFIG_4_ADDR                          (uint32_t)&(ANA->TPM_DMI_CONFIG_4)
#define ANA_FS_TPMCAL_TPM_DIN_HB_M_ADDR                    (uint32_t)&(ANA->TPM_DMI_CONFIG_4)
#define ANA_FS_TPMCAL_TPM_DIN_HB_M_POS                     (0)      /*< bit[11:0]   */
#define ANA_FS_TPMCAL_TPM_DIN_HB_M_MASK                    (0xFFFul << ANA_FS_TPMCAL_TPM_DIN_HB_M_POS)
#define ANA_FS_TPMCAL_TPM_DIN_LB_M_ADDR                    (uint32_t)&(ANA->TPM_DMI_CONFIG_4)
#define ANA_FS_TPMCAL_TPM_DIN_LB_M_POS                     (12)     /*< bit[23:12]  */
#define ANA_FS_TPMCAL_TPM_DIN_LB_M_MASK                    (0xFFFul << ANA_FS_TPMCAL_TPM_DIN_LB_M_POS)

#define ANA_PA1G_CONFIG_0_ADDR                             (uint32_t)&(ANA->PA1G_CONFIG_0)
#define ANA_PA1G_EN_ADDR                                   (uint32_t)&(ANA->PA1G_CONFIG_0)
#define ANA_PA1G_EN_POS                                    (0)      /*< bit[0]      */
#define ANA_PA1G_EN_MASK                                   (0x1ul << ANA_PA1G_EN_POS)
#define ANA_PA1G_DUTY_EN_ADDR                              (uint32_t)&(ANA->PA1G_CONFIG_0)
#define ANA_PA1G_DUTY_EN_POS                               (2)      /*< bit[2]      */
#define ANA_PA1G_DUTY_EN_MASK                              (0x1ul << ANA_PA1G_DUTY_EN_POS)
#define ANA_PA1G_VB_DRIVER_SEL_ADDR                        (uint32_t)&(ANA->PA1G_CONFIG_0)
#define ANA_PA1G_VB_DRIVER_SEL_POS                         (3)      /*< bit[3]      */
#define ANA_PA1G_VB_DRIVER_SEL_MASK                        (0x1ul << ANA_PA1G_VB_DRIVER_SEL_POS)
#define ANA_PA1G_PWR_ADDR                                  (uint32_t)&(ANA->PA1G_CONFIG_0)
#define ANA_PA1G_PWR_POS                                   (6)      /*< bit[12:6]   */
#define ANA_PA1G_PWR_MASK                                  (0x7Ful << ANA_PA1G_PWR_POS)
#define ANA_PA1G_VB2_SEL_ADDR                              (uint32_t)&(ANA->PA1G_CONFIG_0)
#define ANA_PA1G_VB2_SEL_POS                               (21)     /*< bit[23:21]  */
#define ANA_PA1G_VB2_SEL_MASK                              (0x7ul << ANA_PA1G_VB2_SEL_POS)

#define ANA_PA1G_CONFIG_1_ADDR                             (uint32_t)&(ANA->PA1G_CONFIG_1)
#define ANA_PA1G_AMUX_SEL_ADDR                             (uint32_t)&(ANA->PA1G_CONFIG_1)
#define ANA_PA1G_AMUX_SEL_POS                              (0)      /*< bit[2:0]    */
#define ANA_PA1G_AMUX_SEL_MASK                             (0x7ul << ANA_PA1G_AMUX_SEL_POS)
#define ANA_PA1G_TX_RESERVED_ADDR                          (uint32_t)&(ANA->PA1G_CONFIG_1)
#define ANA_PA1G_TX_RESERVED_POS                           (6)      /*< bit[7:6]    */
#define ANA_PA1G_TX_RESERVED_MASK                          (0x3ul << ANA_PA1G_TX_RESERVED_POS)
#define ANA_PA1G_IREF_SEL_ADDR                             (uint32_t)&(ANA->PA1G_CONFIG_1)
#define ANA_PA1G_IREF_SEL_POS                              (8)      /*< bit[10:8]   */
#define ANA_PA1G_IREF_SEL_MASK                             (0x7ul << ANA_PA1G_IREF_SEL_POS)
#define ANA_PA1G_DUTY_SEL_ADDR                             (uint32_t)&(ANA->PA1G_CONFIG_1)
#define ANA_PA1G_DUTY_SEL_POS                              (14)     /*< bit[16:14]  */
#define ANA_PA1G_DUTY_SEL_MASK                             (0x7ul << ANA_PA1G_DUTY_SEL_POS)
#define ANA_PA1G_EN_RAMP_ADDR                              (uint32_t)&(ANA->PA1G_CONFIG_1)
#define ANA_PA1G_EN_RAMP_POS                               (17)     /*< bit[17]     */
#define ANA_PA1G_EN_RAMP_MASK                              (0x1ul << ANA_PA1G_EN_RAMP_POS)
#define ANA_PA1G_SEL_RAMP_ADDR                             (uint32_t)&(ANA->PA1G_CONFIG_1)
#define ANA_PA1G_SEL_RAMP_POS                              (18)     /*< bit[19:18]  */
#define ANA_PA1G_SEL_RAMP_MASK                             (0x3ul << ANA_PA1G_SEL_RAMP_POS)

#define ANA_XO_CONFIG_1_ADDR                               (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_LDOVDD_LV_ADDR                          (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_LDOVDD_LV_POS                           (0)      /*< bit[1:0]    */
#define ANA_XO_40M_LDOVDD_LV_MASK                          (0x3ul << ANA_XO_40M_LDOVDD_LV_POS)
#define ANA_XO_40M_PD_ADDR                                 (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_PD_POS                                  (2)      /*< bit[2]      */
#define ANA_XO_40M_PD_MASK                                 (0x1ul << ANA_XO_40M_PD_POS)
#define ANA_XO_40M_STARTUP_EN_ADDR                         (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_STARTUP_EN_POS                          (3)      /*< bit[3]      */
#define ANA_XO_40M_STARTUP_EN_MASK                         (0x1ul << ANA_XO_40M_STARTUP_EN_POS)
#define ANA_XO_40M_GAIN_CTRL_ADDR                          (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_GAIN_CTRL_POS                           (4)      /*< bit[7:4]    */
#define ANA_XO_40M_GAIN_CTRL_MASK                          (0xFul << ANA_XO_40M_GAIN_CTRL_POS)
#define ANA_XO_40M_CAP_I_CTRL_ADDR                         (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_CAP_I_CTRL_POS                          (8)      /*< bit[14:8]   */
#define ANA_XO_40M_CAP_I_CTRL_MASK                         (0x7Ful << ANA_XO_40M_CAP_I_CTRL_POS)
#define ANA_XO_40M_CAP_O_CTRL_ADDR                         (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_CAP_O_CTRL_POS                          (15)     /*< bit[21:15]  */
#define ANA_XO_40M_CAP_O_CTRL_MASK                         (0x7Ful << ANA_XO_40M_CAP_O_CTRL_POS)
#define ANA_XO_40M_MAINCAP_I_EN_ADDR                       (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_MAINCAP_I_EN_POS                        (22)     /*< bit[22]     */
#define ANA_XO_40M_MAINCAP_I_EN_MASK                       (0x1ul << ANA_XO_40M_MAINCAP_I_EN_POS)
#define ANA_XO_40M_MAINCAP_O_EN_ADDR                       (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_MAINCAP_O_EN_POS                        (23)     /*< bit[23]     */
#define ANA_XO_40M_MAINCAP_O_EN_MASK                       (0x1ul << ANA_XO_40M_MAINCAP_O_EN_POS)
#define ANA_XO_40M_SWING_BOOST_ADDR                        (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_SWING_BOOST_POS                         (24)     /*< bit[24]     */
#define ANA_XO_40M_SWING_BOOST_MASK                        (0x1ul << ANA_XO_40M_SWING_BOOST_POS)
#define ANA_XO_40M_LDO_BYPCAP_EN_ADDR                      (uint32_t)&(ANA->XO_CONFIG_1)
#define ANA_XO_40M_LDO_BYPCAP_EN_POS                       (25)     /*< bit[25]     */
#define ANA_XO_40M_LDO_BYPCAP_EN_MASK                      (0x1ul << ANA_XO_40M_LDO_BYPCAP_EN_POS)

#define ANA_XO_CONFIG_2_ADDR                               (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_CHG_EN_ADDR                        (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_CHG_EN_POS                         (0)      /*< bit[0]      */
#define ANA_XTAL_ENVDTR_CHG_EN_MASK                        (0x1ul << ANA_XTAL_ENVDTR_CHG_EN_POS)
#define ANA_XTAL_ENVDTR_CHG_ICTRL_ADDR                     (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_CHG_ICTRL_POS                      (1)      /*< bit[2:1]    */
#define ANA_XTAL_ENVDTR_CHG_ICTRL_MASK                     (0x3ul << ANA_XTAL_ENVDTR_CHG_ICTRL_POS)
#define ANA_XTAL_ENVDTR_DCMP_ICTRL_ADDR                    (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_DCMP_ICTRL_POS                     (3)      /*< bit[5:3]    */
#define ANA_XTAL_ENVDTR_DCMP_ICTRL_MASK                    (0x7ul << ANA_XTAL_ENVDTR_DCMP_ICTRL_POS)
#define ANA_XTAL_ENVDTR_DISCHG_SR_EN_ADDR                  (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_DISCHG_SR_EN_POS                   (6)      /*< bit[6]      */
#define ANA_XTAL_ENVDTR_DISCHG_SR_EN_MASK                  (0x1ul << ANA_XTAL_ENVDTR_DISCHG_SR_EN_POS)
#define ANA_XTAL_ENVDTR_OTA_ICTRL_ADDR                     (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_OTA_ICTRL_POS                      (7)      /*< bit[9:7]    */
#define ANA_XTAL_ENVDTR_OTA_ICTRL_MASK                     (0x7ul << ANA_XTAL_ENVDTR_OTA_ICTRL_POS)
#define ANA_XTAL_ENVDTR_PD_ADDR                            (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_PD_POS                             (10)     /*< bit[10]     */
#define ANA_XTAL_ENVDTR_PD_MASK                            (0x1ul << ANA_XTAL_ENVDTR_PD_POS)
#define ANA_XTAL_ENVDTR_VTH_P_SEL_ADDR                     (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_VTH_P_SEL_POS                      (11)     /*< bit[13:11]  */
#define ANA_XTAL_ENVDTR_VTH_P_SEL_MASK                     (0x7ul << ANA_XTAL_ENVDTR_VTH_P_SEL_POS)
#define ANA_XTAL_ENVDTR_VTH_N_SEL_ADDR                     (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_XTAL_ENVDTR_VTH_N_SEL_POS                      (14)     /*< bit[16:14]  */
#define ANA_XTAL_ENVDTR_VTH_N_SEL_MASK                     (0x7ul << ANA_XTAL_ENVDTR_VTH_N_SEL_POS)
#define ANA_LV_LPXTAL_BIAS_STARTER_N_ADDR                  (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_LV_LPXTAL_BIAS_STARTER_N_POS                   (17)     /*< bit[17]     */
#define ANA_LV_LPXTAL_BIAS_STARTER_N_MASK                  (0x1ul << ANA_LV_LPXTAL_BIAS_STARTER_N_POS)
#define ANA_LV_LPXTAL_GAIN_CTRL_ADDR                       (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_LV_LPXTAL_GAIN_CTRL_POS                        (18)     /*< bit[23:18]  */
#define ANA_LV_LPXTAL_GAIN_CTRL_MASK                       (0x3Ful << ANA_LV_LPXTAL_GAIN_CTRL_POS)
#define ANA_LV_LPXTAL_LDO_BYP_EN_ADDR                      (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_LV_LPXTAL_LDO_BYP_EN_POS                       (24)     /*< bit[24]     */
#define ANA_LV_LPXTAL_LDO_BYP_EN_MASK                      (0x1ul << ANA_LV_LPXTAL_LDO_BYP_EN_POS)
#define ANA_LV_LPXTAL_LDO_LV_ADDR                          (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_LV_LPXTAL_LDO_LV_POS                           (25)     /*< bit[26:25]  */
#define ANA_LV_LPXTAL_LDO_LV_MASK                          (0x3ul << ANA_LV_LPXTAL_LDO_LV_POS)
#define ANA_LV_LPXTAL_OSC_STARTER_N_ADDR                   (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_LV_LPXTAL_OSC_STARTER_N_POS                    (27)     /*< bit[27]     */
#define ANA_LV_LPXTAL_OSC_STARTER_N_MASK                   (0x1ul << ANA_LV_LPXTAL_OSC_STARTER_N_POS)
#define ANA_LV_LPXTAL_PD_ADDR                              (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_LV_LPXTAL_PD_POS                               (28)     /*< bit[28]     */
#define ANA_LV_LPXTAL_PD_MASK                              (0x1ul << ANA_LV_LPXTAL_PD_POS)
#define ANA_LV_LPXTAL_CORE_VREF_ADDR                       (uint32_t)&(ANA->XO_CONFIG_2)
#define ANA_LV_LPXTAL_CORE_VREF_POS                        (29)     /*< bit[30:29]  */
#define ANA_LV_LPXTAL_CORE_VREF_MASK                       (0x3ul << ANA_LV_LPXTAL_CORE_VREF_POS)

#define ANA_XO_CONFIG_3_ADDR                               (uint32_t)&(ANA->XO_CONFIG_3)
#define ANA_LV_LPXTAL_CAP_I_CTRL_ADDR                      (uint32_t)&(ANA->XO_CONFIG_3)
#define ANA_LV_LPXTAL_CAP_I_CTRL_POS                       (0)      /*< bit[5:0]    */
#define ANA_LV_LPXTAL_CAP_I_CTRL_MASK                      (0x3Ful << ANA_LV_LPXTAL_CAP_I_CTRL_POS)
#define ANA_LV_LPXTAL_CAP_O_CTRL_ADDR                      (uint32_t)&(ANA->XO_CONFIG_3)
#define ANA_LV_LPXTAL_CAP_O_CTRL_POS                       (6)      /*< bit[11:6]   */
#define ANA_LV_LPXTAL_CAP_O_CTRL_MASK                      (0x3Ful << ANA_LV_LPXTAL_CAP_O_CTRL_POS)
#define ANA_LV_LPXTAL_MAINCAP_I_EN_ADDR                    (uint32_t)&(ANA->XO_CONFIG_3)
#define ANA_LV_LPXTAL_MAINCAP_I_EN_POS                     (12)     /*< bit[12]     */
#define ANA_LV_LPXTAL_MAINCAP_I_EN_MASK                    (0x1ul << ANA_LV_LPXTAL_MAINCAP_I_EN_POS)
#define ANA_LV_LPXTAL_MAINCAP_O_EN_ADDR                    (uint32_t)&(ANA->XO_CONFIG_3)
#define ANA_LV_LPXTAL_MAINCAP_O_EN_POS                     (13)     /*< bit[13]     */
#define ANA_LV_LPXTAL_MAINCAP_O_EN_MASK                    (0x1ul << ANA_LV_LPXTAL_MAINCAP_O_EN_POS)

#define ANA_XO_CONFIG_4_ADDR                               (uint32_t)&(ANA->XO_CONFIG_4)
#define ANA_XTAL_ENVDTR_VOUT_DC_ADDR                       (uint32_t)&(ANA->XO_CONFIG_4)
#define ANA_XTAL_ENVDTR_VOUT_DC_POS                        (0)      /*< bit[0]      */
#define ANA_XTAL_ENVDTR_VOUT_DC_MASK                       (0x1ul << ANA_XTAL_ENVDTR_VOUT_DC_POS)

#define ANA_GPIO_CONFIG_1_ADDR                             (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_GPIO_SMT_EN_ADDR                               (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_GPIO_SMT_EN_POS                                (0)      /*< bit[0]      */
#define ANA_GPIO_SMT_EN_MASK                               (0x1ul << ANA_GPIO_SMT_EN_POS)
#define ANA_GPIO_DOUT_STR_ADDR                             (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_GPIO_DOUT_STR_POS                              (1)      /*< bit[2:1]    */
#define ANA_GPIO_DOUT_STR_MASK                             (0x3ul << ANA_GPIO_DOUT_STR_POS)
#define ANA_BB_TESTPAD_EN_ADDR                             (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_BB_TESTPAD_EN_POS                              (3)      /*< bit[3]      */
#define ANA_BB_TESTPAD_EN_MASK                             (0x1ul << ANA_BB_TESTPAD_EN_POS)
#define ANA_MIXER_TESTPAD_EN_ADDR                          (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_MIXER_TESTPAD_EN_POS                           (4)      /*< bit[4]      */
#define ANA_MIXER_TESTPAD_EN_MASK                          (0x1ul << ANA_MIXER_TESTPAD_EN_POS)
#define ANA_FILTER_TESTPAD_EN_ADDR                         (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_FILTER_TESTPAD_EN_POS                          (5)      /*< bit[5]      */
#define ANA_FILTER_TESTPAD_EN_MASK                         (0x1ul << ANA_FILTER_TESTPAD_EN_POS)
#define ANA_RX_TESTPAD_SEL_ADDR                            (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_RX_TESTPAD_SEL_POS                             (6)      /*< bit[7:6]    */
#define ANA_RX_TESTPAD_SEL_MASK                            (0x3ul << ANA_RX_TESTPAD_SEL_POS)
#define ANA_BB_TESTPAD_TIEALL_ADDR                         (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_BB_TESTPAD_TIEALL_POS                          (8)      /*< bit[8]      */
#define ANA_BB_TESTPAD_TIEALL_MASK                         (0x1ul << ANA_BB_TESTPAD_TIEALL_POS)
#define ANA_DIG_SYSCLK_SEL_ADDR                            (uint32_t)&(ANA->GPIO_CONFIG_1)
#define ANA_DIG_SYSCLK_SEL_POS                             (9)      /*< bit[11:9]   */
#define ANA_DIG_SYSCLK_SEL_MASK                            (0x7ul << ANA_DIG_SYSCLK_SEL_POS)

#define ANA_GPREG_CONFIG_1_ADDR                            (uint32_t)&(ANA->GPREG_CONFIG_1)
#define ANA_GPREG_CONFIG_2_ADDR                            (uint32_t)&(ANA->GPREG_CONFIG_2)
#define ANA_GPREG_CONFIG_3_ADDR                            (uint32_t)&(ANA->GPREG_CONFIG_3)
#define ANA_RX_POWSET_ADDR                                 (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_RFLDO_EN_ADDR                               (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_RFLDO_EN_POS                                (0)      /*< bit[0]      */
#define ANA_RX_RFLDO_EN_MASK                               (0x1ul << ANA_RX_RFLDO_EN_POS)
#define ANA_RX_BBLDO_EN_ADDR                               (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_BBLDO_EN_POS                                (1)      /*< bit[1]      */
#define ANA_RX_BBLDO_EN_MASK                               (0x1ul << ANA_RX_BBLDO_EN_POS)
#define ANA_RX_BBLDO_BYPSS_ADDR                            (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_BBLDO_BYPSS_POS                             (2)      /*< bit[2]      */
#define ANA_RX_BBLDO_BYPSS_MASK                            (0x1ul << ANA_RX_BBLDO_BYPSS_POS)
#define ANA_RX_LNA1GCORE_EN_ADDR                           (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_LNA1GCORE_EN_POS                            (3)      /*< bit[3]      */
#define ANA_RX_LNA1GCORE_EN_MASK                           (0x1ul << ANA_RX_LNA1GCORE_EN_POS)
#define ANA_RX_MIX1GCORE_EN_ADDR                           (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_MIX1GCORE_EN_POS                            (5)      /*< bit[5]      */
#define ANA_RX_MIX1GCORE_EN_MASK                           (0x1ul << ANA_RX_MIX1GCORE_EN_POS)
#define ANA_RX_FILT_EN_ADDR                                (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_FILT_EN_POS                                 (7)      /*< bit[7]      */
#define ANA_RX_FILT_EN_MASK                                (0x1ul << ANA_RX_FILT_EN_POS)
#define ANA_RX_PGA_EN_ADDR                                 (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_PGA_EN_POS                                  (8)      /*< bit[8]      */
#define ANA_RX_PGA_EN_MASK                                 (0x1ul << ANA_RX_PGA_EN_POS)
#define ANA_RX_ED_EN_ADDR                                  (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_ED_EN_POS                                   (9)      /*< bit[9]      */
#define ANA_RX_ED_EN_MASK                                  (0x1ul << ANA_RX_ED_EN_POS)
#define ANA_RX_LNA_CURSEL_ADDR                             (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_LNA_CURSEL_POS                              (10)     /*< bit[12:10]  */
#define ANA_RX_LNA_CURSEL_MASK                             (0x7ul << ANA_RX_LNA_CURSEL_POS)
#define ANA_RX_MIX_CURSEL_ADDR                             (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_MIX_CURSEL_POS                              (13)     /*< bit[15:13]  */
#define ANA_RX_MIX_CURSEL_MASK                             (0x7ul << ANA_RX_MIX_CURSEL_POS)
#define ANA_RX_LOGEN_CURSEL_ADDR                           (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_LOGEN_CURSEL_POS                            (16)     /*< bit[18:16]  */
#define ANA_RX_LOGEN_CURSEL_MASK                           (0x7ul << ANA_RX_LOGEN_CURSEL_POS)
#define ANA_RX_BB_CURSEL_ADDR                              (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_BB_CURSEL_POS                               (19)     /*< bit[21:19]  */
#define ANA_RX_BB_CURSEL_MASK                              (0x7ul << ANA_RX_BB_CURSEL_POS)
#define ANA_RX_1GCALSOURCE_EN_ADDR                         (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_1GCALSOURCE_EN_POS                          (22)     /*< bit[22]     */
#define ANA_RX_1GCALSOURCE_EN_MASK                         (0x1ul << ANA_RX_1GCALSOURCE_EN_POS)
#define ANA_RX_RFLDO_SEL_ADDR                              (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_RFLDO_SEL_POS                               (24)     /*< bit[25:24]  */
#define ANA_RX_RFLDO_SEL_MASK                              (0x3ul << ANA_RX_RFLDO_SEL_POS)
#define ANA_RX_BB_CURBGSEL_ADDR                            (uint32_t)&(ANA->RX_POWSET)
#define ANA_RX_BB_CURBGSEL_POS                             (27)     /*< bit[27]     */
#define ANA_RX_BB_CURBGSEL_MASK                            (0x1ul << ANA_RX_BB_CURBGSEL_POS)

#define ANA_RX_GAIN_FILTSET_ADDR                           (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_LNA_GAIN_ADDR                               (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_LNA_GAIN_POS                                (0)      /*< bit[1:0]    */
#define ANA_RX_LNA_GAIN_MASK                               (0x3ul << ANA_RX_LNA_GAIN_POS)
#define ANA_RX_MIX_GAIN_ADDR                               (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_MIX_GAIN_POS                                (2)      /*< bit[3:2]    */
#define ANA_RX_MIX_GAIN_MASK                               (0x3ul << ANA_RX_MIX_GAIN_POS)
#define ANA_RX_FILTGAIN_ADDR                               (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_FILTGAIN_POS                                (4)      /*< bit[6:4]    */
#define ANA_RX_FILTGAIN_MASK                               (0x7ul << ANA_RX_FILTGAIN_POS)
#define ANA_RX_PGAGAIN_ADDR                                (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_PGAGAIN_POS                                 (7)      /*< bit[10:7]   */
#define ANA_RX_PGAGAIN_MASK                                (0xFul << ANA_RX_PGAGAIN_POS)
#define ANA_RX_FILTIFEN_ADDR                               (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_FILTIFEN_POS                                (11)     /*< bit[11]     */
#define ANA_RX_FILTIFEN_MASK                               (0x1ul << ANA_RX_FILTIFEN_POS)
#define ANA_RX_FILTIFRESSEL_ADDR                           (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_FILTIFRESSEL_POS                            (12)     /*< bit[15:12]  */
#define ANA_RX_FILTIFRESSEL_MASK                           (0xFul << ANA_RX_FILTIFRESSEL_POS)
#define ANA_RX_FILTCAPSEL_ADDR                             (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_FILTCAPSEL_POS                              (16)     /*< bit[21:16]  */
#define ANA_RX_FILTCAPSEL_MASK                             (0x3Ful << ANA_RX_FILTCAPSEL_POS)
#define ANA_RX_FILTCAPCAL_ADDR                             (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_FILTCAPCAL_POS                              (22)     /*< bit[26:22]  */
#define ANA_RX_FILTCAPCAL_MASK                             (0x1Ful << ANA_RX_FILTCAPCAL_POS)
#define ANA_RX_FILTIFRESVALUE_ADDR                         (uint32_t)&(ANA->RX_GAIN_FILTSET)
#define ANA_RX_FILTIFRESVALUE_POS                          (27)     /*< bit[31:27]  */
#define ANA_RX_FILTIFRESVALUE_MASK                         (0x1Ful << ANA_RX_FILTIFRESVALUE_POS)

#define ANA_RX_STATUS_ADDR                                 (uint32_t)&(ANA->RX_STATUS)
#define ANA_RX_FLAG_RFUNDERFLOW_ADDR                       (uint32_t)&(ANA->RX_STATUS)
#define ANA_RX_FLAG_RFUNDERFLOW_POS                        (0)      /*< bit[0]      */
#define ANA_RX_FLAG_RFUNDERFLOW_MASK                       (0x1ul << ANA_RX_FLAG_RFUNDERFLOW_POS)
#define ANA_RX_FLAG_RFOVERFLOW_ADDR                        (uint32_t)&(ANA->RX_STATUS)
#define ANA_RX_FLAG_RFOVERFLOW_POS                         (1)      /*< bit[1]      */
#define ANA_RX_FLAG_RFOVERFLOW_MASK                        (0x1ul << ANA_RX_FLAG_RFOVERFLOW_POS)
#define ANA_RX_FLAG_BBOVERFLOW_ADDR                        (uint32_t)&(ANA->RX_STATUS)
#define ANA_RX_FLAG_BBOVERFLOW_POS                         (2)      /*< bit[2]      */
#define ANA_RX_FLAG_BBOVERFLOW_MASK                        (0x1ul << ANA_RX_FLAG_BBOVERFLOW_POS)
#define ANA_RX_FILTCAPCAL_ROUT_ADDR                        (uint32_t)&(ANA->RX_STATUS)
#define ANA_RX_FILTCAPCAL_ROUT_POS                         (5)      /*< bit[9:5]    */
#define ANA_RX_FILTCAPCAL_ROUT_MASK                        (0x1Ful << ANA_RX_FILTCAPCAL_ROUT_POS)
#define ANA_RX_RXGAINSEL_ADDR                              (uint32_t)&(ANA->RX_STATUS)
#define ANA_RX_RXGAINSEL_POS                               (10)     /*< bit[20:10]  */
#define ANA_RX_RXGAINSEL_MASK                              (0x7FFul << ANA_RX_RXGAINSEL_POS)

#define ANA_RX_CONFIG_5_ADDR                               (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_CALIBUFCURSEL_ADDR                          (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_CALIBUFCURSEL_POS                           (0)      /*< bit[2:0]    */
#define ANA_RX_CALIBUFCURSEL_MASK                          (0x7ul << ANA_RX_CALIBUFCURSEL_POS)
#define ANA_RX_FILTAUTOCOMPEN_ADDR                         (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_FILTAUTOCOMPEN_POS                          (3)      /*< bit[3]      */
#define ANA_RX_FILTAUTOCOMPEN_MASK                         (0x1ul << ANA_RX_FILTAUTOCOMPEN_POS)
#define ANA_RX_LNA1GCSCVSEL_ADDR                           (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_LNA1GCSCVSEL_POS                            (6)      /*< bit[7:6]    */
#define ANA_RX_LNA1GCSCVSEL_MASK                           (0x3ul << ANA_RX_LNA1GCSCVSEL_POS)
#define ANA_RX_LNA1GGMVSEL_ADDR                            (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_LNA1GGMVSEL_POS                             (8)      /*< bit[10:8]   */
#define ANA_RX_LNA1GGMVSEL_MASK                            (0x7ul << ANA_RX_LNA1GGMVSEL_POS)
#define ANA_RX_LOFLIPI_ADDR                                (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_LOFLIPI_POS                                 (11)     /*< bit[11]     */
#define ANA_RX_LOFLIPI_MASK                                (0x1ul << ANA_RX_LOFLIPI_POS)
#define ANA_RX_LOFLIPQ_ADDR                                (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_LOFLIPQ_POS                                 (12)     /*< bit[12]     */
#define ANA_RX_LOFLIPQ_MASK                                (0x1ul << ANA_RX_LOFLIPQ_POS)
#define ANA_RX_LOPHASE_ADDR                                (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_LOPHASE_POS                                 (13)     /*< bit[16:13]  */
#define ANA_RX_LOPHASE_MASK                                (0xFul << ANA_RX_LOPHASE_POS)
#define ANA_RX_LOPULSEWIDTH_ADDR                           (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_LOPULSEWIDTH_POS                            (17)     /*< bit[20:17]  */
#define ANA_RX_LOPULSEWIDTH_MASK                           (0xFul << ANA_RX_LOPULSEWIDTH_POS)
#define ANA_RX_MIX1GGMVSEL_ADDR                            (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_MIX1GGMVSEL_POS                             (21)     /*< bit[23:21]  */
#define ANA_RX_MIX1GGMVSEL_MASK                            (0x7ul << ANA_RX_MIX1GGMVSEL_POS)
#define ANA_RX_OPAMPVSEL_ADDR                              (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_OPAMPVSEL_POS                               (24)     /*< bit[26:24]  */
#define ANA_RX_OPAMPVSEL_MASK                              (0x7ul << ANA_RX_OPAMPVSEL_POS)
#define ANA_RX_PGABYPASSEN_ADDR                            (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_PGABYPASSEN_POS                             (27)     /*< bit[27]     */
#define ANA_RX_PGABYPASSEN_MASK                            (0x1ul << ANA_RX_PGABYPASSEN_POS)
#define ANA_RX_PGAFLIPI_ADDR                               (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_PGAFLIPI_POS                                (28)     /*< bit[28]     */
#define ANA_RX_PGAFLIPI_MASK                               (0x1ul << ANA_RX_PGAFLIPI_POS)
#define ANA_RX_PGAFLIPQ_ADDR                               (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_PGAFLIPQ_POS                                (29)     /*< bit[29]     */
#define ANA_RX_PGAFLIPQ_MASK                               (0x1ul << ANA_RX_PGAFLIPQ_POS)
#define ANA_RX_PGAFSETTLE_ADDR                             (uint32_t)&(ANA->RX_CONFIG_5)
#define ANA_RX_PGAFSETTLE_POS                              (30)     /*< bit[30]     */
#define ANA_RX_PGAFSETTLE_MASK                             (0x1ul << ANA_RX_PGAFSETTLE_POS)

#define ANA_RX_CONFIG_6_ADDR                               (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_PKDETCMPHYSEL_ADDR                          (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_PKDETCMPHYSEL_POS                           (0)      /*< bit[1:0]    */
#define ANA_RX_PKDETCMPHYSEL_MASK                          (0x3ul << ANA_RX_PKDETCMPHYSEL_POS)
#define ANA_RX_PKDETMODE_ADDR                              (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_PKDETMODE_POS                               (2)      /*< bit[2]      */
#define ANA_RX_PKDETMODE_MASK                              (0x1ul << ANA_RX_PKDETMODE_POS)
#define ANA_RX_PKDETTHVSEL_ADDR                            (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_PKDETTHVSEL_POS                             (3)      /*< bit[5:3]    */
#define ANA_RX_PKDETTHVSEL_MASK                            (0x7ul << ANA_RX_PKDETTHVSEL_POS)
#define ANA_RX_RESET_PKCAP_ADDR                            (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_RESET_PKCAP_POS                             (6)      /*< bit[6]      */
#define ANA_RX_RESET_PKCAP_MASK                            (0x1ul << ANA_RX_RESET_PKCAP_POS)
#define ANA_RX_RFAGC_HALT_ADDR                             (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_RFAGC_HALT_POS                              (7)      /*< bit[7]      */
#define ANA_RX_RFAGC_HALT_MASK                             (0x1ul << ANA_RX_RFAGC_HALT_POS)
#define ANA_RX_RFAGCTHHIGHVSEL_ADDR                        (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_RFAGCTHHIGHVSEL_POS                         (22)     /*< bit[24:22]  */
#define ANA_RX_RFAGCTHHIGHVSEL_MASK                        (0x7ul << ANA_RX_RFAGCTHHIGHVSEL_POS)
#define ANA_RX_RFAGCTHLOWVSEL_ADDR                         (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_RFAGCTHLOWVSEL_POS                          (25)     /*< bit[27:25]  */
#define ANA_RX_RFAGCTHLOWVSEL_MASK                         (0x7ul << ANA_RX_RFAGCTHLOWVSEL_POS)
#define ANA_RX_PKDETAUTOGENRST_ADDR                        (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_PKDETAUTOGENRST_POS                         (28)     /*< bit[28]     */
#define ANA_RX_PKDETAUTOGENRST_MASK                        (0x1ul << ANA_RX_PKDETAUTOGENRST_POS)
#define ANA_RX_PKDETLONGPULSE_ADDR                         (uint32_t)&(ANA->RX_CONFIG_6)
#define ANA_RX_PKDETLONGPULSE_POS                          (29)     /*< bit[29]     */
#define ANA_RX_PKDETLONGPULSE_MASK                         (0x1ul << ANA_RX_PKDETLONGPULSE_POS)

#define ANA_RX_CONFIG_7_ADDR                               (uint32_t)&(ANA->RX_CONFIG_7)
#define ANA_RX_CONFIG_8_ADDR                               (uint32_t)&(ANA->RX_CONFIG_8)
#define ANA_RX_FILTCALRST_N_ADDR                           (uint32_t)&(ANA->RX_CONFIG_8)
#define ANA_RX_FILTCALRST_N_POS                            (0)      /*< bit[0]      */
#define ANA_RX_FILTCALRST_N_MASK                           (0x1ul << ANA_RX_FILTCALRST_N_POS)
#define ANA_RX_FILTCALMODE_ADDR                            (uint32_t)&(ANA->RX_CONFIG_8)
#define ANA_RX_FILTCALMODE_POS                             (1)      /*< bit[1]      */
#define ANA_RX_FILTCALMODE_MASK                            (0x1ul << ANA_RX_FILTCALMODE_POS)
#define ANA_RX_ENFILTCAL12_ADDR                            (uint32_t)&(ANA->RX_CONFIG_8)
#define ANA_RX_ENFILTCAL12_POS                             (2)      /*< bit[2]      */
#define ANA_RX_ENFILTCAL12_MASK                            (0x1ul << ANA_RX_ENFILTCAL12_POS)
#define ANA_RX_FILTIFSHIFT_ADDR                            (uint32_t)&(ANA->RX_CONFIG_8)
#define ANA_RX_FILTIFSHIFT_POS                             (3)      /*< bit[4:3]    */
#define ANA_RX_FILTIFSHIFT_MASK                            (0x3ul << ANA_RX_FILTIFSHIFT_POS)
#define ANA_RX_PGAOPAMPDRV_ADDR                            (uint32_t)&(ANA->RX_CONFIG_8)
#define ANA_RX_PGAOPAMPDRV_POS                             (5)      /*< bit[7:5]    */
#define ANA_RX_PGAOPAMPDRV_MASK                            (0x7ul << ANA_RX_PGAOPAMPDRV_POS)

#define ANA_DIG_LF_OSC16K_CONFIG1_ADDR                     (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_PD_ADDR                          (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_PD_POS                           (0)      /*< bit[0]      */
#define ANA_DIG_LF_OSC16K_PD_MASK                          (0x1ul << ANA_DIG_LF_OSC16K_PD_POS)
#define ANA_DIG_LF_OSC16K_LDO_PD_ADDR                      (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_LDO_PD_POS                       (1)      /*< bit[1]      */
#define ANA_DIG_LF_OSC16K_LDO_PD_MASK                      (0x1ul << ANA_DIG_LF_OSC16K_LDO_PD_POS)
#define ANA_DIG_LF_OSC_0P6VSEL_ADDR                        (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC_0P6VSEL_POS                         (2)      /*< bit[2]      */
#define ANA_DIG_LF_OSC_0P6VSEL_MASK                        (0x1ul << ANA_DIG_LF_OSC_0P6VSEL_POS)
#define ANA_DIG_LF_OSC16K_XTRACAP_ADDR                     (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_XTRACAP_POS                      (3)      /*< bit[3]      */
#define ANA_DIG_LF_OSC16K_XTRACAP_MASK                     (0x1ul << ANA_DIG_LF_OSC16K_XTRACAP_POS)
#define ANA_DIG_LF_OSC16K_LDOCUR_ADDR                      (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_LDOCUR_POS                       (4)      /*< bit[5:4]    */
#define ANA_DIG_LF_OSC16K_LDOCUR_MASK                      (0x3ul << ANA_DIG_LF_OSC16K_LDOCUR_POS)
#define ANA_DIG_LF_OSC16K_LDOSEL_ADDR                      (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_LDOSEL_POS                       (6)      /*< bit[8:6]    */
#define ANA_DIG_LF_OSC16K_LDOSEL_MASK                      (0x7ul << ANA_DIG_LF_OSC16K_LDOSEL_POS)
#define ANA_DIG_LF_OSC16K_LDOTMP_ADDR                      (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_LDOTMP_POS                       (9)      /*< bit[11:9]   */
#define ANA_DIG_LF_OSC16K_LDOTMP_MASK                      (0x7ul << ANA_DIG_LF_OSC16K_LDOTMP_POS)
#define ANA_DIG_LF_OSC16K_ENAUXCORE_ADDR                   (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_ENAUXCORE_POS                    (12)     /*< bit[12]     */
#define ANA_DIG_LF_OSC16K_ENAUXCORE_MASK                   (0x1ul << ANA_DIG_LF_OSC16K_ENAUXCORE_POS)
#define ANA_DIG_LF_OSC_LDOHV_ADDR                          (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC_LDOHV_POS                           (13)     /*< bit[13]     */
#define ANA_DIG_LF_OSC_LDOHV_MASK                          (0x1ul << ANA_DIG_LF_OSC_LDOHV_POS)
#define ANA_DIG_LF_OSC16K_FREQCTRLSEL_ADDR                 (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG1)
#define ANA_DIG_LF_OSC16K_FREQCTRLSEL_POS                  (16)     /*< bit[27:16]  */
#define ANA_DIG_LF_OSC16K_FREQCTRLSEL_MASK                 (0xFFFul << ANA_DIG_LF_OSC16K_FREQCTRLSEL_POS)

#define ANA_DIG_LF_OSC16K_CONFIG2_ADDR                     (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG2)
#define ANA_DIG_LF_OSC16K_FREQCTRLSELROUT_ADDR             (uint32_t)&(ANA->DIG_LF_OSC16K_CONFIG2)
#define ANA_DIG_LF_OSC16K_FREQCTRLSELROUT_POS              (0)      /*< bit[11:0]   */
#define ANA_DIG_LF_OSC16K_FREQCTRLSELROUT_MASK             (0xFFFul << ANA_DIG_LF_OSC16K_FREQCTRLSELROUT_POS)

#define ANA_SYNTH_CAL_CONFIG_0_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_0)
#define ANA_SYNTH_CAL_CONFIG_1_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_1)
#define ANA_SYNTH_CAL_CONFIG_2_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_2)
#define ANA_SYNTH_CAL_RX_BIT_RATE_ADDR                     (uint32_t)&(ANA->SYNTH_CAL_CONFIG_2)
#define ANA_SYNTH_CAL_RX_BIT_RATE_POS                      (0)      /*< bit[23:0]   */
#define ANA_SYNTH_CAL_RX_BIT_RATE_MASK                     (0xFFFFFFul << ANA_SYNTH_CAL_RX_BIT_RATE_POS)
#define ANA_SYNTH_CAL_RX_OVERSAMPLE_RATE_EN_ADDR           (uint32_t)&(ANA->SYNTH_CAL_CONFIG_2)
#define ANA_SYNTH_CAL_RX_OVERSAMPLE_RATE_EN_POS            (24)     /*< bit[24]     */
#define ANA_SYNTH_CAL_RX_OVERSAMPLE_RATE_EN_MASK           (0x1ul << ANA_SYNTH_CAL_RX_OVERSAMPLE_RATE_EN_POS)

#define ANA_SYNTH_CAL_CONFIG_3_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_3)
#define ANA_SYNTH_CAL_IF_FLIP_EN_ADDR                      (uint32_t)&(ANA->SYNTH_CAL_CONFIG_3)
#define ANA_SYNTH_CAL_IF_FLIP_EN_POS                       (0)      /*< bit[0]      */
#define ANA_SYNTH_CAL_IF_FLIP_EN_MASK                      (0x1ul << ANA_SYNTH_CAL_IF_FLIP_EN_POS)
#define ANA_SYNTH_CAL_IF_FREQ_ADDR                         (uint32_t)&(ANA->SYNTH_CAL_CONFIG_3)
#define ANA_SYNTH_CAL_IF_FREQ_POS                          (4)      /*< bit[31:4]   */
#define ANA_SYNTH_CAL_IF_FREQ_MASK                         (0xFFFFFFFul << ANA_SYNTH_CAL_IF_FREQ_POS)

#define ANA_SYNTH_CAL_TRIG_ADDR                            (uint32_t)&(ANA->SYNTH_CAL_TRIG)
#define ANA_SYNTH_CAL_TRIGGER_ADDR                         (uint32_t)&(ANA->SYNTH_CAL_TRIG)
#define ANA_SYNTH_CAL_TRIGGER_POS                          (0)      /*< bit[0]      */
#define ANA_SYNTH_CAL_TRIGGER_MASK                         (0x1ul << ANA_SYNTH_CAL_TRIGGER_POS)

#define ANA_SYNTH_CAL_CONFIG_4_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_4)
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_IF_ADDR              (uint32_t)&(ANA->SYNTH_CAL_CONFIG_4)
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_IF_POS               (0)      /*< bit[17:0]   */
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_IF_MASK              (0x3FFFFul << ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_IF_POS)
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_IF_ADDR               (uint32_t)&(ANA->SYNTH_CAL_CONFIG_4)
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_IF_POS                (18)     /*< bit[22:18]  */
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_IF_MASK               (0x1Ful << ANA_SYNTH_CAL_TPMCAL_DIV_INT_IF_POS)

#define ANA_SYNTH_CAL_CONFIG_5_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_5)
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_HB_ADDR              (uint32_t)&(ANA->SYNTH_CAL_CONFIG_5)
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_HB_POS               (0)      /*< bit[17:0]   */
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_HB_MASK              (0x3FFFFul << ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_HB_POS)
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_HB_ADDR               (uint32_t)&(ANA->SYNTH_CAL_CONFIG_5)
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_HB_POS                (18)     /*< bit[22:18]  */
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_HB_MASK               (0x1Ful << ANA_SYNTH_CAL_TPMCAL_DIV_INT_HB_POS)

#define ANA_SYNTH_CAL_CONFIG_6_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_6)
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_LB_ADDR              (uint32_t)&(ANA->SYNTH_CAL_CONFIG_6)
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_LB_POS               (0)      /*< bit[17:0]   */
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_LB_MASK              (0x3FFFFul << ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_LB_POS)
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_LB_ADDR               (uint32_t)&(ANA->SYNTH_CAL_CONFIG_6)
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_LB_POS                (18)     /*< bit[22:18]  */
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_LB_MASK               (0x1Ful << ANA_SYNTH_CAL_TPMCAL_DIV_INT_LB_POS)

#define ANA_SYNTH_CAL_CONFIG_7_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_7)
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_RX_ADDR              (uint32_t)&(ANA->SYNTH_CAL_CONFIG_7)
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_RX_POS               (0)      /*< bit[17:0]   */
#define ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_RX_MASK              (0x3FFFFul << ANA_SYNTH_CAL_TPMCAL_DIV_FRAC_RX_POS)
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_RX_ADDR               (uint32_t)&(ANA->SYNTH_CAL_CONFIG_7)
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_RX_POS                (18)     /*< bit[22:18]  */
#define ANA_SYNTH_CAL_TPMCAL_DIV_INT_RX_MASK               (0x1Ful << ANA_SYNTH_CAL_TPMCAL_DIV_INT_RX_POS)

#define ANA_SYNTH_CAL_CONFIG_8_ADDR                        (uint32_t)&(ANA->SYNTH_CAL_CONFIG_8)
#define ANA_PMU_CTL_ADDR                                   (uint32_t)&(ANA->PMU_CTL)
#define ANA_PMU_EN_ADDR                                    (uint32_t)&(ANA->PMU_CTL)
#define ANA_PMU_EN_POS                                     (0)      /*< bit[0]      */
#define ANA_PMU_EN_MASK                                    (0x1ul << ANA_PMU_EN_POS)

#define ANA_PMU_STS_ADDR                                   (uint32_t)&(ANA->PMU_STS)
#define ANA_PMUSTS_SHUTDOWN_ADDR                           (uint32_t)&(ANA->PMU_STS)
#define ANA_PMUSTS_SHUTDOWN_POS                            (0)      /*< bit[0]      */
#define ANA_PMUSTS_SHUTDOWN_MASK                           (0x1ul << ANA_PMUSTS_SHUTDOWN_POS)
#define ANA_PMUSTS_ACTIVE_ADDR                             (uint32_t)&(ANA->PMU_STS)
#define ANA_PMUSTS_ACTIVE_POS                              (1)      /*< bit[1]      */
#define ANA_PMUSTS_ACTIVE_MASK                             (0x1ul << ANA_PMUSTS_ACTIVE_POS)

#define ANA_PMU_CONFIG_0_ADDR                              (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_XO_40M_PD_EN_ADDR                          (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_XO_40M_PD_EN_POS                           (0)      /*< bit[0]      */
#define ANA_PMU_XO_40M_PD_EN_MASK                          (0x1ul << ANA_PMU_XO_40M_PD_EN_POS)
#define ANA_PMU_XO_40M_GAIN_EN_ADDR                        (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_XO_40M_GAIN_EN_POS                         (1)      /*< bit[1]      */
#define ANA_PMU_XO_40M_GAIN_EN_MASK                        (0x1ul << ANA_PMU_XO_40M_GAIN_EN_POS)
#define ANA_PMU_BGR_PD_EN_ADDR                             (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_BGR_PD_EN_POS                              (2)      /*< bit[2]      */
#define ANA_PMU_BGR_PD_EN_MASK                             (0x1ul << ANA_PMU_BGR_PD_EN_POS)
#define ANA_PMU_DIG_LDO_PD_EN_ADDR                         (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_DIG_LDO_PD_EN_POS                          (3)      /*< bit[3]      */
#define ANA_PMU_DIG_LDO_PD_EN_MASK                         (0x1ul << ANA_PMU_DIG_LDO_PD_EN_POS)
#define ANA_PMU_DCDC_PD12_EN_ADDR                          (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_DCDC_PD12_EN_POS                           (4)      /*< bit[4]      */
#define ANA_PMU_DCDC_PD12_EN_MASK                          (0x1ul << ANA_PMU_DCDC_PD12_EN_POS)
#define ANA_PMU_DIG_LDOVDD_PG_PD_EN_ADDR                   (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_DIG_LDOVDD_PG_PD_EN_POS                    (5)      /*< bit[5]      */
#define ANA_PMU_DIG_LDOVDD_PG_PD_EN_MASK                   (0x1ul << ANA_PMU_DIG_LDOVDD_PG_PD_EN_POS)
#define ANA_PMU_DIG_ULPLDOVDD_LV_EN_ADDR                   (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_DIG_ULPLDOVDD_LV_EN_POS                    (6)      /*< bit[6]      */
#define ANA_PMU_DIG_ULPLDOVDD_LV_EN_MASK                   (0x1ul << ANA_PMU_DIG_ULPLDOVDD_LV_EN_POS)
#define ANA_PMU_FLASH_VREF_PD_EN_ADDR                      (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_FLASH_VREF_PD_EN_POS                       (9)      /*< bit[9]      */
#define ANA_PMU_FLASH_VREF_PD_EN_MASK                      (0x1ul << ANA_PMU_FLASH_VREF_PD_EN_POS)
#define ANA_PMU_EXT_FLASH_PD_EN_ADDR                       (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_EXT_FLASH_PD_EN_POS                        (10)     /*< bit[10]     */
#define ANA_PMU_EXT_FLASH_PD_EN_MASK                       (0x1ul << ANA_PMU_EXT_FLASH_PD_EN_POS)
#define ANA_PMU_16K_SHDN_EN_ADDR                           (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_16K_SHDN_EN_POS                            (11)     /*< bit[11]     */
#define ANA_PMU_16K_SHDN_EN_MASK                           (0x1ul << ANA_PMU_16K_SHDN_EN_POS)
#define ANA_PMU_XO_40M_BYCAP_EN_ADDR                       (uint32_t)&(ANA->PMU_CONFIG_0)
#define ANA_PMU_XO_40M_BYCAP_EN_POS                        (12)     /*< bit[12]     */
#define ANA_PMU_XO_40M_BYCAP_EN_MASK                       (0x1ul << ANA_PMU_XO_40M_BYCAP_EN_POS)

#define ANA_PMU_CONFIG_1_ADDR                              (uint32_t)&(ANA->PMU_CONFIG_1)
#define ANA_PMU_DIG_ULPLDOVDD_LV_S_ADDR                    (uint32_t)&(ANA->PMU_CONFIG_1)
#define ANA_PMU_DIG_ULPLDOVDD_LV_S_POS                     (0)      /*< bit[3:0]    */
#define ANA_PMU_DIG_ULPLDOVDD_LV_S_MASK                    (0xFul << ANA_PMU_DIG_ULPLDOVDD_LV_S_POS)

#define ANA_PMU_CONFIG_2_ADDR                              (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_S_LDO_ADDR                             (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_S_LDO_POS                              (0)      /*< bit[1:0]    */
#define ANA_PMU_TIM_S_LDO_MASK                             (0x3ul << ANA_PMU_TIM_S_LDO_POS)
#define ANA_PMU_TIM_A_LDO_ADDR                             (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_A_LDO_POS                              (4)      /*< bit[5:4]    */
#define ANA_PMU_TIM_A_LDO_MASK                             (0x3ul << ANA_PMU_TIM_A_LDO_POS)
#define ANA_PMU_TIM_S_DCDC_ADDR                            (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_S_DCDC_POS                             (8)      /*< bit[9:8]    */
#define ANA_PMU_TIM_S_DCDC_MASK                            (0x3ul << ANA_PMU_TIM_S_DCDC_POS)
#define ANA_PMU_TIM_A_DCDC_ADDR                            (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_A_DCDC_POS                             (12)     /*< bit[15:12]  */
#define ANA_PMU_TIM_A_DCDC_MASK                            (0xFul << ANA_PMU_TIM_A_DCDC_POS)
#define ANA_PMU_TIM_S_LDOVDD_LV_ADDR                       (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_S_LDOVDD_LV_POS                        (16)     /*< bit[17:16]  */
#define ANA_PMU_TIM_S_LDOVDD_LV_MASK                       (0x3ul << ANA_PMU_TIM_S_LDOVDD_LV_POS)
#define ANA_PMU_TIM_A_LDOVDD_LV_ADDR                       (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_A_LDOVDD_LV_POS                        (20)     /*< bit[21:20]  */
#define ANA_PMU_TIM_A_LDOVDD_LV_MASK                       (0x3ul << ANA_PMU_TIM_A_LDOVDD_LV_POS)
#define ANA_PMU_TIM_A_32K_ADDR                             (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_A_32K_POS                              (24)     /*< bit[25:24]  */
#define ANA_PMU_TIM_A_32K_MASK                             (0x3ul << ANA_PMU_TIM_A_32K_POS)
#define ANA_PMU_TIM_A_40M_GAIN_ADDR                        (uint32_t)&(ANA->PMU_CONFIG_2)
#define ANA_PMU_TIM_A_40M_GAIN_POS                         (28)     /*< bit[31:28]  */
#define ANA_PMU_TIM_A_40M_GAIN_MASK                        (0xFul << ANA_PMU_TIM_A_40M_GAIN_POS)

#define ANA_PMU_CONFIG_3_ADDR                              (uint32_t)&(ANA->PMU_CONFIG_3)
#define ANA_PMU_DCDC_TAR_ADDR                              (uint32_t)&(ANA->PMU_CONFIG_3)
#define ANA_PMU_DCDC_TAR_POS                               (0)      /*< bit[1:0]    */
#define ANA_PMU_DCDC_TAR_MASK                              (0x3ul << ANA_PMU_DCDC_TAR_POS)
#define ANA_PMU_DIG_ULPLDOVDD_LV_LOC_ADDR                  (uint32_t)&(ANA->PMU_CONFIG_3)
#define ANA_PMU_DIG_ULPLDOVDD_LV_LOC_POS                   (4)      /*< bit[4]      */
#define ANA_PMU_DIG_ULPLDOVDD_LV_LOC_MASK                  (0x1ul << ANA_PMU_DIG_ULPLDOVDD_LV_LOC_POS)
#define ANA_PMU_TIM_DCDC_BYPASS_ADDR                       (uint32_t)&(ANA->PMU_CONFIG_3)
#define ANA_PMU_TIM_DCDC_BYPASS_POS                        (8)      /*< bit[9:8]    */
#define ANA_PMU_TIM_DCDC_BYPASS_MASK                       (0x3ul << ANA_PMU_TIM_DCDC_BYPASS_POS)
#define ANA_PMU_DCDC_BYPASS_EN_ADDR                        (uint32_t)&(ANA->PMU_CONFIG_3)
#define ANA_PMU_DCDC_BYPASS_EN_POS                         (12)     /*< bit[15:12]  */
#define ANA_PMU_DCDC_BYPASS_EN_MASK                        (0xFul << ANA_PMU_DCDC_BYPASS_EN_POS)

#define ANA_PMU_DBG_REG_ADDR                               (uint32_t)&(ANA->PMU_DBG_REG)
#define ANA_PMU_DBG_SEL_ADDR                               (uint32_t)&(ANA->PMU_DBG_REG)
#define ANA_PMU_DBG_SEL_POS                                (0)      /*< bit[2:0]    */
#define ANA_PMU_DBG_SEL_MASK                               (0x7ul << ANA_PMU_DBG_SEL_POS)

#define ANA_ANCTL_CONFIG_0_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_0)
#define ANA_ANCTL_SEL_CTL_ADDR                             (uint32_t)&(ANA->ANCTL_CONFIG_0)
#define ANA_ANCTL_SEL_CTL_POS                              (0)      /*< bit[0]      */
#define ANA_ANCTL_SEL_CTL_MASK                             (0x1ul << ANA_ANCTL_SEL_CTL_POS)
#define ANA_ANCTL_OSC_SEL_CTL_ADDR                         (uint32_t)&(ANA->ANCTL_CONFIG_0)
#define ANA_ANCTL_OSC_SEL_CTL_POS                          (8)      /*< bit[8]      */
#define ANA_ANCTL_OSC_SEL_CTL_MASK                         (0x1ul << ANA_ANCTL_OSC_SEL_CTL_POS)
#define ANA_ANCTL_RX_SEL_CTL_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_0)
#define ANA_ANCTL_RX_SEL_CTL_POS                           (16)     /*< bit[16]     */
#define ANA_ANCTL_RX_SEL_CTL_MASK                          (0x1ul << ANA_ANCTL_RX_SEL_CTL_POS)

#define ANA_ANCTL_CONFIG_1_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_EN_ADDR                               (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_EN_POS                                (0)      /*< bit[0]      */
#define ANA_ANCTL_TX_EN_MASK                               (0x1ul << ANA_ANCTL_TX_EN_POS)
#define ANA_ANCTL_TX_RESERVED0_ADDR                        (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_RESERVED0_POS                         (1)      /*< bit[3:1]    */
#define ANA_ANCTL_TX_RESERVED0_MASK                        (0x7ul << ANA_ANCTL_TX_RESERVED0_POS)
#define ANA_ANCTL_TX_FREQ_ADDR                             (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_FREQ_POS                              (4)      /*< bit[4]      */
#define ANA_ANCTL_TX_FREQ_MASK                             (0x1ul << ANA_ANCTL_TX_FREQ_POS)
#define ANA_ANCTL_TX_MOD_ADDR                              (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_MOD_POS                               (5)      /*< bit[7:5]    */
#define ANA_ANCTL_TX_MOD_MASK                              (0x7ul << ANA_ANCTL_TX_MOD_POS)
#define ANA_ANCTL_TX_LEN_ADDR                              (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_LEN_POS                               (8)      /*< bit[14:8]   */
#define ANA_ANCTL_TX_LEN_MASK                              (0x7Ful << ANA_ANCTL_TX_LEN_POS)
#define ANA_ANCTL_TX_INV_ADDR                              (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_INV_POS                               (20)     /*< bit[20]     */
#define ANA_ANCTL_TX_INV_MASK                              (0x1ul << ANA_ANCTL_TX_INV_POS)
#define ANA_ANCTL_TX_REP_ADDR                              (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_REP_POS                               (22)     /*< bit[22]     */
#define ANA_ANCTL_TX_REP_MASK                              (0x1ul << ANA_ANCTL_TX_REP_POS)
#define ANA_ANCTL_TPM_CAL_EN_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TPM_CAL_EN_POS                           (23)     /*< bit[23]     */
#define ANA_ANCTL_TPM_CAL_EN_MASK                          (0x1ul << ANA_ANCTL_TPM_CAL_EN_POS)
#define ANA_ANCTL_SINGLE_EN_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_SINGLE_EN_POS                            (24)     /*< bit[24]     */
#define ANA_ANCTL_SINGLE_EN_MASK                           (0x1ul << ANA_ANCTL_SINGLE_EN_POS)
#define ANA_ANCTL_TX_COMPL_CNT_ADDR                        (uint32_t)&(ANA->ANCTL_CONFIG_1)
#define ANA_ANCTL_TX_COMPL_CNT_POS                         (28)     /*< bit[29:28]  */
#define ANA_ANCTL_TX_COMPL_CNT_MASK                        (0x3ul << ANA_ANCTL_TX_COMPL_CNT_POS)

#define ANA_ANCTL_CONFIG_2_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_2)
#define ANA_ANCTL_TX_BIT_RATE_ADDR                         (uint32_t)&(ANA->ANCTL_CONFIG_2)
#define ANA_ANCTL_TX_BIT_RATE_POS                          (0)      /*< bit[19:0]   */
#define ANA_ANCTL_TX_BIT_RATE_MASK                         (0xFFFFFul << ANA_ANCTL_TX_BIT_RATE_POS)

#define ANA_ANCTL_CONFIG_3_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_MAP_00_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_MAP_00_POS                            (0)      /*< bit[1:0]    */
#define ANA_ANCTL_TX_MAP_00_MASK                           (0x3ul << ANA_ANCTL_TX_MAP_00_POS)
#define ANA_ANCTL_TX_MAP_01_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_MAP_01_POS                            (2)      /*< bit[3:2]    */
#define ANA_ANCTL_TX_MAP_01_MASK                           (0x3ul << ANA_ANCTL_TX_MAP_01_POS)
#define ANA_ANCTL_TX_MAP_10_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_MAP_10_POS                            (4)      /*< bit[5:4]    */
#define ANA_ANCTL_TX_MAP_10_MASK                           (0x3ul << ANA_ANCTL_TX_MAP_10_POS)
#define ANA_ANCTL_TX_MAP_11_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_MAP_11_POS                            (6)      /*< bit[7:6]    */
#define ANA_ANCTL_TX_MAP_11_MASK                           (0x3ul << ANA_ANCTL_TX_MAP_11_POS)
#define ANA_ANCTL_TX_RUW4DAT_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_RUW4DAT_POS                           (15)     /*< bit[15]     */
#define ANA_ANCTL_TX_RUW4DAT_MASK                          (0x1ul << ANA_ANCTL_TX_RUW4DAT_POS)
#define ANA_ANCTL_TX_REP_IDX_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_REP_IDX_POS                           (16)     /*< bit[22:16]  */
#define ANA_ANCTL_TX_REP_IDX_MASK                          (0x7Ful << ANA_ANCTL_TX_REP_IDX_POS)
#define ANA_ANCTL_TX_D4VAL_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_D4VAL_POS                             (24)     /*< bit[24]     */
#define ANA_ANCTL_TX_D4VAL_MASK                            (0x1ul << ANA_ANCTL_TX_D4VAL_POS)
#define ANA_ANCTL_TX_STRWPROD_ADDR                         (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_STRWPROD_POS                          (25)     /*< bit[27:25]  */
#define ANA_ANCTL_TX_STRWPROD_MASK                         (0x7ul << ANA_ANCTL_TX_STRWPROD_POS)
#define ANA_ANCTL_TX_ENDWPROD_ADDR                         (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_ENDWPROD_POS                          (28)     /*< bit[30:28]  */
#define ANA_ANCTL_TX_ENDWPROD_MASK                         (0x7ul << ANA_ANCTL_TX_ENDWPROD_POS)
#define ANA_ANCTL_TX_RAMPTPM_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_3)
#define ANA_ANCTL_TX_RAMPTPM_POS                           (31)     /*< bit[31]     */
#define ANA_ANCTL_TX_RAMPTPM_MASK                          (0x1ul << ANA_ANCTL_TX_RAMPTPM_POS)

#define ANA_ANCTL_CONFIG_4_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_4)
#define ANA_ANCTL_RX_EN_ADDR                               (uint32_t)&(ANA->ANCTL_CONFIG_4)
#define ANA_ANCTL_RX_EN_POS                                (0)      /*< bit[0]      */
#define ANA_ANCTL_RX_EN_MASK                               (0x1ul << ANA_ANCTL_RX_EN_POS)
#define ANA_ANCTL_RX_RESERVED0_ADDR                        (uint32_t)&(ANA->ANCTL_CONFIG_4)
#define ANA_ANCTL_RX_RESERVED0_POS                         (1)      /*< bit[3:1]    */
#define ANA_ANCTL_RX_RESERVED0_MASK                        (0x7ul << ANA_ANCTL_RX_RESERVED0_POS)
#define ANA_ANCTL_RX_FREQ_ADDR                             (uint32_t)&(ANA->ANCTL_CONFIG_4)
#define ANA_ANCTL_RX_FREQ_POS                              (4)      /*< bit[4]      */
#define ANA_ANCTL_RX_FREQ_MASK                             (0x1ul << ANA_ANCTL_RX_FREQ_POS)

#define ANA_ANCTL_CONFIG_5_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_5)
#define ANA_ANCTL_RC_CAL_EN_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_5)
#define ANA_ANCTL_RC_CAL_EN_POS                            (0)      /*< bit[0]      */
#define ANA_ANCTL_RC_CAL_EN_MASK                           (0x1ul << ANA_ANCTL_RC_CAL_EN_POS)

#define ANA_ANCTL_CONFIG_6_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_6)
#define ANA_ANCTL_PA_TIM_ENIQ_ADDR                         (uint32_t)&(ANA->ANCTL_CONFIG_6)
#define ANA_ANCTL_PA_TIM_ENIQ_POS                          (10)     /*< bit[12:10]  */
#define ANA_ANCTL_PA_TIM_ENIQ_MASK                         (0x7ul << ANA_ANCTL_PA_TIM_ENIQ_POS)
#define ANA_ANCTL_RX_TIM_STR_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_6)
#define ANA_ANCTL_RX_TIM_STR_POS                           (15)     /*< bit[16:15]  */
#define ANA_ANCTL_RX_TIM_STR_MASK                          (0x3ul << ANA_ANCTL_RX_TIM_STR_POS)
#define ANA_ANCTL_RX_TIM_LDCAP_ADDR                        (uint32_t)&(ANA->ANCTL_CONFIG_6)
#define ANA_ANCTL_RX_TIM_LDCAP_POS                         (17)     /*< bit[18:17]  */
#define ANA_ANCTL_RX_TIM_LDCAP_MASK                        (0x3ul << ANA_ANCTL_RX_TIM_LDCAP_POS)
#define ANA_ANCTL_TIM_FS_TX_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_6)
#define ANA_ANCTL_TIM_FS_TX_POS                            (20)     /*< bit[21:20]  */
#define ANA_ANCTL_TIM_FS_TX_MASK                           (0x3ul << ANA_ANCTL_TIM_FS_TX_POS)
#define ANA_ANCTL_TIM_FS_RST_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_6)
#define ANA_ANCTL_TIM_FS_RST_POS                           (22)     /*< bit[24:22]  */
#define ANA_ANCTL_TIM_FS_RST_MASK                          (0x7ul << ANA_ANCTL_TIM_FS_RST_POS)
#define ANA_ANCTL_TIM_FS_PLL_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_6)
#define ANA_ANCTL_TIM_FS_PLL_POS                           (25)     /*< bit[28:25]  */
#define ANA_ANCTL_TIM_FS_PLL_MASK                          (0xFul << ANA_ANCTL_TIM_FS_PLL_POS)
#define ANA_ANCTL_RX_TIM_ADCWARMUP_ADDR                    (uint32_t)&(ANA->ANCTL_CONFIG_6)
#define ANA_ANCTL_RX_TIM_ADCWARMUP_POS                     (29)     /*< bit[31:29]  */
#define ANA_ANCTL_RX_TIM_ADCWARMUP_MASK                    (0x7ul << ANA_ANCTL_RX_TIM_ADCWARMUP_POS)

#define ANA_ANCTL_CONFIG_7_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_7)
#define ANA_ANCTL_TX_DAT_RD_DIS_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_7)
#define ANA_ANCTL_TX_DAT_RD_DIS_POS                        (0)      /*< bit[0]      */
#define ANA_ANCTL_TX_DAT_RD_DIS_MASK                       (0x1ul << ANA_ANCTL_TX_DAT_RD_DIS_POS)
#define ANA_ANCTL_TX_DAT_RD_EXT_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_7)
#define ANA_ANCTL_TX_DAT_RD_EXT_POS                        (1)      /*< bit[1]      */
#define ANA_ANCTL_TX_DAT_RD_EXT_MASK                       (0x1ul << ANA_ANCTL_TX_DAT_RD_EXT_POS)
#define ANA_ANCTL_TX_DAT_TOG_DIS_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_7)
#define ANA_ANCTL_TX_DAT_TOG_DIS_POS                       (2)      /*< bit[2]      */
#define ANA_ANCTL_TX_DAT_TOG_DIS_MASK                      (0x1ul << ANA_ANCTL_TX_DAT_TOG_DIS_POS)
#define ANA_ANCTL_TX_DAT_RD_CNT_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_7)
#define ANA_ANCTL_TX_DAT_RD_CNT_POS                        (4)      /*< bit[22:4]   */
#define ANA_ANCTL_TX_DAT_RD_CNT_MASK                       (0x7FFFFul << ANA_ANCTL_TX_DAT_RD_CNT_POS)
#define ANA_ANCTL_CONST_PWR_EN_ADDR                        (uint32_t)&(ANA->ANCTL_CONFIG_7)
#define ANA_ANCTL_CONST_PWR_EN_POS                         (24)     /*< bit[24]     */
#define ANA_ANCTL_CONST_PWR_EN_MASK                        (0x1ul << ANA_ANCTL_CONST_PWR_EN_POS)
#define ANA_ANCTL_PWR_CONST_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_7)
#define ANA_ANCTL_PWR_CONST_POS                            (25)     /*< bit[31:25]  */
#define ANA_ANCTL_PWR_CONST_MASK                           (0x7Ful << ANA_ANCTL_PWR_CONST_POS)

#define ANA_ANCTL_CONFIG_7_2_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_7_2)
#define ANA_ANCTL_RAMP_CLK_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_7_2)
#define ANA_ANCTL_RAMP_CLK_POS                             (0)      /*< bit[18:0]   */
#define ANA_ANCTL_RAMP_CLK_MASK                            (0x7FFFFul << ANA_ANCTL_RAMP_CLK_POS)
#define ANA_ANCTL_RAMP_NUM_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_7_2)
#define ANA_ANCTL_RAMP_NUM_POS                             (24)     /*< bit[31:24]  */
#define ANA_ANCTL_RAMP_NUM_MASK                            (0xFFul << ANA_ANCTL_RAMP_NUM_POS)

#define ANA_ANCTL_CONFIG_7_3_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_7_3)
#define ANA_ANCTL_PLL_LOCK_BYPASS_ADDR                     (uint32_t)&(ANA->ANCTL_CONFIG_7_3)
#define ANA_ANCTL_PLL_LOCK_BYPASS_POS                      (24)     /*< bit[24]     */
#define ANA_ANCTL_PLL_LOCK_BYPASS_MASK                     (0x1ul << ANA_ANCTL_PLL_LOCK_BYPASS_POS)
#define ANA_ANCTL_IRC_CPPLL_LOCK_BYPASS_ADDR               (uint32_t)&(ANA->ANCTL_CONFIG_7_3)
#define ANA_ANCTL_IRC_CPPLL_LOCK_BYPASS_POS                (25)     /*< bit[25]     */
#define ANA_ANCTL_IRC_CPPLL_LOCK_BYPASS_MASK               (0x1ul << ANA_ANCTL_IRC_CPPLL_LOCK_BYPASS_POS)
#define ANA_ANCTL_DUP_PLL_BYPASS_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_7_3)
#define ANA_ANCTL_DUP_PLL_BYPASS_POS                       (29)     /*< bit[29]     */
#define ANA_ANCTL_DUP_PLL_BYPASS_MASK                      (0x1ul << ANA_ANCTL_DUP_PLL_BYPASS_POS)
#define ANA_ANCTL_VDDDET_LOC_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_7_3)
#define ANA_ANCTL_VDDDET_LOC_POS                           (30)     /*< bit[30]     */
#define ANA_ANCTL_VDDDET_LOC_MASK                          (0x1ul << ANA_ANCTL_VDDDET_LOC_POS)

#define ANA_ANCTL_CONFIG_7_4_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_7_4)
#define ANA_ANCTL_TX_PKTMOD_GAP_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_7_4)
#define ANA_ANCTL_TX_PKTMOD_GAP_POS                        (0)      /*< bit[9:0]    */
#define ANA_ANCTL_TX_PKTMOD_GAP_MASK                       (0x3FFul << ANA_ANCTL_TX_PKTMOD_GAP_POS)
#define ANA_ANCTL_TX_PKTMOD_NUM_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_7_4)
#define ANA_ANCTL_TX_PKTMOD_NUM_POS                        (16)     /*< bit[23:16]  */
#define ANA_ANCTL_TX_PKTMOD_NUM_MASK                       (0xFFul << ANA_ANCTL_TX_PKTMOD_NUM_POS)
#define ANA_ANCTL_TX_PKTMOD_PDEN_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_7_4)
#define ANA_ANCTL_TX_PKTMOD_PDEN_POS                       (24)     /*< bit[24]     */
#define ANA_ANCTL_TX_PKTMOD_PDEN_MASK                      (0x1ul << ANA_ANCTL_TX_PKTMOD_PDEN_POS)

#define ANA_ANCTL_CONFIG_VOL_DET_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_EN_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_EN_POS                           (0)      /*< bit[0]      */
#define ANA_ANCTL_VOL_DET_EN_MASK                          (0x1ul << ANA_ANCTL_VOL_DET_EN_POS)
#define ANA_ANCTL_VOL_DET_PD_EN_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_PD_EN_POS                        (1)      /*< bit[1]      */
#define ANA_ANCTL_VOL_DET_PD_EN_MASK                       (0x1ul << ANA_ANCTL_VOL_DET_PD_EN_POS)
#define ANA_ANCTL_VOL_DET_RST_N_EN_ADDR                    (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_RST_N_EN_POS                     (2)      /*< bit[2]      */
#define ANA_ANCTL_VOL_DET_RST_N_EN_MASK                    (0x1ul << ANA_ANCTL_VOL_DET_RST_N_EN_POS)
#define ANA_ANCTL_VOL_DET_BUF_EN_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_BUF_EN_POS                       (3)      /*< bit[3]      */
#define ANA_ANCTL_VOL_DET_BUF_EN_MASK                      (0x1ul << ANA_ANCTL_VOL_DET_BUF_EN_POS)
#define ANA_ANCTL_VOL_DET_AAF_EN_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_AAF_EN_POS                       (4)      /*< bit[4]      */
#define ANA_ANCTL_VOL_DET_AAF_EN_MASK                      (0x1ul << ANA_ANCTL_VOL_DET_AAF_EN_POS)
#define ANA_ANCTL_VOL_DET_EDGE_R_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_EDGE_R_POS                       (5)      /*< bit[5]      */
#define ANA_ANCTL_VOL_DET_EDGE_R_MASK                      (0x1ul << ANA_ANCTL_VOL_DET_EDGE_R_POS)
#define ANA_ANCTL_VOL_DET_VIN_MUX_SEL_ADDR                 (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_VIN_MUX_SEL_POS                  (8)      /*< bit[11:8]   */
#define ANA_ANCTL_VOL_DET_VIN_MUX_SEL_MASK                 (0xFul << ANA_ANCTL_VOL_DET_VIN_MUX_SEL_POS)
#define ANA_ANCTL_VOL_DET_CUR_ADDR                         (uint32_t)&(ANA->ANCTL_CONFIG_VOL_DET)
#define ANA_ANCTL_VOL_DET_CUR_POS                          (16)     /*< bit[27:16]  */
#define ANA_ANCTL_VOL_DET_CUR_MASK                         (0xFFFul << ANA_ANCTL_VOL_DET_CUR_POS)

#define ANA_ANCTL_CONFIG_7_5_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_FS_FREQSYNTH_SEL_ADDR                    (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_FS_FREQSYNTH_SEL_POS                     (0)      /*< bit[0]      */
#define ANA_ANCTL_FS_FREQSYNTH_SEL_MASK                    (0x1ul << ANA_ANCTL_FS_FREQSYNTH_SEL_POS)
#define ANA_ANCTL_FS_FREQSYNTH_PD_TX_EN_ADDR               (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_FS_FREQSYNTH_PD_TX_EN_POS                (1)      /*< bit[1]      */
#define ANA_ANCTL_FS_FREQSYNTH_PD_TX_EN_MASK               (0x1ul << ANA_ANCTL_FS_FREQSYNTH_PD_TX_EN_POS)
#define ANA_ANCTL_FS_FREQSYNTH_RST_N_TX_EN_ADDR            (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_FS_FREQSYNTH_RST_N_TX_EN_POS             (2)      /*< bit[2]      */
#define ANA_ANCTL_FS_FREQSYNTH_RST_N_TX_EN_MASK            (0x1ul << ANA_ANCTL_FS_FREQSYNTH_RST_N_TX_EN_POS)
#define ANA_ANCTL_TPM_DMI_EN_TX_EN_ADDR                    (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_TPM_DMI_EN_TX_EN_POS                     (3)      /*< bit[3]      */
#define ANA_ANCTL_TPM_DMI_EN_TX_EN_MASK                    (0x1ul << ANA_ANCTL_TPM_DMI_EN_TX_EN_POS)
#define ANA_ANCTL_FS_FREQSYNTH_PD_RX_EN_ADDR               (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_FS_FREQSYNTH_PD_RX_EN_POS                (4)      /*< bit[4]      */
#define ANA_ANCTL_FS_FREQSYNTH_PD_RX_EN_MASK               (0x1ul << ANA_ANCTL_FS_FREQSYNTH_PD_RX_EN_POS)
#define ANA_ANCTL_FS_FREQSYNTH_RST_N_RX_EN_ADDR            (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_FS_FREQSYNTH_RST_N_RX_EN_POS             (5)      /*< bit[5]      */
#define ANA_ANCTL_FS_FREQSYNTH_RST_N_RX_EN_MASK            (0x1ul << ANA_ANCTL_FS_FREQSYNTH_RST_N_RX_EN_POS)
#define ANA_ANCTL_DASARADC_PD_SEL_ADDR                     (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_DASARADC_PD_SEL_POS                      (8)      /*< bit[8]      */
#define ANA_ANCTL_DASARADC_PD_SEL_MASK                     (0x1ul << ANA_ANCTL_DASARADC_PD_SEL_POS)
#define ANA_ANCTL_DASARADC_PD_EN_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_DASARADC_PD_EN_POS                       (9)      /*< bit[9]      */
#define ANA_ANCTL_DASARADC_PD_EN_MASK                      (0x1ul << ANA_ANCTL_DASARADC_PD_EN_POS)
#define ANA_ANCTL_DASARADC_RST_N_EN_ADDR                   (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_DASARADC_RST_N_EN_POS                    (10)     /*< bit[10]     */
#define ANA_ANCTL_DASARADC_RST_N_EN_MASK                   (0x1ul << ANA_ANCTL_DASARADC_RST_N_EN_POS)
#define ANA_ANCTL_ASARADC_PD_SEL_ADDR                      (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_ASARADC_PD_SEL_POS                       (12)     /*< bit[12]     */
#define ANA_ANCTL_ASARADC_PD_SEL_MASK                      (0x1ul << ANA_ANCTL_ASARADC_PD_SEL_POS)
#define ANA_ANCTL_ASARADC_PD_EN_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_ASARADC_PD_EN_POS                        (13)     /*< bit[13]     */
#define ANA_ANCTL_ASARADC_PD_EN_MASK                       (0x1ul << ANA_ANCTL_ASARADC_PD_EN_POS)
#define ANA_ANCTL_ASARADC_RST_N_EN_ADDR                    (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_ASARADC_RST_N_EN_POS                     (14)     /*< bit[14]     */
#define ANA_ANCTL_ASARADC_RST_N_EN_MASK                    (0x1ul << ANA_ANCTL_ASARADC_RST_N_EN_POS)
#define ANA_ANCTL_IRC_CPPLL_PD_SEL_ADDR                    (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_IRC_CPPLL_PD_SEL_POS                     (16)     /*< bit[16]     */
#define ANA_ANCTL_IRC_CPPLL_PD_SEL_MASK                    (0x1ul << ANA_ANCTL_IRC_CPPLL_PD_SEL_POS)
#define ANA_ANCTL_IRC_CPPLL_PD_EN_ADDR                     (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_IRC_CPPLL_PD_EN_POS                      (17)     /*< bit[17]     */
#define ANA_ANCTL_IRC_CPPLL_PD_EN_MASK                     (0x1ul << ANA_ANCTL_IRC_CPPLL_PD_EN_POS)
#define ANA_ANCTL_IRC_CPPLL_RST_N_EN_ADDR                  (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_IRC_CPPLL_RST_N_EN_POS                   (18)     /*< bit[18]     */
#define ANA_ANCTL_IRC_CPPLL_RST_N_EN_MASK                  (0x1ul << ANA_ANCTL_IRC_CPPLL_RST_N_EN_POS)
#define ANA_ANCTL_TPM_DMI_EN_RX_EN_ADDR                    (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_TPM_DMI_EN_RX_EN_POS                     (24)     /*< bit[24]     */
#define ANA_ANCTL_TPM_DMI_EN_RX_EN_MASK                    (0x1ul << ANA_ANCTL_TPM_DMI_EN_RX_EN_POS)
#define ANA_ANCTL_FS_AFC_EN_TPM_OPT_EN_ADDR                (uint32_t)&(ANA->ANCTL_CONFIG_7_5)
#define ANA_ANCTL_FS_AFC_EN_TPM_OPT_EN_POS                 (26)     /*< bit[26]     */
#define ANA_ANCTL_FS_AFC_EN_TPM_OPT_EN_MASK                (0x1ul << ANA_ANCTL_FS_AFC_EN_TPM_OPT_EN_POS)

#define ANA_ANCTL_CONFIG_7_6_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_7_6)
#define ANA_ANCTL_OSC16K_CAL_EN_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_7_6)
#define ANA_ANCTL_OSC16K_CAL_EN_POS                        (1)      /*< bit[1]      */
#define ANA_ANCTL_OSC16K_CAL_EN_MASK                       (0x1ul << ANA_ANCTL_OSC16K_CAL_EN_POS)

#define ANA_ANCTL_CONFIG_BBP_ADDR                          (uint32_t)&(ANA->ANCTL_CONFIG_BBP)
#define ANA_ANCTL_GAIN_SEL_BBP_ADDR                        (uint32_t)&(ANA->ANCTL_CONFIG_BBP)
#define ANA_ANCTL_GAIN_SEL_BBP_POS                         (0)      /*< bit[0]      */
#define ANA_ANCTL_GAIN_SEL_BBP_MASK                        (0x1ul << ANA_ANCTL_GAIN_SEL_BBP_POS)
#define ANA_ANCTL_BBP_IQCAL_EN_ADDR                        (uint32_t)&(ANA->ANCTL_CONFIG_BBP)
#define ANA_ANCTL_BBP_IQCAL_EN_POS                         (1)      /*< bit[1]      */
#define ANA_ANCTL_BBP_IQCAL_EN_MASK                        (0x1ul << ANA_ANCTL_BBP_IQCAL_EN_POS)
#define ANA_ANCTL_SEL_RX_FORCELNAGAIN_ADDR                 (uint32_t)&(ANA->ANCTL_CONFIG_BBP)
#define ANA_ANCTL_SEL_RX_FORCELNAGAIN_POS                  (2)      /*< bit[2]      */
#define ANA_ANCTL_SEL_RX_FORCELNAGAIN_MASK                 (0x1ul << ANA_ANCTL_SEL_RX_FORCELNAGAIN_POS)
#define ANA_ANCTL_SEL_RX_RFAGC_HALT_ADDR                   (uint32_t)&(ANA->ANCTL_CONFIG_BBP)
#define ANA_ANCTL_SEL_RX_RFAGC_HALT_POS                    (3)      /*< bit[3]      */
#define ANA_ANCTL_SEL_RX_RFAGC_HALT_MASK                   (0x1ul << ANA_ANCTL_SEL_RX_RFAGC_HALT_POS)
#define ANA_ANCTL_GAIN_KEEP_BBP_ADDR                       (uint32_t)&(ANA->ANCTL_CONFIG_BBP)
#define ANA_ANCTL_GAIN_KEEP_BBP_POS                        (4)      /*< bit[4]      */
#define ANA_ANCTL_GAIN_KEEP_BBP_MASK                       (0x1ul << ANA_ANCTL_GAIN_KEEP_BBP_POS)
#define ANA_ANCTL_RXEND_NOTW4_BBP_ADDR                     (uint32_t)&(ANA->ANCTL_CONFIG_BBP)
#define ANA_ANCTL_RXEND_NOTW4_BBP_POS                      (5)      /*< bit[5]      */
#define ANA_ANCTL_RXEND_NOTW4_BBP_MASK                     (0x1ul << ANA_ANCTL_RXEND_NOTW4_BBP_POS)

#define ANA_ANCTL_CONFIG_8_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_8)
#define ANA_ANCTL_CONFIG_9_ADDR                            (uint32_t)&(ANA->ANCTL_CONFIG_9)
#define ANA_ANCTL_CONFIG_10_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_10)
#define ANA_ANCTL_CONFIG_11_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_11)
#define ANA_ANCTL_DCDC_REG0_ADDR                           (uint32_t)&(ANA->ANCTL_DCDC_REG0)
#define ANA_ANCTL_DCDC_EN_IPN_0_NV_ADDR                    (uint32_t)&(ANA->ANCTL_DCDC_REG0)
#define ANA_ANCTL_DCDC_EN_IPN_0_NV_POS                     (0)      /*< bit[1:0]    */
#define ANA_ANCTL_DCDC_EN_IPN_0_NV_MASK                    (0x3ul << ANA_ANCTL_DCDC_EN_IPN_0_NV_POS)
#define ANA_ANCTL_DCDC_EXTRA12_CP_0_NV_ADDR                (uint32_t)&(ANA->ANCTL_DCDC_REG0)
#define ANA_ANCTL_DCDC_EXTRA12_CP_0_NV_POS                 (2)      /*< bit[3:2]    */
#define ANA_ANCTL_DCDC_EXTRA12_CP_0_NV_MASK                (0x3ul << ANA_ANCTL_DCDC_EXTRA12_CP_0_NV_POS)
#define ANA_ANCTL_DCDC_EN_RANDOM12_0_NV_ADDR               (uint32_t)&(ANA->ANCTL_DCDC_REG0)
#define ANA_ANCTL_DCDC_EN_RANDOM12_0_NV_POS                (4)      /*< bit[4]      */
#define ANA_ANCTL_DCDC_EN_RANDOM12_0_NV_MASK               (0x1ul << ANA_ANCTL_DCDC_EN_RANDOM12_0_NV_POS)
#define ANA_ANCTL_DCDC_SW12_F_0_NV_ADDR                    (uint32_t)&(ANA->ANCTL_DCDC_REG0)
#define ANA_ANCTL_DCDC_SW12_F_0_NV_POS                     (6)      /*< bit[8:6]    */
#define ANA_ANCTL_DCDC_SW12_F_0_NV_MASK                    (0x7ul << ANA_ANCTL_DCDC_SW12_F_0_NV_POS)
#define ANA_ANCTL_DCDC_VSEL_0_NV_ADDR                      (uint32_t)&(ANA->ANCTL_DCDC_REG0)
#define ANA_ANCTL_DCDC_VSEL_0_NV_POS                       (9)      /*< bit[11:9]   */
#define ANA_ANCTL_DCDC_VSEL_0_NV_MASK                      (0x7ul << ANA_ANCTL_DCDC_VSEL_0_NV_POS)

#define ANA_ANCTL_DCDC_REG1_ADDR                           (uint32_t)&(ANA->ANCTL_DCDC_REG1)
#define ANA_ANCTL_DCDC_EN_IPN_1_NV_ADDR                    (uint32_t)&(ANA->ANCTL_DCDC_REG1)
#define ANA_ANCTL_DCDC_EN_IPN_1_NV_POS                     (0)      /*< bit[1:0]    */
#define ANA_ANCTL_DCDC_EN_IPN_1_NV_MASK                    (0x3ul << ANA_ANCTL_DCDC_EN_IPN_1_NV_POS)
#define ANA_ANCTL_DCDC_EXTRA12_CP_1_NV_ADDR                (uint32_t)&(ANA->ANCTL_DCDC_REG1)
#define ANA_ANCTL_DCDC_EXTRA12_CP_1_NV_POS                 (2)      /*< bit[3:2]    */
#define ANA_ANCTL_DCDC_EXTRA12_CP_1_NV_MASK                (0x3ul << ANA_ANCTL_DCDC_EXTRA12_CP_1_NV_POS)
#define ANA_ANCTL_DCDC_EN_RANDOM12_1_NV_ADDR               (uint32_t)&(ANA->ANCTL_DCDC_REG1)
#define ANA_ANCTL_DCDC_EN_RANDOM12_1_NV_POS                (4)      /*< bit[4]      */
#define ANA_ANCTL_DCDC_EN_RANDOM12_1_NV_MASK               (0x1ul << ANA_ANCTL_DCDC_EN_RANDOM12_1_NV_POS)
#define ANA_ANCTL_DCDC_SW12_F_1_NV_ADDR                    (uint32_t)&(ANA->ANCTL_DCDC_REG1)
#define ANA_ANCTL_DCDC_SW12_F_1_NV_POS                     (6)      /*< bit[8:6]    */
#define ANA_ANCTL_DCDC_SW12_F_1_NV_MASK                    (0x7ul << ANA_ANCTL_DCDC_SW12_F_1_NV_POS)
#define ANA_ANCTL_DCDC_VSEL_1_NV_ADDR                      (uint32_t)&(ANA->ANCTL_DCDC_REG1)
#define ANA_ANCTL_DCDC_VSEL_1_NV_POS                       (9)      /*< bit[11:9]   */
#define ANA_ANCTL_DCDC_VSEL_1_NV_MASK                      (0x7ul << ANA_ANCTL_DCDC_VSEL_1_NV_POS)

#define ANA_ANCTL_DCDC_REG2_ADDR                           (uint32_t)&(ANA->ANCTL_DCDC_REG2)
#define ANA_ANCTL_DCDC_EN_IPN_2_NV_ADDR                    (uint32_t)&(ANA->ANCTL_DCDC_REG2)
#define ANA_ANCTL_DCDC_EN_IPN_2_NV_POS                     (0)      /*< bit[1:0]    */
#define ANA_ANCTL_DCDC_EN_IPN_2_NV_MASK                    (0x3ul << ANA_ANCTL_DCDC_EN_IPN_2_NV_POS)
#define ANA_ANCTL_DCDC_EXTRA12_CP_2_NV_ADDR                (uint32_t)&(ANA->ANCTL_DCDC_REG2)
#define ANA_ANCTL_DCDC_EXTRA12_CP_2_NV_POS                 (2)      /*< bit[3:2]    */
#define ANA_ANCTL_DCDC_EXTRA12_CP_2_NV_MASK                (0x3ul << ANA_ANCTL_DCDC_EXTRA12_CP_2_NV_POS)
#define ANA_ANCTL_DCDC_EN_RANDOM12_2_NV_ADDR               (uint32_t)&(ANA->ANCTL_DCDC_REG2)
#define ANA_ANCTL_DCDC_EN_RANDOM12_2_NV_POS                (4)      /*< bit[4]      */
#define ANA_ANCTL_DCDC_EN_RANDOM12_2_NV_MASK               (0x1ul << ANA_ANCTL_DCDC_EN_RANDOM12_2_NV_POS)
#define ANA_ANCTL_DCDC_SW12_F_2_NV_ADDR                    (uint32_t)&(ANA->ANCTL_DCDC_REG2)
#define ANA_ANCTL_DCDC_SW12_F_2_NV_POS                     (6)      /*< bit[8:6]    */
#define ANA_ANCTL_DCDC_SW12_F_2_NV_MASK                    (0x7ul << ANA_ANCTL_DCDC_SW12_F_2_NV_POS)
#define ANA_ANCTL_DCDC_VSEL_2_NV_ADDR                      (uint32_t)&(ANA->ANCTL_DCDC_REG2)
#define ANA_ANCTL_DCDC_VSEL_2_NV_POS                       (9)      /*< bit[11:9]   */
#define ANA_ANCTL_DCDC_VSEL_2_NV_MASK                      (0x7ul << ANA_ANCTL_DCDC_VSEL_2_NV_POS)

#define ANA_ANCTL_DCDC_REG3_ADDR                           (uint32_t)&(ANA->ANCTL_DCDC_REG3)
#define ANA_ANCTL_DCDC_TAR_IDLE_ADDR                       (uint32_t)&(ANA->ANCTL_DCDC_REG3)
#define ANA_ANCTL_DCDC_TAR_IDLE_POS                        (0)      /*< bit[1:0]    */
#define ANA_ANCTL_DCDC_TAR_IDLE_MASK                       (0x3ul << ANA_ANCTL_DCDC_TAR_IDLE_POS)
#define ANA_ANCTL_DCDC_TAR_TX_1G_ADDR                      (uint32_t)&(ANA->ANCTL_DCDC_REG3)
#define ANA_ANCTL_DCDC_TAR_TX_1G_POS                       (4)      /*< bit[5:4]    */
#define ANA_ANCTL_DCDC_TAR_TX_1G_MASK                      (0x3ul << ANA_ANCTL_DCDC_TAR_TX_1G_POS)
#define ANA_ANCTL_DCDC_TAR_RX_1G_ADDR                      (uint32_t)&(ANA->ANCTL_DCDC_REG3)
#define ANA_ANCTL_DCDC_TAR_RX_1G_POS                       (8)      /*< bit[9:8]    */
#define ANA_ANCTL_DCDC_TAR_RX_1G_MASK                      (0x3ul << ANA_ANCTL_DCDC_TAR_RX_1G_POS)
#define ANA_ANCTL_DCDC_TAR_TX_2G_ADDR                      (uint32_t)&(ANA->ANCTL_DCDC_REG3)
#define ANA_ANCTL_DCDC_TAR_TX_2G_POS                       (12)     /*< bit[13:12]  */
#define ANA_ANCTL_DCDC_TAR_TX_2G_MASK                      (0x3ul << ANA_ANCTL_DCDC_TAR_TX_2G_POS)
#define ANA_ANCTL_DCDC_TAR_RX_2G_ADDR                      (uint32_t)&(ANA->ANCTL_DCDC_REG3)
#define ANA_ANCTL_DCDC_TAR_RX_2G_POS                       (16)     /*< bit[17:16]  */
#define ANA_ANCTL_DCDC_TAR_RX_2G_MASK                      (0x3ul << ANA_ANCTL_DCDC_TAR_RX_2G_POS)

#define ANA_ANCTL_PA1G_REG0_ADDR                           (uint32_t)&(ANA->ANCTL_PA1G_REG0)
#define ANA_ANCTL_RF_LDO_SEL_RX_ADDR                       (uint32_t)&(ANA->ANCTL_PA1G_REG0)
#define ANA_ANCTL_RF_LDO_SEL_RX_POS                        (0)      /*< bit[1:0]    */
#define ANA_ANCTL_RF_LDO_SEL_RX_MASK                       (0x3ul << ANA_ANCTL_RF_LDO_SEL_RX_POS)
#define ANA_ANCTL_RF_LDO_SEL_TX_ADDR                       (uint32_t)&(ANA->ANCTL_PA1G_REG0)
#define ANA_ANCTL_RF_LDO_SEL_TX_POS                        (2)      /*< bit[3:2]    */
#define ANA_ANCTL_RF_LDO_SEL_TX_MASK                       (0x3ul << ANA_ANCTL_RF_LDO_SEL_TX_POS)
#define ANA_ANCTL_RF_LDO_2G_EN_ADDR                        (uint32_t)&(ANA->ANCTL_PA1G_REG0)
#define ANA_ANCTL_RF_LDO_2G_EN_POS                         (8)      /*< bit[8]      */
#define ANA_ANCTL_RF_LDO_2G_EN_MASK                        (0x1ul << ANA_ANCTL_RF_LDO_2G_EN_POS)
#define ANA_ANCTL_EXT_PA_EN_ADDR                           (uint32_t)&(ANA->ANCTL_PA1G_REG0)
#define ANA_ANCTL_EXT_PA_EN_POS                            (12)     /*< bit[12]     */
#define ANA_ANCTL_EXT_PA_EN_MASK                           (0x1ul << ANA_ANCTL_EXT_PA_EN_POS)
#define ANA_ANCTL_EXT_PA_LOC_ADDR                          (uint32_t)&(ANA->ANCTL_PA1G_REG0)
#define ANA_ANCTL_EXT_PA_LOC_POS                           (13)     /*< bit[13]     */
#define ANA_ANCTL_EXT_PA_LOC_MASK                          (0x1ul << ANA_ANCTL_EXT_PA_LOC_POS)
#define ANA_ANCTL_PA_PWRDLY_PROD_ADDR                      (uint32_t)&(ANA->ANCTL_PA1G_REG0)
#define ANA_ANCTL_PA_PWRDLY_PROD_POS                       (16)     /*< bit[23:16]  */
#define ANA_ANCTL_PA_PWRDLY_PROD_MASK                      (0xFFul << ANA_ANCTL_PA_PWRDLY_PROD_POS)

#define ANA_ANCTL_FS_REG0_ADDR                             (uint32_t)&(ANA->ANCTL_FS_REG0)
#define ANA_ANCTL_FS_IQBUF_ICTRL_RX_ADDR                   (uint32_t)&(ANA->ANCTL_FS_REG0)
#define ANA_ANCTL_FS_IQBUF_ICTRL_RX_POS                    (0)      /*< bit[2:0]    */
#define ANA_ANCTL_FS_IQBUF_ICTRL_RX_MASK                   (0x7ul << ANA_ANCTL_FS_IQBUF_ICTRL_RX_POS)
#define ANA_ANCTL_FS_IQBUF_ICTRL_TX_ADDR                   (uint32_t)&(ANA->ANCTL_FS_REG0)
#define ANA_ANCTL_FS_IQBUF_ICTRL_TX_POS                    (4)      /*< bit[6:4]    */
#define ANA_ANCTL_FS_IQBUF_ICTRL_TX_MASK                   (0x7ul << ANA_ANCTL_FS_IQBUF_ICTRL_TX_POS)
#define ANA_ANCTL_FS_CP_ICTRL_TXSET_ADDR                   (uint32_t)&(ANA->ANCTL_FS_REG0)
#define ANA_ANCTL_FS_CP_ICTRL_TXSET_POS                    (8)      /*< bit[10:8]   */
#define ANA_ANCTL_FS_CP_ICTRL_TXSET_MASK                   (0x7ul << ANA_ANCTL_FS_CP_ICTRL_TXSET_POS)
#define ANA_ANCTL_FS_CP_ICTRL_RXSET_ADDR                   (uint32_t)&(ANA->ANCTL_FS_REG0)
#define ANA_ANCTL_FS_CP_ICTRL_RXSET_POS                    (12)     /*< bit[14:12]  */
#define ANA_ANCTL_FS_CP_ICTRL_RXSET_MASK                   (0x7ul << ANA_ANCTL_FS_CP_ICTRL_RXSET_POS)

#define ANA_ANCTL_FS_REG1_ADDR                             (uint32_t)&(ANA->ANCTL_FS_REG1)
#define ANA_ANCTL_DIS_FS_IQBUF_ICTRL_ADDR                  (uint32_t)&(ANA->ANCTL_FS_REG1)
#define ANA_ANCTL_DIS_FS_IQBUF_ICTRL_POS                   (0)      /*< bit[0]      */
#define ANA_ANCTL_DIS_FS_IQBUF_ICTRL_MASK                  (0x1ul << ANA_ANCTL_DIS_FS_IQBUF_ICTRL_POS)
#define ANA_ANCTL_DIS_FS_CP_ICTRL_ADDR                     (uint32_t)&(ANA->ANCTL_FS_REG1)
#define ANA_ANCTL_DIS_FS_CP_ICTRL_POS                      (1)      /*< bit[1]      */
#define ANA_ANCTL_DIS_FS_CP_ICTRL_MASK                     (0x1ul << ANA_ANCTL_DIS_FS_CP_ICTRL_POS)

#define ANA_ANCTL_WB_REG0_ADDR                             (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_FS_RX_WB_ADDR                        (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_FS_RX_WB_POS                         (0)      /*< bit[0]      */
#define ANA_ANCTL_DIS_FS_RX_WB_MASK                        (0x1ul << ANA_ANCTL_DIS_FS_RX_WB_POS)
#define ANA_ANCTL_DIS_FS_TX_WB_ADDR                        (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_FS_TX_WB_POS                         (1)      /*< bit[1]      */
#define ANA_ANCTL_DIS_FS_TX_WB_MASK                        (0x1ul << ANA_ANCTL_DIS_FS_TX_WB_POS)
#define ANA_ANCTL_DIS_ASARADC_CFG_WB_ADDR                  (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_ASARADC_CFG_WB_POS                   (4)      /*< bit[4]      */
#define ANA_ANCTL_DIS_ASARADC_CFG_WB_MASK                  (0x1ul << ANA_ANCTL_DIS_ASARADC_CFG_WB_POS)
#define ANA_ANCTL_DIS_ASARADC_PDRSTN_WB_ADDR               (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_ASARADC_PDRSTN_WB_POS                (5)      /*< bit[5]      */
#define ANA_ANCTL_DIS_ASARADC_PDRSTN_WB_MASK               (0x1ul << ANA_ANCTL_DIS_ASARADC_PDRSTN_WB_POS)
#define ANA_ANCTL_DIS_TPM_CAL_EN_WB_ADDR                   (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_TPM_CAL_EN_WB_POS                    (24)     /*< bit[24]     */
#define ANA_ANCTL_DIS_TPM_CAL_EN_WB_MASK                   (0x1ul << ANA_ANCTL_DIS_TPM_CAL_EN_WB_POS)
#define ANA_ANCTL_DIS_BBP_IQCAL_EN_WB_ADDR                 (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_BBP_IQCAL_EN_WB_POS                  (25)     /*< bit[25]     */
#define ANA_ANCTL_DIS_BBP_IQCAL_EN_WB_MASK                 (0x1ul << ANA_ANCTL_DIS_BBP_IQCAL_EN_WB_POS)
#define ANA_ANCTL_DIS_TPM_TEST_MODE_WB_ADDR                (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_TPM_TEST_MODE_WB_POS                 (27)     /*< bit[27]     */
#define ANA_ANCTL_DIS_TPM_TEST_MODE_WB_MASK                (0x1ul << ANA_ANCTL_DIS_TPM_TEST_MODE_WB_POS)
#define ANA_ANCTL_DIS_RC_CAL_EN_WB_ADDR                    (uint32_t)&(ANA->ANCTL_WB_REG0)
#define ANA_ANCTL_DIS_RC_CAL_EN_WB_POS                     (28)     /*< bit[28]     */
#define ANA_ANCTL_DIS_RC_CAL_EN_WB_MASK                    (0x1ul << ANA_ANCTL_DIS_RC_CAL_EN_WB_POS)

#define ANA_ANCTL_OSC16K_REG0_ADDR                         (uint32_t)&(ANA->ANCTL_OSC16K_REG0)
#define ANA_ANCTL_OSC16K_SCLKSETTLE_ADDR                   (uint32_t)&(ANA->ANCTL_OSC16K_REG0)
#define ANA_ANCTL_OSC16K_SCLKSETTLE_POS                    (0)      /*< bit[2:0]    */
#define ANA_ANCTL_OSC16K_SCLKSETTLE_MASK                   (0x7ul << ANA_ANCTL_OSC16K_SCLKSETTLE_POS)
#define ANA_ANCTL_OSC16K_TARCOUNT_ADDR                     (uint32_t)&(ANA->ANCTL_OSC16K_REG0)
#define ANA_ANCTL_OSC16K_TARCOUNT_POS                      (3)      /*< bit[20:3]   */
#define ANA_ANCTL_OSC16K_TARCOUNT_MASK                     (0x3FFFFul << ANA_ANCTL_OSC16K_TARCOUNT_POS)
#define ANA_ANCTL_OSC16K_STEPCOARSE_ADDR                   (uint32_t)&(ANA->ANCTL_OSC16K_REG0)
#define ANA_ANCTL_OSC16K_STEPCOARSE_POS                    (21)     /*< bit[26:21]  */
#define ANA_ANCTL_OSC16K_STEPCOARSE_MASK                   (0x3Ful << ANA_ANCTL_OSC16K_STEPCOARSE_POS)
#define ANA_ANCTL_OSC16K_STEPFINE_ADDR                     (uint32_t)&(ANA->ANCTL_OSC16K_REG0)
#define ANA_ANCTL_OSC16K_STEPFINE_POS                      (28)     /*< bit[29:28]  */
#define ANA_ANCTL_OSC16K_STEPFINE_MASK                     (0x3ul << ANA_ANCTL_OSC16K_STEPFINE_POS)
#define ANA_ANCTL_OSC16K_NOSTOP_ADDR                       (uint32_t)&(ANA->ANCTL_OSC16K_REG0)
#define ANA_ANCTL_OSC16K_NOSTOP_POS                        (31)     /*< bit[31]     */
#define ANA_ANCTL_OSC16K_NOSTOP_MASK                       (0x1ul << ANA_ANCTL_OSC16K_NOSTOP_POS)

#define ANA_ANCTL_OSC16K_REG1_ADDR                         (uint32_t)&(ANA->ANCTL_OSC16K_REG1)
#define ANA_ANCTL_OSC16K_NUMSLOWCLKCYCLE_ADDR              (uint32_t)&(ANA->ANCTL_OSC16K_REG1)
#define ANA_ANCTL_OSC16K_NUMSLOWCLKCYCLE_POS               (0)      /*< bit[5:0]    */
#define ANA_ANCTL_OSC16K_NUMSLOWCLKCYCLE_MASK              (0x3Ful << ANA_ANCTL_OSC16K_NUMSLOWCLKCYCLE_POS)
#define ANA_ANCTL_OSC16K_COARSEDIFF_ADDR                   (uint32_t)&(ANA->ANCTL_OSC16K_REG1)
#define ANA_ANCTL_OSC16K_COARSEDIFF_POS                    (16)     /*< bit[24:16]  */
#define ANA_ANCTL_OSC16K_COARSEDIFF_MASK                   (0x1FFul << ANA_ANCTL_OSC16K_COARSEDIFF_POS)

#define ANA_ANCTL_OSC16K_REG2_ADDR                         (uint32_t)&(ANA->ANCTL_OSC16K_REG2)
#define ANA_ANCTL_OSC16K_INITCTRLINDEX_ADDR                (uint32_t)&(ANA->ANCTL_OSC16K_REG2)
#define ANA_ANCTL_OSC16K_INITCTRLINDEX_POS                 (0)      /*< bit[11:0]   */
#define ANA_ANCTL_OSC16K_INITCTRLINDEX_MASK                (0xFFFul << ANA_ANCTL_OSC16K_INITCTRLINDEX_POS)
#define ANA_ANCTL_OSC16K_CALCNT_ADDR                       (uint32_t)&(ANA->ANCTL_OSC16K_REG2)
#define ANA_ANCTL_OSC16K_CALCNT_POS                        (16)     /*< bit[24:16]  */
#define ANA_ANCTL_OSC16K_CALCNT_MASK                       (0x1FFul << ANA_ANCTL_OSC16K_CALCNT_POS)

#define ANA_ANCTL_OSC16K_REG3_ADDR                         (uint32_t)&(ANA->ANCTL_OSC16K_REG3)
#define ANA_ANCTL_OSC16K_COUNTERVALUE_ADDR                 (uint32_t)&(ANA->ANCTL_OSC16K_REG3)
#define ANA_ANCTL_OSC16K_COUNTERVALUE_POS                  (0)      /*< bit[17:0]   */
#define ANA_ANCTL_OSC16K_COUNTERVALUE_MASK                 (0x3FFFFul << ANA_ANCTL_OSC16K_COUNTERVALUE_POS)

#define ANA_ANCTL_TEST_REG_ADDR                            (uint32_t)&(ANA->ANCTL_TEST_REG)
#define ANA_ANCTL_BBP_IQCAL_HANG_ADDR                      (uint32_t)&(ANA->ANCTL_TEST_REG)
#define ANA_ANCTL_BBP_IQCAL_HANG_POS                       (0)      /*< bit[0]      */
#define ANA_ANCTL_BBP_IQCAL_HANG_MASK                      (0x1ul << ANA_ANCTL_BBP_IQCAL_HANG_POS)

#define ANA_ANCTL_ST_REG_ADDR                              (uint32_t)&(ANA->ANCTL_ST_REG)
#define ANA_ANCTL_CUR_ST_ADDR                              (uint32_t)&(ANA->ANCTL_ST_REG)
#define ANA_ANCTL_CUR_ST_POS                               (0)      /*< bit[15:0]   */
#define ANA_ANCTL_CUR_ST_MASK                              (0xFFFFul << ANA_ANCTL_CUR_ST_POS)
#define ANA_ANCTL_CUR_DCDC_TAR_ADDR                        (uint32_t)&(ANA->ANCTL_ST_REG)
#define ANA_ANCTL_CUR_DCDC_TAR_POS                         (16)     /*< bit[18:16]  */
#define ANA_ANCTL_CUR_DCDC_TAR_MASK                        (0x7ul << ANA_ANCTL_CUR_DCDC_TAR_POS)

#define ANA_ANCTL_CONFIG_40_ADDR                           (uint32_t)&(ANA->ANCTL_CONFIG_40)
#define ANA_ANCTL_PWR01_ADDR                               (uint32_t)&(ANA->ANCTL_CONFIG_40)
#define ANA_ANCTL_PWR01_POS                                (0)      /*< bit[6:0]    */
#define ANA_ANCTL_PWR01_MASK                               (0x7Ful << ANA_ANCTL_PWR01_POS)
#define ANA_ANCTL_PWR02_ADDR                               (uint32_t)&(ANA->ANCTL_CONFIG_40)
#define ANA_ANCTL_PWR02_POS                                (8)      /*< bit[14:8]   */
#define ANA_ANCTL_PWR02_MASK                               (0x7Ful << ANA_ANCTL_PWR02_POS)
#define ANA_ANCTL_PWR03_ADDR                               (uint32_t)&(ANA->ANCTL_CONFIG_40)
#define ANA_ANCTL_PWR03_POS                                (16)     /*< bit[22:16]  */
#define ANA_ANCTL_PWR03_MASK                               (0x7Ful << ANA_ANCTL_PWR03_POS)
#define ANA_ANCTL_PWR04_ADDR                               (uint32_t)&(ANA->ANCTL_CONFIG_40)
#define ANA_ANCTL_PWR04_POS                                (24)     /*< bit[30:24]  */
#define ANA_ANCTL_PWR04_MASK                               (0x7Ful << ANA_ANCTL_PWR04_POS)

#define ANA_BASE                                          0x40011000UL
#define ANA                                               ((ANA_T               *) ANA_BASE)

#endif/* __ANA_REG_ER8130_H__*/

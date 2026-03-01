/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __MAC_REG_ER8130_H__
#define __MAC_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"

/* ================================================================================ */
/* ================                      MAC_T                     ================ */
/* ================================================================================ */

/**
  * @brief MAC (MAC_T)
  */

 typedef struct {                                          /*!< MAC_T Structure                                                            */
  __IO uint32_t RX_TOP;                                    /*!< RX_TOP Register (0x000)                                                    */
  __IO uint32_t RX_STS;                                    /*!< RX_STS Register (0x004)                                                    */
  __IO uint32_t RX_802;                                    /*!< RX_802 Register (0x008)                                                    */
  __IO uint32_t RX_802FILT;                                /*!< RX_802FILT Register (0x00C)                                                */
  __IO uint32_t RX_DATCOL;                                 /*!< RX_DATCOL Register (0x010)                                                 */
  __IO uint32_t RX_FEA;                                    /*!< RX_FEA Register (0x014)                                                    */
  __I  uint32_t RF_RX_STS;                                 /*!< RF_RX_STS Register (0x018)                                                 */
  __I  uint32_t RESERVED0;
  __IO uint32_t TX_TOP;                                    /*!< TX_TOP Register (0x020)                                                    */
  __IO uint32_t TX_STS;                                    /*!< TX_STS Register (0x024)                                                    */
  __IO uint32_t TX_CFG;                                    /*!< TX_CFG Register (0x028)                                                    */
  __IO uint32_t TX_CFG2;                                   /*!< TX_CFG2 Register (0x02C)                                                   */
  __IO uint32_t TX_DATDLV;                                 /*!< TX_DATDLV Register (0x030)                                                 */
  __IO uint32_t TX_CFG3;                                   /*!< TX_CFG3 Register (0x034)                                                   */
  __IO uint32_t TX_CFG4;                                   /*!< TX_CFG4 Register (0x038)                                                   */
  __IO uint32_t TX_CFG5;                                   /*!< TX_CFG5 Register (0x03C)                                                   */
  __IO uint32_t CCA1;                                      /*!< CCA1 Register (0x040)                                                      */
  __I  uint32_t CCA2;                                      /*!< CCA2 Register (0x044)                                                      */
  __IO uint32_t RF_TX_STS;                                 /*!< RF_TX_STS Register (0x048)                                                 */
  __IO uint32_t IO_REG;                                    /*!< IO_REG Register (0x04C)                                                    */
  __IO uint32_t CSMA;                                      /*!< CSMA Register (0x050)                                                      */
  __I  uint32_t RESERVED1[3];
  __IO uint32_t INT_ST;                                    /*!< INT_ST Register (0x060)                                                    */
  __IO uint32_t INT_EN;                                    /*!< INT_EN Register (0x064)                                                    */
  __I  uint32_t INT;                                       /*!< INT Register (0x068)                                                       */
  __I  uint32_t RESERVED2;
  __IO uint32_t INFO0;                                     /*!< INFO0 Register (0x070)                                                     */
  __IO uint32_t INFO1;                                     /*!< INFO1 Register (0x074)                                                     */
  __IO uint32_t INFO2;                                     /*!< INFO2 Register (0x078)                                                     */
  __IO uint32_t INFO3;                                     /*!< INFO3 Register (0x07C)                                                     */
  __IO uint32_t TMR_CTL;                                   /*!< TMR_CTL Register (0x080)                                                   */
  __IO uint32_t TMR_STS;                                   /*!< TMR_STS Register (0x084)                                                   */
  __I  uint32_t RESERVED3[4];
  __IO uint32_t TMR_EVTL;                                  /*!< TMR_EVTL Register (0x098)                                                  */
  __I  uint32_t RESERVED4[4];
  __IO uint32_t TX_CRC;                                    /*!< TX_CRC Register (0x0AC)                                                    */
  __IO uint32_t MISC_REG;                                  /*!< MISC_REG Register (0x0B0)                                                  */
  __IO uint32_t CRC_REG;                                   /*!< CRC_REG Register (0x0B4)                                                   */
  __IO uint32_t TXPHR_REG;                                 /*!< TXPHR_REG Register (0x0B8)                                                 */
  __IO uint32_t RXPHR_REG;                                 /*!< RXPHR_REG Register (0x0BC)                                                 */
  __IO uint32_t CRC_REG_EXT0;                              /*!< CRC_REG_EXT0 Register (0x0C0)                                              */
  __IO uint32_t CRC_REG_EXT1;                              /*!< CRC_REG_EXT1 Register (0x0C4)                                              */
  __IO uint32_t CRC_REG_EXT2;                              /*!< CRC_REG_EXT2 Register (0x0C8)                                              */
  __IO uint32_t PRG_DELAY_REG0;                            /*!< PRG_DELAY_REG0 Register (0x0CC)                                            */
  __IO uint32_t PRG_DELAY_REG1;                            /*!< PRG_DELAY_REG1 Register (0x0D0)                                            */
  __IO uint32_t PRG_DELAY_REG2;                            /*!< PRG_DELAY_REG2 Register (0x0D4)                                            */
  __IO uint32_t TX_CRC_STANDALONE;                         /*!< TX_CRC_STANDALONE Register (0x0D8)                                         */
  __IO uint32_t TX_CFG6;                                   /*!< TX_CFG6 Register (0x0DC)                                                   */
} MAC_T;

#define MAC_RX_TOP_ADDR                                    (uint32_t)&(MAC->RX_TOP)
#define MAC_RXEN_ADDR                                      (uint32_t)&(MAC->RX_TOP)
#define MAC_RXEN_POS                                       (0)      /*< bit[0]      */
#define MAC_RXEN_MASK                                      (0x1ul << MAC_RXEN_POS)
#define MAC_RX_FSM_BSY_ADDR                                (uint32_t)&(MAC->RX_TOP)
#define MAC_RX_FSM_BSY_POS                                 (1)      /*< bit[1]      */
#define MAC_RX_FSM_BSY_MASK                                (0x1ul << MAC_RX_FSM_BSY_POS)
#define MAC_RXEN_802_ADDR                                  (uint32_t)&(MAC->RX_TOP)
#define MAC_RXEN_802_POS                                   (4)      /*< bit[4]      */
#define MAC_RXEN_802_MASK                                  (0x1ul << MAC_RXEN_802_POS)
#define MAC_RXEN_TRN_ADDR                                  (uint32_t)&(MAC->RX_TOP)
#define MAC_RXEN_TRN_POS                                   (5)      /*< bit[5]      */
#define MAC_RXEN_TRN_MASK                                  (0x1ul << MAC_RXEN_TRN_POS)
#define MAC_RXEN_TRN_BLE_ADDR                              (uint32_t)&(MAC->RX_TOP)
#define MAC_RXEN_TRN_BLE_POS                               (6)      /*< bit[6]      */
#define MAC_RXEN_TRN_BLE_MASK                              (0x1ul << MAC_RXEN_TRN_BLE_POS)
#define MAC_RXEN_TRN_BLR_ADDR                              (uint32_t)&(MAC->RX_TOP)
#define MAC_RXEN_TRN_BLR_POS                               (7)      /*< bit[7]      */
#define MAC_RXEN_TRN_BLR_MASK                              (0x1ul << MAC_RXEN_TRN_BLR_POS)
#define MAC_RXEN_RESTR_ADDR                                (uint32_t)&(MAC->RX_TOP)
#define MAC_RXEN_RESTR_POS                                 (8)      /*< bit[8]      */
#define MAC_RXEN_RESTR_MASK                                (0x1ul << MAC_RXEN_RESTR_POS)
#define MAC_RX_CCA_ADDR                                    (uint32_t)&(MAC->RX_TOP)
#define MAC_RX_CCA_POS                                     (9)      /*< bit[9]      */
#define MAC_RX_CCA_MASK                                    (0x1ul << MAC_RX_CCA_POS)
#define MAC_MAXRXLEN_ADDR                                  (uint32_t)&(MAC->RX_TOP)
#define MAC_MAXRXLEN_POS                                   (17)     /*< bit[27:17]  */
#define MAC_MAXRXLEN_MASK                                  (0x7FFul << MAC_MAXRXLEN_POS)

#define MAC_RX_STS_ADDR                                    (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKT802OK_ADDR                               (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKT802OK_POS                                (0)      /*< bit[0]      */
#define MAC_RX_PKT802OK_MASK                               (0x1ul << MAC_RX_PKT802OK_POS)
#define MAC_RX_PKT802NGCRC_ADDR                            (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKT802NGCRC_POS                             (1)      /*< bit[1]      */
#define MAC_RX_PKT802NGCRC_MASK                            (0x1ul << MAC_RX_PKT802NGCRC_POS)
#define MAC_RX_PKT802NGFMT_ADDR                            (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKT802NGFMT_POS                             (2)      /*< bit[2]      */
#define MAC_RX_PKT802NGFMT_MASK                            (0x1ul << MAC_RX_PKT802NGFMT_POS)
#define MAC_RX_PKT802NGREJ_ADDR                            (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKT802NGREJ_POS                             (3)      /*< bit[3]      */
#define MAC_RX_PKT802NGREJ_MASK                            (0x1ul << MAC_RX_PKT802NGREJ_POS)
#define MAC_RX_PKTTRN_ADDR                                 (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKTTRN_POS                                  (8)      /*< bit[8]      */
#define MAC_RX_PKTTRN_MASK                                 (0x1ul << MAC_RX_PKTTRN_POS)
#define MAC_RX_PKTTRNNG_ADDR                               (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKTTRNNG_POS                                (9)      /*< bit[9]      */
#define MAC_RX_PKTTRNNG_MASK                               (0x1ul << MAC_RX_PKTTRNNG_POS)
#define MAC_RX_PKTBLE_ADDR                                 (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKTBLE_POS                                  (10)     /*< bit[10]     */
#define MAC_RX_PKTBLE_MASK                                 (0x1ul << MAC_RX_PKTBLE_POS)
#define MAC_RX_PKTBLENG_ADDR                               (uint32_t)&(MAC->RX_STS)
#define MAC_RX_PKTBLENG_POS                                (11)     /*< bit[11]     */
#define MAC_RX_PKTBLENG_MASK                               (0x1ul << MAC_RX_PKTBLENG_POS)
#define MAC_RXLEN_ADDR                                     (uint32_t)&(MAC->RX_STS)
#define MAC_RXLEN_POS                                      (16)     /*< bit[26:16]  */
#define MAC_RXLEN_MASK                                     (0x7FFul << MAC_RXLEN_POS)

#define MAC_RX_802_ADDR                                    (uint32_t)&(MAC->RX_802)
#define MAC_RX_DECPRE_ADDR                                 (uint32_t)&(MAC->RX_802)
#define MAC_RX_DECPRE_POS                                  (0)      /*< bit[0]      */
#define MAC_RX_DECPRE_MASK                                 (0x1ul << MAC_RX_DECPRE_POS)
#define MAC_RX_DECSFD_ADDR                                 (uint32_t)&(MAC->RX_802)
#define MAC_RX_DECSFD_POS                                  (1)      /*< bit[1]      */
#define MAC_RX_DECSFD_MASK                                 (0x1ul << MAC_RX_DECSFD_POS)
#define MAC_RX_KEEPPRE_ADDR                                (uint32_t)&(MAC->RX_802)
#define MAC_RX_KEEPPRE_POS                                 (4)      /*< bit[4]      */
#define MAC_RX_KEEPPRE_MASK                                (0x1ul << MAC_RX_KEEPPRE_POS)
#define MAC_RX_KEEPSFD_ADDR                                (uint32_t)&(MAC->RX_802)
#define MAC_RX_KEEPSFD_POS                                 (5)      /*< bit[5]      */
#define MAC_RX_KEEPSFD_MASK                                (0x1ul << MAC_RX_KEEPSFD_POS)
#define MAC_RX_KEEPPHR_ADDR                                (uint32_t)&(MAC->RX_802)
#define MAC_RX_KEEPPHR_POS                                 (6)      /*< bit[6]      */
#define MAC_RX_KEEPPHR_MASK                                (0x1ul << MAC_RX_KEEPPHR_POS)
#define MAC_RX_KEEPFCS_ADDR                                (uint32_t)&(MAC->RX_802)
#define MAC_RX_KEEPFCS_POS                                 (7)      /*< bit[7]      */
#define MAC_RX_KEEPFCS_MASK                                (0x1ul << MAC_RX_KEEPFCS_POS)
#define MAC_RX_CRCCHKEN_ADDR                               (uint32_t)&(MAC->RX_802)
#define MAC_RX_CRCCHKEN_POS                                (8)      /*< bit[8]      */
#define MAC_RX_CRCCHKEN_MASK                               (0x1ul << MAC_RX_CRCCHKEN_POS)
#define MAC_RX_CRCTYP_ADDR                                 (uint32_t)&(MAC->RX_802)
#define MAC_RX_CRCTYP_POS                                  (9)      /*< bit[9]      */
#define MAC_RX_CRCTYP_MASK                                 (0x1ul << MAC_RX_CRCTYP_POS)
#define MAC_RX_CRCPAD_ADDR                                 (uint32_t)&(MAC->RX_802)
#define MAC_RX_CRCPAD_POS                                  (10)     /*< bit[10]     */
#define MAC_RX_CRCPAD_MASK                                 (0x1ul << MAC_RX_CRCPAD_POS)
#define MAC_RX_APPENDLEN_ADDR                              (uint32_t)&(MAC->RX_802)
#define MAC_RX_APPENDLEN_POS                               (11)     /*< bit[11]     */
#define MAC_RX_APPENDLEN_MASK                              (0x1ul << MAC_RX_APPENDLEN_POS)
#define MAC_RX_APPENDRSSI_ADDR                             (uint32_t)&(MAC->RX_802)
#define MAC_RX_APPENDRSSI_POS                              (12)     /*< bit[12]     */
#define MAC_RX_APPENDRSSI_MASK                             (0x1ul << MAC_RX_APPENDRSSI_POS)
#define MAC_RX_TRN_CRCCHKEN_ADDR                           (uint32_t)&(MAC->RX_802)
#define MAC_RX_TRN_CRCCHKEN_POS                            (13)     /*< bit[13]     */
#define MAC_RX_TRN_CRCCHKEN_MASK                           (0x1ul << MAC_RX_TRN_CRCCHKEN_POS)
#define MAC_RX_TRN_PHRCHKEN_ADDR                           (uint32_t)&(MAC->RX_802)
#define MAC_RX_TRN_PHRCHKEN_POS                            (14)     /*< bit[14]     */
#define MAC_RX_TRN_PHRCHKEN_MASK                           (0x1ul << MAC_RX_TRN_PHRCHKEN_POS)
#define MAC_RX_PHR2B_ADDR                                  (uint32_t)&(MAC->RX_802)
#define MAC_RX_PHR2B_POS                                   (15)     /*< bit[15]     */
#define MAC_RX_PHR2B_MASK                                  (0x1ul << MAC_RX_PHR2B_POS)

#define MAC_RX_802FILT_ADDR                                (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_BEACON_ADDR                             (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_BEACON_POS                              (0)      /*< bit[0]      */
#define MAC_RX_REJ_BEACON_MASK                             (0x1ul << MAC_RX_REJ_BEACON_POS)
#define MAC_RX_REJ_DATA_ADDR                               (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_DATA_POS                                (1)      /*< bit[1]      */
#define MAC_RX_REJ_DATA_MASK                               (0x1ul << MAC_RX_REJ_DATA_POS)
#define MAC_RX_REJ_ACK_ADDR                                (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_ACK_POS                                 (2)      /*< bit[2]      */
#define MAC_RX_REJ_ACK_MASK                                (0x1ul << MAC_RX_REJ_ACK_POS)
#define MAC_RX_REJ_MACCMD_ADDR                             (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_MACCMD_POS                              (3)      /*< bit[3]      */
#define MAC_RX_REJ_MACCMD_MASK                             (0x1ul << MAC_RX_REJ_MACCMD_POS)
#define MAC_RX_REJ_OTHERS_ADDR                             (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_OTHERS_POS                              (4)      /*< bit[4]      */
#define MAC_RX_REJ_OTHERS_MASK                             (0x1ul << MAC_RX_REJ_OTHERS_POS)
#define MAC_RX_REJ_FRMVER_ADDR                             (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_FRMVER_POS                              (5)      /*< bit[5]      */
#define MAC_RX_REJ_FRMVER_MASK                             (0x1ul << MAC_RX_REJ_FRMVER_POS)
#define MAC_RX_FRMVER_THD_ADDR                             (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_FRMVER_THD_POS                              (6)      /*< bit[7:6]    */
#define MAC_RX_FRMVER_THD_MASK                             (0x3ul << MAC_RX_FRMVER_THD_POS)
#define MAC_RX_REJ_DUADR_ADDR                              (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_DUADR_POS                               (8)      /*< bit[8]      */
#define MAC_RX_REJ_DUADR_MASK                              (0x1ul << MAC_RX_REJ_DUADR_POS)
#define MAC_RX_REJ_DBADR_ADDR                              (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_DBADR_POS                               (9)      /*< bit[9]      */
#define MAC_RX_REJ_DBADR_MASK                              (0x1ul << MAC_RX_REJ_DBADR_POS)
#define MAC_RX_REJ_DUPID_ADDR                              (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_DUPID_POS                               (10)     /*< bit[10]     */
#define MAC_RX_REJ_DUPID_MASK                              (0x1ul << MAC_RX_REJ_DUPID_POS)
#define MAC_RX_REJ_DBPID_ADDR                              (uint32_t)&(MAC->RX_802FILT)
#define MAC_RX_REJ_DBPID_POS                               (11)     /*< bit[11]     */
#define MAC_RX_REJ_DBPID_MASK                              (0x1ul << MAC_RX_REJ_DBPID_POS)

#define MAC_RX_DATCOL_ADDR                                 (uint32_t)&(MAC->RX_DATCOL)
#define MAC_DATCOL_DUAL_EN_ADDR                            (uint32_t)&(MAC->RX_DATCOL)
#define MAC_DATCOL_DUAL_EN_POS                             (0)      /*< bit[0]      */
#define MAC_DATCOL_DUAL_EN_MASK                            (0x1ul << MAC_DATCOL_DUAL_EN_POS)
#define MAC_DATCOL_AGC_INI_DIS_ADDR                        (uint32_t)&(MAC->RX_DATCOL)
#define MAC_DATCOL_AGC_INI_DIS_POS                         (1)      /*< bit[1]      */
#define MAC_DATCOL_AGC_INI_DIS_MASK                        (0x1ul << MAC_DATCOL_AGC_INI_DIS_POS)
#define MAC_DATCOL_AGC_RESTR_DIS_ADDR                      (uint32_t)&(MAC->RX_DATCOL)
#define MAC_DATCOL_AGC_RESTR_DIS_POS                       (2)      /*< bit[2]      */
#define MAC_DATCOL_AGC_RESTR_DIS_MASK                      (0x1ul << MAC_DATCOL_AGC_RESTR_DIS_POS)
#define MAC_DATCOL_BBP_RST_DIS_ADDR                        (uint32_t)&(MAC->RX_DATCOL)
#define MAC_DATCOL_BBP_RST_DIS_POS                         (3)      /*< bit[3]      */
#define MAC_DATCOL_BBP_RST_DIS_MASK                        (0x1ul << MAC_DATCOL_BBP_RST_DIS_POS)
#define MAC_DATCOL_BBP_MOD_ADDR                            (uint32_t)&(MAC->RX_DATCOL)
#define MAC_DATCOL_BBP_MOD_POS                             (4)      /*< bit[4]      */
#define MAC_DATCOL_BBP_MOD_MASK                            (0x1ul << MAC_DATCOL_BBP_MOD_POS)

#define MAC_RX_FEA_ADDR                                    (uint32_t)&(MAC->RX_FEA)
#define MAC_WU_RX_EN_ADDR                                  (uint32_t)&(MAC->RX_FEA)
#define MAC_WU_RX_EN_POS                                   (0)      /*< bit[0]      */
#define MAC_WU_RX_EN_MASK                                  (0x1ul << MAC_WU_RX_EN_POS)
#define MAC_RX_TIMEOUT_EN_ADDR                             (uint32_t)&(MAC->RX_FEA)
#define MAC_RX_TIMEOUT_EN_POS                              (4)      /*< bit[4]      */
#define MAC_RX_TIMEOUT_EN_MASK                             (0x1ul << MAC_RX_TIMEOUT_EN_POS)
#define MAC_RX_TIMEOUT_THD_ADDR                            (uint32_t)&(MAC->RX_FEA)
#define MAC_RX_TIMEOUT_THD_POS                             (5)      /*< bit[6:5]    */
#define MAC_RX_TIMEOUT_THD_MASK                            (0x3ul << MAC_RX_TIMEOUT_THD_POS)
#define MAC_RX_TIMEOUT_DSLEEP_ADDR                         (uint32_t)&(MAC->RX_FEA)
#define MAC_RX_TIMEOUT_DSLEEP_POS                          (10)     /*< bit[10]     */
#define MAC_RX_TIMEOUT_DSLEEP_MASK                         (0x1ul << MAC_RX_TIMEOUT_DSLEEP_POS)
#define MAC_RX_TIMEOUT_NUM_ADDR                            (uint32_t)&(MAC->RX_FEA)
#define MAC_RX_TIMEOUT_NUM_POS                             (16)     /*< bit[21:16]  */
#define MAC_RX_TIMEOUT_NUM_MASK                            (0x3Ful << MAC_RX_TIMEOUT_NUM_POS)
#define MAC_RX_MANCH_EN_ADDR                               (uint32_t)&(MAC->RX_FEA)
#define MAC_RX_MANCH_EN_POS                                (30)     /*< bit[30]     */
#define MAC_RX_MANCH_EN_MASK                               (0x1ul << MAC_RX_MANCH_EN_POS)

#define MAC_RF_RX_STS_ADDR                                 (uint32_t)&(MAC->RF_RX_STS)
#define MAC_RF_RX_BSY_ADDR                                 (uint32_t)&(MAC->RF_RX_STS)
#define MAC_RF_RX_BSY_POS                                  (0)      /*< bit[0]      */
#define MAC_RF_RX_BSY_MASK                                 (0x1ul << MAC_RF_RX_BSY_POS)

#define MAC_TX_TOP_ADDR                                    (uint32_t)&(MAC->TX_TOP)
#define MAC_TXEN_ADDR                                      (uint32_t)&(MAC->TX_TOP)
#define MAC_TXEN_POS                                       (0)      /*< bit[0]      */
#define MAC_TXEN_MASK                                      (0x1ul << MAC_TXEN_POS)
#define MAC_TXENCCA_ADDR                                   (uint32_t)&(MAC->TX_TOP)
#define MAC_TXENCCA_POS                                    (1)      /*< bit[1]      */
#define MAC_TXENCCA_MASK                                   (0x1ul << MAC_TXENCCA_POS)
#define MAC_TXSTOP_ADDR                                    (uint32_t)&(MAC->TX_TOP)
#define MAC_TXSTOP_POS                                     (7)      /*< bit[7]      */
#define MAC_TXSTOP_MASK                                    (0x1ul << MAC_TXSTOP_POS)
#define MAC_TXDROPCNT_ADDR                                 (uint32_t)&(MAC->TX_TOP)
#define MAC_TXDROPCNT_POS                                  (8)      /*< bit[10:8]   */
#define MAC_TXDROPCNT_MASK                                 (0x7ul << MAC_TXDROPCNT_POS)
#define MAC_TXW4ACK_ADDR                                   (uint32_t)&(MAC->TX_TOP)
#define MAC_TXW4ACK_POS                                    (12)     /*< bit[12]     */
#define MAC_TXW4ACK_MASK                                   (0x1ul << MAC_TXW4ACK_POS)
#define MAC_TXLEN_ADDR                                     (uint32_t)&(MAC->TX_TOP)
#define MAC_TXLEN_POS                                      (16)     /*< bit[27:16]  */
#define MAC_TXLEN_MASK                                     (0xFFFul << MAC_TXLEN_POS)

#define MAC_TX_STS_ADDR                                    (uint32_t)&(MAC->TX_STS)
#define MAC_TXOK_ADDR                                      (uint32_t)&(MAC->TX_STS)
#define MAC_TXOK_POS                                       (0)      /*< bit[0]      */
#define MAC_TXOK_MASK                                      (0x1ul << MAC_TXOK_POS)
#define MAC_TXERR_ADDR                                     (uint32_t)&(MAC->TX_STS)
#define MAC_TXERR_POS                                      (1)      /*< bit[1]      */
#define MAC_TXERR_MASK                                     (0x1ul << MAC_TXERR_POS)
#define MAC_TXCHBSY_ADDR                                   (uint32_t)&(MAC->TX_STS)
#define MAC_TXCHBSY_POS                                    (2)      /*< bit[2]      */
#define MAC_TXCHBSY_MASK                                   (0x1ul << MAC_TXCHBSY_POS)
#define MAC_TXNOACK_ADDR                                   (uint32_t)&(MAC->TX_STS)
#define MAC_TXNOACK_POS                                    (3)      /*< bit[3]      */
#define MAC_TXNOACK_MASK                                   (0x1ul << MAC_TXNOACK_POS)

#define MAC_TX_CFG_ADDR                                    (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_APPENDPRE_ADDR                              (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_APPENDPRE_POS                               (0)      /*< bit[0]      */
#define MAC_TX_APPENDPRE_MASK                              (0x1ul << MAC_TX_APPENDPRE_POS)
#define MAC_TX_APPENDSFD_ADDR                              (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_APPENDSFD_POS                               (1)      /*< bit[1]      */
#define MAC_TX_APPENDSFD_MASK                              (0x1ul << MAC_TX_APPENDSFD_POS)
#define MAC_TX_APPENDPHR_ADDR                              (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_APPENDPHR_POS                               (2)      /*< bit[2]      */
#define MAC_TX_APPENDPHR_MASK                              (0x1ul << MAC_TX_APPENDPHR_POS)
#define MAC_TX_APPENDFCS_ADDR                              (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_APPENDFCS_POS                               (3)      /*< bit[3]      */
#define MAC_TX_APPENDFCS_MASK                              (0x1ul << MAC_TX_APPENDFCS_POS)
#define MAC_TX_APPENDBLECRC_ADDR                           (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_APPENDBLECRC_POS                            (4)      /*< bit[4]      */
#define MAC_TX_APPENDBLECRC_MASK                           (0x1ul << MAC_TX_APPENDBLECRC_POS)
#define MAC_TX_APPENDPOST_ADDR                             (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_APPENDPOST_POS                              (5)      /*< bit[5]      */
#define MAC_TX_APPENDPOST_MASK                             (0x1ul << MAC_TX_APPENDPOST_POS)
#define MAC_TX_CRCTYP_ADDR                                 (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_CRCTYP_POS                                  (8)      /*< bit[8]      */
#define MAC_TX_CRCTYP_MASK                                 (0x1ul << MAC_TX_CRCTYP_POS)
#define MAC_TX_CRCPAD_ADDR                                 (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_CRCPAD_POS                                  (9)      /*< bit[9]      */
#define MAC_TX_CRCPAD_MASK                                 (0x1ul << MAC_TX_CRCPAD_POS)
#define MAC_TX_PHR2B_ADDR                                  (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_PHR2B_POS                                   (16)     /*< bit[16]     */
#define MAC_TX_PHR2B_MASK                                  (0x1ul << MAC_TX_PHR2B_POS)
#define MAC_TX_PHRDAT_ADDR                                 (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_PHRDAT_POS                                  (17)     /*< bit[17]     */
#define MAC_TX_PHRDAT_MASK                                 (0x1ul << MAC_TX_PHRDAT_POS)
#define MAC_TX_CON_ADDR                                    (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_CON_POS                                     (18)     /*< bit[18]     */
#define MAC_TX_CON_MASK                                    (0x1ul << MAC_TX_CON_POS)
#define MAC_TX_BYPASS_RDY_ADDR                             (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_BYPASS_RDY_POS                              (20)     /*< bit[20]     */
#define MAC_TX_BYPASS_RDY_MASK                             (0x1ul << MAC_TX_BYPASS_RDY_POS)
#define MAC_TX_DIS_EXT_RATE_ADDR                           (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_DIS_EXT_RATE_POS                            (21)     /*< bit[21]     */
#define MAC_TX_DIS_EXT_RATE_MASK                           (0x1ul << MAC_TX_DIS_EXT_RATE_POS)
#define MAC_TX_BYPASS_DATRDY_ADDR                          (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_BYPASS_DATRDY_POS                           (22)     /*< bit[22]     */
#define MAC_TX_BYPASS_DATRDY_MASK                          (0x1ul << MAC_TX_BYPASS_DATRDY_POS)
#define MAC_TX_FEC_EN_ADDR                                 (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_FEC_EN_POS                                  (23)     /*< bit[23]     */
#define MAC_TX_FEC_EN_MASK                                 (0x1ul << MAC_TX_FEC_EN_POS)
#define MAC_TX_PN9PHR_ADDR                                 (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_PN9PHR_POS                                  (24)     /*< bit[24]     */
#define MAC_TX_PN9PHR_MASK                                 (0x1ul << MAC_TX_PN9PHR_POS)
#define MAC_TX_PN9DAT_ADDR                                 (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_PN9DAT_POS                                  (25)     /*< bit[25]     */
#define MAC_TX_PN9DAT_MASK                                 (0x1ul << MAC_TX_PN9DAT_POS)
#define MAC_TX_PN9FCS_ADDR                                 (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_PN9FCS_POS                                  (26)     /*< bit[26]     */
#define MAC_TX_PN9FCS_MASK                                 (0x1ul << MAC_TX_PN9FCS_POS)
#define MAC_TX_PN9INV_ADDR                                 (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_PN9INV_POS                                  (28)     /*< bit[28]     */
#define MAC_TX_PN9INV_MASK                                 (0x1ul << MAC_TX_PN9INV_POS)
#define MAC_TX_MANCH_EN_ADDR                               (uint32_t)&(MAC->TX_CFG)
#define MAC_TX_MANCH_EN_POS                                (30)     /*< bit[30]     */
#define MAC_TX_MANCH_EN_MASK                               (0x1ul << MAC_TX_MANCH_EN_POS)

#define MAC_TX_CFG2_ADDR                                   (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_PRE_NUM_ADDR                                (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_PRE_NUM_POS                                 (0)      /*< bit[3:0]    */
#define MAC_TX_PRE_NUM_MASK                                (0xFul << MAC_TX_PRE_NUM_POS)
#define MAC_TXW4ACK_NUM_ADDR                               (uint32_t)&(MAC->TX_CFG2)
#define MAC_TXW4ACK_NUM_POS                                (4)      /*< bit[5:4]    */
#define MAC_TXW4ACK_NUM_MASK                               (0x3ul << MAC_TXW4ACK_NUM_POS)
#define MAC_TX_PRE_PAT_ADDR                                (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_PRE_PAT_POS                                 (8)      /*< bit[15:8]   */
#define MAC_TX_PRE_PAT_MASK                                (0xFFul << MAC_TX_PRE_PAT_POS)
#define MAC_TXW4ACK_GAP_ADDR                               (uint32_t)&(MAC->TX_CFG2)
#define MAC_TXW4ACK_GAP_POS                                (16)     /*< bit[19:16]  */
#define MAC_TXW4ACK_GAP_MASK                               (0xFul << MAC_TXW4ACK_GAP_POS)
#define MAC_TX_PN7PHR_ADDR                                 (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_PN7PHR_POS                                  (20)     /*< bit[20]     */
#define MAC_TX_PN7PHR_MASK                                 (0x1ul << MAC_TX_PN7PHR_POS)
#define MAC_TX_PN7DAT_ADDR                                 (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_PN7DAT_POS                                  (21)     /*< bit[21]     */
#define MAC_TX_PN7DAT_MASK                                 (0x1ul << MAC_TX_PN7DAT_POS)
#define MAC_TX_PN7FCS_ADDR                                 (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_PN7FCS_POS                                  (22)     /*< bit[22]     */
#define MAC_TX_PN7FCS_MASK                                 (0x1ul << MAC_TX_PN7FCS_POS)
#define MAC_TX_PN7INV_ADDR                                 (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_PN7INV_POS                                  (23)     /*< bit[23]     */
#define MAC_TX_PN7INV_MASK                                 (0x1ul << MAC_TX_PN7INV_POS)
#define MAC_TX_CODED_PHY_EN_ADDR                           (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_CODED_PHY_EN_POS                            (24)     /*< bit[24]     */
#define MAC_TX_CODED_PHY_EN_MASK                           (0x1ul << MAC_TX_CODED_PHY_EN_POS)
#define MAC_TX_CODED_PHY_SCHEME_ADDR                       (uint32_t)&(MAC->TX_CFG2)
#define MAC_TX_CODED_PHY_SCHEME_POS                        (25)     /*< bit[25]     */
#define MAC_TX_CODED_PHY_SCHEME_MASK                       (0x1ul << MAC_TX_CODED_PHY_SCHEME_POS)

#define MAC_TX_DATDLV_ADDR                                 (uint32_t)&(MAC->TX_DATDLV)
#define MAC_DATDLV_DUAL_EN_ADDR                            (uint32_t)&(MAC->TX_DATDLV)
#define MAC_DATDLV_DUAL_EN_POS                             (0)      /*< bit[0]      */
#define MAC_DATDLV_DUAL_EN_MASK                            (0x1ul << MAC_DATDLV_DUAL_EN_POS)
#define MAC_DATDLV_DUAL_PRD_S_ADDR                         (uint32_t)&(MAC->TX_DATDLV)
#define MAC_DATDLV_DUAL_PRD_S_POS                          (15)     /*< bit[15]     */
#define MAC_DATDLV_DUAL_PRD_S_MASK                         (0x1ul << MAC_DATDLV_DUAL_PRD_S_POS)
#define MAC_DATDLV_DUAL_PRD_ADDR                           (uint32_t)&(MAC->TX_DATDLV)
#define MAC_DATDLV_DUAL_PRD_POS                            (16)     /*< bit[31:16]  */
#define MAC_DATDLV_DUAL_PRD_MASK                           (0xFFFFul << MAC_DATDLV_DUAL_PRD_POS)

#define MAC_TX_CFG3_ADDR                                   (uint32_t)&(MAC->TX_CFG3)
#define MAC_TX_SFD_NUM_ADDR                                (uint32_t)&(MAC->TX_CFG3)
#define MAC_TX_SFD_NUM_POS                                 (0)      /*< bit[2:0]    */
#define MAC_TX_SFD_NUM_MASK                                (0x7ul << MAC_TX_SFD_NUM_POS)
#define MAC_TX_PRE1_EN_ADDR                                (uint32_t)&(MAC->TX_CFG3)
#define MAC_TX_PRE1_EN_POS                                 (4)      /*< bit[4]      */
#define MAC_TX_PRE1_EN_MASK                                (0x1ul << MAC_TX_PRE1_EN_POS)
#define MAC_TX_PRE1_NUM_ADDR                               (uint32_t)&(MAC->TX_CFG3)
#define MAC_TX_PRE1_NUM_POS                                (8)      /*< bit[12:8]   */
#define MAC_TX_PRE1_NUM_MASK                               (0x1Ful << MAC_TX_PRE1_NUM_POS)
#define MAC_TX_PRE1_PAT_ADDR                               (uint32_t)&(MAC->TX_CFG3)
#define MAC_TX_PRE1_PAT_POS                                (16)     /*< bit[23:16]  */
#define MAC_TX_PRE1_PAT_MASK                               (0xFFul << MAC_TX_PRE1_PAT_POS)
#define MAC_TX_BPSK_PKT_FORMAT_EN_ADDR                     (uint32_t)&(MAC->TX_CFG3)
#define MAC_TX_BPSK_PKT_FORMAT_EN_POS                      (28)     /*< bit[28]     */
#define MAC_TX_BPSK_PKT_FORMAT_EN_MASK                     (0x1ul << MAC_TX_BPSK_PKT_FORMAT_EN_POS)

#define MAC_TX_CFG4_ADDR                                   (uint32_t)&(MAC->TX_CFG4)
#define MAC_TX_CFG5_ADDR                                   (uint32_t)&(MAC->TX_CFG5)
#define MAC_CCA1_ADDR                                      (uint32_t)&(MAC->CCA1)
#define MAC_CCA_MOD_ADDR                                   (uint32_t)&(MAC->CCA1)
#define MAC_CCA_MOD_POS                                    (8)      /*< bit[9:8]    */
#define MAC_CCA_MOD_MASK                                   (0x3ul << MAC_CCA_MOD_POS)
#define MAC_CCA_DETCYP_ADDR                                (uint32_t)&(MAC->CCA1)
#define MAC_CCA_DETCYP_POS                                 (10)     /*< bit[10]     */
#define MAC_CCA_DETCYP_MASK                                (0x1ul << MAC_CCA_DETCYP_POS)
#define MAC_RSSI_THD_ADDR                                  (uint32_t)&(MAC->CCA1)
#define MAC_RSSI_THD_POS                                   (16)     /*< bit[27:16]  */
#define MAC_RSSI_THD_MASK                                  (0xFFFul << MAC_RSSI_THD_POS)

#define MAC_CCA2_ADDR                                      (uint32_t)&(MAC->CCA2)
#define MAC_CCA_STS_ADDR                                   (uint32_t)&(MAC->CCA2)
#define MAC_CCA_STS_POS                                    (0)      /*< bit[0]      */
#define MAC_CCA_STS_MASK                                   (0x1ul << MAC_CCA_STS_POS)
#define MAC_RSSI_CUR_ADDR                                  (uint32_t)&(MAC->CCA2)
#define MAC_RSSI_CUR_POS                                   (16)     /*< bit[27:16]  */
#define MAC_RSSI_CUR_MASK                                  (0xFFFul << MAC_RSSI_CUR_POS)

#define MAC_RF_TX_STS_ADDR                                 (uint32_t)&(MAC->RF_TX_STS)
#define MAC_RF_TX_RDY_ADDR                                 (uint32_t)&(MAC->RF_TX_STS)
#define MAC_RF_TX_RDY_POS                                  (1)      /*< bit[1]      */
#define MAC_RF_TX_RDY_MASK                                 (0x1ul << MAC_RF_TX_RDY_POS)

#define MAC_IO_REG_ADDR                                    (uint32_t)&(MAC->IO_REG)
#define MAC_RX_PATHMOD_ADDR                                (uint32_t)&(MAC->IO_REG)
#define MAC_RX_PATHMOD_POS                                 (0)      /*< bit[0]      */
#define MAC_RX_PATHMOD_MASK                                (0x1ul << MAC_RX_PATHMOD_POS)
#define MAC_TX_EXT_DAT_ADDR                                (uint32_t)&(MAC->IO_REG)
#define MAC_TX_EXT_DAT_POS                                 (1)      /*< bit[1]      */
#define MAC_TX_EXT_DAT_MASK                                (0x1ul << MAC_TX_EXT_DAT_POS)

#define MAC_CSMA_ADDR                                      (uint32_t)&(MAC->CSMA)
#define MAC_CSMA_MOD_ADDR                                  (uint32_t)&(MAC->CSMA)
#define MAC_CSMA_MOD_POS                                   (0)      /*< bit[0]      */
#define MAC_CSMA_MOD_MASK                                  (0x1ul << MAC_CSMA_MOD_POS)
#define MAC_CSMA_MINBE_ADDR                                (uint32_t)&(MAC->CSMA)
#define MAC_CSMA_MINBE_POS                                 (8)      /*< bit[11:8]   */
#define MAC_CSMA_MINBE_MASK                                (0xFul << MAC_CSMA_MINBE_POS)
#define MAC_CSMA_MAXBE_ADDR                                (uint32_t)&(MAC->CSMA)
#define MAC_CSMA_MAXBE_POS                                 (12)     /*< bit[15:12]  */
#define MAC_CSMA_MAXBE_MASK                                (0xFul << MAC_CSMA_MAXBE_POS)
#define MAC_CSMA_SEED_ADDR                                 (uint32_t)&(MAC->CSMA)
#define MAC_CSMA_SEED_POS                                  (16)     /*< bit[19:16]  */
#define MAC_CSMA_SEED_MASK                                 (0xFul << MAC_CSMA_SEED_POS)
#define MAC_CSMA_CCANUM_ADDR                               (uint32_t)&(MAC->CSMA)
#define MAC_CSMA_CCANUM_POS                                (20)     /*< bit[21:20]  */
#define MAC_CSMA_CCANUM_MASK                               (0x3ul << MAC_CSMA_CCANUM_POS)
#define MAC_CSMA_RETRYNUM_ADDR                             (uint32_t)&(MAC->CSMA)
#define MAC_CSMA_RETRYNUM_POS                              (24)     /*< bit[26:24]  */
#define MAC_CSMA_RETRYNUM_MASK                             (0x7ul << MAC_CSMA_RETRYNUM_POS)

#define MAC_INT_ST_ADDR                                    (uint32_t)&(MAC->INT_ST)
#define MAC_INT_ST_RX_ADDR                                 (uint32_t)&(MAC->INT_ST)
#define MAC_INT_ST_RX_POS                                  (0)      /*< bit[0]      */
#define MAC_INT_ST_RX_MASK                                 (0x1ul << MAC_INT_ST_RX_POS)
#define MAC_INT_ST_RXERR_ADDR                              (uint32_t)&(MAC->INT_ST)
#define MAC_INT_ST_RXERR_POS                               (1)      /*< bit[1]      */
#define MAC_INT_ST_RXERR_MASK                              (0x1ul << MAC_INT_ST_RXERR_POS)
#define MAC_INT_ST_TX_ADDR                                 (uint32_t)&(MAC->INT_ST)
#define MAC_INT_ST_TX_POS                                  (2)      /*< bit[2]      */
#define MAC_INT_ST_TX_MASK                                 (0x1ul << MAC_INT_ST_TX_POS)
#define MAC_INT_ST_TXERR_ADDR                              (uint32_t)&(MAC->INT_ST)
#define MAC_INT_ST_TXERR_POS                               (3)      /*< bit[3]      */
#define MAC_INT_ST_TXERR_MASK                              (0x1ul << MAC_INT_ST_TXERR_POS)
#define MAC_INT_ST_RXTOUT_ADDR                             (uint32_t)&(MAC->INT_ST)
#define MAC_INT_ST_RXTOUT_POS                              (4)      /*< bit[4]      */
#define MAC_INT_ST_RXTOUT_MASK                             (0x1ul << MAC_INT_ST_RXTOUT_POS)
#define MAC_INT_ST_TMREVT_ADDR                             (uint32_t)&(MAC->INT_ST)
#define MAC_INT_ST_TMREVT_POS                              (5)      /*< bit[5]      */
#define MAC_INT_ST_TMREVT_MASK                             (0x1ul << MAC_INT_ST_TMREVT_POS)
#define MAC_INT_ST_SYNCW_ADDR                              (uint32_t)&(MAC->INT_ST)
#define MAC_INT_ST_SYNCW_POS                               (6)      /*< bit[6]      */
#define MAC_INT_ST_SYNCW_MASK                              (0x1ul << MAC_INT_ST_SYNCW_POS)

#define MAC_INT_EN_ADDR                                    (uint32_t)&(MAC->INT_EN)
#define MAC_INT_EN_RX_ADDR                                 (uint32_t)&(MAC->INT_EN)
#define MAC_INT_EN_RX_POS                                  (0)      /*< bit[0]      */
#define MAC_INT_EN_RX_MASK                                 (0x1ul << MAC_INT_EN_RX_POS)
#define MAC_INT_EN_RXERR_ADDR                              (uint32_t)&(MAC->INT_EN)
#define MAC_INT_EN_RXERR_POS                               (1)      /*< bit[1]      */
#define MAC_INT_EN_RXERR_MASK                              (0x1ul << MAC_INT_EN_RXERR_POS)
#define MAC_INT_EN_TX_ADDR                                 (uint32_t)&(MAC->INT_EN)
#define MAC_INT_EN_TX_POS                                  (2)      /*< bit[2]      */
#define MAC_INT_EN_TX_MASK                                 (0x1ul << MAC_INT_EN_TX_POS)
#define MAC_INT_EN_TXERR_ADDR                              (uint32_t)&(MAC->INT_EN)
#define MAC_INT_EN_TXERR_POS                               (3)      /*< bit[3]      */
#define MAC_INT_EN_TXERR_MASK                              (0x1ul << MAC_INT_EN_TXERR_POS)
#define MAC_INT_EN_RXTOUT_ADDR                             (uint32_t)&(MAC->INT_EN)
#define MAC_INT_EN_RXTOUT_POS                              (4)      /*< bit[4]      */
#define MAC_INT_EN_RXTOUT_MASK                             (0x1ul << MAC_INT_EN_RXTOUT_POS)
#define MAC_INT_EN_TMREVT_ADDR                             (uint32_t)&(MAC->INT_EN)
#define MAC_INT_EN_TMREVT_POS                              (5)      /*< bit[5]      */
#define MAC_INT_EN_TMREVT_MASK                             (0x1ul << MAC_INT_EN_TMREVT_POS)
#define MAC_INT_EN_SYNCW_ADDR                              (uint32_t)&(MAC->INT_EN)
#define MAC_INT_EN_SYNCW_POS                               (6)      /*< bit[6]      */
#define MAC_INT_EN_SYNCW_MASK                              (0x1ul << MAC_INT_EN_SYNCW_POS)

#define MAC_INT_ADDR                                       (uint32_t)&(MAC->INT)
#define MAC_INT_RX_ADDR                                    (uint32_t)&(MAC->INT)
#define MAC_INT_RX_POS                                     (0)      /*< bit[0]      */
#define MAC_INT_RX_MASK                                    (0x1ul << MAC_INT_RX_POS)
#define MAC_INT_RXERR_ADDR                                 (uint32_t)&(MAC->INT)
#define MAC_INT_RXERR_POS                                  (1)      /*< bit[1]      */
#define MAC_INT_RXERR_MASK                                 (0x1ul << MAC_INT_RXERR_POS)
#define MAC_INT_TX_ADDR                                    (uint32_t)&(MAC->INT)
#define MAC_INT_TX_POS                                     (2)      /*< bit[2]      */
#define MAC_INT_TX_MASK                                    (0x1ul << MAC_INT_TX_POS)
#define MAC_INT_TXERR_ADDR                                 (uint32_t)&(MAC->INT)
#define MAC_INT_TXERR_POS                                  (3)      /*< bit[3]      */
#define MAC_INT_TXERR_MASK                                 (0x1ul << MAC_INT_TXERR_POS)
#define MAC_INT_RXTOUT_ADDR                                (uint32_t)&(MAC->INT)
#define MAC_INT_RXTOUT_POS                                 (4)      /*< bit[4]      */
#define MAC_INT_RXTOUT_MASK                                (0x1ul << MAC_INT_RXTOUT_POS)
#define MAC_INT_TMREVT_ADDR                                (uint32_t)&(MAC->INT)
#define MAC_INT_TMREVT_POS                                 (5)      /*< bit[5]      */
#define MAC_INT_TMREVT_MASK                                (0x1ul << MAC_INT_TMREVT_POS)
#define MAC_INT_SYNCW_ADDR                                 (uint32_t)&(MAC->INT)
#define MAC_INT_SYNCW_POS                                  (6)      /*< bit[6]      */
#define MAC_INT_SYNCW_MASK                                 (0x1ul << MAC_INT_SYNCW_POS)

#define MAC_INFO0_ADDR                                     (uint32_t)&(MAC->INFO0)
#define MAC_PAN_ID_ADDR                                    (uint32_t)&(MAC->INFO0)
#define MAC_PAN_ID_POS                                     (0)      /*< bit[15:0]   */
#define MAC_PAN_ID_MASK                                    (0xFFFFul << MAC_PAN_ID_POS)

#define MAC_INFO1_ADDR                                     (uint32_t)&(MAC->INFO1)
#define MAC_SADDR_ADDR                                     (uint32_t)&(MAC->INFO1)
#define MAC_SADDR_POS                                      (0)      /*< bit[15:0]   */
#define MAC_SADDR_MASK                                     (0xFFFFul << MAC_SADDR_POS)

#define MAC_INFO2_ADDR                                     (uint32_t)&(MAC->INFO2)
#define MAC_INFO3_ADDR                                     (uint32_t)&(MAC->INFO3)
#define MAC_TMR_CTL_ADDR                                   (uint32_t)&(MAC->TMR_CTL)
#define MAC_TMREVT_EN_ADDR                                 (uint32_t)&(MAC->TMR_CTL)
#define MAC_TMREVT_EN_POS                                  (2)      /*< bit[2]      */
#define MAC_TMREVT_EN_MASK                                 (0x1ul << MAC_TMREVT_EN_POS)
#define MAC_TMREVT_TYP_RXSOF_ADDR                          (uint32_t)&(MAC->TMR_CTL)
#define MAC_TMREVT_TYP_RXSOF_POS                           (8)      /*< bit[8]      */
#define MAC_TMREVT_TYP_RXSOF_MASK                          (0x1ul << MAC_TMREVT_TYP_RXSOF_POS)
#define MAC_TMREVT_TYP_RXEOF_ADDR                          (uint32_t)&(MAC->TMR_CTL)
#define MAC_TMREVT_TYP_RXEOF_POS                           (9)      /*< bit[9]      */
#define MAC_TMREVT_TYP_RXEOF_MASK                          (0x1ul << MAC_TMREVT_TYP_RXEOF_POS)
#define MAC_TMREVT_TYP_TXSOF_ADDR                          (uint32_t)&(MAC->TMR_CTL)
#define MAC_TMREVT_TYP_TXSOF_POS                           (10)     /*< bit[10]     */
#define MAC_TMREVT_TYP_TXSOF_MASK                          (0x1ul << MAC_TMREVT_TYP_TXSOF_POS)
#define MAC_TMREVT_TYP_TXEOF_ADDR                          (uint32_t)&(MAC->TMR_CTL)
#define MAC_TMREVT_TYP_TXEOF_POS                           (11)     /*< bit[11]     */
#define MAC_TMREVT_TYP_TXEOF_MASK                          (0x1ul << MAC_TMREVT_TYP_TXEOF_POS)

#define MAC_TMR_STS_ADDR                                   (uint32_t)&(MAC->TMR_STS)
#define MAC_RXSOF_ADDR                                     (uint32_t)&(MAC->TMR_STS)
#define MAC_RXSOF_POS                                      (8)      /*< bit[8]      */
#define MAC_RXSOF_MASK                                     (0x1ul << MAC_RXSOF_POS)
#define MAC_RXEOF_ADDR                                     (uint32_t)&(MAC->TMR_STS)
#define MAC_RXEOF_POS                                      (9)      /*< bit[9]      */
#define MAC_RXEOF_MASK                                     (0x1ul << MAC_RXEOF_POS)
#define MAC_TXSOF_ADDR                                     (uint32_t)&(MAC->TMR_STS)
#define MAC_TXSOF_POS                                      (10)     /*< bit[10]     */
#define MAC_TXSOF_MASK                                     (0x1ul << MAC_TXSOF_POS)
#define MAC_TXEOF_ADDR                                     (uint32_t)&(MAC->TMR_STS)
#define MAC_TXEOF_POS                                      (11)     /*< bit[11]     */
#define MAC_TXEOF_MASK                                     (0x1ul << MAC_TXEOF_POS)

#define MAC_TMR_EVTL_ADDR                                  (uint32_t)&(MAC->TMR_EVTL)
#define MAC_TX_CRC_ADDR                                    (uint32_t)&(MAC->TX_CRC)
#define MAC_MISC_REG_ADDR                                  (uint32_t)&(MAC->MISC_REG)
#define MAC_MANCH_TYP_ADDR                                 (uint32_t)&(MAC->MISC_REG)
#define MAC_MANCH_TYP_POS                                  (0)      /*< bit[0]      */
#define MAC_MANCH_TYP_MASK                                 (0x1ul << MAC_MANCH_TYP_POS)
#define MAC_DMA_TX_BYTE_REVERSE_ADDR                       (uint32_t)&(MAC->MISC_REG)
#define MAC_DMA_TX_BYTE_REVERSE_POS                        (1)      /*< bit[1]      */
#define MAC_DMA_TX_BYTE_REVERSE_MASK                       (0x1ul << MAC_DMA_TX_BYTE_REVERSE_POS)
#define MAC_DMA_TX_BIT_REVERSE_ADDR                        (uint32_t)&(MAC->MISC_REG)
#define MAC_DMA_TX_BIT_REVERSE_POS                         (2)      /*< bit[2]      */
#define MAC_DMA_TX_BIT_REVERSE_MASK                        (0x1ul << MAC_DMA_TX_BIT_REVERSE_POS)
#define MAC_DMA_RX_BYTE_REVERSE_ADDR                       (uint32_t)&(MAC->MISC_REG)
#define MAC_DMA_RX_BYTE_REVERSE_POS                        (3)      /*< bit[3]      */
#define MAC_DMA_RX_BYTE_REVERSE_MASK                       (0x1ul << MAC_DMA_RX_BYTE_REVERSE_POS)
#define MAC_DMA_RX_BIT_REVERSE_ADDR                        (uint32_t)&(MAC->MISC_REG)
#define MAC_DMA_RX_BIT_REVERSE_POS                         (4)      /*< bit[4]      */
#define MAC_DMA_RX_BIT_REVERSE_MASK                        (0x1ul << MAC_DMA_RX_BIT_REVERSE_POS)
#define MAC_TX_INFI_ADDR                                   (uint32_t)&(MAC->MISC_REG)
#define MAC_TX_INFI_POS                                    (5)      /*< bit[5]      */
#define MAC_TX_INFI_MASK                                   (0x1ul << MAC_TX_INFI_POS)
#define MAC_PN9_INIT_ADDR                                  (uint32_t)&(MAC->MISC_REG)
#define MAC_PN9_INIT_POS                                   (8)      /*< bit[16:8]   */
#define MAC_PN9_INIT_MASK                                  (0x1FFul << MAC_PN9_INIT_POS)
#define MAC_PN9_INIROD_ADDR                                (uint32_t)&(MAC->MISC_REG)
#define MAC_PN9_INIROD_POS                                 (17)     /*< bit[17]     */
#define MAC_PN9_INIROD_MASK                                (0x1ul << MAC_PN9_INIROD_POS)
#define MAC_PN9_TYP_ADDR                                   (uint32_t)&(MAC->MISC_REG)
#define MAC_PN9_TYP_POS                                    (18)     /*< bit[18]     */
#define MAC_PN9_TYP_MASK                                   (0x1ul << MAC_PN9_TYP_POS)
#define MAC_PN7_INIT_ADDR                                  (uint32_t)&(MAC->MISC_REG)
#define MAC_PN7_INIT_POS                                   (19)     /*< bit[25:19]  */
#define MAC_PN7_INIT_MASK                                  (0x7Ful << MAC_PN7_INIT_POS)
#define MAC_PN7_INIROD_ADDR                                (uint32_t)&(MAC->MISC_REG)
#define MAC_PN7_INIROD_POS                                 (26)     /*< bit[26]     */
#define MAC_PN7_INIROD_MASK                                (0x1ul << MAC_PN7_INIROD_POS)
#define MAC_TX_ADDR_INSERT_ADDR                            (uint32_t)&(MAC->MISC_REG)
#define MAC_TX_ADDR_INSERT_POS                             (27)     /*< bit[28:27]  */
#define MAC_TX_ADDR_INSERT_MASK                            (0x3ul << MAC_TX_ADDR_INSERT_POS)
#define MAC_RX_ADDR_CHECK_ADDR                             (uint32_t)&(MAC->MISC_REG)
#define MAC_RX_ADDR_CHECK_POS                              (29)     /*< bit[29]     */
#define MAC_RX_ADDR_CHECK_MASK                             (0x1ul << MAC_RX_ADDR_CHECK_POS)
#define MAC_ADDR_2B_ADDR                                   (uint32_t)&(MAC->MISC_REG)
#define MAC_ADDR_2B_POS                                    (30)     /*< bit[30]     */
#define MAC_ADDR_2B_MASK                                   (0x1ul << MAC_ADDR_2B_POS)

#define MAC_CRC_REG_ADDR                                   (uint32_t)&(MAC->CRC_REG)
#define MAC_CRC_POLY_ADDR                                  (uint32_t)&(MAC->CRC_REG)
#define MAC_CRC_POLY_POS                                   (0)      /*< bit[15:0]   */
#define MAC_CRC_POLY_MASK                                  (0xFFFFul << MAC_CRC_POLY_POS)
#define MAC_CRC_INV_ADDR                                   (uint32_t)&(MAC->CRC_REG)
#define MAC_CRC_INV_POS                                    (17)     /*< bit[17]     */
#define MAC_CRC_INV_MASK                                   (0x1ul << MAC_CRC_INV_POS)
#define MAC_CRC_ENDIAN_IN_ADDR                             (uint32_t)&(MAC->CRC_REG)
#define MAC_CRC_ENDIAN_IN_POS                              (18)     /*< bit[18]     */
#define MAC_CRC_ENDIAN_IN_MASK                             (0x1ul << MAC_CRC_ENDIAN_IN_POS)
#define MAC_CRC_ENDIAN_OUT_ADDR                            (uint32_t)&(MAC->CRC_REG)
#define MAC_CRC_ENDIAN_OUT_POS                             (19)     /*< bit[19]     */
#define MAC_CRC_ENDIAN_OUT_MASK                            (0x1ul << MAC_CRC_ENDIAN_OUT_POS)
#define MAC_CRC_MSB_ADDR                                   (uint32_t)&(MAC->CRC_REG)
#define MAC_CRC_MSB_POS                                    (20)     /*< bit[20]     */
#define MAC_CRC_MSB_MASK                                   (0x1ul << MAC_CRC_MSB_POS)
#define MAC_CRC_BIT_ADDR                                   (uint32_t)&(MAC->CRC_REG)
#define MAC_CRC_BIT_POS                                    (21)     /*< bit[26:21]  */
#define MAC_CRC_BIT_MASK                                   (0x3Ful << MAC_CRC_BIT_POS)
#define MAC_CRCFLEX_ADDR                                   (uint32_t)&(MAC->CRC_REG)
#define MAC_CRCFLEX_POS                                    (27)     /*< bit[27]     */
#define MAC_CRCFLEX_MASK                                   (0x1ul << MAC_CRCFLEX_POS)

#define MAC_TXPHR_REG_ADDR                                 (uint32_t)&(MAC->TXPHR_REG)
#define MAC_TXLEN_SFT_ADDR                                 (uint32_t)&(MAC->TXPHR_REG)
#define MAC_TXLEN_SFT_POS                                  (0)      /*< bit[3:0]    */
#define MAC_TXLEN_SFT_MASK                                 (0xFul << MAC_TXLEN_SFT_POS)
#define MAC_TX_PHRINV_ADDR                                 (uint32_t)&(MAC->TXPHR_REG)
#define MAC_TX_PHRINV_POS                                  (4)      /*< bit[4]      */
#define MAC_TX_PHRINV_MASK                                 (0x1ul << MAC_TX_PHRINV_POS)
#define MAC_TXPHR_PAT_ADDR                                 (uint32_t)&(MAC->TXPHR_REG)
#define MAC_TXPHR_PAT_POS                                  (16)     /*< bit[31:16]  */
#define MAC_TXPHR_PAT_MASK                                 (0xFFFFul << MAC_TXPHR_PAT_POS)

#define MAC_RXPHR_REG_ADDR                                 (uint32_t)&(MAC->RXPHR_REG)
#define MAC_RXLEN_SFT_ADDR                                 (uint32_t)&(MAC->RXPHR_REG)
#define MAC_RXLEN_SFT_POS                                  (0)      /*< bit[3:0]    */
#define MAC_RXLEN_SFT_MASK                                 (0xFul << MAC_RXLEN_SFT_POS)
#define MAC_RX_PHRINV_ADDR                                 (uint32_t)&(MAC->RXPHR_REG)
#define MAC_RX_PHRINV_POS                                  (4)      /*< bit[4]      */
#define MAC_RX_PHRINV_MASK                                 (0x1ul << MAC_RX_PHRINV_POS)
#define MAC_RXPHR_MSK_ADDR                                 (uint32_t)&(MAC->RXPHR_REG)
#define MAC_RXPHR_MSK_POS                                  (16)     /*< bit[31:16]  */
#define MAC_RXPHR_MSK_MASK                                 (0xFFFFul << MAC_RXPHR_MSK_POS)

#define MAC_CRC_REG_EXT0_ADDR                              (uint32_t)&(MAC->CRC_REG_EXT0)
#define MAC_CRC_REG_EXT1_ADDR                              (uint32_t)&(MAC->CRC_REG_EXT1)
#define MAC_CRC_REG_EXT2_ADDR                              (uint32_t)&(MAC->CRC_REG_EXT2)
#define MAC_PRG_DELAY_REG0_ADDR                            (uint32_t)&(MAC->PRG_DELAY_REG0)
#define MAC_TX_PRG_DELAY_SET_ADDR                          (uint32_t)&(MAC->PRG_DELAY_REG0)
#define MAC_TX_PRG_DELAY_SET_POS                           (0)      /*< bit[0]      */
#define MAC_TX_PRG_DELAY_SET_MASK                          (0x1ul << MAC_TX_PRG_DELAY_SET_POS)
#define MAC_RX_PRG_DELAY_SET_ADDR                          (uint32_t)&(MAC->PRG_DELAY_REG0)
#define MAC_RX_PRG_DELAY_SET_POS                           (1)      /*< bit[1]      */
#define MAC_RX_PRG_DELAY_SET_MASK                          (0x1ul << MAC_RX_PRG_DELAY_SET_POS)
#define MAC_TX_PRG_DELAY_EN_ADDR                           (uint32_t)&(MAC->PRG_DELAY_REG0)
#define MAC_TX_PRG_DELAY_EN_POS                            (2)      /*< bit[2]      */
#define MAC_TX_PRG_DELAY_EN_MASK                           (0x1ul << MAC_TX_PRG_DELAY_EN_POS)
#define MAC_RX_PRG_DELAY_EN_ADDR                           (uint32_t)&(MAC->PRG_DELAY_REG0)
#define MAC_RX_PRG_DELAY_EN_POS                            (3)      /*< bit[3]      */
#define MAC_RX_PRG_DELAY_EN_MASK                           (0x1ul << MAC_RX_PRG_DELAY_EN_POS)

#define MAC_PRG_DELAY_REG1_ADDR                            (uint32_t)&(MAC->PRG_DELAY_REG1)
#define MAC_PRG_DELAY_REG2_ADDR                            (uint32_t)&(MAC->PRG_DELAY_REG2)
#define MAC_TX_CRC_STANDALONE_ADDR                         (uint32_t)&(MAC->TX_CRC_STANDALONE)
#define MAC_TX_CRC_CAL_ONLY_EN_ADDR                        (uint32_t)&(MAC->TX_CRC_STANDALONE)
#define MAC_TX_CRC_CAL_ONLY_EN_POS                         (0)      /*< bit[0]      */
#define MAC_TX_CRC_CAL_ONLY_EN_MASK                        (0x1ul << MAC_TX_CRC_CAL_ONLY_EN_POS)
#define MAC_TXLEN_CRC_ADDR                                 (uint32_t)&(MAC->TX_CRC_STANDALONE)
#define MAC_TXLEN_CRC_POS                                  (1)      /*< bit[22:1]   */
#define MAC_TXLEN_CRC_MASK                                 (0x3FFFFFul << MAC_TXLEN_CRC_POS)

#define MAC_TX_CFG6_ADDR                                   (uint32_t)&(MAC->TX_CFG6)
#define MAC_TX_POST_NUM_ADDR                               (uint32_t)&(MAC->TX_CFG6)
#define MAC_TX_POST_NUM_POS                                (0)      /*< bit[3:0]    */
#define MAC_TX_POST_NUM_MASK                               (0xFul << MAC_TX_POST_NUM_POS)
#define MAC_TX_POST_PAT_ADDR                               (uint32_t)&(MAC->TX_CFG6)
#define MAC_TX_POST_PAT_POS                                (4)      /*< bit[11:4]   */
#define MAC_TX_POST_PAT_MASK                               (0xFFul << MAC_TX_POST_PAT_POS)
#define MAC_TX_POST1_EN_ADDR                               (uint32_t)&(MAC->TX_CFG6)
#define MAC_TX_POST1_EN_POS                                (12)     /*< bit[12]     */
#define MAC_TX_POST1_EN_MASK                               (0x1ul << MAC_TX_POST1_EN_POS)
#define MAC_TX_POST1_NUM_ADDR                              (uint32_t)&(MAC->TX_CFG6)
#define MAC_TX_POST1_NUM_POS                               (13)     /*< bit[17:13]  */
#define MAC_TX_POST1_NUM_MASK                              (0x1Ful << MAC_TX_POST1_NUM_POS)
#define MAC_TX_POST1_PAT_ADDR                              (uint32_t)&(MAC->TX_CFG6)
#define MAC_TX_POST1_PAT_POS                               (18)     /*< bit[25:18]  */
#define MAC_TX_POST1_PAT_MASK                              (0xFFul << MAC_TX_POST1_PAT_POS)

#define MAC_BASE                                          0x40020000UL
#define MAC                                               ((MAC_T               *) MAC_BASE)


#endif

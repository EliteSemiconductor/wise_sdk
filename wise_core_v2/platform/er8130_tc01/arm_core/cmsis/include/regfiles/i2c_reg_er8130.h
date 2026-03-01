/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_i2c_soc_b1_20240819.xlsx */
/* ================================================================================
 */
/* ================                      I2C_T ================ */
/* ================================================================================
 */
#ifndef __I2C_REG_ER8130_H__
#define __I2C_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief I2C (I2C_T)
 */

typedef struct {
    /* 0x000 ID */
    union {
        __I uint32_t reg;
        struct {
            __I uint32_t REV_MINOR : 4;  /* [3:0]   */
            __I uint32_t REV_MAJOR : 8;  /* [11:4]  */
            __I uint32_t ID_NUM    : 20; /* [31:12] */
        } bitfield;
    } ID; /* offset 0x000 */

    uint32_t RESERVED0[3];

    /* 0x010 CFG */
    union {
        __I uint32_t reg;
        struct {
            __I uint32_t FIFOSize : 2;  /* [1:0] */
            uint32_t RESERVED     : 30; /* [31:2] */
        } bitfield;
    } CFG; /* offset 0x010 */

    /* 0x014 INT_EN */
    union {
        __IO uint32_t reg;
        struct {
            __IO uint32_t FIFOEmpty : 1;  /* bit[0]  */
            __IO uint32_t FIFOFull  : 1;  /* bit[1]  */
            __IO uint32_t FIFOHalf  : 1;  /* bit[2]  */
            __IO uint32_t AddrHit   : 1;  /* bit[3]  */
            __IO uint32_t ArbLose   : 1;  /* bit[4]  */
            __IO uint32_t Stop      : 1;  /* bit[5]  */
            __IO uint32_t Start     : 1;  /* bit[6]  */
            __IO uint32_t ByteTrans : 1;  /* bit[7]  */
            __IO uint32_t ByteRecv  : 1;  /* bit[8]  */
            __IO uint32_t Cmpl      : 1;  /* bit[9]  */
            uint32_t RESERVED       : 22; /* bit[31:10] */
        } bitfield;
    } INT_EN; /* offset 0x014 */

    /* 0x018 STS */
    union {
        __IO uint32_t reg;
        struct {
            __I uint32_t FIFOEmpty  : 1;  /* bit[0]  */
            __I uint32_t FIFOFull   : 1;  /* bit[1]  */
            __I uint32_t FIFOHalf   : 1;  /* bit[2]  */
            __IO uint32_t AddrHit   : 1;  /* bit[3]  */
            __IO uint32_t ArbLose   : 1;  /* bit[4]  */
            __IO uint32_t Stop      : 1;  /* bit[5]  */
            __IO uint32_t Start     : 1;  /* bit[6]  */
            __IO uint32_t ByteTrans : 1;  /* bit[7]  */
            __IO uint32_t ByteRecv  : 1;  /* bit[8]  */
            __IO uint32_t Cmpl      : 1;  /* bit[9]  */
            __I uint32_t ACK        : 1;  /* bit[10] */
            __I uint32_t BusBusy    : 1;  /* bit[11] */
            __I uint32_t GenCall    : 1;  /* bit[12] */
            __I uint32_t LineSCL    : 1;  /* bit[13] */
            __I uint32_t LineSDA    : 1;  /* bit[14] */
            uint32_t RESERVED       : 17; /* bit[31:15] */
        } bitfield;
    } STS; /* offset 0x018 */

    /* 0x01C ADDR */
    union {
        __IO uint32_t reg;
        struct {
            __IO uint32_t ADDRESS : 10; /* [9:0]   */
            uint32_t RESERVED     : 22; /* [31:10] */
        } bitfield;
    } ADDR; /* offset 0x01C */

    /* 0x020 DATA */
    union {
        __IO uint32_t reg;
        struct {
            __IO uint32_t DATAFIELD : 8;  /* [7:0]   */
            uint32_t RESERVED       : 24; /* [31:8]  */
        } bitfield;
    } DATA; /* offset 0x020 */

    /* 0x024 CTRL */
    union {
        __IO uint32_t reg;
        struct {
            __IO uint32_t DataCnt     : 8;  /* [7:0]   */
            __IO uint32_t Dir         : 1;  /* bit[8]  */
            __IO uint32_t Phase_stop  : 1;  /* bit[9]  */
            __IO uint32_t Phase_data  : 1;  /* bit[10] */
            __IO uint32_t Phase_addr  : 1;  /* bit[11] */
            __IO uint32_t Phase_start : 1;  /* bit[12] */
            uint32_t RESERVED         : 19; /* [31:13] */
        } bitfield;
    } CTRL; /* offset 0x024 */

    /* 0x028 CMD */
    union {
        __IO uint32_t reg;
        struct {
            __IO uint32_t CMD : 3;  /* [2:0]   */
            uint32_t RESERVED : 29; /* [31:3]  */
        } bitfield;
    } CMD; /* offset 0x028 */

    /* 0x02C SETUP */
    union {
        __IO uint32_t reg;
        struct {
            __IO uint32_t IICEn      : 1; /* bit[0]  */
            __IO uint32_t Addressing : 1; /* bit[1]  */
            __IO uint32_t Master     : 1; /* bit[2]  */
            __IO uint32_t DMAEn      : 1; /* bit[3]  */
            __IO uint32_t SCLHi      : 9; /* [12:4]  */
            __IO uint32_t SCLRatio   : 1; /* bit[13] */
            uint32_t RESERVED1       : 2; /* [15:14] */
            __IO uint32_t HDDAT      : 5; /* [20:16] */
            __IO uint32_t SP         : 3; /* [23:21] */
            __IO uint32_t SUDAT      : 5; /* [28:24] */
            uint32_t RESERVED2       : 3; /* [31:29] */
        } bitfield;
    } SETUP; /* offset 0x02C */

} I2C_T;

#define I2C_ID_ADDR (uint32_t)&(I2C->ID)
#define I2C_REV_MINOR_ADDR (uint32_t)&(I2C->ID)
#define I2C_REV_MINOR_POS (0) /*< bit[3:0]    */
#define I2C_REV_MINOR_MASK (0xFul << I2C_REV_MINOR_POS)
#define I2C_REV_MAJOR_ADDR (uint32_t)&(I2C->ID)
#define I2C_REV_MAJOR_POS (4) /*< bit[11:4]   */
#define I2C_REV_MAJOR_MASK (0xFFul << I2C_REV_MAJOR_POS)
#define I2C_ID_NUM_ADDR (uint32_t)&(I2C->ID)
#define I2C_ID_NUM_POS (12) /*< bit[31:12]  */
#define I2C_ID_NUM_MASK (0xFFFFFul << I2C_ID_NUM_POS)

#define I2C_CFG_ADDR (uint32_t)&(I2C->CFG)
#define I2C_FIFOSize_ADDR (uint32_t)&(I2C->CFG)
#define I2C_FIFOSize_POS (0) /*< bit[1:0]    */
#define I2C_FIFOSize_MASK (0x3ul << I2C_FIFOSize_POS)

#define I2C_INT_EN_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_FIFOEmpty_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_FIFOEmpty_POS (0) /*< bit[0]      */
#define I2C_FIFOEmpty_MASK (0x1ul << I2C_FIFOEmpty_POS)
#define I2C_FIFOFull_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_FIFOFull_POS (1) /*< bit[1]      */
#define I2C_FIFOFull_MASK (0x1ul << I2C_FIFOFull_POS)
#define I2C_FIFOHalf_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_FIFOHalf_POS (2) /*< bit[2]      */
#define I2C_FIFOHalf_MASK (0x1ul << I2C_FIFOHalf_POS)
#define I2C_AddrHit_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_AddrHit_POS (3) /*< bit[3]      */
#define I2C_AddrHit_MASK (0x1ul << I2C_AddrHit_POS)
#define I2C_ArbLose_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_ArbLose_POS (4) /*< bit[4]      */
#define I2C_ArbLose_MASK (0x1ul << I2C_ArbLose_POS)
#define I2C_Stop_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_Stop_POS (5) /*< bit[5]      */
#define I2C_Stop_MASK (0x1ul << I2C_Stop_POS)
#define I2C_Start_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_Start_POS (6) /*< bit[6]      */
#define I2C_Start_MASK (0x1ul << I2C_Start_POS)
#define I2C_ByteTrans_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_ByteTrans_POS (7) /*< bit[7]      */
#define I2C_ByteTrans_MASK (0x1ul << I2C_ByteTrans_POS)
#define I2C_ByteRecv_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_ByteRecv_POS (8) /*< bit[8]      */
#define I2C_ByteRecv_MASK (0x1ul << I2C_ByteRecv_POS)
#define I2C_Cmpl_ADDR (uint32_t)&(I2C->INT_EN)
#define I2C_Cmpl_POS (9) /*< bit[9]      */
#define I2C_Cmpl_MASK (0x1ul << I2C_Cmpl_POS)

#define I2C_STS_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_FIFOEmpty_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_FIFOEmpty_POS (0) /*< bit[0]      */
#define I2C_STS_FIFOEmpty_MASK (0x1ul << I2C_STS_FIFOEmpty_POS)
#define I2C_STS_FIFOFull_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_FIFOFull_POS (1) /*< bit[1]      */
#define I2C_STS_FIFOFull_MASK (0x1ul << I2C_STS_FIFOFull_POS)
#define I2C_STS_FIFOHalf_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_FIFOHalf_POS (2) /*< bit[2]      */
#define I2C_STS_FIFOHalf_MASK (0x1ul << I2C_STS_FIFOHalf_POS)
#define I2C_STS_AddrHit_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_AddrHit_POS (3) /*< bit[3]      */
#define I2C_STS_AddrHit_MASK (0x1ul << I2C_STS_AddrHit_POS)
#define I2C_STS_ArbLose_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_ArbLose_POS (4) /*< bit[4]      */
#define I2C_STS_ArbLose_MASK (0x1ul << I2C_STS_ArbLose_POS)
#define I2C_STS_Stop_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_Stop_POS (5) /*< bit[5]      */
#define I2C_STS_Stop_MASK (0x1ul << I2C_STS_Stop_POS)
#define I2C_STS_Start_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_Start_POS (6) /*< bit[6]      */
#define I2C_STS_Start_MASK (0x1ul << I2C_STS_Start_POS)
#define I2C_STS_ByteTrans_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_ByteTrans_POS (7) /*< bit[7]      */
#define I2C_STS_ByteTrans_MASK (0x1ul << I2C_STS_ByteTrans_POS)
#define I2C_STS_ByteRecv_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_ByteRecv_POS (8) /*< bit[8]      */
#define I2C_STS_ByteRecv_MASK (0x1ul << I2C_STS_ByteRecv_POS)
#define I2C_STS_Cmpl_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_Cmpl_POS (9) /*< bit[9]      */
#define I2C_STS_Cmpl_MASK (0x1ul << I2C_STS_Cmpl_POS)
#define I2C_STS_ACK_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_ACK_POS (10) /*< bit[10]     */
#define I2C_STS_ACK_MASK (0x1ul << I2C_STS_ACK_POS)
#define I2C_STS_BusBusy_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_BusBusy_POS (11) /*< bit[11]     */
#define I2C_STS_BusBusy_MASK (0x1ul << I2C_STS_BusBusy_POS)
#define I2C_STS_GenCall_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_GenCall_POS (12) /*< bit[12]     */
#define I2C_STS_GenCall_MASK (0x1ul << I2C_STS_GenCall_POS)
#define I2C_STS_LineSCL_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_LineSCL_POS (13) /*< bit[13]     */
#define I2C_STS_LineSCL_MASK (0x1ul << I2C_STS_LineSCL_POS)
#define I2C_STS_LineSDA_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_LineSDA_POS (14) /*< bit[14]     */
#define I2C_STS_LineSDA_MASK (0x1ul << I2C_STS_LineSDA_POS)
#define I2C_STS_RESERVED_ADDR (uint32_t)&(I2C->STS)
#define I2C_STS_RESERVED_POS (15) /*< bit[31:15]  */
#define I2C_STS_RESERVED_MASK (0x1FFFFul << I2C_STS_RESERVED_POS)

#define I2C_ADDR_ADDR (uint32_t)&(I2C->ADDR)
#define I2C_ADDRESS_ADDR (uint32_t)&(I2C->ADDR)
#define I2C_ADDRESS_POS (0) /*< bit[9:0]    */
#define I2C_ADDRESS_MASK (0x3FFul << I2C_ADDRESS_POS)

#define I2C_DATA_ADDR (uint32_t)&(I2C->DATA)
#define I2C_DATAFIELD_ADDR (uint32_t)&(I2C->DATA)
#define I2C_DATAFIELD_POS (0) /*< bit[7:0]    */
#define I2C_DATAFIELD_MASK (0xFFul << I2C_DATAFIELD_POS)

#define I2C_CTRL_ADDR (uint32_t)&(I2C->CTRL)
#define I2C_DataCnt_ADDR (uint32_t)&(I2C->CTRL)
#define I2C_DataCnt_POS (0) /*< bit[7:0]    */
#define I2C_DataCnt_MASK (0xFFul << I2C_DataCnt_POS)
#define I2C_Dir_ADDR (uint32_t)&(I2C->CTRL)
#define I2C_Dir_POS (8) /*< bit[8]      */
#define I2C_Dir_MASK (0x1ul << I2C_Dir_POS)
#define I2C_Phase_stop_ADDR (uint32_t)&(I2C->CTRL)
#define I2C_Phase_stop_POS (9) /*< bit[9]      */
#define I2C_Phase_stop_MASK (0x1ul << I2C_Phase_stop_POS)
#define I2C_Phase_data_ADDR (uint32_t)&(I2C->CTRL)
#define I2C_Phase_data_POS (10) /*< bit[10]     */
#define I2C_Phase_data_MASK (0x1ul << I2C_Phase_data_POS)
#define I2C_Phase_addr_ADDR (uint32_t)&(I2C->CTRL)
#define I2C_Phase_addr_POS (11) /*< bit[11]     */
#define I2C_Phase_addr_MASK (0x1ul << I2C_Phase_addr_POS)
#define I2C_Phase_start_ADDR (uint32_t)&(I2C->CTRL)
#define I2C_Phase_start_POS (12) /*< bit[12]     */
#define I2C_Phase_start_MASK (0x1ul << I2C_Phase_start_POS)

#define I2C_CMD_ADDR (uint32_t)&(I2C->CMD)
#define I2C_CMD_ADDR (uint32_t)&(I2C->CMD)
#define I2C_CMD_POS (0) /*< bit[2:0]    */
#define I2C_CMD_MASK (0x7ul << I2C_CMD_POS)

#define I2C_SETUP_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_IICEn_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_IICEn_POS (0) /*< bit[0]      */
#define I2C_IICEn_MASK (0x1ul << I2C_IICEn_POS)
#define I2C_Addressing_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_Addressing_POS (1) /*< bit[1]      */
#define I2C_Addressing_MASK (0x1ul << I2C_Addressing_POS)
#define I2C_Master_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_Master_POS (2) /*< bit[2]      */
#define I2C_Master_MASK (0x1ul << I2C_Master_POS)
#define I2C_DMAEn_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_DMAEn_POS (3) /*< bit[3]      */
#define I2C_DMAEn_MASK (0x1ul << I2C_DMAEn_POS)
#define I2C_T_SCLHi_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_T_SCLHi_POS (4) /*< bit[12:4]   */
#define I2C_T_SCLHi_MASK (0x1FFul << I2C_T_SCLHi_POS)
#define I2C_T_SCLRatio_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_T_SCLRatio_POS (13) /*< bit[13]     */
#define I2C_T_SCLRatio_MASK (0x1ul << I2C_T_SCLRatio_POS)
#define I2C_T_HDDAT_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_T_HDDAT_POS (16) /*< bit[20:16]  */
#define I2C_T_HDDAT_MASK (0x1Ful << I2C_T_HDDAT_POS)
#define I2C_T_SP_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_T_SP_POS (21) /*< bit[23:21]  */
#define I2C_T_SP_MASK (0x7ul << I2C_T_SP_POS)
#define I2C_T_SUDAT_ADDR (uint32_t)&(I2C->SETUP)
#define I2C_T_SUDAT_POS (24) /*< bit[28:24]  */
#define I2C_T_SUDAT_MASK (0x1Ful << I2C_T_SUDAT_POS)

#define I2C_BASE 0x40035000UL
#define I2C ((I2C_T *)I2C_BASE)
#define I2C0 I2C
#define I2C1 ((I2C_T *)(I2C_BASE + 0x1000))

#endif /* __I2C_REG_ER8130_H__ */

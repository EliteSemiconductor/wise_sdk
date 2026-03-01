/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_uart_soc_b1_20240510.xlsx */
/* ================================================================================
 */
/* ================                     UART_T ================ */
/* ================================================================================
 */
#ifndef __UART_REG_ER8130_H__
#define __UART_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief UART (UART_T)
 */

typedef struct {     /*!< UART_T Structure     */
    __I uint32_t ID; /*!< ID Register (0x000) */
    __I uint32_t RESERVED0[3];
    __I uint32_t CFG;   /*!< CFG Register (0x010)   */
    __IO uint32_t OSCR; /*!< OSCR Register (0x014) */
    __I uint32_t RESERVED1[2];
    __IO uint32_t RBR_THR_DLL; /*!< RBR_THR_DLL Register (0x020) */
    __IO uint32_t IER_DLM;     /*!< IER_DLM Register (0x024)     */
    __O uint32_t IIR_FCR;      /*!< IIR_FCR Register (0x028)      */
    __IO uint32_t LCR;         /*!< LCR Register (0x02C)         */
    __IO uint32_t MCR;         /*!< MCR Register (0x030)         */
    union {
        __I uint32_t LSR; /*!< Line Status Register */

        struct {
            __I uint32_t DR : 1; /*!< Data ready.This bit is set when there are
                                    incoming received data in the Receiver
                                    Buffer Register (RBR). It is cleared when
                                       all of the received data are read. */
            __I uint32_t
                OE : 1; /*!< Overrun errorThis bit indicates that data in the
                           Receiver Buffer Register (RBR) is overrun. */
            __I uint32_t
                PE : 1; /*!< Parity errorThis bit is set when the received
                           parity does not match with the parity selected in the
                           LCR[5:4]. It is cleared when this register is read.In
                           the FIFO mode, this bit indicates the parity error
                           for the received data at the top of the RXFIFO. */
            __I uint32_t
                FE : 1; /*!< Framing errorThis bit is set when the received STOP
                           bit is not HIGH. It is cleared when this register is
                           read.In the FIFO mode, this bit indicates the framing
                           error for the received data at the top of the RXFIFO.
                         */
            __I uint32_t
                LBREAK : 1; /*!< Line breakThis bit is set when the uart_sin
                               input signal was held LOWfor longer than the time
                               for a full-word transmission. A full-word
                               transmission is the transmission of the START,
                               data, parity, and STOP bits. It is cleared when
                               this register is read.In the FIFO mode, this bit
                               indicates the line break for the received data at
                               the top of the RXFIFO. */
            __I uint32_t
                THRE : 1; /*!< Transmitter Holding Register emptyThis bit is 1
                             when the THR (TXFIFO in the FIFO mode) is empty.
                             Otherwise, it is zero.If the THRE interrupt is
                             enabled, an interrupt is triggered when THRE
                             becomes 1. */
            __I uint32_t TEMT : 1; /*!< Transmitter emptyThis bit is 1 when the
                                      THR (TXFIFO in the FIFO mode) and the
                                      Transmitter Shift Register (TSR) are both
                                      empty. Otherwise, it is zero. */
            __I uint32_t
                ERRF : 1; /*!< Error in RXFIFOIn the FIFO mode, this bit is set
                             when there is at least one parity error, framing
                             error, or line break associated with data in the
                             RXFIFO. It is cleared when this register is read
                             and there is no more error for the rest of data in
                             the RXFIFO. */
        } LSR_b;          /*!< BitSize          */
    };
    __I uint32_t MSR;  /*!< MSR Register (0x038)  */
    __IO uint32_t SCR; /*!< SCR Register (0x03C) */
} UART_T;

#define UART_ID_ADDR (uint32_t)&(UART->ID)
#define UART_REV_MINOR_ADDR (uint32_t)&(UART->ID)
#define UART_REV_MINOR_POS (0) /*< bit[3:0]    */
#define UART_REV_MINOR_MASK (0xFul << UART_REV_MINOR_POS)
#define UART_REV_MAJOR_ADDR (uint32_t)&(UART->ID)
#define UART_REV_MAJOR_POS (4) /*< bit[15:4]   */
#define UART_REV_MAJOR_MASK (0xFFFul << UART_REV_MAJOR_POS)
#define UART_ID_NUM_ADDR (uint32_t)&(UART->ID)
#define UART_ID_NUM_POS (16) /*< bit[31:16]  */
#define UART_ID_NUM_MASK (0xFFFFul << UART_ID_NUM_POS)

#define UART_CFG_ADDR (uint32_t)&(UART->CFG)
#define UART_FIFO_DEPTH_ADDR (uint32_t)&(UART->CFG)
#define UART_FIFO_DEPTH_POS (0) /*< bit[1:0]    */
#define UART_FIFO_DEPTH_MASK (0x3ul << UART_FIFO_DEPTH_POS)

#define UART_OSCR_ADDR (uint32_t)&(UART->OSCR)
#define UART_OSC_ADDR (uint32_t)&(UART->OSCR)
#define UART_OSC_POS (0) /*< bit[4:0]    */
#define UART_OSC_MASK (0x1Ful << UART_OSC_POS)

#define UART_RBR_THR_DLL_ADDR (uint32_t)&(UART->RBR_THR_DLL)
#define UART_RBR_THR_DLL_ADDR (uint32_t)&(UART->RBR_THR_DLL)
#define UART_RBR_THR_DLL_POS (0) /*< bit[7:0]    */
#define UART_RBR_THR_DLL_MASK (0xFFul << UART_RBR_THR_DLL_POS)

#define UART_IER_DLM_ADDR (uint32_t)&(UART->IER_DLM)
#define UART_ERBI_ADDR (uint32_t)&(UART->IER_DLM)
#define UART_ERBI_POS (0) /*< bit[0]      */
#define UART_ERBI_MASK (0x1ul << UART_ERBI_POS)
#define UART_ETHEI_ADDR (uint32_t)&(UART->IER_DLM)
#define UART_ETHEI_POS (1) /*< bit[1]      */
#define UART_ETHEI_MASK (0x1ul << UART_ETHEI_POS)
#define UART_ELSI_ADDR (uint32_t)&(UART->IER_DLM)
#define UART_ELSI_POS (2) /*< bit[2]      */
#define UART_ELSI_MASK (0x1ul << UART_ELSI_POS)
#define UART_EMSI_ADDR (uint32_t)&(UART->IER_DLM)
#define UART_EMSI_POS (3) /*< bit[3]      */
#define UART_EMSI_MASK (0x1ul << UART_EMSI_POS)

#define UART_IIR_FCR_ADDR (uint32_t)&(UART->IIR_FCR)
#define UART_INTRID_ADDR (uint32_t)&(UART->IIR_FCR)
#define UART_INTRID_POS (0) /*< bit[0]      */
#define UART_INTRID_MASK (0xFul << UART_INTRID_POS)
#define UART_FIFOE_ADDR (uint32_t)&(UART->IIR_FCR)
#define UART_FIFOE_POS (0) /*< bit[0]      */
#define UART_FIFOE_MASK (0x1ul << UART_FIFOE_POS)
#define UART_RFIFORST_ADDR (uint32_t)&(UART->IIR_FCR)
#define UART_RFIFORST_POS (1) /*< bit[1]      */
#define UART_RFIFORST_MASK (0x1ul << UART_RFIFORST_POS)
#define UART_TFIFORST_ADDR (uint32_t)&(UART->IIR_FCR)
#define UART_TFIFORST_POS (2) /*< bit[2]      */
#define UART_TFIFORST_MASK (0x1ul << UART_TFIFORST_POS)
#define UART_DMAE_ADDR (uint32_t)&(UART->IIR_FCR)
#define UART_DMAE_POS (3) /*< bit[3]      */
#define UART_DMAE_MASK (0x1ul << UART_DMAE_POS)
#define UART_TFIFOT_ADDR (uint32_t)&(UART->IIR_FCR)
#define UART_TFIFOT_POS (4) /*< bit[5:4]    */
#define UART_TFIFOT_MASK (0x3ul << UART_TFIFOT_POS)
#define UART_RFIFOT_ADDR (uint32_t)&(UART->IIR_FCR)
#define UART_RFIFOT_POS (6) /*< bit[7:6]    */
#define UART_RFIFOT_MASK (0x3ul << UART_RFIFOT_POS)

#define UART_LCR_ADDR (uint32_t)&(UART->LCR)
#define UART_WLS_ADDR (uint32_t)&(UART->LCR)
#define UART_WLS_POS (0) /*< bit[1:0]    */
#define UART_WLS_MASK (0x3ul << UART_WLS_POS)
#define UART_STB_ADDR (uint32_t)&(UART->LCR)
#define UART_STB_POS (2) /*< bit[2]      */
#define UART_STB_MASK (0x1ul << UART_STB_POS)
#define UART_PEN_ADDR (uint32_t)&(UART->LCR)
#define UART_PEN_POS (3) /*< bit[3]      */
#define UART_PEN_MASK (0x1ul << UART_PEN_POS)
#define UART_EPS_ADDR (uint32_t)&(UART->LCR)
#define UART_EPS_POS (4) /*< bit[4]      */
#define UART_EPS_MASK (0x1ul << UART_EPS_POS)
#define UART_SPS_ADDR (uint32_t)&(UART->LCR)
#define UART_SPS_POS (5) /*< bit[5]      */
#define UART_SPS_MASK (0x1ul << UART_SPS_POS)
#define UART_BC_ADDR (uint32_t)&(UART->LCR)
#define UART_BC_POS (6) /*< bit[6]      */
#define UART_BC_MASK (0x1ul << UART_BC_POS)
#define UART_DLAB_ADDR (uint32_t)&(UART->LCR)
#define UART_DLAB_POS (7) /*< bit[7]      */
#define UART_DLAB_MASK (0x1ul << UART_DLAB_POS)

#define UART_MCR_ADDR (uint32_t)&(UART->MCR)
#define UART_DTR_ADDR (uint32_t)&(UART->MCR)
#define UART_DTR_POS (0) /*< bit[0]      */
#define UART_DTR_MASK (0x1ul << UART_DTR_POS)
#define UART_RTS_ADDR (uint32_t)&(UART->MCR)
#define UART_RTS_POS (1) /*< bit[1]      */
#define UART_RTS_MASK (0x1ul << UART_RTS_POS)
#define UART_OUT1_ADDR (uint32_t)&(UART->MCR)
#define UART_OUT1_POS (2) /*< bit[2]      */
#define UART_OUT1_MASK (0x1ul << UART_OUT1_POS)
#define UART_OUT2_ADDR (uint32_t)&(UART->MCR)
#define UART_OUT2_POS (3) /*< bit[3]      */
#define UART_OUT2_MASK (0x1ul << UART_OUT2_POS)
#define UART_LOOP_ADDR (uint32_t)&(UART->MCR)
#define UART_LOOP_POS (4) /*< bit[4]      */
#define UART_LOOP_MASK (0x1ul << UART_LOOP_POS)
#define UART_AFE_ADDR (uint32_t)&(UART->MCR)
#define UART_AFE_POS (5) /*< bit[5]      */
#define UART_AFE_MASK (0x1ul << UART_AFE_POS)

#define UART_LSR_ADDR (uint32_t)&(UART->LSR)
#define UART_DR_ADDR (uint32_t)&(UART->LSR)
#define UART_DR_POS (0) /*< bit[0]      */
#define UART_DR_MASK (0x1ul << UART_DR_POS)
#define UART_OE_ADDR (uint32_t)&(UART->LSR)
#define UART_OE_POS (1) /*< bit[1]      */
#define UART_OE_MASK (0x1ul << UART_OE_POS)
#define UART_PE_ADDR (uint32_t)&(UART->LSR)
#define UART_PE_POS (2) /*< bit[2]      */
#define UART_PE_MASK (0x1ul << UART_PE_POS)
#define UART_FE_ADDR (uint32_t)&(UART->LSR)
#define UART_FE_POS (3) /*< bit[3]      */
#define UART_FE_MASK (0x1ul << UART_FE_POS)
#define UART_LBREAK_ADDR (uint32_t)&(UART->LSR)
#define UART_LBREAK_POS (4) /*< bit[4]      */
#define UART_LBREAK_MASK (0x1ul << UART_LBREAK_POS)
#define UART_THRE_ADDR (uint32_t)&(UART->LSR)
#define UART_THRE_POS (5) /*< bit[5]      */
#define UART_THRE_MASK (0x1ul << UART_THRE_POS)
#define UART_TEMT_ADDR (uint32_t)&(UART->LSR)
#define UART_TEMT_POS (6) /*< bit[6]      */
#define UART_TEMT_MASK (0x1ul << UART_TEMT_POS)
#define UART_ERRF_ADDR (uint32_t)&(UART->LSR)
#define UART_ERRF_POS (7) /*< bit[7]      */
#define UART_ERRF_MASK (0x1ul << UART_ERRF_POS)

#define UART_MSR_ADDR (uint32_t)&(UART->MSR)
#define UART_DCTS_ADDR (uint32_t)&(UART->MSR)
#define UART_DCTS_POS (0) /*< bit[0]      */
#define UART_DCTS_MASK (0x1ul << UART_DCTS_POS)
#define UART_DDSR_ADDR (uint32_t)&(UART->MSR)
#define UART_DDSR_POS (1) /*< bit[1]      */
#define UART_DDSR_MASK (0x1ul << UART_DDSR_POS)
#define UART_TERI_ADDR (uint32_t)&(UART->MSR)
#define UART_TERI_POS (2) /*< bit[2]      */
#define UART_TERI_MASK (0x1ul << UART_TERI_POS)
#define UART_DDCD_ADDR (uint32_t)&(UART->MSR)
#define UART_DDCD_POS (3) /*< bit[3]      */
#define UART_DDCD_MASK (0x1ul << UART_DDCD_POS)
#define UART_CTS_ADDR (uint32_t)&(UART->MSR)
#define UART_CTS_POS (4) /*< bit[4]      */
#define UART_CTS_MASK (0x1ul << UART_CTS_POS)
#define UART_DSR_ADDR (uint32_t)&(UART->MSR)
#define UART_DSR_POS (5) /*< bit[5]      */
#define UART_DSR_MASK (0x1ul << UART_DSR_POS)
#define UART_RI_ADDR (uint32_t)&(UART->MSR)
#define UART_RI_POS (6) /*< bit[6]      */
#define UART_RI_MASK (0x1ul << UART_RI_POS)
#define UART_DCD_ADDR (uint32_t)&(UART->MSR)
#define UART_DCD_POS (7) /*< bit[7]      */
#define UART_DCD_MASK (0x1ul << UART_DCD_POS)

#define UART_SCR_ADDR (uint32_t)&(UART->SCR)
#define UART_SCR_ADDR (uint32_t)&(UART->SCR)
#define UART_SCR_POS (0) /*< bit[7:0]    */
#define UART_SCR_MASK (0xFFul << UART_SCR_POS)

#define UART0_BASEADDR 0x40030000UL
#define UART1_BASEADDR (UART0_BASEADDR + 0x1000)
#define UART2_BASEADDR (UART0_BASEADDR + 0x2000)

#define UART0 ((UART_T *)UART0_BASEADDR)
#define UART1 ((UART_T *)UART1_BASEADDR)
#define UART2 ((UART_T *)UART2_BASEADDR)

#endif

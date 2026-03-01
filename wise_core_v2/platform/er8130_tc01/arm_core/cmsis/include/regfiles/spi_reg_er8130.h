/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_spi_soc_b1_20240413.xlsx */
/* ================================================================================
 */
/* ================                      SPI_T ================ */
/* ================================================================================
 */
#ifndef __SPI_REG_ER8130_H__
#define __SPI_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief SPI (SPI_T)
 */
typedef struct { /*!< SPI0 Structure */

    union {
        __I uint32_t ID; /*!< ID and Revision Register */

        struct {
            __I uint32_t REVMINOR : 4;  /*!< Minor revision number */
            __I uint32_t REVMAJOR : 8;  /*!< Major revision number */
            __I uint32_t ID       : 20; /*!< ID number for ATCSPI200 */
        } ID_b;                         /*!< BitSize */
    };
    __I uint32_t RESERVED0[3];

    union {
        __IO uint32_t TRANSFMT; /*!< SPI Transfer Format Register */

        struct {
            __IO uint32_t CPHA      : 1; /*!< SPI Clock Phase */
            __IO uint32_t CPOL      : 1; /*!< SPI Clock Polarity */
            __IO uint32_t SLVMODE   : 1; /*!< SPI Master/Slave mode selection */
            __IO uint32_t LSB       : 1; /*!< Transfer data with least significant
                                         bit first */
            __IO uint32_t MOSIBIDIR : 1; /*!< Bi-directional MOSI in regular
                                           (single) mode */
            uint32_t                : 2;
            __IO uint32_t DATAMERGE : 1; /*!< Data MergeThis bit only takes effect when
                                 DataLen = 0x7.If DataLen
                                 is 0x7 and DataMerge is 0x1, each access to the
                                 Data Register transfers four unit of data, each
                                 unit is a byte. */
            __IO uint32_t DATALEN   : 5; /*!< The length of each data unit in
                                         bitsThe actual bit number of a data
                                         unit is (DataLen + 1) */
            uint32_t                : 3;
            __IO uint32_t ADDRLEN   : 2; /*!< Address length in bytes */
        } TRANSFMT_b;                    /*!< BitSize */
    };

    union {
        __IO uint32_t DIRECTIO; /*!< SPI Direct IO Control Register */

        struct {
            __IO uint32_t CS_I       : 1; /*!< Status of the SPI CS (chip select)
                                        signal */
            __IO uint32_t SCLK_I     : 1; /*!< Status of the SPI SCLK signal */
            __IO uint32_t MOSI_I     : 1; /*!< Status of the SPI MOSI signalDepends */
            __IO uint32_t MISO_I     : 1; /*!< Status of the SPI MISO signal */
            __IO uint32_t WP_I       : 1; /*!< Status of the SPI Flash write
                                        protect signal */
            __IO uint32_t HOLD_I     : 1; /*!< Status of the SPI Flash hold signal */
            uint32_t                 : 2;
            __IO uint32_t CS_O       : 1; /*!< Output value for the SPI CS (chip
                                      select) signal */
            __IO uint32_t SCLK_O     : 1; /*!< Output value for the SPI SCLK signal */
            __IO uint32_t MOSI_O     : 1; /*!< Output value for the SPI MOSI signal */
            __IO uint32_t MISO_O     : 1; /*!< Output value for the SPI MISO signal */
            __IO uint32_t WP_O       : 1; /*!< Output value for the SPI Flash write
                                      protect signal */
            __IO uint32_t HOLD_O     : 1; /*!< Output value for the SPI Flash hold
                                        signal */
            uint32_t                 : 2;
            __IO uint32_t CS_OE      : 1; /*!< Output enable for SPI CS (chip select)
                                       signal */
            __IO uint32_t SCLK_OE    : 1; /*!< Output enable for the SPI SCLK signal */
            __IO uint32_t MOSI_OE    : 1; /*!< Output enable for the SPI MOSI signal */
            __IO uint32_t MISO_OE    : 1; /*!< Output enable fo the SPI MISO signal */
            __IO uint32_t WP_OE      : 1; /*!< Output enable for the SPI Flash write
                                       protect signal */
            __IO uint32_t HOLD_OE    : 1; /*!< Output enable for the SPI Flash hold
                                         signal */
            uint32_t                 : 2;
            __IO uint32_t DIRECTIOEN : 1; /*!< Enable Direct IO */
        } DIRECTIO_b;                     /*!< BitSize */
    };
    __I uint32_t RESERVED1[2];

    union {
        __IO uint32_t TRANSCTRL; /*!< SPI Transfer Control Register */

        struct {
            __IO uint32_t RDTRANCNT  : 9; /*!< Transfer count for read dataRdTranCnt
                                 indicates the number of
                                 units of data to be received from SPI bus and
                                 stored to the Date Register. The actual
                                 received count is (RdTranCnt+1).           */
            __IO uint32_t DUMMYCNT   : 2; /*!< Dummy data count. The actual dummy count
                                is (DummyCnt +1).The number of dummy cycles on
                                the SPI interface will be (DummyCnt+1)*
                                ((DataLen+1)/SPI IO width) */
            __IO uint32_t TOKENVALUE : 1; /*!< The value of the one-byte special
                                            token following the address phase
                                            for SPI read transfers */
            __IO uint32_t WRTRANCNT  : 9; /*!< Transfer count for write dataWrTranCnt
                                 indicates the number of units of data to be
                                 transmitted to the SPI bus from the Data
                                 Register. The actual transfer count is
                                 (WrTranCnt+1). */
            __IO uint32_t TOKENEN    : 1; /*!< Append an one-byte special token
                                           following the address phase
                                           for SPI read transfers. The value of the special
                                           token should be selected in the TokenValue. */
            __IO uint32_t DUALQUAD   : 2; /*!< The SPI data phase format */
            __IO uint32_t TRANSMODE  : 4; /*!< The transfer mode */
            __IO uint32_t ADDRFMT    : 1; /*!< The SPI address phase format */
            __IO uint32_t ADDREN     : 1; /*!< The SPI address phase enable */
            __IO uint32_t CMDEN      : 1; /*!< The SPI command phase enable */
            uint32_t                 : 1;
        } TRANSCTRL_b; /*!< BitSize */
    };

    union {
        __IO uint32_t CMD; /*!< SPI Command Register */

        struct {
            __IO uint32_t CMD : 8; /*!< SPI Command */
        } CMD_b;                   /*!< BitSize */
    };

    union {
        __IO uint32_t ADDR; /*!< SPI Address Register */

        struct {
            __IO uint32_t ADDR : 24; /*!< SPI Address */
        } ADDR_b;                    /*!< BitSize */
    };

    union {
        __IO uint32_t DATA; /*!< SPI Data Register */

        struct {
            __IO uint32_t DATA : 32; /*!< Data to transmit or the received dataFor
                             writes, data is enqueued to the TX FIFO. The least
                             significant byte is always transmitted first.For
                             reads, data is read and dequeued from the RX FIFO.
                             The least significant byte is the first received
                             byte. */
        } DATA_b;                    /*!< BitSize */
    };

    union {
        __IO uint32_t CTRL; /*!< SPI Control Register */

        struct {
            __IO uint32_t SPIRST    : 1; /*!< SPI resetWrite 1 to reset. It is cleared
                              to 0 after the reset operation completes. */
            __IO uint32_t RXFIFORST : 1; /*!< Receive FIFO resetWrite 1 to
                                           reset. It is cleared to 0 after the
                                           reset operation completes. */
            __IO uint32_t TXFIFORST : 1; /*!< Transmit FIFO resetWrite 1 to
                                           reset. It is cleared to 0 after the
                                           reset operation completes. */
            __IO uint32_t RXDMAEN   : 1; /*!< RX DMA enable */
            __IO uint32_t TXDMAEN   : 1; /*!< TX DMA enable */
            uint32_t                : 3;
            __IO uint32_t RXTHRES   : 5; /*!< Receive (RX) FIFO ThresholdThe RXFIFOInt
                               interrupt or DMA request would be issued for
                               consuming the RX FIFO when the RX data count is
                               more than or equal to the RX FIFO threshold. */
            uint32_t                : 3;
            __IO uint32_t TXTHRES   : 5; /*!< Transmit (TX) FIFO ThresholdThe TXFIFOInt
                               interrupt or DMA request would be issued to
                               replenish the TX FIFO when the TX data count is
                               less than or equal to the TX FIFO threshold. */
        } CTRL_b;                        /*!< BitSize */
    };

    union {
        __O uint32_t STATUS; /*!< SPI Status Register */

        struct {
            __O uint32_t SPIACTIVE : 1; /*!< SPI direct register programming is
                                          in progress */
            uint32_t               : 7;
            __O uint32_t RXNUM     : 5; /*!< Number of valid entries in the Receive
                                      FIFO */
            uint32_t               : 1;
            __O uint32_t RXEMPTY   : 1; /*!< Receive FIFO Empty flag */
            __O uint32_t RXFULL    : 1; /*!< Receive FIFO Full flag */
            __O uint32_t TXNUM     : 5; /*!< Number of valid entries in the Transmit
                                      FIFO */
            uint32_t               : 1;
            __O uint32_t TXEMPTY   : 1; /*!< Transmit FIFO Empty flag */
            __O uint32_t TXFULL    : 1; /*!< Transmit FIFO Full flag */
        } STATUS_b;                     /*!< BitSize */
    };

    union {
        __IO uint32_t INTREN; /*!< SPI Interrupt Enable Register */

        struct {
            __IO uint32_t RXFIFOORINTEN : 1; /*!< Enable the SPI Receive FIFO Overrun
                                     interruptControl whether interrupts are
                                     triggered when the Receive FIFO
                                     overflows.(Slave mode only) */
            __IO uint32_t TXFIFOURINTEN : 1; /*!< Enable the SPI Transmit FIFO Underrun
                                     interruptControl whether minterrupts are
                                     triggered when the Transmit FIFO run out of
                                     data.(Slave mode only) */
            __IO uint32_t RXFIFOINTEN   : 1; /*!< Enable the SPI Receive FIFO Threshold
                                   interruptControl whether interrupts are
                                   triggered when the valid entries are greater
                                   than or equal to the RX FIFO threshold. */
            __IO uint32_t TXFIFOINTEN   : 1; /*!< Enable the SPI Transmit FIFO Threshold
                                   interruptControl whether interrupts are
                                   triggered when the valid entries are less
                                   than or equal to the TX FIFO threshold. */
            __IO uint32_t ENDINTEN      : 1; /*!< Enable the End of SPI Transfer
                                interruptControl whether interrupts are
                                triggered when SPI transfers end.(In slave mode,
                                end of read status transaction doesnt trigger
                                this interrupt) */
            __IO uint32_t SLVCMDEN      : 1; /*!< Enable the Slave Command InterruptControl
                                whether interrupts are triggered whenever slave
                                commands are received in the Slave mode(Slave
                                mode only) */
        } INTREN_b;                          /*!< BitSize */
    };

    union {
        __O uint32_t INTRST; /*!< SPI Interrupt Status Register */

        struct {
            __O uint32_t RXFIFOORINT : 1; /*!< RX FIFO Overrun interruptThis bit is set
                                   when RX FIFO Overrun interrupts occur.(Slave
                                   mode only) */
            __O uint32_t TXFIFOURINT : 1; /*!< TX FIFO Underrun interruptThis bit is set
                                   when TX FIFO Underrun interrupts occur.(Slave
                                   mode only) m*/
            __O uint32_t RXFIFOINT   : 1; /*!< RX FIFO Threshold interruptThis bit is
                                 set when RX FIFO Threshold interrupts occur. */
            __O uint32_t TXFIFOINT   : 1; /*!< TX FIFO Threshold interruptThis bit is
                                 set when TX FIFO Threshold interrupts occur.*/
            __O uint32_t ENDINT      : 1; /*!< End of SPI Transfer interruptThis bit is
                              set when End of SPI Transfer interrupts occur. */
            __O uint32_t SLVCMDINT   : 1; /*!< Slave Command InterruptThis bit is
                                          set when Slave Command interrupts
                                          occur.(Slave mode only) */
        } INTRST_b;                       /*!< BitSize */
    };

    union {
        __IO uint32_t TIMING; /*!< SPI Interface Timing Register */

        struct {
            __IO uint32_t SCLK_DIV : 8; /*!< The clock frequency ratio between
                                          the clock source and SPI interface
                                          SCLKSCLK period = ((SCLK_DIV+1)2)
                                          (Period of the SPI clock source) */
            __IO uint32_t CSHT     : 4; /*!< The minimum time that SPI CS should stay
                            HIGHThe actual duration is (SCLK/2) X (CSHT+1) */
            __IO uint32_t CS2SCLK  : 2; /*!< The minimum time between the edges of SPI
                               CS and the edges of SCLKThe actual duration is
                               (SCLK/2) X (CS2SCLK+1) */
        } TIMING_b;                     /*!< BitSize */
    };
    __I uint32_t RESERVED2[3];

    union {
        __IO uint32_t MEMCTRL; /*!< SPI Memory Access Control Register */

        struct {
            __IO uint32_t MEMRDCMD   : 3; /*!< Selects the SPI command format when
                                          serving the memory mapped reads on the
                                          AHB/EILM bus */
            uint32_t                 : 5;
            __IO uint32_t MEMCTRLCHG : 1; /*!< This bit is set when this register (0x50)
                                  or the SPI Interface Timing Register (0x40) is
                                  written; it is automatically cleared when the
                                  new programming takes effect. */
        } MEMCTRL_b;                      /*!< BitSize */
    };
    __I uint32_t RESERVED3[3];

    union {
        __IO uint32_t SLVST; /*!< SPI Slave Status Register */

        struct {
            __IO uint32_t USR_STATUS : 16; /*!< User defined status flags */
            __IO uint32_t READY      : 1;  /*!< Set this bit to indicate that the
                                          ATCSPI200 is ready for data transaction.When an SPI
                                          transaction other than slave status-reading command
                                          ends, this bit will be cleared to 0. */
            __IO uint32_t OVERRUN    : 1;  /*!< Data overrun occurs in the last
                                          transaction */
            __IO uint32_t UNDERRUN   : 1;  /*!< Data underrun occurs in the last
                                          transaction */
        } SLVST_b;                         /*!< BitSize */
    };

    union {
        __I uint32_t SLVDATACNT; /*!< SPI Slave Data Count Register */

        struct {
            __I uint32_t RCNT : 9; /*!< Slave received data count */
            uint32_t          : 7;
            __I uint32_t WCNT : 9; /*!< Slave transmitted data count */
        } SLVDATACNT_b;            /*!< BitSize */
    };
    __I uint32_t RESERVED4[5];

    union {
        __I uint32_t CONFIG; /*!< Configuration Register */

        struct {
            __I uint32_t RXFIFOSIZE : 2; /*!< Depth of RX FIFO. */
            uint32_t                : 2;
            __I uint32_t TXFIFOSIZE : 2; /*!< Depth of TX FIFO. */
            uint32_t                : 2;
            __I uint32_t DUALSPI    : 1; /*!< Support for Dual I/O SPI. */
            __I uint32_t QUADSPI    : 1; /*!< Support for Quad I/O SPI. */
            uint32_t                : 1;
            __I uint32_t DIRECTIO   : 1; /*!< Support for Direct SPI IO. */
            __I uint32_t AHBMEM     : 1; /*!< Support for memory mapped access
                                         (read-only) through AHB bus */
            __I uint32_t EILMMEM    : 1; /*!< Support for memory mapped access
                                         (read-only) through EILM bus */
            __I uint32_t SLAVE      : 1; /*!< Support for SPI Slave mode */
        } CONFIG_b;                      /*!< BitSize m*/
    };
} SPI_T;

#define SPI_ID_ADDR (uint32_t)&(SPI->ID)
#define SPI_REV_MINOR_ADDR (uint32_t)&(SPI->ID)
#define SPI_REV_MINOR_POS (0) /*< bit[3:0]    */
#define SPI_REV_MINOR_MASK (0xFul << SPI_REV_MINOR_POS)
#define SPI_REV_MAJOR_ADDR (uint32_t)&(SPI->ID)
#define SPI_REV_MAJOR_POS (4) /*< bit[11:4]   */
#define SPI_REV_MAJOR_MASK (0xFFul << SPI_REV_MAJOR_POS)
#define SPI_ID_NUM_ADDR (uint32_t)&(SPI->ID)
#define SPI_ID_NUM_POS (12) /*< bit[31:12]  */
#define SPI_ID_NUM_MASK (0xFFFFFul << SPI_ID_NUM_POS)

#define SPI_TRANS_FMT_ADDR (uint32_t)&(SPI->TRANSFMT)
#define SPI_CPHA_ADDR (uint32_t)&(SPI->TRANSFMT)
#define SPI_CPHA_POS (0) /*< bit[0]      */
#define SPI_CPHA_MASK (0x1ul << SPI_CPHA_POS)
#define SPI_CPOL_ADDR (uint32_t)&(SPI->TRANSFMT)
#define SPI_CPOL_POS (1) /*< bit[1]      */
#define SPI_CPOL_MASK (0x1ul << SPI_CPOL_POS)
#define SPI_SLV_MODE_ADDR (uint32_t)&(SPI->TRANSFMT)
#define SPI_SLV_MODE_POS (2) /*< bit[2]      */
#define SPI_SLV_MODE_MASK (0x1ul << SPI_SLV_MODE_POS)
#define SPI_LSB_ADDR (uint32_t)&(SPI->TRANSFMT)
#define SPI_LSB_POS (3) /*< bit[3]      */
#define SPI_LSB_MASK (0x1ul << SPI_LSB_POS)
#define SPI_MOSI_BI_DIR_ADDR (uint32_t)&(SPI->TRANSFMT)
#define SPI_MOSI_BI_DIR_POS (4) /*< bit[4]      */
#define SPI_MOSI_BI_DIR_MASK (0x1ul << SPI_MOSI_BI_DIR_POS)
#define SPI_DATA_MERGE_ADDR (uint32_t)&(SPI->TRANS_FMT)
#define SPI_DATA_MERGE_POS (7) /*< bit[7]      */
#define SPI_DATA_MERGE_MASK (0x1ul << SPI_DATA_MERGE_POS)
#define SPI_DATA_LEN_ADDR (uint32_t)&(SPI->TRANSFMT)
#define SPI_DATA_LEN_POS (8) /*< bit[12:8]   */
#define SPI_DATA_LEN_MASK (0x1Ful << SPI_DATA_LEN_POS)
#define SPI_ADDR_LEN_ADDR (uint32_t)&(SPI->TRANSFMT)
#define SPI_ADDR_LEN_POS (16) /*< bit[17:16]  */
#define SPI_ADDR_LEN_MASK (0x3ul << SPI_ADDR_LEN_POS)

#define SPI_DIRECT_IO_CTRL_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_CS_I_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_CS_I_POS (0) /*< bit[0]      */
#define SPI_CS_I_MASK (0x1ul << SPI_CS_I_POS)
#define SPI_SCLK_I_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_SCLK_I_POS (1) /*< bit[1]      */
#define SPI_SCLK_I_MASK (0x1ul << SPI_SCLK_I_POS)
#define SPI_MOSI_I_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_MOSI_I_POS (2) /*< bit[2]      */
#define SPI_MOSI_I_MASK (0x1ul << SPI_MOSI_I_POS)
#define SPI_MISO_I_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_MISO_I_POS (3) /*< bit[3]      */
#define SPI_MISO_I_MASK (0x1ul << SPI_MISO_I_POS)
#define SPI_WP_I_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_WP_I_POS (4) /*< bit[4]      */
#define SPI_WP_I_MASK (0x1ul << SPI_WP_I_POS)
#define SPI_HOLD_I_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_HOLD_I_POS (5) /*< bit[6:5]    */
#define SPI_HOLD_I_MASK (0x3ul << SPI_HOLD_I_POS)
#define SPI_CS_O_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_CS_O_POS (8) /*< bit[8]      */
#define SPI_CS_O_MASK (0x1ul << SPI_CS_O_POS)
#define SPI_SCLK_O_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_SCLK_O_POS (9) /*< bit[9]      */
#define SPI_SCLK_O_MASK (0x1ul << SPI_SCLK_O_POS)
#define SPI_MOSI_O_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_MOSI_O_POS (10) /*< bit[10]     */
#define SPI_MOSI_O_MASK (0x1ul << SPI_MOSI_O_POS)
#define SPI_MISO_O_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_MISO_O_POS (11) /*< bit[11]     */
#define SPI_MISO_O_MASK (0x1ul << SPI_MISO_O_POS)
#define SPI_WP_O_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_WP_O_POS (12) /*< bit[12]     */
#define SPI_WP_O_MASK (0x1ul << SPI_WP_O_POS)
#define SPI_HOLD_O_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_HOLD_O_POS (13) /*< bit[13]     */
#define SPI_HOLD_O_MASK (0x1ul << SPI_HOLD_O_POS)
#define SPI_CS_OE_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_CS_OE_POS (16) /*< bit[16]     */
#define SPI_CS_OE_MASK (0x1ul << SPI_CS_OE_POS)
#define SPI_SCLK_OE_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_SCLK_OE_POS (17) /*< bit[17]     */
#define SPI_SCLK_OE_MASK (0x1ul << SPI_SCLK_OE_POS)
#define SPI_MOSI_SE_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_MOSI_SE_POS (18) /*< bit[18]     */
#define SPI_MOSI_SE_MASK (0x1ul << SPI_MOSI_SE_POS)
#define SPI_MISO_SE_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_MISO_SE_POS (19) /*< bit[19]     */
#define SPI_MISO_SE_MASK (0x1ul << SPI_MISO_SE_POS)
#define SPI_WP_OE_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_WP_OE_POS (20) /*< bit[20]     */
#define SPI_WP_OE_MASK (0x1ul << SPI_WP_OE_POS)
#define SPI_HOLD_OE_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_HOLD_OE_POS (21) /*< bit[21]     */
#define SPI_HOLD_OE_MASK (0x1ul << SPI_HOLD_OE_POS)
#define SPI_DIRECT_IO_EN_ADDR (uint32_t)&(SPI->DIRECT_IO_CTRL)
#define SPI_DIRECT_IO_EN_POS (24) /*< bit[24]     */
#define SPI_DIRECT_IO_EN_MASK (0x1ul << SPI_DIRECT_IO_EN_POS)

#define SPI_TRANS_CTRL_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_RD_TRAN_CNT_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_RD_TRAN_CNT_POS (0) /*< bit[8:0]    */
#define SPI_RD_TRAN_CNT_MASK (0x1FFul << SPI_RD_TRAN_CNT_POS)
#define SPI_DMY_CNT_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_DMY_CNT_POS (9) /*< bit[10:9]   */
#define SPI_DMY_CNT_MASK (0x3ul << SPI_DMY_CNT_POS)
#define SPI_TOKEN_VAL_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_TOKEN_VAL_POS (11) /*< bit[11]     */
#define SPI_TOKEN_VAL_MASK (0x1ul << SPI_TOKEN_VAL_POS)
#define SPI_WR_TRAN_CNT_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_WR_TRAN_CNT_POS (12) /*< bit[20:12]  */
#define SPI_WR_TRAN_CNT_MASK (0x1FFul << SPI_WR_TRAN_CNT_POS)
#define SPI_TOKEN_EN_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_TOKEN_EN_POS (21) /*< bit[21]     */
#define SPI_TOKEN_EN_MASK (0x1ul << SPI_TOKEN_EN_POS)
#define SPI_DUAL_QUAD_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_DUAL_QUAD_POS (22) /*< bit[23:22]  */
#define SPI_DUAL_QUAD_MASK (0x3ul << SPI_DUAL_QUAD_POS)
#define SPI_TRANS_MODE_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_TRANS_MODE_POS (24) /*< bit[27:24]  */
#define SPI_TRANS_MODE_MASK (0xFul << SPI_TRANS_MODE_POS)
#define SPI_ADDR_FMT_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_ADDR_FMT_POS (28) /*< bit[28]     */
#define SPI_ADDR_FMT_MASK (0x1ul << SPI_ADDR_FMT_POS)
#define SPI_ADDR_EN_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_ADDR_EN_POS (29) /*< bit[29]     */
#define SPI_ADDR_EN_MASK (0x1ul << SPI_ADDR_EN_POS)
#define SPI_CMD_EN_ADDR (uint32_t)&(SPI->TRANSCTRL)
#define SPI_CMD_EN_POS (30) /*< bit[30]     */
#define SPI_CMD_EN_MASK (0x1ul << SPI_CMD_EN_POS)

#define SPI_CMD_ADDR (uint32_t)&(SPI->CMD)
#define SPI_CMD_ADDR (uint32_t)&(SPI->CMD)
#define SPI_CMD_POS (0) /*< bit[7:0]    */
#define SPI_CMD_MASK (0xFFul << SPI_CMD_POS)

#define SPI_ADDR_ADDR (uint32_t)&(SPI->ADDR)
#define SPI_ADDR_ADDR (uint32_t)&(SPI->ADDR)
#define SPI_ADDR_POS (0) /*< bit[23:0]   */
#define SPI_ADDR_MASK (0xFFFFFFul << SPI_ADDR_POS)

#define SPI_DATA_ADDR (uint32_t)&(SPI->DATA)
#define SPI_CTRL_ADDR (uint32_t)&(SPI->CTRL)
#define SPI_SPI_RST_ADDR (uint32_t)&(SPI->CTRL)
#define SPI_SPI_RST_POS (0) /*< bit[0]      */
#define SPI_SPI_RST_MASK (0x1ul << SPI_SPI_RST_POS)
#define SPI_RX_FIFO_RST_ADDR (uint32_t)&(SPI->CTRL)
#define SPI_RX_FIFO_RST_POS (1) /*< bit[1]      */
#define SPI_RX_FIFO_RST_MASK (0x1ul << SPI_RX_FIFO_RST_POS)
#define SPI_TX_FIFO_RST_ADDR (uint32_t)&(SPI->CTRL)
#define SPI_TX_FIFO_RST_POS (2) /*< bit[2]      */
#define SPI_TX_FIFO_RST_MASK (0x1ul << SPI_TX_FIFO_RST_POS)
#define SPI_RX_DMA_EN_ADDR (uint32_t)&(SPI->CTRL)
#define SPI_RX_DMA_EN_POS (3) /*< bit[3]      */
#define SPI_RX_DMA_EN_MASK (0x1ul << SPI_RX_DMA_EN_POS)
#define SPI_TX_DMA_EN_ADDR (uint32_t)&(SPI->CTRL)
#define SPI_TX_DMA_EN_POS (4) /*< bit[4]      */
#define SPI_TX_DMA_EN_MASK (0x1ul << SPI_TX_DMA_EN_POS)
#define SPI_RX_THRES_ADDR (uint32_t)&(SPI->CTRL)
#define SPI_RX_THRES_POS (8) /*< bit[12:8]   */
#define SPI_RX_THRES_MASK (0x1Ful << SPI_RX_THRES_POS)
#define SPI_TX_THRES_ADDR (uint32_t)&(SPI->CTRL)
#define SPI_TX_THRES_POS (16) /*< bit[20:16]  */
#define SPI_TX_THRES_MASK (0x1Ful << SPI_TX_THRES_POS)

#define SPI_STS_ADDR (uint32_t)&(SPI->STATUS)
#define SPI_SPI_ACTIVE_ADDR (uint32_t)&(SPI->STATUS)
#define SPI_SPI_ACTIVE_POS (0) /*< bit[0]      */
#define SPI_SPI_ACTIVE_MASK (0x1ul << SPI_SPI_ACTIVE_POS)
#define SPI_RX_NUM_ADDR (uint32_t)&(SPI->STATUS)
#define SPI_RX_NUM_POS (8) /*< bit[12:8]   */
#define SPI_RX_NUM_MASK (0x1Ful << SPI_RX_NUM_POS)
#define SPI_RX_EMPTY_ADDR (uint32_t)&(SPI->STATUS)
#define SPI_RX_EMPTY_POS (14) /*< bit[14]     */
#define SPI_RX_EMPTY_MASK (0x1ul << SPI_RX_EMPTY_POS)
#define SPI_RX_FULL_ADDR (uint32_t)&(SPI->STATUS)
#define SPI_RX_FULL_POS (15) /*< bit[15]     */
#define SPI_RX_FULL_MASK (0x1ul << SPI_RX_FULL_POS)
#define SPI_TX_NUM_ADDR (uint32_t)&(SPI->STATUS)
#define SPI_TX_NUM_POS (16) /*< bit[20:16]  */
#define SPI_TX_NUM_MASK (0x1Ful << SPI_TX_NUM_POS)
#define SPI_TX_EMPTY_ADDR (uint32_t)&(SPI->STATUS)
#define SPI_TX_EMPTY_POS (22) /*< bit[22]     */
#define SPI_TX_EMPTY_MASK (0x1ul << SPI_TX_EMPTY_POS)
#define SPI_TX_FULL_ADDR (uint32_t)&(SPI->STATUS)
#define SPI_TX_FULL_POS (23) /*< bit[23]     */
#define SPI_TX_FULL_MASK (0x1ul << SPI_TX_FULL_POS)

#define SPI_INT_EN_ADDR (uint32_t)&(SPI->INTREN)
#define SPI_RX_FIFO_UR_INT_EN_ADDR (uint32_t)&(SPI->INTREN)
#define SPI_RX_FIFO_UR_INT_EN_POS (0) /*< bit[0]      */
#define SPI_RX_FIFO_UR_INT_EN_MASK (0x1ul << SPI_RX_FIFO_UR_INT_EN_POS)
#define SPI_TX_FIFO_UR_INT_EN_ADDR (uint32_t)&(SPI->INTREN)
#define SPI_TX_FIFO_UR_INT_EN_POS (1) /*< bit[1]      */
#define SPI_TX_FIFO_UR_INT_EN_MASK (0x1ul << SPI_TX_FIFO_UR_INT_EN_POS)
#define SPI_RX_FIFO_INT_EN_ADDR (uint32_t)&(SPI->INTREN)
#define SPI_RX_FIFO_INT_EN_POS (2) /*< bit[2]      */
#define SPI_RX_FIFO_INT_EN_MASK (0x1ul << SPI_RX_FIFO_INT_EN_POS)
#define SPI_TX_FIFO_INT_EN_ADDR (uint32_t)&(SPI->INTREN)
#define SPI_TX_FIFO_INT_EN_POS (3) /*< bit[3]      */
#define SPI_TX_FIFO_INT_EN_MASK (0x1ul << SPI_TX_FIFO_INT_EN_POS)
#define SPI_END_INT_EN_ADDR (uint32_t)&(SPI->INTREN)
#define SPI_END_INT_EN_POS (4) /*< bit[4]      */
#define SPI_END_INT_EN_MASK (0x1ul << SPI_END_INT_EN_POS)
#define SPI_SLV_CMD_EN_ADDR (uint32_t)&(SPI->INTREN)
#define SPI_SLV_CMD_EN_POS (5) /*< bit[5]      */
#define SPI_SLV_CMD_EN_MASK (0x1ul << SPI_SLV_CMD_EN_POS)

#define SPI_INT_STS_ADDR (uint32_t)&(SPI->INTRST)
#define SPI_RX_FIFO_OR_INT_ADDR (uint32_t)&(SPI->INTRST)
#define SPI_RX_FIFO_OR_INT_POS (0) /*< bit[0]      */
#define SPI_RX_FIFO_OR_INT_MASK (0x1ul << SPI_RX_FIFO_OR_INT_POS)
#define SPI_TX_FIFO_OR_INT_ADDR (uint32_t)&(SPI->INTRST)
#define SPI_TX_FIFO_OR_INT_POS (1) /*< bit[1]      */
#define SPI_TX_FIFO_OR_INT_MASK (0x1ul << SPI_TX_FIFO_OR_INT_POS)
#define SPI_RX_FIFO_INT_ADDR (uint32_t)&(SPI->INTRST)
#define SPI_RX_FIFO_INT_POS (2) /*< bit[2]      */
#define SPI_RX_FIFO_INT_MASK (0x1ul << SPI_RX_FIFO_INT_POS)
#define SPI_TX_FIFO_INT_ADDR (uint32_t)&(SPI->INTRST)
#define SPI_TX_FIFO_INT_POS (3) /*< bit[3]      */
#define SPI_TX_FIFO_INT_MASK (0x1ul << SPI_TX_FIFO_INT_POS)
#define SPI_END_INT_ADDR (uint32_t)&(SPI->INTRST)
#define SPI_END_INT_POS (4) /*< bit[4]      */
#define SPI_END_INT_MASK (0x1ul << SPI_END_INT_POS)
#define SPI_SLV_CMD_INT_ADDR (uint32_t)&(SPI->INTRST)
#define SPI_SLV_CMD_INT_POS (5) /*< bit[5]      */
#define SPI_SLV_CMD_INT_MASK (0x1ul << SPI_SLV_CMD_INT_POS)

#define SPI_INTF_TIMING_ADDR (uint32_t)&(SPI->TIMING)
#define SPI_SCLK_DIV_ADDR (uint32_t)&(SPI->TIMING)
#define SPI_SCLK_DIV_POS (0) /*< bit[7:0]    */
#define SPI_SCLK_DIV_MASK (0xFFul << SPI_SCLK_DIV_POS)
#define SPI_CSHT_ADDR (uint32_t)&(SPI->TIMING)
#define SPI_CSHT_POS (8) /*< bit[11:8]   */
#define SPI_CSHT_MASK (0xFul << SPI_CSHT_POS)
#define SPI_CS2_SCLK_ADDR (uint32_t)&(SPI->TIMING)
#define SPI_CS2_SCLK_POS (12) /*< bit[13:12]  */
#define SPI_CS2_SCLK_MASK (0x3ul << SPI_CS2_SCLK_POS)

#define SPI_MEM_CTRL_ADDR (uint32_t)&(SPI->MEM_CTRL)
#define SPI_MEM_RD_CMD_ADDR (uint32_t)&(SPI->MEM_CTRL)
#define SPI_MEM_RD_CMD_POS (0) /*< bit[2:0]    */
#define SPI_MEM_RD_CMD_MASK (0x7ul << SPI_MEM_RD_CMD_POS)
#define SPI_MEM_CTRL_CHG_ADDR (uint32_t)&(SPI->MEM_CTRL)
#define SPI_MEM_CTRL_CHG_POS (8) /*< bit[8]      */
#define SPI_MEM_CTRL_CHG_MASK (0x1ul << SPI_MEM_CTRL_CHG_POS)

#define SPI_SLV_STS_ADDR (uint32_t)&(SPI->SLVST)
#define SPI_USR_STS_ADDR (uint32_t)&(SPI->SLVST)
#define SPI_USR_STS_POS (0) /*< bit[15:0]   */
#define SPI_USR_STS_MASK (0xFFFFul << SPI_USR_STS_POS)
#define SPI_READY_ADDR (uint32_t)&(SPI->SLVST)
#define SPI_READY_POS (16) /*< bit[16]     */
#define SPI_READY_MASK (0x1ul << SPI_READY_POS)
#define SPI_OVER_RUN_ADDR (uint32_t)&(SPI->SLVST)
#define SPI_OVER_RUN_POS (17) /*< bit[17]     */
#define SPI_OVER_RUN_MASK (0x1ul << SPI_OVER_RUN_POS)
#define SPI_UNDER_RUN_ADDR (uint32_t)&(SPI->SLVST)
#define SPI_UNDER_RUN_POS (18) /*< bit[18]     */
#define SPI_UNDER_RUN_MASK (0x1ul << SPI_UNDER_RUN_POS)

#define SPI_SLV_DATA_CNT_ADDR (uint32_t)&(SPI->SLVDATACNT)
#define SPI_RCNT_ADDR (uint32_t)&(SPI->SLVDATACNT)
#define SPI_RCNT_POS (0) /*< bit[8:0]    */
#define SPI_RCNT_MASK (0x1FFul << SPI_RCNT_POS)
#define SPI_WCNT_ADDR (uint32_t)&(SPI->SLVDATACNT)
#define SPI_WCNT_POS (16) /*< bit[24:16]  */
#define SPI_WCNT_MASK (0x1FFul << SPI_WCNT_POS)

#define SPI_CFG_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_RX_FIFO_SIZE_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_RX_FIFO_SIZE_POS (0) /*< bit[1:0]    */
#define SPI_RX_FIFO_SIZE_MASK (0x3ul << SPI_RX_FIFO_SIZE_POS)
#define SPI_TX_FIFO_SIZE_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_TX_FIFO_SIZE_POS (4) /*< bit[5:4]    */
#define SPI_TX_FIFO_SIZE_MASK (0x3ul << SPI_TX_FIFO_SIZE_POS)
#define SPI_DUAL_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_DUAL_POS (8) /*< bit[8]      */
#define SPI_DUAL_MASK (0x1ul << SPI_DUAL_POS)
#define SPI_QUAD_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_QUAD_POS (9) /*< bit[9]      */
#define SPI_QUAD_MASK (0x1ul << SPI_QUAD_POS)
#define SPI_DIRECT_IO_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_DIRECT_IO_POS (11) /*< bit[11]     */
#define SPI_DIRECT_IO_MASK (0x1ul << SPI_DIRECT_IO_POS)
#define SPI_AHB_MEM_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_AHB_MEM_POS (12) /*< bit[12]     */
#define SPI_AHB_MEM_MASK (0x1ul << SPI_AHB_MEM_POS)
#define SPI_EILM_MEM_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_EILM_MEM_POS (13) /*< bit[13]     */
#define SPI_EILM_MEM_MASK (0x1ul << SPI_EILM_MEM_POS)
#define SPI_SLV_ADDR (uint32_t)&(SPI->CONFIG)
#define SPI_SLV_POS (14) /*< bit[14]     */
#define SPI_SLV_MASK (0x1ul << SPI_SLV_POS)

#define SPI_BASE 0x40033000UL
#define SPI0 ((SPI_T *)SPI_BASE)
#define SPI1 ((SPI_T *)(SPI_BASE + 0x1000))

#endif /* __SPI_REG_ER8130_H__ */

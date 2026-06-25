/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_spi_api.h
 * @brief SPI (Serial Peripheral Interface) driver APIs.
 *
 * @ingroup WISE_SPI
 *
 * This header provides the interfaces for configuring and using SPI in
 * master/slave mode, including clock settings, data format, transfer
 * message structures, and event callback registration.
 */

#ifndef __WISE_SPI_API_H
#define __WISE_SPI_API_H

#include "api/wise_gpio_api.h"
#include "hal_intf_pmu.h"
#include "hal_intf_spi.h"
#include "types.h"
#include "wise_core.h"
#include <stdint.h>

/**
 * @defgroup WISE_SPI WISE SPI APIs
 * @ingroup WISE_CORE_API
 * @brief SPI configuration, transfer and event callback interfaces.
 * @{
 */

/**
 * @typedef SPI_EVT_CALLBACK_T
 * @brief SPI event callback function type.
 *
 * @param spi_channel SPI channel index.
 * @param spi_event   Event code (one of ::WISE_SPI_CB_EVENT_T or HAL-defined).
 * @param context     User-defined context pointer.
 */
typedef void (*SPI_EVT_CALLBACK_T)(uint8_t spi_channel, uint32_t spi_event, void *context);

/* ================================================================== */
/*  Enums                                                             */
/* ================================================================== */

/**
 * @enum WISE_SPI_ENABLE_T
 * @brief Generic 0/1 toggle for SPI struct fields.
 */
typedef enum {
    WISE_SPI_DISABLE = 0,
    WISE_SPI_ENABLE  = 1,
} WISE_SPI_ENABLE_T;

/**
 * @enum WISE_SPI_ROLE
 * @brief SPI role (selected by which @c _open function is called).
 */
typedef enum {
    E_SPI_ROLE_MASTER = 0, /**< SPI master role. */
    E_SPI_ROLE_SLAVE,      /**< SPI slave role. */
} WISE_SPI_ROLE;

/**
 * @enum WISE_SPI_BLOCK_MODE_T
 * @brief SPI operation blocking mode (software policy).
 */
typedef enum {
    E_SPI_BLOCK_MODE = 0, /**< Blocking operations (API waits until done). */
    E_SPI_NONBLOCK_MODE,  /**< Non-blocking operations. */
} WISE_SPI_BLOCK_MODE_T;

/**
 * @enum SPI_STATE
 * @brief SPI interface state.
 */
typedef enum {
    E_SPI_STATE_OFF = 0, /**< SPI interface disabled. */
    E_SPI_STATE_ON,      /**< SPI interface enabled. */
} SPI_STATE;

/**
 * @enum WISE_SPI_CLOCK_MODE_T
 * @brief SPI clock polarity and phase (TransFmt.CPOL bit 1, CPHA bit 0).
 *
 * @b CPOL sets the SCLK @e idle level. @b CPHA selects the @e sampling edge --
 * per datasheet: CPHA=0 samples on odd (leading) SCLK edges, CPHA=1 on even
 * (trailing) edges. The sender shifts data out on the opposite edge so it is
 * stable when the receiver samples.
 *
 *   CPOL=0 (idle LOW):              CPOL=1 (idle HIGH):
 *           ┌──┐  ┌──┐                     ┌──┐  ┌──┐
 *   SCLK ───┘  └──┘  └──            SCLK ──┘  └──┘  └──   (drawn idle-low)
 *           ↑  ↓                    -> for CPOL=1 the waveform is inverted:
 *        CPHA0 CPHA1                   idle high, leading edge = falling.
 *
 *   With CPHA=0 the FIRST (leading) edge of each SCLK period samples; with
 *   CPHA=1 the SECOND (trailing) edge samples.
 *
 *   Mode  CPOL CPHA  SCLK idle  Data sampled on
 *   ----  ---- ----  ---------  -----------------------------
 *    0     0    0     low        1st edge (leading,  rising)
 *    1     0    1     low        2nd edge (trailing, falling)
 *    2     1    0     high       1st edge (leading,  falling)
 *    3     1    1     high       2nd edge (trailing, rising)
 *
 * Master and slave MUST use the same mode. Mode 0 is the common default.
 */
typedef enum {
    CLOCK_MODE0 = 0, /**< CPOL = 0, CPHA = 0. Idle low,  sample on rising (1st) edge. */
    CLOCK_MODE1 = 1, /**< CPOL = 0, CPHA = 1. Idle low,  sample on falling (2nd) edge. */
    CLOCK_MODE2 = 2, /**< CPOL = 1, CPHA = 0. Idle high, sample on falling (1st) edge. */
    CLOCK_MODE3 = 3, /**< CPOL = 1, CPHA = 1. Idle high, sample on rising (2nd) edge. */
} WISE_SPI_CLOCK_MODE_T;

/**
 * @enum WISE_SPI_BIT_ORDER_T
 * @brief Bit order on the wire (TransFmt.LSB, bit 3).
 */
typedef enum {
    SPI_MSB_FIRST = 0, /**< Most significant bit first. */
    SPI_LSB_FIRST,     /**< Least significant bit first. */
} WISE_SPI_BIT_ORDER_T;

/**
 * @enum WISE_SPI_MASTER_CLOCK_SEL_T
 * @brief SPI master clock selection (drives the SCLK divider).
 *
 * Defines the supported set of SPI master SCLK frequencies. The HAL
 * maps each enum value to the corresponding SCLK divider on the
 * underlying ATCSPI200 controller.
 *
 * @note This selects the SCLK driven in MASTER mode. In SLAVE mode the master
 *       drives SCLK, so this field is ignored; the slave can only RECEIVE SCLK
 *       up to 10 MHz reliably (do not clock a slave faster than
 *       ::E_SPI_CLOCK_SEL_10M).
 */
typedef enum {
    E_SPI_CLOCK_SEL_100K = 0, /**< 100 kHz SPI clock. */
    E_SPI_CLOCK_SEL_200K,     /**< 200 kHz SPI clock. */
    E_SPI_CLOCK_SEL_400K,     /**< 400 kHz SPI clock. */
    E_SPI_CLOCK_SEL_500K,     /**< 500 kHz SPI clock. */
    E_SPI_CLOCK_SEL_800K,     /**< 800 kHz SPI clock. */
    E_SPI_CLOCK_SEL_1M,       /**< 1 MHz SPI clock. */
    E_SPI_CLOCK_SEL_2M,       /**< 2 MHz SPI clock. */
    E_SPI_CLOCK_SEL_4M,       /**< 4 MHz SPI clock. */
    E_SPI_CLOCK_SEL_5M,       /**< 5 MHz SPI clock. */
    E_SPI_CLOCK_SEL_10M,      /**< 10 MHz SPI clock. Max for SLAVE-mode RX. */
    E_SPI_CLOCK_SEL_20M,      /**< 20 MHz SPI clock. MASTER mode only. */
    E_SPI_CLOCK_SEL_MAX       /**< Sentinel, not a valid selection. */
} WISE_SPI_MASTER_CLOCK_SEL_T;

/**
 * @enum WISE_SPI_IO_MODE_T
 * @brief Data wiring + lane width of the channel, set once at open.
 *
 * Combines the two HW knobs that together describe the data path -- 3-wire
 * MOSI-bidirectional (TransFmt.MOSIBiDir, bit 4) and the data-lane count
 * (TransCtrl.DualQuad, bits 23:22) -- into one choice, because the only
 * meaningful combinations are these four (3-wire is single-lane only; dual/quad
 * are inherently bidirectional, so MOSIBiDir does not apply to them). The driver
 * derives MOSIBiDir and DualQuad from this value.
 *
 * @note SLAVE mode supports SINGLE / 3WIRE only. DUAL and QUAD are MASTER-only:
 *       multi-lane slave RECEIVE is unreliable on this controller (premature-END
 *       / leading-byte loss -- HW errata), so ::wise_spi_slave_open rejects
 *       DUAL/QUAD and returns ::WISE_FAIL.
 */
typedef enum {
    WISE_SPI_IO_SINGLE = 0, /**< 1-bit, separate MOSI/MISO (4-wire, full duplex). */
    WISE_SPI_IO_3WIRE  = 1, /**< 1-bit, MOSI bidirectional (3-wire, half duplex). */
    WISE_SPI_IO_DUAL   = 2, /**< 2-bit data IO (half duplex). Master only. */
    WISE_SPI_IO_QUAD   = 3, /**< 4-bit data IO (half duplex). Master only. */
} WISE_SPI_IO_MODE_T;

/**
 * @enum WISE_SPI_TRANS_MODE_T
 * @brief Transfer sequence (TransCtrl.TransMode, bits 27:24).
 *
 * Numeric values match the hardware encoding exactly.
 */
typedef enum {
    WISE_SPI_TM_WRITE_AND_READ  = 0x0, /**< Write and read concurrently. */
    WISE_SPI_TM_WRITE_ONLY      = 0x1, /**< Write only. */
    WISE_SPI_TM_READ_ONLY       = 0x2, /**< Read only. */
    WISE_SPI_TM_WRITE_THEN_READ = 0x3, /**< Write then read. */
    WISE_SPI_TM_READ_THEN_WRITE = 0x4, /**< Read then write. */
    WISE_SPI_TM_WRITE_DMY_READ  = 0x5, /**< Write, dummy cycles, then read. */
    WISE_SPI_TM_READ_DMY_WRITE  = 0x6, /**< Read, dummy cycles, then write. */
    WISE_SPI_TM_NONE_DATA       = 0x7, /**< No data phase (caller must enable cmd_en or addr_len > NONE). */
    WISE_SPI_TM_DMY_WRITE       = 0x8, /**< Dummy then write. */
    WISE_SPI_TM_DMY_READ        = 0x9, /**< Dummy then read. */
} WISE_SPI_TRANS_MODE_T;

/**
 * @enum WISE_SPI_ADDR_LEN_T
 * @brief Address phase byte width (TransFmt.AddrLen, bits 17:16 +
 *        TransCtrl.AddrEn enable bit).
 *
 * ::WISE_SPI_ADDR_NONE clears AddrEn (no address phase issued).
 */
typedef enum {
    WISE_SPI_ADDR_NONE = 0, /**< No address phase (AddrEn = 0). */
    WISE_SPI_ADDR_1B   = 1, /**< 1-byte address. */
    WISE_SPI_ADDR_2B   = 2, /**< 2-byte address. */
    WISE_SPI_ADDR_3B   = 3, /**< 3-byte address (24-bit). */
} WISE_SPI_ADDR_LEN_T;

/**
 * @enum WISE_SPI_ADDR_FMT_T
 * @brief Address phase IO format (TransCtrl.AddrFmt, bit 28).
 *
 * Honoured only when @c addr_len != ::WISE_SPI_ADDR_NONE.
 */
typedef enum {
    WISE_SPI_ADDR_FMT_SINGLE  = 0, /**< Address phase uses single line. */
    WISE_SPI_ADDR_FMT_AS_DATA = 1, /**< Address phase uses same IO width as data phase. */
} WISE_SPI_ADDR_FMT_T;

/**
 * @enum WISE_SLAVE_CMD_T
 * @brief SPI slave command set for specific protocols.
 */
typedef enum {
    READ_STATUS_SINGLE_IO = 0x05, /**< Read status via single I/O. */
    READ_STATUS_DUAL_IO   = 0x15, /**< Read status via dual I/O. */
    READ_STATUS_QUAD_IO   = 0x25, /**< Read status via quad I/O. */
    READ_DATA_SINGLE_IO   = 0x0B, /**< Read data via single I/O. */
    READ_DATA_DUAL_IO     = 0x0C, /**< Read data via dual I/O. */
    READ_DATA_QUAD_IO     = 0x0E, /**< Read data via quad I/O. */
    WRITE_DATA_SINGLE_IO  = 0x51, /**< Write data via single I/O. */
    WRITE_DATA_DUAL_IO    = 0x52, /**< Write data via dual I/O. */
    WRITE_DATA_QUAD_IO    = 0x54, /**< Write data via quad I/O. */
    USER_CMD              = 0x0A, /**< Generic user-defined command. */
} WISE_SLAVE_CMD_T;

/**
 * @enum WISE_SPI_RESET_TYPE
 * @brief Bitmask specifying which parts of the SPI core to reset.
 */
typedef enum {
    WISE_SPI_RESET_CORE = (1 << 0),         /**< Reset SPI core logic. */
    WISE_SPI_RESET_RX   = (1 << 1),         /**< Reset RX path/FIFO. */
    WISE_SPI_RESET_TX   = (1 << 2),         /**< Reset TX path/FIFO. */
    WISE_SPI_RESET_ALL  = WISE_SPI_RESET_CORE | WISE_SPI_RESET_RX | WISE_SPI_RESET_TX,
} WISE_SPI_RESET_TYPE;

/**
 * @enum WISE_SPI_CB_EVENT_T
 * @brief SPI callback events.
 */
typedef enum {
    WISE_SPI_EVENT_SLAVE_COMMAND_INTERRUPT, /**< Slave command interrupt event. */
    WISE_SPI_EVENT_TRANSFER_DONE,           /**< A transfer has completed. */
    WISE_SPI_MAX_EVENTS = SPI_MAX_EVENTS,   /**< Total number of SPI events (alias). */
} WISE_SPI_CB_EVENT_T;

/* ================================================================== */
/*  Structures                                                        */
/* ================================================================== */

/**
 * @struct WISE_SPI_CONF_T
 * @brief Channel-wide SPI configuration, applied once per open.
 *
 * The role (master / slave) is selected by which @c _open function is
 * called (::wise_spi_master_open or ::wise_spi_slave_open), not by a
 * field here.
 *
 * Maps to:
 * - Transfer Format Register (0x10) — @ref clock_mode, @ref bit_order,
 *   @ref io_mode (3-wire bit), @ref data_bit_width, @ref data_merge.
 * - SCLK divider (separate register) — @ref clock_sel.
 * - Software policy — @ref block_mode, @ref dma_enable.
 */
typedef struct {
    WISE_SPI_CLOCK_MODE_T    clock_mode;      /**< CPOL/CPHA. TransFmt bits 1:0. */
    WISE_SPI_BIT_ORDER_T     bit_order;       /**< MSB/LSB first. TransFmt.LSB bit 3. */
    WISE_SPI_IO_MODE_T       io_mode;         /**< Data wiring + lane width (single/3wire/dual/quad). Driver derives TransFmt.MOSIBiDir + TransCtrl.DualQuad. */
    uint8_t                  data_bit_width;  /**< Bits per frame, 1..32. HW field DataLen+1 (bits 12:8). */
    WISE_SPI_ENABLE_T        data_merge;      /**< FIFO byte-pack mode. TransFmt.DataMerge bit 7. Only meaningful when data_bit_width == 8. */
    WISE_SPI_MASTER_CLOCK_SEL_T clock_sel;    /**< SCLK frequency selection (master only). Slave mode ignores this. */
    WISE_SPI_BLOCK_MODE_T    block_mode;      /**< Blocking vs non-blocking API behaviour. */
    WISE_SPI_ENABLE_T        dma_enable;      /**< Enable DMA for TX/RX. */
} WISE_SPI_CONF_T;

/**
 * @struct WISE_SPI_XFER_MSG_T
 * @brief Per-transfer SPI message descriptor.
 *
 * Each ::wise_spi_msg_xfer call programs the SPI Transfer Control
 * Register (0x20) plus the Command and Address registers from this
 * struct.
 *
 * SPI master transaction structure:
 *
 *     CS  ─\__________________________________________________/─
 *     SCLK   ┌─┐┌─┐...     ┌─┐┌─┐...      ┌─┐┌─┐...      ┌─┐┌─┐...
 *           CMD phase     ADDR phase    DUMMY phase    DATA phase
 *
 *           cmd_en        addr_len      dummy_len      trans_mode
 *           cmd           addr_fmt                     tx_data_buff
 *                         address                      tx_unit_count
 *                                                      rx_data_buff
 *                                                      rx_unit_count
 *
 * Each phase is optional; whether it appears on the wire depends on
 * @ref cmd_en, @ref addr_len, @ref dummy_len, and @ref trans_mode.
 *
 * @note The DUMMY phase position is set by @ref trans_mode, not fixed after
 *       ADDR: e.g. {Dummy,Read}/{Dummy,Write} put it first, while
 *       {Write,Dummy,Read}/{Read,Dummy,Write} put it between the two data
 *       sub-phases. The data-lane width (single/dual/quad) is a channel
 *       property set at open via ::WISE_SPI_CONF_T io_mode; the driver
 *       programs TransCtrl.DualQuad from it each transfer, and (when
 *       @ref addr_fmt == ::WISE_SPI_ADDR_FMT_AS_DATA) it also widens the
 *       ADDR phase.
 *
 * Maps to:
 * - Transfer Control Register (0x20) — @ref cmd_en, @ref addr_len
 *   (enable bit), @ref addr_fmt, @ref trans_mode, @ref dummy_len,
 *   @ref tx_unit_count, @ref rx_unit_count, plus DualQuad derived from
 *   the channel's ::WISE_SPI_CONF_T io_mode.
 * - Command Register (0x24) — @ref cmd.
 * - Address Register (0x28) — @ref address (24-bit).
 * - Transfer Format Register (0x10) — driver re-programs the AddrLen
 *   bits from @ref addr_len before each transfer.
 */
typedef struct {
    /* === command phase ============================================ */
    WISE_SPI_ENABLE_T      cmd_en;         /**< Enable command phase. TransCtrl bit 30. */
    uint8_t                cmd;            /**< 8-bit command. Honoured only when cmd_en == WISE_SPI_ENABLE. */

    /* === address phase ============================================ */
    WISE_SPI_ADDR_LEN_T    addr_len;       /**< Address byte width. ::WISE_SPI_ADDR_NONE clears TransCtrl.AddrEn. */
    WISE_SPI_ADDR_FMT_T    addr_fmt;       /**< Address IO format. TransCtrl.AddrFmt bit 28. Honoured only when addr_len != WISE_SPI_ADDR_NONE. */
    uint32_t               address;        /**< Address value, lower 24 bits used. Honoured only when addr_len != WISE_SPI_ADDR_NONE. */

    /* === data phase =============================================== */
    WISE_SPI_TRANS_MODE_T  trans_mode;     /**< Transfer sequence. TransCtrl bits 27:24. (Data-lane width comes from WISE_SPI_CONF_T.io_mode, set at open.) */
    void                  *tx_data_buff;   /**< Pointer to TX buffer. */
    uint16_t               tx_unit_count;  /**< Number of TX units. Range 0..512; 0 means no TX. HW field WrTranCnt+1, 9 bits. */
    void                  *rx_data_buff;   /**< Pointer to RX buffer. */
    uint16_t               rx_unit_count;  /**< Number of RX units. Range 0..512; 0 means no RX. HW field RdTranCnt+1, 9 bits. */

    /* === dummy phase ============================================== */
    uint8_t                dummy_len;      /**< Dummy unit count, range 1..4 (HW field DummyCnt+1, 2 bits). Used only for trans_mode WRITE_DMY_READ / READ_DMY_WRITE / DMY_WRITE / DMY_READ. */
} WISE_SPI_XFER_MSG_T;

/* ==== API functions ====================================================== */

/**
 * @brief Initialize SPI subsystem.
 *
 * Must be called before using other SPI APIs.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_spi_init(void);

/**
 * @brief Deinitialize SPI subsystem.
 *
 * @retval WISE_SUCCESS Deinitialization successful.
 * @retval WISE_FAIL    Deinitialization failed.
 */
WISE_STATUS wise_spi_deinit(void);

/**
 * @brief Open an SPI channel as master.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] spi_conf    Configuration to apply at open.
 *
 * @retval WISE_SUCCESS Channel opened successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_master_open(uint8_t spi_channel, const WISE_SPI_CONF_T *spi_conf);

/**
 * @brief Open an SPI channel as slave.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] spi_conf    Configuration to apply at open.
 *
 * @note Slave mode supports @ref WISE_SPI_IO_SINGLE / @ref WISE_SPI_IO_3WIRE
 *       only. @ref WISE_SPI_IO_DUAL / @ref WISE_SPI_IO_QUAD are rejected
 *       (HW errata: multi-lane slave receive is unreliable) and return
 *       ::WISE_FAIL.
 *
 * @retval WISE_SUCCESS Channel opened successfully.
 * @retval WISE_FAIL    Operation failed (incl. dual/quad requested in slave mode).
 */
WISE_STATUS wise_spi_slave_open(uint8_t spi_channel, const WISE_SPI_CONF_T *spi_conf);

/**
 * @brief Close an SPI channel.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @retval WISE_SUCCESS Channel closed successfully.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_close(uint8_t spi_channel);

/* -- Master simple transfers ------------------------------------------- */

/**
 * @brief Master write (blocking) on SPI.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] data_ptr    Pointer to TX buffer.
 * @param[in] len         Number of bytes to transmit.
 *
 * @retval WISE_SUCCESS Transmission successful.
 * @retval WISE_FAIL    Transmission failed.
 */
WISE_STATUS wise_spi_write(uint8_t spi_channel, void *data_ptr, uint16_t len);

/**
 * @brief Master read (blocking) from SPI.
 *
 * @param[in]  spi_channel SPI channel index.
 * @param[out] data_ptr    Pointer to RX buffer.
 * @param[in]  len         Number of bytes to receive.
 *
 * @retval WISE_SUCCESS Reception successful.
 * @retval WISE_FAIL    Reception failed.
 */
WISE_STATUS wise_spi_read(uint8_t spi_channel, void *data_ptr, uint16_t len);

/**
 * @brief Master write a single byte.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] in_byte     Byte to transmit.
 *
 * @retval WISE_SUCCESS Transmission successful.
 * @retval WISE_FAIL    Transmission failed.
 */
WISE_STATUS wise_spi_write_byte(uint8_t spi_channel, uint8_t in_byte);

/**
 * @brief Master read a single byte.
 *
 * @param[in]  spi_channel SPI channel index.
 * @param[out] out_byte    Pointer to variable receiving the byte.
 *
 * @retval WISE_SUCCESS Reception successful.
 * @retval WISE_FAIL    Reception failed.
 */
WISE_STATUS wise_spi_read_byte(uint8_t spi_channel, uint8_t *out_byte);

/**
 * @brief Execute a formatted SPI transfer.
 *
 * Uses ::WISE_SPI_XFER_MSG_T to describe command, address, dummy
 * cycles and TX/RX buffers.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] msg         Pointer to transfer message.
 *
 * @note Max 512 data bytes per transaction (HW transfer-count / slave RCNT are
 *       9-bit). Split larger payloads into multiple transactions.
 *
 * @retval WISE_SUCCESS Transfer successful.
 * @retval WISE_FAIL    Transfer failed.
 */
WISE_STATUS wise_spi_msg_xfer(uint8_t spi_channel, WISE_SPI_XFER_MSG_T *msg);

/* -- Slave-side helpers ------------------------------------------------ */

/**
 * @brief Indicate that the SPI slave is ready to receive commands.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @retval WISE_SUCCESS Operation successful.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_slave_set_ready(uint8_t spi_channel);

/**
 * @brief Get the number of bytes received by the SPI slave.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @return Number of bytes received in the last transaction.
 *
 * @note The underlying hardware counter (SLVDATACNT.RCNT) is 9-bit, so a single
 *       transaction can carry at most 512 bytes and the count WRAPS to 0 at
 *       exactly 512 (values 0..511 are distinct). After a completed transfer a
 *       returned 0 therefore means 512 bytes were received. Transfers larger
 *       than 512 bytes must be split into multiple transactions.
 */
uint16_t wise_spi_slave_get_recv_count(uint8_t spi_channel);

/**
 * @brief Get the most recent SPI command received in slave mode.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @return Last command byte.
 */
uint8_t wise_spi_slave_get_cmd(uint8_t spi_channel);

/**
 * @brief Prepare TX data into the slave FIFO or internal buffer.
 *
 * Typically used before starting a non-blocking slave transfer.
 *
 * @param[in] spi_channel   SPI channel index.
 * @param[in] tx_fifo_ptr   Pointer to TX buffer.
 * @param[in] tx_unit_count Number of TX units (words/bytes).
 */
void wise_spi_slave_prepare(uint8_t spi_channel, void *tx_fifo_ptr, uint16_t tx_unit_count);

/* -- Events ----------------------------------------------------------- */

/**
 * @brief Register an SPI event callback.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] cb          Callback function.
 * @param[in] context     User context pointer.
 *
 * @retval WISE_SUCCESS Callback registered.
 * @retval WISE_FAIL    Registration failed.
 */
WISE_STATUS wise_spi_register_event_callback(uint8_t spi_channel, SPI_EVT_CALLBACK_T cb, void *context);

/**
 * @brief Unregister an SPI event callback.
 *
 * @param[in] spi_channel SPI channel index.
 *
 * @retval WISE_SUCCESS Callback unregistered.
 * @retval WISE_FAIL    Operation failed.
 */
WISE_STATUS wise_spi_unregister_event_callback(uint8_t spi_channel);

/* -- Reset ------------------------------------------------------------ */

/**
 * @brief Reset parts of the SPI controller.
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] type        Reset type mask, see ::WISE_SPI_RESET_TYPE.
 */
void wise_spi_reset(uint8_t spi_channel, WISE_SPI_RESET_TYPE type);


/** @} */ /* end of WISE_SPI group */

#endif /* __WISE_SPI_API_H */

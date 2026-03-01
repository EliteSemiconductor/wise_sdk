/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_radio_api.h
 * @brief Public API for ESMT ER81xx radio driver.
 *
 * @ingroup WISE_RADIO
 *
 * This header file declares the data types and function prototypes required
 * to configure and operate the ER81xx radio, including modulation, channel
 * plan, packet format, and runtime control (TX/RX, CCA, power level, etc.).
 */

#ifndef __WSIE_RADIO_API_H_
#define __WSIE_RADIO_API_H_

//#include "cmsis/include/a002.h"
#include "hal_intf_radio.h"
#include "types.h"
#include "wise_core.h"

/**
 * @defgroup WISE_Radio_APIs WISE Radio APIs
 * @ingroup WISE_CORE_API
 * @brief Public interface for the WISE radio subsystem.
 *
 * The WISE Radio APIs provide configuration and runtime control of the
 * ER81xx radio, including RF parameters, packet format, event handling,
 * and data plane TX/RX operations.
 * @{
 */


/* ==== Define ============================================================== */
/**
 * @brief Maximum length of the platform name string (in bytes).
 */
#define MAX_PLATFORM_NAME_LEN                   16

/**
 * @name Radio event flags
 * @brief Bit flags reported via ::WISE_RADIO_EVT_CB.
 * @{
 */
#define WISE_RADIO_EVT_RX_FRAME                 BIT0 /**< One or more frames have been received. */
#define WISE_RADIO_EVT_RX_ERR                   BIT1 /**< An error occurred in the RX path. */
#define WISE_RADIO_EVT_RX_TIMEOUT               BIT2 /**< RX operation timed out. */
#define WISE_RADIO_EVT_TX_DONE                  BIT3 /**< A frame has been transmitted successfully. */
#define WISE_RADIO_EVT_TX_ERR                   BIT4 /**< An error occurred in the TX path. */
#define WISE_RADIO_EVT_RX_SYNC_WORD             BIT5 /**< RX path has detected a sync word. */
/** @} */

/**
 * @brief Maximum supported preamble length in bytes.
 */
#define MAX_PREAMBLE_LEN                        4

/**
 * @brief Maximum supported sync word length in bytes.
 */
#define MAX_SYNCWORD_LEN                        8

/**
 * @brief Maximum supported PHR length in bytes.
 */
#define MAX_PHR_LEN                             2

/**
 * @brief Maximum TX power level (device-specific units).
 */
#define MAX_TX_PWR_LEVEL                        127

/**
 * @brief Special RSSI value used to indicate an invalid or unavailable result.
 */
#define INVALID_RSSI                            127

/**
 * @brief Frame format flags (used with WISE_RADIO_PKT_FMT_T::frame_format).
 */
#define FRAME_FMT_NONE                          0
#define FRAME_FMT_PREAMBLE_EN                   BIT0 /**< Enable preamble field. */
#define FRAME_FMT_SYNCWORD_EN                   BIT1 /**< Enable sync word field. */
#define FRAME_FMT_HEADER_EN                     BIT2 /**< Enable header (PHR) field. */
#define FRAME_FMT_CRC_EN                        BIT3 /**< Enable CRC field. */
#define FRAME_FMT_WHITENING_EN                  BIT4 /**< Enable data whitening. */
#define FRAME_FMT_MSB_FIRST                     BIT5 /**< Use MSB-first bit ordering. */
#define FRAME_FMT_FEC_EN                        BIT6 /**< Enable forward error correction (FEC). */

/**
 * @brief Header configuration flags (used with WISE_RADIO_PHR_T::hdr_config).
 */
#define PHR_BYTE_ENDIAN_MSB_FIRST               BIT0 /**< PHR bytes are stored MSB-first. */
/**
 * @note Bit offset and bit-endian configuration must not be enabled at the same time.
 */
#define PHR_LENGTH_BIT_ENDIAN_MSB_FIRST         BIT1 /**< Length field bits are MSB-first. */
#define PHR_LENGTH_INCLUDE_CRC                  BIT2 /**< Length field includes CRC bytes. */

/**
 * @brief Bit offsets used in CRC configuration bitfield.
 */
#define CRC_CFG_OFFSET_INPUT_BIT_ENDIAN         0
#define CRC_CFG_OFFSET_OUTPUT_BIT_ENDIAN        1
#define CRC_CFG_OFFSET_OUTPUT_BYTE_ENDIAN       3
#define CRC_CFG_OFFSET_INCLUDE_HDR              4
#define CRC_CFG_OFFSET_INVERT                   5

/**
 * @enum CRC_CFG_FLAGS_T
 * @brief CRC configuration flags controlling bit/byte endianness and post-processing.
 *
 * These values are composed into a bitfield (e.g. WISE_RADIO_CRC_T::crc_config)
 * using the offsets defined by @ref CRC_CFG_OFFSET_INPUT_BIT_ENDIAN and related
 * macros.
 */
typedef enum
{
    /* Input bit endianness */
    CRC_INPUT_BIT_ENDIAN_LSB_FIRST   = (0 << CRC_CFG_OFFSET_INPUT_BIT_ENDIAN),      /**< CRC Input bits sampled LSB-first. */
    CRC_INPUT_BIT_ENDIAN_MSB_FIRST   = (1 << CRC_CFG_OFFSET_INPUT_BIT_ENDIAN),      /**< CRC Input bits sampled MSB-first. */

    /* Output bit endianness */
    CRC_OUTPUT_BIT_ENDIAN_LSB_FIRST  = (0 << CRC_CFG_OFFSET_OUTPUT_BIT_ENDIAN),     /**< CRC result shifted LSB-first. */
    CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST  = (1 << CRC_CFG_OFFSET_OUTPUT_BIT_ENDIAN),     /**< CRC result shifted MSB-first. */

    /* Output byte endianness */
    CRC_OUTPUT_BYTE_ENDIAN_LSB_FIRST = (0 << CRC_CFG_OFFSET_OUTPUT_BYTE_ENDIAN),    /**< CRC bytes in LSB-first order. */
    CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST = (1 << CRC_CFG_OFFSET_OUTPUT_BYTE_ENDIAN),    /**< CRC bytes in MSB-first order. */

    /* Include header in CRC calculation */
    CRC_INCLUDE_HEADER_OFF           = (0 << CRC_CFG_OFFSET_INCLUDE_HDR),           /**< Header bytes excluded from CRC. */
    CRC_INCLUDE_HEADER_ON            = (1 << CRC_CFG_OFFSET_INCLUDE_HDR),           /**< Header bytes included in CRC. */

    /* Invert final CRC result */
    CRC_INVERT_OFF                   = (0 << CRC_CFG_OFFSET_INVERT),                /**< Do not invert CRC result. */
    CRC_INVERT_ON                    = (1 << CRC_CFG_OFFSET_INVERT)                 /**< Invert CRC result before output. */

} CRC_CFG_FLAGS_T;

/**
 * @enum WISE_MODULATION_T
 * @brief Supported modulation schemes.
 */
typedef enum 
{
    E_MOD_TYPE_BPSK      = 0x0, /**< Binary Phase-Shift Keying (BPSK). */
    E_MOD_TYPE_QPSK      = 0x1, /**< Quadrature Phase-Shift Keying (QPSK). */
    E_MOD_TYPE_OQPSK     = 0x2, /**< Offset QPSK (OQPSK). */
    E_MOD_TYPE_FSK       = 0x3, /**< Frequency-Shift Keying (FSK). */
    E_MOD_TYPE_GFSK      = 0x4, /**< Gaussian FSK (GFSK). */
    E_MOD_TYPE_4FSK      = 0x5, /**< 4-level FSK. */
    E_MOD_TYPE_BPSK_RAMP = 0x6, /**< BPSK with ramp shaping (e.g. Sigfox profile). */
    E_MOD_TYPE_OOK       = 0x7, /**< On-Off Keying (OOK). */
} WISE_MODULATION_T;

/**
 * @enum WISE_PHY_MODE_T
 * @brief High-level PHY operating modes.
 */
typedef enum 
{
    E_PHY_TRANSPARENT       = 0, /**< Transparent (raw) PHY mode. */
    E_PHY_802154            = 1, /**< IEEE 802.15.4-compliant PHY mode. */
    E_PHY_MBUS              = 5, /**< Wireless M-Bus PHY mode. */
} WISE_PHY_MODE_T;

/**
 * @enum WISE_WHITENING_T
 * @brief Data whitening configuration.
 */
typedef enum 
{
    E_WHITENING_DISABLE = 0,    /**< Disable payload whitening. */
    E_WHITENING_PN9,            /**< PN9-based payload whitening. */
} WISE_WHITENING_T;

/**
 * @enum WISE_CCA_T
 * @brief Clear Channel Assessment policy.
 */
typedef enum 
{
    E_WISE_CCA_DISABLE = 0, /**< Disable CCA. */
    E_WISE_CCA_CSMA,        /**< Enable CSMA/CA (Carrier Sense Multiple Access). */
    E_WISE_CCA_LBT          /**< Enable Listen-Before-Talk mode. */
} WISE_CCA_T;

/**
 * @enum WISE_RADIO_PKT_TYPE_T
 * @brief Packet length mode.
 */
typedef enum {
    PKT_FIXED_LENGTH = 0,   /**< Fixed-length packet mode. */
    PKT_VARIABLE_LENGTH,    /**< Variable-length (length field based) mode. */
} WISE_RADIO_PKT_TYPE_T;

/**
 * @enum WISE_CRC_POLY_SEL_T
 * @brief Supported CRC polynomial selections.
 */
typedef enum {
    CRC_POLYNOMIAL_NONE = 0,    /**< CRC disabled. */
    CRC_POLYNOMIAL_CRC8,        /**< Standard CRC-8 polynomial. */
    CRC_POLYNOMIAL_CRC16,       /**< Standard CRC-16 polynomial. */
    CRC_POLYNOMIAL_CCITT_16,    /**< CRC-CCITT (16-bit) polynomial. */
    CRC_POLYNOMIAL_KERMIT,      /**< CRC-KERMIT (16-bit) polynomial. */
    CRC_POLYNOMIAL_DNP16,       /**< DNP 16-bit CRC polynomial. */
    CRC_POLYNOMIAL_BLE24,       /**< BLE 24-bit CRC polynomial. */
    CRC_POLYNOMIAL_MAX          /**< Sentinel, not a valid selection. */
} WISE_CRC_POLY_SEL_T;

/**
 * @enum WISE_DATA_RATE_T
 * @brief Supported data rates.
 *
 * Enumeration values map to device-specific data rate configuration indices.
 */
typedef enum {
    E_DATA_RATE_4P8K = 0,
    E_DATA_RATE_12P5K,
    E_DATA_RATE_32P768K,
    E_DATA_RATE_50K,
    E_DATA_RATE_100K,
    E_DATA_RATE_125K,
    E_DATA_RATE_200K,
    E_DATA_RATE_250K,
    E_DATA_RATE_500K,
    E_DATA_RATE_1M,
    E_DATA_RATE_2M,
    E_DATA_RATE_MAX, /**< Sentinel, not a valid rate. */
} WISE_DATA_RATE_T;

/**
 * @enum WISE_FRAME_CODEC_T
 * @brief Line coding applied to frames.
 */
typedef enum
{
    E_FRAME_CODEC_NONE = 0,     /**< No additional line coding. */
    E_FRAME_CODEC_NRZ,          /**< NRZ (Non-Return-to-Zero) coding. */
    E_FRAME_CODEC_MANCHESTER,   /**< Manchester line coding. */
    E_FRAME_CODEC_3OUTOF6,      /**< 3-out-of-6 line coding. */
} WISE_FRAME_CODEC_T;

/**
 * @enum 
 * @brief Validation result of a received frame.
 */
enum
{
    RX_FRAME_INVALID = 0,       /**< Frame failed validation (for example CRC error). */
    RX_FRAME_VALID = 1,         /**< Frame passed validation. */
};

/**
 * @enum WISE_RADIO_RX_MODE_T
 * @brief RX operation mode.
 */
typedef enum {
    RADIO_RX_CONTINUOUS = 0,    /**< Continuous reception until explicitly stopped. */
    RADIO_RX_ONE_SHOT,          /**< Single burst reception, stops automatically. */
} WISE_RADIO_RX_MODE_T;

/**
 * @enum WISE_TX_TRANSITION_T
 * @brief TX state transition after a transmission finishes.
 */
typedef enum
{
    TX_FINISH_TO_IDLE = 0,      /**< Put radio to idle mode after tx is finished. */
    TX_FINISH_TO_RX = 1,        /**< Automatically switch to RX after TX is finished. */
} WISE_TX_TRANSITION_T;

/**
 * @struct WISE_RADIO_CFG_T
 * @brief Radio RF and PHY configuration.
 *
 * This structure encapsulates the RF configuration (modulation, channel
 * plan, deviation, data rate) and basic framing parameters such as preamble,
 * sync word, and line coding.
 */
typedef struct {
    /* System / modulation */
    WISE_MODULATION_T modulation;   /**< Selected modulation scheme. */

    /* Channel configuration */
    uint32_t ch_freq_min;           /**< Minimum channel center frequency, in Hz. */
    uint32_t ch_freq_max;           /**< Maximum channel center frequency, in Hz. */
    uint32_t ch_spacing;            /**< Channel spacing, in Hz. */

    /* Modem parameters */
    uint32_t deviation;             /**< Frequency deviation, in Hz (for FSK/GFSK). */
    WISE_DATA_RATE_T data_rate;     /**< Symbol/data rate selection. */
    uint32_t syncword;              /**< Sync word value. */
    uint8_t sync_word_len;          /**< Sync word length, in bytes. */

    /* Preamble configuration */
    uint8_t preamble;               /**< Preamble pattern. */
    uint8_t preamble_len;           /**< Preamble length, in bytes. */
} WISE_RADIO_CFG_T;

/**
 * @struct WISE_RADIO_PHR_T
 * @brief Packet header (PHR) configuration.
 *
 * Defines the header byte count and the bit-field location of the length
 * information.
 */
typedef struct
{
    uint8_t hdr_bytes;              /**< Number of header bytes (valid range: 0–2). */
    uint8_t hdr_config;             /**< Header configuration bitmask (see PHR_* flags). */
    uint8_t length_bit_size;        /**< Length field size in bits (valid range: 8–12). */
    uint8_t length_bit_offset;      /**< Bit offset of the length field within the header. */
} WISE_RADIO_PHR_T;

/**
 * @struct WISE_RADIO_CRC_T
 * @brief CRC computation configuration.
 *
 * Specifies polynomial, seed, and endianness options used in CRC calculation.
 */
typedef struct
{
    uint8_t crc_config;             /**< CRC configuration flags (see ::CRC_CFG_FLAGS_T). */
    uint8_t crc_poly_sel;           /**< CRC polynomial selection (see ::WISE_CRC_POLY_SEL_T). */
    uint32_t crc_seed;              /**< CRC initial seed value. */
} WISE_RADIO_CRC_T;

/**
 * @struct WISE_RADIO_PKT_FMT_T
 * @brief Packet format configuration.
 *
 * Controls packet layout, frame format options, PHR settings and CRC behavior.
 */
typedef struct {
    WISE_RADIO_PKT_TYPE_T pkt_type; /**< Packet type (fixed or variable length). */
    uint8_t frame_format;           /**< Frame format mask (see FRAME_FMT_* macros). */
    uint32_t max_pkt_length;        /**< Maximum packet length, in bytes. */
    
    WISE_RADIO_PHR_T phr;           /**< PHR configuration. */
    WISE_RADIO_CRC_T crc;           /**< CRC configuration. */

    // whitening configure
    // TBD
    
    WISE_FRAME_CODEC_T frame_codec; /**< Frame line-coding scheme. */
} WISE_RADIO_PKT_FMT_T;

/**
 * @typedef WISE_RADIO_BUFFER_T
 * @brief Radio buffer type alias.
 *
 * Alias of ::WISE_BUFFER_T used by the radio driver to manage RX/TX buffers.
 */
typedef WISE_BUFFER_T WISE_RADIO_BUFFER_T;

/**
 * @struct WISE_RADIO_CCA_T
 * @brief Clear Channel Assessment (CCA) configuration.
 *
 * Defines thresholds and timing parameters for CSMA/CA or LBT operation.
 */
typedef struct {
    WISE_CCA_T mode;   /**< CCA operating mode. */
    uint8_t retry;     /**< Maximum number of CCA retries. */
    int8_t threshold;  /**< CCA threshold, typically in RSSI units. */
    uint16_t duration; /**< CCA sensing duration, in microseconds. */
    uint16_t backoff;  /**< Backoff time between retries, in microseconds. */
    uint16_t timeout;  /**< Total CCA timeout, in microseconds. */
} WISE_RADIO_CCA_T;

/**
 * @struct WISE_RX_META_T
 * @brief Metadata associated with a received frame.
 *
 * Provides contextual information for a frame delivered by the RX path.
 */
typedef struct {
    uint32_t timestamp;     /**< Reception timestamp (device-specific time base). */
    uint32_t ch_frequency;  /**< Channel frequency used for the reception, in Hz. */
    uint16_t data_len;      /**< Received payload length, in bytes. */
    int8_t rssi;            /**< Measured RSSI at reception. */
    uint8_t valid;          /**< Frame validity flag: @ref RX_FRAME_VALID or @ref RX_FRAME_INVALID. */
    uint8_t ch_index;       /**< Channel index. */
    uint8_t reserved[3];    /**< Reserved for alignment; not used. */
} WISE_RX_META_T;

/**
 * @struct WISE_RADIO_VERSION_INFO_T
 * @brief Radio driver and platform identification.
 */
typedef struct {
    uint8_t platform_name[MAX_PLATFORM_NAME_LEN]; /**< Null-terminated platform name string. */
    uint16_t platform_id;                         /**< Numeric platform identifier. */
    uint8_t ver_major;                            /**< Major version number. */
    uint8_t ver_minor;                            /**< Minor version number. */
    uint8_t ver_build;                            /**< Build or patch version number. */
    uint32_t sha_1;                               /**< Truncated SHA-1 for build identification. */
} WISE_RADIO_VERSION_INFO_T;

/**
 * @typedef WISE_RADIO_EVT_T
 * @brief Radio event bitmask type.
 *
 * Values are constructed from WISE_RADIO_EVT_* macros.
 */
typedef uint32_t WISE_RADIO_EVT_T;

/**
 * @typedef WISE_RADIO_EVT_CB
 * @brief Radio event callback type.
 *
 * @param evt Event bitmask indicating one or more events (see WISE_RADIO_EVT_*).
 */
typedef void (*WISE_RADIO_EVT_CB)(WISE_RADIO_EVT_T evt);


/* ==== FUNCTION ============================================================ */

/**
 * @brief Retrieve radio driver version information.
 *
 * Populates a ::WISE_RADIO_VERSION_INFO_T structure with platform name,
 * platform identifier, version numbers and build hash.
 *
 * @param[out] out_ver_info Pointer to a structure that receives version information.
 *
 * @retval 0    Operation completed successfully.
 * @retval <0   Invalid argument or internal failure.
 */
int8_t wise_radio_get_version(WISE_RADIO_VERSION_INFO_T *out_ver_info);

/**
 * @brief Initialize a radio interface instance.
 *
 * Allocates and initializes resources associated with the specified radio
 * interface. This function must be invoked before using any other API on
 * the same interface.
 *
 * @param[in] intf_idx Radio interface index.
 *
 * @retval 0    Initialization completed successfully.
 * @retval <0   Initialization failed.
 */
int8_t wise_radio_init(int8_t intf_idx);

/**
 * @brief Deinitialize a radio interface instance.
 *
 * Releases resources and returns the interface to an uninitialized state.
 *
 * @param[in] intf_idx Radio interface index.
 */
void wise_radio_deinit(int8_t intf_idx);

/**
 * @brief Register the buffer pool used for radio RX/TX.
 *
 * Associates a pre-allocated buffer pool with the given radio interface.
 * The buffer pool must remain valid for the entire duration of radio usage.
 *
 * @param[in] intf_idx    Radio interface index.
 * @param[in] buffer_pool Pointer to a ::WISE_RADIO_BUFFER_T pool.
 *
 * @retval 0   Configuration successful.
 * @retval <0  Invalid argument or configuration error.
 */
int8_t wise_radio_set_buffer(int8_t intf_idx, WISE_RADIO_BUFFER_T *buffer_pool);

/**
 * @brief Configure RF and packet format parameters.
 *
 * Applies the RF configuration (modulation, channel plan, deviation, etc.) and
 * packet format configuration (PHR, CRC, frame format) to the specified
 * interface.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] cfg      Pointer to ::WISE_RADIO_CFG_T.
 * @param[in] pkt_fmt  Pointer to ::WISE_RADIO_PKT_FMT_T.
 *
 * @retval 0   Configuration successful.
 * @retval <0  Invalid parameter or configuration failure.
 */
int8_t wise_radio_config(int8_t intf_idx, WISE_RADIO_CFG_T *cfg, WISE_RADIO_PKT_FMT_T *pkt_fmt);

/**
 * @brief Configure the channel frequency table.
 *
 * Installs a channel table for the specified interface. Each entry represents
 * a channel center frequency in Hz.
 *
 * @param[in] intf_idx   Radio interface index.
 * @param[in] freq_table Pointer to an array of channel frequencies, in Hz.
 * @param[in] ch_num     Number of entries in @p freq_table.
 *
 * @retval 0   Configuration successful.
 * @retval <0  Invalid parameter.
 */
int8_t wise_radio_set_channel_table(int8_t intf_idx, uint32_t *freq_table, uint8_t ch_num);

/**
 * @brief Register a callback for radio events.
 *
 * The callback is invoked when one or more enabled events (see WISE_RADIO_EVT_*)
 * occur on the specified interface.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] evt_cb   Callback function of type ::WISE_RADIO_EVT_CB.
 *
 * @retval 0   Registration successful.
 * @retval <0  Invalid parameter.
 */
int8_t wise_radio_set_evt_callback(int8_t intf_idx, WISE_RADIO_EVT_CB evt_cb);

/**
 * @brief Enable or disable specific radio events.
 *
 * Controls which events will trigger the registered event callback.
 *
 * @param[in] intf_idx     Radio interface index.
 * @param[in] enable_mask  Bitmask of events to enable (OR of WISE_RADIO_EVT_*).
 *
 * @retval 0   Operation successful.
 * @retval <0  Invalid parameter.
 */
int8_t wise_radio_set_evt_enable(int8_t intf_idx, uint32_t enable_mask);

/**
 * @brief Query the frequency of a channel.
 *
 * Retrieves the configured center frequency for the specified channel index.
 *
 * @param[in]  intf_idx Radio interface index.
 * @param[in]  ch_index Channel index.
 * @param[out] ch_freq  Pointer to a variable that receives the frequency in Hz.
 *
 * @retval 0   Operation successful.
 * @retval <0  Invalid parameter.
 */
int8_t wise_radio_get_channel_freq(int8_t intf_idx, uint8_t ch_index, uint32_t *ch_freq);

/**
 * @brief Query the number of configured channels.
 *
 * Returns the number of channels available for the specified interface.
 *
 * @param[in] intf_idx Radio interface index.
 *
 * @return Number of channels configured for the interface.
 */
uint8_t wise_radio_get_channel_num(int8_t intf_idx);

/**
 * @brief Configure CSMA/CA (or LBT) parameters for transmission.
 *
 * When enabled, transmissions will be governed by the specified CCA policy
 * before a frame is sent.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] enable   Non-zero to enable CCA; zero to disable.
 * @param[in] csma_cfg Pointer to ::WISE_RADIO_CCA_T containing CCA parameters.
 */
void wise_radio_set_tx_csma(int8_t intf_idx, uint8_t enable, WISE_RADIO_CCA_T *csma_cfg);

/**
 * @brief Configure the TX core I/O mode.
 *
 * Selects whether subsequent core-level TX operations are executed in
 * blocking or non-blocking mode, as specified by ::CORE_IO_MODE_T.
 *
 * In blocking mode, the API does not return until the I/O operation has
 * completed or failed. In non-blocking mode, the API returns immediately,
 * and completion is typically reported through events or polling.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] io_mode  Core I/O mode (for example, ::CORE_IO_BLOCKING or ::CORE_IO_NONBLOCKING).
 */
void wise_radio_set_tx_io_mode(int8_t intf_idx, CORE_IO_MODE_T io_mode);

/**
 * @brief Transmit a frame on the specified channel.
 *
 * Queues a frame for transmission over the given channel index.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] ch_index Channel index.
 * @param[in] pframe   Pointer to frame payload.
 * @param[in] length   Frame length, in bytes.
 *
 * @retval 0   Transmission accepted.
 * @retval <0  Transmission rejected or configuration error.
 */
int8_t wise_radio_tx_frame(int8_t intf_idx, uint8_t ch_index, uint8_t *pframe, uint16_t length);

/**
 * @brief Configure the radio state after a TX operation finishes.
 *
 * If the TX state transition is set to ::TX_FINISH_TO_RX, the radio will start
 * listening on the same channel immediately after the transmission completes.
 *
 * @param[in] intf_idx   Radio interface index.
 * @param[in] transition Next radio state after TX is finished.
 *
 * @retval 0   State transition configured.
 * @retval <0  State transition rejected.
 */
int8_t wise_radio_set_tx_state_transition(int8_t intf_idx, WISE_TX_TRANSITION_T transition);

/**
 * @brief Set the transmit power level.
 *
 * Adjusts the TX output power for the specified interface.
 *
 * @param[in] intf_idx  Radio interface index.
 * @param[in] pwr_value Power level value (device-specific range).
 *
 * @retval 0   Operation successful.
 * @retval <0  Invalid parameter.
 */
int8_t wise_radio_set_tx_pwr(int8_t intf_idx, uint8_t pwr_value);

/**
 * @brief Get the current transmit power level.
 *
 * @param[in] intf_idx Radio interface index.
 *
 * @return Current power level in device-specific units.
 */
uint8_t wise_radio_get_tx_pwr(int8_t intf_idx);

/**
 * @brief Set the transmit power level in unit of dbm.
 *
 * Adjusts the TX output power for the specified interface.
 *
 * @param[in] intf_idx  Radio interface index.
 * @param[in] pwr_dbm   Power level value in dbm.
 *
 * @retval 0   Operation successful.
 * @retval <0  Invalid parameter.
 */
int8_t wise_radio_set_tx_pwr_dbm(int8_t intf_idx, int8_t pwr_dbm);

/**
 * @brief Get the current transmit power level in unit of dbm.
 *
 * @param[in] intf_idx Radio interface index.
 *
 * @return Current power level in dbm.
 */
uint8_t wise_radio_get_tx_pwr_dbm(int8_t intf_idx);

/**
 * @brief Gets the raw RSSI value.
 *
 * This function retrieves the raw RSSI value from the radio interface. This
 * function does not require the reception process to be started.
 *
 * @param[in] intfIdx The index of the radio interface.
 * @return The raw RSSI value.
 */
uint16_t wise_radio_get_raw_rssi(int8_t intf_idx);

/**
 * @brief Enable or disable single-tone (unmodulated carrier) mode.
 *
 * Configures the radio to output a continuous unmodulated carrier at the
 * current RF configuration. This mode is typically used for RF production
 * testing and calibration.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] ch_index Radio channel index.
 * @param[in] pwr_lv   Power level for the tone.
 * @param[in] enable   Non-zero to enable; zero to disable.
 */
void wise_radio_enable_singletone(int8_t intf_idx, uint8_t ch_index, uint8_t pwr_lv, uint8_t enable);

/**
 * @brief Set RX operation mode.
 *
 * Configures the RX behavior (continuous or one-shot) for the specified
 * interface. The selected mode will be used by subsequent RX operations.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] rx_mode  RX mode selection (continuous or one-shot).
 *
 * @retval 0   RX mode configured successfully.
 * @retval <0  RX mode cannot be set.
 */
int8_t wise_radio_set_rx_mode(int8_t intf_idx, WISE_RADIO_RX_MODE_T rx_mode);

/**
 * @brief Start reception on a given channel.
 *
 * Places the radio into RX state on the specified channel. The RX behavior
 * (continuous or one-shot) is determined by the last call to
 * ::wise_radio_set_rx_mode.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] ch_index Channel index.
 *
 * @retval 0   RX started successfully.
 * @retval <0  RX could not be started.
 */
int8_t wise_radio_rx_start(int8_t intf_idx, uint8_t ch_index);

/**
 * @brief Gets the Received Signal Strength Indicator (RSSI).
 *
 * This function retrieves the RSSI value, which indicates the strength of the
 * received signal. Note that this function should be called after starting
 * the reception process, i.e. wise_radio_rx_start.
 *
 * @param[in] intfIdx The index of the radio interface.
 * @return The RSSI value.
 */
int8_t wise_radio_get_rssi(int8_t intf_idx);

/**
 * @brief Stop reception.
 *
 * Forces the radio out of RX state for the specified interface.
 *
 * @param[in] intf_idx Radio interface index.
 */
void wise_radio_rx_stop(int8_t intf_idx);

/**
 * @brief Query the number of received frames pending in the RX queue.
 *
 * @param[in] intf_idx Radio interface index.
 *
 * @return Number of frames available to be read.
 */
uint8_t wise_radio_get_rx_frame_num(int8_t intf_idx);

/**
 * @brief Read a received frame into a user buffer.
 *
 * Copies the next available frame from the RX queue into the provided buffer,
 * and returns its length and metadata. After reading the frame data by this API
 * call, the internal frame buffer will be released.
 *
 * @param[in]  intf_idx Radio interface index.
 * @param[out] pframe   Pointer to the user buffer that receives frame data.
 * @param[out] out_len  Pointer to a variable that receives the frame length, in bytes.
 * @param[out] meta     Pointer to ::WISE_RX_META_T receiving frame metadata.
 *
 * @retval 0   Frame read successfully.
 * @retval <0  No frame available or read error.
 */
int8_t wise_radio_read_rx_frame(int8_t intf_idx, uint8_t *pframe, uint16_t *out_len, WISE_RX_META_T *meta);

/**
 * @brief Retrieve the address and metadata of a received frame without copying.
 *
 * Provides the internal buffer address and metadata of the next frame in the
 * RX queue, without copying payload bytes. The returned address remains
 * valid until the frame is released.
 *
 * @param[in]  intf_idx       Radio interface index.
 * @param[out] out_frame_addr Pointer to a variable that receives the internal frame address.
 * @param[out] out_meta       Pointer to ::WISE_RX_META_T receiving frame metadata.
 *
 * @retval 0   Information retrieved successfully.
 * @retval <0  No frame available or invalid parameter.
 */
int8_t wise_radio_get_rx_frame_info(int8_t intf_idx, uint32_t *out_frame_addr, WISE_RX_META_T *out_meta);

/**
 * @brief Set the maximum allowed RX frame length.
 *
 * Frames exceeding this length may be discarded or marked invalid,
 * depending on the driver implementation.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] max_len  Maximum accepted frame length, in bytes.
 *
 * @retval 0   Configuration successful.
 * @retval <0  Invalid parameter.
 */
int8_t wise_radio_set_rx_max_frame_length(int8_t intf_idx, uint16_t max_len);

/**
 * @brief Release a previously consumed RX frame.
 *
 * Notifies the radio driver that the current RX frame can be freed and its
 * buffer returned to the pool.
 *
 * @param[in] intf_idx Radio interface index.
 *
 * @retval 0   Frame released successfully.
 * @retval <0  No frame to release or internal error.
 */
int8_t wise_radio_release_rx_frame(int8_t intf_idx);

void wise_radio_enable_prbs9(int8_t intf_idx, uint8_t enable);

/** @} */ /* end of WISE_Radio_APIs group */

#endif /* __WSIE_RADIO_API_H_ */

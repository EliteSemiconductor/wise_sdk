/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_flash_api.h
 * @brief Flash and OTP memory access APIs.
 *
 * @ingroup WISE_FLASH
 *
 * This header provides the interface for managing and interacting with
 * on-chip flash and OTP memory, including initialization, information
 * query, read/write, erase operations, and master boot information access.
 */

#ifndef __WISE_FLASH_API_H
#define __WISE_FLASH_API_H

#include "wise_core.h"
#include "esmt_chip_specific.h"
#include "hal_intf_flash.h"
#include "types.h"

/**
 * @defgroup WISE_FLASH WISE Flash & SHadow Area APIs
 * @ingroup WISE_CORE_API
 * @brief Flash/OTP memory information, read/write and erase operations.
 * @{
 */

/**
 * @brief Base mapping address of the flash memory.
 */
#define WISE_FLASH_MAP_ADDR                 CHIP_FLASH_MAP_ADDR


/**
 * @struct WISE_FLASH_INFO_T
 * @brief Flash memory information.
 *
 * Contains total flash size and erase granularity parameters.
 */
typedef struct {
    uint32_t flashSize;     /**< Total flash size in bytes. */
    uint32_t blockSize;     /**< Size of a single erase block in bytes. */
    uint32_t minEraseSize;  /**< Minimum erase size in bytes. */
} WISE_FLASH_INFO_T;

/**
 * @brief Initialize the flash memory interface.
 *
 * Performs required setup for the flash subsystem and prepares it for read
 * and write operations. Must be called before any other flash API.
 */
void wise_flash_init(void);

/**
 * @brief Retrieve information about flash memory.
 *
 * Fills a ::WISE_FLASH_INFO_T structure with properties of the flash,
 * such as total size and block size.
 *
 * @param[out] outInfo Pointer to a ::WISE_FLASH_INFO_T structure that
 *                     receives the flash information.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed (for example, invalid parameter or HW error).
 */
int32_t wise_flash_get_info(WISE_FLASH_INFO_T *outInfo);

/**
 * @brief Erase a flash sector at the given offset.
 *
 * The erase operation is performed on a sector-aligned region starting
 * at @p flashOffset.
 *
 * @param[in] flashOffset Offset in flash (from @ref WISE_FLASH_MAP_ADDR) where
 *                        the sector to erase begins.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_sector_erase(uint32_t flashOffset);

/**
 * @brief Erase a flash block at the given offset.
 *
 * The erase operation is performed on a block-aligned region starting
 * at @p flashOffset.
 *
 * @param[in] flashOffset Offset in flash (from @ref WISE_FLASH_MAP_ADDR) where
 *                        the block to erase begins.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_block_erase(uint32_t flashOffset);

/**
 * @brief Write data to flash memory.
 *
 * Writes @p u32ByteLen bytes from @p pu8Buf into flash starting at
 * the specified offset. The caller is responsible for ensuring that the
 * target region has been erased when required by the underlying flash
 * technology.
 *
 * @param[in] flashOffset Offset in flash (from @ref WISE_FLASH_MAP_ADDR)
 *                        where the data should be written.
 * @param[in] pu8Buf      Pointer to buffer containing data to write.
 * @param[in] u32ByteLen  Number of bytes to write.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_write(uint32_t flashOffset, uint8_t *pu8Buf, uint32_t u32ByteLen);

/**
 * @brief Read data from flash memory.
 *
 * Reads @p u32ByteLen bytes from flash starting at @p flashOffset into
 * the buffer pointed to by @p pu8Buf.
 *
 * @param[in]  flashOffset Offset in flash (from @ref WISE_FLASH_MAP_ADDR)
 *                         where the read starts.
 * @param[out] pu8Buf      Pointer to buffer that receives the data.
 * @param[in]  u32ByteLen  Number of bytes to read.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_read(uint32_t flashOffset, uint8_t *pu8Buf, uint32_t u32ByteLen);

/**
 * @brief Read the unique ID (UID) from flash or device.
 *
 * @param[out] uid   Pointer to buffer that receives UID bytes.
 * @param[in,out] ioLen On input, specifies the size of @p uid buffer.
 *                      On output, receives the actual UID length.
 *
 * @retval 0   UID read successfully.
 * @retval <0  Operation failed or buffer too small.
 */
int32_t wise_flash_get_uid(uint8_t *uid, uint8_t* ioLen);

/**
 * @struct FLASH_SHAD_RGN_INFO_T
 * @brief Flash shadow region memory information.
 *
 * Describes the erase granularity and number of sectors in the flash shadow region.
 */
typedef struct {
    uint32_t sectorSize;    /**< Size of a single sector of shadow region in bytes. */
    uint32_t sectorNum;     /**< Number of sectors in the shadown region memory. */
} FLASH_SHAD_RGN_INFO_T;

/**
 * @brief Retrieve information about the OTP memory.
 *
 * Fills a ::FLASH_SHAD_RGN_INFO_T structure with OTP sector size and total
 * number of sectors.
 *
 * @param[out] outInfo Pointer to a ::FLASH_SHAD_RGN_INFO_T structure that
 *                     receives the OTP information.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_shadow_get_info(FLASH_SHAD_RGN_INFO_T *outInfo);

/**
 * @brief Erase a sector of OTP memory.
 *
 * Performs an erase operation on the specified OTP sector.
 *
 * @param[in] sector Index of the sector in OTP memory to erase.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_shadow_sector_erase(int32_t sector);

/**
 * @brief Write data to OTP memory.
 *
 * Writes @p u32ByteLen bytes from @p pu8Buf into OTP at the given
 * @p sector and @p otpOffset.
 *
 * @param[in] sector     Target sector index in OTP memory.
 * @param[in] otpOffset  Offset within the sector where data is written.
 * @param[in] pu8Buf     Pointer to buffer containing data to write.
 * @param[in] u32ByteLen Number of bytes to write.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_shadow_write(int32_t sector, uint32_t otpOffset, uint8_t *pu8Buf, uint32_t u32ByteLen);

/**
 * @brief Read data from OTP memory.
 *
 * Reads @p u32ByteLen bytes from OTP at the given @p sector and
 * @p otpOffset into @p pu8Buf.
 *
 * @param[in]  sector     Target sector index in OTP memory.
 * @param[in]  otpOffset  Offset within the sector to read from.
 * @param[out] pu8Buf     Pointer to buffer receiving the data.
 * @param[in]  u32ByteLen Number of bytes to read.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_shadow_read(int32_t sector, uint32_t otpOffset, uint8_t *pu8Buf, uint32_t u32ByteLen);

/**
 * @enum CONSOLE_BAUDRATE_T
 * @brief UART console baudrate selection.
 */
typedef enum {
	UART_BAUD_115200, /**< 115200 bps. */
    UART_BAUD_230400, /**< 230400 bps. */
    UART_BAUD_460800, /**< 460800 bps. */
} CONSOLE_BAUDRATE_T;

/**
 * @enum XIP_SPI_MODE_T
 * @brief SPI mode for XIP (Execute-In-Place).
 */
typedef enum {
    XIP_SPI_SINGLE, /**< Single-bit SPI mode. */
    XIP_SPI_DUAL,   /**< Dual I/O SPI mode. */
    XIP_SPI_QUAD,   /**< Quad I/O SPI mode. */
} XIP_SPI_MODE_T;

/**
 * @enum XIP_SPI_SPEED_T
 * @brief SPI speed selection for XIP.
 */
typedef enum {
    XIP_SPI_40M, /**< 40 MHz SPI clock. */
    XIP_SPI_20M, /**< 20 MHz SPI clock. */
} XIP_SPI_SPEED_T;

/**
 * @enum XIP_CLOCK_T
 * @brief XIP clock source selection.
 */
typedef enum {
    XIP_CLK_1T,      /**< 1T clock. */
    XIP_CLK_HALF_T,  /**< 1/2T clock. */
} XIP_CLOCK_T;

/**
 * @enum XIP_CACHE_T
 * @brief XIP cache configuration.
 */
typedef enum {
    XIP_CACHE_2W, /**< 2-way cache. */
    XIP_CACHE_1W, /**< 1-way cache. */
} XIP_CACHE_T;

/**
 * @struct WISE_MSBI_INFO_T
 * @brief Master Boot Information stored in OTP.
 *
 * Contains console configuration and XIP-related boot parameters.
 */
typedef struct {
    uint32_t consoleEnable;             /**< Non-zero to enable console, zero to disable. */
    CONSOLE_BAUDRATE_T consoleBaudrate; /**< UART console baudrate setting. */
    uint32_t bootCrcEnable;             /**< Non-zero to enable boot CRC check. */
    XIP_SPI_MODE_T xipSpiMode;          /**< XIP SPI mode. */
    XIP_SPI_SPEED_T xipSpiSpeed;        /**< XIP SPI speed. */
    XIP_CLOCK_T xipClock;               /**< XIP clock source. */
    XIP_CACHE_T xipCache;               /**< XIP cache configuration. */
} WISE_MSBI_INFO_T;

/**
 * @brief Read Master Boot Information from OTP.
 *
 * Retrieves master boot configuration stored in OTP into @p outInfo.
 *
 * @param[out] outInfo Pointer to a ::WISE_MSBI_INFO_T structure that
 *                     receives the data.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_shadow_read_msbi_info(WISE_MSBI_INFO_T *outInfo);

/**
 * @brief Write Master Boot Information to OTP.
 *
 * Writes the content of @p inInfo into OTP master boot region.
 *
 * @param[in] inInfo Pointer to a ::WISE_MSBI_INFO_T structure containing
 *                   data to be written.
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_flash_shadow_write_msbi_info(WISE_MSBI_INFO_T *inInfo);

/** @} */ /* end of WISE_FLASH group */

#endif /* __WISE_FLASH_API_H */

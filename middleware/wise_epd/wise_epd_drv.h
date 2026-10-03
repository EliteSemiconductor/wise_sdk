/*
 * Copyright (C) 2026 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_epd_drv.h
 * @brief EPD panel driver interface and bus helpers.
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 *
 * Only needed to write a new panel driver. Applications use wise_epd_api.h.
 *
 * Adding a panel:
 * 1. Copy drivers/wise_epd_drv_jd_bwry_122x250.c and adapt the command set.
 * 2. Define a <tt>const WISE_EPD_DRV_T</tt> with the panel geometry, power-up /
 *    reset timing (::WISE_EPD_TIMING_T) and ops.
 * 3. Declare it in wise_epd_drivers.h.
 *
 * The middleware owns the power, SPI and reset sequence; a driver only issues
 * panel commands through the wise_epd_bus_* helpers below. Each call of an op
 * happens with the panel powered, reset and idle.
 */

#ifndef __WISE_EPD_DRV_H
#define __WISE_EPD_DRV_H

#include <stdint.h>

#include "wise_epd_api.h"

/**
 * @struct WISE_EPD_FRAME_T
 * @brief Frame data handed to ::WISE_EPD_DRV_T write_frame.
 *
 * Already checked against the panel geometry and format.
 */
typedef struct {
    const uint8_t *data;      /**< First byte of the top row, in ::WISE_EPD_COLOR_T encoding. */
    uint16_t       stride;    /**< Bytes between rows. 0 = every row is the same (fill). */
    uint16_t       row_bytes; /**< Bytes to send per row. */
} WISE_EPD_FRAME_T;

/**
 * @struct WISE_EPD_TIMING_T
 * @brief Power-up and reset delays of a panel, applied by ::wise_epd_bus_open.
 *
 * Take the values from the panel datasheet / vendor reference code.
 */
typedef struct {
    uint16_t power_on_ms;   /**< After switching panel power on, before claiming the bus. */
    uint16_t io_ready_ms;   /**< After SPI and control pins are configured, before reset. */
    uint16_t reset_low_ms;  /**< RST held low (reset pulse width). */
    uint16_t reset_high_ms; /**< After RST released, before BUSY is polled. */
} WISE_EPD_TIMING_T;

/**
 * @struct WISE_EPD_DRV_S
 * @brief Panel driver descriptor. Instances are @c const and live in flash.
 */
struct WISE_EPD_DRV_S {
    const char          *name;               /**< Short name for logs. */
    uint16_t             width;              /**< Panel width in pixels (source lines). */
    uint16_t             height;             /**< Panel height in pixels (gate lines). */
    WISE_EPD_PIXEL_FMT_T format;             /**< Native pixel format. */
    GPIO_STATUS          busy_level;         /**< BUSY pin level while the panel is busy. */
    WISE_EPD_TIMING_T    timing;             /**< Power-up and reset delays. */
    uint32_t             reset_timeout_ms;   /**< Max wait for idle after hardware reset. */
    uint32_t             refresh_timeout_ms; /**< Max wait for a full refresh to finish. */

    /**
     * @brief Send the panel initialization sequence. May be NULL.
     * @return ::WISE_SUCCESS or a WISE_EPD_ERR_* code.
     */
    WISE_STATUS (*init)(void);

    /**
     * @brief Send one frame into panel RAM.
     * @param[in] frame Frame to send.
     * @return ::WISE_SUCCESS or a WISE_EPD_ERR_* code.
     */
    WISE_STATUS (*write_frame)(const WISE_EPD_FRAME_T *frame);

    /**
     * @brief Turn on panel power and refresh from panel RAM; return when done.
     * @return ::WISE_SUCCESS or a WISE_EPD_ERR_* code.
     */
    WISE_STATUS (*refresh)(void);

    /**
     * @brief Turn off panel power and enter deep sleep. May be NULL.
     * @return ::WISE_SUCCESS or a WISE_EPD_ERR_* code.
     */
    WISE_STATUS (*sleep)(void);
};

/**
 * @brief Send a command byte (D/C low).
 *
 * @param[in] cmd Command byte.
 *
 * @retval WISE_SUCCESS     Sent.
 * @retval WISE_EPD_ERR_BUS SPI transfer failed.
 */
WISE_STATUS wise_epd_bus_write_cmd(uint8_t cmd);

/**
 * @brief Send a command byte followed by its parameter bytes.
 *
 * @param[in] cmd  Command byte.
 * @param[in] data Parameter bytes; may be NULL if @p len is 0.
 * @param[in] len  Number of parameter bytes.
 *
 * @retval WISE_SUCCESS     Sent.
 * @retval WISE_EPD_ERR_BUS SPI transfer failed.
 */
WISE_STATUS wise_epd_bus_write_cmd_data(uint8_t cmd, const uint8_t *data, uint32_t len);

/**
 * @brief Send a command byte, then read its response bytes from the panel.
 *
 * Reads use the panel's bidirectional SDA line (SPI 3-wire mode), one byte per
 * ::wise_spi_read_byte. CS stays asserted from the command to the last byte.
 *
 * @param[in]  cmd  Command byte.
 * @param[out] data Buffer for the response; may be NULL if @p len is 0.
 * @param[in]  len  Number of bytes to read.
 *
 * @retval WISE_SUCCESS     Command sent and @p len bytes read.
 * @retval WISE_EPD_ERR_BUS SPI transfer failed.
 */
WISE_STATUS wise_epd_bus_read_cmd_data(uint8_t cmd, uint8_t *data, uint32_t len);

/**
 * @brief Start a data burst: assert CS with D/C high.
 *
 * Follow with any number of ::wise_epd_bus_data_write, then
 * ::wise_epd_bus_data_end. CS stays asserted for the whole burst.
 */
void wise_epd_bus_data_begin(void);

/**
 * @brief Send data bytes inside a burst, one ::wise_spi_write_byte per byte.
 *
 * @param[in] data Bytes to send.
 * @param[in] len  Number of bytes.
 *
 * @retval WISE_SUCCESS     Sent.
 * @retval WISE_EPD_ERR_BUS SPI transfer failed.
 */
WISE_STATUS wise_epd_bus_data_write(const uint8_t *data, uint32_t len);

/**
 * @brief End a data burst: release CS.
 */
void wise_epd_bus_data_end(void);

/**
 * @brief Wait until the panel leaves the busy state.
 *
 * @param[in] timeout_ms Maximum wait in milliseconds.
 *
 * @retval WISE_SUCCESS         Panel idle.
 * @retval WISE_EPD_ERR_TIMEOUT Still busy after @p timeout_ms.
 */
WISE_STATUS wise_epd_bus_wait_idle(uint32_t timeout_ms);

/**
 * @brief Wait until the panel enters the busy state (e.g. a refresh started).
 *
 * @param[in] timeout_ms Maximum wait in milliseconds.
 *
 * @retval WISE_SUCCESS         Panel busy.
 * @retval WISE_EPD_ERR_TIMEOUT Still idle after @p timeout_ms.
 */
WISE_STATUS wise_epd_bus_wait_busy(uint32_t timeout_ms);

/**
 * @brief Blocking delay.
 *
 * @param[in] ms Delay in milliseconds.
 */
void wise_epd_bus_delay_ms(uint32_t ms);

/* ---- Used by wise_epd_api.c only ----------------------------------------- */

/**
 * @brief Store wiring and park the pins with the panel powered off.
 *
 * @param[in] cfg   Board wiring.
 * @param[in] busy_level BUSY level while busy, from the driver.
 */
void wise_epd_bus_setup(const WISE_EPD_CFG_T *cfg, GPIO_STATUS busy_level);

/**
 * @brief Power the panel, claim SPI and pins, and pulse reset.
 *
 * @param[in] timing Panel delays for each step, from the driver descriptor.
 *
 * @retval WISE_SUCCESS     Panel powered and reset (caller waits for idle).
 * @retval WISE_EPD_ERR_BUS SPI open failed.
 */
WISE_STATUS wise_epd_bus_open(const WISE_EPD_TIMING_T *timing);

/**
 * @brief Release SPI, park the pins and power the panel off.
 */
void wise_epd_bus_close(void);

#endif

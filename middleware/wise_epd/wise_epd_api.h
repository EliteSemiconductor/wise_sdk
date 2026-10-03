/*
 * Copyright (C) 2026 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_epd_api.h
 * @brief E-paper display (EPD) middleware APIs.
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 *
 * The caller draws a complete frame into its own buffer, describes it with
 * ::WISE_EPD_IMAGE_T and hands it to ::wise_epd_display. The panel-specific
 * work (init sequence, data commands, refresh, sleep) is done by the panel
 * driver selected at ::wise_epd_init, so application code does not change when
 * the board is fitted with a different panel.
 *
 * Typical use:
 * @code
 * #include "wise_epd_api.h"
 * #include "wise_epd_drivers.h"
 *
 * static const WISE_EPD_CFG_T epdCfg = {
 *     .spi_channel = 0, .spi_clock = E_SPI_CLOCK_SEL_4M,
 *     .sclk_pin = 4, .mosi_pin = 5, .cs_pin = 6, .dc_pin = 7,
 *     .rst_pin = 8, .busy_pin = 9,
 *     .pwr_pin = 10, .pwr_active_level = GPIO_LOW,
 * };
 *
 * wise_epd_init(&wise_epd_drv_jd_bwry_122x250, &epdCfg);
 *
 * WISE_EPD_IMAGE_T img = {
 *     .width = 122, .height = 250, .format = WISE_EPD_FMT_2BPP_BWRY,
 *     .stride = 0, .data = frameBuf,
 * };
 * wise_epd_display(&img);
 * @endcode
 */

#ifndef __WISE_EPD_API_H
#define __WISE_EPD_API_H

#include <stdint.h>

#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_spi_api.h"

/**
 * @name EPD status codes
 * Returned by the EPD APIs in addition to ::WISE_SUCCESS / ::WISE_FAIL.
 * @{
 */
#define WISE_EPD_ERR_PARAM      (-2) /**< Invalid argument. */
#define WISE_EPD_ERR_NOT_INIT   (-3) /**< ::wise_epd_init has not been called. */
#define WISE_EPD_ERR_FORMAT     (-4) /**< Image size or pixel format does not match the panel. */
#define WISE_EPD_ERR_BUS        (-5) /**< SPI open or transfer failed. */
#define WISE_EPD_ERR_TIMEOUT    (-6) /**< Panel BUSY pin did not reach the expected level in time. */
#define WISE_EPD_ERR_UNSUPPORT  (-7) /**< The selected driver does not implement this operation. */
/** @} */

/** Pin index value meaning "not connected" (e.g. panel power is not switched). */
#define WISE_EPD_PIN_NONE       (0xFF)

/**
 * @enum WISE_EPD_PIXEL_FMT_T
 * @brief Pixel format of an image buffer.
 *
 * Pixels are packed row by row, left to right, starting at the most
 * significant bit of each byte. Each row starts on a byte boundary; unused
 * bits at the end of a row are ignored.
 *
 * Pixel values use ::WISE_EPD_COLOR_T, whatever the panel's own encoding is;
 * the driver translates when the panel differs.
 */
typedef enum {
    WISE_EPD_FMT_1BPP_BW   = 0, /**< 1 bit per pixel: 0 = black, 1 = white. */
    WISE_EPD_FMT_2BPP_BWRY = 1, /**< 2 bits per pixel: black / white / yellow / red (see ::WISE_EPD_COLOR_T). */
    WISE_EPD_FMT_2BPP_BWR  = 2, /**< 2 bits per pixel, same values as BWRY, for black / white / red panels.
                                     Yellow pixels are shown as red. */
} WISE_EPD_PIXEL_FMT_T;

/**
 * @enum WISE_EPD_COLOR_T
 * @brief Pixel values used in image buffers and by ::wise_epd_clear.
 *
 * ::WISE_EPD_FMT_1BPP_BW uses only BLACK and WHITE; ::WISE_EPD_FMT_2BPP_BWR
 * has no YELLOW.
 */
typedef enum {
    WISE_EPD_COLOR_BLACK  = 0, /**< Black. */
    WISE_EPD_COLOR_WHITE  = 1, /**< White. */
    WISE_EPD_COLOR_YELLOW = 2, /**< Yellow (::WISE_EPD_FMT_2BPP_BWRY only). */
    WISE_EPD_COLOR_RED    = 3, /**< Red (2 bpp formats only). */
} WISE_EPD_COLOR_T;

/** Bits per pixel of a ::WISE_EPD_PIXEL_FMT_T. */
#define WISE_EPD_FMT_BPP(fmt)            (((fmt) == WISE_EPD_FMT_1BPP_BW) ? 1u : 2u)

/** Minimum bytes per row for an image @p w pixels wide in format @p fmt. */
#define WISE_EPD_ROW_BYTES(w, fmt)       ((((uint32_t)(w) * WISE_EPD_FMT_BPP(fmt)) + 7u) / 8u)

/**
 * @struct WISE_EPD_IMAGE_T
 * @brief Describes a caller-owned, fully drawn frame.
 *
 * The buffer is only read, and only during ::wise_epd_display, so it may live
 * in flash (a @c const array) or RAM.
 */
typedef struct {
    uint16_t             width;  /**< Image width in pixels; must equal the panel width. */
    uint16_t             height; /**< Image height in pixels; must equal the panel height. */
    WISE_EPD_PIXEL_FMT_T format; /**< Pixel format; must equal the panel's native format. */
    uint16_t             stride; /**< Bytes from the start of one row to the next. 0 = ::WISE_EPD_ROW_BYTES(width, format). */
    const uint8_t       *data;   /**< First byte of the top row. */
} WISE_EPD_IMAGE_T;

/**
 * @struct WISE_EPD_CFG_T
 * @brief Board wiring of the panel.
 *
 * SCLK and MOSI are routed to the SPI controller; CS, D/C, RST and PWR are
 * driven as GPIO outputs and BUSY is read as a GPIO input.
 */
typedef struct {
    uint8_t                     spi_channel;      /**< SPI channel (0 or 1). */
    WISE_SPI_MASTER_CLOCK_SEL_T spi_clock;        /**< SCLK frequency; check the panel's write-cycle limit. */
    uint8_t                     sclk_pin;         /**< SPI clock pin. */
    uint8_t                     mosi_pin;         /**< SPI data-out pin (panel SDA/DIN). */
    uint8_t                     cs_pin;           /**< Chip select pin, active low. */
    uint8_t                     dc_pin;           /**< Data/command select pin: low = command, high = data. */
    uint8_t                     rst_pin;          /**< Reset pin, active low. */
    uint8_t                     busy_pin;         /**< Busy status pin from the panel. */
    uint8_t                     pwr_pin;          /**< Panel power switch pin, or ::WISE_EPD_PIN_NONE if always powered. */
    GPIO_STATUS                 pwr_active_level; /**< Level on @ref pwr_pin that turns the panel on. */
} WISE_EPD_CFG_T;

/**
 * @struct WISE_EPD_INFO_T
 * @brief Properties of the selected panel, for sizing the caller's buffer.
 */
typedef struct {
    const char          *name;      /**< Driver name. */
    uint16_t             width;     /**< Panel width in pixels. */
    uint16_t             height;    /**< Panel height in pixels. */
    WISE_EPD_PIXEL_FMT_T format;    /**< Native pixel format expected by ::wise_epd_display. */
    uint16_t             row_bytes; /**< Minimum bytes per row (::WISE_EPD_ROW_BYTES). */
} WISE_EPD_INFO_T;

/* Driver descriptor, defined in wise_epd_drv.h. */
struct WISE_EPD_DRV_S;
/** Panel driver handle; pick one from wise_epd_drivers.h. */
typedef struct WISE_EPD_DRV_S WISE_EPD_DRV_T;

/**
 * @brief Select the panel driver and board wiring.
 *
 * Configures the power pin so the panel stays off; the SPI channel and the
 * remaining pins are only claimed while ::wise_epd_display or ::wise_epd_clear
 * runs. Can be called again to switch driver or wiring.
 *
 * @param[in] drv Panel driver, e.g. one declared in wise_epd_drivers.h.
 * @param[in] cfg Board wiring. Copied; need not stay valid after the call.
 *
 * @retval WISE_SUCCESS        Ready to display.
 * @retval WISE_EPD_ERR_PARAM  @p drv or @p cfg is NULL or invalid.
 */
WISE_STATUS wise_epd_init(const WISE_EPD_DRV_T *drv, const WISE_EPD_CFG_T *cfg);

/**
 * @brief Release the module.
 *
 * Leaves the panel powered off and the pins in their low-power state.
 */
void wise_epd_deinit(void);

/**
 * @brief Get the properties of the selected panel.
 *
 * @param[out] info Panel properties.
 *
 * @retval WISE_SUCCESS          @p info filled.
 * @retval WISE_EPD_ERR_PARAM    @p info is NULL.
 * @retval WISE_EPD_ERR_NOT_INIT ::wise_epd_init has not been called.
 */
WISE_STATUS wise_epd_get_info(WISE_EPD_INFO_T *info);

/**
 * @brief Show a full frame on the panel.
 *
 * Powers the panel up, resets and initializes it, sends the image, runs a
 * full refresh, then puts the panel into deep sleep and powers it off.
 * Blocks until the refresh completes (several seconds on colour panels).
 *
 * @param[in] img Image to show. Size and format must match the panel.
 *
 * @retval WISE_SUCCESS          Frame shown.
 * @retval WISE_EPD_ERR_PARAM    @p img or its data is NULL, or stride too small.
 * @retval WISE_EPD_ERR_NOT_INIT ::wise_epd_init has not been called.
 * @retval WISE_EPD_ERR_FORMAT   Size or pixel format differs from the panel.
 * @retval WISE_EPD_ERR_BUS      SPI open or transfer failed.
 * @retval WISE_EPD_ERR_TIMEOUT  Panel stayed busy too long.
 */
WISE_STATUS wise_epd_display(const WISE_EPD_IMAGE_T *img);

/**
 * @brief Fill the whole panel with one colour.
 *
 * Same power sequence as ::wise_epd_display, without needing a frame buffer.
 *
 * @param[in] color Fill colour; must be valid for the panel's format.
 *
 * @retval WISE_SUCCESS          Panel filled.
 * @retval WISE_EPD_ERR_PARAM    @p color not supported by the panel's format.
 * @retval WISE_EPD_ERR_NOT_INIT ::wise_epd_init has not been called.
 * @retval WISE_EPD_ERR_BUS      SPI open or transfer failed.
 * @retval WISE_EPD_ERR_TIMEOUT  Panel stayed busy too long.
 */
WISE_STATUS wise_epd_clear(WISE_EPD_COLOR_T color);

#endif

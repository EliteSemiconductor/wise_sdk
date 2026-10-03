/*
 * Copyright (C) 2026 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_epd_drivers.h
 * @brief EPD panel drivers available to ::wise_epd_init, and board-level panel selection.
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 *
 * Select the panel fitted on the board in the board config file
 * (BOARD_CONFIG_FILE) with one of the WISE_EPD_PANEL_* IDs:
 * @code
 * #define EPD_PANEL    WISE_EPD_PANEL_JD79651_BWR_152X296
 * @endcode
 * Application code then uses only the selected-panel macros:
 * @code
 * static uint8_t frame[WISE_EPD_PANEL_FRAME_BYTES];
 * wise_epd_init(&WISE_EPD_PANEL_DRIVER, &cfg);
 * @endcode
 *
 * Each driver is in its own source file under drivers/; unused drivers are
 * dropped by the linker (--gc-sections).
 */

#ifndef __WISE_EPD_DRIVERS_H
#define __WISE_EPD_DRIVERS_H

#include "wise_epd_api.h"

/**
 * @name Panel IDs for EPD_PANEL
 * @{
 */
#define WISE_EPD_PANEL_JD_BWRY_122X250      1 /**< ::wise_epd_drv_jd_bwry_122x250 */
#define WISE_EPD_PANEL_JD79651_BWR_152X296  2 /**< ::wise_epd_drv_jd79651_bwr_152x296 */
/** @} */

/**
 * 2.13" 122 x 250 black / white / yellow / red panel with JD (FITI) controller.
 * Format ::WISE_EPD_FMT_2BPP_BWRY, 31 bytes per row. BUSY is low while busy.
 * Source: drivers/wise_epd_drv_jd_bwry_122x250.c
 */
extern const WISE_EPD_DRV_T wise_epd_drv_jd_bwry_122x250;

/**
 * BOE 2.66" 152 x 296 black / white / red panel (MEA-266SACJMEE02A) with
 * JD79651 controller. Format ::WISE_EPD_FMT_2BPP_BWR, 38 bytes per row.
 * BUSY is low while busy.
 * Source: drivers/wise_epd_drv_jd79651_bwr_152x296.c
 */
extern const WISE_EPD_DRV_T wise_epd_drv_jd79651_bwr_152x296;

/*
 * Selected panel, from EPD_PANEL in the board config file.
 *
 *   WISE_EPD_PANEL_DRIVER       driver to pass to wise_epd_init (take its address)
 *   WISE_EPD_PANEL_WIDTH        width in pixels
 *   WISE_EPD_PANEL_HEIGHT       height in pixels
 *   WISE_EPD_PANEL_FORMAT       native WISE_EPD_PIXEL_FMT_T
 *   WISE_EPD_PANEL_ROW_BYTES    bytes per row of a frame buffer
 *   WISE_EPD_PANEL_FRAME_BYTES  bytes of a full frame buffer
 */
#if defined(EPD_PANEL)

#if (EPD_PANEL == WISE_EPD_PANEL_JD_BWRY_122X250)
#define WISE_EPD_PANEL_DRIVER wise_epd_drv_jd_bwry_122x250
#define WISE_EPD_PANEL_WIDTH  122
#define WISE_EPD_PANEL_HEIGHT 250
#define WISE_EPD_PANEL_FORMAT WISE_EPD_FMT_2BPP_BWRY
#elif (EPD_PANEL == WISE_EPD_PANEL_JD79651_BWR_152X296)
#define WISE_EPD_PANEL_DRIVER wise_epd_drv_jd79651_bwr_152x296
#define WISE_EPD_PANEL_WIDTH  152
#define WISE_EPD_PANEL_HEIGHT 296
#define WISE_EPD_PANEL_FORMAT WISE_EPD_FMT_2BPP_BWR
#else
#error "EPD_PANEL is not a known WISE_EPD_PANEL_* ID"
#endif

#define WISE_EPD_PANEL_ROW_BYTES   WISE_EPD_ROW_BYTES(WISE_EPD_PANEL_WIDTH, WISE_EPD_PANEL_FORMAT)
#define WISE_EPD_PANEL_FRAME_BYTES (WISE_EPD_PANEL_ROW_BYTES * WISE_EPD_PANEL_HEIGHT)

#endif /* EPD_PANEL */

#endif

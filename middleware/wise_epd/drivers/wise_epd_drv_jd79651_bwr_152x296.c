/*
 * Copyright (C) 2026 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/*
 * BOE 2.66" 152 x 296 black / white / red panel (MEA-266SACJMEE02A),
 * JD79651 controller. Command sequence follows the vendor sample code.
 *
 * Panel RAM is two 1-bit planes, MSB = leftmost pixel:
 *   DTM1 (0x10) black/white plane: 1 = white, 0 = black
 *   DTM2 (0x13) red plane:         1 = red (overrides the B/W plane)
 * The caller's 2 bpp frame (WISE_EPD_COLOR_T values) is split into the two
 * planes row by row while it is sent.
 *
 * Each RAM row is JD79651_RAM_WIDTH pixels. With PSR 0xCF this is the panel
 * width, 152 pixels = 19 bytes (sending 160-pixel rows was seen to skew the
 * image diagonally on hardware). For a wider RAM row the visible pixels are placed from
 * JD79651_COL_OFFSET and the rest is padded (white, not red).
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "../wise_epd_drv.h"
#include "../wise_epd_drivers.h"

#define JD79651_WIDTH              (152u)
#define JD79651_HEIGHT             (296u)
#ifndef JD79651_RAM_WIDTH
#define JD79651_RAM_WIDTH          (152u) /* RAM row length in pixels, multiple of 8 */
#endif
#define JD79651_RAM_ROW_BYTES      (JD79651_RAM_WIDTH / 8u)

#ifndef JD79651_COL_OFFSET
#define JD79651_COL_OFFSET         (0u) /* first RAM column of the visible area */
#endif

#define JD79651_REFRESH_TIMEOUT_MS (30000u)
#define JD79651_PON_TIMEOUT_MS     (5000u)
#define JD79651_POF_TIMEOUT_MS     (5000u)

#define JD79651_CMD_PSR  0x00 /* panel setting */
#define JD79651_CMD_POF  0x02 /* power off */
#define JD79651_CMD_PON  0x04 /* power on */
#define JD79651_CMD_DSLP 0x07 /* deep sleep */
#define JD79651_CMD_DTM1 0x10 /* black/white data */
#define JD79651_CMD_DRF  0x12 /* display refresh */
#define JD79651_CMD_DTM2 0x13 /* red data */

#define JD79651_PSR_VALUE        0xCF /* from vendor init_code() */
#define JD79651_DSLP_CHECK_CODE  0xA5

#if (JD79651_RAM_WIDTH % 8u) != 0
#error "JD79651_RAM_WIDTH must be a multiple of 8"
#endif
#if (JD79651_COL_OFFSET + JD79651_WIDTH) > JD79651_RAM_WIDTH
#error "JD79651_COL_OFFSET pushes the visible area past the RAM row"
#endif

typedef enum {
    JD79651_PLANE_BW = 0,
    JD79651_PLANE_RED,
} JD79651_PLANE_T;

/* Convert one 2 bpp source row into one RAM row of the given plane. */
static void _jd79651_pack_row(const uint8_t *src, JD79651_PLANE_T plane, uint8_t *dst)
{
    uint16_t x;

    /* padding: white in the B/W plane, not red in the red plane */
    memset(dst, (plane == JD79651_PLANE_BW) ? 0xFF : 0x00, JD79651_RAM_ROW_BYTES);

    for (x = 0; x < JD79651_WIDTH; x++) {
        uint8_t color = (uint8_t)((src[x / 4] >> (6 - 2 * (x % 4))) & 0x3);
        uint16_t col  = (uint16_t)(x + JD79651_COL_OFFSET);
        uint8_t mask  = (uint8_t)(0x80 >> (col % 8));
        bool set;

        if (plane == JD79651_PLANE_BW) {
            set = (color != WISE_EPD_COLOR_BLACK);
        } else {
            set = (color == WISE_EPD_COLOR_RED) || (color == WISE_EPD_COLOR_YELLOW);
        }

        if (set) {
            dst[col / 8] |= mask;
        } else {
            dst[col / 8] &= (uint8_t)~mask;
        }
    }
}

static WISE_STATUS _jd79651_write_plane(uint8_t cmd, JD79651_PLANE_T plane, const WISE_EPD_FRAME_T *frame)
{
    uint8_t ramRow[JD79651_RAM_ROW_BYTES];
    const uint8_t *row = frame->data;
    WISE_STATUS status;
    uint16_t y;

    status = wise_epd_bus_write_cmd(cmd);
    if (status != WISE_SUCCESS) {
        return status;
    }

    wise_epd_bus_data_begin();
    for (y = 0; (y < JD79651_HEIGHT) && (status == WISE_SUCCESS); y++) {
        _jd79651_pack_row(row, plane, ramRow);
        status  = wise_epd_bus_data_write(ramRow, sizeof(ramRow));
        row    += frame->stride;
    }
    wise_epd_bus_data_end();

    return status;
}

static WISE_STATUS _jd79651_init(void)
{
    static const uint8_t psr = JD79651_PSR_VALUE;

    return wise_epd_bus_write_cmd_data(JD79651_CMD_PSR, &psr, 1);
}

static WISE_STATUS _jd79651_write_frame(const WISE_EPD_FRAME_T *frame)
{
    WISE_STATUS status;

    status = _jd79651_write_plane(JD79651_CMD_DTM1, JD79651_PLANE_BW, frame);
    if (status != WISE_SUCCESS) {
        return status;
    }
    return _jd79651_write_plane(JD79651_CMD_DTM2, JD79651_PLANE_RED, frame);
}

static WISE_STATUS _jd79651_refresh(void)
{
    WISE_STATUS status;

    status = wise_epd_bus_write_cmd(JD79651_CMD_PON);
    if (status != WISE_SUCCESS) {
        return status;
    }
    status = wise_epd_bus_wait_idle(JD79651_PON_TIMEOUT_MS);
    if (status != WISE_SUCCESS) {
        return status;
    }

    status = wise_epd_bus_write_cmd(JD79651_CMD_DRF);
    if (status != WISE_SUCCESS) {
        return status;
    }
    status = wise_epd_bus_wait_busy(1000);
    if (status != WISE_SUCCESS) {
        return status;
    }
    return wise_epd_bus_wait_idle(JD79651_REFRESH_TIMEOUT_MS);
}

static WISE_STATUS _jd79651_sleep(void)
{
    static const uint8_t dslpParam = JD79651_DSLP_CHECK_CODE;
    WISE_STATUS status;

    status = wise_epd_bus_write_cmd(JD79651_CMD_POF);
    if (status != WISE_SUCCESS) {
        return status;
    }
    /* Best effort: still request deep sleep if power-off did not report idle. */
    status = wise_epd_bus_wait_idle(JD79651_POF_TIMEOUT_MS);
    wise_epd_bus_write_cmd_data(JD79651_CMD_DSLP, &dslpParam, 1);

    return status;
}

const WISE_EPD_DRV_T wise_epd_drv_jd79651_bwr_152x296 = {
    .name       = "jd79651_bwr_152x296",
    .width      = JD79651_WIDTH,
    .height     = JD79651_HEIGHT,
    .format     = WISE_EPD_FMT_2BPP_BWR,
    .busy_level = GPIO_LOW,
    .timing =
        {
            /* vendor EPD_Reset(): RST high 100 ms, low 60 ms, high 100 ms */
            .power_on_ms   = 20,
            .io_ready_ms   = 100,
            .reset_low_ms  = 60,
            .reset_high_ms = 100,
        },
    .reset_timeout_ms   = 1000,
    .refresh_timeout_ms = JD79651_REFRESH_TIMEOUT_MS,
    .init               = _jd79651_init,
    .write_frame        = _jd79651_write_frame,
    .refresh            = _jd79651_refresh,
    .sleep              = _jd79651_sleep,
};

/*
 * Copyright (C) 2026 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/*
 * 2.13" 122 x 250 black / white / yellow / red panel, JD (FITI) controller.
 *
 * Panel RAM holds 2 bits per pixel, MSB first, with the same values as
 * WISE_EPD_COLOR_T (00 black, 01 white, 10 yellow, 11 red), so frames are sent
 * unchanged. Each 122-pixel row is padded to 31 bytes.
 */

#include <stddef.h>

#include "../wise_epd_drv.h"
#include "../wise_epd_drivers.h"

#define JD_WIDTH              (122u)
#define JD_HEIGHT             (250u)
#define JD_REFRESH_TIMEOUT_MS (30000u)

/* Booster start-up / shut-down; long on some panels, so allow plenty. */
#ifndef JD_PON_TIMEOUT_MS
#define JD_PON_TIMEOUT_MS (10000u)
#endif
#ifndef JD_POF_TIMEOUT_MS
#define JD_POF_TIMEOUT_MS (5000u)
#endif

#define JD_CMD_PSR  0x00 /* panel setting */
#define JD_CMD_POF  0x02 /* power off */
#define JD_CMD_PON  0x04 /* power on */
#define JD_CMD_DSLP 0x07 /* deep sleep */
#define JD_CMD_DTM1 0x10 /* data start transmission */
#define JD_CMD_DRF  0x12 /* display refresh */
#define JD_CMD_4D   0x4D
#define JD_CMD_BE   0xBE

#define JD_DSLP_CHECK_CODE 0xA5

/*
 * The vendor init table below is not sent yet: the reference code ran with it
 * disabled and the panel's power-on defaults. Set to 1 once checked against
 * the panel datasheet.
 */
#ifndef EPD_JD_BWRY_SEND_INIT_TABLE
#define EPD_JD_BWRY_SEND_INIT_TABLE 0
#endif

#if EPD_JD_BWRY_SEND_INIT_TABLE
/* Each entry: length (command + parameters), command, parameters... */
static const uint8_t jdInitTable[] = {
    2, JD_CMD_4D,  0x78,
    3, JD_CMD_PSR, 0x0F, 0x29,
    2, JD_CMD_BE,  0x00,
};
#endif

static WISE_STATUS _jd_init(void)
{
#if EPD_JD_BWRY_SEND_INIT_TABLE
    uint32_t i = 0;

    while (i < sizeof(jdInitTable)) {
        uint8_t len         = jdInitTable[i];
        WISE_STATUS status = wise_epd_bus_write_cmd_data(jdInitTable[i + 1], &jdInitTable[i + 2], len - 1u);

        if (status != WISE_SUCCESS) {
            return status;
        }
        i += 1u + len;
    }
    return wise_epd_bus_wait_idle(1000);
#else
    return WISE_SUCCESS;
#endif
}

static WISE_STATUS _jd_write_frame(const WISE_EPD_FRAME_T *frame)
{
    WISE_STATUS status;
    const uint8_t *row = frame->data;
    uint16_t h;

    status = wise_epd_bus_write_cmd(JD_CMD_DTM1);
    if (status != WISE_SUCCESS) {
        return status;
    }

    wise_epd_bus_data_begin();
    if (frame->stride == frame->row_bytes) {
        /* Rows are contiguous: one burst for the whole frame. */
        status = wise_epd_bus_data_write(row, (uint32_t)frame->row_bytes * JD_HEIGHT);
    } else {
        for (h = 0; (h < JD_HEIGHT) && (status == WISE_SUCCESS); h++) {
            status = wise_epd_bus_data_write(row, frame->row_bytes);
            row += frame->stride;
        }
    }
    wise_epd_bus_data_end();

    if (status != WISE_SUCCESS) {
        return status;
    }
    return wise_epd_bus_wait_idle(1000);
}

static WISE_STATUS _jd_refresh(void)
{
    static const uint8_t drfParam = 0x00; /* 0: AC VCOM, 1: DC VCOM */
    WISE_STATUS status;

    status = wise_epd_bus_write_cmd(JD_CMD_PON);
    if (status != WISE_SUCCESS) {
        return status;
    }
    status = wise_epd_bus_wait_idle(JD_PON_TIMEOUT_MS);
    if (status != WISE_SUCCESS) {
        return status;
    }

    status = wise_epd_bus_write_cmd_data(JD_CMD_DRF, &drfParam, 1);
    if (status != WISE_SUCCESS) {
        return status;
    }
    status = wise_epd_bus_wait_busy(1000);
    if (status != WISE_SUCCESS) {
        return status;
    }
    return wise_epd_bus_wait_idle(JD_REFRESH_TIMEOUT_MS);
}

static WISE_STATUS _jd_sleep(void)
{
    static const uint8_t pofParam  = 0x00;
    static const uint8_t dslpParam = JD_DSLP_CHECK_CODE;
    WISE_STATUS status;

    status = wise_epd_bus_write_cmd_data(JD_CMD_POF, &pofParam, 1);
    if (status != WISE_SUCCESS) {
        return status;
    }
    /* Best effort: still request deep sleep if power-off did not report idle. */
    status = wise_epd_bus_wait_idle(JD_POF_TIMEOUT_MS);
    wise_epd_bus_write_cmd_data(JD_CMD_DSLP, &dslpParam, 1);

    return status;
}

const WISE_EPD_DRV_T wise_epd_drv_jd_bwry_122x250 = {
    .name               = "jd_bwry_122x250",
    .width              = JD_WIDTH,
    .height             = JD_HEIGHT,
    .format             = WISE_EPD_FMT_2BPP_BWRY,
    .busy_level         = GPIO_LOW,
    .timing =
        {
            .power_on_ms   = 20,
            .io_ready_ms   = 10,
            .reset_low_ms  = 20,
            .reset_high_ms = 20,
        },
    .reset_timeout_ms   = 1000,
    .refresh_timeout_ms = JD_REFRESH_TIMEOUT_MS,
    .init               = _jd_init,
    .write_frame        = _jd_write_frame,
    .refresh            = _jd_refresh,
    .sleep              = _jd_sleep,
};

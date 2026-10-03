/*
 * Copyright (C) 2026 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* WISE_EPD_LOG_LEVEL overrides WISE_LOG_LEVEL for this module only. */
#ifdef WISE_EPD_LOG_LEVEL
#undef WISE_LOG_LEVEL
#define WISE_LOG_LEVEL WISE_EPD_LOG_LEVEL
#endif

#include <stddef.h>
#include <string.h>

#include "wise_epd_api.h"
#include "wise_epd_drv.h"
#include "util_debug_log.h"

/* Largest row wise_epd_clear builds on the stack: 400 px at 2 bpp. */
#define EPD_MAX_ROW_BYTES (100u)

static const WISE_EPD_DRV_T *epdDrv = NULL;

static WISE_STATUS _epd_run(const WISE_EPD_FRAME_T *frame)
{
    WISE_STATUS status;

    status = wise_epd_bus_open(&epdDrv->timing);
    if (status != WISE_SUCCESS) {
        return status;
    }

    status = wise_epd_bus_wait_idle(epdDrv->reset_timeout_ms);
    if (status != WISE_SUCCESS) {
        WISE_LOG_ERR("EPD %s: no response after reset\n", epdDrv->name);
        goto finish;
    }

    if (epdDrv->init != NULL) {
        status = epdDrv->init();
        if (status != WISE_SUCCESS) {
            WISE_LOG_ERR("EPD %s: init failed (%ld)\n", epdDrv->name, (long)status);
            goto finish;
        }
    }

    status = epdDrv->write_frame(frame);
    if (status != WISE_SUCCESS) {
        WISE_LOG_ERR("EPD %s: write_frame failed (%ld)\n", epdDrv->name, (long)status);
        goto finish;
    }

    status = epdDrv->refresh();
    if (status != WISE_SUCCESS) {
        WISE_LOG_ERR("EPD %s: refresh failed (%ld)\n", epdDrv->name, (long)status);
    }

finish:
    /* Sleep even after a failed refresh so the panel is never left driving. */
    if (epdDrv->sleep != NULL) {
        WISE_STATUS sleepStatus = epdDrv->sleep();

        if (sleepStatus != WISE_SUCCESS) {
            WISE_LOG_ERR("EPD %s: sleep failed (%ld)\n", epdDrv->name, (long)sleepStatus);
        }

        if (status == WISE_SUCCESS) {
            status = sleepStatus;
        }
    }
    wise_epd_bus_close();

    return status;
}

WISE_STATUS wise_epd_init(const WISE_EPD_DRV_T *drv, const WISE_EPD_CFG_T *cfg)
{
    if ((drv == NULL) || (cfg == NULL) || (drv->write_frame == NULL) || (drv->refresh == NULL)) {
        return WISE_EPD_ERR_PARAM;
    }
    if (cfg->spi_channel > 1) {
        return WISE_EPD_ERR_PARAM;
    }
    if (WISE_EPD_ROW_BYTES(drv->width, drv->format) > EPD_MAX_ROW_BYTES) {
        return WISE_EPD_ERR_PARAM;
    }

    wise_epd_bus_setup(cfg, drv->busy_level);
    epdDrv = drv;

    return WISE_SUCCESS;
}

void wise_epd_deinit(void)
{
    epdDrv = NULL;
}

WISE_STATUS wise_epd_get_info(WISE_EPD_INFO_T *info)
{
    if (info == NULL) {
        return WISE_EPD_ERR_PARAM;
    }
    if (epdDrv == NULL) {
        return WISE_EPD_ERR_NOT_INIT;
    }

    info->name      = epdDrv->name;
    info->width     = epdDrv->width;
    info->height    = epdDrv->height;
    info->format    = epdDrv->format;
    info->row_bytes = (uint16_t)WISE_EPD_ROW_BYTES(epdDrv->width, epdDrv->format);

    return WISE_SUCCESS;
}

WISE_STATUS wise_epd_display(const WISE_EPD_IMAGE_T *img)
{
    WISE_EPD_FRAME_T frame;
    uint16_t rowBytes;

    if ((img == NULL) || (img->data == NULL)) {
        return WISE_EPD_ERR_PARAM;
    }
    if (epdDrv == NULL) {
        return WISE_EPD_ERR_NOT_INIT;
    }
    if ((img->width != epdDrv->width) || (img->height != epdDrv->height) || (img->format != epdDrv->format)) {
        WISE_LOG_ERR("EPD image %ux%u fmt %d, panel %ux%u fmt %d\n", img->width, img->height, img->format,
                     epdDrv->width, epdDrv->height, epdDrv->format);
        return WISE_EPD_ERR_FORMAT;
    }

    rowBytes = (uint16_t)WISE_EPD_ROW_BYTES(img->width, img->format);
    if ((img->stride != 0) && (img->stride < rowBytes)) {
        return WISE_EPD_ERR_PARAM;
    }

    frame.data      = img->data;
    frame.stride    = (img->stride != 0) ? img->stride : rowBytes;
    frame.row_bytes = rowBytes;

    return _epd_run(&frame);
}

WISE_STATUS wise_epd_clear(WISE_EPD_COLOR_T color)
{
    uint8_t row[EPD_MAX_ROW_BYTES];
    WISE_EPD_FRAME_T frame;
    uint8_t fill;

    if (epdDrv == NULL) {
        return WISE_EPD_ERR_NOT_INIT;
    }

    if (epdDrv->format == WISE_EPD_FMT_1BPP_BW) {
        if (color > WISE_EPD_COLOR_WHITE) {
            return WISE_EPD_ERR_PARAM;
        }
        fill = (color == WISE_EPD_COLOR_WHITE) ? 0xFF : 0x00;
    } else {
        if ((color > WISE_EPD_COLOR_RED) ||
            ((epdDrv->format == WISE_EPD_FMT_2BPP_BWR) && (color == WISE_EPD_COLOR_YELLOW))) {
            return WISE_EPD_ERR_PARAM;
        }
        fill = (uint8_t)(color * 0x55); /* replicate the 2-bit value into all four pixels */
    }

    frame.row_bytes = (uint16_t)WISE_EPD_ROW_BYTES(epdDrv->width, epdDrv->format);
    frame.stride    = 0;
    frame.data      = row;
    memset(row, fill, frame.row_bytes);

    return _epd_run(&frame);
}

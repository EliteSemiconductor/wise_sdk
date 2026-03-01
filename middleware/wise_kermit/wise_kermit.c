/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if MIDDLEWARE_WISE_KERMIT
#include "api/wise_flash_api.h"
#include "api/wise_uart_api.h"
#include "wise_core.h"
#include "kermit.h"
#include "wise_file_system.h"
#include "wise_kermit.h"

#define KERMIT_BUFFER_LEN 0x00000800 // reserve 2K for kermit transmission

typedef int (*KERMIT_OUTPUT_PROC)(struct k_data *k, uint8_t *s, int n);

static struct k_data *kData    = (struct k_data *)0;
static struct k_response *resp = (struct k_response *)0;

static struct {
    uint8_t target;
    uint8_t result;
    int8_t uartIntf;
    uint32_t address;
    uint32_t recvLen;
    uint32_t flashSize;
    uint32_t sectorSize;
    uint32_t sectorOffset;
    uint16_t crc;
} stKermitLoad;

int _kermit_write_data(struct k_data *k, uint8_t *p, int n);
int _kermit_read_pkt(struct k_data *k, uint8_t *p, int len, int fc);
int _kermit_output_flash(struct k_data *k, uint8_t *s, int n);
int _kermit_output_ram(struct k_data *k, uint8_t *s, int n);

int _kermit_write_data(struct k_data *k, uint8_t *p, int n)
{
    int x;
    int max;

    max = 10; /* Loop breaker */

    while (n > 0) { /* Keep trying till done */
        wise_uart_write(stKermitLoad.uartIntf, p, n);
        x = n;

        if (x < 0 || --max < 1) { /* Errors are fatal */
            return (X_ERROR);
        }
        n -= x;
        p += x;
    }

    return (X_OK); /* Success */
}

int _kermit_read_pkt(struct k_data *k, uint8_t *p, int len, int fc)
{
    int x, n;
    short flag;
    uint8_t c;
    /*
      Timeout not implemented in this sample.
      It should not be needed.  All non-embedded Kermits that are capable of
      making connections are also capable of timing out, and only one Kermit
      needs to time out.  NOTE: This simple example waits for SOH and then
      reads everything up to the negotiated packet terminator.  A more robust
      version might be driven by the value of the packet-length field.
    */

#ifdef F_CTRLC
    short ccn;
    ccn = 0;
#endif /* F_CTRLC */

    flag = n = 0; /* Init local variables */

    while (1) {
        uint8_t inputC;

        if (WISE_SUCCESS !=
            wise_uart_read_char(stKermitLoad.uartIntf, &inputC)) {
            continue;
        }

        x = inputC;
        c = (k->parity) ? x & 0x7f : x & 0xff; /* Strip parity */

#ifdef F_CTRLC
        /* In remote mode only: three consecutive ^C's to quit */
        if (k->remote && c == (UCHAR)3) {
            if (++ccn > 2) {
                // debug(DB_MSG,"readpkt ^C^C^C",0,0);
                return (-1);
            }
        } else {
            ccn = 0;
        }
#endif /* F_CTRLC */

        if (!flag && c != SOH) { /* No start of packet yet */
            continue;            /* so discard these bytes. */
        }
        if (c == SOH) { /* Start of packet */
            flag = 1;   /* Remember */
            continue;   /* But discard. */
        } else if ((c == CR) || (c == '\012')) {
            return (n);
        } else {                  /* Contents of packet */
            if (n++ > P_PKTLEN) { /* Check length */
                return (0);
            } else {
                *p++ = x & 0xff;
            }
        }
    }

    return (-1);
}

int _kermit_output_flash(struct k_data *k, uint8_t *s, int n)
{
    if (n > 0) {
        uint32_t flashWriteStart = stKermitLoad.address + stKermitLoad.recvLen;
        uint32_t sectorSpace =
            stKermitLoad.sectorSize - stKermitLoad.sectorOffset;

        stKermitLoad.crc = crc16_kermit(s, n, stKermitLoad.crc);

        while (n > 0) {
            int writeLen = 0;

            if ((flashWriteStart % stKermitLoad.sectorSize) == 0) {
                if (HAL_ERR == wise_flash_sector_erase(flashWriteStart)) {
                    debug_print("erase failed\n");
                    return X_ERROR;
                }

                sectorSpace               = stKermitLoad.sectorSize;
                stKermitLoad.sectorOffset = 0;
            }

            writeLen = n > sectorSpace ? sectorSpace : n;

            wise_flash_write(flashWriteStart, s, writeLen);

            flashWriteStart += writeLen;
            n               -= writeLen;
            sectorSpace     -= writeLen;

            stKermitLoad.recvLen      += writeLen;
            stKermitLoad.sectorOffset += writeLen;
        }
    } else if (n < 0) {
        debug_print("kermit output err\n");
        return X_ERROR;
    }

    return X_OK;
}

int _kermit_output_ram(struct k_data *k, uint8_t *s, int n)
{
    if (n > 0) {
        memcpy((void *)(stKermitLoad.address + stKermitLoad.recvLen), s, n);
        stKermitLoad.recvLen += n;
    } else if (n < 0) {
        debug_print("kermit output err\n");
        return X_ERROR;
    }

    return X_OK;
}

int32_t wise_kermit_init(int8_t target, uint32_t param, int8_t uartIntf)
{
    int32_t result = WISE_SUCCESS;

    memset(&stKermitLoad, 0, sizeof(stKermitLoad));

    stKermitLoad.uartIntf = uartIntf;
    stKermitLoad.crc      = 0xFFFF; // CRC init value

    switch (target) {
    case E_KERMIT_TARGET_FS: 
    {
#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
        WISE_FS_INFO_T fsInfo;

        wise_fs_get_fs_info(&fsInfo);
        if ((fsInfo.storageSize == 0) || (fsInfo.eraseSize == 0)) {
            debug_print("Invalid flash info\n");
            result = WISE_FAIL;
            goto finish;
        }

        stKermitLoad.target     = E_KERMIT_TARGET_FS;
        stKermitLoad.flashSize  = fsInfo.storageSize;
        stKermitLoad.sectorSize = fsInfo.eraseSize;

        if ((param > 0) && (param < fsInfo.parNum)) {
            stKermitLoad.address      = fsInfo.partitions[param].offset;
            stKermitLoad.sectorOffset = 0;
        } else {
            result = WISE_FAIL;
            goto finish;
        }
#else
        debug_print("File system module not exists\n");
#endif
    } 
    break;

    case E_KERMIT_TARGET_FLASH: {
        WISE_FLASH_INFO_T flashInfo;

        wise_flash_get_info(&flashInfo);

        stKermitLoad.target     = E_KERMIT_TARGET_FLASH;
        stKermitLoad.flashSize  = flashInfo.flashSize;
        stKermitLoad.sectorSize = flashInfo.minEraseSize;

        stKermitLoad.address      = param;
        stKermitLoad.sectorOffset = 0;
    } break;

    case E_KERMIT_TARGET_RAM: {
        stKermitLoad.target  = E_KERMIT_TARGET_RAM;
        stKermitLoad.address = param;
    } break;

    default:
        return WISE_FAIL;
    }

finish:
    return result;
}

int32_t wise_kermit_load(uint32_t *rxDataLen, uint16_t *rxDataCrc)
{
    uint8_t *kermitBuffer = malloc(KERMIT_BUFFER_LEN);
    int32_t status        = WISE_SUCCESS;
    uint32_t bufferPtr    = 0;
    int32_t bufferResidue = 0;
    uint8_t *i_buf;
    uint8_t *o_buf;
    uint8_t *inbuf;
    short r_slot;
    int rx_len;
    uint32_t fileSize = 0;

    if (!kermitBuffer) {
        debug_print("Failed to allocate kermit buffer\n");
        return WISE_FAIL;
    }

    bufferPtr     = (uint32_t)kermitBuffer;
    bufferResidue = KERMIT_BUFFER_LEN;

    if (bufferResidue < sizeof(struct k_data)) {
        debug_print("kbuf err\n");
        goto finish;
    }

    kData          = (struct k_data *)bufferPtr;
    bufferPtr     += sizeof(struct k_data);
    bufferResidue -= sizeof(struct k_data);

    resp           = (struct k_response *)bufferPtr;
    bufferPtr     += sizeof(struct k_response);
    bufferResidue -= sizeof(struct k_response);

    i_buf          = (uint8_t *)bufferPtr;
    bufferPtr     += IBUFLEN;
    bufferResidue -= IBUFLEN;

    o_buf          = (uint8_t *)bufferPtr;
    bufferPtr     += OBUFLEN;
    bufferResidue -= OBUFLEN;

    if (bufferResidue < 0) {
        debug_print("out of kbuf\n");
        goto finish;
    }

    kData->parity  = 0;                  /* Communications parity */
    kData->bct     = 1;                  /* Block check type */
    kData->obuf    = o_buf;              /* File output buffer */
    kData->obuflen = OBUFLEN;            /* File output buffer length */
    kData->obufpos = 0;                  /* File output buffer position */
    kData->rxd     = _kermit_read_pkt;   /* for reading packets */
    kData->txd     = _kermit_write_data; /* for sending packets */

    if ((stKermitLoad.target == E_KERMIT_TARGET_FS) ||
        (stKermitLoad.target == E_KERMIT_TARGET_FLASH)) {
        kData->writef = _kermit_output_flash; /* for writing to output file */
    } else {
        kData->writef = _kermit_output_ram;
    }

    kData->dbf = 0;

    /* Force Type 3 Block Check (16-bit CRC) on all packets, or not */
    kData->bctf = 0;

    status = kermit(K_INIT, kData, 0, "", resp);
    if (status == X_ERROR) {
        goto finish;
    }

    while (status != X_DONE) {
        inbuf  = getrslot(kData); /* Allocate a window slot */
        rx_len = kData->rxd(kData, inbuf, P_PKTLEN); /* Try to read a packet */

        if (rx_len < 1) {
            freerslot(kData);
            if (rx_len < 0) {
                status = WISE_FAIL;
                break;
            }
        }

        status = kermit(K_RUN, kData, rx_len, "", resp);
        switch (status) {
        case X_OK:
            continue;

        case X_DONE:
            status = WISE_SUCCESS;
            goto finish;
            break;

        case X_ERROR:
            status = WISE_FAIL;
            goto finish;
            break;
        }
    }

finish:
    if (status == WISE_SUCCESS) {
        if (rxDataLen) {
            *rxDataLen = stKermitLoad.recvLen;
        }
        if (rxDataCrc) {
            *rxDataCrc = stKermitLoad.crc;
        }
    }

    free(kermitBuffer);
    return status;
}
#endif /* MIDDLEWARE_WISE_KERMIT */

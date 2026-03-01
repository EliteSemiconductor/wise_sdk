/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "wise_core.h"
#include "util.h"
#include "wise_fifo.h"

#define MAX_FIFO_NUM 8

typedef struct {
    uint8_t *buffer;
    uint16_t bufLen;
    uint16_t wPos;
    uint16_t rPos;
    uint16_t dataLen;
} ST_WISE_FIFO_T;

uint8_t fifoNum                               = 0;
static ST_WISE_FIFO_T *fifoBank[MAX_FIFO_NUM] = {0};

int8_t wise_fifo_check_empty()
{
    int i;

    if (fifoNum) {
        for (i = 0; i < fifoNum; i++) {
            if ((fifoBank[i]) && (fifoBank[i]->dataLen > 0)) {
                return 0;
            }
        }
    }

    return 1;
}

WISE_FIFO_REF wise_fifo_create(uint8_t *buffer, uint16_t bufLen)
{
    ST_WISE_FIFO_T *fifoHandle =
        (ST_WISE_FIFO_T *)malloc(sizeof(ST_WISE_FIFO_T));

    if (!fifoHandle) {
        return NULL;
    }

    memset(fifoHandle, 0, sizeof(ST_WISE_FIFO_T));

    fifoHandle->buffer = buffer;
    fifoHandle->bufLen = bufLen;

    if (fifoNum < MAX_FIFO_NUM) {
        fifoBank[fifoNum] = fifoHandle;
        fifoNum++;
    } else {
        debug_print("WARNING!! fifo bank overflow\n");
    }

    return fifoHandle;
}

void wise_fifo_reset(WISE_FIFO_REF fifoRef)
{
    ST_WISE_FIFO_T *fifo = (ST_WISE_FIFO_T *)fifoRef;

    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    fifo->wPos    = 0;
    fifo->rPos    = 0;
    fifo->dataLen = 0;
    memset(fifo->buffer, 0, fifo->bufLen);

    CORE_EXIT_CRITICAL();
}

int8_t wise_fifo_push_char(WISE_FIFO_REF fifoRef, uint8_t inputC)
{
    ST_WISE_FIFO_T *fifo = (ST_WISE_FIFO_T *)fifoRef;
    int8_t result        = WISE_FAIL;
    CORE_DECLARE_IRQ_STATE;

    if (!CORE_IS_IN_ISR()) {
        CORE_ENTER_CRITICAL();
    }

    if (fifo->dataLen < fifo->bufLen) {
        fifo->buffer[fifo->wPos] = inputC;
        fifo->wPos++;
        if (fifo->wPos == fifo->bufLen) {
            fifo->wPos = 0;
        }
        fifo->dataLen++;

        result = WISE_SUCCESS;
    }
    
    if (!CORE_IS_IN_ISR()) {
        CORE_EXIT_CRITICAL();
    }

    return result;
}

int8_t wise_fifo_pop_char(WISE_FIFO_REF fifoRef, uint8_t *c)
{
    ST_WISE_FIFO_T *fifo = (ST_WISE_FIFO_T *)fifoRef;
    int8_t result        = WISE_FAIL;

    if (fifo->dataLen > 0) {
        CORE_DECLARE_IRQ_STATE;

        if (!CORE_IS_IN_ISR()) {
            CORE_ENTER_CRITICAL();
        }

        *c = fifo->buffer[fifo->rPos];
        fifo->rPos++;
        if (fifo->rPos == fifo->bufLen) {
            fifo->rPos = 0;
        }
        fifo->dataLen--;

        if (!CORE_IS_IN_ISR()) {
            CORE_EXIT_CRITICAL();
        }

        result = WISE_SUCCESS;
    }

    return result;
}

int8_t wise_fifo_push_char_no_crit(WISE_FIFO_REF fifoRef, uint8_t inputC)
{
    ST_WISE_FIFO_T *fifo = (ST_WISE_FIFO_T *)fifoRef;
    int8_t result        = WISE_FAIL;
    
    if (fifo->dataLen < fifo->bufLen) {
        fifo->buffer[fifo->wPos] = inputC;
        fifo->wPos++;
        if (fifo->wPos == fifo->bufLen) {
            fifo->wPos = 0;
        }
        fifo->dataLen++;

        result = WISE_SUCCESS;
    }
    
    return result;
}

int8_t wise_fifo_pop_char_no_crit(WISE_FIFO_REF fifoRef, uint8_t *c)
{
    ST_WISE_FIFO_T *fifo = (ST_WISE_FIFO_T *)fifoRef;
    int8_t result        = WISE_FAIL;

    if (fifo->dataLen > 0) 
    {
        *c = fifo->buffer[fifo->rPos];
        fifo->rPos++;
        if (fifo->rPos == fifo->bufLen) {
            fifo->rPos = 0;
        }
        fifo->dataLen--;

        result = WISE_SUCCESS;
    }

    return result;
}

uint16_t wise_fifo_push(WISE_FIFO_REF fifoRef, uint8_t *buf, uint16_t len)
{
    ST_WISE_FIFO_T *fifo = (ST_WISE_FIFO_T *)fifoRef;
    uint16_t residue     = 0;
    uint8_t *dataPtr     = buf;
    
    if (len > (fifo->bufLen - fifo->dataLen)) {
        len = fifo->bufLen - fifo->dataLen;
    }

    residue = fifo->bufLen - fifo->wPos - 1;
    if (residue >= len) {
        residue = len;
    }

    memcpy(&fifo->buffer[fifo->wPos], dataPtr, residue);
    dataPtr += residue;
    residue  = len - residue;

    if (residue) {
        memcpy(&fifo->buffer[0], dataPtr, residue);
    }

    fifo->wPos += len;
    if (fifo->wPos >= fifo->bufLen) {
        fifo->wPos = fifo->wPos - fifo->bufLen;
    }

    return len;
}

uint16_t wise_fifo_pop(WISE_FIFO_REF fifoRef, uint8_t *buf, uint16_t len)
{
    ST_WISE_FIFO_T *fifo = (ST_WISE_FIFO_T *)fifoRef;
    uint8_t *dataPtr     = buf;
    uint16_t readLen     = 0;
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    if (!CORE_IS_IN_ISR()) {
        CORE_ENTER_CRITICAL();
    }

    len = len > fifo->dataLen ? fifo->dataLen : len;

    readLen = fifo->bufLen - fifo->rPos - 1;
    if (readLen >= len) {
        readLen = len;
    }

    memcpy(dataPtr, &fifo->buffer[fifo->rPos], readLen);
    dataPtr += readLen;
    readLen  = len - readLen;

    if (readLen) {
        memcpy(dataPtr, &fifo->buffer[0], readLen);
    }

    fifo->rPos += len;
    if (fifo->rPos >= fifo->bufLen) {
        fifo->rPos = fifo->rPos - fifo->bufLen;
    }

    if (!CORE_IS_IN_ISR()) {
        CORE_EXIT_CRITICAL();
    }

    return len;
}

int16_t wise_fifo_get_data_len(WISE_FIFO_REF fifoH)
{
    return ((ST_WISE_FIFO_T *)fifoH)->dataLen;
}

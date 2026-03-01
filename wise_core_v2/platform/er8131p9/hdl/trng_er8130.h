/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __TRNG_ER8130_H
#define __TRNG_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

#define MIN_SAMPLE_CNT_SHORTEST 11
#define MIN_SAMPLE_CNT_SHORT 15
#define MIN_SAMPLE_CNT_LONG 19
#define MIN_SAMPLE_CNT_LONGEST 21
#define SAMPLE_CNT_DEFAULT 100
#define TRNG_SAMPLE_CNT SAMPLE_CNT_DEFAULT

#define TRNG_EHR_DATA_NUM 6 // type: uint32

#define TRNG_STS_VALID BIT0
#define TRNG_STS_AUTOCORR_ERR BIT1
#define TRNG_STS_CRNGT_ERR BIT2
#define TRNG_STS_VN_ERR BIT3

enum {
    TRNG_ENTROPY_INVERTER_SHORTEST =
        0, //!<		0: TRNG select the shortest inverter chain length
    TRNG_ENTROPY_INVERTER_SHORT,   //!<		1: TRNG select the short inverter
                                   //!<chain length
    TRNG_ENTROPY_INVERTER_LONG,    //!<		2: TRNG select the long inverter
                                   //!<chain length
    TRNG_ENTROPY_INVERTER_LONGEST, //!<		3: TRNG select the longest inverter
                                   //!<chain length
};

void trng_sw_rst_er8130(void);
uint8_t trng_get_busy_er8130(void);
uint32_t trng_get_status_er8130(void);
void trng_clear_status_er8130(uint32_t status);
void trng_config_er8130(void);
void trng_start_er8130(void);
uint8_t trng_get_ehr_data_er8130(uint32_t *ehr_buf);
#endif /* __TRNG_ER8130_H  */

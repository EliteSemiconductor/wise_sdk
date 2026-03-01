/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __GPTMR_ER8130_H
#define __GPTMR_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

HAL_STATUS gptmr_open_er8130(GPTMR_T *gptmr, uint32_t mode);
void gptmr_start_er8130(GPTMR_T *gptmr);
void gptmr_stop_er8130(GPTMR_T *gptmr);
void gptmr_enable_int_er8130(GPTMR_T *gptmr);
void gptmr_disable_int_er8130(GPTMR_T *gptmr);
bool gptmr_is_enabled_er8130(GPTMR_T *gptmr);
void gptmr_clear_int_flag_er8130(GPTMR_T *gptmr);
void gptmr_set_cnt_er8130(GPTMR_T *gptmr, uint32_t u32Cnt);
void gptmr_set_freq_er8130(GPTMR_T *gptmr, uint32_t u32Freq);
void gptmr_reset_counter_er8130(GPTMR_T *gptmr);
uint32_t gptmr_get_cnt_er8130(GPTMR_T *gptmr);
void gptmr_output_config_er8130(GPTMR_T *gptmr, uint32_t u32PinMask);
uint32_t gptmr_get_reload_er8130(GPTMR_T *gptmr);

#endif /* __GPTMR_ER8130_H */

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_EXTPMU_H
#define __HAL_DRV_EXTPMU_H

#include "hal_intf_extpmu.h"
#include <stdint.h>

#ifdef CHIP_HAS_LFOSC
#define MS_TO_TMR_PROD(m) (m * 16384 / 1000)
#else
#define MS_TO_TMR_PROD(m) (m * 32768 / 1000)
#endif

void hal_drv_extpmu_set_shutdown(uint8_t wu_src, uint32_t wu_time);
void hal_drv_extpmu_select_clk_source(uint8_t clk_src);
void hal_drv_extpmu_disable_shutdown_control_for_32k_16k_osc(void);
void hal_drv_extpmu_enable_external_pmu(uint8_t enable);

#endif // __HAL_DRV_EXTPMU_H
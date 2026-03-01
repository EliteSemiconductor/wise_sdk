/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_EXTPMU_H
#define __HAL_INTF_EXTPMU_H

#include "esmt_chip_specific.h"
#include "types.h"
#include <stdbool.h>
#include <stdint.h>

enum EXTPMU_CLK_SRC {
    EXTPMU_CLK_SRC_32K = 0, // 32KHz clock source
    EXTPMU_CLK_SRC_16K = 1  // 16KHz clock source
};

enum EXTPMU_WAKEUP_SRC {
    EXTPMU_WAKEUP_SRC_WUTMR = 0,
    EXTPMU_WAKEUP_SRC_NFC = 3,
    EXTPMU_WAKEUP_SRC_GPIO = 4
};

void hal_intf_extpmu_enable_external_pmu(uint8_t enable);
void hal_intf_extpmu_set_shutdown(uint8_t wu_src, uint32_t wu_time, uint8_t wake_pin_idx);
void hal_intf_extpmu_select_clk_source(uint8_t clk_src);
void hal_intf_extpmu_disable_shutdown_control_for_32k_16k_osc(void);
uint32_t hal_intf_extpmu_get_wutmr_counter(void);

#endif // __HAL_INTF_EXTPMU_H

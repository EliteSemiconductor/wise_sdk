/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_pmu_api.h"

int32_t wise_pmu_module_clk_disable(uint32_t module_mask)
{
    hal_intf_pmu_module_clk_disable(module_mask);
    return WISE_SUCCESS;
}

int32_t wise_pmu_module_clk_enable(uint32_t module_mask)
{
    hal_intf_pmu_module_clk_enable(module_mask);
    return WISE_SUCCESS;
}

bool wise_pmu_module_clk_is_enabled(uint32_t module_mask)
{
    return hal_intf_pmu_module_clk_is_enabled(module_mask);
}

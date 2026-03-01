/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/extpmu_er8130.h"

void _extpmu_ana_time_config(void)
{
    uint8_t tim_a_dcdc = 8;
    uint8_t tim_40m_gain = 10;
    uint32_t reg = REG_R32(EXTPMU_EXT_PMU_TIM_REG_ADDR);

    reg &= ~(EXTPMU_PMU_TIM_A_DCDC_MASK | EXTPMU_PMU_TIM_A_40M_GAIN_MASK);
    reg |= (((tim_a_dcdc << EXTPMU_PMU_TIM_A_DCDC_POS) & EXTPMU_PMU_TIM_A_DCDC_MASK) |
            ((tim_40m_gain << EXTPMU_PMU_TIM_A_40M_GAIN_POS) & EXTPMU_PMU_TIM_A_40M_GAIN_MASK));
    REG_W32(EXTPMU_EXT_PMU_TIM_REG_ADDR, reg);
}

void extpmu_set_enable_er8130(uint8_t enable)
{
    _extpmu_ana_time_config();
    REG_W32(EXTPMU_EXT_PMU_GLO_REG_ADDR, 0x0);
    if (enable) {
        REG_W32(EXTPMU_EXT_PMU_GLO_REG_ADDR, EXTPMU_PMU_EN_MASK);
    }
}

void extpmu_set_wakeup_time_er8130(uint32_t time)
{
    REG_W32(EXTPMU_EXT_PMU_TMR_PROD_REG_ADDR, time);
}

void extpmu_set_sleep_er8130(uint8_t wutmr_en)
{
    uint8_t reg = EXTPMU_SLEEP_EN_MASK;

    if (wutmr_en) {
        reg |= EXTPMU_WUTMR_EN_MASK;
    }

    REG_W32(EXTPMU_EXT_PMU_CTL_REG_ADDR, reg);
}

void extpmu_set_wakeup_source_er8130(uint8_t wu_src)
{
    uint32_t reg = REG_R32(EXTPMU_EXT_PMU_WU_REG_ADDR);

    reg &= ~(EXTPMU_PMU_WUBY_WUTMR_MASK | EXTPMU_PMU_WUBY_NFC_MASK | EXTPMU_PMU_WUBY_GPIO_MASK);
    reg |= ((1 << wu_src) & (EXTPMU_PMU_WUBY_WUTMR_MASK | EXTPMU_PMU_WUBY_NFC_MASK | EXTPMU_PMU_WUBY_GPIO_MASK));
    REG_W32(EXTPMU_EXT_PMU_WU_REG_ADDR, reg);
}

/**
 * @brief Set the clock source for the external PMU.
 * @param clk_src [0] : 32KHz, [1] : 16KHz
 */
void extpmu_clk_source_sel_er8130(uint8_t clk_src)
{
    uint32_t reg = REG_R32(EXTPMU_EXT_PMU_CFG_REG_ADDR);

    // set clock source
    reg &= ~EXTPMU_PMU_CLK_SRC_MASK;
    reg |= (clk_src << EXTPMU_PMU_CLK_SRC_POS) & EXTPMU_PMU_CLK_SRC_MASK;

    // write back to register
    REG_W32(EXTPMU_EXT_PMU_CFG_REG_ADDR, reg);
}

void extpmu_disable_extpmu_shutdown_control_for_32k_16k_osc(void)
{
    uint32_t reg;

    // addr:0x4000A008
    reg  = REG_R32(EXTPMU_EXT_PMU_CFG_REG_ADDR);
    reg &= ~(EXTPMU_PMU_32K_SHDN_EN_MASK | EXTPMU_PMU_16K_SHDN_EN_MASK);
    REG_W32(EXTPMU_EXT_PMU_CFG_REG_ADDR, reg);
}
/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/extpmu_er8130.h"
#include "hal_intf_sys.h"

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

void extpmu_enable_external_pmu_er8130(uint8_t enable)
{
    _extpmu_ana_time_config();
    
    REG_W32(EXTPMU_EXT_PMU_GLO_REG_ADDR, enable);
}

void extpmu_set_wakeup_time_er8130(uint32_t time)
{
    REG_W32(EXTPMU_EXT_PMU_TMR_PROD_REG_ADDR, time);
}

void extpmu_set_sleep_er8130(uint8_t wutmr_en)
{
    uint32_t reg = 0x0;

    //it needs to check status before enter shutdown mode:
    while(reg != 0x00554D50) {
         reg = REG_R32(EXTPMU_EXT_PMU_PERIPH_ID_0_ADDR);
         printf("Check : EXTPMU periph ID\n");
    }    

    reg = 0x0;
    while(reg & 0x00000038) {
        reg = REG_R32(EXTPMU_EXT_PMU_STS_REG_ADDR);
        printf("Check : EXTPMU sts\n");
    }

    reg = EXTPMU_SLEEP_EN_MASK;
    if (wutmr_en) {
        reg |= EXTPMU_WUTMR_EN_MASK;
    }

    REG_W32(EXTPMU_EXT_PMU_CTL_REG_ADDR, reg);
}

void extpmu_enable_wakeup_source_er8130(uint8_t wu_src)
{
    uint32_t reg = REG_R32(EXTPMU_EXT_PMU_WU_REG_ADDR);

    reg &= ~(EXTPMU_PMU_WUBY_WUTMR_MASK | EXTPMU_PMU_WUBY_NFC_MASK | EXTPMU_PMU_WUBY_GPIO_MASK);
    //reg |= ((1 << wu_src) & (EXTPMU_PMU_WUBY_WUTMR_MASK | EXTPMU_PMU_WUBY_NFC_MASK | EXTPMU_PMU_WUBY_GPIO_MASK));
    reg |= (wu_src & (EXTPMU_PMU_WUBY_WUTMR_MASK | EXTPMU_PMU_WUBY_NFC_MASK | EXTPMU_PMU_WUBY_GPIO_MASK));
    REG_W32(EXTPMU_EXT_PMU_WU_REG_ADDR, reg);
}

void extpmu_disable_wakeup_source_er8130(uint8_t wu_src)
{
    uint32_t reg = REG_R32(EXTPMU_EXT_PMU_WU_REG_ADDR);

    reg &= (~wu_src);
    REG_W32(EXTPMU_EXT_PMU_WU_REG_ADDR, reg);
}

/**
 * @brief Set the GPIO pin for wakeup source
 * @param gpio_pin GPIO pin number (0-15)
 * @note This function should be called before enabling GPIO wakeup
 */
void extpmu_set_wakeup_gpio_pin_er8130(uint8_t gpio_pin)
{
    uint32_t reg;

    if (gpio_pin >= CHIP_GPIO_NUM) {
        printf("%s: Invalid GPIO pin: %u (valid range: 0-%u)\n",
               __func__, gpio_pin, CHIP_GPIO_NUM - 1);
        return;
    }

    reg = REG_R32(EXTPMU_EXT_PMU_CFG2_REG_ADDR);
    reg &= ~EXTPMU_PMU_WUBY_GPIO_TAR_MASK;
    reg |= (gpio_pin << EXTPMU_PMU_WUBY_GPIO_TAR_POS) & EXTPMU_PMU_WUBY_GPIO_TAR_MASK;
    REG_W32(EXTPMU_EXT_PMU_CFG2_REG_ADDR, reg);
}

/**
 * @brief Get the current GPIO pin configured for wakeup
 * @return GPIO pin number (0-15)
 */
uint8_t extpmu_get_wakeup_gpio_pin_er8130(void)
{
    uint32_t reg;
    uint8_t gpio_pin;

    reg = REG_R32(EXTPMU_EXT_PMU_CFG2_REG_ADDR);
    gpio_pin = (reg & EXTPMU_PMU_WUBY_GPIO_TAR_MASK) >> EXTPMU_PMU_WUBY_GPIO_TAR_POS;

    return gpio_pin;
}

/**
 * @brief Set the clock source for the external PMU.
 * @param clk_src [0] : 32KHz, [1] : 16KHz
 */
void extpmu_clk_source_sel_er8130(uint8_t clk_src)
{
    uint32_t reg;
    uint8_t pmu_clk_src = 1;

    switch(clk_src) {
    case SYS_LFOSC_CLK_SRC_INTERNAL_16K:
        pmu_clk_src = 1;
        break;
    case SYS_LFOSC_CLK_SRC_EXTERNAL_32K:
        pmu_clk_src = 0;
        break;
    case SYS_LFOSC_CLK_SRC_INTERNAL_32K:
        printf("%s: ER8130A did not support Internal 32K clock selecting\n", __func__);
        return;
    default:
        printf("Invalid clk_src: %u\n", clk_src);
        return;
    };
        
    // set clock source
    reg = REG_R32(EXTPMU_EXT_PMU_CFG_REG_ADDR);
    reg &= ~EXTPMU_PMU_CLK_SRC_MASK;
    reg |= (pmu_clk_src << EXTPMU_PMU_CLK_SRC_POS) & EXTPMU_PMU_CLK_SRC_MASK;
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

/**
 * @brief Get the wakeup timer counter value
 * @return uint32_t The current counter value from EXT_PMU_STS3_REG
 */
uint32_t extpmu_get_wutmr_counter_er8130(void)
{
    return REG_R32(EXTPMU_EXT_PMU_STS3_REG_ADDR);
}
/**
 * @brief Get EXTPMU interrupt status
 * @return Interrupt status (1: interrupt occurred, 0: no interrupt)
 */
uint32_t extpmu_get_int_status_er8130(void)
{
    return (EXTPMU->EXT_PMU_INT_ST & EXTPMU_INT_ST_EXT_WAKEUP_MASK);
}

/**
 * @brief Clear EXTPMU interrupt status
 * @note Write 1 to clear
 */
void extpmu_clear_int_status_er8130(void)
{
    EXTPMU->EXT_PMU_INT_ST |= EXTPMU_INT_ST_EXT_WAKEUP_MASK;
}

/**
 * @brief Enable EXTPMU interrupt
 * @note Sets interrupt enable register and enables NVIC
 */
void extpmu_set_int_enable_er8130(void)
{
    EXTPMU->EXT_PMU_INT_EN = EXTPMU_INT_EN_EXT_WAKEUP_MASK;
    NVIC_EnableIRQ((IRQn_Type)EXTPMU_IRQn);
}

/**
 * @brief Disable EXTPMU interrupt
 * @note Clears interrupt enable register and disables NVIC
 */
void extpmu_set_int_disable_er8130(void)
{
    EXTPMU->EXT_PMU_INT_EN = 0;
    NVIC_DisableIRQ((IRQn_Type)EXTPMU_IRQn);
}


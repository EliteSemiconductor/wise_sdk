/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_pwm_slow_0120.xlsx */
/* ================================================================================
 */
/* ================                    PWMSLOW_T ================ */
/* ================================================================================
 */
#ifndef __PWMSLOW_REG_ER8130_H__
#define __PWMSLOW_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief PWMSLOW (PWMSLOW_T)
 */

typedef struct {
    __IO uint32_t PWM_EN;           /*!< PWM_EN Register (0x000)*/
    __IO uint32_t INT_EN;           /*!< INT_EN Register (0x004)*/
    __IO uint32_t RAW_INT_STATUS;   /*!< RAW_INT_STATUS Register (0x008)*/
    __I uint32_t MASKED_INT_STATUS; /*!< MASKED_INT_STATUS Register (0x00C)*/
    __IO uint32_t PWM_MODE_SEL;     /*!< PWM_MODE_SEL Register (0x010)*/
    __IO uint32_t PWM_PERIOD_0;     /*!< PWM_PERIOD_0 Register (0x014)*/
    __IO uint32_t HIGH_PERIOD_0;    /*!< HIGH_PERIOD_0 Register (0x018)*/
    __IO uint32_t SHOT_NUM_0;       /*!< SHOT_NUM_0 Register (0x01C)*/
    __O uint32_t ONESHOT_TRIGGER;   /*!< ONESHOT_TRIGGER Register (0x020)*/
    __I uint32_t RESERVED0[1007];
    __I uint32_t PERIPH_ID_LEN_REG; /*!< PERIPH_ID_LEN_REG Register (0xFE0)*/
    __I uint32_t RESERVED1[3];
    __IO uint32_t PERIPH_ID0_REG; /*!< PERIPH_ID0_REG Register (0xFF0)*/
} PWMSLOW_T;

#define PWMSLOW_PWM_EN_ADDR (uint32_t)&(PWMSLOW->PWM_EN)
#define PWMSLOW_PWM_EN_ADDR (uint32_t)&(PWMSLOW->PWM_EN)
#define PWMSLOW_PWM_EN_POS (0) /*< bit[0]      */
#define PWMSLOW_PWM_EN_MASK (0x1ul << PWMSLOW_PWM_EN_POS)
#define PWMSLOW_OUT_EN_0_ADDR (uint32_t)&(PWMSLOW->PWM_EN)
#define PWMSLOW_OUT_EN_0_POS (1) /*< bit[1]      */
#define PWMSLOW_OUT_EN_0_MASK (0x1ul << PWMSLOW_OUT_EN_0_POS)

#define PWMSLOW_INT_EN_ADDR (uint32_t)&(PWMSLOW->INT_EN)
#define PWMSLOW_INT_EN_0_ADDR (uint32_t)&(PWMSLOW->INT_EN)
#define PWMSLOW_INT_EN_0_POS (0) /*< bit[0]      */
#define PWMSLOW_INT_EN_0_MASK (0x1ul << PWMSLOW_INT_EN_0_POS)

#define PWMSLOW_RAW_INT_STATUS_ADDR (uint32_t)&(PWMSLOW->RAW_INT_STATUS)
#define PWMSLOW_INT_ST_0_ADDR (uint32_t)&(PWMSLOW->RAW_INT_STATUS)
#define PWMSLOW_INT_ST_0_POS (0) /*< bit[0]      */
#define PWMSLOW_INT_ST_0_MASK (0x1ul << PWMSLOW_INT_ST_0_POS)

#define PWMSLOW_MASKED_INT_STATUS_ADDR (uint32_t)&(PWMSLOW->MASKED_INT_STATUS)
#define PWMSLOW_MASKED_INT_ST_0_ADDR (uint32_t)&(PWMSLOW->MASKED_INT_STATUS)
#define PWMSLOW_MASKED_INT_ST_0_POS (0) /*< bit[0]      */
#define PWMSLOW_MASKED_INT_ST_0_MASK (0x1ul << PWMSLOW_MASKED_INT_ST_0_POS)

#define PWMSLOW_PWM_MODE_SEL_ADDR (uint32_t)&(PWMSLOW->PWM_MODE_SEL)
#define PWMSLOW_PWM_MODE_0_ADDR (uint32_t)&(PWMSLOW->PWM_MODE_SEL)
#define PWMSLOW_PWM_MODE_0_POS (0) /*< bit[0]      */
#define PWMSLOW_PWM_MODE_0_MASK (0x1ul << PWMSLOW_PWM_MODE_0_POS)
#define PWMSLOW_ONESHOT_0_ADDR (uint32_t)&(PWMSLOW->PWM_MODE_SEL)
#define PWMSLOW_ONESHOT_0_POS (1) /*< bit[1]      */
#define PWMSLOW_ONESHOT_0_MASK (0x1ul << PWMSLOW_ONESHOT_0_POS)
#define PWMSLOW_PARK_0_ADDR (uint32_t)&(PWMSLOW->PWM_MODE_SEL)
#define PWMSLOW_PARK_0_POS (2) /*< bit[2]      */
#define PWMSLOW_PARK_0_MASK (0x1ul << PWMSLOW_PARK_0_POS)
#define PWMSLOW_INVERSE_0_ADDR (uint32_t)&(PWMSLOW->PWM_MODE_SEL)
#define PWMSLOW_INVERSE_0_POS (3) /*< bit[3]      */
#define PWMSLOW_INVERSE_0_MASK (0x1ul << PWMSLOW_INVERSE_0_POS)

#define PWMSLOW_PWM_PERIOD_0_ADDR (uint32_t)&(PWMSLOW->PWM_PERIOD_0)
#define PWMSLOW_HIGH_PERIOD_0_ADDR (uint32_t)&(PWMSLOW->HIGH_PERIOD_0)
#define PWMSLOW_SHOT_NUM_0_ADDR (uint32_t)&(PWMSLOW->SHOT_NUM_0)
#define PWMSLOW_SHOT_NUM_0_ADDR (uint32_t)&(PWMSLOW->SHOT_NUM_0)
#define PWMSLOW_SHOT_NUM_0_POS (0) /*< bit[7:0]    */
#define PWMSLOW_SHOT_NUM_0_MASK (0xFFul << PWMSLOW_SHOT_NUM_0_POS)

#define PWMSLOW_ONESHOT_TRIGGER_ADDR (uint32_t)&(PWMSLOW->ONESHOT_TRIGGER)
#define PWMSLOW_W1T_ONESHOT_TRIGGER_0_ADDR (uint32_t)&(PWMSLOW->ONESHOT_TRIGGER)
#define PWMSLOW_W1T_ONESHOT_TRIGGER_0_POS (0) /*< bit[0]      */
#define PWMSLOW_W1T_ONESHOT_TRIGGER_0_MASK                                     \
    (0x1ul << PWMSLOW_W1T_ONESHOT_TRIGGER_0_POS)

#define PWMSLOW_PERIPH_ID_LEN_REG_ADDR (uint32_t)&(PWMSLOW->PERIPH_ID_LEN_REG)
#define PWMSLOW_PERIPH_ID_LEN_ADDR (uint32_t)&(PWMSLOW->PERIPH_ID_LEN_REG)
#define PWMSLOW_PERIPH_ID_LEN_POS (0) /*< bit[3:0]    */
#define PWMSLOW_PERIPH_ID_LEN_MASK (0xFul << PWMSLOW_PERIPH_ID_LEN_POS)

#define PWMSLOW_PERIPH_ID0_REG_ADDR (uint32_t)&(PWMSLOW->PERIPH_ID0_REG)
#define PWMSLOW_BASE 0x40017000UL
#define PWMSLOW ((PWMSLOW_T *)PWMSLOW_BASE)

#endif /* __PWMSLOW_REG_ER8130_H__ */

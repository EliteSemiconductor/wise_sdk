/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_pwm_0125 */
/* ================================================================================
 */
/* ================                      PWM_T ================ */
/* ================================================================================
 */
#ifndef __PWM_REG_ER8130_H__
#define __PWM_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief PWM (PWM_T)
 */

typedef struct {                    /*!< PWM_T Structure*/
    __IO uint32_t EN;               /*!< EN Register (0x000)*/
    __IO uint32_t INT_EN;           /*!< INT_EN Register (0x004)*/
    __IO uint32_t RAW_INT_STATUS;   /*!< RAW_INT_STATUS Register (0x008)*/
    __I uint32_t MASKED_INT_STATUS; /*!< MASKED_INT_STATUS Register (0x00C)*/
    __IO uint32_t MODE_SEL;         /*!< MODE_SEL Register (0x010)*/
    __IO uint32_t PERIOD_0;         /*!< PERIOD_0 Register (0x014)*/
    __IO uint32_t HIGH_PERIOD_0;    /*!< HIGH_PERIOD_0 Register (0x018)*/
    __IO uint32_t SHOT_NUM_0;       /*!< SHOT_NUM_0 Register (0x01C)*/
    __IO uint32_t PERIOD_1;         /*!< PERIOD_1 Register (0x020)*/
    __IO uint32_t HIGH_PERIOD_1;    /*!< HIGH_PERIOD_1 Register (0x024)*/
    __IO uint32_t SHOT_NUM_1;       /*!< SHOT_NUM_1 Register (0x028)*/
    __IO uint32_t PERIOD_2;         /*!< PERIOD_2 Register (0x02C)*/
    __IO uint32_t HIGH_PERIOD_2;    /*!< HIGH_PERIOD_2 Register (0x030)*/
    __IO uint32_t SHOT_NUM_2;       /*!< SHOT_NUM_2 Register (0x034)*/
    __IO uint32_t PERIOD_3;         /*!< PERIOD_3 Register (0x038)*/
    __IO uint32_t HIGH_PERIOD_3;    /*!< HIGH_PERIOD_3 Register (0x03C)*/
    __IO uint32_t SHOT_NUM_3;       /*!< SHOT_NUM_3 Register (0x040)*/
    __IO uint32_t PERIOD_4;         /*!< PERIOD_4 Register (0x044)*/
    __IO uint32_t HIGH_PERIOD_4;    /*!< HIGH_PERIOD_4 Register (0x048)*/
    __IO uint32_t SHOT_NUM_4;       /*!< SHOT_NUM_4 Register (0x04C)*/
    __IO uint32_t PERIOD_5;         /*!< PERIOD_5 Register (0x050)*/
    __IO uint32_t HIGH_PERIOD_5;    /*!< HIGH_PERIOD_5 Register (0x054)*/
    __IO uint32_t SHOT_NUM_5;       /*!< SHOT_NUM_5 Register (0x058)*/
    __O uint32_t ONESHOT_TRIGGER;   /*!< ONESHOT_TRIGGER Register (0x05C)*/
    __I uint32_t RESERVED0[992];
    __I uint32_t PERIPH_ID_LEN_REG; /*!< PERIPH_ID_LEN_REG Register (0xFE0)*/
    __I uint32_t RESERVED1[3];
    __IO uint32_t PERIPH_ID0_REG; /*!< PERIPH_ID0_REG Register (0xFF0)*/
} PWM_T;

#define PWM_EN_ADDR (uint32_t)&(PWM->EN)
#define PWM_PWM_EN_ADDR (uint32_t)&(PWM->EN)
#define PWM_PWM_EN_POS (0) /*< bit[0]      */
#define PWM_PWM_EN_MASK (0x1ul << PWM_PWM_EN_POS)
#define PWM_OUT_EN_0_ADDR (uint32_t)&(PWM->EN)
#define PWM_OUT_EN_0_POS (1) /*< bit[1]      */
#define PWM_OUT_EN_0_MASK (0x1ul << PWM_OUT_EN_0_POS)
#define PWM_OUT_EN_1_ADDR (uint32_t)&(PWM->EN)
#define PWM_OUT_EN_1_POS (2) /*< bit[2]      */
#define PWM_OUT_EN_1_MASK (0x1ul << PWM_OUT_EN_1_POS)
#define PWM_OUT_EN_2_ADDR (uint32_t)&(PWM->EN)
#define PWM_OUT_EN_2_POS (3) /*< bit[3]      */
#define PWM_OUT_EN_2_MASK (0x1ul << PWM_OUT_EN_2_POS)
#define PWM_OUT_EN_3_ADDR (uint32_t)&(PWM->EN)
#define PWM_OUT_EN_3_POS (4) /*< bit[4]      */
#define PWM_OUT_EN_3_MASK (0x1ul << PWM_OUT_EN_3_POS)
#define PWM_OUT_EN_4_ADDR (uint32_t)&(PWM->EN)
#define PWM_OUT_EN_4_POS (5) /*< bit[5]      */
#define PWM_OUT_EN_4_MASK (0x1ul << PWM_OUT_EN_4_POS)
#define PWM_OUT_EN_5_ADDR (uint32_t)&(PWM->EN)
#define PWM_OUT_EN_5_POS (6) /*< bit[6]      */
#define PWM_OUT_EN_5_MASK (0x1ul << PWM_OUT_EN_5_POS)

#define PWM_INT_EN_ADDR (uint32_t)&(PWM->INT_EN)
#define PWM_INT_EN_0_ADDR (uint32_t)&(PWM->INT_EN)
#define PWM_INT_EN_0_POS (0) /*< bit[0]      */
#define PWM_INT_EN_0_MASK (0x1ul << PWM_INT_EN_0_POS)
#define PWM_INT_EN_1_ADDR (uint32_t)&(PWM->INT_EN)
#define PWM_INT_EN_1_POS (1) /*< bit[1]      */
#define PWM_INT_EN_1_MASK (0x1ul << PWM_INT_EN_1_POS)
#define PWM_INT_EN_2_ADDR (uint32_t)&(PWM->INT_EN)
#define PWM_INT_EN_2_POS (2) /*< bit[2]      */
#define PWM_INT_EN_2_MASK (0x1ul << PWM_INT_EN_2_POS)
#define PWM_INT_EN_3_ADDR (uint32_t)&(PWM->INT_EN)
#define PWM_INT_EN_3_POS (3) /*< bit[3]      */
#define PWM_INT_EN_3_MASK (0x1ul << PWM_INT_EN_3_POS)
#define PWM_INT_EN_4_ADDR (uint32_t)&(PWM->INT_EN)
#define PWM_INT_EN_4_POS (4) /*< bit[4]      */
#define PWM_INT_EN_4_MASK (0x1ul << PWM_INT_EN_4_POS)
#define PWM_INT_EN_5_ADDR (uint32_t)&(PWM->INT_EN)
#define PWM_INT_EN_5_POS (5) /*< bit[5]      */
#define PWM_INT_EN_5_MASK (0x1ul << PWM_INT_EN_5_POS)

#define PWM_RAW_INT_STATUS_ADDR (uint32_t)&(PWM->RAW_INT_STATUS)
#define PWM_INT_ST_0_ADDR (uint32_t)&(PWM->RAW_INT_STATUS)
#define PWM_INT_ST_0_POS (0) /*< bit[0]      */
#define PWM_INT_ST_0_MASK (0x1ul << PWM_INT_ST_0_POS)
#define PWM_INT_ST_1_ADDR (uint32_t)&(PWM->RAW_INT_STATUS)
#define PWM_INT_ST_1_POS (1) /*< bit[1]      */
#define PWM_INT_ST_1_MASK (0x1ul << PWM_INT_ST_1_POS)
#define PWM_INT_ST_2_ADDR (uint32_t)&(PWM->RAW_INT_STATUS)
#define PWM_INT_ST_2_POS (2) /*< bit[2]      */
#define PWM_INT_ST_2_MASK (0x1ul << PWM_INT_ST_2_POS)
#define PWM_INT_ST_3_ADDR (uint32_t)&(PWM->RAW_INT_STATUS)
#define PWM_INT_ST_3_POS (3) /*< bit[3]      */
#define PWM_INT_ST_3_MASK (0x1ul << PWM_INT_ST_3_POS)
#define PWM_INT_ST_4_ADDR (uint32_t)&(PWM->RAW_INT_STATUS)
#define PWM_INT_ST_4_POS (4) /*< bit[4]      */
#define PWM_INT_ST_4_MASK (0x1ul << PWM_INT_ST_4_POS)
#define PWM_INT_ST_5_ADDR (uint32_t)&(PWM->RAW_INT_STATUS)
#define PWM_INT_ST_5_POS (5) /*< bit[5]      */
#define PWM_INT_ST_5_MASK (0x1ul << PWM_INT_ST_5_POS)

#define PWM_MASKED_INT_STATUS_ADDR (uint32_t)&(PWM->MASKED_INT_STATUS)
#define PWM_MASKED_INT_ST_0_ADDR (uint32_t)&(PWM->MASKED_INT_STATUS)
#define PWM_MASKED_INT_ST_0_POS (0) /*< bit[0]      */
#define PWM_MASKED_INT_ST_0_MASK (0x1ul << PWM_MASKED_INT_ST_0_POS)
#define PWM_MASKED_INT_ST_1_ADDR (uint32_t)&(PWM->MASKED_INT_STATUS)
#define PWM_MASKED_INT_ST_1_POS (1) /*< bit[1]      */
#define PWM_MASKED_INT_ST_1_MASK (0x1ul << PWM_MASKED_INT_ST_1_POS)
#define PWM_MASKED_INT_ST_2_ADDR (uint32_t)&(PWM->MASKED_INT_STATUS)
#define PWM_MASKED_INT_ST_2_POS (2) /*< bit[2]      */
#define PWM_MASKED_INT_ST_2_MASK (0x1ul << PWM_MASKED_INT_ST_2_POS)
#define PWM_MASKED_INT_ST_3_ADDR (uint32_t)&(PWM->MASKED_INT_STATUS)
#define PWM_MASKED_INT_ST_3_POS (3) /*< bit[3]      */
#define PWM_MASKED_INT_ST_3_MASK (0x1ul << PWM_MASKED_INT_ST_3_POS)
#define PWM_MASKED_INT_ST_4_ADDR (uint32_t)&(PWM->MASKED_INT_STATUS)
#define PWM_MASKED_INT_ST_4_POS (4) /*< bit[4]      */
#define PWM_MASKED_INT_ST_4_MASK (0x1ul << PWM_MASKED_INT_ST_4_POS)
#define PWM_MASKED_INT_ST_5_ADDR (uint32_t)&(PWM->MASKED_INT_STATUS)
#define PWM_MASKED_INT_ST_5_POS (5) /*< bit[5]      */
#define PWM_MASKED_INT_ST_5_MASK (0x1ul << PWM_MASKED_INT_ST_5_POS)

#define PWM_MODE_SEL_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PWM_MODE_0_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PWM_MODE_0_POS (0) /*< bit[0]      */
#define PWM_PWM_MODE_0_MASK (0x1ul << PWM_PWM_MODE_0_POS)
#define PWM_PWM_MODE_1_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PWM_MODE_1_POS (1) /*< bit[1]      */
#define PWM_PWM_MODE_1_MASK (0x1ul << PWM_PWM_MODE_1_POS)
#define PWM_PWM_MODE_2_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PWM_MODE_2_POS (2) /*< bit[2]      */
#define PWM_PWM_MODE_2_MASK (0x1ul << PWM_PWM_MODE_2_POS)
#define PWM_PWM_MODE_3_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PWM_MODE_3_POS (3) /*< bit[3]      */
#define PWM_PWM_MODE_3_MASK (0x1ul << PWM_PWM_MODE_3_POS)
#define PWM_PWM_MODE_4_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PWM_MODE_4_POS (4) /*< bit[4]      */
#define PWM_PWM_MODE_4_MASK (0x1ul << PWM_PWM_MODE_4_POS)
#define PWM_PWM_MODE_5_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PWM_MODE_5_POS (5) /*< bit[5]      */
#define PWM_PWM_MODE_5_MASK (0x1ul << PWM_PWM_MODE_5_POS)
#define PWM_ONESHOT_0_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_ONESHOT_0_POS (6) /*< bit[6]      */
#define PWM_ONESHOT_0_MASK (0x1ul << PWM_ONESHOT_0_POS)
#define PWM_ONESHOT_1_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_ONESHOT_1_POS (7) /*< bit[7]      */
#define PWM_ONESHOT_1_MASK (0x1ul << PWM_ONESHOT_1_POS)
#define PWM_ONESHOT_2_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_ONESHOT_2_POS (8) /*< bit[8]      */
#define PWM_ONESHOT_2_MASK (0x1ul << PWM_ONESHOT_2_POS)
#define PWM_ONESHOT_3_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_ONESHOT_3_POS (9) /*< bit[9]      */
#define PWM_ONESHOT_3_MASK (0x1ul << PWM_ONESHOT_3_POS)
#define PWM_ONESHOT_4_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_ONESHOT_4_POS (10) /*< bit[10]     */
#define PWM_ONESHOT_4_MASK (0x1ul << PWM_ONESHOT_4_POS)
#define PWM_ONESHOT_5_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_ONESHOT_5_POS (11) /*< bit[11]     */
#define PWM_ONESHOT_5_MASK (0x1ul << PWM_ONESHOT_5_POS)
#define PWM_PARK_0_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PARK_0_POS (12) /*< bit[12]     */
#define PWM_PARK_0_MASK (0x1ul << PWM_PARK_0_POS)
#define PWM_PARK_1_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PARK_1_POS (13) /*< bit[13]     */
#define PWM_PARK_1_MASK (0x1ul << PWM_PARK_1_POS)
#define PWM_PARK_2_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PARK_2_POS (14) /*< bit[14]     */
#define PWM_PARK_2_MASK (0x1ul << PWM_PARK_2_POS)
#define PWM_PARK_3_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PARK_3_POS (15) /*< bit[15]     */
#define PWM_PARK_3_MASK (0x1ul << PWM_PARK_3_POS)
#define PWM_PARK_4_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PARK_4_POS (16) /*< bit[16]     */
#define PWM_PARK_4_MASK (0x1ul << PWM_PARK_4_POS)
#define PWM_PARK_5_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_PARK_5_POS (17) /*< bit[17]     */
#define PWM_PARK_5_MASK (0x1ul << PWM_PARK_5_POS)
#define PWM_INVERSE_0_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_INVERSE_0_POS (18) /*< bit[18]     */
#define PWM_INVERSE_0_MASK (0x1ul << PWM_INVERSE_0_POS)
#define PWM_INVERSE_1_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_INVERSE_1_POS (19) /*< bit[19]     */
#define PWM_INVERSE_1_MASK (0x1ul << PWM_INVERSE_1_POS)
#define PWM_INVERSE_2_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_INVERSE_2_POS (20) /*< bit[20]     */
#define PWM_INVERSE_2_MASK (0x1ul << PWM_INVERSE_2_POS)
#define PWM_INVERSE_3_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_INVERSE_3_POS (21) /*< bit[21]     */
#define PWM_INVERSE_3_MASK (0x1ul << PWM_INVERSE_3_POS)
#define PWM_INVERSE_4_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_INVERSE_4_POS (22) /*< bit[22]     */
#define PWM_INVERSE_4_MASK (0x1ul << PWM_INVERSE_4_POS)
#define PWM_INVERSE_5_ADDR (uint32_t)&(PWM->MODE_SEL)
#define PWM_INVERSE_5_POS (23) /*< bit[23]     */
#define PWM_INVERSE_5_MASK (0x1ul << PWM_INVERSE_5_POS)

#define PWM_PERIOD_0_ADDR (uint32_t)&(PWM->PERIOD_0)
#define PWM_HIGH_PERIOD_0_ADDR (uint32_t)&(PWM->HIGH_PERIOD_0)
#define PWM_SHOT_NUM_0_ADDR (uint32_t)&(PWM->SHOT_NUM_0)
#define PWM_SHOT_NUM_0_POS (0) /*< bit[7:0]    */
#define PWM_SHOT_NUM_0_MASK (0xFFul << PWM_SHOT_NUM_0_POS)

#define PWM_PERIOD_1_ADDR (uint32_t)&(PWM->PERIOD_1)
#define PWM_HIGH_PERIOD_1_ADDR (uint32_t)&(PWM->HIGH_PERIOD_1)
#define PWM_SHOT_NUM_1_ADDR (uint32_t)&(PWM->SHOT_NUM_1)
#define PWM_SHOT_NUM_1_POS (0) /*< bit[7:0]    */
#define PWM_SHOT_NUM_1_MASK (0xFFul << PWM_SHOT_NUM_1_POS)

#define PWM_PERIOD_2_ADDR (uint32_t)&(PWM->PERIOD_2)
#define PWM_HIGH_PERIOD_2_ADDR (uint32_t)&(PWM->HIGH_PERIOD_2)
#define PWM_SHOT_NUM_2_ADDR (uint32_t)&(PWM->SHOT_NUM_2)
#define PWM_SHOT_NUM_2_POS (0) /*< bit[7:0]    */
#define PWM_SHOT_NUM_2_MASK (0xFFul << PWM_SHOT_NUM_2_POS)

#define PWM_PERIOD_3_ADDR (uint32_t)&(PWM->PERIOD_3)
#define PWM_HIGH_PERIOD_3_ADDR (uint32_t)&(PWM->HIGH_PERIOD_3)
#define PWM_SHOT_NUM_3_ADDR (uint32_t)&(PWM->SHOT_NUM_3)
#define PWM_SHOT_NUM_3_POS (0) /*< bit[7:0]    */
#define PWM_SHOT_NUM_3_MASK (0xFFul << PWM_SHOT_NUM_3_POS)

#define PWM_PERIOD_4_ADDR (uint32_t)&(PWM->PERIOD_4)
#define PWM_HIGH_PERIOD_4_ADDR (uint32_t)&(PWM->HIGH_PERIOD_4)
#define PWM_SHOT_NUM_4_ADDR (uint32_t)&(PWM->SHOT_NUM_4)
#define PWM_SHOT_NUM_4_POS (0) /*< bit[7:0]    */
#define PWM_SHOT_NUM_4_MASK (0xFFul << PWM_SHOT_NUM_4_POS)

#define PWM_PERIOD_5_ADDR (uint32_t)&(PWM->PERIOD_5)
#define PWM_HIGH_PERIOD_5_ADDR (uint32_t)&(PWM->HIGH_PERIOD_5)
#define PWM_SHOT_NUM_5_ADDR (uint32_t)&(PWM->SHOT_NUM_5)
#define PWM_SHOT_NUM_5_POS (0) /*< bit[7:0]    */
#define PWM_SHOT_NUM_5_MASK (0xFFul << PWM_SHOT_NUM_5_POS)

#define PWM_ONESHOT_TRIGGER_ADDR (uint32_t)&(PWM->ONESHOT_TRIGGER)
#define PWM_W1T_ONESHOT_TRIGGER_ADDR (uint32_t)&(PWM->ONESHOT_TRIGGER)
#define PWM_W1T_ONESHOT_TRIGGER_POS (0) /*< bit[0]      */
#define PWM_W1T_ONESHOT_TRIGGER_MASK (0x1ul << PWM_W1T_ONESHOT_TRIGGER_POS)

#define PWM_PERIPH_ID_LEN_REG_ADDR (uint32_t)&(PWM->PERIPH_ID_LEN_REG)
#define PWM_PERIPH_ID_LEN_ADDR (uint32_t)&(PWM->PERIPH_ID_LEN_REG)
#define PWM_PERIPH_ID_LEN_POS (0) /*< bit[3:0]    */
#define PWM_PERIPH_ID_LEN_MASK (0xFul << PWM_PERIPH_ID_LEN_POS)

#define PWM_PERIPH_ID0_REG_ADDR (uint32_t)&(PWM->PERIPH_ID0_REG)
#define PWM_BASE 0x4003A000UL
#define PWM ((PWM_T *)PWM_BASE)

#endif /* __PWM_REG_ER8130_H__ */

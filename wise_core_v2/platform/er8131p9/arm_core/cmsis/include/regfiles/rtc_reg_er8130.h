/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_rtc_soc_b1_20240619.xlsx */
/* ================================================================================
 */
/* ================                      RTC_T ================ */
/* ================================================================================
 */
#ifndef __RTC_REG_ER8130_H__
#define __RTC_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"
/**
 * @brief RTC (RTC_T)
 */
typedef struct {
    __I uint32_t RESERVED0[4];

    union {
        __IO uint32_t CNTR; /*!< Counter Register */

        struct {
            __IO uint32_t SEC  : 6;  /*!< Second field of current time;
                                         range: 0 ~ 59 */
            __IO uint32_t MIN  : 6;  /*!< Minute field of current time;
                                         range: 0 ~ 59 */
            __IO uint32_t HOUR : 5;  /*!< Hour field of current time;
                                         range: 0 ~ 23 */
            __IO uint32_t DAY  : 15; /*!< Day passed after RTC enabled */
        } CNTR_b;                    /*!< BitSize */
    };

    __IO uint32_t ALARM0; /*!< Alarm Register */

    union {
        __IO uint32_t CTRL; /*!< Control Register */

        struct {
            __IO uint32_t RTC_EN        : 1; /*!< Enable RTC */
            __IO uint32_t ALARM0_WAKEUP : 1; /*!< Enable alarm wakeup signal */
            __IO uint32_t ALARM0_INT    : 1; /*!< Enable alarm interrupt */
            __IO uint32_t DAY  : 1; /*!< Enable day interrupt; day interrupt
                                         is generated when hour of RTC time
                                         changes from 23 to 0 */
            __IO uint32_t HOUR : 1; /*!< Enable hour interrupt; hour interrupt
                                         is generated when minute of RTC time
                                         changes from 59 to 0 */
            __IO uint32_t MIN  : 1; /*!< Enable minute interrupt; minute
                                       interrupt  is generated when second of RTC
                                       time  changes from 59 to 0 */
            __IO uint32_t
                SEC : 1; /*!< Enable second interrupt; second interrupt
                              is generated when one second passed */
            __IO uint32_t HSEC          : 1; /*!< Enable half-second interrupt;
                                                half-second          interrupt is generated when
                                                half-second          passed */
            __IO uint32_t ALARM1_WAKEUP : 1; /*!< Enable alarm wakeup signal */
            __IO uint32_t ALARM2_WAKEUP : 1; /*!< Enable alarm wakeup signal */
            __IO uint32_t ALARM3_WAKEUP : 1; /*!< Enable alarm wakeup signal */
            __IO uint32_t ALARM4_WAKEUP : 1; /*!< Enable alarm wakeup signal */
            __IO uint32_t ALARM5_WAKEUP : 1; /*!< Enable alarm wakeup signal */
            __IO uint32_t ALARM1_INT    : 1; /*!< Enable alarm interrupt */
            __IO uint32_t ALARM2_INT    : 1; /*!< Enable alarm interrupt */
            __IO uint32_t ALARM3_INT    : 1; /*!< Enable alarm interrupt */
            __IO uint32_t ALARM4_INT    : 1; /*!< Enable alarm interrupt */
            __IO uint32_t ALARM5_INT    : 1; /*!< Enable alarm interrupt */
        } CTRL_b;                            /*!< BitSize */
    };

    union {
        __IO uint32_t STATUS; /*!< Status Register */

        struct {
            uint32_t : 2; /*!< Reserved */
            __IO uint32_t
                ALARM_INT0 : 1; /*!< Alarm interrupt status, write 1 to clear */
            __IO uint32_t
                DAY : 1; /*!< Day interrupt status, write 1 to clear */
            __IO uint32_t
                HOUR : 1; /*!< Hour interrupt status, write 1 to clear */
            __IO uint32_t
                MIN : 1; /*!< Minute interrupt status, write 1 to clear */
            __IO uint32_t
                SEC : 1; /*!< Second interrupt status, write 1 to clear */
            __IO uint32_t
                HSEC : 1; /*!< Half-second interrupt status, write 1 to clear */
            __IO uint32_t
                ALARM_INT1 : 1; /*!< Alarm interrupt status, write 1 to clear */
            __IO uint32_t
                ALARM_INT2 : 1; /*!< Alarm interrupt status, write 1 to clear */
            __IO uint32_t
                ALARM_INT3 : 1; /*!< Alarm interrupt status, write 1 to clear */
            __IO uint32_t
                ALARM_INT4 : 1; /*!< Alarm interrupt status, write 1 to clear */
            __IO uint32_t
                ALARM_INT5 : 1; /*!< Alarm interrupt status, write 1 to clear */
            uint32_t       : 3; /*!< Reserved */
            __IO uint32_t
                WRITEDONE : 1; /*!< Indicates the synchronization progress of
                                    RTC register updates. This bit becomes zero
                                    when any of RTC control registers
                                    (Counter, Alarm, Control) are updated.
                                    It returns to one when all prior updates
                                    have been successfully synchronized to
                                    the RTC clock domain. */
        } STATUS_b;            /*!< BitSize */
    };

    __IO uint32_t ALARM1; /*!< Alarm Register */
    __IO uint32_t ALARM2; /*!< Alarm Register */
    __IO uint32_t ALARM3; /*!< Alarm Register */
    __IO uint32_t ALARM4; /*!< Alarm Register */
    __IO uint32_t ALARM5; /*!< Alarm Register */
} RTC_T;

#define RTC_ID_ADDR (uint32_t)&(RTC->ID)
#define RTC_REV_MINOR_ADDR (uint32_t)&(RTC->ID)
#define RTC_REV_MINOR_POS (0) /*< bit[3:0]    */
#define RTC_REV_MINOR_MASK (0xFul << RTC_REV_MINOR_POS)
#define RTC_REV_MAJOR_ADDR (uint32_t)&(RTC->ID)
#define RTC_REV_MAJOR_POS (4) /*< bit[11:4]   */
#define RTC_REV_MAJOR_MASK (0xFFul << RTC_REV_MAJOR_POS)
#define RTC_ID_NUM_ADDR (uint32_t)&(RTC->ID)
#define RTC_ID_NUM_POS (12) /*< bit[31:12]  */
#define RTC_ID_NUM_MASK (0xFFFFFul << RTC_ID_NUM_POS)

#define RTC_CNTR_ADDR (uint32_t)&(RTC->CNTR)
#define RTC_CNTR_SEC_ADDR (uint32_t)&(RTC->CNTR)
#define RTC_CNTR_SEC_POS (0) /*< bit[5:0]    */
#define RTC_CNTR_SEC_MASK (0x3Ful << RTC_CNTR_SEC_POS)
#define RTC_CNTR_MIN_ADDR (uint32_t)&(RTC->CNTR)
#define RTC_CNTR_MIN_POS (6) /*< bit[11:6]   */
#define RTC_CNTR_MIN_MASK (0x3Ful << RTC_CNTR_MIN_POS)
#define RTC_CNTR_HOUR_ADDR (uint32_t)&(RTC->CNTR)
#define RTC_CNTR_HOUR_POS (12) /*< bit[16:12]  */
#define RTC_CNTR_HOUR_MASK (0x1Ful << RTC_CNTR_HOUR_POS)
#define RTC_CNTR_DAY_ADDR (uint32_t)&(RTC->CNTR)
#define RTC_CNTR_DAY_POS (17) /*< bit[31:17]  */
#define RTC_CNTR_DAY_MASK (0x7FFFul << RTC_CNTR_DAY_POS)

#define RTC_ALARM_ADDR (uint32_t)&(RTC->ALARM)
#define RTC_ALARM_SEC_ADDR (uint32_t)&(RTC->ALARM)
#define RTC_ALARM_SEC_POS (0) /*< bit[5:0]    */
#define RTC_ALARM_SEC_MASK (0x3Ful << RTC_ALARM_SEC_POS)
#define RTC_ALARM_MIN_ADDR (uint32_t)&(RTC->ALARM)
#define RTC_ALARM_MIN_POS (6) /*< bit[11:6]   */
#define RTC_ALARM_MIN_MASK (0x3Ful << RTC_ALARM_MIN_POS)
#define RTC_ALARM_HOUR_ADDR (uint32_t)&(RTC->ALARM)
#define RTC_ALARM_HOUR_POS (12) /*< bit[16:12]  */
#define RTC_ALARM_HOUR_MASK (0x1Ful << RTC_ALARM_HOUR_POS)

#define RTC_CTRL_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_RTC_EN_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_RTC_EN_POS (0) /*< bit[0]      */
#define RTC_RTC_EN_MASK (0x1ul << RTC_RTC_EN_POS)
#define RTC_ALARM_WAKEUP_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_ALARM_WAKEUP_POS (1) /*< bit[1]      */
#define RTC_ALARM_WAKEUP_MASK (0x1ul << RTC_ALARM_WAKEUP_POS)
#define RTC_CTRL_ALARM_INT_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_ALARM_INT_POS (2) /*< bit[2]      */
#define RTC_CTRL_ALARM_INT_MASK (0x1ul << RTC_CTRL_ALARM_INT_POS)
#define RTC_CTRL_DAY_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_DAY_POS (3) /*< bit[3]      */
#define RTC_CTRL_DAY_MASK (0x1ul << RTC_CTRL_DAY_POS)
#define RTC_CTRL_HOUR_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_HOUR_POS (4) /*< bit[4]      */
#define RTC_CTRL_HOUR_MASK (0x1ul << RTC_CTRL_HOUR_POS)
#define RTC_CTRL_MIN_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_MIN_POS (5) /*< bit[5]      */
#define RTC_CTRL_MIN_MASK (0x1ul << RTC_CTRL_MIN_POS)
#define RTC_CTRL_SEC_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_SEC_POS (6) /*< bit[6]      */
#define RTC_CTRL_SEC_MASK (0x1ul << RTC_CTRL_SEC_POS)
#define RTC_CTRL_HSEC_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_HSEC_POS (7) /*< bit[7]      */
#define RTC_CTRL_HSEC_MASK (0x1ul << RTC_CTRL_HSEC_POS)
#define RTC_ALARM1_WAKEUP_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_ALARM1_WAKEUP_POS (8) /*< bit[8]      */
#define RTC_ALARM1_WAKEUP_MASK (0x1ul << RTC_ALARM1_WAKEUP_POS)
#define RTC_ALARM2_WAKEUP_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_ALARM2_WAKEUP_POS (9) /*< bit[9]      */
#define RTC_ALARM2_WAKEUP_MASK (0x1ul << RTC_ALARM2_WAKEUP_POS)
#define RTC_ALARM3_WAKEUP_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_ALARM3_WAKEUP_POS (10) /*< bit[10]     */
#define RTC_ALARM3_WAKEUP_MASK (0x1ul << RTC_ALARM3_WAKEUP_POS)
#define RTC_ALARM4_WAKEUP_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_ALARM4_WAKEUP_POS (11) /*< bit[11]     */
#define RTC_ALARM4_WAKEUP_MASK (0x1ul << RTC_ALARM4_WAKEUP_POS)
#define RTC_ALARM5_WAKEUP_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_ALARM5_WAKEUP_POS (12) /*< bit[12]     */
#define RTC_ALARM5_WAKEUP_MASK (0x1ul << RTC_ALARM5_WAKEUP_POS)
#define RTC_CTRL_ALARM1_INT_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_ALARM1_INT_POS (13) /*< bit[13]     */
#define RTC_CTRL_ALARM1_INT_MASK (0x1ul << RTC_CTRL_ALARM1_INT_POS)
#define RTC_CTRL_ALARM2_INT_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_ALARM2_INT_POS (14) /*< bit[14]     */
#define RTC_CTRL_ALARM2_INT_MASK (0x1ul << RTC_CTRL_ALARM2_INT_POS)
#define RTC_CTRL_ALARM3_INT_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_ALARM3_INT_POS (15) /*< bit[15]     */
#define RTC_CTRL_ALARM3_INT_MASK (0x1ul << RTC_CTRL_ALARM3_INT_POS)
#define RTC_CTRL_ALARM4_INT_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_ALARM4_INT_POS (16) /*< bit[16]     */
#define RTC_CTRL_ALARM4_INT_MASK (0x1ul << RTC_CTRL_ALARM4_INT_POS)
#define RTC_CTRL_ALARM5_INT_ADDR (uint32_t)&(RTC->CTRL)
#define RTC_CTRL_ALARM5_INT_POS (17) /*< bit[17]     */
#define RTC_CTRL_ALARM5_INT_MASK (0x1ul << RTC_CTRL_ALARM5_INT_POS)

#define RTC_STS_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_ALARM_INT_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_ALARM_INT_POS (2) /*< bit[2]      */
#define RTC_STS_ALARM_INT_MASK (0x1ul << RTC_STS_ALARM_INT_POS)
#define RTC_STS_DAY_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_DAY_POS (3) /*< bit[3]      */
#define RTC_STS_DAY_MASK (0x1ul << RTC_STS_DAY_POS)
#define RTC_STS_HOUR_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_HOUR_POS (4) /*< bit[4]      */
#define RTC_STS_HOUR_MASK (0x1ul << RTC_STS_HOUR_POS)
#define RTC_STS_MIN_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_MIN_POS (5) /*< bit[5]      */
#define RTC_STS_MIN_MASK (0x1ul << RTC_STS_MIN_POS)
#define RTC_STS_SEC_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_SEC_POS (6) /*< bit[6]      */
#define RTC_STS_SEC_MASK (0x1ul << RTC_STS_SEC_POS)
#define RTC_STS_HSEC_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_HSEC_POS (7) /*< bit[7]      */
#define RTC_STS_HSEC_MASK (0x1ul << RTC_STS_HSEC_POS)
#define RTC_STS_ALARM1_INT_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_ALARM1_INT_POS (8) /*< bit[8]      */
#define RTC_STS_ALARM1_INT_MASK (0x1ul << RTC_STS_ALARM1_INT_POS)
#define RTC_STS_ALARM2_INT_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_ALARM2_INT_POS (9) /*< bit[9]      */
#define RTC_STS_ALARM2_INT_MASK (0x1ul << RTC_STS_ALARM2_INT_POS)
#define RTC_STS_ALARM3_INT_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_ALARM3_INT_POS (10) /*< bit[10]     */
#define RTC_STS_ALARM3_INT_MASK (0x1ul << RTC_STS_ALARM3_INT_POS)
#define RTC_STS_ALARM4_INT_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_ALARM4_INT_POS (11) /*< bit[11]     */
#define RTC_STS_ALARM4_INT_MASK (0x1ul << RTC_STS_ALARM4_INT_POS)
#define RTC_STS_ALARM5_INT_ADDR (uint32_t)&(RTC->STS)
#define RTC_STS_ALARM5_INT_POS (12) /*< bit[12]     */
#define RTC_STS_ALARM5_INT_MASK (0x1ul << RTC_STS_ALARM5_INT_POS)
#define RTC_WRITE_DONE_ADDR (uint32_t)&(RTC->STS)
#define RTC_WRITE_DONE_POS (16) /*< bit[16]     */
#define RTC_WRITE_DONE_MASK (0x1ul << RTC_WRITE_DONE_POS)

#define RTC_ALARM1_ADDR (uint32_t)&(RTC->ALARM1)
#define RTC_ALARM1_SEC_ADDR (uint32_t)&(RTC->ALARM1)
#define RTC_ALARM1_SEC_POS (0) /*< bit[5:0]    */
#define RTC_ALARM1_SEC_MASK (0x3Ful << RTC_ALARM1_SEC_POS)
#define RTC_ALARM1_MIN_ADDR (uint32_t)&(RTC->ALARM1)
#define RTC_ALARM1_MIN_POS (6) /*< bit[11:6]   */
#define RTC_ALARM1_MIN_MASK (0x3Ful << RTC_ALARM1_MIN_POS)
#define RTC_ALARM1_HOUR_ADDR (uint32_t)&(RTC->ALARM1)
#define RTC_ALARM1_HOUR_POS (12) /*< bit[16:12]  */
#define RTC_ALARM1_HOUR_MASK (0x1Ful << RTC_ALARM1_HOUR_POS)

#define RTC_ALARM2_ADDR (uint32_t)&(RTC->ALARM2)
#define RTC_ALARM2_SEC_ADDR (uint32_t)&(RTC->ALARM2)
#define RTC_ALARM2_SEC_POS (0) /*< bit[5:0]    */
#define RTC_ALARM2_SEC_MASK (0x3Ful << RTC_ALARM2_SEC_POS)
#define RTC_ALARM2_MIN_ADDR (uint32_t)&(RTC->ALARM2)
#define RTC_ALARM2_MIN_POS (6) /*< bit[11:6]   */
#define RTC_ALARM2_MIN_MASK (0x3Ful << RTC_ALARM2_MIN_POS)
#define RTC_ALARM2_HOUR_ADDR (uint32_t)&(RTC->ALARM2)
#define RTC_ALARM2_HOUR_POS (12) /*< bit[16:12]  */
#define RTC_ALARM2_HOUR_MASK (0x1Ful << RTC_ALARM2_HOUR_POS)

#define RTC_ALARM3_ADDR (uint32_t)&(RTC->ALARM3)
#define RTC_ALARM3_SEC_ADDR (uint32_t)&(RTC->ALARM3)
#define RTC_ALARM3_SEC_POS (0) /*< bit[5:0]    */
#define RTC_ALARM3_SEC_MASK (0x3Ful << RTC_ALARM3_SEC_POS)
#define RTC_ALARM3_MIN_ADDR (uint32_t)&(RTC->ALARM3)
#define RTC_ALARM3_MIN_POS (6) /*< bit[11:6]   */
#define RTC_ALARM3_MIN_MASK (0x3Ful << RTC_ALARM3_MIN_POS)
#define RTC_ALARM3_HOUR_ADDR (uint32_t)&(RTC->ALARM3)
#define RTC_ALARM3_HOUR_POS (12) /*< bit[16:12]  */
#define RTC_ALARM3_HOUR_MASK (0x1Ful << RTC_ALARM3_HOUR_POS)

#define RTC_ALARM4_ADDR (uint32_t)&(RTC->ALARM4)
#define RTC_ALARM4_SEC_ADDR (uint32_t)&(RTC->ALARM4)
#define RTC_ALARM4_SEC_POS (0) /*< bit[5:0]    */
#define RTC_ALARM4_SEC_MASK (0x3Ful << RTC_ALARM4_SEC_POS)
#define RTC_ALARM4_MIN_ADDR (uint32_t)&(RTC->ALARM4)
#define RTC_ALARM4_MIN_POS (6) /*< bit[11:6]   */
#define RTC_ALARM4_MIN_MASK (0x3Ful << RTC_ALARM4_MIN_POS)
#define RTC_ALARM4_HOUR_ADDR (uint32_t)&(RTC->ALARM4)
#define RTC_ALARM4_HOUR_POS (12) /*< bit[16:12]  */
#define RTC_ALARM4_HOUR_MASK (0x1Ful << RTC_ALARM4_HOUR_POS)

#define RTC_ALARM5_ADDR (uint32_t)&(RTC->ALARM5)
#define RTC_ALARM5_SEC_ADDR (uint32_t)&(RTC->ALARM5)
#define RTC_ALARM5_SEC_POS (0) /*< bit[5:0]    */
#define RTC_ALARM5_SEC_MASK (0x3Ful << RTC_ALARM5_SEC_POS)
#define RTC_ALARM5_MIN_ADDR (uint32_t)&(RTC->ALARM5)
#define RTC_ALARM5_MIN_POS (6) /*< bit[11:6]   */
#define RTC_ALARM5_MIN_MASK (0x3Ful << RTC_ALARM5_MIN_POS)
#define RTC_ALARM5_HOUR_ADDR (uint32_t)&(RTC->ALARM5)
#define RTC_ALARM5_HOUR_POS (12) /*< bit[16:12]  */
#define RTC_ALARM5_HOUR_MASK (0x1Ful << RTC_ALARM5_HOUR_POS)

#define RTC_BASE 0x40016000UL
#define RTC ((RTC_T *)RTC_BASE)

#endif /* __RTC_REG_ER8130_H__ */

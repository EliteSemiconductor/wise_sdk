/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/wutmr_er8130.h"

#define WUTMR_RESET_HOLD_MS (10u)
#define WUTMR_XTAL_FREQ_HZ (40000000u)
#define WUTMR_LOOP_CYCLES (4u)
#define WUTMR_DELAY_LOOP_CNT ((WUTMR_RESET_HOLD_MS * (WUTMR_XTAL_FREQ_HZ / 1000u)) / WUTMR_LOOP_CYCLES)

#define WUTMR_CLK_HZ (32768u)
#define WUTMR_RESET_THRESHOLD_HOURS (36u)
#define WUTMR_RESET_THRESHOLD_CNT (WUTMR_CLK_HZ * WUTMR_RESET_THRESHOLD_HOURS * 3600u)

static inline void _wutmr_reset_delay(void)
{
    volatile uint32_t i;
    for (i = 0; i < WUTMR_DELAY_LOOP_CNT; i++) {
        __asm volatile("nop");
    }
}

static inline uint8_t _wutmr_need_reset_for_overflow(uint32_t cur0, uint32_t add_cnt)
{
    if (cur0 >= WUTMR_RESET_THRESHOLD_CNT) {
        return 1u;
    }
    if ((0xFFFFFFFFu - cur0) <= add_cnt) {
        return 1u;
    }
    return 0u;
}

static inline uint8_t _wutmr_is_wakup_event(void)
{
    return (uint8_t)(WUTMR->STS & WUTMR_WUTMR_EVT_WAKEUP_MASK);
}

static inline uint8_t _wutmr_is_wakup_response(void)
{
    return (uint8_t)(WUTMR->STS & WUTMR_WUTMR_RSP_WAKEUP_MASK);
}

static inline uint8_t _wutmr_get_sync(void)
{
    return (uint8_t)(WUTMR->CUR2 & WUTMR_WUTMR_SYNC_MASK);
}

void wutmr_reset_er8130(void)
{
    WUTMR->CTL_REG |= WUTMR_WUTMR_CTL_RST_MASK;
    _wutmr_reset_delay(); // at least 10ms
    WUTMR->CTL_REG &= ~WUTMR_WUTMR_CTL_RST_MASK;
}

void wutmr_set_cnt_er8130(uint32_t wu_cnt)
{
    uint8_t status;
    uint32_t cur0;

    cur0 = wutmr_get_cnt_er8130();
    if (_wutmr_need_reset_for_overflow(cur0, wu_cnt)) {
        wutmr_reset_er8130();
    }

    WUTMR->CTL_REG = 0;
    while (_wutmr_is_wakup_event() || _wutmr_is_wakup_response())
        ;
    status = _wutmr_get_sync();
    while (status == _wutmr_get_sync())
        ;
    WUTMR->PER_REG = wu_cnt;
}

uint32_t wutmr_get_cnt_er8130(void)
{
    uint8_t status;

    status = _wutmr_get_sync();
    while (status == _wutmr_get_sync())
        ;

    return WUTMR->CUR0;
}

void wutmr_set_enable_er8130(uint8_t period_en)
{
    WUTMR->CTL_REG = 0;
    WUTMR->CTL_REG = ((period_en << WUTMR_WUTMR_REP_EN_POS) & WUTMR_WUTMR_REP_EN_MASK) | WUTMR_WUTMR_EN_MASK;
}

void wutmr_set_disable_er8130(void)
{
    WUTMR->CTL_REG = 0;
}

uint32_t wutmr_get_int_status_er8130(void)
{
    return (WUTMR->INT_ST & WUTMR_INT_ST_WUTMR_MASK);
}

void wutmr_clear_int_status_er8130(void)
{
    WUTMR->INT_ST |= WUTMR_INT_ST_WUTMR_MASK;
}

void wutmr_set_int_enable_er8130(void)
{
    WUTMR->INT_EN = WUTMR_INT_EN_WUTMR_MASK;
    NVIC_EnableIRQ((IRQn_Type)WUTMR_IRQn);
}

void wutmr_set_int_disable_er8130(void)
{
    WUTMR->INT_EN = 0;
    NVIC_DisableIRQ((IRQn_Type)WUTMR_IRQn);
}

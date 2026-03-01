/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/wutmr_er8130.h"

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

void wutmr_set_cnt_er8130(uint32_t wu_cnt)
{
    uint8_t status;
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
    while (status == _wutmr_get_sync());
    
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

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/wdt_er8130.h"

static void wdt_lock_er8130(void)
{
    WDT->LOCK = 0;
}

static void wdt_unlock_er8130(void)
{
    WDT->LOCK = 0x1ACCE551;
}

void wdt_config_er8130(uint32_t period, uint32_t reset_en)
{
    wdt_unlock_er8130();
    WDT->LOAD = period;
    WDT->CTRL = 0;
    WDT->CTRL = (reset_en << WDT_RESEN_POS);
    wdt_lock_er8130();
}

void wdt_start_er8130(void)
{
    wdt_unlock_er8130();
    WDT->CTRL |= WDT_INTEN_MASK;
    wdt_lock_er8130();
    NVIC_EnableIRQ((IRQn_Type)WDT_IRQn);
}

void wdt_stop_er8130(void)
{
    wdt_unlock_er8130();
    WDT->CTRL &= ~WDT_INTEN_MASK;
    wdt_lock_er8130();
    NVIC_DisableIRQ((IRQn_Type)WDT_IRQn);
}

uint32_t wdt_get_period_er8130(void)
{
    return WDT->VALUE;
}

void wdt_set_period_er8130(uint32_t period)
{
    wdt_unlock_er8130();
    WDT->LOAD = period;
    wdt_lock_er8130();
}
/**
 *  @brief
 *  A write of any value to the WDOGINTCLR register
 *  clears the watchdog interrupt,
 *  and reloads the counter from the value in WDOGLOAD
 */
void wdt_clear_int_flag_er8130(void)
{
    wdt_unlock_er8130();
    WDT->INTCLR = 1;
    wdt_lock_er8130();
}

uint32_t wdt_get_status_er8130(void)
{
    return WDT->MIS;
}

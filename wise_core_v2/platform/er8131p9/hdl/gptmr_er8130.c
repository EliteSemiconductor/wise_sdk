/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/gptmr_er8130.h"

HAL_STATUS gptmr_open_er8130(GPTMR_T *gptmr, uint32_t mode)
{
    gptmr->CTRL = 0;
    if (mode == 1) {
        return HAL_NO_ERR;
    } else if (mode == 2) {
        gptmr->CTRL |= GPTMR_EXT_EN_MASK;
        return HAL_NO_ERR;
    } else if (mode == 4) {
        gptmr->CTRL |= GPTMR_EXT_EN_MASK;
        gptmr->CTRL |= GPTMR_EXT_CLK_MASK;
        return HAL_NO_ERR;
    } else {
        return HAL_ERR;
    }
}

void gptmr_start_er8130(GPTMR_T *gptmr)
{
    gptmr->CTRL |= GPTMR_EN_MASK;
}

void gptmr_stop_er8130(GPTMR_T *gptmr)
{
    gptmr->CTRL &= (~GPTMR_EN_MASK);
}

bool gptmr_is_enabled_er8130(GPTMR_T *gptmr)
{
    return ((gptmr->CTRL & GPTMR_EN_MASK) != 0);
}

void gptmr_enable_int_er8130(GPTMR_T *gptmr)
{
    gptmr->CTRL |= GPTMR_INT_EN_MASK;
}

void gptmr_disable_int_er8130(GPTMR_T *gptmr)
{
    gptmr->CTRL &= (~GPTMR_INT_EN_MASK);
}

void gptmr_clear_int_flag_er8130(GPTMR_T *gptmr)
{
    gptmr->INTCLEAR = 1;
}

void gptmr_set_cnt_er8130(GPTMR_T *gptmr, uint32_t cnt)
{
    gptmr->RELOAD = cnt;
    gptmr->VALUE  = cnt;
}

void gptmr_set_freq_er8130(GPTMR_T *gptmr, uint32_t freq)
{
    gptmr->RELOAD = SystemCoreClock / freq;
    gptmr->VALUE  = SystemCoreClock / freq;
}

void gptmr_reset_counter_er8130(GPTMR_T *gptmr)
{
    gptmr->VALUE = 0;
}

uint32_t gptmr_get_cnt_er8130(GPTMR_T *gptmr)
{
    return gptmr->VALUE;
}

uint32_t gptmr_get_reload_er8130(GPTMR_T *gptmr)
{
    return gptmr->RELOAD;
}

// void gptmr_output_config_er8130(GPTMR_T* gptmr, uint32_t u32PinMask)
//{
//	uint32_t u32Pin, u32GPTMR_Channel;
//
//	for(u32GPTMR_Channel = 0ul; u32GPTMR_Channel < 6; u32GPTMR_Channel++)
//	{
//		if((((GPTMR_T*) (gptmr))) == (((GPTMR_T*)
//(GPTMR0_BASE+(u32GPTMR_Channel*0x1000)))))
//		{
//			for(u32Pin = 0ul; u32Pin < GPIO_PIN_MAX; u32Pin++)
//			{
//				if(u32PinMask & (1ul << u32Pin))
//				{
//					(*((volatile uint32_t
//*)(GPIO_BASE+0x300+(0x4*u32GPTMR_Channel)))) = u32Pin; 					GPIO_SetMode((1ul <<
//u32Pin), GPIO_MODE); 					break;
//				}
//			}
//			break;
//		}
//	}
// }

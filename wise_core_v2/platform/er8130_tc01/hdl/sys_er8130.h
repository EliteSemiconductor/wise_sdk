/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef _SYS_ER8130_H_
#define _SYS_ER8130_H_

#include "esmt_chip_specific.h"
#include "types.h"

#define SYS_CHIP_ID_ER8130 ESMT_SOC_CHIP_ID
#define SYS_UNLOCK_VALUE 0x1ACCE551
#define DUT_CHIP_ID SYS_CHIP_ID_ER8130

#define INT_ST_RX BIT0
#define INT_ST_RXERR BIT1
#define INT_ST_TX BIT2
#define INT_ST_TXERR BIT3
#define INT_ST_TMRTHD BIT4
#define INT_ST_TMREVT BIT5
#define INT_ST_TX_FIFO_FULL BIT6
#define INT_ST_RX_FIFO_EMPTY BIT7
#define INT_ST_WAKEUP BIT8
#define INT_ST_NFC BIT9
#define INT_ST_TX_FIFO_AEMPTY BIT10
#define INT_ST_RX_FIFO_AFULL BIT11
#define INT_ST_RX_SYNCW BIT12

uint32_t sys_get_chip_id_er8130();
void sys_set_remap_er8130(uint32_t remap_addr);
void sys_lock_er8130();
void sys_unlock_er8130(void);
//void sys_gpio_cfg_er8130(bool swd_enable, bool swd_pullup, bool tcxo_enable);
void sys_cfg_swd_er8130(bool swd_enable);
void sys_cfg_tcxo_er8130(bool tcxo_enable);
void sys_chip_reset_er8130();
uint8_t sys_get_pa_type_er8130(void);
void sys_set_pa_type_er8130(uint8_t pa_type);
void sys_set_delay_er8130(uint32_t us);
uint8_t sys_get_board_match_type_er8130(void);
void sys_set_board_match_type_er8130(uint8_t mat_type);
void sys_set_board_gain_ctrl_er8130(uint8_t gain_ctrl);
uint8_t sys_get_board_gain_ctrl_er8130();
uint32_t sys_get_xtal_cfg_er8130(void);
void sys_set_xtal_cfg_er8130(uint8_t xtal_i, uint8_t xtal_o);


#define SYS_GET_PA_TYPE()                   sys_get_pa_type_er8130()
#define SYS_SET_PA_TYPE(pa_type)            sys_set_pa_type_er8130(pa_type)
#define SYS_GET_BOARD_MATCH_TYPE()          sys_get_board_match_type_er8130()
#define SYS_SET_BOARD_MATCH_TYPE(mat_type)  sys_set_board_match_type_er8130(mat_type)
#define SYS_SET_BOARD_40M_GAIN(gain_ctrl)   sys_set_board_gain_ctrl_er8130(gain_ctrl)
#define SYS_GET_BOARD_40M_GAIN()            sys_get_board_gain_ctrl_er8130()
#define SYS_SET_DELAY(us)                   sys_set_delay_er8130(us)
#define SYS_GET_XTAL_CFG()                  sys_get_xtal_cfg_er8130();
#define SYS_SET_XTAL_CFG(xtal_i, xtal_o)    sys_set_xtal_cfg_er8130(xtal_i, xtal_o)
#endif

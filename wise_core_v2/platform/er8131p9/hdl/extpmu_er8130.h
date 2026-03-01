#ifndef __EXTPMU_ER8130_H
#define __EXTPMU_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

/*
typedef enum {
	EXTPMU_WU_SRC_WUTMR = 0,
	EXTPMU_WU_SRC_NFC=3,
	EXTPMU_WU_SRC_GPIO=4
} EXTPMU_WU_SRC;
*/

void extpmu_enable_external_pmu_er8130(uint8_t enable);
void extpmu_set_wakeup_time_er8130(uint32_t time);
void extpmu_set_sleep_er8130(uint8_t wutmr_en);
void extpmu_enable_wakeup_source_er8130(uint8_t wu_src);
void extpmu_disable_wakeup_source_er8130(uint8_t wu_src);
void extpmu_set_wakeup_gpio_pin_er8130(uint8_t gpio_pin);
uint8_t extpmu_get_wakeup_gpio_pin_er8130(void);
void extpmu_clk_source_sel_er8130(uint8_t clk_src);
void extpmu_disable_extpmu_shutdown_control_for_32k_16k_osc(void);
uint32_t extpmu_get_wutmr_counter_er8130(void);

uint32_t extpmu_get_int_status_er8130(void);
void extpmu_clear_int_status_er8130(void);
void extpmu_set_int_enable_er8130(void);
void extpmu_set_int_disable_er8130(void);
void extpmu_set_wakeup_by_nfc_er8130(uint8_t enable);

#endif

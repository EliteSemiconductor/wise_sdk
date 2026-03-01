#ifndef __EXTPMU_ER8130_H
#define __EXTPMU_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
	EXTPMU_WU_SRC_WUTMR = 0,
	EXTPMU_WU_SRC_NFC=3,
	EXTPMU_WU_SRC_GPIO=4
} EXTPMU_WU_SRC;

void extpmu_set_enable_er8130(uint8_t enable);
void extpmu_set_wakeup_time_er8130(uint32_t time);
void extpmu_set_sleep_er8130(uint8_t wutmr_en);
void extpmu_set_wakeup_source_er8130(uint8_t wu_src);
void extpmu_clk_source_sel_er8130(uint8_t clk_src);
void extpmu_disable_extpmu_shutdown_control_for_32k_16k_osc(void);
#endif

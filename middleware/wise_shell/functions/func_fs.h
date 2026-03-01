/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __FUNC_FS_H
#define __FUNC_FS_H

#include <stdio.h>

#include "wise_core.h"
#include "wise.h"

#include "wise_file_system.h"
#include "wise_gptmr_api.h"

int32_t load_cfg_from_fs(void);
void cfg_set_default(void);
int32_t save_cfg_to_fs(void);
void fs_radio_cfg_init(void);

#endif //__FUNC_FS_H

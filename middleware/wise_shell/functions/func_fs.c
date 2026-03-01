/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "func_fs.h"
#include "func_rf.h"
#include <stdint.h>
#if MIDDLEWARE_WISE_SHELL
#include "shell_commands.h"
#endif

void cfg_set_default(void)
{
#if ES_DEVICE_RADIO
    memcpy((void *)&fs_radio_cfg, (void *)&radioCfg[defRadioIntf],
           sizeof(WISE_RADIO_CFG_T));
#endif
}

int32_t save_cfg_to_fs(void)
{
#if ES_DEVICE_RADIO
    WISE_FS_INFO_T fs_info = {0};
    int32_t result         = WISE_SUCCESS;

    if (WISE_SUCCESS != wise_fs_get_fs_info(&fs_info)) {
        CLI_PRINTF("\nNo file system, please do fs format first\n");
        return result;
    }

    wise_fs_reload();

    if (WISE_SUCCESS != wise_fs_user_data_update(0x0, sizeof(WISE_RADIO_CFG_T),
                                                 (uint8_t *)&fs_radio_cfg)) {
        result = WISE_FAIL;
    }

    return result;
#endif
}

int32_t load_cfg_from_fs(void)
{
#if ES_DEVICE_RADIO
    WISE_FS_INFO_T fs_info = {0};
    uint32_t data_len      = sizeof(WISE_RADIO_CFG_T);
    int32_t result         = WISE_FAIL;
    WISE_RADIO_CFG_T rxTmpCfg;

    if (WISE_SUCCESS != wise_fs_get_fs_info(&fs_info)) {
        CLI_PRINTF("\nNo file system, please do fs format first\n");
        return result;
    }

    wise_fs_reload();

    wise_fs_user_data_read(0x0, data_len, (uint8_t *)&rxTmpCfg);
    if (rxTmpCfg.syncword != 0xFFFFFFFF) {
        memcpy((char *)&fs_radio_cfg, (char *)&rxTmpCfg,
               sizeof(WISE_RADIO_CFG_T));
        result = WISE_SUCCESS;
    }
    return result;
#endif
}

void fs_radio_cfg_init(void)
{
#if ES_DEVICE_RADIO
    WISE_FS_INFO_T fs_info = {0};

    if (WISE_SUCCESS == wise_fs_get_fs_info(&fs_info)) {
        if (WISE_SUCCESS != load_cfg_from_fs()) {
            cfg_set_default();
            save_cfg_to_fs();
            debug_print("\nset default\n");
        }
    } else {
        CLI_PRINTF("\nNo file system, please do fs format first\n");
        while (1) {
            wise_main_proc();
        }
    }
#endif
}

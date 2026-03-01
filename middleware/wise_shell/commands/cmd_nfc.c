/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_NFC
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wise_core.h"
#include "api/wise_nfc_api.h"
#include "cmd_nfc.h"


static int _nfcInitCmd(int argc, char **argv);
static int _txpwrCmd(int argc, char **argv);
static int _confCmd(int argc, char **argv);
static int _workCmd(int argc, char **argv);
static int _lockCmd(int argc, char **argv);
static int _dpeCmd(int argc, char **argv);
static int _blkCmd(int argc, char **argv);
static int _hostCmd(int argc, char **argv);
static int _intCmd(int argc, char **argv);
static int _pwsrcCmd(int argc, char **argv);
static int _wkupCmd(int argc, char **argv);



static const struct shellCommand m_NFCCmdlist[] = {
    {"help", "nfc function", _helpCmd},    
    {"init", "init nfc system", _nfcInitCmd},
    {"conf", "nfc configure", _confCmd},    
    {"txpwr", "nfc tx power", _txpwrCmd},
    {"work", "nfc work mode", _workCmd},
    {"lock", "nfc lock mode", _lockCmd},
    {"dpe", "nfc dpe control", _dpeCmd},
    {"blk", "nfc block read/write", _blkCmd},
    {"host", "nfc host (lock/unlock)", _hostCmd},
    {"int", "nfc int (int_idx)", _intCmd},
    {"pwsrc", "nfc pwrsrc (src)", _pwsrcCmd},
    {"wkup", "nfc wakeup from mode (sleep/shutdowm)", _wkupCmd},
};

int NFCCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_NFCCmdlist, (sizeof(m_NFCCmdlist) / sizeof(struct shellCommand)));
}

void _my_nfc_callback(void *context, uint8_t idx)
{
    printf("%s : NFC event triggered! idx = %d \n", __func__, idx);
}

static int _nfcInitCmd(int argc, char **argv)
{
    uint8_t initMode = 1;

    WISE_NFC_CFG_T nfc_cfg = {
        .work_mode = 1, //0 = default, 1 = full mode
        .lock_mode = 1, //0 = mode 1, 1 = mode 2
        .int_idx = NFC_INT_IDX_1,
    };
    
    uint32_t tag_cnt[] = {
        0xC8743400,
        0x801CE3E1,
        0x0000009E,
        0x001210E1,
        0x65025419,
        0x2069486E,
        0x49202E2E,
        0x206D6120,
        0x79687053,
        0x7420786E,
        0xFE363063,
        0x00000000,
        0x00000000,
    };

    if (argc == 3) {
        initMode = (strtoul(argv[2], NULL, 10) & 0x01);

        if (initMode == 0) {
            wise_nfc_init();
            wise_nfc_config(&nfc_cfg);
            wise_nfc_block_write_data(0, 14, tag_cnt);
            wise_nfc_register_int_callback(NFC_INT_IDX_0, _my_nfc_callback, NULL);
            wise_nfc_register_int_callback(NFC_INT_IDX_1, _my_nfc_callback, NULL);

            printf("nfc init [%d] done\n", initMode);
        }
        
    }

    // handle unexpect input behavior
    if (argc == 2 || argc > 3) {
        debug_print("input error!\n");
        debug_print("Usage:init [0/1]\n");
        debug_print("[0]:normal\n");
        debug_print("[1]:TBD\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static void _nfc_show_info(void)
{
    debug_print("work_mode: %d (0 = default, 1 = full mode)\n", wise_nfc_get_mem_work_mode());
    debug_print("lock_mode: %d (0 = mode 1, 1 = mode 2)\n", wise_nfc_get_mem_lock_mode());
    debug_print("interupt idx: %d\n", wise_nfc_get_interrupt_idx());
}

static int _confCmd(int argc, char **argv)
{
    if (argc == 2) {
        _nfc_show_info();
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf conf\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _txpwrCmd(int argc, char **argv)
{
#if 0
    uint8_t inputPwr;
    uint8_t Pwr;

    if (argc == 2) {
        //Pwr = radioGetTxPwr();
        debug_print("Current tx power %d\n", Pwr);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (argc == 3) {
        if (isRadioConfigured() == 0) {
            debug_print("Please exec rf init first!!\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        inputPwr = strtoul(argv[2], NULL, 10);

        if (isInputPwrVaild(inputPwr)) {
            radioSetTxPwr(inputPwr);
            Pwr = radioGetTxPwr();
            debug_print("Set tx power %d\n", Pwr);
        } else {
            debug_print("Tx power range (0-%d).\n", MAX_TX_PWR_LEVEL);
        }

        return SHLCMD_HIST | SHLCMD_DONE;
    }

    debug_print("input error!\n");
#endif    
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _workCmd(int argc, char **argv)
{
    uint8_t work_mode = 0;

    if (argc == 2) {
        work_mode = wise_nfc_get_mem_work_mode();
        
        debug_print("mem work mode = %d (0=defaul/ 1=full)\n", work_mode);
    }


    if (argc == 3) {
        work_mode = strtoul(argv[2], NULL, 10);

        wise_nfc_set_mem_work_mode(work_mode);

        debug_print("set work mode = %d(0=defaul/ 1=full)\n", work_mode);
    }

    if (argc != 2 && argc != 3) {
        debug_print("input error!\n");
        debug_print("Usage:nfc work [0/1] \n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _lockCmd(int argc, char **argv)
{
    uint8_t lock_mode = 0;
    
    if (argc == 2) {
        
        lock_mode = wise_nfc_get_mem_lock_mode();
            
        debug_print("mem lock mode = %d (0=mode1/ 1=mode2) \n", lock_mode);
    }


    if (argc == 3) {
        lock_mode = strtoul(argv[2], NULL, 10);
        
        wise_nfc_set_mem_lock_mode(lock_mode);
        
        debug_print("set lock mode = %d (0=mode1/ 1=mode2) \n", lock_mode);

    } 


    if (argc != 2 && argc != 3) {
        debug_print("input error!\n");
        debug_print("Usage:nfc lock [0/1]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _dpeCmd(int argc, char **argv)
{
    if (argc == 2) {
        debug_print("dpe status info = 0x%x\n", wise_nfc_get_dpe_status_info());
        debug_print("dpe ctrl info = 0x%x\n", wise_nfc_get_dpe_ctrl_info());
    }


    if (argc == 3) {

    } 


    if (argc != 2 && argc != 3){
        debug_print("input error!\n");
        debug_print("Usage:nfc dpe \n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _blkCmd(int argc, char **argv)
{
    uint32_t block_idx;
    uint32_t rx_data_buff;
    uint32_t tx_data_buff;


    if (argc == 3) {
        block_idx = strtoul(argv[2], NULL, 16);
    
        wise_nfc_block_read_data(block_idx, 1, &rx_data_buff);
        
        debug_print("r block[0x%lx] = 0x%08lx\n", block_idx, rx_data_buff);
    }


    if (argc == 4) {
        block_idx = strtoul(argv[2], NULL, 16);
        tx_data_buff = strtoul(argv[3], NULL, 16);

        wise_nfc_block_write_data(block_idx, 1, &tx_data_buff);
        
        wise_nfc_block_read_data(block_idx, 1, &rx_data_buff);

        debug_print("w block[0x%lx] = 0x%08lx => 0x%08lx\n", block_idx, tx_data_buff, rx_data_buff);
    } 


    if (argc != 3 && argc != 4){
        debug_print("input error!\n");
        debug_print("Usage:nfc block [block num(HEX)] [block write data(HEX)]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _hostCmd(int argc, char **argv)
{
    uint8_t lock = 0;

    if (argc == 3) {
        lock = strtoul(argv[2], NULL, 10);
        
        wise_nfc_set_host_locked(lock);
        
        debug_print("set host lock = %d ( 1=locked/ 0=unlocked) \n", lock);

    } 


    if (argc != 3) {
        debug_print("input error!\n");
        debug_print("Usage:nfc host [1=locked/ 0=unlocked]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _intCmd(int argc, char **argv)
{
    uint8_t int_idx = 0;

     if (argc == 2) {
        int_idx = wise_nfc_get_interrupt_idx();
        
        debug_print("host int index = %d \n", int_idx);
     }   

    if (argc == 3) {
        int_idx = strtoul(argv[2], NULL, 10);
        
        wise_nfc_set_interrupt_idx(int_idx);
        
        debug_print("set host int index = %d \n", int_idx);

    } 


    if (argc != 2 && argc != 3) {
        debug_print("input error!\n");
        debug_print("Usage:nfc int [int_idx(0/1)]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _pwsrcCmd(int argc, char **argv)
{
    uint8_t pwr_src = 0;

    if (argc == 2) {
        pwr_src = wise_nfc_get_pwr_src_idx();
        
        debug_print("current nfc power src = %d (0=mcu/ 1=reader)\n", pwr_src);
    }

    
    if (argc == 3) {
        pwr_src = strtoul(argv[2], NULL, 10);

        if (pwr_src >= 2) {
            debug_print("power source is bad = %d\n", pwr_src);
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    
        wise_nfc_switch_pwr_src(pwr_src);
        
        debug_print("switch power src = %d (0=mcu/ 1=reader) \n", pwr_src);

    } 


    if (argc != 2 && argc != 3) {
        debug_print("input error!\n");
        debug_print("Usage:nfc pwrsrc [0=mcu/ 1=reader]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;

}

static int _wkupCmd(int argc, char **argv)
{
    uint8_t pwr_mode = 0;
    uint8_t mode;

    if (argc == 3) {
        mode = strtoul(argv[2], NULL, 10);

        if (mode >= 2) {
            debug_print("sleep mode is bad = %d\n", mode);
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        if (mode == 1)
            pwr_mode = PWR_MODE_SLEEP;
        if (mode == 2)
            pwr_mode = PWR_MODE_SHUTDOWN;
        
        wise_nfc_set_wakeup_config(pwr_mode);
        
        debug_print("set wakeup by NFC from mode :[%d] (1=sleep/ 2=shutdown) \n", mode);

    } 


    if (argc != 3) {
        debug_print("input error!\n");
        debug_print("Usage:nfc wkup [1=sleep/ 2=shutdown]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;

}

#endif /* SHELL_NFC */

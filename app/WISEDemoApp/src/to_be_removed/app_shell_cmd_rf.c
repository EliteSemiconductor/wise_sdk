/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#include "app_shell_cmd_rf.h"
#include "app_shell_func_rf.h"

#include "radio_lib/wise_radio_api.h"
#include "wise_core.h"

#include "shell.h"
#include "shell_commands.h"

static int _initCmd(int argc, char **argv);
static int _confCmd(int argc, char **argv);
static int _recvCmd(int argc, char **argv);
static int _sendCmd(int argc, char **argv);
static int _chCmd(int argc, char **argv);
static int _txpwrCmd(int argc, char **argv);
static int _rxlogCmd(int argc, char **argv);
static int _ccaCmd(int argc, char **argv);
static int _singCmd(int argc, char **argv);
static int _filtCmd(int argc, char **argv);
static int _perCmd(int argc, char **argv);

static const struct shellCommand m_radioCmdList[] = {
    {"help", "radio function", _helpCmd},
    {"init", "init radio system", _initCmd},
    {"conf", "radio configure", _confCmd},
    {"recv", "radio rx on/off", _recvCmd},
    {"send", "radio tx pkt number", _sendCmd},
    {"ch", "radio channel number", _chCmd},
    {"txpwr", "radio tx power", _txpwrCmd},
    {"rxlog", "show rx data", _rxlogCmd},
    {"cca", "enable rx cca mode", _ccaCmd},
    {"sing", "enable tx single-tone", _singCmd},
    {"filt", "radio rx filter enable", _filtCmd},
    {"per", "radio PER test", _perCmd},
};

int RadioCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_radioCmdList,
                                sizeof(m_radioCmdList) /
                                    sizeof(struct shellCommand));
}

static int _initCmd(int argc, char **argv)
{
    uint8_t initMode = 1;

    //file system radio config
#if 0 //chester
    fs_radio_cfg_init();
#endif

    if (argc == 3) {
        initMode = (strtoul(argv[2], NULL, 10) & 0x01);
        if (initMode == 0) {
            radioInit();
        } else if (initMode == 1) {
            radioInit802154();
        } else {
            goto invalid;
        }
    } else {
        goto invalid;
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    debug_print("Invalid parameters.\n");
    debug_print("Usage:init [0/1]\n");
    debug_print("[0]:Transparent Mode\n");
    debug_print("[1]:802.15.4 Mode\n");

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _confCmd(int argc, char **argv)
{
    if (argc == 2) {
        radioDebug();
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf conf\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _recvCmd(int argc, char **argv)
{
    if (argc == 3) {
        if (strcmp(argv[2], "on") == 0) {
            if (isRadioConfigured() == 0) {
                debug_print("Please exec rf init first!!\n");
                return SHLCMD_HIST | SHLCMD_DONE;
            } else {
                radioRxOn();
                debug_print("radio rx on\n");
            }
        } else if (strcmp(argv[2], "off") == 0) {
            radioRxOff();
            debug_print("radio rx off\n");
        } else {
            goto invalid;
        }
    }
    // handle unexpect input behavior
    if (argc == 2 || argc > 3) {
    invalid:
        debug_print("input error!\n");
        debug_print("Usage:rf recv [on/off]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _sendCmd(int argc, char **argv)
{
    uint32_t testCount = 1;

    if (argc == 3) {
        if (isRadioConfigured() == 0) {
            debug_print("Please exec rf init first!!\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
        testCount = strtoul(argv[2], NULL, 10);
        ;
        if (testCount == 0) {
            testCount = 1;
        }

        radioTestSend(testCount);
        debug_print("tx pkt %ld end\n", testCount);
    }
    if (argc == 2) {
        debug_print("input error!\n");
        debug_print("Usage:rf send [pkt num(DEC)]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _chCmd(int argc, char **argv)
{
    uint8_t channelIdx, chNum;
    uint32_t freq;

    if (argc == 2) {
        chNum = radioGetChannel();
        freq  = radioGetFreq();
        debug_print("Current channel %d freq=%lu\n", chNum, freq);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (argc == 3) {
        if (isRadioConfigured() == 0) {
            debug_print("Please exec rf init first!!\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
        channelIdx = strtoul(argv[2], NULL, 10);
        ;
        int chNum = radioGetValidChannelNumber();
        if ((channelIdx >= chNum)) {
            debug_print("Out of channel range (0-%d).\n", chNum - 1);
            return SHLCMD_HIST | SHLCMD_DONE;
        }
        radioSetChannel(channelIdx);
        chNum = radioGetChannel();
        freq  = radioGetFreq();
        debug_print("Set channel %d freq=%lu\n", chNum, freq);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    debug_print("input error!\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _txpwrCmd(int argc, char **argv)
{
    uint8_t inputPwr;
    uint8_t Pwr;

    if (argc == 2) {
        Pwr = radioGetTxPwr();
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
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _rxlogCmd(int argc, char **argv)
{
    if (argc == 3) {
        if (strcmp(argv[2], "on") == 0) {
            radioSetRxLog(1);
            debug_print("enable rx data showing\n");
        } else if (strcmp(argv[2], "off") == 0) {
            radioSetRxLog(0);
            debug_print("disable rx data showing\n");
        } else {
            goto invalid;
        }
    }
    // handle unexpect input behavior
    if (argc == 2 || argc > 3) {
    invalid:
        debug_print("input error!\n");
        debug_print("Usage:rf rxlog [on/off]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _ccaCmd(int argc, char **argv)
{
    uint32_t cca_times;
    int8_t rssi;
    uint8_t phy_idx;
    uint32_t i = 0;

    if (argc == 3) {
        if (isRadioConfigured() == 0) {
            debug_print("Please exec rf init first!!\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        cca_times = strtoul(argv[2], NULL, 10);

        while (cca_times--) {
            rssi = radioGetRssi();
            i++;
            debug_print("[%d][phy %d] rssi = %d\n", i, phy_idx, rssi);
        }
    }

    // handle unexpect input behavior
    if (argc == 2 || argc > 3) {
        debug_print("input error!\n");
        debug_print("Usage:rf cca [times]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _singCmd(int argc, char **argv)
{
    uint32_t pwr_lv;

    if (argc == 3) {
        if (isRadioConfigured() == 0) {
            debug_print("Please exec rf init first!!\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        if (strcmp(argv[2], "off") == 0) {
            radioSingleToneOff();
        } else {
            pwr_lv = strtoul(argv[2], NULL, 10);

            if (pwr_lv <= 127) {
                radioSingleToneOn(pwr_lv);
            } else {
                debug_print("power level is limited to 0-127\n");
            }
        }
    }

    // handle unexpect input behavior
    if (argc == 2 || argc > 3) {
        debug_print("input error!\n");
        debug_print("Usage:rf sing [0-127]/off\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _filtCmd(int argc, char **argv)
{
    uint16_t panId;
    uint16_t saddr;
    uint8_t laddr[8] = {0};
    uint32_t laddr_l;
    uint32_t laddr_h;

    if (argc == 4) {
        if (strcmp(argv[2], "panid") == 0) {
            panId = (uint16_t)strtoul(argv[3], NULL, 16);
            setPanId(panId);
            debug_print("enable rx panid filter : 0x%04x\n", panId);
        } else if (strcmp(argv[2], "saddr") == 0) {
            saddr = (uint16_t)strtoul(argv[3], NULL, 16);
            setShortAddress(saddr);
            debug_print("enable rx short address filter : 0x%04x\n", saddr);
        }

        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (argc == 5) {
        if (strcmp(argv[2], "laddr") == 0) {
            laddr_l = strtoul(argv[3], NULL, 16);
            laddr_h = strtoul(argv[4], NULL, 16);
            for (uint8_t i = 0; i < 4; i++) {
                laddr[i]     = (laddr_l >> (8 * (i))) & 0xFF;
                laddr[i + 4] = (laddr_h >> (8 * (i))) & 0xFF;
            }
            setLongAddress(laddr);
            debug_print("enable rx long address filter: 0x%08lx%08lx\n",
                        laddr_l, laddr_h);
        }

        return SHLCMD_HIST | SHLCMD_DONE;
    }

    // handle unexpect input behavior
    if (argc > 5) {
        debug_print("input error!\n");
        debug_print("Usage:rf filt [item] [value]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _perCmd(int argc, char **argv)
{
    uint32_t testCount = 0;

    if (argc == 4) {
        testCount = strtoul(argv[3], NULL, 10);

        if (0 == strcmp(argv[2], "rx")) {
            debug_print("Start PER test receiver testCount=%d\n", testCount);
            startPERTest(0, testCount);
        } else if (0 == strcmp(argv[2], "tx")) {
            debug_print("Start PER test sender testCount=%d\n", testCount);
            startPERTest(1, testCount);
        } else {
            goto invalid;
        }
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    debug_print("Invalid parameters\n");
    debug_print("Usage: rf per [rx/tx] [count]\n");

    return SHLCMD_HIST | SHLCMD_DONE;
}

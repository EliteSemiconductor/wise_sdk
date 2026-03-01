/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_RADIO
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "wise_core.h"
#include "wise_radio_api.h"
#include "wise_radio_wmbus_api.h"
#include "wise_sys_api.h"
#include "wise_nfc_api.h"
#include "cmd_rf.h"
#include "util_crc.h"
#include "shell_func.h"

#ifdef WISE_WMBUS_SW_HELPER
#include "wise_wmbus_crypto.h"
#include "internal_radio_ctrl.h"
#include "util_crc.h"
#include "wise_crypto_api.h"
#endif

static int _phyCmd(int argc, char **argv);
static int _rfInitCmd(int argc, char **argv);
static int _rfConfCmd(int argc, char **argv);
static int _rfRecvCmd(int argc, char **argv);
static int _rfSendCmd(int argc, char **argv);
static int _chCmd(int argc, char **argv);
static int _rfTxpwrCmd(int argc, char **argv);
static int _rxlogCmd(int argc, char **argv);
static int _ccaCmd(int argc, char **argv);
static int _singCmd(int argc, char **argv);
static int _rfDumpCmd(int argc, char **argv);
static int _rfRegCmd(int argc, char **argv);
static int _filtCmd(int argc, char **argv);
static int _drCmd(int argc, char **argv);
static int _deviCmd(int argc, char **argv);
static int _modCmd(int argc, char **argv);
static int _phymodeCmd(int argc, char **argv);
static int _swCmd(int argc, char **argv);
static int _rfInfoCmd(int argc, char **argv);
static int _rxMaxLenCmd(int argc, char **argv);
static int _worCmd(int argc, char **argv);
static int _paCmd(int argc, char **argv);
static int _matchCmd(int argc, char **argv);
static int _freqCmd(int argc, char **argv);
static int _testCmd(int argc, char **argv);
static int _wmbusCmd(int argc, char **argv);
static int _bitorderCmd(int argc, char **argv);
static int _lencfgCmd(int argc, char **argv);
static int _crcCmd(int argc, char **argv);
static int _sgTestCmd(int argc, char **argv);
static int _CntCmd(int argc, char **argv);
static int _ulpldoCmd(int argc, char **argv);
static int _bodCmd(int argc, char **argv);



static const struct shellCommand m_radioCmdList[] = {
    {"help", "radio function", _helpCmd},
    {"phy", "select radio phy 0/1", _phyCmd},
    {"init", "init radio system", _rfInitCmd},
    {"conf", "radio configure", _rfConfCmd},
    {"recv", "radio rx on/off", _rfRecvCmd},
    {"send", "radio tx pkt number", _rfSendCmd},
    {"ch", "radio channel number", _chCmd},
    {"txpwr", "radio tx power", _rfTxpwrCmd},
    {"rxlog", "show rx data", _rxlogCmd},
    {"cca", "enable rx cca mode", _ccaCmd},
    {"sing", "set tx single-tone enable", _singCmd},
    {"dump", "dump phy[0/1] register", _rfDumpCmd},
    {"reg", "write/read phy[0/1] register", _rfRegCmd},
    {"filt", "radio rx filter enable", _filtCmd},
    {"dr", "radio data rate", _drCmd},
    {"devi", "radio freq deviation", _deviCmd},
    {"mod", "radio modulation", _modCmd},
    {"pmode", "radio phy mode", _phymodeCmd},
    {"sw", "radio syncword", _swCmd},
    {"rxlen", "radio rx maximum length", _rxMaxLenCmd},
    {"wor", "radio wor on/off", _worCmd},
    {"pa", "radio pa type", _paCmd},
    {"match", "radio matching type", _matchCmd},
    {"freq", "radio channel frequency", _freqCmd},
    {"wmbus", "wmbus init ", _wmbusCmd},
    {"test", "radio test", _testCmd},
    {"bitorder", "Bit transmission order: 0=LSB first, 1=MSB first", _bitorderCmd},
    {"lencfg", "Packet type: 0=Fixed length, 1=Variable length", _lencfgCmd},
    {"crc", "CRC subcommands: show|cfg|set|clr|poly|seed", _crcCmd },
    {"sg", "SG TEST: 1=on, 0=off", _sgTestCmd },
    {"cnt", "show cnt info: 1=show info, 0=clear", _CntCmd },
    {"info", "radio information", _rfInfoCmd},
    {"ulpldo", "ulpldo testing 0=default, 1=select", _ulpldoCmd},
    {"bod", "set bod level [0-7]/off", _bodCmd},
};

int RadioCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_radioCmdList, sizeof(m_radioCmdList) / sizeof(struct shellCommand));
}

static int _phyCmd(int argc, char **argv)
{
    uint8_t phy_intf = 0;
    uint32_t chip_id = 0;

    if (argc == 3) {
        phy_intf = (strtoul(argv[2], NULL, 10) & 0x01);
        setPhyIntf(phy_intf);
        if (phy_intf == 0) {
            chip_id = wise_sys_get_chip_id();
        }

#ifdef ES_DEVICE_TRX_RADIO
        if (phy_intf == 1) {
            chip_id = wise_trx_sys_get_chip_id();
        }
#endif

        debug_print("chip id = 0x%08lx \n", chip_id);
    }

    if (argc == 2) {
        debug_print("radio phy %d\n", getPhyIntf());
    }

    // handle unexpect input behavior
    if (argc > 3) {
        debug_print("input error!\n");
        debug_print("Usage:rf phy [0/1]\n");
        debug_print("[0]:Internal Mode\n");
        debug_print("[1]:Externtal Mode\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _rfInitCmd(int argc, char **argv)
{
    uint8_t initMode = 1;

    // file system radio config

    if (argc == 3) {
        initMode = (strtoul(argv[2], NULL, 10));

        radioRxOff();
        switch (initMode) {
        case 0:
            radioInit();
            break;
        case 1:
            radioInit802154();
            break;
        case 2:
            radioInitBleMode();
            break;
        default:
            debug_print("system did not support init mode = %d\n", initMode);
            break;
        }
    }

    // handle unexpect input behavior
    if (argc == 2 || argc > 3) {
        debug_print("input error!\n");
        debug_print("Usage:init [0/1]\n");
        debug_print("[0]:Transparent Mode\n");
        debug_print("[1]:802.15.4 Mode\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _rfConfCmd(int argc, char **argv)
{
    if (argc == 2) {
        radioDebug();
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf conf\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _rfRecvCmd(int argc, char **argv)
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

static int _rfSendCmd(int argc, char **argv)
{
    uint32_t testCount = 1;

    if (argc == 3) {
        if (isRadioConfigured() == 0) {
            debug_print("Please exec rf init first!!\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        testCount = strtoul(argv[2], NULL, 10);

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

static int _rfTxpwrCmd(int argc, char **argv)
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
            phy_idx = getPhyIntf();
            rssi    = wise_radio_get_rssi(phy_idx);
            i++;
            debug_print("[%ld][phy %d] rssi = %d\n", i, phy_idx, rssi);
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
            wise_radio_enable_singletone(getPhyIntf(), 0, 127, DISABLE);
        } else {
            pwr_lv = strtoul(argv[2], NULL, 10);

            if (pwr_lv <= 127) {
                wise_radio_enable_singletone(getPhyIntf(), 0, pwr_lv, ENABLE);
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

static int _rfDumpCmd(int argc, char **argv)
{
    uint32_t startAddr = 0;
    uint32_t length    = 0;

    uint32_t reg_addr;
    uint8_t phy_idx = getPhyIntf();
    uint32_t value  = 0;

    if (argc == 4) {
        startAddr = strtoul(argv[2], NULL, 16);
        length    = strtoul(argv[3], NULL, 16);
        reg_addr  = startAddr;
        debug_print("\n");

        while (length) {
            if (phy_idx == 0) {
                value = *(volatile uint32_t *)(reg_addr);
            }
#ifdef ES_DEVICE_TRX_RADIO
            if (phy_idx == 1) {
                value = hal_trx_read_reg32(reg_addr);
            }
#endif
            debug_print("0x%04lx 0x%08lx\n", reg_addr, value);

            reg_addr += 4;
            length   -= 1;
        }

        debug_print("\n");

        fflush(stdout);

    } else {
        // handle unexpect input behavior
        debug_print("input error!\n");
        debug_print("Usage:rf dump addr_hex length_hex\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _rfRegCmd(int argc, char **argv)
{
    uint32_t reg    = 0;
    uint32_t value  = 0;
    uint8_t phy_idx = getPhyIntf();

    if (argc == 4) {
        reg   = strtoul(argv[2], NULL, 16);
        value = strtoul(argv[3], NULL, 16);

        if (phy_idx == 0) {
            *(volatile uint32_t *)(reg) = value;
            debug_print("w %08lx %08lx=> %08lx\n", reg, value, *(volatile uint32_t *)(reg));
        }

#ifdef ES_DEVICE_TRX_RADIO
        if (phy_idx == 1) {
            hal_trx_write_reg32(reg, value);
            debug_print("w %08lx %08lx=> %08lx\n", reg, value, hal_trx_read_reg32(reg));
        }
#endif

    } else if (argc == 3) {
        reg = strtoul(argv[2], NULL, 16);

        if (phy_idx == 0) {
            value = *(volatile uint32_t *)(reg);
        }

#ifdef ES_DEVICE_TRX_RADIO
        if (phy_idx == 1) {
            value = hal_trx_read_reg32(reg);
        }
#endif
        debug_print("r %08lx=> %08lx\n", reg, value);
    } else {
        // handle unexpect input behavior
        debug_print("input error!\n");
        debug_print("Usage:rf reg addr_hex value_hex for write\n");
        debug_print("Usage:rf reg addr_hex for read\n");
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
                laddr[i] = (laddr_l >> (8 * (i))) & 0xFF;
                laddr[i + 4] = (laddr_h >> (8 * (i))) & 0xFF;
            }

            setLongAddress(laddr);
            debug_print("enable rx long address filter: 0x%08lx%08lx\n", laddr_l, laddr_h);
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

static int _drCmd(int argc, char **argv)
{
    uint32_t data_rate;
    int32_t is_support_rate;

    if (argc == 3) {

        data_rate = strtoul(argv[2], NULL, 10);

        is_support_rate = radioGetDatarateIndex(data_rate);

        if (is_support_rate == -1) {
            debug_print("Unsupported data rate! Supported values are:\n");
            debug_print("4800 bps, 12500 bps, 32768 bps, 50000 bps, 100000 bps, 125000 bps, 200000 bps, 250000 bps, 500000 bps, 1000000 bps, 2000000 bps\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        } else {
            radioCfg[defRadioIntf].data_rate = is_support_rate;
            debug_print("Data rate %ld bps\n", data_rate);
        }

        clearRadioConfigured();

        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf dr [data rate (bps)]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _deviCmd(int argc, char **argv)
{
    uint32_t freq_devi;

    if (argc == 3) {

        freq_devi = strtoul(argv[2], NULL, 10);

        radioCfg[defRadioIntf].deviation = freq_devi;
        debug_print("set deviation %ld Hz\n", freq_devi);
        
        clearRadioConfigured();
        
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf devi [freq deviation(Hz)]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}
static int _modCmd(int argc, char **argv)
{
    uint32_t mod;

    if (argc == 3) {
        mod = strtoul(argv[2], NULL, 10);
        
        radioCfg[defRadioIntf].modulation = mod;

        const char *mod_str = "UNKNOWN";
        switch (mod) {
        case 3:
            mod_str = "FSK";
            break;
        case 4:
            mod_str = "GFSK";
            break;
        }

        debug_print("Modulation updated: %lu (%s)\n", mod, mod_str);
        clearRadioConfigured();
        
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("Input error!\n");
        debug_print("Usage: rf mod [modulation(3:FSK, 4:GFSK)]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _phymodeCmd(int argc, char **argv)
{
    uint32_t mode;

    if (argc == 3) {
        mode = strtoul(argv[2], NULL, 10);
        
        //radioCfg[defRadioIntf].phy_mode = mode;
        _wise_radio_set_phy_mode(defRadioIntf, mode);
    
        const char *mode_str = "UNKNOWN";
        switch (mode) {
        case 0:
            mode_str = "TRANSPARENT";
            break;
        case 1:
            mode_str = "802.15.4";
            break;
        case 5:
            mode_str = "MBUS";
            break;
        }

        debug_print("Phy mode updated: %lu (%s)\n", mode, mode_str);
        clearRadioConfigured();
        
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf pmode [phy mode(0:TRANSPARENT, 1:802.15.4, 5:MBUS)]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _swCmd(int argc, char **argv)
{
    uint32_t sw1, sw_len = 4;

    if (argc == 4) {

        sw1    = strtoul(argv[2], NULL, 16);
        sw_len = strtoul(argv[3], NULL, 10);

        radioCfg[defRadioIntf].syncword      = sw1;
        radioCfg[defRadioIntf].sync_word_len = sw_len;

        debug_print("Sync word updated: 0x%08lX (len = %lu bytes)\n", sw1, sw_len);
        clearRadioConfigured();
        
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("Input error!\n");
        debug_print("Usage: rf sw [syncword] [sync_word_len_bytes]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _rfInfoCmd(int argc, char **argv)
{
    if (argc == 2) {
        radioDebug();

        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("input error!\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
}

static int _paCmd(int argc, char **argv)
{
    uint8_t pa_type;

    if (argc == 2) {
        debug_print("pa_type = %d\n", wise_sys_get_pa_type());
    } else if (argc == 3) {
        pa_type = strtoul(argv[2], NULL, 10);

        if (pa_type > 1) {
            debug_print("pa_type is bad = %d (0=10dB/1=14dB)\n", pa_type);
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        wise_sys_set_pa_type(pa_type);
        debug_print("set pa_type = %d\n", wise_sys_get_pa_type());

        clearRadioConfigured();

        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf pa [pa_type(0=10dB/1=14dB)]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _matchCmd(int argc, char **argv)
{
    uint8_t match_type;

    if (argc == 2) {
        debug_print("match_type = %d\n", wise_sys_get_board_match_type());
    } else if (argc == 3) {
        match_type = strtoul(argv[2], NULL, 10);

        if (match_type > 2) {
            debug_print("match_type is bad = %d (0=915/1=868/2=490)\n", match_type);
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        wise_sys_set_board_match_type(match_type);
        debug_print("set match_type = %d\n", wise_sys_get_board_match_type());

        clearRadioConfigured();

        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf match [match_type(0=915/1=868/2=490)]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _freqCmd(int argc, char **argv)
{
    uint32_t ch_freq;

    if (argc == 3) {

        ch_freq = strtoul(argv[2], NULL, 10);

        radioCfg[defRadioIntf].ch_freq_min = ch_freq;
        radioCfg[defRadioIntf].ch_freq_max = ch_freq+2000000;
        radioCfg[defRadioIntf].ch_spacing = 200000;
    
        debug_print("Center frequency= %ld Hz\n", ch_freq);
        clearRadioConfigured();
        
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf freq [channel freq]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _rxMaxLenCmd(int argc, char **argv)
{
    uint32_t rxlen;

    if (argc == 3) {

        rxlen = strtoul(argv[2], NULL, 10);

        radioPktFmt[defRadioIntf].max_pkt_length = rxlen;
    
        debug_print("Packet length = %ld bytes\n", rxlen);
        clearRadioConfigured();
        
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf rxlen [maxmimum packet length]\n");
    }
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _wmbusCmd(int argc, char **argv)
{
    uint8_t mbus_role;
    uint8_t mbus_act;
    uint8_t mbus_mode;

    if (argc == 5) {
        if (!strcmp(argv[2], "s")) {
            mbus_mode = WMBUS_MODE_S1;
        } else if (!strcmp(argv[2], "t")) {
            mbus_mode = WMBUS_MODE_T1;
        } else if (!strcmp(argv[2], "c")) {
            mbus_mode = WMBUS_MODE_C1;
        } else if (!strcmp(argv[2], "r")) {
            mbus_mode = WMBUS_MODE_R2;
        } else {
            debug_print("wmbus mode is bad (wmbus_mode = s/t/c/r)\n");
            goto error;
        }

        mbus_role = strtoul(argv[3], NULL, 10);
        mbus_act  = strtoul(argv[4], NULL, 10);

        if (mbus_role > 1) {
            debug_print("wmbus role is bad = %d (0=other/1=meter)\n", mbus_role);
            goto error;
        }

        if (mbus_act > 1) {
            debug_print("wmbus activity is bad = %d (0=RX/1=TX)\n", mbus_act);
            goto error;
        }

        radioInitWMBUS(mbus_mode, mbus_role, mbus_act);

    } else {
        debug_print("input error!\n");
        debug_print("Usage:rf wmbus [mbus_mode(s/t/c/r)][mbus_role(0=other/1=meter)] [mbus_act(0=RX/1=TX)]\n");
    }

error:
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _testCmd(int argc, char **argv)
{
    uint32_t test_idx;
    //uint32_t freqValue;
    //uint32_t freq;
    uint32_t val;

    if (argc == 3 || argc == 4) {
        if (isRadioConfigured() == 0) {
            debug_print("Please exec rf init first!!\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        test_idx = strtoul(argv[2], NULL, 10);

        if (test_idx == 0) {
            //freqValue = strtoul(argv[3], NULL, 10);
            //radioSetFreq(freqValue);
            //freq = radioGetFreq();

            //debug_print("Set val=%lx\n", val);
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        if (test_idx == 1) {
            val = strtoul(argv[3], NULL, 16);

            radioTest(val);

            debug_print("Set val=%lx\n", val);
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }

    debug_print("input error!\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _worCmd(int argc, char **argv)
{
    if (argc == 4) {
        uint32_t wu_time = strtoul(argv[2], NULL, 10);
        uint32_t rx_to   = strtoul(argv[3], NULL, 10);

        debug_print("Enable WOR function, wutime = %ld\n", wu_time);

        // ===== Enter to Sleep Mode =====
        hal_intf_radio_wor_start(0, wu_time, rx_to);
        // ==== Wake up from WOR mode ====
        // wise_init();
        debug_print("Wakeup from WOR mode\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        goto invalid;
    }

    // handle unexpect input behavior
    if (argc == 2 || argc > 4) {
    invalid:
        debug_print("input error!\n");
        debug_print("Usage:rf wor [on/off]\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _bitorderCmd(int argc, char **argv)
{
    if (argc == 3){
        uint32_t enable = strtoul(argv[2], NULL, 10);
        set_frame_fmt_msb_first((bool)enable);
        debug_print("set frame fmt: %s\n",
            get_frame_fmt_msb_first() ? 
            "Most significant bit transmitted first" : 
            "Least significant bit transmitted first");
        clearRadioConfigured();
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        goto invalid;
    }
    invalid:
        debug_print("input error!\n");
        debug_print("Usage:rf bitorder [0(LSB first)|1(MSB first)]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
}


static int _lencfgCmd(int argc, char **argv)
{
    if (argc == 3){
        WISE_RADIO_PKT_TYPE_T pkt_type = strtoul(argv[2], NULL, 10);
        radio_set_pkt_type(pkt_type);
        debug_print("Length config: %s\n",
            pkt_type ? "VARIABLE_LENGTH" : "FIXED_LENGTH");
        clearRadioConfigured();
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        goto invalid;
    }
    invalid:
        debug_print("input error!\n");
        debug_print("Usage:rf lencfg[0(FIXED Length)|1(VARIABLE LENGTH)]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
}

static void print_crc_config(uint32_t c)
{
    debug_print("  cfg mask: 0x%08lX\n", (unsigned long)c);
    debug_print("    INPUT_BIT_ENDIAN:   %s\n",
        (c & CRC_INPUT_BIT_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
    debug_print("    OUTPUT_BIT_ENDIAN:  %s\n",
        (c & CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
    debug_print("    OUTPUT_BYTE_ENDIAN: %s\n",
        (c & CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
    debug_print("    INCLUDE_HEADER:     %s\n",
        (c & CRC_INCLUDE_HEADER_ON) ? "ON" : "OFF");
    debug_print("    INVERT:             %s\n",
        (c & CRC_INVERT_ON) ? "ON" : "OFF");
}
static const char* crc_poly_to_str(int32_t p)
{
    switch ((WISE_CRC_POLY_SEL_T)p) {
        case CRC_POLYNOMIAL_NONE:      return "NONE";
        case CRC_POLYNOMIAL_CRC8:      return "CRC-8";
        case CRC_POLYNOMIAL_CRC16:     return "CRC-16";
        case CRC_POLYNOMIAL_CCITT_16:  return "CCITT-16";
        case CRC_POLYNOMIAL_KERMIT:    return "KERMIT";
        case CRC_POLYNOMIAL_DNP16:     return "DNP-16";
        case CRC_POLYNOMIAL_BLE24:     return "BLE-24";
        default:                       return "UNKNOWN";
    }
}
static int _crcShowCmd(int argc, char **argv)
{
    (void)argv;
    if (argc != 3) goto invalid;

    uint32_t cfg = radio_get_crc_config();
    int32_t  poly = radio_get_crc_poly();
    uint16_t seed = radio_get_crc_seed();

    debug_print("CRC settings:\n");
    print_crc_config(cfg);
    debug_print("  poly: %s (0x%04X)\n", crc_poly_to_str(poly), util_crc16_get_poly(poly));
    debug_print("  seed: 0x%04X\n", seed);
    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    debug_print("input error!\n");
    debug_print("Usage: rf crc show\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _crcCfgCmd(int argc, char **argv)
{
    if (argc == 4) {
        uint32_t mask;
        if (!parse_u32_dec_hex(argv[3], &mask)) goto invalid;

        radio_set_crc_config(mask);
        debug_print("CRC config updated.\n");
        print_crc_config(mask);
        clearRadioConfigured();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid:
    debug_print("input error!\n");
    debug_print("Usage: rf crc cfg <mask>\n");
    debug_print("  <mask> accepts dec/hex, e.g. 0x19\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _crcPolyCmd(int argc, char **argv)
{
    if (argc == 4) {
        uint32_t v;
        if (!parse_u32_dec_hex(argv[3], &v)) goto invalid;

        WISE_CRC_POLY_SEL_T poly;
        switch (v) {
            case 0: poly = CRC_POLYNOMIAL_CRC16;     break;
            case 1: poly = CRC_POLYNOMIAL_CCITT_16;  break;
            default: goto invalid;
        }

        if (!radio_set_crc_poly(poly)) goto invalid;
        debug_print("CRC poly set: %s (%lu)\n", crc_poly_to_str(poly), (unsigned long)v);
        clearRadioConfigured();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid:
    debug_print("input error!\n");
    debug_print("Usage: rf crc poly <id>\n");
    debug_print("  0 = CRC16, 1 = CCITT-16\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _crcSeedCmd(int argc, char **argv)
{
    if (argc == 4) {
        uint16_t seed;
        if (!parse_u16_dec_hex(argv[3], &seed)) goto invalid;

        radio_set_crc_seed(seed);
        debug_print("CRC seed set to 0x%04X\n", seed);
        clearRadioConfigured();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid:
    debug_print("input error!\n");
    debug_print("Usage: rf crc seed <value>\n");
    debug_print("  examples: rf crc seed 0xFFFF  |  rf crc seed 0\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _crcSetCmd(int argc, char **argv)
{
    if (argc == 4) {
        uint32_t mask;
        if (!parse_u32_dec_hex(argv[3], &mask)) goto invalid;

        radio_crc_set_flags(mask);
        uint32_t now = radio_get_crc_config();
        debug_print("CRC flags SET (OR): 0x%08lX\n", (unsigned long)mask);
        print_crc_config(now);
        clearRadioConfigured();
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    debug_print("input error!\n");
    debug_print("Usage: rf crc set <mask>\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _crcClrCmd(int argc, char **argv)
{
    if (argc == 4) {
        uint32_t mask;
        if (!parse_u32_dec_hex(argv[3], &mask)) goto invalid;

        radio_crc_clr_flags(mask);
        uint32_t now = radio_get_crc_config();
        debug_print("CRC flags CLR (AND ~): 0x%08lX\n", (unsigned long)mask);
        print_crc_config(now);
        clearRadioConfigured();
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    debug_print("input error!\n");
    debug_print("Usage: rf crc clr <mask>\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _crcCmd(int argc, char **argv)
{
    if (argc < 3) goto invalid;

    const char *sub = argv[2];
    if      (strcmp(sub, "show") == 0) return _crcShowCmd(argc, argv);
    else if (strcmp(sub, "cfg")  == 0) return _crcCfgCmd(argc, argv);
    else if (strcmp(sub, "poly") == 0) return _crcPolyCmd(argc, argv);
    else if (strcmp(sub, "seed") == 0) return _crcSeedCmd(argc, argv);
    else if (strcmp(sub, "set")  == 0) return _crcSetCmd(argc, argv);
    else if (strcmp(sub, "clr")  == 0) return _crcClrCmd(argc, argv);

invalid:
    debug_print("input error!\n");
    debug_print("Usage:\n");
    debug_print("  rf crc show\n");
    debug_print("  rf crc cfg  <mask>\n");
    debug_print("  rf crc set  <mask>\n");
    debug_print("  rf crc clr  <mask>\n");
    debug_print("  rf crc poly <id>     # 0=CRC16, 1=CCITT-16\n");
    debug_print("  rf crc seed <value>  # e.g. 0xFFFF or 0\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _sgTestCmd(int argc, char **argv)
{
    if (argc == 3) {
        uint32_t enable = strtoul(argv[2], NULL, 10);

        if (enable <= 1) {
            
            set_sg_setting(enable);
            
            debug_print("SG test mode %s.\n", enable ? "enabled" : "disabled");
            clearRadioConfigured();
            
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }

    debug_print("Input error!\n");
    debug_print("Usage: sg [0=disable | 1=enable]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _CntCmd(int argc, char **argv)
{
     if (argc == 3) {
        uint8_t enable = strtoul(argv[2], NULL, 10);

        radio_clear_test_cnt(enable);
        
        return SHLCMD_HIST | SHLCMD_DONE;
  
    }

    debug_print("Input error!\n");
    debug_print("Usage: cnt [0=clear cnt | 1=show cnt]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _ulpldoCmd(int argc, char **argv)
{
     if (argc == 3) {
        uint8_t enable = strtoul(argv[2], NULL, 10);

        _wise_radio_set_ulpldo(enable);
        
        return SHLCMD_HIST | SHLCMD_DONE;
  
    }

    debug_print("Input error!\n");
    debug_print("Usage: ulpldo [0=default | 1=select]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _bodCmd(int argc, char **argv)
{
    uint32_t bod_lv;

    if (argc == 3) {
        if (isRadioConfigured() == 0) {
            debug_print("Please exec rf init first!!\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        if (strcmp(argv[2], "off") == 0) {
            wise_radio_enable_bod(getPhyIntf(), 0, DISABLE);
        } else {
            bod_lv = strtoul(argv[2], NULL, 10);

            if (bod_lv <= 7) {
                wise_radio_enable_bod(getPhyIntf(), bod_lv, ENABLE);
            } else {
                debug_print("bod level is limited to 0-7\n");
            }
        }
    }

    // handle unexpect input behavior
    if (argc == 2 || argc > 3) {
        debug_print("input error!\n");
        debug_print("Usage:rf sing [0-7]/off\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}


#endif /* SHELL_RADIO */

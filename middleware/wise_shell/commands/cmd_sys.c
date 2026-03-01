/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#include <string.h>
#if SHELL_SYS
#include "cmd_sys.h"
#include "wise_sys_api.h"
#include "wise_pmu_api.h"
#include "wise_wutmr_api.h"
#include "wise_gpio_api.h"
#include "hal_intf_pmu.h"
#include "hdl/flctl_er8130.h"
#include "hal_intf_flash.h"
//#include "cmsis/include/regfiles/xip_reg_er8130.h"
#include "wise.h"
#include "shell_func.h"

#if (defined MIDDLEWARE_WISE_TIMER_HUB) && (MIDDLEWARE_WISE_TIMER_HUB == 1)
#include "wise_timer_hub.h"

static void _wutmr_callback(void *context);
static void tmrhubCallback(void*context);
#endif

static int _sleepCmd(int argc, char **argv);
static int _sclkCmd(int argc, char **argv);
static int _wutmrCmd(int argc, char **argv);
static int _asaradcCmd(int argc, char **argv);
#if (defined MIDDLEWARE_WISE_TIMER_HUB) && (MIDDLEWARE_WISE_TIMER_HUB == 1)
static int _tmrhubCmd(int argc, char **argv);
#endif
static int _battCmd(int argc, char **argv);
//static int _xipclkCmd(int argc, char **argv);
//static int _xipspiCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"sleep", "put SoC into a sleep mode", _sleepCmd},
    {"sclk", "configure clock source of LFOSC", _sclkCmd},
    //{"xipclk", "configure XIP clock divider", _xipclkCmd},
    //{"xipspi", "enable XIP Quad SPI mode", _xipspiCmd},
    {"wutmr", "setup wakeup timer", _wutmrCmd},
    {"asaradc", "setup asaradc", _asaradcCmd},
#if (defined MIDDLEWARE_WISE_TIMER_HUB) && (MIDDLEWARE_WISE_TIMER_HUB == 1)
    {"tmrhub", "wutmr hub test", _tmrhubCmd},
#endif
    {"batt", "read battery voltage", _battCmd},
};

extern WISE_STATUS wise_asaradc_config_detail(WISE_ASARADC_VREF vref, bool fetch_mode, uint8_t timer_mode1, uint8_t timer_mode2, bool daf_enable,
                                uint8_t daf_shift_n, ASARADC_VIN_SEL_T vin_sel);
extern void wise_asaradc_start(void);
extern bool wise_asaradc_is_ready(void);
extern void wise_asaradc_enable_interrupt(bool enable);
extern bool wise_asaradc_check_interrupt(void);
extern void wise_asaradc_clear_interrupt(void);
extern bool wise_asaradc_read_data(WISE_ASARADC_DATA_T *result);
extern bool wise_asaradc_capture_once(ASARADC_VIN_SEL_T vin_sel, WISE_ASARADC_DATA_T *out_data);
extern int32_t wise_asaradc_code_to_mV(uint16_t code);
extern bool wise_asaradc_cal_table_set(const uint16_t *code, const int32_t *mv, uint8_t n, bool clamp);


int SYSCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

uint8_t sleep_mode = 0;
static int _sleepCmd(int argc, char **argv)
{
    if (argc == 2) {
        debug_print("current sleep mode = %d (0=cpu idle/ 1=sleep/ 2=shutdown)\n", sleep_mode);
    } else if (argc >= 3) {
        uint32_t sleepMs = WISE_TIME_FOREVER;

        sleep_mode = strtoul(argv[2], NULL, 10);
        if (argc == 4) {
            sleepMs = strtoul(argv[3], NULL, 10);
        }

        if (sleepMs == 0) {
#define TEST_WAKEUP_PIN 13
            WISE_GPIO_CFG_T gpioCfg = {
                .pin_idx   = TEST_WAKEUP_PIN,
                .mode      = MODE_PIO_FUNC_GPIO,
                .direction = GPIO_DIR_INPUT,
                .int_en    = GPIO_INT_ENABLE,
                .int_type  = GPIO_INT_TYPE_LEVEL_LOW,
            };
            sleepMs = WISE_TIME_FOREVER;

            debug_print("setup GPIO %d to wakeup\n", TEST_WAKEUP_PIN);

            wise_gpio_cfg(&gpioCfg);
        }

        if (sleep_mode >= 3) {
            debug_print("sleep mode is bad = %d\n", sleep_mode);
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        if (sleep_mode == 0) {
            wise_system_idle(sleepMs);
        }

        if (sleep_mode == 1) {
            wise_system_sleep(sleepMs);
        }

        if (sleep_mode == 2) {
            SYS_SHUTDOWN_CFT_T shutdownCfg =
            {
                .wake_src = SHUTDOWN_WAKE_SRC_WUTMR,
                .shutdown_ms = 60000,
            };
            
            wise_sys_enter_shutdown_mode(shutdownCfg);
        }

        //debug_print("set sleep mode = %d(0=cpu idle/ 1=sleep/ 2=shutdown)\n", sleep_mode);
    } else {
        goto invalid_param;
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid_param:
    debug_print("input error!\n");
    debug_print("Usage: sys sleep [mode] [sleep time]\n");
    debug_print("   [mode]          0: CPU idle\n");
    debug_print("                   1: SoC sleep mode\n");
    debug_print("                   2: SoC shutdown mode\n");
    debug_print("   [sleep time]    interval in ms to sleep\n");
    debug_print("                   use 0 to confiture GPIO 13 as wakeup source\n");
    debug_print("                   NULL parameter will put SoC into sleep forever\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

uint8_t sclk_idx       = 0;
uint8_t sclk_work_mode = 0;
static int _sclkCmd(int argc, char **argv)
{
    if ((argc < 2) || (argc > 4)) {
        goto invalid_param;
    }

    if (argc == 2) {
        debug_print("current sclk idx = %d, sclk_work_mode = %d\n", sclk_idx, sclk_work_mode);
        goto invalid_param;
    }

    if (argc <= 4) {
        WISE_LFOSC_SRC_T oscCfg = {0};
        uint8_t doCal           = 0;

        sclk_idx       = strtoul(argv[2], NULL, 10);
        sclk_work_mode = strtoul(argv[3], NULL, 10);

        if (sclk_idx > 3) {
            goto invalid_param;
        }

        if ((sclk_idx == 1 && sclk_work_mode >= 3) || (sclk_idx == 2 && sclk_work_mode >= 4)) {
            goto invalid_param;
        }

        switch (sclk_idx) {
        case 0:
            oscCfg.clk_src = SYS_LFOSC_CLK_DISABLE;
            doCal          = 0;
            printf("set clk src = disable\n");
            break;

        case 1:
            oscCfg.clk_src       = SYS_LFOSC_CLK_SRC_INTERNAL_32K;
            oscCfg.mode.mode_32k = sclk_work_mode;
            doCal                = 1;
            printf("set clk src = internal 32k\n");
            break;

        case 2:
            oscCfg.clk_src       = SYS_LFOSC_CLK_SRC_INTERNAL_16K;
            oscCfg.mode.mode_16k = sclk_work_mode;
            doCal                = 1;
            printf("set clk src = internal 16k\n");
            break;

        case 3:
            oscCfg.clk_src = SYS_LFOSC_CLK_SRC_EXTERNAL_32K;
            doCal          = 0;
            printf("set clk src = external 32K\n");
            break;
        }

        wise_sys_lfosc_clk_src_config(oscCfg);
        if (doCal) {
            wise_sys_lfosc_clk_calibration();
        }

        debug_print("set LFOSC clk=%d, mode=%d\n", sclk_idx, sclk_work_mode);
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid_param:
    debug_print("Invalid params:\n");
    debug_print("Usage:sys sclk [clk_src] [mode] \n");
    debug_print("   clk_src:    0=disable\n");
    debug_print("               1=internal 32K\n");
    debug_print("               2=internal 16K\n");
    debug_print("               3=external 32K\n");
    debug_print("   mode:       for internal 32K\n");
    debug_print("                   0: 32K mode\n");
    debug_print("                   1: 16K mode\n");
    debug_print("                   2: 8K mode\n");
    debug_print("               for internal 16K\n");
    debug_print("                   0: Temparature compensation\n");
    debug_print("                   1: 32K mode\n");
    debug_print("                   2: Low power\n");
    debug_print("                   3: Low power 0.6V\n");
    debug_print("               not used for external 32K\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static uint8_t wuTmrRunning = 0;
static uint8_t wuTmrMode    = 0;
void _wutmr_callback(void *context)
{
    debug_print("wutmr counter=%lu\n", wise_wutmr_get_counter());

    if (wuTmrMode == WISE_WUTMR_ONE_SHOT) {
        wise_wutmr_disable_int();
        wuTmrRunning = 0;
    }
}

static int _wutmrCmd(int argc, char **argv)
{
    if (argc < 3) {
        goto invalid_param;
    }

    if (0 == strcmp(argv[2], "start")) {
        uint32_t mode     = 0;
        uint32_t interval = 0;

        mode     = strtoul(argv[3], NULL, 10);
        interval = strtoul(argv[4], NULL, 10);

        if (((mode != 0) && (mode != 1)) || (interval == 0)) {
            goto invalid_param;
        }

        if (wuTmrRunning == 0) {
            wuTmrMode = mode;

            wise_wutmr_register_callback(_wutmr_callback, NULL);
            wise_wutmr_enable_int();
            wise_wutmr_start(interval, mode);

            wuTmrRunning = 1;

            debug_print("wutmr start mode=%lu interval=%lu counter=%lu\n", mode, interval, wise_wutmr_get_counter());
        } else {
            debug_print("wutmr is already running\n");
        }
    } else if (0 == strcmp(argv[2], "stop")) {
        wise_wutmr_disable_int();
        wise_wutmr_stop();
        wuTmrRunning = 0;

        debug_print("wutmr terminated\n");
    } else if (0 == strcmp(argv[2], "counter")) {
        uint32_t counter = (uint32_t)wise_wutmr_get_counter();

        debug_print("counter=%lu\n", (uint32_t)counter);
    } else {
        goto invalid_param;
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid_param:
    debug_print("Invalid params:\n");
    debug_print("Usage: sys wutmr [start/stop/counter] [options]\n");
    debug_print("   start [type] [interval_ms]\n");
    debug_print("       type:       0: one shot, 1: periodic\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}
static void asaradc_print_usage(void)
{
    printf("Usage:\n"
           "  asaradc init\n"
           "  asaradc config <verf:0(1.6v)|1(2.4v)> \n"
           "                 <fetch_mode:0|1> <timer_mode1:u8> <timer_mode2:u8> \n"
           "                 <daf_en:0|1> <daf_shift_n:u8> <vin_sel:u8>\n"
           "  asaradc start\n"
           "  asaradc ready            (print 0/1)\n"
           "  asaradc irq on|off|check|clear\n"
           "  asaradc read             (blocking-if-ready else fail)\n"
           "  asaradc once <vin_sel:u8>\n"
           "Notes:\n"
           "  - read/once will print: 12bit=<val> 27bit=<val>\n"
           "  asaradc calset requires >=2 points (code,mV pairs)\n"
           "Notes:\n"
           "  - Example: asaradc calset 121 69 704 298 1792 728 3729 1496\n"
           "             (defines piecewise calibration table)\n");
}

static bool parse_u8(const char *s, uint8_t *out)
{
    unsigned v = 0;
    if (!s || !out) {
        return false;
    }
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        return (sscanf(s, "%x", &v) == 1) && (*out = (uint8_t)v, true);
    }
    return (sscanf(s, "%u", &v) == 1) && (*out = (uint8_t)v, true);
}

int _asaradcCmd(int argc, char **argv)
{
    if (argc < 2) {
        asaradc_print_usage();
        return -1;
    }

    const char *sub = argv[2];

    /* asaradc init */
    if (strcmp(sub, "init") == 0) {
        WISE_STATUS st = wise_asaradc_init();
        if (st != WISE_SUCCESS) {
            printf("asaradc: init failed (%d)\n", (int)st);
            return -1;
        }
        printf("asaradc: init OK\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    /* asaradc config <ver> <scaler_en> <scaler_lv> <fetch> <t1> <t2> <daf_en> <daf_shift> <vin> */
    if (strcmp(sub, "config") == 0) {
        if (argc < 9) {
            asaradc_print_usage();
            return -1;
        }
        uint8_t verf, fetch_u8, t1, t2, daf_en_u8, daf_shift, vin;
        if (!parse_u8(argv[3], &verf) || !parse_u8(argv[4], &fetch_u8) || !parse_u8(argv[5], &t1) || !parse_u8(argv[6], &t2) ||
            !parse_u8(argv[7], &daf_en_u8) || !parse_u8(argv[8], &daf_shift) || !parse_u8(argv[9], &vin)) {
            printf("asaradc: config parse error\n");
            return -1;
        }
        bool verf_mode  = (verf != 0);
        bool fetch_mode = (fetch_u8 != 0);
        bool daf_enable = (daf_en_u8 != 0);

        WISE_STATUS st = wise_asaradc_config_detail(verf_mode, fetch_mode, t1, t2, daf_enable, daf_shift, vin);
        if (st != WISE_SUCCESS) {
            printf("asaradc: config failed (%d)\n", (int)st);
            return -1;
        }
        printf("asaradc: config OK (verf = %u fetch=%u t1=%u t2=%u daf_en=%u daf_shift=%u vin=%u)\n", (unsigned)verf, (unsigned)fetch_u8,
               (unsigned)t1, (unsigned)t2, (unsigned)daf_en_u8, (unsigned)daf_shift, (unsigned)vin);
        return SHLCMD_HIST | SHLCMD_DONE;
        ;
    }

    /* asaradc start */
    if (strcmp(sub, "start") == 0) {
        wise_asaradc_start();
        printf("asaradc: start\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    /* asaradc ready */
    if (strcmp(sub, "ready") == 0) {
        bool rdy = wise_asaradc_is_ready();
        printf("%d\n", rdy ? 1 : 0);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    /* asaradc irq on|off|check|clear */
    if (strcmp(sub, "irq") == 0) {
        if (argc < 3) {
            asaradc_print_usage();
            return -1;
        }
        const char *op = argv[3];
        if (strcmp(op, "on") == 0) {
            wise_asaradc_enable_interrupt(true);
            printf("asaradc: irq enabled\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        } else if (strcmp(op, "off") == 0) {
            wise_asaradc_enable_interrupt(false);
            printf("asaradc: irq disabled\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        } else if (strcmp(op, "check") == 0) {
            bool ms = wise_asaradc_check_interrupt();
            printf("%d\n", ms ? 1 : 0);
            return SHLCMD_HIST | SHLCMD_DONE;
        } else if (strcmp(op, "clear") == 0) {
            wise_asaradc_clear_interrupt();
            printf("asaradc: irq cleared\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        } else {
            asaradc_print_usage();
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }

    /* asaradc read */
    if (strcmp(sub, "read") == 0) {
        WISE_ASARADC_DATA_T d;
        if (!wise_asaradc_read_data(&d)) {
            printf("asaradc: not ready\n");
            return -1;
        }
        uint16_t code = d.data_12bit;
        int32_t mv    = wise_asaradc_code_to_mV(code);

        printf("12bit=%u 27bit=%u -> %ld mV\n", (unsigned)code, (unsigned)d.data_27bit, (long)mv);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    /* asaradc once <vin> */
    if (strcmp(sub, "once") == 0) {
        if (argc < 3) {
            asaradc_print_usage();
            return -1;
        }
        uint8_t vin;
        if (!parse_u8(argv[3], &vin)) {
            printf("asaradc: once parse error\n");
            return -1;
        }
        WISE_ASARADC_DATA_T d;
        if (!wise_asaradc_capture_once(vin, &d)) {
            printf("asaradc: once failed\n");
            return -1;
        }
        uint16_t code = d.data_12bit;
        int32_t mv    = wise_asaradc_code_to_mV(code);

        printf("12bit=%u 27bit=%u -> %ld mV\n", (unsigned)code, (unsigned)d.data_27bit, (long)mv);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    /* asaradc calset <code1> <mv1> <code2> <mv2> ... */
    if (strcmp(sub, "calset") == 0) {
        if ((argc - 3) < 4 || ((argc - 3) % 2) != 0) {
            printf("asaradc: calset requires at least 2 code/mV pairs\n");
            return -1;
        }

        size_t num_points = (argc - 3) / 2;
        uint16_t code_list[6];
        int32_t mv_list[6];

        if (num_points > 6) {
            printf("asaradc: too many points (max 6)\n");
            return -1;
        }

        for (size_t i = 0; i < num_points; ++i) {
            if (!parse_u16_dec_hex(argv[3 + i * 2], &code_list[i]) || !parse_u16_dec_hex(argv[4 + i * 2], (uint16_t *)&mv_list[i])) {
                printf("asaradc: parse error at pair %u\n", i);
                return -1;
            }
        }

        bool ok = wise_asaradc_cal_table_set(code_list, mv_list, num_points, true); // clamp = true
        if (!ok) {
            printf("asaradc: calset failed (bad input?)\n");
            return -1;
        }

        printf("asaradc: calset OK (%u points)\n", num_points);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    asaradc_print_usage();
    return SHLCMD_HIST | SHLCMD_DONE;
}

#if (defined MIDDLEWARE_WISE_TIMER_HUB) && (MIDDLEWARE_WISE_TIMER_HUB == 1)
struct
{
    int32_t ch;
    TIMER_HUB_MODE_T mode;
    uint32_t startTick;
    uint32_t periodMs;
} tmrhubChannels[WISE_TIMER_HUB_CH_NUM] =
{
    {-1, 0, 0, 0},
    {-1, 0, 0, 0},
    {-1, 0, 0, 0},
    {-1, 0, 0, 0},                
};

static void tmrhubCallback(void*context)
{
    int slot = (int)context;
    uint32_t now = wise_wutmr_get_counter();
    uint32_t periodTick = wise_wutmr_ms_to_clk(tmrhubChannels[slot].periodMs);
    uint32_t actPeriodTick = (uint32_t)(now - tmrhubChannels[slot].startTick);
    
    //printf("ch=%d period=%lu occur=%lu\n", tmrhubChannels[slot].ch, wise_wutmr_ms_to_clk(tmrhubChannels[slot].periodMs), (uint32_t)(now - tmrhubChannels[slot].startTick));
    printf("ch%ld p=%lu diff=%ld\n", tmrhubChannels[slot].ch, periodTick, (int32_t)(actPeriodTick - periodTick));
                
    if(tmrhubChannels[slot].mode == WISE_WUTMR_PERIODIC)
    {
        tmrhubChannels[slot].startTick = now;
    }
}

int getEmptySlot()
{
    int newSlot = -1;
    int i;

    for(i = 0; i < WISE_TIMER_HUB_CH_NUM; i++)
    {
        if(tmrhubChannels[i].ch == -1)
        {
            newSlot = i;
            break;
        }
    }

    return newSlot;
}

void startRandomTmr()
{
    int i;

    for(i = 0; i < WISE_TIMER_HUB_CH_NUM; i++)
    {
        
    }
}

static int _tmrhubCmd(int argc, char **argv)
{
    if(argc < 3)
        goto invalid;

    if(0 == strcmp(argv[2], "start"))
    {
        uint32_t mode = 0, period = 0;
        int newSlot = getEmptySlot();
        int tmrCh = -1;
        
        if(argc != 5)
            goto invalid;

        mode = strtoul(argv[3], NULL, 10);
        period = strtoul(argv[4], NULL, 10);

        if((mode != 0) && (mode != 1))
            goto invalid;
        if(period == 0)
            goto invalid;

        if(newSlot == -1)
        {
            printf("No empty timer slot\n");
            goto finish;
        }

        tmrCh = wise_timer_hub_alloc_ch(tmrhubCallback, (void*)newSlot);
        if(tmrCh < 0)
        {
            printf("failed to alloc timer hub channel\n");
            goto finish;
        }

        tmrhubChannels[newSlot].ch = tmrCh;
        tmrhubChannels[newSlot].mode = mode;
        tmrhubChannels[newSlot].startTick = wise_wutmr_get_counter();
        tmrhubChannels[newSlot].periodMs = period;

        wise_timer_hub_start_ch(tmrCh, mode, period);

        printf("timer hub started slot=%d ch=%d\n", newSlot, tmrCh);
    }
    else if(0 == strcmp(argv[2], "stop"))
    {
        
    }
    else if(0 == strcmp(argv[2], "now"))
    {
        printf("cur tick=%lu\n", wise_wutmr_get_counter());
    }
    else
        goto invalid;

finish:
    return SHLCMD_HIST | SHLCMD_DONE;
    
invalid:
    printf("Invlalid params.\n");
    printf("Usage:\n");
    printf("    sys tmrhub start [mode: 0=one_shot 1=peridical] [period_ms]\n");
    printf("    sys tmrhub stop [slot]\n");
    printf("    sys tmrhub now\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}
#endif

static int _battCmd(int argc, char **argv)
{
    static uint8_t inited = 0;
    uint16_t sadcVal = 0;
    uint32_t readCount = 0;
    int i;

    if(argc <= 2)
        goto invalid_params;
    if(argc >= 3)
        readCount = strtoul(argv[2], NULL, 10);

    if(readCount == 0)
        readCount = 1;
    
    if(!inited)
    {
        wise_asaradc_init();
        wise_asaradc_config(ASARADC_VREF_1P6V);

        inited = 1;
    }

    for(i = 0; i < readCount; i++)
    {
        sadcVal = 0;
        if(WISE_SUCCESS == wise_asaradc_read_input(ASARADC_VIN_BATTERY, &sadcVal))
        {
            printf("%d sadc=%u\n", i, sadcVal);
        }
        else
        {
            printf("sadc timeout\n");
        }
    }
    
    return SHLCMD_HIST | SHLCMD_DONE;

invalid_params:
    printf("Usage:\n");
    printf("    sys batt [count]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

#if 0

static int _xipclkCmd(int argc, char **argv)
{
/*
    if (argc == 2) {
        debug_print("Usage: sys xipclk [divider]\n");
        debug_print("   divider:    0 = 40MHz (1x)\n");
        debug_print("               1 = 20MHz (2x)\n");
        debug_print("               2 = 10MHz (4x)\n");
        debug_print("               3 = 5MHz  (8x)\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (argc == 3) {
        uint8_t clk_div;

        clk_div = strtoul(argv[2], NULL, 10);

        if (clk_div > 3) {
            goto invalid_param;
        }

        hal_intf_pmu_set_xip_clk(clk_div);

        debug_print("XIP clock set to divider=%d (%s)\n", clk_div,
                    clk_div == 0 ? "40MHz" :
                    clk_div == 1 ? "20MHz" :
                    clk_div == 2 ? "10MHz" : "5MHz");

        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid_param:
    debug_print("Invalid parameter!\n");
    debug_print("Usage: sys xipclk [divider]\n");
    debug_print("   divider:    0 = 40MHz (1x)\n");
    debug_print("               1 = 20MHz (2x)\n");
    debug_print("               2 = 10MHz (4x)\n");
    debug_print("               3 = 5MHz  (8x)\n");
*/
    return SHLCMD_HIST | SHLCMD_DONE;
}


static int _xipspiCmd(int argc, char **argv)
{
#if 0
    uint8_t mode = XIP_SPI_MODE_QUAD_IO;
    const char *mode_name = "Quad I/O";
    uint8_t flash_cmd = 0xEB;

    if (argc == 2) {
        debug_print("Usage: sys xipspi [mode]\n");
        debug_print("\n");
        debug_print("Available modes:\n");
        debug_print("  normal  - Normal SPI (1 data line, cmd=0x03)\n");
        debug_print("  dual    - Dual Output (2 data lines, cmd=0x3B)\n");
        debug_print("  quad    - Quad Output (4 data lines, cmd=0x6B)\n");
        debug_print("  dualio  - Dual I/O (2 addr+data lines, cmd=0xBB)\n");
        debug_print("  quadio  - Quad I/O (4 addr+data lines, cmd=0xEB, default)\n");
        debug_print("\n");
        debug_print("If no mode specified, defaults to Quad I/O mode\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    /*
    if (argc == 3) {
        if (strcmp(argv[2], "normal") == 0) {
            mode = XIP_SPI_MODE_NORMAL;
            mode_name = "Normal SPI";
            flash_cmd = 0x03;
        }
        else if (strcmp(argv[2], "dual") == 0) {
            mode = XIP_SPI_MODE_DUAL_OUT;
            mode_name = "Dual Output";
            flash_cmd = 0x3B;
        }
        else if (strcmp(argv[2], "quad") == 0) {
            mode = XIP_SPI_MODE_QUAD_OUT;
            mode_name = "Quad Output";
            flash_cmd = 0x6B;
        }
        else if (strcmp(argv[2], "dualio") == 0) {
            mode = XIP_SPI_MODE_DUAL_IO;
            mode_name = "Dual I/O";
            flash_cmd = 0xBB;
        }
        else if (strcmp(argv[2], "quadio") == 0) {
            mode = XIP_SPI_MODE_QUAD_IO;
            mode_name = "Quad I/O";
            flash_cmd = 0xEB;
        }
        else {
            goto invalid_param;
        }
    }
    else if (argc > 3) {
        goto invalid_param;
    }
    */
    
    hal_intf_xip_set_spi_mode(2);

    debug_print("XIP %s mode enabled\n", mode_name);
    debug_print("  - Flash read command: 0x%02X\n", flash_cmd);
    if(mode == XIP_SPI_MODE_QUAD_OUT && mode == XIP_SPI_MODE_QUAD_IO) {
        debug_print("  - Flash QE bit set (Quad mode enabled)\n");
    }
#endif
    return SHLCMD_HIST | SHLCMD_DONE;

/*
invalid_param:
    debug_print("Invalid parameter!\n");
    debug_print("Usage: sys xipspi [mode]\n");
    debug_print("Valid modes: normal, dual, quad, dualio, quadio\n");
    debug_print("Type 'sys xipspi' for detailed help\n");

    return SHLCMD_HIST | SHLCMD_DONE;*/
}
#endif

#endif /* SHELL_GPIO */

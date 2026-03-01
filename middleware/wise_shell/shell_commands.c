/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "shell_commands.h"
#include "wise_core.h"
#include "api/wise_sys_api.h"
#include "cmsis/include/er8xxx.h"
#include "es_platform_components.h"
#include "shell.h"

#if SHELL_FILESYSTEM
#include "commands/cmd_fs.h"
#endif
#if SHELL_RADIO
#include "commands/cmd_rf.h"
#ifdef WISE_WMBUS_SW_HELPER
#include "commands/cmd_wmbus.h"
#endif
#endif
#if SHELL_TIMER
#include "commands/cmd_timer.h"
#endif
#if SHELL_RTC
#include "commands/cmd_rtc.h"
#endif
#if SHELL_SPI
#include "commands/cmd_spi.h"
#endif
#if SHELL_I2C
#include "commands/cmd_i2c.h"
#endif
#if SHELL_TRNG
#include "commands/cmd_trng.h"
#endif
#if SHELL_WDT
#include "commands/cmd_wdt.h"
#endif
#if SHELL_PWM
#include "commands/cmd_pwm.h"
#endif
#if SHELL_PWMSLOW
#include "commands/cmd_pwmslow.h"
#endif
#if SHELL_GPIO
#include "commands/cmd_gpio.h"
#endif
#ifdef SHELL_USER

#endif
#if SHELL_PIO
#include "commands/cmd_pio.h"
#endif
#if SHELL_CRYPTO
#include "commands/cmd_crypto.h"
#endif
#if SHELL_NFC
#include "commands/cmd_nfc.h"
#endif
#if SHELL_EFUSE
#include "commands/cmd_efuse.h"
#endif
#if SHELL_SYS
#include "commands/cmd_sys.h"
#endif
#if (defined SHELL_KERMIT) && (SHELL_KERMIT == 1)
#include "commands/cmd_kermit.h"
#endif


static int _rebootCmd(int argc, char **argv);
static int _abortCmd(int argc, char **argv);
static int _TBDCmd(int argc, char **argv);
static int _dumpCmd(int argc, char **argv);
static int _read8Cmd(int argc, char **argv);
static int _write8Cmd(int argc, char **argv);
static int _read32Cmd(int argc, char **argv);
static int _write32Cmd(int argc, char **argv);
static int _memCmd(int argc, char **argv);
#if 0
extern int UserCommandHandler(int argc, char **argv);
#endif

const struct shellCommand *g_cmdList;
size_t g_cmdListSize;

struct shellCommand *activeCmdList = NULL;
size_t activeCmdListSize           = 0;

#if (SHELL_EXTERNAL_CMD_TABLE == DISABLE)
static struct shellCommand m_shlcmdlst[] = {
    // basic command set
    {"help", "help", _helpCmd},
    {"dump", "dump addr(HEX) length(HEX)", _dumpCmd},
    {"r8", "dump addr(HEX)", _read8Cmd},
    {"w8", "dump addr(HEX)", _write8Cmd},
    {"r32", "dump addr(HEX)", _read32Cmd},
    {"w32", "dump addr(HEX)", _write32Cmd},
    {"mem", "read or write memory", _memCmd},
    {"reset", "reboot system", _rebootCmd},
    {"<C-g>", "abort", _abortCmd},
    {"<C-c>", "TBD", _TBDCmd},
#if SHELL_FILESYSTEM
    {"fs", "<fs help>", FileSystemCommandHandler},
#endif
#if SHELL_RADIO
    {"rf", "<rf help>", RadioCommandHandler},
#ifdef WISE_WMBUS_SW_HELPER
    {"wmbus", "<rf help>", WmbusCommandHandler},
#endif
#endif
#if SHELL_TIMER
    {"timer", "<timer help>", TimerCommandHandler},
#endif
#if SHELL_RTC
    {"rtc", "<rtc help>", RTCCommandHandler},
#endif
#if SHELL_SPI
    {"spi", "<spi help>", SPICommandHandler},
#endif
#if SHELL_I2C
    {"i2c", "<i2c help>", I2CCommandHandler},
#endif
#if SHELL_TRNG
    {"trng", "<trng help>", TRNGCommandHandler},
#endif
#if SHELL_WDT
    {"wdt", "<wdt help>", WDTCommandHandler},
#endif
#if SHELL_PWM
    {"pwm", "<pwm help>", PWMCommandHandler},
#endif
#if SHELL_PWMSLOW
    {"pwmslow", "<pwmslow help>", PWMSLOWCommandHandler},
#endif
#if SHELL_GPIO
    {"gpio", "<gpio help>", GPIOCommandHandler},
#endif
#if SHELL_PIO
    {"pio", "<pio help>", PIOCommandHandler},
#endif
#if SHELL_NFC
    {"nfc", "<nfc help>", NFCCommandHandler},
#endif
#if SHELL_EFUSE
    {"efuse", "<efuse help>", EFUSECommandHandler},
#endif
#if SHELL_CRYPTO
    {"crypto", "<crypto help>", CRYPTOCommandHandler},
#endif
#if SHELL_SYS
    {"sys", "<sys help>", SYSCommandHandler},
#endif
#if (defined SHELL_KERMIT) && (SHELL_KERMIT == 1)
    {"kermit", "<kermit help>", KermitCommandHandler},
#endif
};
#endif //(SHELL_EXTERNAL_CMD_TABLE == DISABLE)

static int _rebootCmd(int argc, char **argv)
{
    // NVIC_SystemReset();
    wise_sys_chip_reset();

    return SHLCMD_DONE;
}

static int _dumpCmd(int argc, char **argv)
{
    uint32_t addr;
    uint32_t len;
    if (argc == 3) {
        addr = strtoul(argv[1], NULL, 16);
        len  = strtoul(argv[2], NULL, 16);
        printf("addr=%lx len=%lx\n", addr, len);
        dump_buffer((unsigned char *)addr, len);
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        printf("input error!\n");
        printf("Usage:dump addr(HEX) length(HEX)\n");
    }
    return SHLCMD_DONE;
}

static int _read8Cmd(int argc, char **argv)
{
    uint32_t reg;
    uint8_t value;

    if (argc == 2) {
        reg   = strtoul(argv[1], NULL, 16);
        value = *(volatile unsigned char *)(reg);
        printf("r %08lx=> %02x\n", reg, value);
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        printf(INVALID_PARAM_MESSAGE);
        printf("r8 addr_hex\n");
    }
    return SHLCMD_DONE;
}

static int _write8Cmd(int argc, char **argv)
{
    uint32_t reg   = 0;
    uint32_t value = 0;
    uint8_t wVal   = 0;

    if (argc == 3) {
        reg                              = strtoul(argv[1], NULL, 16);
        value                            = strtoul(argv[2], NULL, 16);
        wVal                             = value & 0xff;
        *(volatile unsigned char *)(reg) = wVal;
        printf("w %08lx %02x=> %02x\n", reg, wVal, *(volatile unsigned char *)(reg));
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        printf("input error!\n");
        printf("w8 addr_hex value_hex\n");
    }
    return SHLCMD_DONE;
}

static int _read32Cmd(int argc, char **argv)
{
    uint32_t reg;
    uint32_t value;

    if (argc == 2) {
        reg   = strtoul(argv[1], NULL, 16);
        value = *(volatile uint32_t *)(reg);
        printf("r %08lx=> %08lx\n", reg, value);
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        printf("input error!\n");
        printf("r32 addr_hex\n");
    }
    return SHLCMD_DONE;
}

static int _write32Cmd(int argc, char **argv)
{
    uint32_t reg;
    uint32_t value;

    if (argc == 3) {
        reg                         = strtoul(argv[1], NULL, 16);
        value                       = strtoul(argv[2], NULL, 16);
        *(volatile uint32_t *)(reg) = value;
        printf("w %08lx %08lx=> %08lx\n", reg, value, *(volatile uint32_t *)(reg));
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        printf("input error!\n");
        printf("w32 addr_hex value_hex\n");
    }
    return SHLCMD_DONE;
}

bool isValidMemoryAddress(uint32_t addr)
{
    if ((addr <= 0x000FFFFF) ||                       // EFLASH
        (addr >= 0x20000000 && addr <= 0x20007FFF) || // SRAM
        (addr >= 0x40008000 && addr <= 0x40008FFF) || // CACHE
        (addr >= 0x40009000 && addr <= 0x40009FFF) || // PCRMU
        (addr >= 0x4000A000 && addr <= 0x4000AFFF) || // EXT_PMU
        (addr >= 0x4000F000 && addr <= 0x4000FFFF) || // SYS_CTRL
        (addr >= 0x40011000 && addr <= 0x40011FFF) || // ANA_CTRL
        (addr >= 0x40012000 && addr <= 0x40012FFF) || // EFLASH_CTRL
        (addr >= 0x40013000 && addr <= 0x40013FFF) || // eFUSE_CTRL
        (addr >= 0x40014000 && addr <= 0x40014FFF) || // IO_CTRL
        (addr >= 0x40016000 && addr <= 0x40016FFF) || // RTC
        (addr >= 0x40017000 && addr <= 0x40017FFF) || // PWM_SLOW
        (addr >= 0x40018000 && addr <= 0x40018FFF) || // WUTMR
        (addr >= 0x40019000 && addr <= 0x40019FFF) || // WDOG
        (addr >= 0x4001A000 && addr <= 0x4001AFFF) || // GPTMR 0
        (addr >= 0x4001B000 && addr <= 0x4001BFFF) || // GPTMR 1
        (addr >= 0x4001C000 && addr <= 0x4001CFFF) || // GPTMR 2
        (addr >= 0x4001D000 && addr <= 0x4001DFFF) || // GPTMR 3
        (addr >= 0x4001E000 && addr <= 0x4001EFFF) || // GPTMR 4
        (addr >= 0x4001F000 && addr <= 0x4001FFFF) || // GPTMR 5
        (addr >= 0x40020000 && addr <= 0x40020FFF) || // MAC
        (addr >= 0x40021000 && addr <= 0x40021FFF) || // NFC
        (addr >= 0x40028000 && addr <= 0x40028FFF) || // BBP
        (addr >= 0x40030000 && addr <= 0x40030FFF) || // UART0
        (addr >= 0x40031000 && addr <= 0x40031FFF) || // UART1
        (addr >= 0x40032000 && addr <= 0x40032FFF) || // UART2
        (addr >= 0x40033000 && addr <= 0x40034FFF) || // SPI0
        (addr >= 0x40034000 && addr <= 0x40034FFF) || // SPI1
        (addr >= 0x40035000 && addr <= 0x40035FFF) || // I2C0
        (addr >= 0x40036000 && addr <= 0x40036FFF) || // I2C1
        (addr >= 0x40037000 && addr <= 0x40037FFF) || // AES-256
        (addr >= 0x40038000 && addr <= 0x40038FFF) || // SHA256
        (addr >= 0x40039000 && addr <= 0x40039FFF) || // TRNG
        (addr >= 0x4003A000 && addr <= 0x4003AFFF) || // PWM
        (addr >= 0x4003B000 && addr <= 0x4003BFFF) || // DMA_CTRL
        (addr >= 0x4003C000 && addr <= 0x4003CFFF) || // XIP_HW_AST
#if defined(CONFIG_ER8130_TC04)
        (addr >= 0x4003D000 && addr <= 0x4003DFFF) || // PERIPH_IO
#endif
        (addr >= 0x40040000 && addr <= 0x40040FFF)) { // Modem CSR
        return true;
    }
    return false;
}

static int _memCmd(int argc, char **argv)
{
    uint32_t address;
    uint32_t value;
    uint32_t count;
    uint32_t i;

    if (argc < 2 || argc > 4) {
        printf("Input error! Usage:\n");
        printf("  read: mem [address]\n");
        printf("  continuous read: mem [address] -c [count in HEX]\n");
        printf("  write: mem [address] [value]\n");
        return SHLCMD_DONE | SHLCMD_HIST;
    }

    address = strtoul(argv[1], NULL, 16);
    if (isValidMemoryAddress(address) == false) {
        printf("Invalid address!\nmem");
        return SHLCMD_DONE | SHLCMD_HIST;
    }
    if (argc == 2) {
        // Read operation
        value = *(volatile uint32_t *)(address);
        printf("Read: %08lx => %08lx\n", address, value);
    } else if (argc == 3) {
        // Write operation: mem [address] [value]
        if (strcmp(argv[2], "-c") == 0) {
            printf("Error: Missing count after '-c'.\n");
            return SHLCMD_DONE | SHLCMD_HIST;
        } else {
            value                           = strtoul(argv[2], NULL, 16);
            *(volatile uint32_t *)(address) = value;
            printf("Write: %08lx %08lx => %08lx\n", address, value, *(volatile uint32_t *)(address));
        }
    } else if (argc == 4) {
        if (strcmp(argv[2], "-c") == 0) {
            // Continuous read operation: mem [address] -c [count]
            count = strtoul(argv[3], NULL, 16);
            for (i = 0; i < count; i++) {
                value = *(volatile uint32_t *)(address + (i << 2));
                printf("Read: %08lx => %08lx\n", (address + (i << 2)), value);
            }
        } else {
            // Write operation: mem [address] [value]
            value                           = strtoul(argv[2], NULL, 16);
            *(volatile uint32_t *)(address) = value;
            printf("Write: %08lx %08lx => %08lx\n", address, value, *(volatile uint32_t *)(address));
        }
    }

    return SHLCMD_DONE | SHLCMD_HIST;
}

static int _abortCmd(int argc, char **argv)
{
#if SHELL_TIMER
    for (uint8_t i = 0; i < CHIP_TIMER_CHANNEL_NUM; i++) {
        if (wise_timer_is_enabled(i) == ENABLE) {
            if (wise_timer_stop(i) == WISE_SUCCESS) {
                printf("timer %d stopped\n", i);
            } else {
                printf("timer stop failed for channel %d\n", i);
            }

            if (WISE_SUCCESS == wise_timer_close(i)) {
                printf("timer %d callback unregistered\n", i);
            } else {
                printf("timer callback unregister failed for channel %d\n", i);
            }
        }
    }
    printf("Timer abort\n");
#endif
#if SHELL_SPI
    extern volatile uint8_t slave_cmd_handler;
    slave_cmd_handler = 0;
#endif

#if SHELL_I2C
    extern volatile uint8_t i2c_slave_cmd_handler;
    i2c_slave_cmd_handler = 0;
#endif

    return SHLCMD_DONE;
}

static int _TBDCmd(int argc, char **argv)
{
    printf("<C-c>\n");
    return SHLCMD_DONE;
}
int shlcmd_parse(int argc, char **argv)
{
    if (argc <= 0) {
        return 0;
    }

    g_cmdList     = activeCmdList;
    g_cmdListSize = activeCmdListSize;

    for (uint32_t i = 0; i < activeCmdListSize; i++) {
        if (strcmp(activeCmdList[i].mName, argv[0]) == 0) {
            int res;
            res = (activeCmdList[i].mHandler(argc, argv) & (SHLCMD_DONE | SHLCMD_HIST));
            return SHLCMD_KNOWN | res;
        }
    }

    return SHLCMD_UNKNOWN;
}

int _helpCmd(int argc, char **argv)
{
    commonHelpCmd(argc, argv, g_cmdList, g_cmdListSize);
    return SHLCMD_DONE;
}

int commonHelpCmd(int argc, char **argv, const struct shellCommand *cmdList, size_t cmdListSize)
{
    printf("Usage: %s [subcommand]\n", argv[0]);

    for (size_t i = 0; i < cmdListSize; i++) {
        printf("   %-12s %s\n", cmdList[i].mName, cmdList[i].mDesc);
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

int commonCommandHandler(int argc, char **argv, const struct shellCommand *cmdList, size_t cmdListSize)
{
    g_cmdList     = cmdList;
    g_cmdListSize = cmdListSize;

    if (argc == 1) {
        return commonHelpCmd(argc, argv, cmdList, cmdListSize);
    }

    for (size_t i = 0; i < cmdListSize; i++) {
        if (strcmp(argv[1], cmdList[i].mName) == 0) {
            return cmdList[i].mHandler(argc, argv);
        }
    }

    printf("unknown command\n");
    return 0;
}

int shlcmd_init()
{
#if (SHELL_EXTERNAL_CMD_TABLE == DISABLE)
    activeCmdList     = m_shlcmdlst;
    activeCmdListSize = sizeof(m_shlcmdlst) / sizeof(struct shellCommand);
#else
    activeCmdList     = NULL;
    activeCmdListSize = 0;
#endif

    return 0;
}

int shlcmd_set_ext_cmd_table(struct shellCommand *cmdTable, size_t cmdTableSize)
{
#if (SHELL_EXTERNAL_CMD_TABLE == DISABLE)
    return -1;
#endif

    activeCmdList     = cmdTable;
    activeCmdListSize = cmdTableSize;

    return 0;
}

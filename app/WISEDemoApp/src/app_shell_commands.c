/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#include "wise_shell_v2/src/shell.h"
#include "wise_core.h"
#include "wise.h"
#include "wise_sys_api.h"
#include "wise_uart_api.h"
#include <stdio.h>

#define SHELL_UART_CH               0

/* ========================================================================== */
/* Shell Backend                                                              */
/* ========================================================================== */

bool shell_uart_read_char(char *ch)
{
    uint8_t tmp;
    if (wise_uart_read_char(SHELL_UART_CH, &tmp) == WISE_SUCCESS) {
        *ch = (char)tmp;
        return true;
    }
    return false;
}

static void shell_uart_write_str(const char *s)
{
    while (*s) {
        wise_uart_write_char(SHELL_UART_CH, (uint8_t)*s++);
    }
}

/* ========================================================================== */
/* common demo shell init                                                     */
/* ========================================================================== */

void app_shell_init(const char* prompt)
{
    shell_config_t cfg = {
        .read_char = shell_uart_read_char,
        .write_str = shell_uart_write_str,
        .prompt    = NULL,
    };

    cfg.prompt = prompt;

    shell_init(&cfg);
}

/* ========================================================================== */
/* help Command                                                               */
/* ========================================================================== */

static int cmd_help(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    size_t cmd_count = 0;
    const shell_command_t *const *cmds = shell_get_commands(&cmd_count);

    printf("\r\n");
    printf("Available commands:\r\n");

    for (size_t i = 0; i < cmd_count; i++) {
        const shell_command_t *cmd = cmds[i];
        if (!cmd || !cmd->name) {
            continue;
        }

        printf("  %-12s", cmd->name);

        if (cmd->subcmds && cmd->subcmd_count > 0) {
            printf(" <subcmd>");
        }

        if (cmd->help) {
            printf("  - %s", cmd->help);
        }
        printf("\r\n");
    }

    printf("\r\nType '<command>' for subcommands\r\n\r\n");
    return 0;
}

SHELL_CMD_AUTO(help, cmd_help, "Show available commands");

/* ========================================================================== */
/* info Command                                                               */
/* ========================================================================== */

static int cmd_info(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    extern void print_banner(char* appName);

    print_banner((char*)DEMO_APP_NAME);

    return 0;
}

SHELL_CMD_AUTO(info, cmd_info, "Display system info");

/* ========================================================================== */
/* reset Command                                                              */
/* ========================================================================== */

static int cmd_reset(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    wise_sys_chip_reset();

    printf("Failed to chip reset\r\n");
    return 0;
}

SHELL_CMD_AUTO(reset, cmd_reset, "Do chip reset");

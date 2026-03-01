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
#include <stdio.h>

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

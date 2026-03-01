/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __SHELL_COMMANDS_H
#define __SHELL_COMMANDS_H

#include <stdio.h>
#include <string.h>
#include "wise_core.h"

#define SHLCMD_UNKNOWN 1
#define SHLCMD_KNOWN 2
#define SHLCMD_DONE 4
#define SHLCMD_HIST 8

typedef int (*shellCommandHandler)(int argc, char **argv);

typedef struct shellCommand {
    const char *mName;
    const char *mDesc;
    shellCommandHandler mHandler;
} shellCommand;

int shlcmd_init();
int shlcmd_set_ext_cmd_table(struct shellCommand *cmdTable,
                             size_t cmdTableSize);
int shlcmd_parse(int argc, char **argv);
extern const struct shellCommand *g_cmdList;
extern size_t g_cmdListSize;
int _helpCmd(int argc, char **argv);
int commonHelpCmd(int argc, char **argv, const struct shellCommand *cmdList,
                  size_t cmdListSize);
int commonCommandHandler(int argc, char **argv,
                         const struct shellCommand *cmdList,
                         size_t cmdListSize);

#define shell_output(...)                                                      \
    do {                                                                       \
        if (1) {                                                               \
            printf(__VA_ARGS__);                                               \
            fflush(stdout);                                                    \
        }                                                                      \
    } while (0)


#ifndef CLI_PRINTF
#define CLI_PRINTF debug_print
#endif

#endif // __SHELL_COMMANDS_H

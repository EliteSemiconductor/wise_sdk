#ifndef SHELL_CMD_H
#define SHELL_CMD_H

#include <stddef.h>
#include <stdbool.h>

#include "shell.h"

void shell_cmd_exec_line(const char *line, const shell_command_t *const *cmds,
                         size_t cmd_count, shell_write_str_fn write_str);

size_t shell_cmd_collect_matches(const char *prefix, size_t prefix_len,
                                 const shell_command_t *const *cmds,
                                 size_t cmd_count, const char **out,
                                 size_t out_cap);

size_t shell_cmd_collect_sub_matches(const char *cmd_name, const char *prefix,
                                     size_t prefix_len,
                                     const shell_command_t *const *cmds,
                                     size_t cmd_count, const char **out,
                                     size_t out_cap);

const char *shell_cmd_find_help(const char *name,
                                const shell_command_t *const *cmds,
                                size_t cmd_count);

const char *shell_cmd_find_sub_help(const char *cmd_name, const char *sub_name,
                                    const shell_command_t *const *cmds,
                                    size_t cmd_count);

#endif /* SHELL_CMD_H */

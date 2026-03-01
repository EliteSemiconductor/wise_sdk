#include "../shell_config.h"
#include "shell_cmd.h"
#include <string.h>
#include <ctype.h>

/* Parse arguments with quote support ("..." and '...') */
static int _parse_args(char *line, char **argv, int max_args)
{
    int argc        = 0;
    char *write_pos = line;
    char *arg_start = NULL;
    bool in_arg     = false;
    bool in_quote   = false;
    char quote_char = '\0';

    while (*line && argc < max_args) {
        char c = *line;

        if (c == '"' || c == '\'') {
            if (!in_quote) {
                in_quote   = true;
                quote_char = c;
                if (!in_arg) {
                    arg_start    = write_pos;
                    argv[argc++] = arg_start;
                    in_arg       = true;
                }
            } else if (c == quote_char) {
                in_quote   = false;
                quote_char = '\0';
            } else {
                *write_pos++ = c;
            }
            line++;
            continue;
        }

        if (isspace((unsigned char)c) && !in_quote) {
            if (in_arg) {
                *write_pos++ = '\0';
                in_arg       = false;
            }
        } else {
            if (!in_arg) {
                arg_start    = write_pos;
                argv[argc++] = arg_start;
                in_arg       = true;
            }
            *write_pos++ = c;
        }

        line++;
    }

    if (in_arg) {
        *write_pos = '\0';
    }

    /* Tolerate unclosed quotes */
    if (in_arg && in_quote) {
        *write_pos = '\0';
    }

    return argc;
}

static inline void _w(shell_write_str_fn w, const char *s)
{
    if (w && s) {
        w(s);
    }
}

static void _show_subcmds(const shell_command_t *cmd, shell_write_str_fn w)
{
    if (!cmd || !w || !cmd->subcmds) {
        return;
    }

    _w(w, "\r\nAvailable subcommands for '");
    _w(w, cmd->name);
    _w(w, "':\r\n");

    if (cmd->subcmd_count == 0) {
        _w(w, "  (none)\r\n\r\n");
        return;
    }

    size_t max_len = 0;
    for (size_t i = 0; i < cmd->subcmd_count; i++) {
        if (cmd->subcmds[i].name) {
            size_t len = strlen(cmd->subcmds[i].name);
            if (len > max_len) {
                max_len = len;
            }
        }
    }

    for (size_t i = 0; i < cmd->subcmd_count; i++) {
        const shell_command_t *sc = &cmd->subcmds[i];
        if (!sc || !sc->name) {
            continue;
        }

        _w(w, "  ");
        _w(w, sc->name);

        for (size_t pad = strlen(sc->name); pad < max_len; pad++) {
            _w(w, " ");
        }

        if (sc->help) {
            _w(w, "  - ");
            _w(w, sc->help);
        }
        _w(w, "\r\n");
    }
    _w(w, "\r\n");
}

static bool _exec_cmd(int argc, char **argv, const shell_command_t *const *cmds,
                      size_t cmd_count, shell_write_str_fn w)
{
    if (argc == 0 || !argv || !argv[0]) {
        return false;
    }

    for (size_t i = 0; i < cmd_count; i++) {
        const shell_command_t *cmd = cmds ? cmds[i] : NULL;
        if (!cmd || !cmd->name) {
            continue;
        }

        if (strcmp(argv[0], cmd->name) == 0) {
            /* Simple command: has handler, run directly */
            if (cmd->handler) {
                cmd->handler(argc, argv);
                return true;
            }

            /* Command group: has subcmds */
            if (cmd->subcmds && cmd->subcmd_count > 0) {
                if (argc < 2 || !argv[1] || argv[1][0] == '\0') {
                    _show_subcmds(cmd, w);
                    return true;
                }

                for (size_t j = 0; j < cmd->subcmd_count; j++) {
                    const shell_command_t *sc = &cmd->subcmds[j];
                    if (sc && sc->name && strcmp(argv[1], sc->name) == 0) {
                        if (sc->handler) {
                            sc->handler(argc - 1, argv + 1);
                        }
                        return true;
                    }
                }

                _w(w, "Unknown subcommand: ");
                _w(w, argv[1]);
                _w(w, "\r\n");
                _show_subcmds(cmd, w);
                return true;
            }

            return true;
        }
    }

    return false;
}

void shell_cmd_exec_line(const char *line, const shell_command_t *const *cmds,
                         size_t cmd_count, shell_write_str_fn write_str)
{
    if (!line || line[0] == '\0') {
        return;
    }

    char temp_buf[SHELL_INPUT_BUF_SIZE];
    strncpy(temp_buf, line, sizeof(temp_buf) - 1);
    temp_buf[sizeof(temp_buf) - 1] = '\0';

    char *argv[16];
    int argc = _parse_args(temp_buf, argv, 16);
    if (argc == 0) {
        return;
    }

    if (_exec_cmd(argc, argv, cmds, cmd_count, write_str)) {
        return;
    }

    _w(write_str, "Unknown command: ");
    _w(write_str, argv[0]);
    _w(write_str, "\r\n");
}

size_t shell_cmd_collect_matches(const char *prefix, size_t prefix_len,
                                 const shell_command_t *const *cmds,
                                 size_t cmd_count, const char **out,
                                 size_t out_cap)
{
    size_t n = 0;

    for (size_t i = 0; i < cmd_count && n < out_cap; i++) {
        const shell_command_t *cmd = cmds ? cmds[i] : NULL;
        const char *name           = (cmd && cmd->name) ? cmd->name : NULL;
        if (name && strncmp(prefix, name, prefix_len) == 0) {
            out[n++] = name;
        }
    }

    return n;
}

size_t shell_cmd_collect_sub_matches(const char *cmd_name, const char *prefix,
                                     size_t prefix_len,
                                     const shell_command_t *const *cmds,
                                     size_t cmd_count, const char **out,
                                     size_t out_cap)
{
    size_t n = 0;
    if (!cmd_name) {
        return 0;
    }

    for (size_t i = 0; i < cmd_count; i++) {
        const shell_command_t *cmd = cmds ? cmds[i] : NULL;
        if (!cmd || !cmd->name || strcmp(cmd_name, cmd->name) != 0) {
            continue;
        }
        if (!cmd->subcmds) {
            break;
        }
        for (size_t j = 0; j < cmd->subcmd_count && n < out_cap; j++) {
            const char *name = cmd->subcmds[j].name;
            if (name && strncmp(prefix, name, prefix_len) == 0) {
                out[n++] = name;
            }
        }
        break;
    }
    return n;
}

const char *shell_cmd_find_help(const char *name,
                                const shell_command_t *const *cmds,
                                size_t cmd_count)
{
    if (!name) {
        return NULL;
    }

    for (size_t i = 0; i < cmd_count; i++) {
        const shell_command_t *cmd = cmds ? cmds[i] : NULL;
        if (cmd && cmd->name && strcmp(name, cmd->name) == 0) {
            return cmd->help;
        }
    }

    return NULL;
}

const char *shell_cmd_find_sub_help(const char *cmd_name, const char *sub_name,
                                    const shell_command_t *const *cmds,
                                    size_t cmd_count)
{
    if (!cmd_name || !sub_name) {
        return NULL;
    }

    for (size_t i = 0; i < cmd_count; i++) {
        const shell_command_t *cmd = cmds ? cmds[i] : NULL;
        if (!cmd || !cmd->name || strcmp(cmd_name, cmd->name) != 0) {
            continue;
        }
        if (!cmd->subcmds) {
            break;
        }
        for (size_t j = 0; j < cmd->subcmd_count; j++) {
            const shell_command_t *sc = &cmd->subcmds[j];
            if (sc && sc->name && strcmp(sub_name, sc->name) == 0) {
                return sc->help;
            }
        }
        break;
    }
    return NULL;
}

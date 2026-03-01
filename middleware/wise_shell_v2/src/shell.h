/**
 * @file shell.h
 * @brief shell.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */

#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../shell_config.h"

/* =====================================================================
 * ANSI
 * ===================================================================== */
#define ANSI_CURSOR_UP "\033[A"
#define ANSI_CURSOR_DOWN "\033[B"
#define ANSI_CURSOR_RIGHT "\033[C"
#define ANSI_CURSOR_LEFT "\033[D"
#define ANSI_CLEAR_SCREEN "\033[2J"
#define ANSI_CLEAR_LINE "\033[K"
#define ANSI_CURSOR_COL1 "\033[G"
#define ANSI_RESET "\033[0m"
#define ANSI_REVERSE "\033[7m"

/* =====================================================================
 * ASCII CHAR 
 * ===================================================================== */
#define ASCII_SOH 0x01
#define ASCII_STX 0x02
#define ASCII_ETX 0x03
#define ASCII_EOT 0x04
#define ASCII_ENQ 0x05
#define ASCII_BEL 0x07
#define ASCII_BS 0x08
#define ASCII_HT 0x09
#define ASCII_LF 0x0A
#define ASCII_VT 0x0B
#define ASCII_FF 0x0C
#define ASCII_CR 0x0D
#define ASCII_NAK 0x15
#define ASCII_ESC 0x1B
#define ASCII_DEL 0x7F

#define CHAR_CTRL_A ASCII_SOH
#define CHAR_CTRL_B ASCII_STX
#define CHAR_CTRL_C ASCII_ETX
#define CHAR_CTRL_D ASCII_EOT
#define CHAR_CTRL_E ASCII_ENQ
#define CHAR_CTRL_K ASCII_VT
#define CHAR_CTRL_L ASCII_FF
#define CHAR_CTRL_U ASCII_NAK
#define CHAR_CTRL_G ASCII_BEL
#define CHAR_BACKSPACE ASCII_BS
#define CHAR_TAB ASCII_HT
#define CHAR_DEL ASCII_DEL
#define CHAR_ENTER_LF ASCII_LF
#define CHAR_ENTER_CR ASCII_CR
#define CHAR_ESC ASCII_ESC

/* =====================================================================
 * KEY DEFINE 
 * ===================================================================== */
#define KEY_CTRL_A CHAR_CTRL_A
#define KEY_CTRL_C CHAR_CTRL_C
#define KEY_CTRL_D CHAR_CTRL_D
#define KEY_CTRL_E CHAR_CTRL_E
#define KEY_CTRL_K CHAR_CTRL_K
#define KEY_CTRL_L CHAR_CTRL_L
#define KEY_CTRL_U CHAR_CTRL_U
#define KEY_CTRL_G CHAR_CTRL_G
#define KEY_BACKSPACE CHAR_BACKSPACE
#define KEY_TAB CHAR_TAB
#define KEY_DEL CHAR_DEL
#define KEY_ENTER_CR CHAR_ENTER_CR
#define KEY_ENTER_LF CHAR_ENTER_LF
#define KEY_ESC CHAR_ESC

/* =====================================================================
 * Command Handler
 * ===================================================================== */

typedef int (*shell_cmd_handler_fn)(int argc, char **argv);

/* =====================================================================
 * Unified Command Structure
 * ===================================================================== */
typedef struct shell_command {
    const char *name;
    shell_cmd_handler_fn handler;           /* Simple commands have handler */
    const struct shell_command *subcmds;    /* Command groups have subcmds */
    size_t subcmd_count;
    const char *help;
} shell_command_t;

/* =====================================================================
 * Register Macros (for defining subcommands)
 * ===================================================================== */

/* Simple command */
#define SHELL_CMD(_name, _handler, _help)                                      \
    { .name = #_name, .handler = (_handler), .help = (_help) }

/* Command group */
#define SHELL_CMD_GROUP(_name, _subcmds, _help)                                \
    { .name = #_name,                                                          \
      .subcmds = (_subcmds),                                                   \
      .subcmd_count = sizeof(_subcmds) / sizeof((_subcmds)[0]),                \
      .help = (_help) }

/* =====================================================================
 * Auto-Register Macros (using linker section)
 * ===================================================================== */

/* Helper macro: linker symbol name */
#define _SHELL_CONCAT(_a, _b) _a##_b
#define SHELL_CONCAT(_a, _b)  _SHELL_CONCAT(_a, _b)

/* Auto-register simple command */
#define SHELL_CMD_AUTO(_name, _handler, _help)                                 \
    static const shell_command_t SHELL_CONCAT(_shell_cmd_, _name) = {          \
        .name = #_name, .handler = (_handler), .help = (_help)};               \
    static const shell_command_t *SHELL_CONCAT(_shell_cmd_ptr_, _name)         \
        __attribute__((section(".shell_cmds"), used)) =                        \
            &SHELL_CONCAT(_shell_cmd_, _name)

/* Auto-register command group */
#define SHELL_CMD_GROUP_AUTO(_name, _subcmds, _help)                           \
    static const shell_command_t SHELL_CONCAT(_shell_cmd_, _name) = {          \
        .name         = #_name,                                                \
        .subcmds      = (_subcmds),                                            \
        .subcmd_count = sizeof(_subcmds) / sizeof((_subcmds)[0]),              \
        .help         = (_help)};                                              \
    static const shell_command_t *SHELL_CONCAT(_shell_cmd_ptr_, _name)         \
        __attribute__((section(".shell_cmds"), used)) =                        \
            &SHELL_CONCAT(_shell_cmd_, _name)

/* =====================================================================
 * Backend Callbacks
 * ===================================================================== */
typedef bool (*shell_read_char_fn)(char *c);
typedef void (*shell_write_str_fn)(const char *s);

/* =====================================================================
 * Shell Config
 * ===================================================================== */
typedef struct {
    shell_read_char_fn read_char;
    shell_write_str_fn write_str;
    const char *prompt;
} shell_config_t;

/* =====================================================================
 * Shell API
 * ===================================================================== */
void shell_init(const shell_config_t *cfg);
void shell_task(void);
void shell_process_char(char c);

/* =====================================================================
 * Command Break API - for long-running commands
 * ===================================================================== */

/**
 * @brief Check if user requested to break current command (Ctrl+C)
 *
 * Use this in long-running command loops:
 * @code
 * while (running) {
 *     do_work();
 *     if (shell_is_break_requested()) {
 *         PRINTF("Aborted by user\r\n");
 *         break;
 *     }
 * }
 * @endcode
 *
 * @return true if Ctrl+C was pressed during command execution
 */
bool shell_is_break_requested(void);

/**
 * @brief Poll for break request (call this in tight loops)
 *
 * This function reads pending input and checks for Ctrl+C.
 * Use when your command doesn't yield to shell_task().
 *
 * @return true if Ctrl+C was pressed
 */
bool shell_poll_break(void);

/* =====================================================================
 * Shell Helper API
 * ===================================================================== */
const shell_command_t *const *shell_get_commands(size_t *count);

#endif /* SHELL_H */

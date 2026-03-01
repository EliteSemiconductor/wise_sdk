/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if MIDDLEWARE_WISE_SHELL
#include "wise_core.h"
#include "api/wise_uart_api.h"
#include <string.h>

#include "cmdhist.h"
#include "shell.h"
#include "shell_commands.h"
#include "wise.h"

char *argv[32];
char buf[SHELL_DEFAULT_BUFSIZE];
int8_t shellTaskId = WISE_INVALID_INDEX;

static int _shell_line_collector(int n, char *buf, char inputC);
static bool _parse_char_input(int *pkey, char inputC);

// Function to tokenize input line into arguments
static int _parse_argv(char *line, int max_tokens, char **argv, const char *delim)
{
    int i;
    char *token;
    char *saveptr = NULL;

    token = strtok_r(line, delim, &saveptr);

    for (i = 0; token != NULL && i < max_tokens; i++) {
        argv[i] = token;
        token   = strtok_r(NULL, delim, &saveptr);
    }

    argv[i] = NULL; // Ensure argv ends with NULL for exec functions
    return i;       // Return number of tokens parsed
}

static bool _parse_char_input(int *pkey, char inputC)
{
    static uint8_t s_codeBufIdx = 0;
    static char s_codeBuf[5];

    // Predefined key combinations
    const struct keytbl {
        int key;
        char code[5];
    } c_combKeyTbl[] = {
        {KEY_UP, {0x1b, 0x5b, 0x41, 0}},
        {KEY_DOWN, {0x1b, 0x5b, 0x42, 0}},
    };

    {
        // Reset buffer index if it exceeds the size limit
        if (s_codeBufIdx >= sizeof(s_codeBuf) - 1) {
            s_codeBufIdx = 0;
        }

        // Handle single key input when buffer index is 0 and not an escape
        // character
        if (s_codeBufIdx == 0 && inputC != 0x1b) {
            *pkey = inputC;
            return true; // Return true indicating a valid single key press
        }

        // Append received byte to the buffer and check against predefined key
        // combinations
        s_codeBuf[s_codeBufIdx++] = (char)inputC;
        s_codeBuf[s_codeBufIdx]   = '\0'; // Ensure buffer is null-terminated

        // Check if the received code matches any predefined key combination
        for (uint8_t i = 0; i < sizeof(c_combKeyTbl) / sizeof(c_combKeyTbl[0]); i++) {
            if (strcmp(c_combKeyTbl[i].code, s_codeBuf) == 0) {
                s_codeBufIdx = 0; // Reset buffer index on match
                *pkey        = c_combKeyTbl[i].key;
                return true; // Return true indicating a valid key combination
            }
        }
    }

    return false; // No match found, return false
}

// Function to print shell prompt
void shell_print_prompt(void)
{
    printf("%s", BACH_CONSOLE_PROMPT);
}

// Function to print newline character
static void _terminal_newline(void)
{
    putchar(CARRIAGE_RETURN);
    putchar(NEW_LINE);
}

// Function to perform backspace operation 'n' times
static void _terminal_backspace(int n)
{
    for (int i = 0; i < n; i++) {
        putchar(BACKSPACE);
        putchar(SPACE);
        putchar(BACKSPACE);
    }
}

// Function to handle special key commands
static int _special_key_cmd(int n, char *buf, int bs, const char *skey)
{
    if (n > (int)strlen(skey)) {
        strcpy(buf, skey);
        _terminal_backspace(bs);
        return 1; // Return 1 indicating special key command handled
    }

    return 0; // Return 0 indicating no special key command handled
}

// Function to initialize shell
void shell_init(void)
{
    shlcmd_init();

    shell_print_prompt(); // Print initial shell prompt
    cmdhist_init();       // Initialize command history
}

//=====================

// Function to execute shell process
static int _shell_line_collector(int n, char *buf, char inputC)
{
    int c   = 0;
    int res = 0;
    static char s_xbuf[SHELL_DEFAULT_BUFSIZE];
    static int s_xidx = 0;

    if (_parse_char_input(&c, inputC)) {
        if (c == KEY_BACKSPACE || c == KEY_DEL) {
            if (s_xidx < (int)sizeof(s_xbuf)) {
                if (s_xidx > 0) {
                    s_xidx--;
                    _terminal_backspace(1);
                }
            }
        } else if (c == CARRIAGE_RETURN || c == NEW_LINE) {
            if (s_xidx < (int)sizeof(s_xbuf)) {
                s_xbuf[s_xidx++] = 0;
                strncpy(buf, s_xbuf, n);
                _terminal_newline();
                s_xidx = 0;
                res    = 1;
                goto finish;
            } else {
                _terminal_newline();
                strncpy(buf, s_xbuf, n);
                _terminal_newline();
                s_xidx = 0;
                res    = 1;
            }
        } else if (c == KEY_CTRL_C) {
            if (_special_key_cmd(n, buf, s_xidx, "<C-c>")) {
                // return special key to shell for special treatment
                s_xidx = 0;
                res    = 1;
                goto finish;
            }
        } else if (c == KEY_CTRL_G) {
            if (_special_key_cmd(n, buf, s_xidx, "<C-g>")) {
                // return special key to shell for special treatment
                s_xidx = 0;
                res    = 1;
                goto finish;
            }
        } else if (c == KEY_UP) {
            int n = s_xidx;

            if (cmdhist_prev(sizeof(s_xbuf), s_xbuf)) {
                s_xidx = strlen(s_xbuf); // Update s_xidx to the length of the
                                         // retrieved command
                _terminal_backspace(n);
                printf("%s", s_xbuf);
                goto finish;
            }
        } else if (c == KEY_DOWN) {
            int n = s_xidx;

            if (cmdhist_next(sizeof(s_xbuf), s_xbuf)) {
                s_xidx = strlen(s_xbuf);
                _terminal_backspace(n);
                printf("%s", s_xbuf);
                goto finish;
            }
        } else if (c >= KEY_SPACE && c <= KEY_TILDE) {
            putchar(c);

            if (s_xidx < (int)sizeof(s_xbuf)) {
                s_xbuf[s_xidx++] = c;
            }
        }
    }

finish:
    fflush(stdout);
    return res;
}

void eshell_char_input(uint8_t inputC)
{
    int argc;

    if (_shell_line_collector(SHELL_DEFAULT_BUFSIZE, buf, inputC)) {
        int n = strlen(buf);

        if (n > SHELL_DEFAULT_BUFSIZE) {
            return; // Return if input line exceeds maximum length
        }

        char buf2[n + 1];
        strcpy(buf2, buf);
        argc = _parse_argv(buf2, sizeof(argv) / sizeof(char *), argv,
                           " "); // Parse input arguments

        if (argc) {
            int res = shlcmd_parse(argc, argv); // Parse and execute shell command

            if (!!(res & SHLCMD_UNKNOWN)) {
                printf("unknown command\n"); // Print message for unknown command
            }

            if (!!(res & SHLCMD_HIST)) {
                cmdhist_add(buf); // Add command to history if history flag is set
            }
        }

        shell_print_prompt(); // Print shell prompt after command execution
        fflush(stdout);       // Flush stdout to ensure prompt is printed immediately
    }
}

// Function to initialize eshell
void eshell_init(void)
{
    shell_init(); // Initialize shell
}

#endif /* MIDDLEWARE_WISE_SHELL */

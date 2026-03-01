/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __SHELL_H_
#define __SHELL_H_

#include <stdbool.h>
#include <stdint.h>

#if 0
#include "func_fs.h"
#include "func_rf.h"
#endif

#define CARRIAGE_RETURN '\r'
#define NEW_LINE '\n'
#define SPACE ' '
#define BACKSPACE '\b'

#define KEY_UP 0x1B5B41
#define KEY_DOWN 0x1B5B42
#define KEY_CTRL_C 0x03
#define KEY_CTRL_D 0x04
#define KEY_CTRL_G 0x07
#define KEY_BACKSPACE 0x08
#define KEY_DEL 0x7F
#define KEY_SPACE 0x20
#define KEY_TILDE 0x7E

#define SHELL_DEFAULT_BUFSIZE 128
#define BACH_CONSOLE_PROMPT "ESMT> "
#define INVALID_PARAM_MESSAGE "Invalid parameters\nUsage:\n"

void shell_init(void);
void shell_task(void);
void shell_print_prompt();
void eshell_init(void);
void eshell_proc(void);
void eshell_char_input(uint8_t inputC);

#endif // __SHELL_H_

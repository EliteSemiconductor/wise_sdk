#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char *buf;
    uint16_t cap;
    uint16_t len;
    uint16_t cursor;
} shell_line_t;

void shell_input_init(shell_line_t *l, char *buf, uint16_t cap);
void shell_input_clear(shell_line_t *l);
bool shell_input_insert_char(shell_line_t *l, char c);
bool shell_input_backspace(shell_line_t *l);
bool shell_input_delete(shell_line_t *l);
bool shell_input_move_left(shell_line_t *l);
bool shell_input_move_right(shell_line_t *l);
void shell_input_move_home(shell_line_t *l);
void shell_input_move_end(shell_line_t *l);
bool shell_input_kill_to_start(shell_line_t *l);
bool shell_input_kill_to_end(shell_line_t *l);

#endif /* SHELL_INPUT_H */

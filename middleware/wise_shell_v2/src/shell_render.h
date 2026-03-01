#ifndef SHELL_RENDER_H
#define SHELL_RENDER_H

#include <stdint.h>
#include "shell.h"
#include "shell_input.h"

typedef struct {
    shell_write_str_fn write_str;
} shell_render_t;

void shell_render_init(shell_render_t *r, shell_write_str_fn write_str);
void shell_render_write(shell_render_t *r, const char *s);
void shell_render_cursor_left(shell_render_t *r, uint16_t n);
void shell_render_cursor_right(shell_render_t *r, uint16_t n);
void shell_render_cursor_up(shell_render_t *r, uint16_t n);
void shell_render_cursor_down(shell_render_t *r, uint16_t n);
void shell_render_clear_screen(shell_render_t *r);
void shell_render_clear_line(shell_render_t *r);
void shell_render_redraw_line(shell_render_t *r, const char *prompt,
                              const shell_line_t *line,
                              uint16_t *last_line_len);

#endif /* SHELL_RENDER_H */

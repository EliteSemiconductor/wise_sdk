#include "shell_render.h"
#include <string.h>

void shell_render_init(shell_render_t *r, shell_write_str_fn write_str)
{
    if (!r) {
        return;
    }
    r->write_str = write_str;
}

void shell_render_write(shell_render_t *r, const char *s)
{
    if (r && r->write_str && s) {
        r->write_str(s);
    }
}

static void _cursor_repeat(shell_render_t *r, const char *seq, uint16_t n)
{
    while (n--) {
        shell_render_write(r, seq);
    }
}

void shell_render_cursor_left(shell_render_t *r, uint16_t n)
{
    while (n--) {
        shell_render_write(r, "\b");
    }
}

void shell_render_cursor_right(shell_render_t *r, uint16_t n)
{
    _cursor_repeat(r, ANSI_CURSOR_RIGHT, n);
}

void shell_render_cursor_up(shell_render_t *r, uint16_t n)
{
    _cursor_repeat(r, ANSI_CURSOR_UP, n);
}

void shell_render_cursor_down(shell_render_t *r, uint16_t n)
{
    _cursor_repeat(r, ANSI_CURSOR_DOWN, n);
}

void shell_render_clear_screen(shell_render_t *r)
{
    shell_render_write(r, ANSI_CLEAR_SCREEN);
}

void shell_render_clear_line(shell_render_t *r)
{
    shell_render_write(r, ANSI_CURSOR_COL1 ANSI_CLEAR_LINE);
}

void shell_render_redraw_line(shell_render_t *r, const char *prompt,
                              const shell_line_t *line, uint16_t *last_line_len)
{
    if (!r || !line || !line->buf || !last_line_len) {
        return;
    }

    if (line->len < line->cap) {
        line->buf[line->len] = '\0';
    }

    shell_render_write(r, ANSI_CURSOR_COL1 ANSI_CLEAR_LINE);

    uint16_t new_len = 0;
    if (prompt) {
        shell_render_write(r, prompt);
        new_len = (uint16_t)strlen(prompt);
    }

    if (line->len > 0) {
        shell_render_write(r, line->buf);
        new_len = (uint16_t)(new_len + line->len);
    }

    *last_line_len = new_len;

    if (line->cursor < line->len) {
        shell_render_cursor_left(r, (uint16_t)(line->len - line->cursor));
    }
}

#include "shell_input.h"
#include <string.h>

static inline void _ensure_null(shell_line_t *l)
{
    if (!l || !l->buf || l->cap == 0) {
        return;
    }
    if (l->len >= l->cap) {
        l->len = (uint16_t)(l->cap - 1);
    }
    l->buf[l->len] = '\0';
}

void shell_input_init(shell_line_t *l, char *buf, uint16_t cap)
{
    if (!l) {
        return;
    }
    l->buf    = buf;
    l->cap    = cap;
    l->len    = 0;
    l->cursor = 0;
    if (buf && cap) {
        buf[0] = '\0';
    }
}

void shell_input_clear(shell_line_t *l)
{
    if (!l || !l->buf || l->cap == 0) {
        return;
    }
    l->len    = 0;
    l->cursor = 0;
    l->buf[0] = '\0';
}

bool shell_input_insert_char(shell_line_t *l, char c)
{
    if (!l || !l->buf || l->cap == 0) {
        return false;
    }
    if (l->len >= (uint16_t)(l->cap - 1)) {
        return false;
    }

    if (l->cursor < l->len) {
        memmove(l->buf + l->cursor + 1, l->buf + l->cursor,
                (size_t)(l->len - l->cursor));
    }
    l->buf[l->cursor] = c;
    l->cursor++;
    l->len++;
    _ensure_null(l);
    return true;
}

bool shell_input_backspace(shell_line_t *l)
{
    if (!l || !l->buf || l->cap == 0) {
        return false;
    }
    if (l->cursor == 0 || l->len == 0) {
        return false;
    }

    memmove(l->buf + l->cursor - 1, l->buf + l->cursor,
            (size_t)(l->len - l->cursor));
    l->cursor--;
    l->len--;
    _ensure_null(l);
    return true;
}

bool shell_input_delete(shell_line_t *l)
{
    if (!l || !l->buf || l->cap == 0) {
        return false;
    }
    if (l->cursor >= l->len) {
        return false;
    }

    memmove(l->buf + l->cursor, l->buf + l->cursor + 1,
            (size_t)(l->len - l->cursor - 1));
    l->len--;
    _ensure_null(l);
    return true;
}

bool shell_input_move_left(shell_line_t *l)
{
    if (!l) {
        return false;
    }
    if (l->cursor == 0) {
        return false;
    }
    l->cursor--;
    return true;
}

bool shell_input_move_right(shell_line_t *l)
{
    if (!l) {
        return false;
    }
    if (l->cursor >= l->len) {
        return false;
    }
    l->cursor++;
    return true;
}

void shell_input_move_home(shell_line_t *l)
{
    if (!l) {
        return;
    }
    l->cursor = 0;
}

void shell_input_move_end(shell_line_t *l)
{
    if (!l) {
        return;
    }
    l->cursor = l->len;
}

bool shell_input_kill_to_start(shell_line_t *l)
{
    if (!l || !l->buf) {
        return false;
    }
    if (l->cursor == 0) {
        return false;
    }

    memmove(l->buf, l->buf + l->cursor, (size_t)(l->len - l->cursor));
    l->len    = (uint16_t)(l->len - l->cursor);
    l->cursor = 0;
    _ensure_null(l);
    return true;
}

bool shell_input_kill_to_end(shell_line_t *l)
{
    if (!l || !l->buf) {
        return false;
    }
    if (l->cursor >= l->len) {
        return false;
    }

    l->len = l->cursor;
    _ensure_null(l);
    return true;
}

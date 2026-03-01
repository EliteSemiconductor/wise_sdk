#include "shell_complete.h"
#include <string.h>
#include "shell_cmd.h"

void shell_complete_init(shell_complete_t *c)
{
    if (!c) {
        return;
    }
#if SHELL_FEATURE_COMPLETION
    c->match_count   = 0;
    c->menu_mode     = false;
    c->menu_count    = 0;
    c->menu_selected = 0;
    memset(c->menu_prefix, 0, sizeof(c->menu_prefix));
#endif
}

#if SHELL_FEATURE_COMPLETION
bool shell_complete_is_menu_mode(const shell_complete_t *c)
{
    return c && c->menu_mode;
}

static void _clear_menu_lines(shell_complete_t *c, shell_render_t *r)
{
    if (!c || !r) {
        return;
    }
    for (uint16_t i = 0; i < c->menu_count; i++) {
        shell_render_cursor_down(r, 1);
        shell_render_write(r, "\r" ANSI_CLEAR_LINE);
    }
    for (uint16_t i = 0; i < c->menu_count; i++) {
        shell_render_cursor_up(r, 1);
    }
    shell_render_write(r, "\r");
}

static void _exit_menu_mode(shell_complete_t *c)
{
    if (!c) {
        return;
    }
    c->menu_mode     = false;
    c->menu_count    = 0;
    c->menu_selected = 0;
    c->match_count   = 0;
}

static void _redraw_menu(shell_complete_t *c, shell_render_t *r,
                         const char *prompt,
                         const shell_command_t *const *cmds, size_t cmd_count,
                         shell_line_t *line, uint16_t *last_line_len)
{
    (void)prompt;
    if (!c || !r) {
        return;
    }

    shell_render_write(r, "\r\n");
    for (uint16_t i = 0; i < c->menu_count; i++) {
        if (i == c->menu_selected) {
            shell_render_write(r, ANSI_REVERSE);
        }
        shell_render_write(r, "  ");
        shell_render_write(r, c->matches[i]);

        const char *help = shell_cmd_find_help(c->matches[i], cmds, cmd_count);
        if (help) {
            shell_render_write(r, " - ");
            shell_render_write(r, help);
        }

        if (i == c->menu_selected) {
            shell_render_write(r, ANSI_RESET);
        }
        if (i < c->menu_count - 1) {
            shell_render_write(r, "\r\n");
        }
    }

    for (uint16_t i = 0; i < c->menu_count; i++) {
        shell_render_cursor_up(r, 1);
    }
    shell_render_write(r, "\r");
    shell_render_redraw_line(r, prompt, line, last_line_len);
}

static void _redraw_menu_sub(shell_complete_t *c, shell_render_t *r,
                             const char *prompt, const char *cmd_name,
                             const shell_command_t *const *cmds,
                             size_t cmd_count, shell_line_t *line,
                             uint16_t *last_line_len)
{
    shell_render_write(r, "\r\n");
    for (uint16_t i = 0; i < c->menu_count; i++) {
        if (i == c->menu_selected) {
            shell_render_write(r, ANSI_REVERSE);
        }
        shell_render_write(r, "  ");
        shell_render_write(r, c->matches[i]);

        const char *help =
            shell_cmd_find_sub_help(cmd_name, c->matches[i], cmds, cmd_count);
        if (help) {
            shell_render_write(r, " - ");
            shell_render_write(r, help);
        }

        if (i == c->menu_selected) {
            shell_render_write(r, ANSI_RESET);
        }
        if (i < c->menu_count - 1) {
            shell_render_write(r, "\r\n");
        }
    }

    for (uint16_t i = 0; i < c->menu_count; i++) {
        shell_render_cursor_up(r, 1);
    }
    shell_render_write(r, "\r");
    shell_render_redraw_line(r, prompt, line, last_line_len);
}

void shell_complete_menu_up(shell_complete_t *c, shell_render_t *render,
                            const char *prompt,
                            const shell_command_t *const *cmds,
                            size_t cmd_count, shell_line_t *line,
                            uint16_t *last_line_len)
{
    if (!c || !c->menu_mode || c->menu_selected == 0) {
        return;
    }
    c->menu_selected--;
    _clear_menu_lines(c, render);

    const char *space = strchr(c->menu_prefix, ' ');
    if (space) {
        char cmd_name[SHELL_MAX_CMD_NAME];
        size_t cmd_len = (size_t)(space - c->menu_prefix);
        if (cmd_len >= sizeof(cmd_name)) {
            cmd_len = sizeof(cmd_name) - 1;
        }
        memcpy(cmd_name, c->menu_prefix, cmd_len);
        cmd_name[cmd_len] = '\0';
        _redraw_menu_sub(c, render, prompt, cmd_name, cmds, cmd_count, line,
                         last_line_len);
    } else {
        _redraw_menu(c, render, prompt, cmds, cmd_count, line, last_line_len);
    }
}

void shell_complete_menu_down(shell_complete_t *c, shell_render_t *render,
                              const char *prompt,
                              const shell_command_t *const *cmds,
                              size_t cmd_count, shell_line_t *line,
                              uint16_t *last_line_len)
{
    if (!c || !c->menu_mode) {
        return;
    }
    if (c->menu_selected < c->menu_count - 1) {
        c->menu_selected++;
    } else {
        c->menu_selected = 0;
    }
    _clear_menu_lines(c, render);

    const char *space = strchr(c->menu_prefix, ' ');
    if (space) {
        char cmd_name[SHELL_MAX_CMD_NAME];
        size_t cmd_len = (size_t)(space - c->menu_prefix);
        if (cmd_len >= sizeof(cmd_name)) {
            cmd_len = sizeof(cmd_name) - 1;
        }
        memcpy(cmd_name, c->menu_prefix, cmd_len);
        cmd_name[cmd_len] = '\0';
        _redraw_menu_sub(c, render, prompt, cmd_name, cmds, cmd_count, line,
                         last_line_len);
    } else {
        _redraw_menu(c, render, prompt, cmds, cmd_count, line, last_line_len);
    }
}

void shell_complete_menu_select(shell_complete_t *c, shell_render_t *render,
                                const char *prompt, shell_line_t *line,
                                uint16_t *last_line_len)
{
    if (!c || !c->menu_mode || c->menu_selected >= c->menu_count) {
        return;
    }

    _clear_menu_lines(c, render);

    const char *selected = c->matches[c->menu_selected];
    const char *space    = strchr(c->menu_prefix, ' ');

    if (space) {
        size_t cmd_len = (size_t)(space - c->menu_prefix);
        line->len      = 0;
        line->cursor   = 0;

        if (cmd_len < (size_t)(line->cap - 1)) {
            memcpy(line->buf, c->menu_prefix, cmd_len);
            line->len = (uint16_t)cmd_len;
        }
        if (line->len < (uint16_t)(line->cap - 1)) {
            line->buf[line->len++] = ' ';
        }
        while (*selected && line->len < (uint16_t)(line->cap - 2)) {
            line->buf[line->len++] = *selected++;
        }
        if (line->len < (uint16_t)(line->cap - 1)) {
            line->buf[line->len++] = ' ';
        }
        line->buf[line->len] = '\0';
        line->cursor         = line->len;
    } else {
        strncpy(line->buf, selected, (size_t)(line->cap - 2));
        line->buf[line->cap - 2] = '\0';
        line->len                = (uint16_t)strlen(line->buf);
        if (line->len < (uint16_t)(line->cap - 1)) {
            line->buf[line->len++] = ' ';
            line->buf[line->len]   = '\0';
        }
        line->cursor = line->len;
    }

    _exit_menu_mode(c);
    shell_render_redraw_line(render, prompt, line, last_line_len);
}

void shell_complete_menu_cancel(shell_complete_t *c, shell_render_t *render,
                                const char *prompt, shell_line_t *line,
                                uint16_t *last_line_len)
{
    if (!c || !c->menu_mode) {
        return;
    }
    _clear_menu_lines(c, render);

    strncpy(line->buf, c->menu_prefix, (size_t)(line->cap - 1));
    line->buf[line->cap - 1] = '\0';
    line->len                = (uint16_t)strlen(line->buf);
    line->cursor             = line->len;

    _exit_menu_mode(c);
    shell_render_redraw_line(render, prompt, line, last_line_len);
}

void shell_complete_exit_menu_on_typing(shell_complete_t *c,
                                        shell_render_t *render)
{
    if (!c || !c->menu_mode) {
        return;
    }
    _clear_menu_lines(c, render);
    _exit_menu_mode(c);
}

void shell_complete_handle_tab(shell_complete_t *c, shell_line_t *line,
                               const shell_command_t *const *cmds,
                               size_t cmd_count, shell_render_t *render,
                               const char *prompt, uint16_t *last_line_len)
{
    if (!c || !line || !render) {
        return;
    }
    if (line->len == 0) {
        return;
    }

    line->buf[line->len] = '\0';
    const char *space    = strchr(line->buf, ' ');

    if (!space) {
        c->match_count = (uint16_t)shell_cmd_collect_matches(
            line->buf, line->len, cmds, cmd_count, c->matches,
            SHELL_MAX_COMPLETION_MATCHES);

        if (c->match_count == 0) {
            return;
        } else if (c->match_count == 1) {
            strncpy(line->buf, c->matches[0], (size_t)(line->cap - 2));
            line->buf[line->cap - 2] = '\0';
            line->len                = (uint16_t)strlen(line->buf);
            if (line->len < (uint16_t)(line->cap - 1)) {
                line->buf[line->len++] = ' ';
                line->buf[line->len]   = '\0';
            }
            line->cursor = line->len;
            shell_render_redraw_line(render, prompt, line, last_line_len);
            return;
        }

        /* Enter menu mode for multiple matches */
        c->menu_mode     = true;
        c->menu_count    = c->match_count;
        c->menu_selected = 0;
        strncpy(c->menu_prefix, line->buf, sizeof(c->menu_prefix) - 1);
        c->menu_prefix[sizeof(c->menu_prefix) - 1] = '\0';

        _redraw_menu(c, render, prompt, cmds, cmd_count, line, last_line_len);
        return;
    }

    size_t cmd_len = (size_t)(space - line->buf);
    char cmd_name[SHELL_MAX_CMD_NAME];
    if (cmd_len >= sizeof(cmd_name)) {
        return;
    }
    memcpy(cmd_name, line->buf, cmd_len);
    cmd_name[cmd_len] = '\0';

    const char *sub_start = space + 1;
    while (*sub_start == ' ' || *sub_start == '\t') {
        sub_start++;
    }
    size_t sub_len = strlen(sub_start);

    c->match_count = (uint16_t)shell_cmd_collect_sub_matches(
        cmd_name, sub_start, sub_len, cmds, cmd_count, c->matches,
        SHELL_MAX_COMPLETION_MATCHES);

    if (c->match_count == 0) {
        return;
    } else if (c->match_count == 1) {
        line->len     = (uint16_t)(sub_start - line->buf);
        const char *p = c->matches[0];
        while (p && *p && line->len < (uint16_t)(line->cap - 2)) {
            line->buf[line->len++] = *p++;
        }
        if (line->len < (uint16_t)(line->cap - 1)) {
            line->buf[line->len++] = ' ';
        }
        line->buf[line->len] = '\0';
        line->cursor         = line->len;
        shell_render_redraw_line(render, prompt, line, last_line_len);
        return;
    }

    /* Enter menu mode for multiple matches */
    c->menu_mode     = true;
    c->menu_count    = c->match_count;
    c->menu_selected = 0;
    strncpy(c->menu_prefix, line->buf, sizeof(c->menu_prefix) - 1);
    c->menu_prefix[sizeof(c->menu_prefix) - 1] = '\0';

    _redraw_menu_sub(c, render, prompt, cmd_name, cmds, cmd_count, line,
                     last_line_len);
}
#endif /* SHELL_FEATURE_COMPLETION */

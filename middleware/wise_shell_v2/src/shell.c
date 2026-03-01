#include "shell.h"
#include "cmdhist.h"

#include <string.h>
#include <ctype.h>

#include "shell_input.h"
#include "shell_render.h"
#include "shell_cmd.h"
#include "shell_complete.h"

static char s_input_buf[SHELL_INPUT_BUF_SIZE];

/* =====================================================================
 * Linker Section (defined in linker script)
 * ===================================================================== */

extern const shell_command_t *__shell_cmds_start[];
extern const shell_command_t *__shell_cmds_end[];

/* =====================================================================
 * Global state
 * ===================================================================== */

static shell_read_char_fn s_read_char = NULL;
static const char *s_prompt           = NULL;

/* Command access helpers */
#define SHELL_CMD_COUNT() ((size_t)(__shell_cmds_end - __shell_cmds_start))
#define SHELL_CMDS()      ((const shell_command_t *const *)__shell_cmds_start)

static shell_line_t s_line;
static shell_render_t s_render;
static uint16_t s_last_line_len  = 0;
static uint8_t s_esc_state       = 0;
static char s_last_enter         = 0;
static bool s_break_requested    = false;
static bool s_in_command         = false;
static shell_complete_t s_complete;

static inline void _sh_write(const char *s)
{
    shell_render_write(&s_render, s);
}

static void _sh_redraw(void)
{
    shell_render_redraw_line(&s_render, s_prompt, &s_line, &s_last_line_len);
}

static void _sh_clear_line_state(void)
{
    shell_input_clear(&s_line);
#if SHELL_FEATURE_HISTORY
    cmdhist_reset_browse();
#endif
    _sh_redraw();
}

#if SHELL_FEATURE_HISTORY
static void _sh_hist_up(void)
{
    const char *hist = cmdhist_prev();
    if (!hist) {
        return;
    }
    strncpy(s_line.buf, hist, (size_t)(s_line.cap - 1));
    s_line.buf[s_line.cap - 1] = '\0';
    s_line.len                 = (uint16_t)strlen(s_line.buf);
    s_line.cursor              = s_line.len;
    _sh_redraw();
}

static void _sh_hist_down(void)
{
    const char *hist = cmdhist_next();
    if (hist) {
        strncpy(s_line.buf, hist, (size_t)(s_line.cap - 1));
        s_line.buf[s_line.cap - 1] = '\0';
        s_line.len                 = (uint16_t)strlen(s_line.buf);
    } else {
        s_line.buf[0] = '\0';
        s_line.len    = 0;
    }
    s_line.cursor = s_line.len;
    _sh_redraw();
}
#endif

/* =====================================================================
 * Public API
 * ===================================================================== */

void shell_init(const shell_config_t *cfg)
{
    if (!cfg || !cfg->read_char) {
        return;
    }

    s_read_char = cfg->read_char;

    shell_render_init(&s_render, cfg->write_str);

    s_prompt = cfg->prompt;

    memset(s_input_buf, 0, sizeof(s_input_buf));
    shell_input_init(&s_line, s_input_buf, (uint16_t)sizeof(s_input_buf));

    s_last_line_len = 0;
    s_esc_state     = 0;
    s_last_enter    = 0;

    shell_complete_init(&s_complete);

#if SHELL_FEATURE_HISTORY
    cmdhist_init();
#endif

    if (s_prompt) {
        _sh_write(ANSI_CURSOR_COL1);
        _sh_write(s_prompt);
        s_last_line_len = (uint16_t)strlen(s_prompt);
    }
}

void shell_process_char(char c)
{
    /* Handle \r\n or \n\r pair as single Enter */
    if (c == KEY_ENTER_CR || c == KEY_ENTER_LF) {
        if (s_last_enter != 0 && s_last_enter != c) {
            s_last_enter = 0;
            return; /* skip second byte of CR+LF / LF+CR */
        }
        s_last_enter = c;
    } else {
        s_last_enter = 0;
    }

    /* ESC sequence */
    if (s_esc_state == 0 && (unsigned char)c == KEY_ESC) {
        s_esc_state = 1;
        return;
    }
    if (s_esc_state == 1) {
        if (c == '[') {
            s_esc_state = 2;
            return;
        }
        if ((unsigned char)c == KEY_ESC) {
            return;
        }
        s_esc_state = 0;
    } else if (s_esc_state == 2) {
        s_esc_state = 0;

        switch (c) {
        case 'A': /* Up */
#if SHELL_FEATURE_COMPLETION
            if (shell_complete_is_menu_mode(&s_complete)) {
                shell_complete_menu_up(&s_complete, &s_render, s_prompt,
                                       SHELL_CMDS(), SHELL_CMD_COUNT(), &s_line,
                                       &s_last_line_len);
            } else
#endif
#if SHELL_FEATURE_HISTORY
            {
                _sh_hist_up();
            }
#endif
            return;

        case 'B': /* Down */
#if SHELL_FEATURE_COMPLETION
            if (shell_complete_is_menu_mode(&s_complete)) {
                shell_complete_menu_down(&s_complete, &s_render, s_prompt,
                                         SHELL_CMDS(), SHELL_CMD_COUNT(), &s_line,
                                         &s_last_line_len);
            } else
#endif
#if SHELL_FEATURE_HISTORY
            {
                _sh_hist_down();
            }
#endif
            return;

        case 'C': /* Right */
#if SHELL_FEATURE_COMPLETION
            if (!shell_complete_is_menu_mode(&s_complete))
#endif
            {
                if (shell_input_move_right(&s_line)) {
                    shell_render_cursor_right(&s_render, 1);
                }
            }
            return;

        case 'D': /* Left */
#if SHELL_FEATURE_COMPLETION
            if (!shell_complete_is_menu_mode(&s_complete))
#endif
            {
                if (shell_input_move_left(&s_line)) {
                    shell_render_cursor_left(&s_render, 1);
                }
            }
            return;
        }
        return;
    }

    /* Enter */
    if (c == KEY_ENTER_CR || c == KEY_ENTER_LF) {
#if SHELL_FEATURE_COMPLETION
        if (shell_complete_is_menu_mode(&s_complete)) {
            shell_complete_menu_select(&s_complete, &s_render, s_prompt,
                                       &s_line, &s_last_line_len);
            return;
        }
#endif

        _sh_write("\r\n");
        s_line.buf[s_line.len] = '\0';

        if (s_line.len > 0) {
            s_break_requested = false;
            s_in_command      = true;

            shell_cmd_exec_line(s_line.buf, SHELL_CMDS(), SHELL_CMD_COUNT(),
                                s_render.write_str);

            s_in_command      = false;
            s_break_requested = false;

#if SHELL_FEATURE_HISTORY
            cmdhist_add(s_line.buf);
#endif
        }

        shell_input_clear(&s_line);
#if SHELL_FEATURE_HISTORY
        cmdhist_reset_browse();
#endif

        if (s_prompt) {
            _sh_write(ANSI_CURSOR_COL1);
            _sh_write(s_prompt);
            s_last_line_len = (uint16_t)strlen(s_prompt);
        } else {
            s_last_line_len = 0;
        }
        return;
    }

    /* Ctrl+C */
    if (c == KEY_CTRL_C) {
        /* If command is running, set break flag */
        if (s_in_command) {
            s_break_requested = true;
            return;
        }

#if SHELL_FEATURE_COMPLETION
        if (shell_complete_is_menu_mode(&s_complete)) {
            shell_complete_menu_cancel(&s_complete, &s_render, s_prompt,
                                       &s_line, &s_last_line_len);
            return;
        }
#endif
        _sh_write("^C\r\n");
        _sh_clear_line_state();

        if (s_prompt) {
            _sh_write(ANSI_CURSOR_COL1);
            _sh_write(s_prompt);
            s_last_line_len = (uint16_t)strlen(s_prompt);
        } else {
            s_last_line_len = 0;
        }
        return;
    }

    /* Ctrl+L */
    if (c == KEY_CTRL_L) {
        shell_render_clear_screen(&s_render);
        _sh_redraw();
        return;
    }

    /* Ctrl+U / Ctrl+K */
    if (c == KEY_CTRL_U) {
        if (shell_input_kill_to_start(&s_line)) {
            _sh_redraw();
        }
        return;
    }
    if (c == KEY_CTRL_K) {
        if (shell_input_kill_to_end(&s_line)) {
            _sh_redraw();
        }
        return;
    }

    /* Ctrl+A / Ctrl+E */
    if (c == KEY_CTRL_A) {
        if (s_line.cursor > 0) {
            shell_render_cursor_left(&s_render, s_line.cursor);
            shell_input_move_home(&s_line);
        }
        return;
    }
    if (c == KEY_CTRL_E) {
        if (s_line.cursor < s_line.len) {
            shell_render_cursor_right(&s_render,
                                      (uint16_t)(s_line.len - s_line.cursor));
            shell_input_move_end(&s_line);
        }
        return;
    }

    /* Backspace */
    if (c == KEY_DEL || c == KEY_BACKSPACE) {
        if (shell_input_backspace(&s_line)) {
            _sh_redraw();
        }
        return;
    }

    /* Tab */
#if SHELL_FEATURE_COMPLETION
    if (c == KEY_TAB) {
        if (shell_complete_is_menu_mode(&s_complete)) {
            shell_complete_menu_down(&s_complete, &s_render, s_prompt,
                                     SHELL_CMDS(), SHELL_CMD_COUNT(), &s_line,
                                     &s_last_line_len);
        } else {
            shell_complete_handle_tab(&s_complete, &s_line, SHELL_CMDS(),
                                      SHELL_CMD_COUNT(), &s_render, s_prompt,
                                      &s_last_line_len);
        }
        return;
    }

    /* typing while in menu mode exits menu */
    if (shell_complete_is_menu_mode(&s_complete)) {
        shell_complete_exit_menu_on_typing(&s_complete, &s_render);
    }
#endif

#if SHELL_FEATURE_HISTORY
    cmdhist_reset_browse();
#endif

    /* printable */
    if (isprint((unsigned char)c)) {
        if (shell_input_insert_char(&s_line, c)) {
            _sh_redraw();
        }
    }
}

void shell_task(void)
{
    char c;

    if (!s_read_char || !s_read_char(&c)) {
        return;
    }

    shell_process_char(c);
}

const shell_command_t *const *shell_get_commands(size_t *count)
{
    if (count) {
        *count = SHELL_CMD_COUNT();
    }
    return SHELL_CMDS();
}

/* =====================================================================
 * Command Break API
 * ===================================================================== */

bool shell_is_break_requested(void)
{
    return s_break_requested;
}

bool shell_poll_break(void)
{
    if (!s_read_char) {
        return s_break_requested;
    }

    /* Read any pending characters and check for Ctrl+C */
    char c;
    while (s_read_char(&c)) {
        if (c == KEY_CTRL_C) {
            s_break_requested = true;
            break;
        }
    }

    return s_break_requested;
}

#ifndef SHELL_COMPLETE_H
#define SHELL_COMPLETE_H

#include <stdint.h>
#include <stdbool.h>

#include "shell.h"
#include "shell_input.h"
#include "shell_render.h"

typedef struct {
#if SHELL_FEATURE_COMPLETION
    const char *matches[SHELL_MAX_COMPLETION_MATCHES];
    uint16_t match_count;
    bool menu_mode;
    uint16_t menu_count;
    uint16_t menu_selected;
    char menu_prefix[SHELL_MENU_PREFIX_SIZE];
#endif
} shell_complete_t;

void shell_complete_init(shell_complete_t *c);

#if SHELL_FEATURE_COMPLETION
void shell_complete_handle_tab(shell_complete_t *c, shell_line_t *line,
                               const shell_command_t *const *cmds,
                               size_t cmd_count, shell_render_t *render,
                               const char *prompt, uint16_t *last_line_len);
bool shell_complete_is_menu_mode(const shell_complete_t *c);
void shell_complete_menu_up(shell_complete_t *c, shell_render_t *render,
                            const char *prompt,
                            const shell_command_t *const *cmds,
                            size_t cmd_count, shell_line_t *line,
                            uint16_t *last_line_len);
void shell_complete_menu_down(shell_complete_t *c, shell_render_t *render,
                              const char *prompt,
                              const shell_command_t *const *cmds,
                              size_t cmd_count, shell_line_t *line,
                              uint16_t *last_line_len);
void shell_complete_menu_select(shell_complete_t *c, shell_render_t *render,
                                const char *prompt, shell_line_t *line,
                                uint16_t *last_line_len);
void shell_complete_menu_cancel(shell_complete_t *c, shell_render_t *render,
                                const char *prompt, shell_line_t *line,
                                uint16_t *last_line_len);
void shell_complete_exit_menu_on_typing(shell_complete_t *c,
                                        shell_render_t *render);
#endif

#endif /* SHELL_COMPLETE_H */

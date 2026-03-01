#ifndef SHELL_CONFIG_H
#define SHELL_CONFIG_H

/* =====================================================================
 * User Configuration (Only 3 options!)
 * ===================================================================== */

/* Enable command history (up/down arrows) */
#ifndef SHELL_FEATURE_HISTORY
#define SHELL_FEATURE_HISTORY 1
#endif

/* Enable tab completion */
#ifndef SHELL_FEATURE_COMPLETION
#define SHELL_FEATURE_COMPLETION 1
#endif

/* Input buffer size (32-1024 bytes) */
#ifndef SHELL_INPUT_BUF_SIZE
#define SHELL_INPUT_BUF_SIZE 128
#endif

/* =====================================================================
 * Internal Derived Values (Do not modify)
 * ===================================================================== */

#define CMDHIST_HISTORY_SIZE        10
#define CMDHIST_MAX_LEN             (SHELL_INPUT_BUF_SIZE / 2)
#define SHELL_MAX_COMPLETION_MATCHES 16
#define SHELL_MENU_PREFIX_SIZE      64
#define SHELL_MAX_CMD_NAME          32

/* =====================================================================
 * Validation
 * ===================================================================== */

#if SHELL_INPUT_BUF_SIZE < 32
#error "SHELL_INPUT_BUF_SIZE too small (minimum 32)"
#endif

#if SHELL_INPUT_BUF_SIZE > 1024
#warning "SHELL_INPUT_BUF_SIZE is very large (>1KB)"
#endif

#endif /* SHELL_CONFIG_H */

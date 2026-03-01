#ifndef CMDHIST_H
#define CMDHIST_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* =====================================================================
 * Const Define
 * ===================================================================== */

#ifndef CMDHIST_HISTORY_SIZE
#define CMDHIST_HISTORY_SIZE 10
#endif

#ifndef CMDHIST_MAX_LEN
#define CMDHIST_MAX_LEN 64
#endif

#define CMDHIST_NOT_BROWSING UINT16_MAX

/* =====================================================================
 * Warning
 * ===================================================================== */

#if CMDHIST_HISTORY_SIZE < 1
#error "CMDHIST_HISTORY_SIZE must be at least 1"
#endif

#if CMDHIST_MAX_LEN < 16
#error "CMDHIST_MAX_LEN must be at least 16"
#endif

#if CMDHIST_HISTORY_SIZE > UINT16_MAX
#error "CMDHIST_HISTORY_SIZE too large (max 65535)"
#endif

/* =====================================================================
 * Public API
 * ===================================================================== */

void cmdhist_init(void);
bool cmdhist_add(const char *line);
const char *cmdhist_prev(void);
const char *cmdhist_next(void);
void cmdhist_reset_browse(void);
uint16_t cmdhist_get_count(void);
bool cmdhist_is_browsing(void);
void cmdhist_clear(void);

#endif /* CMDHIST_H */

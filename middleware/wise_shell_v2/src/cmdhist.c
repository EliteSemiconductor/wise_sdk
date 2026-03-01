#include "cmdhist.h"
#include <string.h>
#include <ctype.h>

static char s_hist_buffer[CMDHIST_HISTORY_SIZE][CMDHIST_MAX_LEN];

static uint16_t s_next_index = 0;
static uint16_t s_count      = 0;

static uint16_t s_browse_pos = CMDHIST_NOT_BROWSING;

static inline uint16_t _logical_to_phys(uint16_t logical)
{
    if (s_count < CMDHIST_HISTORY_SIZE) {
        return logical;
    }

    return (s_next_index + logical) % CMDHIST_HISTORY_SIZE;
}

static bool _is_empty_or_whitespace(const char *str)
{
    if (!str || str[0] == '\0') {
        return true;
    }

    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return false;
        }
        str++;
    }

    return true;
}

static const char *_get_last_entry(void)
{
    if (s_count == 0) {
        return NULL;
    }

    uint16_t last_logical = s_count - 1;
    uint16_t last_phys    = _logical_to_phys(last_logical);
    return s_hist_buffer[last_phys];
}

void cmdhist_init(void)
{
    memset(s_hist_buffer, 0, sizeof(s_hist_buffer));

    s_next_index = 0;
    s_count      = 0;
    s_browse_pos = CMDHIST_NOT_BROWSING;
}

bool cmdhist_add(const char *line)
{
    if (!line) {
        return false;
    }

    if (_is_empty_or_whitespace(line)) {
        return false;
    }

    /* Skip duplicate (same as last command) */
    const char *last = _get_last_entry();
    if (last && strcmp(last, line) == 0) {
        return false;
    }

    strncpy(s_hist_buffer[s_next_index], line, CMDHIST_MAX_LEN - 1);
    s_hist_buffer[s_next_index][CMDHIST_MAX_LEN - 1] = '\0';

    s_next_index = (s_next_index + 1) % CMDHIST_HISTORY_SIZE;

    if (s_count < CMDHIST_HISTORY_SIZE) {
        s_count++;
    }

    s_browse_pos = CMDHIST_NOT_BROWSING;

    return true;
}

const char *cmdhist_prev(void)
{
    if (s_count == 0) {
        return NULL;
    }

    if (s_browse_pos == CMDHIST_NOT_BROWSING) {
        s_browse_pos = s_count - 1;
    } else if (s_browse_pos > 0) {
        s_browse_pos--;
    } else {
        /* do noting */
    }

    uint16_t phys = _logical_to_phys(s_browse_pos);
    return s_hist_buffer[phys];
}

const char *cmdhist_next(void)
{
    if (s_count == 0) {
        return NULL;
    }

    if (s_browse_pos == CMDHIST_NOT_BROWSING) {
        return NULL;
    }

    if (s_browse_pos < s_count - 1) {
        s_browse_pos++;
        uint16_t phys = _logical_to_phys(s_browse_pos);
        return s_hist_buffer[phys];
    }

    s_browse_pos = CMDHIST_NOT_BROWSING;
    return NULL;
}

void cmdhist_reset_browse(void)
{
    s_browse_pos = CMDHIST_NOT_BROWSING;
}

uint16_t cmdhist_get_count(void)
{
    return s_count;
}

bool cmdhist_is_browsing(void)
{
    return (s_browse_pos != CMDHIST_NOT_BROWSING);
}

void cmdhist_clear(void)
{
    memset(s_hist_buffer, 0, sizeof(s_hist_buffer));

    s_next_index = 0;
    s_count      = 0;
    s_browse_pos = CMDHIST_NOT_BROWSING;
}

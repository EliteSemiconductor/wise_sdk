/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>

#define MAX_SHELL_CMD_HISTORY 4
#define MAX_SHELL_CMD_LEN 64

static char m_cmdhist[MAX_SHELL_CMD_HISTORY][MAX_SHELL_CMD_LEN];
static int m_cmdidx, m_rtrvIdx;

static int _cmdhist_inc(int idx)
{
    idx++;

    if (idx >= MAX_SHELL_CMD_HISTORY) {
        idx = 0;
    }

    return idx;
}

static int _cmdhist_dec(int idx)
{
    idx--;

    if (idx < 0) {
        idx = MAX_SHELL_CMD_HISTORY - 1;
    }

    return idx;
}

void cmdhist_init()
{
    m_cmdidx  = 0;
    m_rtrvIdx = m_cmdidx;
    memset(m_cmdhist, 0, sizeof(m_cmdhist));
}

void cmdhist_add(const char *buf)
{
    strncpy(m_cmdhist[m_cmdidx], buf, MAX_SHELL_CMD_LEN - 1);
    m_cmdhist[m_cmdidx][MAX_SHELL_CMD_LEN - 1] =
        '\0'; // Ensure null-termination
    m_cmdidx  = _cmdhist_inc(m_cmdidx);
    m_rtrvIdx = m_cmdidx;
}

int cmdhist_prev(int n, char *buf)
{
    m_rtrvIdx = _cmdhist_dec(m_rtrvIdx);

    if (m_cmdhist[m_rtrvIdx][0] == '\0') {
        return 0; // Empty history slot
    }

    strncpy(buf, m_cmdhist[m_rtrvIdx], n - 1);
    buf[n - 1] = '\0'; // Ensure null-termination
    return 1;
}

int cmdhist_next(int n, char *buf)
{
    m_rtrvIdx = _cmdhist_inc(m_rtrvIdx);

    if (m_cmdhist[m_rtrvIdx][0] == '\0') {
        return 0; // Empty history slot
    }

    strncpy(buf, m_cmdhist[m_rtrvIdx], n - 1);
    buf[n - 1] = '\0'; // Ensure null-termination
    return 1;
}

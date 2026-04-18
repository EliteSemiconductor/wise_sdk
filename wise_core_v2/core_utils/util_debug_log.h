#ifndef _UTIL_DEBUG_LOG_H
#define _UTIL_DEBUG_LOG_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* =========================
 * Global runtime switch
 * ========================= */
extern bool g_wise_log_enable;

/* =========================
 * Compile-time log level
 * ========================= */
 
/* =========================
 * Log Level Definition
 * =========================
 * 0: NONE
 * 1: ERR
 * 2: INFO
 * 3: DBG
 */
#ifndef WISE_LOG_LEVEL
#define WISE_LOG_LEVEL                                      0
#endif

/* =========================
 * Base macro (with runtime control)
 * ========================= */
#define WISE_LOG_PRINT(tag, fmt, arg...)                    \
    do {                                                    \
        if (g_wise_log_enable) {                            \
            printf("[%s] " fmt, tag, ##arg);                \
            fflush(stdout);                                 \
        }                                                   \
    } while (0)

/* ERR */
#if (WISE_LOG_LEVEL >= 1)
#define WISE_LOG_ERR(fmt, arg...)                           WISE_LOG_PRINT("ERR", fmt, ##arg)
#else
#define WISE_LOG_ERR(fmt, arg...)
#endif

/* INFO */
#if (WISE_LOG_LEVEL >= 2)
#define WISE_LOG_INFO(fmt, arg...)                          WISE_LOG_PRINT("INFO", fmt, ##arg)
#else
#define WISE_LOG_INFO(fmt, arg...)
#endif

/* DBG */
#if (WISE_LOG_LEVEL >= 3)
#define WISE_LOG_DBG(fmt, arg...)                           WISE_LOG_PRINT("DBG", fmt, ##arg)
#else
#define WISE_LOG_DBG(fmt, arg...)
#endif

void util_log_enable(bool enable);

#endif

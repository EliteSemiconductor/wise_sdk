#ifndef _UTIL_DEBUG_LOG_H
#define _UTIL_DEBUG_LOG_H

#include <stdint.h>
#include "wise_core.h"

#define DBG_PRINT(fmt, arg...)                                                                                                                       \
    do {                                                                                                                                             \
        printf("[%s][%d]" fmt, __func__, __LINE__, ##arg);                                                                                           \
    } while (0)

#ifdef DEBUG_LOG_ENABLED
#define debug_printf(...) printf(__VA_ARGS__);
#else
#define debug_printf(...) ((void)0)
#endif

#endif

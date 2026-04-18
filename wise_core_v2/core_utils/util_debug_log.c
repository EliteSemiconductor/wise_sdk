#include "util_debug_log.h"

bool g_wise_log_enable = true;

void util_log_enable(bool enable)
{
    g_wise_log_enable = enable;
}


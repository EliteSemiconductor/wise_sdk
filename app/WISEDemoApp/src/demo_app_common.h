#ifndef __DEMO_APP_COMMON_H
#define __DEMO_APP_COMMON_H

#include <stdio.h>
#include <stdint.h>
#include "wise.h"
#include "wise_core.h"
#include "util.h"
#include "app_shell_commands.h"
#include "wise_shell_v2/src/shell.h"

/* The application name is supplied by each demo (demo_xxx_main.c defines
 * DEMO_APP_NAME and passes it here) instead of via a -D preprocessor define. */
void demo_app_common_init(const char *appName);
void print_banner(char* appName);
const char *demo_app_get_name(void);

#endif

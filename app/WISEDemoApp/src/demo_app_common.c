#include <stdio.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_radio_api.h"
#include "demo_app_common.h"
#include "wise_sys_api.h"

void print_bytes(uint8_t* data, int num)
{
    int i;
    
    for(i = 0; i < num; i++)
        printf("%02x", data[i]);
}

void print_banner(char* appName)
{
    WISE_SDK_VERSION_T sdkVer = {0};
    WISE_RADIO_VERSION_INFO_T radioVer = {0};
    uint8_t chipUnique[8] = {0};
    
    wise_core_get_version(&sdkVer);
    wise_radio_get_version(&radioVer);

    wise_sys_get_chip_unique(chipUnique);

    printf("\n");
    printf("============================================\n");
    printf("   ESMT WISE Demo Application: %s\n", appName);
    printf("   Built@ %s %s\n", __DATE__, __TIME__);
    printf("   WISE SDK Version %d.%04d %08x\n", sdkVer.verMajor, sdkVer.verMinor, sdkVer.signature);
    printf("   Radio Platform: %s V%d.%02d.%02d\n", radioVer.platform_name, radioVer.ver_major, radioVer.ver_minor, radioVer.ver_build);
    printf("   Chip Unique: ");
    print_bytes(chipUnique, 8);
    printf("\n");
    printf("============================================\n");
    fflush(stdout);
}

/* App name supplied by the demo via demo_app_common_init(); cached here so the
 * 'info' shell command (app_shell_commands.c) can print it later. */
static const char *s_app_name = "WISE Demo";

const char *demo_app_get_name(void)
{
    return s_app_name;
}

void demo_app_common_init(const char *appName)
{
    if (appName != NULL) {
        s_app_name = appName;
    }

    if (WISE_SUCCESS != wise_init())
	{
	    while (1);
	}

	print_banner((char*)s_app_name);
}

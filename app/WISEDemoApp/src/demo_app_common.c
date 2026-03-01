#include <stdio.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_radio_api.h"
#include "demo_app_common.h"
#include "wise_sys_api.h"

void print_banner(char* appName)
{
    WISE_SDK_VERSION_T sdkVer = {0};
    WISE_RADIO_VERSION_INFO_T radioVer = {0};

    wise_core_get_version(&sdkVer);
    wise_radio_get_version(&radioVer);

    printf("\n");
    printf("============================================\n");
    printf("   ESMT WISE Demo Application: %s\n", appName);
    printf("   Built@ %s %s\n", __DATE__, __TIME__);
    printf("   WISE SDK Version %d.%04d %08x\n", sdkVer.verMajor, sdkVer.verMinor, sdkVer.signature);
    printf("   Radio Platform: %s V%d.%02d.%02d\n", radioVer.platform_name, radioVer.ver_major, radioVer.ver_minor, radioVer.ver_build);
    printf("============================================\n");
    fflush(stdout);
}

void demo_app_common_init()
{
    if (WISE_SUCCESS != wise_init())
	{
	    while (1);
	}

	print_banner((char*)DEMO_APP_NAME);
}

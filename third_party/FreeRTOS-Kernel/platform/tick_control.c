/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* Platform includes. */
#include "wise.h"
#include "wise_wutmr_api.h"

#if (configOVERRIDE_DEFAULT_TICK_CONFIGURATION  > 0)
volatile bool is_tickless_wakeup = false;
void WUTimer_CallBackHandler(void *context)
{
    (void) context;

    if (is_tickless_wakeup == false)
    {
        if(xTaskIncrementTick() != pdFALSE)
        {
            portYIELD();
        }
    }
}

void vPortSetupTimerInterrupt(void)
{
    static bool is_cal_done = false;

    uint32_t intervalMs = 1000 / configTICK_RATE_HZ;
    #if (1000 % configTICK_RATE_HZ != 0)
        #warning "configTICK_RATE_HZ Non-integer tick period; timing accuracy may be degraded"
    #endif

    if (is_cal_done == false)
    {
        wise_wutmr_clk_calibrate(500);
        wise_wutmr_register_callback(WUTimer_CallBackHandler, NULL);
        is_cal_done = true;
    }

    NVIC_SetPriority(WUTMR_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
    wise_wutmr_enable_int();
    wise_wutmr_start(intervalMs, WISE_WUTMR_PERIODIC);
}
#endif

#if (configUSE_TICKLESS_IDLE == 2)
static uint32_t ulCountsRemainder = 0;
static uint32_t ulOverSleepErrorTicks = 0;
static uint32_t clkRate = 0;
uint32_t Calculate_Actual_Slept_Ticks(uint32_t ulElapsedCounts, TickType_t xExpectedIdleTime)
{
    uint64_t ullTotalCounts;
    uint32_t ulActualTicks;

    if (clkRate == 0)
    {
        clkRate = wise_wutmr_get_clk_rate();
    }

    ullTotalCounts = (uint64_t)ulElapsedCounts + ulCountsRemainder;

    ulActualTicks = (uint32_t)((ullTotalCounts * configTICK_RATE_HZ) / clkRate);

    ulCountsRemainder = (uint32_t)(ullTotalCounts - (((uint64_t)ulActualTicks * clkRate) / configTICK_RATE_HZ));

    if (ulActualTicks >= xExpectedIdleTime)
    {
        ulOverSleepErrorTicks = ulActualTicks - xExpectedIdleTime;
        return xExpectedIdleTime - 1;
    }
    else
    {
        return ulActualTicks;
    }
}

void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
    uint32_t ulOriginalMask = portSET_INTERRUPT_MASK_FROM_ISR();

    uint32_t ulPreSleepCount = wise_wutmr_get_counter();

    is_tickless_wakeup = true;

    uint32_t intervalMs = ((xExpectedIdleTime - ulOverSleepErrorTicks) * 1000) / configTICK_RATE_HZ;

    configPRE_SLEEP_PROCESSING(xExpectedIdleTime);

    wise_system_sleep(intervalMs);

    uint32_t ulElapsedCounts = wise_wutmr_get_counter() - ulPreSleepCount + (ulOverSleepErrorTicks * clkRate) / configTICK_RATE_HZ;

    ulOverSleepErrorTicks = 0;

    uint32_t ulActualTicks = Calculate_Actual_Slept_Ticks(ulElapsedCounts, xExpectedIdleTime);

    vTaskStepTick(ulActualTicks);

    configPOST_SLEEP_PROCESSING(xExpectedIdleTime);

    vPortSetupTimerInterrupt();

    is_tickless_wakeup = false;

    portCLEAR_INTERRUPT_MASK_FROM_ISR(ulOriginalMask);
}

void vPreSleepProcessing( uint32_t xExpectedIdleTime )
{
    NVIC_SetPriority(WUTMR_IRQn, 0);
}

void vPostSleepProcessing( uint32_t xExpectedIdleTime )
{

}
#endif

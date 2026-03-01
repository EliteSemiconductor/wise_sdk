/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if MIDDLEWARE_WISE_SYSTEM
#include "wise_core.h"
#include "api/wise_flash_api.h"
#include "api/wise_gpio_api.h"
#include "api/wise_spi_api.h"
#include "api/wise_sys_api.h"
#include "api/wise_tick_api.h"
#include "api/wise_uart_api.h"
#include "api/wise_wutmr_api.h"
#include "wise.h"

/*
#include "api/wise_timer_api.h"

#include "api/wise_pmu_api.h"
#include "api/wise_radio_api.h"
#include "api/wise_sha_api.h"
*/

#if MIDDLEWARE_WISE_SHELL
#include "shell.h"
#elif MIDDLEWARE_WISE_SHELL_V2
#include "wise_shell_v2/intf/intf_shell.h"
#endif

#if MIDDLEWARE_WISE_CTRL_CMD
#include "wise_ctrl_cmd.h"
#include "wise_ctrl_packet.h"
#endif

#ifdef ES_DEVICE_TRX_RADIO
#include "radio_lib/xcvr04/host_intf/host_intf_api.h"
#endif
// #define TASK_PROFILING

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
#include "wise_file_system.h"
#endif

typedef enum {
    E_TASK_STATE_IDLE = 0,
    E_TASK_STATE_RUN  = 1
} E_TASK_STATE_T;

typedef struct {
    wise_task_proc proc;
    void *pData;
    E_TASK_STATE_T state;
    int8_t name[WISE_MAX_TASK_NAME_LEN];
    uint8_t priority;
    uint32_t lastTS;
#ifdef TASK_PROFILING
    uint32_t totalRunTime;
    uint32_t maxProcessTime;
    uint32_t activeCount;
#endif
} ST_WISE_TASK_T;

typedef struct {
    E_PWR_MODE_EN_T pmEnable;
    uint8_t awakeCount;
} ST_WISE_PWR_CTRL_T;

typedef struct {
    uint8_t uartIntf;
    WISE_UART_CFG_T config;
    uint8_t *rxBuffer;
    uint32_t rxBufferSize;
    uint8_t *txBuffer;
    uint32_t txBufferSize;
} ST_UART_PRE_CONF_T;

static uint32_t rebootAfterMs   = 0;
static uint32_t rebootTimeStart = 0;
// static ST_WISE_PWR_CTRL_T pwrCtrl;

static ST_WISE_TASK_T taskBank[WISE_MAX_TASK_NUM];

#if (ES_UART0_RX_BUF_SIZE != 0)
uint8_t uartRxBuffer0[ES_UART0_RX_BUF_SIZE];
#else
#define uartRxBuffer0 NULL
#endif

#if (ES_UART1_RX_BUF_SIZE != 0)
uint8_t uartRxBuffer1[ES_UART1_RX_BUF_SIZE];
#else
#define uartRxBuffer1 NULL
#endif

#if (ES_UART2_RX_BUF_SIZE != 0)
uint8_t uartRxBuffer2[ES_UART2_RX_BUF_SIZE];
#else
#define uartRxBuffer2 NULL
#endif

#if (ES_UART0_TX_BUF_SIZE != 0)
uint8_t uartTxBuffer0[ES_UART0_TX_BUF_SIZE];
#else
#define uartTxBuffer0 NULL
#endif

#if (ES_UART1_TX_BUF_SIZE != 0)
uint8_t uartTxBuffer1[ES_UART1_TX_BUF_SIZE];
#else
#define uartTxBuffer1 NULL
#endif

#if (ES_UART2_TX_BUF_SIZE != 0)
uint8_t uartTxBuffer2[ES_UART2_TX_BUF_SIZE];
#else
#define uartTxBuffer2 NULL
#endif

static void _platform_init();
static void _peripheral_init();
static void _peripheral_restore();
static void _soft_init();
static void _hal_proc();
static void _soft_proc();
static void _wise_process_task();
void _wise_uart_dispatch_task();

extern void _wise_process_event();
extern uint8_t _wise_get_evt_num();
extern void _wise_schlr_proc();

static ST_UART_PRE_CONF_T uartCfg[] = {
#if (defined(ES_COMP_ENABLE_UART_0) && (ES_COMP_ENABLE_UART_0 == ENABLE))
    {
        0,
        {
            ES_UART0_BAUDRATE,
            E_UART_DATA_8_BIT,
            E_UART_PARITY_NONE,
            E_UART_STOP_1_BIT,
            0,
        },
        uartRxBuffer0,
        ES_UART0_RX_BUF_SIZE,
        uartTxBuffer0,
        ES_UART0_TX_BUF_SIZE,
    },
#endif
#if (defined(ES_COMP_ENABLE_UART_1) && (ES_COMP_ENABLE_UART_1 == ENABLE))
    {
        1,
        {
            ES_UART1_BAUDRATE,
            E_UART_DATA_8_BIT,
            E_UART_PARITY_NONE,
            E_UART_STOP_1_BIT,
            0,
        },
        uartRxBuffer1,
        ES_UART1_RX_BUF_SIZE,
        uartTxBuffer1,
        ES_UART1_TX_BUF_SIZE,
    },
#endif
#if (defined(ES_COMP_ENABLE_UART_2) && (ES_COMP_ENABLE_UART_2 == ENABLE))
    {
        2,
        {
            ES_UART2_BAUDRATE,
            E_UART_DATA_8_BIT,
            E_UART_PARITY_NONE,
            E_UART_STOP_1_BIT,
            0,
        },
        uartRxBuffer2,
        ES_UART2_RX_BUF_SIZE,
        uartTxBuffer2,
        ES_UART2_TX_BUF_SIZE,
    },
#endif
};

#if ES_DEVICE_PIO
uint8_t uartIOCfg[][2] = {
#if (defined(ES_COMP_ENABLE_UART_0) && (ES_COMP_ENABLE_UART_0 == ENABLE))
    {MODE_PIO_FUNC_UART0_TX, ES_UART0_TX_PIN}, {MODE_PIO_FUNC_UART0_RX, ES_UART0_RX_PIN},
#endif
#if (defined(ES_COMP_ENABLE_UART_1) && (ES_COMP_ENABLE_UART_1 == ENABLE))
    {MODE_PIO_FUNC_UART1_TX, ES_UART1_TX_PIN}, {MODE_PIO_FUNC_UART1_RX, ES_UART1_RX_PIN},
#endif
#if (defined(ES_COMP_ENABLE_UART_2) && (ES_COMP_ENABLE_UART_2 == ENABLE))
    {MODE_PIO_FUNC_UART2_TX, ES_UART2_TX_PIN}, {MODE_PIO_FUNC_UART2_RX, ES_UART2_RX_PIN},
#endif
};
#else //ES_DEVICE_PIO
WISE_GPIO_CFG_T uartIOCfg[] = {
#if (defined(ES_COMP_ENABLE_UART_0) && (ES_COMP_ENABLE_UART_0 == ENABLE))
    {ES_UART0_TX_PIN, MODE_PERI_1, GPIO_DIR_NONE, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
    {ES_UART0_RX_PIN, MODE_PERI_1, GPIO_DIR_NONE, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
#endif
#if (defined(ES_COMP_ENABLE_UART_1) && (ES_COMP_ENABLE_UART_1 == ENABLE))
    {ES_UART1_TX_PIN, MODE_PERI_1, GPIO_DIR_NONE, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
    {ES_UART1_RX_PIN, MODE_PERI_1, GPIO_DIR_NONE, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
#endif
#if (defined(ES_COMP_ENABLE_UART_2) && (ES_COMP_ENABLE_UART_2 == ENABLE))
    {ES_UART2_TX_PIN, MODE_PERI_1, GPIO_DIR_NONE, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
    {ES_UART2_RX_PIN, MODE_PERI_1, GPIO_DIR_NONE, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
#endif
};
#endif //ES_DEVICE_PIO

static const uint8_t dma_channel_map[SYS_DMA_CHANNEL_NUM] = {DMA_CH0_FUNC, DMA_CH1_FUNC, DMA_CH2_FUNC, DMA_CH3_FUNC, DMA_CH4_FUNC, DMA_CH5_FUNC};

static const WISE_SYS_BOARD_PROPERTY_T boardProperty = {
    .tcxo_output_en = BOARD_TCXO_OUTPUT_EN,
    .pa_type = BOARD_PA_TYPE,
    .matching_type = BOARD_BAND_MATCHING,
    .gain_ctrl_40m = BOARD_40M_GAIN_CTRL,
    .gain_ctrl_40m_s = BOARD_40M_GAIN_CTRL_S,
    .cap_xtal_i = BOARD_40M_CAP_XTAL_I,
    .cap_xtal_o = BOARD_40M_CAP_XTAL_O,
    .sram_retain = BOARD_SRAM_RETAIN,
};

static void _platform_init()
{
    WISE_LFOSC_SRC_T oscCfg = {
        .clk_src          = ES_DEVICE_LFOSC_SELECT,
        .mode.mode_select = ES_DEVICE_LFOSC_OPTION,
        .calFinish        = 0,
    };

    if (WISE_SUCCESS != wise_core_init()) {
        while (1)
            ;
    }

#ifndef TARGET_SBL
    wise_sys_lfosc_clk_src_config(oscCfg);
    wise_sys_lfosc_clk_calibration();

    wise_wutmr_init();
    wise_wutmr_enable();
#endif

    wise_sys_set_board_property(&boardProperty);

    wise_gpio_init();
    wise_tick_init();
    wise_flash_init();

#ifndef TARGET_SBL
    wise_sys_init_dma_channel(dma_channel_map);
    wise_sys_dma_channel_export();
#endif
}

static void _peripheral_init()
{
    int i;
    int cfgNum             = 0;
    WISE_BUFFER_T rxBuffer = {0}, txBuffer = {0};

    wise_uart_init();

    cfgNum = sizeof(uartCfg) / sizeof(ST_UART_PRE_CONF_T);
    for (i = 0; i < cfgNum; i++) {
        rxBuffer.length = uartCfg[i].rxBufferSize;
        rxBuffer.addr   = (uint32_t)uartCfg[i].rxBuffer;
        txBuffer.length = uartCfg[i].txBufferSize;
        txBuffer.addr   = (uint32_t)uartCfg[i].txBuffer;

        wise_uart_config(uartCfg[i].uartIntf, &uartCfg[i].config);
        wise_uart_set_buffer(uartCfg[i].uartIntf, &rxBuffer, &txBuffer);
        wise_uart_enable(uartCfg[i].uartIntf, WISE_UART_FLAG_RX | WISE_UART_FLAG_TX);
        wise_uart_enable_interrupt(uartCfg[i].uartIntf);
    }

#if ES_DEVICE_PIO
    cfgNum = sizeof(uartIOCfg) / 2;
    for (i = 0; i < cfgNum; i++) {
        wise_gpio_func_cfg(uartIOCfg[i][1], uartIOCfg[i][0]);
    }
#else
    cfgNum = sizeof(uartIOCfg) / sizeof(WISE_GPIO_CFG_T);
    for (i = 0; i < cfgNum; i++) {
        wise_gpio_cfg(&uartIOCfg[i]);
    }
#endif //ES_DEVICE_PIO

#if (defined MIDDLEWARE_WISE_WUTMR_CLK_CAL) && (MIDDLEWARE_WISE_WUTMR_CLK_CAL == 1)
    wise_wutmr_clk_calibrate(2000); //sw clock calibration for 1000ms
#endif
}

static void _peripheral_restore()
{
    int i;
    int cfgNum = 0;

    cfgNum = sizeof(uartCfg) / sizeof(ST_UART_PRE_CONF_T);
    for (i = 0; i < cfgNum; i++) {
        wise_uart_config(uartCfg[i].uartIntf, &uartCfg[i].config);
        wise_uart_enable(uartCfg[i].uartIntf, WISE_UART_FLAG_RX | WISE_UART_FLAG_TX);
        wise_uart_enable_interrupt(uartCfg[i].uartIntf);
    }

#if ES_DEVICE_PIO
    cfgNum = sizeof(uartIOCfg) / 2;
    for (i = 0; i < cfgNum; i++) {
        wise_gpio_func_cfg(uartIOCfg[i][1], uartIOCfg[i][0]);
    }
#else
    cfgNum = sizeof(uartIOCfg) / sizeof(WISE_GPIO_CFG_T);
    for (i = 0; i < cfgNum; i++) {
        wise_gpio_cfg(&uartIOCfg[i]);
    }
#endif
}

#if (MIDDLEWARE_WISE_SHELL || MIDDLEWARE_WISE_SHELL_V2 || MIDDLEWARE_WISE_CTRL_CMD)
void _wise_uart_dispatch_task()
{
    uint8_t inputC;

    if ((wise_uart_check_data_len(STDIO_UART_PORT) > 0) && (wise_uart_read_char(STDIO_UART_PORT, &inputC) != WISE_FAIL)) {
#if (MIDDLEWARE_WISE_CTRL_CMD == ENABLE) && (CTRL_CMD_UART_PORT == STDIO_UART_PORT)
        if (wise_pkt_input(inputC)) {
            return;
        }
#endif

#if MIDDLEWARE_WISE_SHELL
        eshell_char_input(inputC);
#elif MIDDLEWARE_WISE_SHELL_V2
        shell_process_char(inputC);
#endif
    }

#if (MIDDLEWARE_WISE_CTRL_CMD == ENABLE) && (CTRL_CMD_UART_PORT != STDIO_UART_PORT)
    if ((wise_uart_check_data_len(CTRL_CMD_UART_PORT) > 0) && (wise_uart_read_char(CTRL_CMD_UART_PORT, &inputC) != WISE_FAIL)) {
        wise_pkt_input(inputC);
    }
#endif
}

#if MIDDLEWARE_WISE_CTRL_CMD
static int32_t _ctrl_pkt_handler(uint8_t *buffer, int len)
{
    if (len > 0) {
        wise_ctrl_cmd_process(buffer, len);
    }

    return WISE_SUCCESS;
}

static int32_t _ctrl_pkt_output(uint8_t *buffer, int len)
{
    if (len > 0) {
        wise_uart_write(CTRL_CMD_UART_PORT, buffer, len);
    }
    return WISE_SUCCESS;
}
#endif // MIDDLEWARE_WISE_CTRL_CMD

#endif //(MIDDLEWARE_WISE_SHELL|| MIDDLEWARE_WISE_SHELL_V2 || MIDDLEWARE_WISE_CTRL_CMD)

static void _soft_init()
{
    memset(taskBank, 0, sizeof(taskBank));

#if MIDDLEWARE_WISE_SYSTEM
    wise_event_init();
    wise_schlr_init();
#endif

#if MIDDLEWARE_WISE_SHELL_V2
    intf_shell_init();
#elif MIDDLEWARE_WISE_SHELL
    eshell_init();
#endif

#if MIDDLEWARE_WISE_CTRL_CMD
    wise_ctrl_cmd_init();
    wise_pkt_init(_ctrl_pkt_output, _ctrl_pkt_handler);
#endif

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
    wise_fs_init();
#endif
}

static void _hal_proc()
{
    wise_sys_proc();

#if ES_DEVICE_UART
    wise_uart_proc();
#endif
}

static void _soft_proc()
{
    _wise_process_task();

#if MIDDLEWARE_WISE_SYSTEM
    _wise_process_event();
    _wise_schlr_proc();
#endif

#if (MIDDLEWARE_WISE_SHELL || MIDDLEWARE_WISE_SHELL_V2 || MIDDLEWARE_WISE_CTRL_CMD)
    _wise_uart_dispatch_task();

#if MIDDLEWARE_WISE_CTRL_CMD
    wise_pkt_proc();
#endif // MIDDLEWARE_WISE_CTRL_CMD
#endif

#ifdef ES_DEVICE_TRX_RADIO
    host_intf_poll_radio_int_pin();
#endif

    if (rebootAfterMs > 0) {
        uint32_t timeLapsed = wise_tick_get_counter() - rebootTimeStart;

        if (timeLapsed > MS_TO_CLK(rebootAfterMs)) {
            wise_sys_chip_reset();
        }
    }
}

int32_t wise_init()
{
    _platform_init();
    _peripheral_init();
    _soft_init();
    
    return WISE_SUCCESS;
}

void wise_main_proc()
{
    _hal_proc();
    _soft_proc();
}

void wise_system_idle(uint32_t idleMs)
{
    WISE_DECLARE_CRITICAL;

    WISE_ENTER_CRITICAL();

    if (idleMs != WISE_TIME_FOREVER) {
        wise_wutmr_enable_int();
        wise_wutmr_start(idleMs, WISE_WUTMR_ONE_SHOT);
    }

    wise_sys_set_cpu_pd();
    wise_system_restore();

    if (idleMs != WISE_TIME_FOREVER) {
        wise_wutmr_stop();
        wise_wutmr_disable_int();
    }

    WISE_EXIT_CRITICAL();
}

void wise_system_sleep(uint32_t sleepMs)
{
    WISE_DECLARE_CRITICAL;

    WISE_ENTER_CRITICAL();

    if (sleepMs != WISE_TIME_FOREVER) {
        wise_wutmr_enable_int();
        wise_wutmr_start(sleepMs, WISE_WUTMR_ONE_SHOT);
    }

    wise_sys_enter_sleep_mode();
    wise_system_restore();

    if (sleepMs != WISE_TIME_FOREVER) {
        wise_wutmr_stop();
        wise_wutmr_disable_int();
    }

    WISE_EXIT_CRITICAL();
}

void wise_system_restore()
{
    _peripheral_restore();
}

void wise_pm_enable(E_PWR_MODE_EN_T enable)
{
}

void wise_pm_set_mode(E_PWR_MODE_T mode)
{
}

#if 0
static E_PWR_MODE_T wiseTaskCheckPWRMode()
{
    ST_WISE_TASK_T* task = NULL;
    int i;
    
    for(i = 0; i < WISE_MAX_TASK_NUM; i++)
    {
        task = &taskBank[i];
        if(task->state == E_TASK_STATE_RUN)
            return E_WISE_PM_KEEP_AWAKE;
    }

    return E_WISE_PM_ALLOW_SLEEP;
}
#endif

#if 0
void gpio_pin_irq_callback(void* cb_data)
{
	printf("%s: cb_data pointer = %p\n", __func__, cb_data);
}

void rtc_alarm0_isr_cb(void* cb_data)
{
	printf("[%s] ALARM!! \n", __func__);
}
#endif

void pmu_isr_cb(void *cb_data)
{
    printf("[%s] BOD event!! \n", __func__);
}

#if 0
int8_t wise_pm_is_system_busy()
{
    if(0 == _wise_get_evt_num() && (pwrCtrl.awakeCount == 0)
        && (1 == wise_fifo_check_empty()))
    {
        uint32_t sleepUs = wiseSchlrNextEventTime();

        if(sleepUs == 0xffffffff)
        {
            //todo: call sleep API
            //wise_delay_us(sleepUs);
            //wise_pmu_set_sleep(sleepUs);
            return 1;
        }
    }

    return 0;
}
#endif

#if 0
void wise_main_proc()
{
    if(rebootAfterMs > 0)
    {
        uint32_t now = wise_get_clk_counter();
        
        if(CLK_TO_MS(time_diff(rebootTimeStart, now)) >= rebootAfterMs)
            NVIC_SystemReset();
    }


    wise_radio_loop();


    _wise_process_task();
    _wise_process_event();
    wise_schlr_proc();

#ifdef WISE_PM_CTRL
#ifndef PROTOCOL_PM_CTRL
    wise_core_disable_irq();
    if(0 == _wise_get_evt_num() && (pwrCtrl.awakeCount == 0)
        && (1 == wise_fifo_check_empty()))
    {
        uint32_t sleepUs = wiseSchlrNextEventTime();

        if(sleepUs > 0)
        {
#if 0
            uint32_t now = wise_get_tu();
            
            //debug_print("s %08x\n", sleepUs);
            now = time_diff(now, wise_get_tu());
            sleepUs -= (SCHLR_UNIT_TO_US(now));
#endif
            
            //todo: call sleep API
            //wise_delay_us(sleepUs);
            wise_pmu_set_sleep(sleepUs);
        }
        else
            debug_print(".");
    }
    wise_core_enable_irq();
#endif
#endif

}
#endif

#if 0
void wise_pm_enable(E_PWR_MODE_EN_T enable)
{
    pwrCtrl.pmEnable = enable;
}

void wise_pm_set_mode(E_PWR_MODE_T mode)
{
    if(mode == E_WISE_PM_KEEP_AWAKE)
    {
        pwrCtrl.awakeCount++;
        if(pwrCtrl.awakeCount >= 20)
        {
            debug_print("BUG!! many awakenCount!!\n");
        }
    }
    else
    {
        if(pwrCtrl.awakeCount > 0)
        {
            pwrCtrl.awakeCount--;
        }
        else
        {
            debug_print("BUG!! negative awakenCount!!\n");
        }
    }
}
#endif

int8_t wise_create_task(wise_task_proc pollProc, void *pData, int8_t *name, uint8_t priority)
{
    int i;
    ST_WISE_TASK_T *newTask = NULL;

    for (i = 0; i < WISE_MAX_TASK_NUM; i++) {
        newTask = &taskBank[i];

        if (newTask->proc == NULL) {
            newTask->proc     = pollProc;
            newTask->pData    = pData;
            newTask->state    = E_TASK_STATE_IDLE;
            newTask->priority = priority;
            newTask->lastTS   = 0;

            if (name) {
                strncpy((char *)newTask->name, (char *)name, WISE_MAX_TASK_NAME_LEN - 1);
            } else {
                newTask->name[0] = 0;
            }

            return i;
        }
    }

    return WISE_INVALID_INDEX;
}

int8_t wise_remove_task(int8_t taskId)
{
    if ((taskId >= 0) && (taskId < WISE_MAX_TASK_NUM)) {
        memset(&taskBank[taskId], 0, sizeof(ST_WISE_TASK_T));
    }

    return WISE_SUCCESS;
}

int8_t wise_set_task_active(int8_t taskId)
{
    if ((taskId >= 0) && (taskId < WISE_MAX_TASK_NUM)) {
        taskBank[taskId].state = E_TASK_STATE_RUN;
    }

    return WISE_SUCCESS;
}

int8_t wise_set_task_idle(int8_t taskId)
{
    if ((taskId >= 0) && (taskId < WISE_MAX_TASK_NUM)) {
        taskBank[taskId].state = E_TASK_STATE_IDLE;
    }

    return WISE_SUCCESS;
}

int8_t wise_request_reboot(uint32_t afterMs)
{
    if (rebootAfterMs != 0) {
        debug_print("already in rebooting\n");
        return WISE_FAIL;
    }

    rebootAfterMs   = afterMs;
    rebootTimeStart = wise_tick_get_counter();

    return WISE_SUCCESS;
}

static void _wise_process_task()
{
    int i;

    for (i = 0; i < WISE_MAX_TASK_NUM; i++) {
        ST_WISE_TASK_T *pTask = &taskBank[i];

        if ((pTask->proc) && (pTask->state == E_TASK_STATE_RUN)) {
#ifdef TASK_PROFILING
            uint32_t startTime   = (uint32_t)wise_rtc_get_clock_value();
            uint32_t processTime = 0;

            (pTask->proc)(pTask->pData);

            processTime = (uint32_t)wise_rtc_get_clock_value() - startTime;

            if (processTime > pTask->maxProcessTime) {
                pTask->maxProcessTime = processTime;
            }
            pTask->totalRunTime += processTime;
            pTask->activeCount++;
#else
            (pTask->proc)(pTask->pData);
#endif
        }
    }

#ifdef TASK_PROFILING
    if (profilingClearReq) {
        wise_task_profiling_reset();
        profilingClearReq = 0;
    }
#endif
}

uint8_t *debugPins  = NULL;
uint8_t debugPinNum = 0;
void wise_debug_io_init(uint8_t *pinArray, uint8_t pinNum)
{
    int i;

    if (pinArray && pinNum) {
        WISE_GPIO_CFG_T pinCfg = {0, MODE_GPIO, GPIO_DIR_OUTPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE};

        debugPins   = pinArray;
        debugPinNum = pinNum;

        for (i = 0; i < debugPinNum; i++) {
            pinCfg.pin_idx = debugPins[i];
            wise_gpio_cfg(&pinCfg);
            wise_gpio_write(debugPins[i], GPIO_LOW);
        }
    }
}

void wise_debug_io_high(uint8_t index)
{
    if ((debugPinNum) && (index < debugPinNum)) {
        wise_gpio_write(debugPins[index], GPIO_HIGH);
    }
}

void wise_debug_io_low(uint8_t index)
{
    if ((debugPinNum) && (index < debugPinNum)) {
        wise_gpio_write(debugPins[index], GPIO_LOW);
    }
}

void wise_debug_io_toggle(uint8_t index)
{
    if ((debugPinNum) && (index < debugPinNum)) {
        wise_gpio_toggle(debugPins[index]);
    }
}

#endif /* MIDDLEWARE_WISE_SYSTEM*/

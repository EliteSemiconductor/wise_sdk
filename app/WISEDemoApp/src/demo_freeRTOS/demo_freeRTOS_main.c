/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file main.c
 * @brief FreeRTOS example: UART CLI with inter-task message queue and LED heartbeat.
 *
 * @ingroup WISE_EXAMPLE_APP_FREERTOS_CLI
 *
 * This example application demonstrates:
 * - A UART-based command line interface (CLI) using wise_shell_v2
 * - Asynchronous task triggering via a FreeRTOS queue
 * - A worker task that consumes messages and reports processing latency
 * - A heartbeat LED task toggling a GPIO output periodically
 *
 * The CLI command `ping` allocates a payload buffer on heap and sends a message
 * to the worker task through a queue. The worker prints the payload and frees
 * the allocated buffer when done.
 *
 * @note This file is documented as an Example APP (not Core API / Middleware).
 * @note This example uses libc APIs such as strlen/memcpy; ensure <string.h>
 *       is included if required by your build settings.
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "wise.h"
#include "wise_core.h"
#include "wise_uart_api.h"
#include "wise_sys_api.h"
#include "wise_gpio_api.h"
#include "wise_wutmr_api.h"
#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"
#include "es_platform_components.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


/**
 * @defgroup WISE_EXAMPLE_APP_FREERTOS_CLI FreeRTOS CLI Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: FreeRTOS tasks + queue messaging + UART shell.
 *
 * This demo integrates the following Core API modules:
 * - @ref WISE_UART  for shell UART backend
 * - @ref WISE_GPIO  for LED heartbeat output
 * - @ref WISE_SYS   for system services (via demo init)
 *
 * And uses FreeRTOS primitives:
 * - Task creation and scheduling
 * - Queue for inter-task communication
 *
 * @{
 */

#define DEMO_APP_PROMPT             "FreeRTOS> "
#define MSG_QUEUE_LEN               5                 /**< Max number of pending messages. */
#define MSG_SIZE                    sizeof(ShellMsg_t) /**< Size of one queue item in bytes. */


/* ========================================================================== */
/* Message Types and Queue                                                    */
/* ========================================================================== */

/**
 * @enum MsgType_t
 * @brief Supported command types delivered to the worker task via message queue.
 */
typedef enum {
    CMD_TYPE_PING,     /**< Normal ping message. */
    CMD_TYPE_SYS_INFO  /**< Request system information (placeholder). */
} MsgType_t;

/**
 * @struct ShellMsg_t
 * @brief Message object passed from CLI to worker task.
 *
 * @note The @ref payload is allocated on heap in ::cmd_ping and must be freed
 *       by the consumer (worker task) after processing.
 */
typedef struct {
    MsgType_t   type;      /**< Message type, see ::MsgType_t. */
    char       *payload;   /**< Heap buffer containing message string. */
    TickType_t  timestamp; /**< Tick when message was enqueued. */
} ShellMsg_t;


/** Global queue handle for shell-to-worker messaging. */
QueueHandle_t xShellQueue = NULL;

extern bool shell_uart_read_char(char *ch);

/**
 * @brief FreeRTOS task: process UART input and feed characters to shell engine.
 *
 * Reads all available characters from UART and passes them to ::shell_process_char().
 *
 * @param[in] pvParameters Unused task parameter.
 */
void vShellTask(void *pvParameters)
{
    char c;
    (void)pvParameters;

    for (;;) {
        while (shell_uart_read_char(&c)) {
            shell_process_char(c);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/* ========================================================================== */
/* ping Command                                                               */
/* ========================================================================== */

/**
 * @brief Shell command: send a ping message to worker task.
 *
 * Usage:
 * @code
 * ping string
 * @endcode
 *
 * The command allocates a heap buffer for the payload and sends a message
 * to the worker task through @ref xShellQueue. The worker task is responsible
 * for freeing the allocated buffer.
 *
 * @param[in] argc Number of arguments.
 * @param[in] argv Argument list (argv[1] is the string message).
 *
 * @return 0 on success, negative value on invalid parameters.
 */
static int cmd_ping(int argc, char **argv)
{
    if (argc != 2) {
        printf("Invalid parameters:\nUsage: ping string\n");
        return -1;
    }

    size_t len = strlen(argv[1]) + 1;
    char *payload = (char *)pvPortMalloc(len);

    srand(xTaskGetTickCount());

    if (payload != NULL) {
        memcpy(payload, argv[1], len);

        MsgType_t randomType = (MsgType_t)(rand() % 2);

        ShellMsg_t xMsg = {
            .type = randomType,
            .payload = payload,
            .timestamp = xTaskGetTickCount()
        };

        if (xQueueSend(xShellQueue, &xMsg, 0) != pdPASS) {
            vPortFree(payload);
        }
    }
    return 0;
}

/** Register shell command "ping". */
SHELL_CMD_AUTO(ping, cmd_ping, "Ping message to worker");

/* ========================================================================== */
/* LED Blink Task                                                             */
/* ========================================================================== */

/**
 * @brief FreeRTOS task: heartbeat LED blinking.
 *
 * Configures GPIO pin 5 as output and toggles it periodically.
 *
 * @param[in] pv Unused task parameter.
 */
void vLEDBlinkTask(void *pv)
{
    (void)pv;

    wise_gpio_set_mode(5, MODE_GPIO);
    wise_gpio_set_direction(5, GPIO_DIR_OUTPUT);

    for (;;) {
        wise_gpio_toggle(5);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* ========================================================================== */
/* Worker Task                                                                */
/* ========================================================================== */

/**
 * @brief FreeRTOS task: worker that consumes messages from the shell queue.
 *
 * Waits for a @ref ShellMsg_t message from @ref xShellQueue, prints payload and
 * processing delay, then frees the payload buffer.
 *
 * @param[in] pvParameters Unused task parameter.
 */
void vWorkerTask(void *pvParameters)
{
    ShellMsg_t xMsg;
    (void)pvParameters;

    for (;;) {
        if (xQueueReceive(xShellQueue, &xMsg, portMAX_DELAY) == pdPASS) {
            TickType_t xNow = xTaskGetTickCount();

            printf("\n[Worker] Receive cmd!\n");
            printf("- Content: %s\n", xMsg.payload);
            printf("- Delay: %u ms\n",
                   (unsigned int)((xNow - xMsg.timestamp) * portTICK_PERIOD_MS));

            if (xMsg.type == CMD_TYPE_PING) {
                /* Placeholder: handle ping type. */
            } else if (xMsg.type == CMD_TYPE_SYS_INFO) {
                /* Placeholder: handle system-info type. */
            }

            printf("[Worker] Finish!\n> ");
            vPortFree(xMsg.payload);
        }
    }
}

/* ========================================================================== */
/* Main Entry Point                                                           */
/* ========================================================================== */

/**
 * @brief Main entry: initialize demo environment, create RTOS objects, and start scheduler.
 *
 * Steps:
 * 1. Initialize common demo environment and shell
 * 2. Create message queue
 * 3. Create tasks with assigned priorities
 * 4. Start the FreeRTOS scheduler
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    /* Create RTOS Objects */
    xShellQueue = xQueueCreate(MSG_QUEUE_LEN, MSG_SIZE);

    if (xShellQueue != NULL) {
        /* Task Creation with Priority Assignments */
        xTaskCreate(vShellTask,     "Shell",   512, NULL, configMAX_PRIORITIES - 1, NULL);
        xTaskCreate(vWorkerTask,    "Worker",  256, NULL, configMAX_PRIORITIES - 2, NULL);
        xTaskCreate(vLEDBlinkTask,  "LED",     128, NULL, configMAX_PRIORITIES - 3, NULL);

        /* Start Scheduler */
        vTaskStartScheduler();
    }

    for( ; ; );
}

/** @} */ /* end of WISE_EXAMPLE_APP_FREERTOS_CLI */

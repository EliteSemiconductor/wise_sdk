/* Auto-generated es_platform_components.h */
#ifndef _COMPONENTS_DEFAULT_H
#define _COMPONENTS_DEFAULT_H

/**
 * ============================
 *    TARGET
 * ============================
 */
#define CONFIG_ER8130 1
#define TARGET_APP 1
#define WORKAROUND_TX 1

/**
 * ============================
 *    DEVICE
 * ============================
 */
#define ES_DEVICE_PMU 1
#define ES_DEVICE_UART 1
#define ES_DEVICE_RADIO 1
#define ES_DEVICE_RADIO_INTF_NUM 1
#define ES_DEVICE_TIMER 1
#define ES_DEVICE_SPI 1
#define ES_DEVICE_RTC 1
#define ES_DEVICE_I2C 1
#define ES_DEVICE_PWM 1
#define ES_DEVICE_PWMSLOW 1
#define ES_DEVICE_PIO 1

/**
 * ============================
 *    UART CONFIG
 * ============================
 */
#define ES_COMP_ENABLE_UART_0 1
#define ES_UART0_BAUDRATE 115200
#define ES_UART0_TX_PIN 0
#define ES_UART0_RX_PIN 1
#define ES_UART0_RX_BUF_SIZE 256
//#define ES_UART0_TX_BUF_SIZE 2048

/**
 * ============================
 *    RADIO CONFIG
 * ============================
 */
#define ER81XX_MATCHING_FREQ_915MHZ 1

/**
 * ============================
 *    DMA CONFIG
 * ============================
 */
#define DMA_CH0_FUNC SYS_DMA_FUNC_SPI0_TX
#define DMA_CH1_FUNC SYS_DMA_FUNC_SPI0_RX
#define DMA_CH2_FUNC SYS_DMA_FUNC_AES_AUTH_IN
#define DMA_CH3_FUNC SYS_DMA_FUNC_AES_IN
#define DMA_CH4_FUNC SYS_DMA_FUNC_AES_OUT
#define DMA_CH5_FUNC SYS_DMA_FUNC_SHA

/**
 * ============================
 *    MIDDLEWARE CONFIG
 * ============================
 */
#define MIDDLEWARE_RETARGET_STDIO 1
#define MIDDLEWARE_WISE_SHELL 0
#define MIDDLEWARE_WISE_SHELL_V2 1
#define MIDDLEWARE_WISE_SYSTEM 1
#define MIDDLEWARE_WISE_CTRL_CMD 1
/**
 * ============================
 *    MIDDLEWARE RETARGET
 * ============================
 */
#define STDIO_UART_PORT 0
#define STDIO_PRINTF_HDL 1
#define STDIO_PRINTF_DRV 1
#define STDIO_PRINTF_INTF 1
#define STDIO_PRINTF_API 1

/**
 * ============================
 *    MIDDLEWARE SHELL
 * ============================
 */
#define SHELL_RADIO 1
#define SHELL_TIMER 1
#define SHELL_RTC 1
#define SHELL_SPI 1
#define SHELL_I2C 1
#define SHELL_PWM 1
#define SHELL_PWMSLOW 1
#define SHELL_TRNG 1
#define SHELL_WDT 1
#define SHELL_GPIO 1
#define SHELL_PIO 1
#define SHELL_CRYPTO 1
#define SHELL_SYS 1
#define SHELL_NFC 1
#define SHELL_EFUSE 1

#if MIDDLEWARE_WISE_CTRL_CMD
#define CTRL_CMD_UART_PORT 0
#endif /* MIDDLEWARE_WISE_CTRL_CMD */

#endif /* _COMPONENTS_DEFAULT_H */

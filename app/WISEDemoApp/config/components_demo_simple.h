/* Auto-generated es_platform_components.h */
#ifndef _COMPONENTS_DEFAULT_H
#define _COMPONENTS_DEFAULT_H

#define ES_DEVICE_UART                                  1
#define ES_DEVICE_PIO                                   1

#define DMA_CH0_FUNC                                    SYS_DMA_FUNC_UNSED
#define DMA_CH1_FUNC                                    SYS_DMA_FUNC_UNSED
#define DMA_CH2_FUNC                                    SYS_DMA_FUNC_UNSED
#define DMA_CH3_FUNC                                    SYS_DMA_FUNC_UNSED
#define DMA_CH4_FUNC                                    SYS_DMA_FUNC_UNSED
#define DMA_CH5_FUNC                                    SYS_DMA_FUNC_UNSED

#define MIDDLEWARE_WISE_SYSTEM                          1
#define MIDDLEWARE_WISE_CTRL_CMD                        0
#define MIDDLEWARE_WISE_FLASH_FILESYSTEM                0
#define MIDDLEWARE_RETARGET_STDIO                       1
#define STDIO_UART_PORT                                 0

#define CONFIG_HDL_PRINTF                               0
#define CONFIG_DRV_PRINTF                               0
#define CONFIG_INTF_PRINTF                              0
#define CONFIG_API_PRINTF                               0

#define MIDDLEWARE_WISE_SHELL                           0
#define MIDDLEWARE_WISE_SHELL_V2                        0

#define SHELL_RADIO                                     0
#define SHELL_TIMER                                     0
#define SHELL_RTC                                       0
#define SHELL_SPI                                       0
#define SHELL_I2C                                       0
#define SHELL_PWM                                       0
#define SHELL_PWMSLOW                                   0
#define SHELL_TRNG                                      0
#define SHELL_WDT                                       0
#define SHELL_GPIO                                      0
#define SHELL_PIO                                       0
#define SHELL_CRYPTO                                    0
#define SHELL_NFC                                       0
#define SHELL_SYS                                       0



#if MIDDLEWARE_WISE_CTRL_CMD
#define CTRL_CMD_UART_PORT                              0
#endif /* MIDDLEWARE_WISE_CTRL_CMD */

#define ES_COMP_ENABLE_UART_0                           1

/* Disable TX FIFO to use blocking mode - avoids FIFO-related issues */
#define ES_UART0_TX_BUF_SIZE                            0

#endif /* _COMPONENTS_DEFAULT_H */

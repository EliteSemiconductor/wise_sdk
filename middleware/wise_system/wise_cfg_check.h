#ifndef __WISE_CFG_CHECK_H 
#define __WISE_CFG_CHECK_H

#ifndef ES_UART0_RX_BUF_SIZE
#define ES_UART0_RX_BUF_SIZE                            0
#endif

#ifndef ES_UART1_RX_BUF_SIZE
#define ES_UART1_RX_BUF_SIZE                            0
#endif

#ifndef ES_UART2_RX_BUF_SIZE
#define ES_UART2_RX_BUF_SIZE                            0
#endif

#ifndef ES_UART0_TX_BUF_SIZE
#define ES_UART0_TX_BUF_SIZE                            0
#endif

#ifndef ES_UART1_TX_BUF_SIZE
#define ES_UART1_TX_BUF_SIZE                            0
#endif

#ifndef ES_UART2_TX_BUF_SIZE
#define ES_UART2_TX_BUF_SIZE                            1
#endif

#ifndef DMA_CH0_FUNC
#define DMA_CH0_FUNC                                    SYS_DMA_FUNC_UNSED
#endif

#ifndef DMA_CH1_FUNC
#define DMA_CH1_FUNC                                    SYS_DMA_FUNC_UNSED
#endif

#ifndef DMA_CH2_FUNC
#define DMA_CH2_FUNC                                    SYS_DMA_FUNC_UNSED
#endif

#ifndef DMA_CH3_FUNC
#define DMA_CH3_FUNC                                    SYS_DMA_FUNC_UNSED
#endif

#ifndef DMA_CH4_FUNC
#define DMA_CH4_FUNC                                    SYS_DMA_FUNC_UNSED
#endif

#ifndef DMA_CH5_FUNC
#define DMA_CH5_FUNC                                    SYS_DMA_FUNC_UNSED
#endif

#ifndef ES_DEVICE_PIO
#define ES_DEVICE_PIO                                   0
#endif

#ifndef ES_DEVICE_LFOSC_SELECT
#define ES_DEVICE_LFOSC_SELECT                          SYS_LFOSC_CLK_SRC_INTERNAL_16K
#define ES_DEVICE_LFOSC_OPTION                          LFOSC_16K_MODE_TEMP_COMP
#endif 

#ifndef BOARD_40M_GAIN_CTRL
#define BOARD_40M_GAIN_CTRL                             1
#endif

#ifndef BOARD_40M_GAIN_CTRL_S
#define BOARD_40M_GAIN_CTRL_S                           15
#endif

#ifndef BOARD_SRAM_RETAIN
#define BOARD_SRAM_RETAIN                               1
#endif

#if defined(BOARD_CONFIG_FILE)
#include BOARD_CONFIG_FILE
#else
#include "board_default_cfg.h"
#endif

#endif


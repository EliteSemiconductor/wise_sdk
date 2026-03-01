#ifndef __BOARD_DEFAULT_CFG_H 
#define __BOARD_DEFAULT_CFG_H

#define ES_BOARD_NAME								"ER8130 TC01 DBG"

#if (defined ES_COMP_ENABLE_UART_0) && (ES_COMP_ENABLE_UART_0 == 1)
#define ES_UART0_BAUDRATE                           115200
#define ES_UART0_TX_PIN                             0
#define ES_UART0_RX_PIN                             1
#define ES_UART0_RX_BUF_SIZE                        256
#endif

#define BOARD_TCXO_OUTPUT_EN                        0   //0: disable, 1:enable
#define BOARD_PA_TYPE                               0   //0: 10db, 1: 14db
#define BOARD_BAND_MATCHING                         0   //0: 915, 1: 868, 2: 490
#define BOARD_40M_GAIN_CTRL                         1   //1 ~ 8
#define BOARD_40M_CAP_XTAL_I                        16  //default = 64
#define BOARD_40M_CAP_XTAL_O                        16  //default = 64
#define BOARD_SRAM_RETAIN                           1   //0: 32k, 1: 64K

#endif

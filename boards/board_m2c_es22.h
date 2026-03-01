#ifndef __BOARD_DEFAULT_CFG_H 
#define __BOARD_DEFAULT_CFG_H

#define ES_BOARD_NAME									"ES22"

// UART0
#define ES_COMP_ENABLE_UART_0 							1
#define ES_UART0_BAUDRATE                               460800
#define ES_UART0_TX_PIN                                 10 //10
#define ES_UART0_RX_PIN                                 11 //11
#define ES_UART0_RX_BUF_SIZE                            256

// SPI0
#define WISE_SPI_0_SCLK_PIN                             2
#define WISE_SPI_0_CS_PIN                               3
#define WISE_SPI_0_MOSI_PIN                             1
#define WISE_SPI_0_BITRATE                              1000000

// Radio configuration
#define BOARD_TCXO_OUTPUT_EN                            0   //0: disable, 1:enable
#define BOARD_PA_TYPE                                   0   //0: 10db, 1: 14db
#define BOARD_BAND_MATCHING                             0   //0: 915, 1: 868, 2: 490
#define BOARD_40M_GAIN_CTRL                             3   //1 ~ 8
#define BOARD_40M_CAP_XTAL_I                            37  //default = 64
#define BOARD_40M_CAP_XTAL_O                            37  //default = 64

// EPD interface
#define EPD_SPI_INTF                                    0
#define EPD_POWER_PIN                                   9
#define EPD_RESET_PIN                                   13
#define EPD_BUSY_PIN                                    12
#define EPD_CMD_PIN                                     0
#define EPD_SPI_CS_PIN                                  WISE_SPI_0_CS_PIN
#define EPD_SPI_SCLK_PIN                                WISE_SPI_0_SCLK_PIN
#define EPD_SPI_SDA_PIN                                 WISE_SPI_0_MOSI_PIN


#endif

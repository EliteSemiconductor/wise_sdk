/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __ES_PLATFORM_COMPONENTS_H
#define __ES_PLATFOMR_COMPONENTS_H

/* ************************************************************************  */
/* COMPONRNTS CONFIGURATION                                                  */
/* ************************************************************************  */
//#define CONFIG_ER8130_TC04
/* ------------------------------------------------------------------------- */

/* ==== CORE COMPONRNTS SELECTIVE ========================================== */
#define ES_DEVICE_SYS 1
/* ------------------------------------------------------------------------- */
#define ES_DEVICE_PMU 0
/* ------------------------------------------------------------------------- */
#define ES_DEVICE_UART 1
/* ------------------------------------------------------------------------- */
#define ES_DEVICE_RADIO 0
/* ------------------------------------------------------------------------- */
#define ES_DEVICE_TIMER 0
/* ------------------------------------------------------------------------- */
#define ES_DEVICE_SPI 0
/* ------------------------------------------------------------------------- */
#define ES_DEVICE_PIO 1

/* ==== MIDDLEWARE COMPONRNTS SELECTIVE ==================================== */
/* ---- RETARGET ----------------------------------------------------------- */
#define MIDDLEWARE_RETARGET_STDIO 1
/* ---- WISE SHELL --------------------------------------------------------- */
#define MIDDLEWARE_WISE_SHELL 1
/* ---- WISE CTRL CMD ------------------------------------------------------ */
#define MIDDLEWARE_WISE_CTRL_CMD 1
/* --------------- WISE SYSTEM --------------------------------------------- */
#define MIDDLEWARE_WISE_SYSTEM 1

/* --------------- WISE FLASH FILE SYSTEM----------------------------------- */
#define MIDDLEWARE_WISE_FLASH_FILESYSTEM 1
/* --------------- WISE KERMIT --------------------------------------------- */
#define MIDDLEWARE_WISE_KERMIT 1

/* ************************************************************************  */
/* PARAMETER CONFIGURATION                                                   */
/* ************************************************************************  */

/* ==== UART PARAMETER CONFIGURATION ======================================= */
#if ES_DEVICE_UART
#define ES_COMP_ENABLE_UART_0 1
#define ES_UART0_BAUDRATE 115200
#define ES_UART0_TX_PIN 0
#define ES_UART0_RX_PIN 1
#define ES_UART0_RX_BUF_SIZE 256

/*
#define ES_COMP_ENABLE_UART_1               1
#define ES_UART1_BAUDRATE                   115200
#define ES_UART1_TX_PIN                     2
#define ES_UART1_RX_PIN                     3
#define ES_UART1_RX_BUF_SIZE                256
*/

//#define ES_COMP_ENABLE_UART_2               0
#endif /* ES_DEVICE_UART */
/* ==== RADIO PARAMETER CONFIGURATION ======================================= */
#if ES_DEVICE_RADIO
#define ES_DEVICE_RADIO_INTF_NUM 1 //2 for external
//#define ES_DEVICE_ENABLE_EXT_RADIO
#endif /* ES_DEVICE_RADIO */
/* ==== SYS PARAMETER CONFIGURATION ======================================= */
#if ES_DEVICE_SYS
#define ES_OSC_STABLE_TIME 2000000 // unit: us, recommend at least one seconds
#define ES_OSC_READJ_TIME 200000   // unit: us
#endif                             /* ES_DEVICE_SYS */
/* ==== PMU PARAMETER CONFIGURATION ======================================= */
#if ES_DEVICE_PMU
//TODO
#endif /* ES_DEVICE_PMU */
/* ==== TIMER PARAMETER CONFIGURATION ======================================= */
#if ES_DEVICE_TIMER
//TODO
#endif /* ES_DEVICE_TIMER */

/* ==== MIDDLEWARE RETARGET PARAMETER CONFIGURATION ======================== */
#if MIDDLEWARE_RETARGET_STDIO
#define STDIO_UART_PORT 0 // [0]:UART0 [1]:UART1 [2]:UART2
#endif                    /* MIDDLEWARE_RETARGET_STDIO */

/* ==== MIDDLEWARE SHELL PARAMETER CONFIGURATION =========================== */
#if MIDDLEWARE_WISE_SHELL
#define SHELL_EXTERNAL_CMD_TABLE 1
/* ------------------------------------------------------------------------- */
#define SHELL_FILESYSTEM 0
/* ------------------------------------------------------------------------- */
#define SHELL_RADIO 0
/* ------------------------------------------------------------------------- */
#define SHELL_TIMER 0
/* ------------------------------------------------------------------------- */
#define SHELL_SPI 0
/* ------------------------------------------------------------------------- */
#define SHELL_I2C 0
/* ------------------------------------------------------------------------- */
#define SHELL_TRNG 0
/* ------------------------------------------------------------------------- */
#define SHELL_XT 0
/* ------------------------------------------------------------------------- */
#define SHELL_USER 0
/* ------------------------------------------------------------------------- */
#endif /* MIDDLEWARE_RETARGET_STDIO */

#if MIDDLEWARE_WISE_CTRL_CMD
#define CTRL_CMD_UART_PORT 0
#endif /* MIDDLEWARE_WISE_CTRL_CMD */

/* ************************************************************************  */
/* PROTOCOL COMPONRNTS SELECTIVE                                             */
/* ************************************************************************  */
//TODO

/* ************************************************************************  */
/* APPLICATION COMPONRNTS SELECTIVE                                          */
/* ************************************************************************  */
//TODO
#endif /* __ES_PLATFORM_COMPONENTS_H */

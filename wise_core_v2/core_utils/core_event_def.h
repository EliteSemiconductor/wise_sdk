/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file core_event_def.h
 * @brief Definitions for core event categories and specific events.
 *
 * This header file provides macro definitions for core event categories and specific events related to different modules.
 * These definitions can be used to manage and handle events in the system.
 */

#ifndef _CORE_EVENT_DEF_H_
#define _CORE_EVENT_DEF_H_

// Event catagory
/**
 * @def CORE_EVT_MODULE_RADIO
 * @brief Event category for radio module events.
 */
#define CORE_EVT_MODULE_RADIO ((uint8_t)0x01)

/**
 * @def CORE_EVT_MODULE_NFC
 * @brief Event category for NFC module events.
 */
#define CORE_EVT_MODULE_NFC ((uint8_t)0x02)

/**
 * @def CORE_EVT_MODULE_WAKEUP
 * @brief Event category for wakeup module events.
 */
#define CORE_EVT_MODULE_WAKEUP ((uint8_t)0x03)

/**
 * @def CORE_EVT_MODULE_TIMER
 * @brief Event category for timer module events.
 */
#define CORE_EVT_MODULE_TIMER ((uint8_t)0x04)

// Radio events
/**
 * @def CORE_EVT_RADIO_TX
 * @brief Event for radio transmission.
 */
#define CORE_EVT_RADIO_TX BIT0

/**
 * @def CORE_EVT_RADIO_RX
 * @brief Event for radio reception.
 */
#define CORE_EVT_RADIO_RX BIT1

// kevinyang, 20240305, Radio events for V2
/**
 * @def CORE_EVT_RADIO_TX_FIN
 * @brief Event for completed radio transmission.
 */
#define CORE_EVT_RADIO_TX_FIN BIT0

/**
 * @def CORE_EVT_RADIO_TX_FAIL
 * @brief Event for failed radio transmission.
 */
#define CORE_EVT_RADIO_TX_FAIL BIT1

/**
 * @def CORE_EVT_RADIO_RX_FIN
 * @brief Event for completed radio reception.
 */
#define CORE_EVT_RADIO_RX_FIN BIT2

/**
 * @def CORE_EVT_RADIO_RX_FAIL
 * @brief Event for failed radio reception.
 */
#define CORE_EVT_RADIO_RX_FAIL BIT3

#endif /* _CORE_EVENT_DEF_H_ */

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_ctrl_cmd_def.h
 * @brief wise_ctrl_cmd_def.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */


#ifndef __WISE_CTRL_CMD_DEF_H
#define __WISE_CTRL_CMD_DEF_H

#define WISE_CMD_INVALID                                                0xff
#define WISE_CMD_SUCCESS                                                1
#define WISE_CMD_FAIL                                                   0

// command type define
#define WISE_CMD_TYPE_FU                                                0x00
#define WISE_CMD_TYPE_SYS                                               0x01
#define WISE_CMD_TYPE_FS                                                0x02
#define WISE_CMD_TYPE_OTA                                               0x03
#define WISE_CMD_TYPE_UR                                                0x04
#define WISE_CMD_TYPE_APP                                               0x05 // Application layer commands

#define WISE_CMD_TYPE_EVENT                                             0x70 // used for notification from device to PC

// sub command code for WISE_CMD_TYPE_FU
#define CMD_FU_START                                                    0x00
#define CMD_FU_ING                                                      0x01
#define CMD_FU_FINISH                                                   0x02

// sub command code for WISE_CMD_TYPE_SYS
#define CMD_SYS_ECHO                                                    0x00
#define CMD_SYS_REBOOT                                                  0x01
#define CMD_SYS_VER_INFO                                                0x02
#define CMD_SYS_REG_WRITE                                               0x03
#define CMD_SYS_REG_READ                                                0x04
#define CMD_SYS_MEM_ALLOC                                               0x05
#define CMD_SYS_MEM_WRITE                                               0x06
#define CMD_SYS_MEM_READ                                                0x07
#define CMD_SYS_SET_INTERRUPT                                           0x08

// sub command code for WISE_CMD_TYPE_FS
#define CMD_FS_INFO                                                     0x00
#define CMD_FS_FORMAT                                                   0x01
#define CMD_FS_PAR_INFO                                                 0x02
#define CMD_FS_PAR_ERASE                                                0x03
#define CMD_FS_PAR_UPDATE_START                                         0x04
#define CMD_FS_PAR_UPDATE_SEG                                           0x05
#define CMD_FS_PAR_UPDATE_FINISH                                        0x06

// sub command code for WISE_CMD_TYPE_UR
#define CMD_UR_LOAD                                                     0x00
#define CMD_UR_SAVE                                                     0x01

// event command code for WISE_CMD_TYPE_APP
#define CMD_APP_WMBUS_INIT                                              0x00
#define CMD_APP_WMBUS_SNIFFER                                           0x01

// event definition for WISE_CMD_TYPE_EVENT
#define EVT_SYSTEM_READY                                                0x00
#define EVT_INTERRUPT_STATUS                                            0x01
#define EVT_MAC_INTERRUPT                                               0x02


#endif

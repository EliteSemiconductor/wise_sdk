/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if MIDDLEWARE_WISE_CTRL_CMD
#include <stdlib.h>
#include <string.h>

#ifndef TARGET_ARDUINO
#include "wise.h"
#endif

#include "wise_ctrl_cmd.h"
#include "wise_ctrl_cmd_def.h"

#include "wise_core.h"

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
#include "wise_file_system.h"
#endif

#define DISABLE_IAP_CMD

// #define REG_W32(r, v)       (*((volatile unsigned long*)(r)) = (v))
// #define REG_R32(r)          (*((volatile unsigned long*)(r)))

typedef struct {
    uint8_t cmd;
    void (*cmdHandler)(uint8_t *cmdBuf, uint16_t len);
} CMD_ENTRY_T;

static uint8_t ctrlTmpBuf[PKT_MAX_PAYLOAD_LEN];

struct {
    uint32_t signature;
    int32_t parIndex;
    uint32_t totalLen;
    uint32_t finishLen;
    uint16_t dataCrc;
    uint16_t seqNo;
} stParUpdate = {0};

int32_t sendResponse(uint8_t cmdType, uint8_t subCmd, uint8_t *data,
                     uint16_t dataLen);
void sendRespNoData(uint8_t cmdType, uint8_t subCmd, uint8_t resp);

void fuCmdHandler(uint8_t *cmdBuf, uint16_t len);
void sysCmdHandler(uint8_t *cmdBuf, uint16_t len);
void fileSystemCmdHandler(uint8_t *cmdBuf, uint16_t len);
void otaCmdHandler(uint8_t *cmdBuf, uint16_t len);
void urCmdHandler(uint8_t *cmdBuf, uint16_t len);

void processSubCommand(uint8_t *cmdBuf, uint16_t len, CMD_ENTRY_T *cmdTable);

// fu command handler
void fuCmdStart(uint8_t *cmdBuf, uint16_t len);
void fuCmdData(uint8_t *cmdBuf, uint16_t len);
void fuCmdFinish(uint8_t *cmdBuf, uint16_t len);

// sys command handler
void sysCmdEcho(uint8_t *cmdBuf, uint16_t len);
void sysCmdReboot(uint8_t *cmdBuf, uint16_t len);
void sysCmdVerInfo(uint8_t *cmdBuf, uint16_t len);
void sysCmdRegWrite(uint8_t *cmdBuf, uint16_t len);
void sysCmdRegRead(uint8_t *cmdBuf, uint16_t len);
void sysCmdMemAlloc(uint8_t *cmdBuf, uint16_t len);
void sysCmdMemWrite(uint8_t *cmdBuf, uint16_t len);
void sysCmdMemRead(uint8_t *cmdBuf, uint16_t len);

// file system command handler
void fsCmdInfo(uint8_t *cmdBuf, uint16_t len);
void fsCmdFormat(uint8_t *cmdBuf, uint16_t len);
void fsCmdParInfo(uint8_t *cmdBuf, uint16_t len);
void fsCmdParErase(uint8_t *cmdBuf, uint16_t len);
void fsCmdParUpdateStart(uint8_t *cmdBuf, uint16_t len);
void fsCmdParUpdateSegment(uint8_t *cmdBuf, uint16_t len);
void fsCmdParUpdateFinish(uint8_t *cmdBuf, uint16_t len);

// user data command handler
void urCmdLoad(uint8_t *cmdBuf, uint16_t len);
void urCmdSave(uint8_t *cmdBuf, uint16_t len);

void resetParUpdate();
void cmdErrorDebug(uint8_t cmdType, uint8_t cmd, uint8_t *err);

CMD_ENTRY_T cmdTypeTable[] = {
#ifndef DISABLE_IAP_CMD
    {WISE_CMD_TYPE_FU, fuCmdHandler},
#endif
    {WISE_CMD_TYPE_SYS, sysCmdHandler},
    {WISE_CMD_TYPE_FS, fileSystemCmdHandler},
    {WISE_CMD_TYPE_OTA, otaCmdHandler},
    {WISE_CMD_TYPE_UR, urCmdHandler},

    {WISE_CMD_INVALID, NULL}, // table end
};

#ifndef DISABLE_IAP_CMD
CMD_ENTRY_T fuCmdTable[] = {
    {CMD_FU_START, fuCmdStart},
    {CMD_FU_ING, fuCmdData},
    {CMD_FU_FINISH, fuCmdFinish},

    {WISE_CMD_INVALID, NULL}, // table end
};
#endif

CMD_ENTRY_T sysCmdTable[] = {
    {CMD_SYS_ECHO, sysCmdEcho},
    {CMD_SYS_REBOOT, sysCmdReboot},
    {CMD_SYS_VER_INFO, sysCmdVerInfo},
    {CMD_SYS_REG_WRITE, sysCmdRegWrite},
    {CMD_SYS_REG_READ, sysCmdRegRead},
    {CMD_SYS_MEM_ALLOC, sysCmdMemAlloc},
    {CMD_SYS_MEM_WRITE, sysCmdMemWrite},
    {CMD_SYS_MEM_READ, sysCmdMemRead},

    {WISE_CMD_INVALID, NULL}, // table end
};

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
CMD_ENTRY_T fileSysCmdTable[] = {
    {CMD_FS_INFO, fsCmdInfo},
    {CMD_FS_FORMAT, fsCmdFormat},
    {CMD_FS_PAR_INFO, fsCmdParInfo},
    {CMD_FS_PAR_ERASE, fsCmdParErase},
    {CMD_FS_PAR_UPDATE_START, fsCmdParUpdateStart},
    {CMD_FS_PAR_UPDATE_SEG, fsCmdParUpdateSegment},
    {CMD_FS_PAR_UPDATE_FINISH, fsCmdParUpdateFinish},

    {WISE_CMD_INVALID, NULL}, // table end
};
#endif

CMD_ENTRY_T otaCmdTable[] = {
    {WISE_CMD_INVALID, NULL}, // table end
};

CMD_ENTRY_T urCmdTable[] = {
    {CMD_UR_LOAD, urCmdLoad},
    {CMD_UR_SAVE, urCmdSave},

    {WISE_CMD_INVALID, NULL}, // table end
};

int32_t wise_ctrl_cmd_init()
{
    return WISE_SUCCESS;
}

void cmdErrorDebug(uint8_t cmdType, uint8_t cmd, uint8_t *err)
{
    debug_print("%02x%02x %s\n", cmdType, cmd, err);
}

int32_t sendResponse(uint8_t cmdType, uint8_t subCmd, uint8_t *data,
                     uint16_t dataLen)
{
    uint32_t bufAddr = 0, bufLen = 0;
    uint8_t *cmd;

    wise_pkt_get_output_buf(&bufAddr, &bufLen);
    if (!bufAddr || !bufLen) {
        return WISE_FAIL;
    }

    if (dataLen > bufLen) {
        return WISE_FAIL;
    }

    cmd = (uint8_t *)bufAddr;

    cmd[0] = cmdType | WISE_FLAG_RESP;
    cmd[1] = subCmd;
    memcpy(&cmd[2], data, dataLen);

    wise_pkt_start_output(dataLen + 2, PKT_OUTPUT_BUFFERRED);

    return WISE_SUCCESS;
}

void sendRespNoData(uint8_t cmdType, uint8_t subCmd, uint8_t result)
{
    uint32_t bufAddr = 0, bufLen = 0;
    uint8_t *resp;

    wise_pkt_get_output_buf(&bufAddr, &bufLen);
    if (!bufAddr || !bufLen) {
        return;
    }

    resp = (uint8_t *)bufAddr;

    resp[0] = cmdType | WISE_FLAG_RESP;
    resp[1] = subCmd;
    resp[2] = result;

    wise_pkt_start_output(3, PKT_OUTPUT_BUFFERRED);
}

int32_t wise_ctrl_cmd_process(uint8_t *cmdBuf, uint16_t len)
{
    uint8_t cmdType = cmdBuf[0] & WISE_MASK_CMD_TYPE;
    uint8_t isResp  = cmdBuf[0] & WISE_FLAG_RESP ? 1 : 0;
    int32_t cmdIdx  = 0;

    if (isResp) {
        // todo: received response for some command
        return WISE_SUCCESS;
    }

    while (1) {
        if (cmdTypeTable[cmdIdx].cmd == WISE_CMD_INVALID) {
            debug_print("unknown command\n");
            dump_buffer(cmdBuf, len);
            break;
        }

        if (cmdTypeTable[cmdIdx].cmd == cmdType) {
            (cmdTypeTable[cmdIdx].cmdHandler)(cmdBuf, len);
            break;
        }

        cmdIdx++;
    }

    return WISE_SUCCESS;
}

void processSubCommand(uint8_t *cmdBuf, uint16_t len, CMD_ENTRY_T *cmdTable)
{
    int32_t cmdIdx = 0;
    uint8_t subCmd = cmdBuf[1];

    while (1) {
        if (cmdTable[cmdIdx].cmd == WISE_CMD_INVALID) {
            debug_print("unknown command\n");
            dump_buffer(cmdBuf, len);
            break;
        }

        if (cmdTable[cmdIdx].cmd == subCmd) {
            (cmdTable[cmdIdx].cmdHandler)(&cmdBuf[2], len - 2);
            break;
        }

        cmdIdx++;
    }
}

#ifndef DISABLE_IAP_CMD
void fuCmdHandler(uint8_t *cmdBuf, uint16_t len)
{
    processSubCommand(cmdBuf, len, fuCmdTable);
}
#endif

void sysCmdHandler(uint8_t *cmdBuf, uint16_t len)
{
    processSubCommand(cmdBuf, len, sysCmdTable);
}

void fileSystemCmdHandler(uint8_t *cmdBuf, uint16_t len)
{
#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
    processSubCommand(cmdBuf, len, fileSysCmdTable);
#endif
}

void otaCmdHandler(uint8_t *cmdBuf, uint16_t len)
{
    processSubCommand(cmdBuf, len, otaCmdTable);
}

void urCmdHandler(uint8_t *cmdBuf, uint16_t len)
{
    processSubCommand(cmdBuf, len, urCmdTable);
}

#ifndef DISABLE_IAP_CMD
// fu command handler
void fuCmdStart(uint8_t *cmdBuf, uint16_t len)
{
    const uint8_t MY_TYPE = WISE_CMD_TYPE_FU;
    const uint8_t MY_CMD  = CMD_FU_START;
    uint32_t sig          = FS_SIG_FUTEMP; // LOAD_BE_32(&cmdBuf[0]);
    uint32_t totalLen     = LOAD_BE_32(&cmdBuf[0]);
    uint16_t crc          = LOAD_BE_16(&cmdBuf[4]);
    int32_t parIndex      = WISE_INVALID_INDEX;
    PAR_INFO_T parInfo    = {0};

    if (len != 6) {
        goto resp_fail;
    }

    //    if(stParUpdate.signature != 0)
    //    {
    //        debug_print("%08x\n", stParUpdate.signature);
    //        cmdErrorDebug(MY_TYPE, MY_CMD, "f1");
    //    }

    resetParUpdate();

    wise_fs_reload();

    parIndex = wise_fs_par_sig_to_index(sig);

    if ((parIndex == WISE_INVALID_INDEX) || (parIndex <= 1)) {
        goto resp_fail;
    }

    wise_fs_get_par_info(sig, &parInfo);
    if (parInfo.dataLen != 0xffffffff) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f2");
        goto resp_fail;
    }

    stParUpdate.signature = sig;
    stParUpdate.parIndex  = parIndex;
    stParUpdate.totalLen  = totalLen;
    stParUpdate.finishLen = 0;
    stParUpdate.dataCrc   = crc;
    stParUpdate.seqNo     = 0;

    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);

    return;

resp_fail:
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
}

void fuCmdData(uint8_t *cmdBuf, uint16_t len)
{
    const uint8_t MY_TYPE = WISE_CMD_TYPE_FU;
    const uint8_t MY_CMD  = CMD_FU_ING;
    //	uint32_t secAddr = LOAD_BE_32(&cmdBuf[0]);
    uint16_t secLen = LOAD_BE_16(&cmdBuf[4]);
    //	uint8_t *pBuf = &cmdBuf[6];
    //    uint8_t secErase = cmdBuf[sizeof(secAddr) + sizeof(secLen) + secLen];

    // if(WISE_SUCCESS != wise_iap_write_sector(secAddr, pBuf, secErase))
    //     goto update_fail;

    stParUpdate.finishLen += secLen;

    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);

    return;

    // update_fail:
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
}

void fuCmdFinish(uint8_t *cmdBuf, uint16_t len)
{
    const uint8_t MY_TYPE = WISE_CMD_TYPE_FU;
    const uint8_t MY_CMD  = CMD_FU_FINISH;
    // uint16_t crcVerify = 0;

    if (stParUpdate.finishLen != stParUpdate.totalLen) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f1");
        goto resp_fail;
    }

    if (WISE_SUCCESS != wise_fs_par_update_info(stParUpdate.parIndex,
                                                stParUpdate.totalLen,
                                                stParUpdate.dataCrc)) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f2");
        goto resp_fail;
    }

    // debug_print("finished par update\n");

    resetParUpdate();

    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);

    //	debug_print("system reboot in 2 seconds\n");
    //    wise_request_reboot(2000);

    //	NVIC_SystemReset();

    return;

resp_fail:
    resetParUpdate();
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
}
#endif // DISABLE_IAP_CMD

// sys command handler
void sysCmdEcho(uint8_t *cmdBuf, uint16_t len)
{
    sendResponse(WISE_CMD_TYPE_SYS, CMD_SYS_ECHO, &cmdBuf[0], len);
}

void sysCmdReboot(uint8_t *cmdBuf, uint16_t len)
{
    // uint32_t counter;
    // uint32_t loopCount = 2;

    sendResponse(WISE_CMD_TYPE_SYS, CMD_SYS_REBOOT, &cmdBuf[0], len);
    
    //debug_print("system reboot in 2 seconds\n");
    wise_request_reboot(500);
}

void sysCmdVerInfo(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_SYS CMD_SYS_VER_INFO
    //
    //  RESP:
    //  |    0   |    1   |    2   |    3   |    4   |    5   |    6   |    7 |
    //  |--------|--------|--------|--------|--------|--------|--------|--------|
    //  |cmdResp |role    |verMajor|verMinor|
    //
    //  cmdResp     1 byte          0=OK, 1=Fail
    //  role        1 byte          0 for ROM, 1 for SBL, 2 for APP
    //  verMajor    1 byte          major version number
    //  verMinor    1 byte          minor version number

    const uint8_t MY_TYPE = WISE_CMD_TYPE_SYS;
    const uint8_t MY_CMD  = CMD_SYS_VER_INFO;

    memset(&ctrlTmpBuf[0], 0, PKT_MAX_PAYLOAD_LEN);

    ctrlTmpBuf[0] = WISE_CMD_SUCCESS;
#ifdef TARGET_ROM
    ctrlTmpBuf[1] = 0;
#elif defined TARGET_SBL
    ctrlTmpBuf[1] = 1;
#else
    ctrlTmpBuf[1] = 2;
#endif

    ctrlTmpBuf[2] = 1; // V1.00
    ctrlTmpBuf[3] = 0;

    sendResponse(MY_TYPE, MY_CMD, ctrlTmpBuf, 4);
}

void sysCmdRegWrite(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_SYS CMD_SYS_REG_WRITE
    //  |   0-3  |   4-7  |   8-11 |  12-15 |  ...   |  ...   |
    //  |--------|--------|--------|--------|--------|--------|
    //  |  addr0 | value0 |  addr1 | value1 |  ...   |  ...   |
    //
    //  addr-value  8 bytes         write value to assigned address, big endian
    //                              an address 0x00000000 means to delay value
    //                              micro seconds
    //
    //  RESP:
    //  |    0   |
    //  |--------|
    //  |cmdResp |
    //
    //  cmdResp     1 byte          0=fail, 1=success

    const uint8_t MY_TYPE = WISE_CMD_TYPE_SYS;
    const uint8_t MY_CMD  = CMD_SYS_REG_WRITE;
    uint8_t regNum        = len / 8;
    uint8_t *pData        = cmdBuf;

    while (regNum--) {
        unsigned int addr, value;

        addr  = LOAD_BE_32(&pData[0]);
        value = LOAD_BE_32(&pData[4]);

        if (addr != 0) {
            REG_W32(addr, value);
        } else {
            // wise_delay_us(value);
        }

        pData += 8;
    }

    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);
}

void sysCmdRegRead(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_SYS CMD_SYS_REG_READ
    //  |   0-3  |   4-7  |   8-11 |  12-15 |
    //  |--------|--------|--------|--------|
    //  |  addr0 | addr1  |  addr2 | ...    |
    //
    //  addr        4 bytes         register address to read, big endian
    //
    //  RESP:
    //  |    0   |   1-4  |   5-8  |  9-12  | ...    |
    //  |--------|--------|--------|--------|--------|
    //  |cmdResp | value1 | value2 | value3 | ...    |
    //
    //  cmdResp     1 byte          0=fail, 1=success
    //  value       4 bytes         values read from asigned registers

    const uint8_t MY_TYPE = WISE_CMD_TYPE_SYS;
    const uint8_t MY_CMD  = CMD_SYS_REG_READ;
    uint8_t regNum        = len / 4;
    uint8_t *pData        = cmdBuf;
    int i;

    memset(&ctrlTmpBuf[0], 0, PKT_MAX_PAYLOAD_LEN);

    ctrlTmpBuf[0] = WISE_CMD_SUCCESS;
    for (i = 0; i < regNum; i++) {
        unsigned int addr, value;

        addr  = LOAD_BE_32(&pData[0]);
        value = REG_R32(addr);

        PUT_BE_32(&ctrlTmpBuf[1 + i * 4], value);

        pData += 4;
    }

    sendResponse(MY_TYPE, MY_CMD, ctrlTmpBuf, (1 + regNum * 4));
}

void sysCmdMemAlloc(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_SYS CMD_SYS_MEM_ALLOC
    //  |   0-3  |
    //  |--------|
    //  |  size  |
    //
    //  size        4 bytes         size of the requested buffer
    //
    //  RESP:
    //  |    0   |   1-4  |
    //  |--------|--------|
    //  |cmdResp | addr   |
    //
    //  cmdResp     1 byte          0=fail, 1=success
    //  addr        4 bytes         address of allocated buffer

    const uint8_t MY_TYPE = WISE_CMD_TYPE_SYS;
    const uint8_t MY_CMD  = CMD_SYS_MEM_ALLOC;
    uint32_t size         = 0;
    uint8_t *bufPtr       = NULL;

    size = LOAD_BE_32(&cmdBuf[0]);
    if (size) {
        bufPtr = (uint8_t *)malloc(size);
    }

    ctrlTmpBuf[0] = WISE_CMD_SUCCESS;
    PUT_BE_32(&ctrlTmpBuf[1], (uint32_t)bufPtr);

    sendResponse(MY_TYPE, MY_CMD, ctrlTmpBuf, 5);
}

void sysCmdMemWrite(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_SYS CMD_SYS_MEM_WRITE
    //  |   0-3  |   4-7  |   8-N  |
    //  |--------|--------|--------|
    //  |  addr  | length |  data  |
    //
    //  addr        4 bytes         target address to write data
    //  length      4 bytes         length of data to write
    //  data        N bytes         data to write
    //
    //  RESP:
    //  |    0   |
    //  |--------|
    //  |cmdResp |
    //
    //  cmdResp     1 byte          0=fail, 1=success
    const uint8_t MY_TYPE = WISE_CMD_TYPE_SYS;
    const uint8_t MY_CMD  = CMD_SYS_MEM_WRITE;
    uint32_t addr, size;

    addr = LOAD_BE_32(&cmdBuf[0]);
    size = LOAD_BE_32(&cmdBuf[4]);

    if ((addr > 0) && (size > 0)) {
        memcpy((void *)addr, &cmdBuf[8], size);

        sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);
    } else {
        sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
    }
}

void sysCmdMemRead(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_SYS CMD_SYS_MEM_READ
    //  |   0-3  |   4-7  |
    //  |--------|--------|
    //  |  addr  | length |
    //
    //  addr        4 bytes         target address to write data
    //  length      4 bytes         length of data to write
    //
    //  RESP:
    //  |    0   |   N    |
    //  |--------|--------|
    //  |cmdResp |  data  |
    //
    //  cmdResp     1 byte          0=fail, 1=success
    const uint8_t MY_TYPE = WISE_CMD_TYPE_SYS;
    const uint8_t MY_CMD  = CMD_SYS_MEM_READ;
    uint32_t addr, size;

    addr = LOAD_BE_32(&cmdBuf[0]);
    size = LOAD_BE_32(&cmdBuf[4]);

    if ((addr > 0) && (size > 0)) {
        memset(&ctrlTmpBuf[0], 0, PKT_MAX_PAYLOAD_LEN);

        ctrlTmpBuf[0] = WISE_CMD_SUCCESS;

        memcpy(&ctrlTmpBuf[1], (void *)addr, size);

        sendResponse(MY_TYPE, MY_CMD, ctrlTmpBuf, size + 1);
    } else {
        sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
    }
}

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
static void respFSInfoResp(uint8_t cmdType, uint8_t cmdCode)
{
    WISE_FS_INFO_T fsInfo = {0};

    if (WISE_SUCCESS == wise_fs_get_fs_info(&fsInfo)) {
        int i;
        uint8_t *pTemp;

        memset(&ctrlTmpBuf[0], 0, PKT_MAX_PAYLOAD_LEN);

        ctrlTmpBuf[0] = WISE_CMD_SUCCESS;
        ctrlTmpBuf[1] = fsInfo.fsVersion;
        PUT_BE_32(&ctrlTmpBuf[2], fsInfo.storageSize);
        ctrlTmpBuf[6] = fsInfo.parNum;

        pTemp = &ctrlTmpBuf[7];
        for (i = 0; i < fsInfo.parNum; i++) {
            uint32_t dataLen = 0;

            PUT_BE_32(&pTemp[0], fsInfo.partitions[i].sig);
            PUT_BE_32(&pTemp[4], fsInfo.partitions[i].offset);
            PUT_BE_32(&pTemp[8], fsInfo.partitions[i].parLen);

            dataLen = fsInfo.partitions[i].dataLen;
            if (dataLen == 0xffffffff) {
                dataLen = 0;
            }
            PUT_BE_32(&pTemp[12], dataLen);

            pTemp += 16;
        }

        sendResponse(cmdType, cmdCode, ctrlTmpBuf, (pTemp - &ctrlTmpBuf[0]));
    } else {
        sendRespNoData(cmdType, cmdCode, WISE_CMD_FAIL);
    }
}

// file system command handler
void fsCmdInfo(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_FS CMD_FS_INFO
    //
    //  RESP:
    //  |    0   |    1   |    2   |    3   |    4   |    5   |    6   |    7 |
    //  |--------|--------|--------|--------|--------|--------|--------|--------|
    //  |cmdResp |fsVer   |storageSize                        |parNum
    //  |partitionInfo x N
    //
    //  cmdResp     1 byte          0=FS not found, 1=FS found
    //  fsVer       1 byte          version of current file system
    //  storageSize 4 bytes         total size of storage
    //  parNum      1 byte          number of partitions in flash
    //  partitionInfo  (parNum x 16) bytes
    //
    const uint8_t MY_TYPE = WISE_CMD_TYPE_FS;
    const uint8_t MY_CMD  = CMD_FS_INFO;
    // WISE_FS_INFO_T fsInfo = {0};

#if 0
    if(WISE_SUCCESS == wise_fs_get_fs_info(&fsInfo))
    {
        int i;
        uint8_t* pTemp;
        
        memset(&ctrlTmpBuf[0], 0, PKT_MAX_PAYLOAD_LEN);

        ctrlTmpBuf[0] = WISE_CMD_SUCCESS;
        ctrlTmpBuf[1] = fsInfo.fsVersion;
        PUT_BE_32(&ctrlTmpBuf[2], fsInfo.storageSize);
        ctrlTmpBuf[6] = fsInfo.parNum;

        pTemp = &ctrlTmpBuf[7];
        for(i = 0; i < fsInfo.parNum; i++)
        {
            uint32_t dataLen = 0;
            
            PUT_BE_32(&pTemp[0], fsInfo.partitions[i].sig);
            PUT_BE_32(&pTemp[4], fsInfo.partitions[i].offset);
            PUT_BE_32(&pTemp[8], fsInfo.partitions[i].parLen);

            dataLen = fsInfo.partitions[i].dataLen;
            if(dataLen == 0xffffffff)
                dataLen = 0;
            PUT_BE_32(&pTemp[12], dataLen);

            pTemp += 16;
        }

        sendResponse(MY_TYPE, MY_CMD, ctrlTmpBuf, (pTemp - &ctrlTmpBuf[0]));
    }
    else
    {
        sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
    }
#else
    respFSInfoResp(MY_TYPE, MY_CMD);
#endif
}

void fsCmdFormat(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_FS CMD_FS_FORMAT
    //  |    0   |   1-5  |   6-9  |  10-13 |  ...   |
    //  |--------|--------|--------|--------|--------|
    //  | parNum |   sig  | offset |  size  |  ...   |
    //
    //  parNum      1 byte          numbers of new partition table
    //  sig         4 bytes         signature of partition 0
    //  offset      4 bytes         offset of partition 0
    //  size        4 bytes         size of partition 0
    //  ...
    //  sig         4 bytes         signature of partition (parNum-1)
    //  offset      4 bytes         offset of partition (parNum-1)
    //  size        4 bytes         size of partition (parNum-1)
    //
    //  RESP:
    //  |    0   |    1   |    2   |    3   |    4   |    5   |    6   |    7 |
    //  |--------|--------|--------|--------|--------|--------|--------|--------|
    //  |cmdResp |fsVer   |storageSize                        |parNum
    //  |partitionInfo x N
    //
    //  cmdResp     1 byte          0=FS not found, 1=FS found
    //  fsVer       1 byte          version of current file system
    //  storageSize 4 bytes         total size of storage
    //  parNum      1 byte          number of partitions in flash
    //  partitionInfo  (parNum x 16) bytes
    //

    const uint8_t MY_TYPE          = WISE_CMD_TYPE_FS;
    const uint8_t MY_CMD           = CMD_FS_FORMAT;
    uint8_t parNum                 = 0;
    WISE_FS_PART_PARAM_T *parTable = (WISE_FS_PART_PARAM_T *)&ctrlTmpBuf[0];
    int i;
    uint8_t *pData = NULL;

    parNum = cmdBuf[0];
    if (parNum > FS_MAX_PAR_NUM) {
        goto resp_fail;
    }

    debug_print("New table: %d partitions\n", parNum);

    pData = &cmdBuf[1];
    for (i = 0; i < parNum; i++) {
        parTable[i].sig     = LOAD_BE_32(pData);
        pData              += 4;
        parTable[i].offset  = LOAD_BE_32(pData);
        pData              += 4;
        parTable[i].size    = LOAD_BE_32(pData);
        pData              += 4;
    }

    if (WISE_FAIL == wise_fs_create(parNum, parTable)) {
        goto resp_fail;
    }

    // sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);
    respFSInfoResp(MY_TYPE, MY_CMD);

    return;

resp_fail:
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
}

void fsCmdParInfo(uint8_t *cmdBuf, uint16_t len)
{
}

void fsCmdParErase(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_FS CMD_FS_PAR_ERASE
    //  |    0   |    1   |    2   |    3   |    4   |
    //  |--------|--------|--------|--------|--------|
    //  |parSel  |parIndex/signature
    //
    //  parSel      1 byte          0=select by index, 1=select by signature
    //  select      1 or 4 bytes    partition index(1byte) or partition
    //  signature(4bytes)
    //
    //  RESP
    //  |    0   |
    //  |--------|
    //  |cmdResp |
    //
    //  cmdResp     1 byte          0=fail, 1=success
    //
    const uint8_t MY_TYPE = WISE_CMD_TYPE_FS;
    const uint8_t MY_CMD  = CMD_FS_PAR_ERASE;
    int32_t parIndex      = WISE_INVALID_INDEX;

    if (cmdBuf[0] == 0) {
        parIndex = cmdBuf[1];
    } else if (cmdBuf[0] == 1) {
        parIndex = wise_fs_par_sig_to_index(LOAD_BE_32(&cmdBuf[1]));
    } else {
        goto resp_fail;
    }

    if (parIndex == WISE_INVALID_INDEX) {
        goto resp_fail;
    }

#ifdef TARGET_SBL
    if (parIndex >= 1)
#else
    if (parIndex > 1)
#endif
    {
        debug_print("erasing partition %ld\n", parIndex);

        wise_fs_par_erase(parIndex);
        sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);
    } else {
        debug_print("forbidden access to partition %ld\n", parIndex);
        goto resp_fail;
    }

    return;

resp_fail:
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
}

void resetParUpdate()
{
    memset(&stParUpdate, 0, sizeof(stParUpdate));
    stParUpdate.parIndex = -1;
}

void fsCmdParUpdateStart(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_FS CMD_FS_PAR_UPDATE_START
    //  |      0 ~ 3      |      4 ~ 7      |      8 ~ 9      |
    //  |--------|--------|--------|--------|--------|--------|
    //  | signature       | totalLen        | crc             |
    //
    //  signature       4 bytes         signature of target partition, big
    //  endian totalLen        4 bytes         total data length to write crc 2
    //  bytes         CRC16 CITT of data to be transmited
    //
    //  RESP
    //  |    0   |
    //  |--------|
    //  |cmdResp |
    //
    //  cmdResp     1 byte          0=fail, 1=success
    //
    const uint8_t MY_TYPE = WISE_CMD_TYPE_FS;
    const uint8_t MY_CMD  = CMD_FS_PAR_UPDATE_START;
    uint32_t sig          = LOAD_BE_32(&cmdBuf[0]);
    uint32_t totalLen     = LOAD_BE_32(&cmdBuf[4]);
    uint16_t crc          = LOAD_BE_16(&cmdBuf[8]);
    int32_t parIndex      = WISE_INVALID_INDEX;
    PAR_INFO_T parInfo    = {0};

    if (len != 10) {
        goto resp_fail;
    }

    if (stParUpdate.signature != 0) {
        debug_print("%08lx\n", stParUpdate.signature);
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f1");
    }

    resetParUpdate();

    wise_fs_reload();

    parIndex = wise_fs_par_sig_to_index(sig);

#ifdef TARGET_SBL
    if ((parIndex == WISE_INVALID_INDEX) || (parIndex < 1)) {
        goto resp_fail;
    }
#elif defined TARGET_APP
    if ((parIndex == WISE_INVALID_INDEX) || (parIndex <= 1)) {
        goto resp_fail;
    }
#else
#error "undefined ER8xxx plaform"
#endif

    wise_fs_get_par_info(sig, &parInfo);
    if (parInfo.dataLen != 0xffffffff) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f2");
        goto resp_fail;
    }

    stParUpdate.signature = sig;
    stParUpdate.parIndex  = parIndex;
    stParUpdate.totalLen  = totalLen;
    stParUpdate.dataCrc   = crc;
    stParUpdate.seqNo     = 0;

    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);

    return;

resp_fail:
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
}

void fsCmdParUpdateSegment(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_FS CMD_FS_PAR_UPDATE_SEG
    //  |      0 ~ 1      |      2 ~ 3      |     4 ~ (N+4)
    //  |--------|--------|--------|--------|----------------------|
    //  | seqNo           | segLen          | segment data         |
    //
    //  seqNo           2 bytes         sequence number of the segment, start
    //  from 0 segLen          2 bytes         data length of the segment
    //  segment data    N bytes
    //
    //  RESP
    //  |    0   |
    //  |--------|
    //  |cmdResp |
    //
    //  cmdResp     1 byte          0=fail, 1=success
    //
    const uint8_t MY_TYPE = WISE_CMD_TYPE_FS;
    const uint8_t MY_CMD  = CMD_FS_PAR_UPDATE_SEG;
    uint16_t seqNo        = LOAD_BE_16(&cmdBuf[0]);
    // uint16_t segLen = LOAD_BE_16(&cmdBuf[2]);
    uint16_t dataLen = len - 4;

    if (len <= 4) {
        goto resp_fail;
    }

    if ((stParUpdate.parIndex == -1) || (stParUpdate.signature == 0)) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f1");
        goto resp_fail;
    }

    if (seqNo != stParUpdate.seqNo) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f2");
        goto resp_fail;
    }

    // debug_print("w seq=%d len=%d\n", seqNo, segLen);

    if (WISE_SUCCESS != wise_fs_par_write_data(stParUpdate.parIndex,
                                               stParUpdate.finishLen, dataLen,
                                               &cmdBuf[4])) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f3");
        goto resp_fail;
    }

    stParUpdate.seqNo++;
    stParUpdate.finishLen += dataLen;

    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);
    return;

resp_fail:
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
}

void fsCmdParUpdateFinish(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_FS CMD_FS_PAR_UPDATE_FINISH
    //
    //  RESP
    //  |    0   |
    //  |--------|
    //  |cmdResp |
    //
    //  cmdResp     1 byte          0=fail, 1=success
    //
    const uint8_t MY_TYPE = WISE_CMD_TYPE_FS;
    const uint8_t MY_CMD  = CMD_FS_PAR_UPDATE_FINISH;
    // uint16_t crcVerify = 0;

    if (stParUpdate.finishLen != stParUpdate.totalLen) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f1");
        goto resp_fail;
    }

    if (WISE_SUCCESS != wise_fs_par_update_info(stParUpdate.parIndex,
                                                stParUpdate.totalLen,
                                                stParUpdate.dataCrc)) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f2");
        goto resp_fail;
    }

    // debug_print("finished par update\n");

    resetParUpdate();

    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);
    return;

resp_fail:
    resetParUpdate();
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
}
#endif //MIDDLEWARE_WISE_FLASH_FILESYSTEM

// User data command handler
void urCmdLoad(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_UR CMD_UR_LOAD
    //
    memset(&ctrlTmpBuf[0], 0, PKT_MAX_PAYLOAD_LEN);

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
    const uint8_t MY_TYPE = WISE_CMD_TYPE_UR;
    const uint8_t MY_CMD  = CMD_UR_LOAD;
    uint32_t addr         = LOAD_BE_32(&cmdBuf[0]);
    uint16_t cfg_len      = LOAD_BE_16(&cmdBuf[4]);

    ctrlTmpBuf[0] = WISE_CMD_SUCCESS;
    if (WISE_SUCCESS !=
        wise_fs_user_data_read(addr, cfg_len, (uint8_t *)&ctrlTmpBuf[1])) {
        goto resp_fail;
    }

    sendResponse(MY_TYPE, MY_CMD, ctrlTmpBuf, cfg_len);

    resp_fail:
        sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);
#endif

    return;


}

void urCmdSave(uint8_t *cmdBuf, uint16_t len)
{
    //  CMD: WISE_CMD_TYPE_UR CMD_UR_SAVE
    //

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
    const uint8_t MY_TYPE = WISE_CMD_TYPE_UR;
    const uint8_t MY_CMD  = CMD_UR_SAVE;
    uint32_t addr         = LOAD_BE_32(&cmdBuf[0]);
    uint16_t cfg_len      = LOAD_BE_16(&cmdBuf[4]);

    wise_fs_reload();

    if (WISE_SUCCESS != wise_fs_user_data_update(addr, cfg_len, &cmdBuf[6])) {
        cmdErrorDebug(MY_TYPE, MY_CMD, (uint8_t *)"f1");
        goto resp_fail;
    }

    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_SUCCESS);

    wise_request_reboot(1000);

resp_fail:
    sendRespNoData(MY_TYPE, MY_CMD, WISE_CMD_FAIL);

#endif

    return;
}
#endif /* MIDDLEWARE_WISE_CTRL_CMD */

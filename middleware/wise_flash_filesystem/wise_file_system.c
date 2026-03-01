/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
#include <stdio.h>
#include <stdlib.h>

// #include "er8xxx.h"
#include "wise_core.h"
#include "wise_flash_api.h"
#ifndef TARGET_ARDUINO
#include "wise.h"
#endif
#include "wise_file_system.h"
#include "util_crc.h"

#define PATCH_HDR_LEN                       12
#define FS_CRC16_SEED                       0xFFFF
#define FS_CRC_POLY                         CRC_TYPE_16_KERMIT
#define PAR_HDR_OFFSET(offset, length)      (offset + length - fsInfo.flashInfo.minEraseSize)

typedef struct {
    WISE_FLASH_INFO_T flashInfo;
    uint32_t storageSize;
    uint32_t fsHdrOffset;
    uint32_t fsSig;

    uint32_t patchOffset;
    uint32_t patchSig;
    uint32_t patchSize;
    uint32_t patchCrc;

    uint8_t version;
    uint8_t parNum;
    uint8_t dataLen;
    uint8_t attribute;
    uint16_t crc;
    uint16_t padding;

    PAR_INFO_T parInfo[FS_MAX_PAR_NUM];

    int8_t userParIdx;
} WISE_FS_T;

static uint8_t fsHdrBuf[FS_BUF_SIZE];
//static uint8_t fsTmpBuf[WISE_FLASH_SECTOR_SIZE];
static uint8_t* fsTmpBuf = NULL;
static WISE_FS_T fsInfo;
static uint8_t parEntryNum = 0;
static WISE_FS_PART_PARAM_T parEntries[FS_MAX_PAR_NUM];
static uint8_t* fsCrcTable = NULL;

// offset and size should be aligned to block size
static const WISE_FS_PART_PARAM_T defPartitions[4] = {
//   signature      offset      size
    {FS_SIG_SBL,	0,			0x10000},
    {FS_SIG_APP,	0x10000, 	0x28000},
    {FS_SIG_FUTEMP, 0x38000, 	0x28000},
    {FS_SIG_USER,	0x60000, 	0x1B0000},
};

static const WISE_FS_PART_PARAM_T defPartitions_512[4] = {
//   signature      offset      size
    {FS_SIG_SBL,	0,			0x10000},
    {FS_SIG_APP,	0x10000, 	0x30000},
    {FS_SIG_FUTEMP, 0x40000, 	0x30000},
    {FS_SIG_USER,	0x70000, 	0x1B0000},
};


static int32_t loadFSTable();
static int32_t loadParInfo(uint32_t offset, uint32_t size, PAR_INFO_T *parInfo);
static int32_t writeParInfo(PAR_INFO_T *parInfo);
static int32_t searchPartition(uint32_t sig);
static uint16_t calcPartitionCRC(uint32_t offset, uint32_t size);
static void partitionErase(uint32_t sig, uint32_t offset, uint32_t size, uint8_t eraseAll);
static int32_t checkCustomParTable(uint8_t parNum, WISE_FS_PART_PARAM_T *parTable);
static int32_t checkPatchArea();
static uint16_t fsCalcCrc16(uint8_t *buf, int length, uint16_t startValue);

int32_t wise_fs_init()
{
    int i, idx_usr = 0, total_size = 0;
    const WISE_FS_PART_PARAM_T* defTable = NULL;
    
    memset(&fsHdrBuf[0], 0, sizeof(fsHdrBuf));
    memset(&fsInfo, 0, sizeof(WISE_FS_T));

    wise_flash_get_info(&fsInfo.flashInfo);
    
    if(!fsCrcTable)
    {
        fsCrcTable = malloc(CRC_16_TABLE_SIZE);
        if(!fsCrcTable)
        {
            debug_print("fs failed to allocate buffer\n");
            return WISE_FAIL;
        }

        memset(fsCrcTable, 0, CRC_16_TABLE_SIZE);
        util_crc16_gen_table(FS_CRC_POLY, (uint16_t*)fsCrcTable);
    }
    
    //create default partition table
    memset(&parEntries[0], 0, sizeof(parEntries));
    
    if(fsInfo.flashInfo.flashSize <= 0x40000)
    {
        parEntryNum = sizeof(defPartitions) / sizeof(WISE_FS_PART_PARAM_T);
        defTable = defPartitions;
    }
    else
    {
        parEntryNum = sizeof(defPartitions_512) / sizeof(WISE_FS_PART_PARAM_T);
        defTable = defPartitions_512;
    }
    
    
    for (i = 0; i < parEntryNum; i++) {
        memcpy(&parEntries[i], &defTable[i], sizeof(WISE_FS_PART_PARAM_T));
        if(parEntries[i].sig == FS_SIG_USER) {
        	idx_usr = i;
       	}
        total_size += parEntries[i].size;
    }

    if(total_size >= fsInfo.flashInfo.flashSize) {
    	parEntries[idx_usr].size -= (total_size - fsInfo.flashInfo.flashSize + fsInfo.flashInfo.minEraseSize * 2);	//1 partition hdr + 1 gc zones
    }
    fsInfo.storageSize = fsInfo.flashInfo.flashSize - fsInfo.flashInfo.minEraseSize; // should get from platform driver
    fsInfo.fsHdrOffset = fsInfo.flashInfo.flashSize - fsInfo.flashInfo.minEraseSize;
    fsInfo.userParIdx = WISE_INVALID_INDEX;
    fsInfo.patchOffset = fsInfo.fsHdrOffset - fsInfo.flashInfo.minEraseSize;

    fsTmpBuf = malloc(fsInfo.flashInfo.minEraseSize);
    if(!fsTmpBuf)
    {
        debug_print("Failed to allocate FS tmp buffer, require size %08lx\n", fsInfo.flashInfo.minEraseSize);
        return WISE_FAIL;
    }
    memset(&fsTmpBuf[0], 0, fsInfo.flashInfo.minEraseSize);

    loadFSTable();
    checkPatchArea();

    return WISE_SUCCESS;
}

static int32_t loadParInfo(uint32_t offset, uint32_t size, PAR_INFO_T *parInfo)
{
    uint8_t parHdr[PAR_HDR_LEN];

    wise_flash_read(PAR_HDR_OFFSET(offset, size), &parHdr[0], PAR_HDR_LEN);

    parInfo->sig     = LOAD_BE_32(&parHdr[PAR_HDR_SIG_OFFSET]);
    parInfo->dataLen = LOAD_BE_32(&parHdr[PAR_HDR_LEN_OFFSET]);
    parInfo->crc     = LOAD_BE_16(&parHdr[PAR_HDR_CRC_OFFSET]);
    parInfo->attr    = parHdr[PAR_HDR_ATTR_OFFSET];

    return WISE_SUCCESS;
}

static int32_t writeParInfo(PAR_INFO_T *parInfo)
{
    uint8_t *parHdr;
    uint32_t infoOffset = PAR_HDR_OFFSET(parInfo->offset, parInfo->parLen);

    memset(&fsTmpBuf[0], 0xff, PAR_HDR_LEN);
    parHdr = &fsTmpBuf[0];

    PUT_BE_32(&parHdr[PAR_HDR_SIG_OFFSET], parInfo->sig);
    PUT_BE_32(&parHdr[PAR_HDR_LEN_OFFSET], parInfo->dataLen);
    PUT_BE_16(&parHdr[PAR_HDR_CRC_OFFSET], parInfo->crc);
    parHdr[PAR_HDR_ATTR_OFFSET] = parInfo->attr;

    wise_flash_sector_erase(infoOffset);
    wise_flash_write(infoOffset, &fsTmpBuf[0], PAR_HDR_LEN);

    return WISE_SUCCESS;
}

static int32_t searchPartition(uint32_t sig)
{
    int i;
    int parIndex = WISE_INVALID_INDEX;

    if (fsInfo.parNum == 0) {
        goto finish;
    }

    for (i = 0; i < fsInfo.parNum; i++) {
        if (sig == fsInfo.parInfo[i].sig) {
            if (fsInfo.parInfo[i].parLen == 0) {
                debug_print("BUG!! invalid partition size %d\n", i);
            } else {
                parIndex = i;
            }

            break;
        }
    }

finish:
    return parIndex;
}

static uint16_t calcPartitionCRC(uint32_t offset, uint32_t size)
{
    uint16_t crcCalc = FS_CRC16_SEED;
    uint32_t segLen  = 0;

    while (size) {
        segLen = size > FS_BUF_SIZE ? FS_BUF_SIZE : size;

        wise_flash_read(offset, &fsTmpBuf[0], segLen);
        crcCalc = fsCalcCrc16(&fsTmpBuf[0], segLen, crcCalc);

        size   -= segLen;
        offset += segLen;
    }

    return crcCalc;
}

int32_t loadFSTable()
{
    uint32_t tableSig = 0;
    int i;

    if (fsInfo.storageSize == 0) {
        return WISE_FAIL;
    }

    wise_flash_read(fsInfo.fsHdrOffset, &fsHdrBuf[0], FS_BUF_SIZE);

    tableSig = LOAD_BE_32(&fsHdrBuf[FS_HDR_SIG_OFFSET]);
    if (tableSig == FS_SIG_FS) {
        uint8_t dataLen  = fsHdrBuf[FS_HDR_LEN_OFFSET];
        uint16_t crc     = LOAD_BE_16(&fsHdrBuf[FS_HDR_CRC_OFFSET]);
        uint16_t crcCalc = 0;

        crcCalc = fsCalcCrc16(&fsHdrBuf[FS_HDR_TABLE_OFFSET], dataLen, FS_CRC16_SEED);
        if (crc != crcCalc) {
            debug_print("FS table crc error\n");
            return WISE_FAIL;
        }

        fsInfo.fsSig     = tableSig;
        fsInfo.version   = fsHdrBuf[FS_HDR_VER_OFFSET];
        fsInfo.parNum    = fsHdrBuf[FS_HDR_NUM_OFFSET];
        fsInfo.dataLen   = fsHdrBuf[FS_HDR_LEN_OFFSET];
        fsInfo.attribute = fsHdrBuf[FS_HDR_ATTR_OFFSET];
        fsInfo.crc       = crc;

        for (i = 0; i < fsInfo.parNum; i++) {
            PAR_INFO_T *par = &fsInfo.parInfo[i];

            par->offset = LOAD_BE_32(&fsHdrBuf[FS_HDR_TABLE_OFFSET + i * PAR_ENTRY_SIZE]);
            par->parLen = LOAD_BE_32(&fsHdrBuf[FS_HDR_TABLE_OFFSET + i * PAR_ENTRY_SIZE + 4]);

            loadParInfo(par->offset, par->parLen, par);

            if (par->sig == FS_SIG_USER) {
                fsInfo.userParIdx = i;
            }
        }
    } else {
        debug_print("Partition table not found (%08lx)\n", tableSig);
        return WISE_FAIL;
    }

    return WISE_SUCCESS;
}

static void partitionErase(uint32_t sig, uint32_t offset, uint32_t size,
                           uint8_t eraseAll)
{
    uint32_t infoOffset = 0;

    memset(&fsTmpBuf[0], 0xff, PAR_HDR_LEN);

    PUT_BE_32(&fsTmpBuf[PAR_HDR_SIG_OFFSET], sig);
    infoOffset = PAR_HDR_OFFSET(offset, size);

#ifdef TARGET_SBL
    if (sig != FS_SIG_SBL)
#else // TARGET_APP
    if ((sig != FS_SIG_SBL) && (sig != FS_SIG_APP))
#endif
    {
        if (eraseAll) {
        	uint32_t end = offset + size;
        	uint32_t addr = offset;

        	while ((addr < end) && (addr % fsInfo.flashInfo.blockSize != 0)) {
        	    wise_flash_sector_erase(addr);
        	    addr += fsInfo.flashInfo.minEraseSize;
        	}

        	while ((addr + fsInfo.flashInfo.blockSize <= end)) {
        	    wise_flash_block_erase(addr);
        	    addr += fsInfo.flashInfo.blockSize;
        	}

        	while (addr < end) {
        	    wise_flash_sector_erase(addr);
        	    addr += fsInfo.flashInfo.minEraseSize;
        	}

        } else {
            wise_flash_sector_erase(infoOffset);
        }

        wise_flash_write(infoOffset, &fsTmpBuf[0], PAR_HDR_LEN);
    }
#ifdef TARGET_SBL
    else {
        wise_flash_sector_erase(infoOffset);
        wise_flash_write(infoOffset, &fsTmpBuf[0], PAR_HDR_LEN);
    }
#endif
}

int32_t checkCustomParTable(uint8_t parNum, WISE_FS_PART_PARAM_T *parTable)
{
    int32_t result = WISE_SUCCESS;
    int i;
    uint32_t SECTOR_MASK   = fsInfo.flashInfo.minEraseSize - 1;
    PAR_INFO_T *curAppPar = (PAR_INFO_T *)&fsInfo.parInfo[1];

    if (parNum < 2) {
        return WISE_FAIL;
    }

    for (i = 0; i < parNum; i++) {
        WISE_FS_PART_PARAM_T *par = &parTable[i];

        if (i == 0) // should be SBL
        {
            if ((par->sig != FS_SIG_SBL) || (par->offset & SECTOR_MASK) || (par->size & SECTOR_MASK)) {
                debug_print("Unsupported SBL partition\n");
                return WISE_FAIL;
            }
        } else if (i == 1) // should be APP
        {
            if ((par->sig != FS_SIG_APP) || (par->offset & SECTOR_MASK)) {
                debug_print("Unsupported APP partition\n");
                return WISE_FAIL;
            }

            if (curAppPar->dataLen) // APP partition already in use, allows to be extended.
            {
                if ((par->size & SECTOR_MASK) || (par->size < curAppPar->parLen)) {
                	debug_print("new APP partition size should be larger than old size %ld\n", curAppPar->parLen);
                    return WISE_FAIL;
                }
            }
        } else {
            if ((par->offset & SECTOR_MASK) || (par->size & SECTOR_MASK)) {
                debug_print("partition not aligned to SECTOR boundary\n");
                return WISE_FAIL;
            }

            if (par->sig == FS_SIG_FUTEMP) {
                if (par->size != parTable[1].size) {
                	debug_print("Partition FUTEMP should be equal to the APP partition\n");
                    return WISE_FAIL;
                }
            }
        }
    }

    return result;
}

int32_t wise_fs_create_default()
{
    return wise_fs_create(0, NULL);
}

int32_t wise_fs_create(uint8_t parNum, WISE_FS_PART_PARAM_T *parTable)
{
    int i;
    uint8_t dataLen = 0;
    uint16_t crc    = 0;

    if (parNum && parTable) {
        if (WISE_FAIL == checkCustomParTable(parNum, parTable)) {
            return WISE_FAIL;
        }

        debug_print("custom partion table parNum=%d\n", parNum);
        for (i = 0; i < parNum; i++) {
            parEntries[i].sig    = parTable[i].sig;
            parEntries[i].offset = parTable[i].offset;
            parEntries[i].size   = parTable[i].size;

            debug_print("   %d sig=%08lx offset=%08lx size=%08lx\n", 
                            i,
                            parEntries[i].sig, 
                            parEntries[i].offset,
                            parEntries[i].size);
        }

        parEntryNum = parNum;
    }

    debug_print("Create %d partitions...\n", parEntryNum);

    memset(&fsHdrBuf[0], 0xff, sizeof(fsHdrBuf));

    dataLen = PAR_ENTRY_SIZE * parEntryNum;

    PUT_BE_32(&fsHdrBuf[FS_HDR_SIG_OFFSET], FS_SIG_FS);
    fsHdrBuf[FS_HDR_LEN_OFFSET]  = dataLen;
    fsHdrBuf[FS_HDR_NUM_OFFSET]  = parEntryNum;
    fsHdrBuf[FS_HDR_VER_OFFSET]  = FS_VERSION;
    fsHdrBuf[FS_HDR_ATTR_OFFSET] = 0xff; // leave erase state

    for (i = 0; i < parEntryNum; i++) {
        PUT_BE_32(&fsHdrBuf[FS_HDR_TABLE_OFFSET + i * PAR_ENTRY_SIZE], parEntries[i].offset);
        PUT_BE_32(&fsHdrBuf[FS_HDR_TABLE_OFFSET + i * PAR_ENTRY_SIZE + 4], parEntries[i].size);
    }

    crc = fsCalcCrc16(&fsHdrBuf[FS_HDR_TABLE_OFFSET], dataLen, FS_CRC16_SEED);
    PUT_BE_16(&fsHdrBuf[FS_HDR_CRC_OFFSET], crc);

    wise_flash_sector_erase(fsInfo.fsHdrOffset);
    wise_flash_write(fsInfo.fsHdrOffset, &fsHdrBuf[0], sizeof(fsHdrBuf));

    debug_print("Finish\n");

    debug_print("Initialize partions...\n");
    for (i = 0; i < parEntryNum; i++) {
        debug_print("Par %d %08lx-%08lx\n", i, parEntries[i].offset, parEntries[i].size);

#ifdef TARGET_SBL
        if (parEntries[i].sig == FS_SIG_SBL) {
            partitionErase(parEntries[i].sig, parEntries[i].offset, parEntries[i].size, 0);
        } else {
            partitionErase(parEntries[i].sig, parEntries[i].offset, parEntries[i].size, 1);
        }

#else // TARGET_APP
        if ((parEntries[i].sig != FS_SIG_SBL) &&
            (parEntries[i].sig != FS_SIG_APP)) {
            partitionErase(parEntries[i].sig, parEntries[i].offset, parEntries[i].size, 1);
        } else if (parEntries[i].sig == FS_SIG_APP) {
            PAR_INFO_T *appPar = &fsInfo.parInfo[i];

            if (appPar->parLen < parEntries[i].size) {
                uint32_t infoOffset = 0;

                debug_print("Exntend APP partition to %08lx\n", parEntries[i].size);

                infoOffset = PAR_HDR_OFFSET(appPar->offset, appPar->parLen);
                debug_print("erasing orig partition header@%08lx\n", infoOffset);
                
                wise_flash_sector_erase(infoOffset);

                appPar->parLen = parEntries[i].size;
                writeParInfo(appPar);
            }
        }
#endif
    }

    loadFSTable();

    return WISE_SUCCESS;
}

int32_t wise_fs_check_par_valid(uint32_t sig)
{
    int32_t parIndex = WISE_INVALID_INDEX;
    int32_t result   = WISE_FAIL;

    parIndex = searchPartition(sig);
    if (parIndex != WISE_INVALID_INDEX) {
        PAR_INFO_T *partition = &fsInfo.parInfo[parIndex];

        if ((partition->dataLen != 0) && (partition->dataLen != 0xffffffff)) {
            uint16_t crcCalc = calcPartitionCRC(partition->offset, partition->dataLen);

            if (crcCalc == partition->crc) {
                result = WISE_SUCCESS;
            }
        }
    }

    return result;
}

int32_t wise_fs_get_par_info(uint32_t sig, PAR_INFO_T *parInfo)
{
    int32_t parIndex = WISE_INVALID_INDEX;

    if (parInfo) {
        parIndex = searchPartition(sig);
        if (parIndex != WISE_INVALID_INDEX) {
            memcpy(parInfo, &fsInfo.parInfo[parIndex], sizeof(PAR_INFO_T));
        }
    }

    return parIndex;
}

int32_t wise_fs_par_copy(uint32_t srcSig, uint32_t dstSig)
{
    int32_t result = WISE_FAIL;
    int32_t srcIdx = searchPartition(srcSig);
    int32_t dstIdx = searchPartition(dstSig);

    if ((srcIdx != WISE_INVALID_INDEX) && (dstIdx != WISE_INVALID_INDEX)) {
        uint32_t totalSize, blockSize, offset;
        PAR_INFO_T *srcPar = &fsInfo.parInfo[srcIdx];
        PAR_INFO_T *dstPar = &fsInfo.parInfo[dstIdx];
        // uint16_t crcCalc = 0;

        partitionErase(dstPar->sig, dstPar->offset, dstPar->parLen, 1);

        totalSize = srcPar->dataLen;
        offset    = 0;
        while (totalSize) {
            blockSize = totalSize > FS_BUF_SIZE ? FS_BUF_SIZE : totalSize;

            debug_print("%08lx->%08lx\n", srcPar->offset + offset, dstPar->offset + offset);

            wise_flash_read(srcPar->offset + offset, &fsTmpBuf[0], blockSize);
            wise_flash_write(dstPar->offset + offset, &fsTmpBuf[0], blockSize);

            totalSize -= blockSize;
            offset    += blockSize;
        }

        if (WISE_SUCCESS ==
            wise_fs_par_update_info(dstIdx, srcPar->dataLen, srcPar->crc)) {
            result = WISE_SUCCESS;
        } else {
            debug_print("partition copy failed\n");
        }
    }

    return result;
}

int32_t wise_fs_par_invalidate(uint32_t sig)
{
    int32_t parIndex = searchPartition(sig);
    int32_t result   = WISE_FAIL;

    if (parIndex != WISE_INVALID_INDEX) {
        partitionErase(fsInfo.parInfo[parIndex].sig, fsInfo.parInfo[parIndex].offset, fsInfo.parInfo[parIndex].parLen, 0);
        result = WISE_SUCCESS;
    }

    return result;
}

uint16_t wise_fs_get_par_crc(int32_t parIndex)
{
    return fsInfo.parInfo[parIndex].crc;
}

uint32_t wise_fs_par_index_to_sig(int32_t parIndex)
{
    uint32_t sig = 0xffffffff;

    if ((parIndex >= 0) && (parIndex < fsInfo.parNum)) {
        sig = fsInfo.parInfo[parIndex].sig;
    }

    return sig;
}

int32_t wise_fs_par_sig_to_index(uint32_t sig)
{
    return searchPartition(sig);
}

int32_t wise_fs_reload()
{
    int32_t result = WISE_FAIL;

    loadFSTable();
    if ((fsInfo.storageSize > 0) && (fsInfo.parNum > 0)) {
        result = WISE_SUCCESS;
    }

    return result;
}

int32_t wise_fs_get_fs_info(WISE_FS_INFO_T *outFsInfo)
{
    int32_t result = WISE_FAIL;

    if (outFsInfo) {
        loadFSTable();

        if (fsInfo.storageSize > 0) {
            // int i;

            memset(outFsInfo, 0, sizeof(WISE_FS_INFO_T));

            outFsInfo->storageSize = fsInfo.storageSize;
            outFsInfo->eraseSize   = fsInfo.flashInfo.minEraseSize;
            outFsInfo->fsVersion   = fsInfo.version;
            outFsInfo->parNum      = fsInfo.parNum;

            memcpy(&outFsInfo->partitions[0], &fsInfo.parInfo[0], (sizeof(PAR_INFO_T) * FS_MAX_PAR_NUM));

            result = WISE_SUCCESS;
        }
    }

    return result;
}

int32_t wise_fs_par_erase(int32_t parIndex)
{
    int32_t result = WISE_FAIL;

    if ((parIndex != WISE_INVALID_INDEX) && (parIndex < fsInfo.parNum)) {
        partitionErase(fsInfo.parInfo[parIndex].sig, fsInfo.parInfo[parIndex].offset, fsInfo.parInfo[parIndex].parLen, 1);
        result = WISE_SUCCESS;
    }

    return result;
}

int32_t wise_fs_par_write_data(int32_t parIndex, uint32_t parOffset, uint32_t dataLen, uint8_t *data)
{
    int32_t result = WISE_FAIL;

    if ((parIndex != WISE_INVALID_INDEX) && (parIndex < fsInfo.parNum)) {
        PAR_INFO_T *partition = &fsInfo.parInfo[parIndex];
        uint32_t writeOffset  = partition->offset + parOffset;

        if ((writeOffset + dataLen) >
            PAR_HDR_OFFSET(partition->offset, partition->parLen)) {
            debug_print("exceeds partition size\n");
            return WISE_FAIL;
        }

        if (WISE_SUCCESS == wise_flash_write(writeOffset, &data[0], dataLen)) {
            result = WISE_SUCCESS;
        }
    }

    return result;
}

int32_t wise_fs_par_update_info(int32_t parIndex, uint32_t dataLen, uint16_t crcCheck)
{
    int32_t result = WISE_FAIL;

    if ((parIndex != WISE_INVALID_INDEX) && (parIndex < fsInfo.parNum)) {
        PAR_INFO_T *partition = &fsInfo.parInfo[parIndex];
        uint16_t crcCalc      = 0;

        partition->dataLen = dataLen;
        crcCalc = calcPartitionCRC(partition->offset, partition->dataLen);

        if (crcCalc == crcCheck) {
            partition->crc = crcCheck;
            writeParInfo(partition);
            result = WISE_SUCCESS;
        }
    }

    return result;
}

int32_t wise_fs_user_data_erase()
{
    // int32_t result = WISE_FAIL;

    if (fsInfo.userParIdx == WISE_INVALID_INDEX) {
        return WISE_FAIL;
    }

    return wise_fs_par_erase(fsInfo.userParIdx);
}

uint32_t wise_fs_user_get_size()
{
    if (fsInfo.userParIdx == WISE_INVALID_INDEX) {
        return 0;
    }

    return fsInfo.parInfo[fsInfo.userParIdx].dataLen;
}

int32_t wise_fs_user_data_read(uint32_t offset, uint32_t readLen, uint8_t *pBuf)
{
    // int32_t result = WISE_FAIL;
    PAR_INFO_T *partition = NULL;

    if (fsInfo.userParIdx == WISE_INVALID_INDEX) {
        return WISE_FAIL;
    }

    partition  = &fsInfo.parInfo[fsInfo.userParIdx];
    offset    += partition->offset;

    return wise_flash_read(offset, pBuf, readLen);
}

int32_t wise_fs_user_page_erase(uint32_t offset)
{
    uint32_t secMask      = fsInfo.flashInfo.minEraseSize - 1;
    int32_t result        = WISE_SUCCESS;
    PAR_INFO_T *partition = NULL;
    uint32_t secStart     = 0;

    if ((fsInfo.userParIdx == WISE_INVALID_INDEX) || (offset & secMask)) {
        return WISE_FAIL;
    }

    partition = &fsInfo.parInfo[fsInfo.userParIdx];

    if (offset >= partition->parLen) {
        debug_print("exceed user data area\n");
        return WISE_FAIL;
    }

    secStart = partition->offset + offset;
    result   = wise_flash_sector_erase(secStart);

    debug_print("user page erase %08lx\n", secStart);

    return result;
}

int32_t wise_fs_user_data_write(uint32_t offset, uint32_t dataLen, uint8_t *pData)
{
    if (fsInfo.userParIdx == WISE_INVALID_INDEX) {
        return WISE_FAIL;
    }

    // debug_print("user data write %08x-%08x\n", offset, dataLen);
    // dump_buffer(pData, dataLen);

    return wise_fs_par_write_data(fsInfo.userParIdx, offset, dataLen, pData);
}

int32_t wise_fs_user_data_update(uint32_t offset, uint32_t dataLen, uint8_t *pData)
{
    int32_t result        = WISE_SUCCESS;
    PAR_INFO_T *partition = NULL;
    uint32_t secStart     = 0;
    uint32_t secMask      = fsInfo.flashInfo.minEraseSize - 1;

    if (fsInfo.userParIdx == WISE_INVALID_INDEX) {
        return WISE_FAIL;
    }

    partition = &fsInfo.parInfo[fsInfo.userParIdx];

    if ((offset >= partition->parLen) ||
        ((offset + dataLen) >= partition->parLen)) {
        debug_print("exceed user data area\n");
        return WISE_FAIL;
    }

    // debug_print("user data %08x-%08x\n", partition->offset,
    // partition->parLen); debug_print("flash block=%08x erase=%08x\n",
    // fsInfo.flashInfo.blockSize, fsInfo.flashInfo.minEraseSize);

    while (dataLen) {
        uint32_t writeSize = 0;
        uint32_t secOffset = 0;

        secStart = partition->offset + (offset & ~secMask);
        // debug_print("secStart=%08x\n", secStart);

        result = wise_flash_read(secStart, &fsTmpBuf[0], fsInfo.flashInfo.minEraseSize);
        if (result != WISE_SUCCESS) {
            break;
        }

        secOffset = offset & secMask;
        writeSize = (secOffset + dataLen) > fsInfo.flashInfo.minEraseSize ? (fsInfo.flashInfo.minEraseSize - secOffset) : (dataLen);

        // debug_print("writeSize=%d\n", writeSize);
        memcpy(&fsTmpBuf[secOffset], pData, writeSize);

        // debug_print("secOffset=%08x writeSize=%08x\n", secOffset, writeSize);

        result = wise_flash_sector_erase(secStart);
        if (result != WISE_SUCCESS) {
            break;
        }

        result = wise_flash_write(secStart, &fsTmpBuf[0], fsInfo.flashInfo.minEraseSize);
        if (result != WISE_SUCCESS) {
            break;
        }

        dataLen -= writeSize;
        pData   += writeSize;
        offset  += writeSize;
    }

    return result;
}

void wise_fs_debug_info()
{
    int i;
    uint32_t sig = 0;

    if (fsInfo.fsSig != FS_SIG_FS) {
        debug_print("ESMT file system not found\n");
        return;
    }

    debug_print("FS Info\n");
    debug_print("   ver: %d\n", fsInfo.version);
    debug_print("   attr: %02X\n", fsInfo.attribute);
    debug_print("   crc: %04X\n", fsInfo.crc);
    debug_print("   parNum: %d\n", fsInfo.parNum);

    for (i = 0; i < fsInfo.parNum; i++) {
        PAR_INFO_T *par = &fsInfo.parInfo[i];
        sig             = par->sig;

        debug_print("   partition %d %08lx-%08lx\n", i, par->offset, par->parLen);
        debug_print("       sig: %c%c%c%c\n", (int)(sig >> 24), (int)(sig >> 16), (int)(sig >> 8), (int)(sig & 0xff));
        debug_print("       dataLen: %08lx\n", par->dataLen);
    }

    if (fsInfo.patchOffset && fsInfo.patchSize) {
        debug_print("   Patch data found @%08lx len=%08lx\n", fsInfo.patchOffset, fsInfo.patchSize);
    }
}

static int32_t checkPatchArea()
{
    int32_t result = WISE_FAIL;
    uint32_t ptSig, ptLen, ptCrc, crcCalc;

    if (fsInfo.patchOffset == 0) {
        goto finish;
    }

    memset(&fsTmpBuf[0], 0, fsInfo.flashInfo.minEraseSize);
    wise_flash_read(fsInfo.patchOffset, &fsTmpBuf[0], fsInfo.flashInfo.minEraseSize);

    ptSig = LOAD_BE_32(&fsTmpBuf[0]);
    ptLen = LOAD_BE_32(&fsTmpBuf[4]);
    ptCrc = LOAD_BE_32(&fsTmpBuf[8]);
    if ((ptSig != FS_SIG_PATCH) || (ptLen == 0) ||
        (ptLen >= fsInfo.flashInfo.minEraseSize)) {
        // debug_print("Empty patch area\n");
        goto finish;
    }

    // wise_flash_read(fsInfo.patchOffset + PATCH_HDR_LEN, &fsTmpBuf[0], ptLen);
    crcCalc = fsCalcCrc16(&fsTmpBuf[PATCH_HDR_LEN], ptLen, FS_CRC16_SEED);
    if (crcCalc != ptCrc) {
        debug_print("Patch data cruption!!\n");
        goto finish;
    }

    fsInfo.patchSig  = ptSig;
    fsInfo.patchSize = ptLen;
    fsInfo.patchCrc  = ptCrc;
    
    debug_print("Patch data found %08lx-%08lx\n", fsInfo.patchOffset, fsInfo.patchSize);

    result = WISE_SUCCESS;
    return result;

finish:
    fsInfo.patchSig  = 0;
    fsInfo.patchSize = 0;
    fsInfo.patchCrc  = 0;
    return result;
}

int32_t wise_fs_patch_get_info(uint32_t *outOffset, int32_t *outLen)
{
    int32_t result = WISE_FAIL;

    if (WISE_SUCCESS != checkPatchArea()) {
        goto finish;
    }

    *outOffset = fsInfo.patchOffset + PATCH_HDR_LEN;
    *outLen    = fsInfo.patchSize;

    result = WISE_SUCCESS;

finish:
    return result;
}

int32_t wise_fs_patch_read(int32_t bufLen, uint8_t *pBuf)
{
    int32_t result = WISE_FAIL;

    if (WISE_SUCCESS != checkPatchArea()) {
        goto finish;
    }
    if (bufLen < (int32_t)fsInfo.patchSize) {
        goto finish;
    }

    memcpy(pBuf, &fsTmpBuf[PATCH_HDR_LEN], fsInfo.patchSize);

    result = WISE_SUCCESS;

finish:
    return result;
}

int32_t wise_fs_patch_update(int32_t dataLen, uint8_t *pData)
{
    int32_t result   = WISE_FAIL;
    uint32_t crcCalc = 0;

    if (!fsInfo.patchOffset) {
        goto finish;
    }
    if (dataLen >= (int32_t)fsInfo.flashInfo.minEraseSize) {
        goto finish;
    }

    memset(&fsTmpBuf[0], 0, fsInfo.flashInfo.minEraseSize);

    memcpy(&fsTmpBuf[PATCH_HDR_LEN], pData, dataLen);
    crcCalc = fsCalcCrc16(&fsTmpBuf[PATCH_HDR_LEN], dataLen, FS_CRC16_SEED);

    PUT_BE_32(&fsTmpBuf[0], FS_SIG_PATCH);
    PUT_BE_32(&fsTmpBuf[4], dataLen);
    PUT_BE_32(&fsTmpBuf[8], crcCalc);

    wise_flash_sector_erase(fsInfo.patchOffset);
    wise_flash_write(fsInfo.patchOffset, &fsTmpBuf[0], dataLen + PATCH_HDR_LEN);
    result = WISE_SUCCESS;

finish:
    return result;
}

int32_t wise_fs_patch_erase()
{
    int32_t result = WISE_FAIL;

    if (!fsInfo.patchOffset) {
        goto finish;
    }

    wise_flash_sector_erase(fsInfo.patchOffset);

    fsInfo.patchSig  = 0;
    fsInfo.patchSize = 0;
    fsInfo.patchCrc  = 0;

    result = WISE_SUCCESS;

finish:
    return result;
}

static uint16_t fsCalcCrc16(uint8_t *buf, int length, uint16_t startValue)
{
    return util_crc16_calc(FS_CRC_POLY, (uint16_t*)fsCrcTable, buf, length, startValue);
}
#endif /* MIDDLEWARE_WISE_FLASH_FILESYSTEM */

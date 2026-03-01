/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_flash_api.h"
#include "util.h"
#include "util_crc.h"

#define PACK_XIP_CFG(m, c)              (((m << 4) & 0xf0) | (c & 0xf))
#define XIP_CFG_MODE(v)                 ((v >> 4) & 0xf)
#define XIP_CFG_CLK(v)                  (v & 0xf)

enum
{
    E_OTP_ADDR_DEC = 0,
    E_OTP_ADDR_INC = 1,
};

typedef struct
{
    uint32_t id;
    uint32_t flashSize;
    uint32_t blockSize;
    uint32_t sectorSize;
    uint32_t otpStartOffset;
    uint32_t otpSectorSize;
    uint8_t otpSectorNum;
    uint8_t otpOffsetDir;
    uint8_t uidLength;
    uint8_t xipCfg;
} ST_FLASH_INFO_T;

static const ST_FLASH_INFO_T flashSupportList[] =
{
    {
        0x0014381C,				//ID
        0x100000,				//flash size
		0x10000,                //block size
        0x1000,					//sector size
        0xFF000,				//OTP start offset
        512,					//OTP sector size
        3,						//OTP sector number
        E_OTP_ADDR_DEC,			//OTP sector address calculation
        12,                     //UID length
        0,                      //XIP config
    },
    {
        0x0015381C,             //ID
        0x200000,               //flash size
		0x10000,                //block size
        0x1000,                 //sector size
        0x1FF000,               //OTP start offset
        512,                    //OTP sector size
        3,                      //OTP sector number
        E_OTP_ADDR_DEC,         //OTP sector address calculation
        12,                     //UID length
        0,                      //XIP config
    },
    {
        0x0014481C,             //ID
        0x100000,               //flash size
		0x10000,                //block size
        0x1000,                 //sector size
        0xFF000,                //OTP start offset
        1024,                   //OTP sector size
        3,                      //OTP sector number
        E_OTP_ADDR_DEC,         //OTP sector address calculation
        16,                     //UID length
        0,                      //XIP config
    },
    {
        0x0015481C,             //ID
        0x200000,               //flash size
		0x10000,                //block size
        0x1000,                 //sector size
        0x1FF000,               //OTP start offset
        1024,                   //OTP sector size
        3,                      //OTP sector number
        E_OTP_ADDR_DEC,         //OTP sector address calculation
        16,                     //UID length
        0,                      //XIP config
    },
    {
        0x00146085,             //ID
        0x100000,               //flash size
		0x10000,                //block size
        0x1000,                 //sector size
        0x3000,                 //OTP start offset
        512,                    //OTP sector size
        3,                      //OTP sector number
        E_OTP_ADDR_DEC,         //OTP sector address calculation
        16,                     //UID length
        0,                      //XIP config
    },
    {
        0x00136085,             //ID
        0x80000,                //flash size
		0x10000,                //block size
        0x1000,                 //sector size
        0x3000,                 //OTP start offset
        512,                    //OTP sector size
        3,                      //OTP sector number
        E_OTP_ADDR_DEC,         //OTP sector address calculation
        16,                     //UID length
        PACK_XIP_CFG(XIP_SPI_MODE_QUAD, XIP_SPI_CLK_40M),
    },
	{
		0x0013381C,             //ID
	    0x80000,                //flash size
		0x10000,                //block size
	    0x1000,                 //sector size
	    0x7F000,                //OTP start offset
	    512,                    //OTP sector size
	    1,                      //OTP sector number
	    E_OTP_ADDR_DEC,         //OTP sector address calculation
	    12,                     //UID length
	    0,                      //XIP config
	},
};

static ST_FLASH_INFO_T* activeFlash = NULL;
static uint32_t flashID = 0;

void _flash_probe();
int32_t _flash_get_xip_cfg(uint8_t* mode, uint8_t* clk);

void _flash_probe()
{
    int i, listNum;

    if(activeFlash)
        return;
    
    listNum = sizeof(flashSupportList) / sizeof(ST_FLASH_INFO_T);

    hal_intf_flash_get_dev_id(&flashID);

    for(i = 0; i < listNum; i++)
    {
        if(flashID == flashSupportList[i].id)
        {
            activeFlash = (ST_FLASH_INFO_T*)&flashSupportList[i];
            break;
        }
    }

    if(!activeFlash)
    {
        //debug_print("flash probe failed\n");
        while(1);
    }
}

int32_t _flash_get_xip_cfg(uint8_t* mode, uint8_t* clk)
{
    if(!activeFlash)
        return WISE_FAIL;

    if(mode)
        *mode = XIP_CFG_MODE(activeFlash->xipCfg);

    if(clk)
        *clk = XIP_CFG_CLK(activeFlash->xipCfg);

    return WISE_SUCCESS;
}


void wise_flash_init()
{
    hal_intf_flash_init();
    _flash_probe();
}

int32_t wise_flash_get_info(WISE_FLASH_INFO_T *outInfo)
{
    if (outInfo) {
        outInfo->flashSize    = activeFlash->flashSize;
        outInfo->blockSize    = activeFlash->blockSize;
        outInfo->minEraseSize = activeFlash->sectorSize;

        //debug_print("FLASH ID: %08x\n", (unsigned int)activeFlash->id);
    }

    return WISE_SUCCESS;
}

int32_t wise_flash_get_uid(uint8_t *uid, uint8_t* ioLen)
{
    int32_t result = WISE_SUCCESS;
    uint8_t *tmpBuf = NULL;

    if(activeFlash->uidLength == 0)
        return WISE_FAIL;

    tmpBuf = malloc(activeFlash->uidLength);
    memset(tmpBuf, 0, activeFlash->uidLength);
    hal_intf_flash_get_uid(tmpBuf);

    *ioLen = *ioLen > activeFlash->uidLength ? activeFlash->uidLength : *ioLen;
    memcpy(uid, tmpBuf, *ioLen);

    free(tmpBuf);
    
    /*
    hal_intf_flash_get_uid(uid);

    memset(uid_chk, 0xFF, sizeof(uid_chk));
    if (memcmp(uid, uid_chk, sizeof(uid_chk)) == 0) {
        result = WISE_FAIL;
    }
    */
    return result;
}

int32_t wise_flash_sector_erase(uint32_t flashOffset)
{
    int32_t result = WISE_FAIL;
    uint32_t mask = activeFlash->sectorSize - 1;
    
    if (flashOffset & mask) {
        return WISE_FAIL;
    }

    result = hal_intf_flash_sector_erase(flashOffset);

    return result;
}

int32_t wise_flash_block_erase(uint32_t flashOffset)
{
    int32_t result = WISE_FAIL;
    uint32_t mask;

    if(activeFlash->blockSize == 0)
        return WISE_FAIL;

    mask = activeFlash->blockSize - 1;
    
    if (flashOffset & mask) {
        return WISE_FAIL;
    }

    result = hal_intf_flash_block_erase(flashOffset);

    return result;
}

int32_t wise_flash_write(uint32_t flashOffset, uint8_t *pu8Buf, uint32_t u32ByteLen)
{
#define TEMP_BUF_LEN 16

    int32_t result      = WISE_FAIL;
    uint32_t segLen     = 0;
    uint8_t ramTempMode = 0;
    uint8_t tempBuf[TEMP_BUF_LEN];

    if (pu8Buf < (uint8_t *)(WISE_FLASH_MAP_ADDR + activeFlash->flashSize)) {
        // in case the source buffer is in flash
        // use a small ram buffer as temp because it happens rarely.
        ramTempMode = 1;
    }

    while (u32ByteLen) {
        uint8_t *pSrc = pu8Buf;

        if (flashOffset & CHIP_FLASH_INTERNAL_PAGE_MASK) {
            uint32_t headLen = CHIP_FLASH_INTERNAL_PAGE_SIZE - (flashOffset & CHIP_FLASH_INTERNAL_PAGE_MASK);

            segLen = u32ByteLen > headLen ? headLen : u32ByteLen;
        } else {
            segLen = u32ByteLen > CHIP_FLASH_INTERNAL_PAGE_SIZE ? CHIP_FLASH_INTERNAL_PAGE_SIZE
                                                       : u32ByteLen;
        }

        if (ramTempMode) {
            if (segLen >= TEMP_BUF_LEN) {
                segLen = TEMP_BUF_LEN;
            }

            memcpy(&tempBuf[0], pu8Buf, segLen);
            pSrc = &tempBuf[0];
        }

        if (hal_intf_flash_write(flashOffset, pSrc, segLen) != WISE_SUCCESS) {
            goto finish;
        }

        flashOffset += segLen;
        u32ByteLen  -= segLen;
        pu8Buf      += segLen;
    }

    result = WISE_SUCCESS;

finish:
    return result;
}

int32_t wise_flash_read(uint32_t flashOffset, uint8_t *pu8Buf, uint32_t u32ByteLen)
{
    return hal_intf_flash_read(flashOffset, pu8Buf, u32ByteLen);
}

//==================== APIs for shadow region access =====================
#define OTP_SEC_HEADER_LEN              4
#define OTP_SEC_FDB_LEN                 4
#define OTP_SEC_ID  					0x454D
#define OTP_SEC_STS_ACTIVE              (0xAA)
#define OTP_MAX_FILE_NUM                (16)
#define CRC16_INIT						0x0000
#define OTP_FILE_HEADER_LEN             10
#define REC_ID_MSTER_BOOT  				0x4D534249
#define FHDR_ID_OFFSET					0
#define FHDR_LEN_OFFSET					4
#define FHDR_CRC_OFFSET					6
#define FHDR_STATUS_OFFSET				8
#define FHDR_CHECKSUM_OFFSET			9
#define BOOT_INFO_LEN                   12

uint32_t _otpSecStart(int8_t sector)
{
    if(activeFlash->otpOffsetDir == E_OTP_ADDR_DEC)
        return (activeFlash->otpStartOffset - sector * activeFlash->sectorSize);
    else
        return (activeFlash->otpStartOffset + sector * activeFlash->sectorSize);
}

int32_t wise_flash_shadow_get_info(FLASH_SHAD_RGN_INFO_T *outInfo)
{
	if (outInfo) {
		outInfo->sectorNum = activeFlash->otpSectorNum;
		outInfo->sectorSize = activeFlash->otpSectorSize;
	}

	return WISE_SUCCESS;
}

int32_t wise_flash_shadow_sector_erase(int32_t sector)
{
	int32_t result = WISE_FAIL;
	uint32_t startAddr = _otpSecStart(sector);

	if(activeFlash->otpSectorNum == 0)
		return WISE_FAIL;

	if((sector < 0) || (sector >= activeFlash->otpSectorNum))
        return WISE_FAIL;

	result = hal_intf_otp_sector_erase(startAddr);

	return result;
}

int32_t wise_flash_shadow_write(int32_t sector, uint32_t otpOffset, uint8_t *pu8Buf,
                         uint32_t u32ByteLen)
{
	int32_t result = WISE_FAIL;
	uint32_t segLen = 0;
    uint32_t startAddr = _otpSecStart(sector) + otpOffset;

    if(activeFlash->otpSectorNum == 0)
    	return WISE_FAIL;

    if((sector < 0) || (sector >= activeFlash->otpSectorNum))
        return WISE_FAIL;

    if((otpOffset + u32ByteLen) > activeFlash->otpSectorSize)
   	    return WISE_FAIL;

	while(u32ByteLen)
    {
        uint8_t* pSrc = pu8Buf;

        if(startAddr & 0xFF)
        {
            uint32_t headLen = 0x100 - (startAddr & 0xFF);

            segLen = u32ByteLen > headLen ? headLen : u32ByteLen;
        }
        else
        {
            segLen = u32ByteLen > 0x100 ? 0x100 : u32ByteLen;
        }

        result = hal_intf_otp_write(startAddr, pSrc, segLen);

        startAddr += segLen;
        u32ByteLen -= segLen;
        pu8Buf += segLen;
    }

	return result;
}

int32_t wise_flash_shadow_read(int32_t sector, uint32_t otpOffset, uint8_t *pu8Buf,
                        uint32_t u32ByteLen)
{
	int32_t result = WISE_FAIL;
	uint32_t startAddr = _otpSecStart(sector) + otpOffset;

    if(activeFlash->otpSectorNum == 0)
    	return WISE_FAIL;

    if((sector < 0) || (sector >= activeFlash->otpSectorNum))
        return WISE_FAIL;

    if((otpOffset + u32ByteLen) > activeFlash->otpSectorSize)
	    return WISE_FAIL;

    result = hal_intf_otp_read(startAddr, pu8Buf, u32ByteLen);

	return result;
}

static void _parse_msbi_info(WISE_MSBI_INFO_T *out, const uint8_t *info)
{
    out->consoleEnable  = (info[0] >> 0) & 0x01;
    out->bootCrcEnable  = (info[0] >> 1) & 0x01;
    out->xipSpiMode     = (info[0] >> 2) & 0x03;
    out->xipSpiSpeed    = (info[0] >> 4) & 0x01;
    out->xipClock       = (info[0] >> 5) & 0x01;
    out->xipCache       = (info[0] >> 6) & 0x01;
    out->consoleBaudrate = (info[8] >> 0) & 0x03;
}

static void _build_msbi_info(uint8_t *info, const WISE_MSBI_INFO_T *in)
{
    info[0] = ((in->consoleEnable & 0x01) << 0);
    info[0] |= ((in->bootCrcEnable & 0x01) << 1);
    info[0] |= ((in->xipSpiMode & 0x03) << 2);
    info[0] |= ((in->xipSpiSpeed & 0x01) << 4);
    info[0] |= ((in->xipClock & 0x01) << 5);
    info[0] |= ((in->xipCache & 0x01) << 6);
    info[8] = (in->consoleBaudrate & 0x03) << 0;
}

static bool _find_msbi_file(uint8_t *secIndex, uint16_t *dataStart, uint16_t *dataLen, uint16_t *actureLen)
{
	bool msbiFind = false;

    for(int i = 0; i < activeFlash->otpSectorNum; i++)
    {
        uint8_t secInfo[OTP_SEC_HEADER_LEN];
        wise_flash_shadow_read(i, 0, secInfo, OTP_SEC_HEADER_LEN);

        if(LOAD_BE_16(&secInfo[0]) != OTP_SEC_ID ||
            secInfo[2] > OTP_MAX_FILE_NUM || secInfo[3] != OTP_SEC_STS_ACTIVE)
        {
            continue;
        }

        int maxFdbNum = secInfo[2];
        uint8_t *fdbBuffer = malloc(maxFdbNum * OTP_SEC_FDB_LEN);
        if(!fdbBuffer)
        {
        	return false;
        }

        wise_flash_shadow_read(i, OTP_SEC_HEADER_LEN, fdbBuffer, maxFdbNum * OTP_SEC_FDB_LEN);

        for(int j = 0; j < maxFdbNum; j++)
        {
            uint16_t fileStart = LOAD_BE_16(&fdbBuffer[j * OTP_SEC_FDB_LEN]);
            uint16_t fileSize  = LOAD_BE_16(&fdbBuffer[j * OTP_SEC_FDB_LEN + 2]);

            if(fileStart == 0xFFFF || fileSize == 0xFFFF)
            {
            	break;
            }

            uint8_t fileHeader[OTP_FILE_HEADER_LEN];
            wise_flash_shadow_read(i, fileStart, fileHeader, OTP_FILE_HEADER_LEN);

            uint8_t sum = 0;
            for(int k = 0; k < OTP_FILE_HEADER_LEN; k++)
            {
            	sum += fileHeader[k];
            }
            if(sum != 0)
            {
            	continue;
            }

            uint32_t fileID = LOAD_BE_32(&fileHeader[FHDR_ID_OFFSET]);
            uint8_t  status = fileHeader[FHDR_STATUS_OFFSET];
            if(fileID == REC_ID_MSTER_BOOT && status == OTP_SEC_STS_ACTIVE)
            {
                *secIndex = i;
                *dataStart = fileStart + OTP_FILE_HEADER_LEN;
                *dataLen = LOAD_BE_16(&fileHeader[FHDR_LEN_OFFSET]);
                *actureLen = fileStart + fileSize;
                msbiFind = true;
            }
        }
        free(fdbBuffer);
    }

    return msbiFind;
}

int32_t wise_flash_shadow_read_msbi_info(WISE_MSBI_INFO_T *outInfo)
{
    if(activeFlash->otpSectorNum == 0 || !outInfo)
    {
    	return WISE_FAIL;
    }

    uint8_t secIndex;
    uint16_t dataStart, dataLen, actureLen;

    if(!_find_msbi_file(&secIndex, &dataStart, &dataLen, &actureLen))
    {
        return WISE_FAIL;
    }

    uint8_t info[BOOT_INFO_LEN] = {0};
    wise_flash_shadow_read(secIndex, dataStart, info, BOOT_INFO_LEN);

    _parse_msbi_info(outInfo, info);

    return WISE_SUCCESS;
}

int32_t wise_flash_shadow_write_msbi_info(WISE_MSBI_INFO_T *inInfo)
{
    if(activeFlash->otpSectorNum == 0 || !inInfo)
    {
    	return WISE_FAIL;
    }

    uint8_t secIndex;
    uint16_t dataStart, dataLen, actureLen;

    if(!_find_msbi_file(&secIndex, &dataStart, &dataLen, &actureLen))
    {
        return WISE_FAIL;
    }

    uint8_t *data = malloc(actureLen);
    if(!data)
    {
    	return WISE_FAIL;
    }

    wise_flash_shadow_read(secIndex, 0, data, actureLen);

    _build_msbi_info(&data[dataStart], inInfo);

    uint8_t *crcTable = malloc(2 * 256);
    if(!crcTable)
    {
        free(data);
        return WISE_FAIL;
    }
    util_crc16_gen_table(CRC_TYPE_16_MCRF4XX, (uint16_t*)crcTable);
    uint16_t crcCalc = util_crc16_calc(CRC_TYPE_16_MCRF4XX, (uint16_t *)crcTable, &data[dataStart], dataLen, CRC16_INIT);
    PUT_BE_16(&data[dataStart - OTP_FILE_HEADER_LEN + FHDR_CRC_OFFSET], crcCalc);
    free(crcTable);

    uint8_t hdrSum = 0;
    for(int i = 0; i < OTP_FILE_HEADER_LEN - 1; i++)
    {
        hdrSum += data[dataStart - OTP_FILE_HEADER_LEN + i];
    }
    data[dataStart - OTP_FILE_HEADER_LEN + FHDR_CHECKSUM_OFFSET] = (uint8_t)(0xFF - hdrSum + 1);

    wise_flash_shadow_sector_erase(secIndex);
    wise_flash_shadow_write(secIndex, 0, data, actureLen);

    free(data);
    return WISE_SUCCESS;
}


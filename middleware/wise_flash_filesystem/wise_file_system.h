/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_file_system.h
 * @brief wise_file_system.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */


#ifndef __WISE_FILE_SYSTEM_H
#define __WISE_FILE_SYSTEM_H

#include <stdint.h>

#include "wise_flash_api.h"

// #define FS_SIG_FS               (uint32_t)'ESFS'
// #define FS_SIG_SBL              (uint32_t)'ESBL'
// #define FS_SIG_APP              (uint32_t)'ESAP'
// #define FS_SIG_FUTEMP           (uint32_t)'ESFU'
// #define FS_SIG_USER             (uint32_t)'ESUR'
// #define FS_SIG_PATCH            (uint32_t)'ESPT'
#define FS_SIG_FS                   0x45534653  // 'ESFS'
#define FS_SIG_SBL                  0x4553424C  // 'ESBL'
#define FS_SIG_APP                  0x45534150  // 'ESAP'
#define FS_SIG_FUTEMP               0x45534655  // 'ESFU'
#define FS_SIG_USER                 0x45535552  // 'ESUR'
#define FS_SIG_PATCH                0x45535054  // 'ESPT'

/*
#define FS_SECTOR_SIZE          WISE_FLASH_SECTOR_SIZE
#define FS_BLOCK_SIZE           WISE_FLASH_BLOCK_SIZE
#define FS_STORAGE_SIZE         WISE_FLASH_SIZE
*/

// #define FS_MIN_ERASE_SIZE       FS_SECTOR_SIZE

#define FS_BUF_SIZE                 256

// File system definitions
#define FS_VERSION                  1
#define FS_HDR_LEN                  32
#define FS_PAR_INFO_LEN             8
#define FS_MAX_PAR_NUM              8

#define FS_HDR_SIG_OFFSET           0
#define FS_HDR_SIG_BYTES            4
#define FS_HDR_LEN_OFFSET           (FS_HDR_SIG_OFFSET + FS_HDR_SIG_BYTES)
#define FS_HDR_LEN_BYTES            1
#define FS_HDR_NUM_OFFSET           (FS_HDR_LEN_OFFSET + FS_HDR_LEN_BYTES)
#define FS_HDR_NUM_BYTES            1
#define FS_HDR_VER_OFFSET           (FS_HDR_NUM_OFFSET + FS_HDR_NUM_BYTES)
#define FS_HDR_VER_BYTES            1
#define FS_HDR_ATTR_OFFSET          (FS_HDR_VER_OFFSET + FS_HDR_VER_BYTES)
#define FS_HDR_ATTR_BYTES           1
#define FS_HDR_CRC_OFFSET           (FS_HDR_ATTR_OFFSET + FS_HDR_ATTR_BYTES)
#define FS_HDR_CRC_BYTES            2

#define FS_HDR_TABLE_OFFSET         FS_HDR_LEN

// Partition definitions
#define PAR_HDR_LEN                 16

#define PAR_HDR_SIG_OFFSET          0
#define PAR_HDR_SIG_BYTES           4
#define PAR_HDR_LEN_OFFSET          (PAR_HDR_SIG_OFFSET + PAR_HDR_SIG_BYTES)
#define PAR_HDR_LEN_BYTES           4
#define PAR_HDR_CRC_OFFSET          (PAR_HDR_LEN_OFFSET + PAR_HDR_LEN_BYTES)
#define PAR_HDR_CRC_BYTES           2
#define PAR_HDR_ATTR_OFFSET         (PAR_HDR_CRC_OFFSET + PAR_HDR_CRC_BYTES)

#define PAR_ENTRY_SIZE              8

typedef struct {
    uint32_t sig;
    uint32_t offset;
    uint32_t parLen;
    uint32_t dataLen;
    uint16_t crc;
    uint8_t attr;
    uint8_t reserved;
} PAR_INFO_T;

typedef struct {
    uint32_t storageSize;
    uint32_t eraseSize;
    uint8_t fsVersion;
    uint8_t parNum;
    PAR_INFO_T partitions[FS_MAX_PAR_NUM];
} WISE_FS_INFO_T;

typedef struct {
    uint32_t sig;
    uint32_t offset;
    uint32_t size;
} WISE_FS_PART_PARAM_T;

int32_t wise_fs_init();
int32_t wise_fs_create_default();
int32_t wise_fs_create(uint8_t parNum, WISE_FS_PART_PARAM_T *parTable);
// int32_t wise_fs_format();
int32_t wise_fs_reload();
int32_t wise_fs_get_fs_info(WISE_FS_INFO_T *outFsInfo);
int32_t wise_fs_check_par_valid(uint32_t sig);
int32_t wise_fs_par_sig_to_index(uint32_t sig);
int32_t wise_fs_get_par_info(uint32_t sig, PAR_INFO_T *parInfo);
int32_t wise_fs_par_copy(uint32_t srcSig, uint32_t dstSrg);
int32_t wise_fs_par_invalidate(uint32_t sig);

uint16_t wise_fs_get_par_crc(int32_t parIndex);
uint32_t wise_fs_par_index_to_sig(int32_t parIndex);
int32_t wise_fs_par_erase(int32_t parIndex);
int32_t wise_fs_par_write_data(int32_t parIndex, uint32_t offset,
                               uint32_t dataLen, uint8_t *data);
int32_t wise_fs_par_update_info(int32_t parIndex, uint32_t dataLen,
                                uint16_t crcCheck);

uint32_t wise_fs_user_get_size();
int32_t wise_fs_user_data_erase();
int32_t wise_fs_user_page_erase(uint32_t offset);
int32_t wise_fs_user_data_read(uint32_t offset, uint32_t dataLen,
                               uint8_t *pBuf);
int32_t wise_fs_user_data_write(uint32_t offset, uint32_t dataLen,
                                uint8_t *pData);
int32_t wise_fs_user_data_update(uint32_t offset, uint32_t dataLen,
                                 uint8_t *pData);

int32_t wise_fs_patch_get_info(uint32_t *outOffset, int32_t *outLen);
int32_t wise_fs_patch_read(int32_t bufLen, uint8_t *pBuf);
int32_t wise_fs_patch_update(int32_t dataLen, uint8_t *pData);
int32_t wise_fs_patch_erase();

// debug API
void wise_fs_debug_info();

#endif

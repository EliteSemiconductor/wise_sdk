#ifndef __EFUSE_ER8130_H
#define __EFUSE_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

#define EFUSE_CMD_READ_HWORD 0x02
#define EFUSE_CMD_READ_WORD 0x04
#define EFUSE_CMD_WRITE_HWORD 0x0A
#define EFUSE_CMD_WRITE_WORD 0x0C

#define EFUSE_ADDR_WORD_MASK 0x3
#define EFUSE_UNIQUE_ID0_ADDR 0x0000
#define EFUSE_UNIQUE_ID1_ADDR 0x0004
#define EFUSE_UNIQUE_ID2_ADDR 0x0008

HAL_STATUS efuse_read_er8130(uint8_t base_idx, uint32_t addr, uint8_t *buf);
HAL_STATUS efuse_write_er8130(uint8_t base_idx, uint32_t addr, uint8_t *buf);
void efuse_get_chip_unique_er8130(uint8_t* id_arr);

#endif

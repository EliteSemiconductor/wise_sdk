#ifndef __EFUSE_ER8130_H
#define __EFUSE_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

#define EFUSE_CMD_READ_HWORD 0x02
#define EFUSE_CMD_READ_WORD 0x04
#define EFUSE_CMD_WRITE_HWORD 0x0A
#define EFUSE_CMD_WRITE_WORD 0x0C

int8_t efuse_read_word_er8130(uint16_t addr, uint32_t *data);
int8_t efuse_write_word_er8130(uint16_t addr, uint32_t data);

#endif

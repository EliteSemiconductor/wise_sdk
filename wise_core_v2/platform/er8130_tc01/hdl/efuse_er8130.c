#include "hdl/efuse_er8130.h"

#define EFUSE_ADDR_WORD_MASK 0x3

static uint8_t _efuse_wait_busy(void);
static void _efuse_config(uint8_t cmd, uint16_t addr);
static void _efuse_write_word(uint32_t data);
static uint32_t _efuse_read_word(void);

static uint8_t _efuse_wait_busy(void)
{
    return (uint8_t)((REG_R32(EFUCTL_CTL0_ADDR) >> EFUCTL_CMD_POS) &
                     EFUCTL_CMD_MASK);
}

static void _efuse_config(uint8_t cmd, uint16_t addr)
{
    uint32_t reg;
    reg = ((cmd << EFUCTL_CMD_POS) & EFUCTL_CMD_MASK) |
          ((addr << EFUCTL_ADR_POS) & EFUCTL_ADR_MASK);
    REG_W32(EFUCTL_CTL0_ADDR, reg);
}

static void _efuse_write_word(uint32_t data)
{
    REG_W32(EFUCTL_CTL1_ADDR, data);
}

static uint32_t _efuse_read_word(void)
{
    return REG_R32(EFUCTL_CTL1_ADDR);
}

int8_t efuse_read_word_er8130(uint16_t addr, uint32_t *data)
{
    int8_t status = HAL_NO_ERR;

    if (addr & EFUSE_ADDR_WORD_MASK) {
        status = HAL_ERR;
        goto error;
    }

    while (_efuse_wait_busy())
        ;
    _efuse_config(EFUSE_CMD_READ_WORD, addr);
    while (_efuse_wait_busy())
        ;
    *data = _efuse_read_word();

error:
    return status;
}

int8_t efuse_write_word_er8130(uint16_t addr, uint32_t data)
{
    int8_t status = HAL_NO_ERR;

    if (addr & EFUSE_ADDR_WORD_MASK) {
        status = HAL_ERR;
        goto error;
    }

    while (_efuse_wait_busy())
        ;
    _efuse_config(EFUSE_CMD_WRITE_WORD, addr);
    _efuse_write_word(data);
    while (_efuse_wait_busy())
        ;

error:
    return status;
}

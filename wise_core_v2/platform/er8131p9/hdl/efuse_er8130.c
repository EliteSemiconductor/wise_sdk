#include "hdl/efuse_er8130.h"
#include "hal_intf_efuse.h"

static void set_efuse_ctl_cfg(uint8_t base_idx, uint8_t cmd, uint16_t addr)
{
    uint32_t reg_addr = 0x0;
    uint32_t reg_val  = 0x0;

    switch(base_idx){
    case EFUSE_BASE_MCU:
        reg_addr = EFUSE_MCU_EFUCTL_CTL0_ADDR;
        reg_val  = ((cmd << EFUSE_MCU_EFUCTL_CMD_POS) & EFUSE_MCU_EFUCTL_CMD_MASK) |
                   ((addr << EFUSE_MCU_EFUCTL_ADR_POS) & EFUSE_MCU_EFUCTL_ADR_MASK);
        break;
    case EFUSE_BASE_NFC:
        reg_addr = EFUSE_NFC_EFUCTL_CTL0_ADDR;
        reg_val  = ((cmd << EFUSE_NFC_EFUCTL_CMD_POS) & EFUSE_NFC_EFUCTL_CMD_MASK) |
                   ((addr << EFUSE_NFC_EFUCTL_ADR_POS) & EFUSE_NFC_EFUCTL_ADR_MASK);
        break;
    default:
        break;
    }


    REG_W32(reg_addr, reg_val);
}

static void _efuse_write_word(uint8_t base_idx, uint32_t val)
{
    uint32_t reg_addr = 0x0;

    switch(base_idx){
    case EFUSE_BASE_MCU:
        reg_addr = EFUSE_MCU_EFUCTL_CTL1_ADDR;
        break;
    case EFUSE_BASE_NFC:
        reg_addr = EFUSE_NFC_EFUCTL_CTL1_ADDR;
        break;
    default:
        break;
    }

    printf("write val = 0x%08lx\n", val);

    REG_W32(reg_addr, val);
}

static uint32_t _efuse_read_word(uint8_t base_idx)
{
    uint32_t reg_addr = 0x0;

    switch(base_idx){
    case EFUSE_BASE_MCU:
        reg_addr = EFUSE_MCU_EFUCTL_CTL1_ADDR;
        break;
    case EFUSE_BASE_NFC:
        reg_addr = EFUSE_NFC_EFUCTL_CTL1_ADDR;
        break;
    default:
        break;
    }

    return REG_R32(reg_addr);
}

static inline uint8_t wait_for_efuse_idle(uint8_t base_idx)
{
    uint8_t val = 1;
    uint32_t time_out = 1000;

    while (val && time_out--) {

        switch(base_idx) {
        case EFUSE_BASE_MCU:
            val = (uint8_t)((REG_R32(EFUSE_MCU_EFUCTL_CTL0_ADDR) >> EFUSE_MCU_EFUCTL_CMD_POS) & EFUSE_MCU_EFUCTL_CMD_MASK);
            break;
        case EFUSE_BASE_NFC:
            val = (uint8_t)((REG_R32(EFUSE_NFC_EFUCTL_CTL0_ADDR) >> EFUSE_NFC_EFUCTL_CMD_POS) & EFUSE_NFC_EFUCTL_CMD_MASK);
            break;
        default:
            return 1;
        }

    }

    if (time_out == 0) {
        printf("efuse time out\n");
    }

    return val;
}

bool is_efuse_addr_valid(uint8_t base_idx, uint32_t addr)
{
    bool val = true;

    if (addr & EFUSE_ADDR_WORD_MASK) {
        //address [0] always set to 0
        //address [1] set to 0 when operate at 32 bits mode
        printf("efuse address is misaligned:\n");
        printf("address [0] always set to 0\n");
        printf("address [1] set to 0 when operate at 32 bits mode\n");
        val = false;
    }

    switch(base_idx){
    case EFUSE_BASE_MCU:
        if (addr >= 0x0010) {
            val = false;
            printf("Address is exceed MCU EFUSE limitation.\n");
        }    
        break;
    case EFUSE_BASE_NFC:
        //no limitation
        break;
    default:
        break;
    }

    return val;
}

HAL_STATUS efuse_read_er8130(uint8_t base_idx, uint32_t addr, uint8_t *buf)
{
    uint32_t val = 0;

    if(!is_efuse_addr_valid(base_idx, addr))
        return HAL_ERR;

    if (wait_for_efuse_idle(base_idx))
        return HAL_ERR;
    
    set_efuse_ctl_cfg(base_idx, EFUSE_CMD_READ_WORD, addr);
    
    if (wait_for_efuse_idle(base_idx))
        return HAL_ERR;

    val = _efuse_read_word(base_idx);

    memcpy(buf, &val, sizeof(uint32_t));

    return HAL_NO_ERR;
}

HAL_STATUS efuse_write_er8130(uint8_t base_idx, uint32_t addr, uint8_t *buf)
{
    uint32_t val = 0;

    if(!is_efuse_addr_valid(base_idx, addr))
        return HAL_ERR;
    
    if (wait_for_efuse_idle(base_idx))
        return HAL_ERR;

    memcpy(&val, buf, sizeof(uint32_t));

    //it needs to write data at first
    _efuse_write_word(base_idx, val);

    //then set efuse config
    set_efuse_ctl_cfg(base_idx, EFUSE_CMD_WRITE_WORD, addr);
    
    if (wait_for_efuse_idle(base_idx))
        return HAL_ERR;

    return HAL_NO_ERR;
}

void efuse_get_chip_unique_er8130(uint8_t* id_arr)
{
    uint32_t unique_id[3] = {0};

    efuse_read_er8130(EFUSE_BASE_MCU, EFUSE_UNIQUE_ID0_ADDR, (uint8_t*)&unique_id[0]);
    efuse_read_er8130(EFUSE_BASE_MCU, EFUSE_UNIQUE_ID1_ADDR, (uint8_t*)&unique_id[1]);
    efuse_read_er8130(EFUSE_BASE_MCU, EFUSE_UNIQUE_ID2_ADDR, (uint8_t*)&unique_id[2]);

    //adjustment
    unique_id[0] = ((unique_id[0] >> 16) & 0xFFFF) | ((unique_id[1] & 0xFFFF) << 16);
    unique_id[1] = ((unique_id[1] >> 16) & 0xFFFF) | ((unique_id[2] & 0xFFFF) << 16);

    // chip id has 8-byte
    memcpy(id_arr, unique_id, sizeof(uint32_t)*2);
}


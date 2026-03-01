#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t mod_idx;
    const uint32_t (*config)[2];
    uint32_t config_size;
    const uint32_t (*lpf)[2];
} MODIDX_CSR_CFG;

typedef struct {
    uint32_t data_rate;
    MODIDX_CSR_CFG *modidx_cfgs;
    size_t mod_count;
} MODEM_CFG_GRP;

typedef struct {
    uint8_t wmbus_mode;
    uint32_t data_rate;
    MODIDX_CSR_CFG *modidx_cfgs;
    size_t mod_count;
} MODEM_CFG_GRP_WMBUS;

extern MODEM_CFG_GRP modem_config_group_normal[7];
extern MODEM_CFG_GRP_WMBUS modem_config_group_wmbus[6];
extern const uint32_t bbp_basic_setting[9][2];
extern const uint32_t bbp_gain_table[35][2];

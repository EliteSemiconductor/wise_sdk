#ifndef WMBUS_DTM_H
#define WMBUS_DTM_H

#include <stdint.h>

#define WMBUS_DTM_MIN_FRAME_LENGTH 10U
#define WMBUS_DTM_MAX_FRAME_LENGTH 256U

typedef enum {
    WMBUS_DTM_STATE_UNINITIALIZED = 0,
    WMBUS_DTM_STATE_READY,
    WMBUS_DTM_STATE_TX,
    WMBUS_DTM_STATE_RX
} WMBUS_DTM_state_t;

typedef enum {
    WMBUS_DTM_PATTERN_AA = 0,
    WMBUS_DTM_PATTERN_55,
    WMBUS_DTM_PATTERN_0F,
    WMBUS_DTM_PATTERN_F0,
    WMBUS_DTM_PATTERN_FF,
    WMBUS_DTM_PATTERN_00,
    WMBUS_DTM_PATTERN_COUNT
} WMBUS_DTM_pattern_t;

typedef struct {
    uint32_t tx_requested;
    uint32_t tx_accepted;
    uint32_t tx_done;
    uint32_t tx_error;
    uint32_t tx_rejected;
    uint32_t rx_good;
    uint32_t rx_error;
    uint32_t rx_format_error;
    uint32_t rx_pattern_error;
    uint32_t rx_sequence_lost;
    uint32_t rx_sequence_error;
    int16_t last_rssi;
} WMBUS_DTM_statistics_t;

typedef struct {
    WMBUS_DTM_state_t state;
    uint8_t role;
    uint8_t mode;
    uint16_t tx_frame_length;
    WMBUS_DTM_pattern_t tx_pattern;
    uint32_t tx_interval_ms;
    WMBUS_DTM_statistics_t statistics;
} WMBUS_DTM_status_t;

int8_t wmbus_link_DTM_system_init(void);
int8_t wmbus_link_DTM_init(uint8_t role, uint8_t mode);
int8_t wmbus_link_DTM_deinit(void);
int8_t wmbus_link_DTM_tx(uint16_t frame_length,
                         WMBUS_DTM_pattern_t pattern,
                         uint32_t count,
                         uint32_t interval_ms);
int8_t wmbus_link_DTM_rx(uint8_t on);
int8_t wmbus_link_DTM_stop(void);
int8_t wmbus_link_DTM_reset(void);
void wmbus_link_DTM_get_status(WMBUS_DTM_status_t *status);
const char *wmbus_link_DTM_get_state_name(WMBUS_DTM_state_t state);
const char *wmbus_link_DTM_get_role_name(uint8_t role);
const char *wmbus_link_DTM_get_mode_name(uint8_t mode);
const char *wmbus_link_DTM_get_pattern_name(WMBUS_DTM_pattern_t pattern);

#endif /* WMBUS_DTM_H */

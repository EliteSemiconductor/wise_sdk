#ifndef WMBUS_CONNECTION_H
#define WMBUS_CONNECTION_H

#include <stdint.h>
#include <stdbool.h>

#include "wmbus_datalink_ring_buffer.h"
#include "wmbus_datalink_dll.h"

#define MAX_WMBUS_CONNECTIONS 5

typedef enum {
    WMBUS_TX_IN_FLIGHT_NONE = 0,
    WMBUS_TX_IN_FLIGHT_DATA,
    WMBUS_TX_IN_FLIGHT_NULL_GENERAL
} wmbus_tx_in_flight_type_t;

// refer to WMBUS_dll_header_t
typedef struct {
    uint32_t id;                   // Address, device ID
    uint16_t manufacturer;         // Address, manufacturer ID
    uint8_t version;               // Address, version
    uint8_t device_type;           // Address, device type

    wmbus_ring_buffer_t tx_queue;  // queues of Tx buffer
    uint8_t *rx_buffer;
    uint16_t rx_length;

    uint16_t tx_in_flight_length;
    uint8_t *tx_in_flight_buffer;
    wmbus_tx_in_flight_type_t tx_in_flight_type;

    uint16_t tx_next_length;
    uint8_t *tx_next_buffer;
    wmbus_tx_in_flight_type_t tx_next_type;

    uint16_t tx_nke_in_flight_length;
    uint8_t *tx_nke_in_flight_buffer;

    uint32_t connection_sequence;
    uint8_t acc_mismatch_retry_count;
} wmbus_connection_t;

// WMBus Connection Table
typedef struct {
    wmbus_connection_t *connections;
    uint16_t count;	//used number
    uint16_t max_connections;
    uint16_t ring_buffer_max;
    uint16_t buffer_max_len;
} wmbus_connection_table_t;


void wmbus_link_init_connection_table(uint16_t max_connections, uint16_t ring_buffer_max, uint16_t buffer_max_len);
bool wmbus_link_add_connection(WMBUS_Address *dev_info_p);
bool wmbus_link_remove_connection(uint32_t device_id);
void wmbus_link_notify_connection_failure(WMBUS_connection_failure_t reason,
                                          const wmbus_connection_t *connection);
wmbus_connection_t* wmbus_link_find_connection(wmbus_connection_t *new_conn);
wmbus_connection_t* wmbus_link_find_connection_by_id(uint32_t id);
wmbus_connection_t* wmbus_link_peek_connection_by_id(uint32_t id);
//only supported for GW
void wmbus_link_find_tx_queue_by_id(uint32_t id);
void wmbus_link_use_default_tx_queue(void);
void wmbus_link_free_connection_table(void);
void wmbus_link_print_connections(void);

#endif  // WMBUS_CONNECTION_H

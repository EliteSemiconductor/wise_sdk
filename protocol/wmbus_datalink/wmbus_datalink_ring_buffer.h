#ifndef WMBUS_RING_BUFFER_H
#define WMBUS_RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define PARTITION 4
#define EACH_SIZE 256

typedef struct {
//    uint8_t buffer[PARTITION][EACH_SIZE];
//    uint16_t lengths[PARTITION];
    uint8_t **buffer;     // [ring_buffer_max][buffer_max_len]
    uint16_t *lengths;    // packet length
    uint8_t head;	//index for writing
    uint8_t tail;	//index for reading
    uint8_t count;
    uint8_t reserved;
} wmbus_ring_buffer_t;

void wmbus_link_ring_buffer_init(uint16_t ring_buffer_max, uint16_t buffer_max_len);
bool wmbus_link_ring_buffer_is_empty(void);
bool wmbus_link_ring_buffer_is_full(void);
bool wmbus_link_ring_buffer_write(const uint8_t *data, uint16_t length);
bool wmbus_link_ring_buffer_read_clear(uint8_t *data, uint16_t *length);
bool wmbus_link_ring_buffer_read(uint8_t *data, uint16_t *length);
bool wmbus_link_ring_buffer_remove(void);
void wmbus_link_ring_buffer_free(void);
void wmbus_link_dump_ring_buffer_data(void);

#endif // WMBUS_RING_BUFFER_H


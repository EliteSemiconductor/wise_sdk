#ifndef WMBUS_DTM_H
#define WMBUS_DTM_H

#include <stdint.h>

typedef struct
{
    // Type (0: RX | 1: TX)
    uint8_t type;

    // RF channel
    uint8_t channel;

    // Length of test data
    uint8_t data_len;

    /* Packet payload
     * 0x00 Repeated "10101010"
     * 0x01 Repeated "01010101"
     * 0x02 Repeated "00001111"
     * 0x03 Repeated "11110000"
     * 0x04 Repeated "11111111"
     * 0x05 Repeated "00000000"
     * 0x08-0xFF Reserved for future use
     */
    uint8_t payload;

}wmbus_dtm_params_t;

//Transmitter test Packet Payload Type
enum
{
    //10101010 TX test payload type
    PAYL_10101010            = 0x00,
    //01010101 TX test payload type
    PAYL_01010101,
    //00001111 TX test payload type
    PAYL_00001111,
    //11110000 TX test payload type
    PAYL_11110000,
    //All 1s TX test payload type
    PAYL_ALL_1,
    //All 0s TX test payload type
    PAYL_ALL_0,
};

void wmbus_link_DTM_init(uint8_t role, uint8_t mode);
void wmbus_link_DTM_tx(uint8_t data_len, uint8_t data_type, uint32_t count, uint32_t interval);
void wmbus_link_DTM_rx(uint8_t on);



#endif // WMBUS_DTM_H

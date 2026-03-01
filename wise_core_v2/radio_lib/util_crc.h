#ifndef __UTIL_CRC_H 
#define __UTIL_CRC_H

#include <stdint.h>

#define UTIL_CRC_OK                 (0)
#define UTIL_CRC_FAIL               (-1)

typedef enum 
{
    //type                              poly        RefIn           RefOut
    CRC_TYPE_8_CCITT = 0,           //  0x07        0               0           
    CRC_TYPE_8_ROHC,                //  0x07        1               1
    CRC_TYPE_8_CDMA2000,            //  0x9B        0               0
    CRC_TYPE_8_EBU,                 //  0x1D        1               1
    CRC_TYPE_8_I_CODE,              //  0x1D        0               0
    CRC_TYPE_8_DARC,                //  0x39        1               1
    CRC_TYPE_8_DVB_S2,              //  0xD5        0               0  
    CRC_TYPE_8_MAXIM,               //  0x31        1               1
    CRC_TYPE_8_CRC8,                //  0x31        0               0
    CRC_TYPE_8_NUM,
} UTIL_CRC8_TYPE_T;

#define CRC_TYPE_8_ITU              CRC_TYPE_8_CCITT

typedef enum
{
    //type                              poly        in_reflect      out_reflect
    CRC_TYPE_16_CCITT,              //  0x1021      0               0
    CRC_TYPE_16_MCRF4XX,            //  0x1021      1               1
    CRC_TYPE_16_BUYPASS,            //  0x8005      0               0
    CRC_TYPE_16_ARC,                //  0x8005      1               1
    CRC_TYPE_16_DECT_R,             //  0x0589      0               0
    CRC_TYPE_16_EN_13757,           //  0x3D65      0               0
    CRC_TYPE_16_DNP,                //  0x3D65      1               1
    CRC_TYPE_16_CDMA2000,           //  0xC867      0               0
    CRC_TYPE_16_TELEDISK,           //  0xA097      0               0
    CRC_TYPE_16_NUM,
} UTIL_CRC16_TYPE_T;

#define CRC_TYPE_16_CRC16           CRC_TYPE_16_ARC
#define CRC_TYPE_16_IBM             CRC_TYPE_16_ARC
#define CRC_TYPE_16_AUG_CCITT       CRC_TYPE_16_CCITT
#define CRC_TYPE_16_GENIBUS         CRC_TYPE_16_CCITT
#define CRC_TYPE_16_XMODEM          CRC_TYPE_16_CCITT
#define CRC_TYPE_16_RIELLO          CRC_TYPE_16_MCRF4XX
#define CRC_TYPE_16_TMS37157        CRC_TYPE_16_MCRF4XX
#define CRC_TYPE_16_A               CRC_TYPE_16_MCRF4XX
#define CRC_TYPE_16_KERMIT          CRC_TYPE_16_MCRF4XX
#define CRC_TYPE_16_X25             CRC_TYPE_16_MCRF4XX
#define CRC_TYPE_16_DDS_110         CRC_TYPE_16_BUYPASS
#define CRC_TYPE_16_MAXIM           CRC_TYPE_16_ARC
#define CRC_TYPE_16_USB             CRC_TYPE_16_ARC
#define CRC_TYPE_16_MODBUS          CRC_TYPE_16_ARC
#define CRC_TYPE_16_DECT_X          CRC_TYPE_16_DECT_R

#define CRC_8_TABLE_SIZE            (256)
#define CRC_16_TABLE_SIZE           (512)

//for CRC 8 algorithm, table size should be (sizeof(uint8_t) * 256)
int8_t util_crc8_gen_table(uint8_t crcType, uint8_t *table);
uint8_t util_crc8_calc(uint8_t crcType, uint8_t *crcTable, uint8_t *data, uint32_t length, uint8_t startVal);
uint8_t bit_reverse_8(uint8_t value);

//for CRC 16 algorithm, table size should be (sizeof(uint16_t) * 256)
int8_t util_crc16_gen_table(uint8_t crcType, uint16_t *table);
uint16_t util_crc16_calc(uint8_t crcType, uint16_t *crcTable, uint8_t *data, uint32_t length, uint16_t startVal);
uint16_t bit_reverse_16(uint16_t value);
uint16_t util_crc16_get_poly(uint8_t crcType);

void util_crc_list();

#endif

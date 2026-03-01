#ifndef __UTIL_BITSTREAM_CODEC_H 
#define __UTIL_BITSTREAM_CODEC_H

#include <stdint.h>

enum
{
    BS_TYPE_NRZ = 0,
    BS_TYPE_MANCHESTER,
    BS_TYPE_3_OF_6,
};


int32_t util_bitstream_encode(uint8_t type, const uint8_t *input, uint32_t input_len, uint8_t *output_bits, uint32_t *output_len);
int32_t util_bitstream_decode(uint8_t type, const uint8_t *input_bits, uint32_t input_len, uint8_t *output);
void verify_codec(void);

#endif


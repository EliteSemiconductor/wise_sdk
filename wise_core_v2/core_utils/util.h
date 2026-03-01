/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file util.h
 * @brief Generic utility macros and helper functions.
 *
 * @ingroup WISE_UTIL
 *
 * This header provides byte-order conversion macros, CRC helpers,
 * simple pseudo-random number generation, and debug utilities.
 */

#ifndef __UTIL_H
#define __UTIL_H

#include "platform/hal_intf/types.h"
#include <stdint.h>

/**
 * @defgroup WISE_UTIL WISE Utility APIs
 * @ingroup WISE_CORE_API
 * @brief Byte-order helpers, CRC utilities, simple RNG and debug helpers.
 * @{
 */

/** @name Little-endian store helpers
 *  @brief Store integer values to memory in little-endian order.
 *  @{
 */

/**
 * @brief Store a 16-bit value in little-endian format.
 *
 * @param p Pointer to at least 2 bytes of writable memory.
 * @param v 16-bit value to be stored.
 */
#define PUT_LE_16(p, v)                                                         \
    do {                                                                        \
        ((uint8_t *)p)[1] = (v >> 8) & 0xff;                                    \
        ((uint8_t *)p)[0] = v & 0xff;                                           \
    } while (0)

/**
 * @brief Store a 24-bit value in little-endian format.
 *
 * Only the lower 24 bits of @p v are stored.
 *
 * @param p Pointer to at least 3 bytes of writable memory.
 * @param v 24-bit value (in low 24 bits) to be stored.
 */
#define PUT_LE_24(p, v)                                                         \
    do {                                                                        \
        ((uint8_t *)p)[2] = (v >> 16) & 0xff;                                   \
        ((uint8_t *)p)[1] = (v >> 8) & 0xff;                                    \
        ((uint8_t *)p)[0] = v & 0xff;                                           \
    } while (0)

/**
 * @brief Store a 32-bit value in little-endian format.
 *
 * @param p Pointer to at least 4 bytes of writable memory.
 * @param v 32-bit value to be stored.
 */
#define PUT_LE_32(p, v)                                                         \
    do {                                                                        \
        ((uint8_t *)p)[3] = (v >> 24) & 0xff;                                   \
        ((uint8_t *)p)[2] = (v >> 16) & 0xff;                                   \
        ((uint8_t *)p)[1] = (v >> 8) & 0xff;                                    \
        ((uint8_t *)p)[0] = v & 0xff;                                           \
    } while (0)

/** @} */

/** @name Big-endian store helpers
 *  @brief Store integer values to memory in big-endian order.
 *  @{
 */

/**
 * @brief Store a 16-bit value in big-endian format.
 *
 * @param p Pointer to at least 2 bytes of writable memory.
 * @param v 16-bit value to be stored.
 */
#define PUT_BE_16(p, v)                                                         \
        do {                                                                    \
            ((uint8_t *)p)[0] = (v >> 8) & 0xff;                                \
            ((uint8_t *)p)[1] = v & 0xff;                                       \
        } while (0)
    
/**
 * @brief Store a 24-bit value in big-endian format.
 *
 * Only the lower 24 bits of @p v are stored.
 *
 * @param p Pointer to at least 3 bytes of writable memory.
 * @param v 24-bit value (in low 24 bits) to be stored.
 */
#define PUT_BE_24(p, v)                                                         \
        do {                                                                    \
            ((uint8_t *)p)[0] = (v >> 16) & 0xff;                               \
            ((uint8_t *)p)[1] = (v >> 8) & 0xff;                                \
            ((uint8_t *)p)[2] = v & 0xff;                                       \
        } while (0)
    
/**
 * @brief Store a 32-bit value in big-endian format.
 *
 * @param p Pointer to at least 4 bytes of writable memory.
 * @param v 32-bit value to be stored.
 */
#define PUT_BE_32(p, v)                                                         \
        do {                                                                    \
            ((uint8_t *)p)[0] = (v >> 24) & 0xff;                               \
            ((uint8_t *)p)[1] = (v >> 16) & 0xff;                               \
            ((uint8_t *)p)[2] = (v >> 8) & 0xff;                                \
            ((uint8_t *)p)[3] = v & 0xff;                                       \
        } while (0)

/** @} */

/** @name Big-endian load helpers
 *  @brief Load integer values from memory in big-endian order.
 *  @{
 */

/**
 * @brief Load a 32-bit big-endian value from memory.
 *
 * @param x Pointer to at least 4 bytes of readable memory.
 *
 * @return 32-bit value interpreted as big-endian.
 */
#define LOAD_BE_32(x) ((uint32_t)((((uint8_t *)x)[0] << 24) | (((uint8_t *)x)[1] << 16) | (((uint8_t *)x)[2] << 8) | ((uint8_t *)x)[3]))
                                                
/**
 * @brief Load a 24-bit big-endian value from memory.
 *
 * @param x Pointer to at least 3 bytes of readable memory.
 *
 * @return 24-bit value (in low 24 bits) interpreted as big-endian.
 */
#define LOAD_BE_24(x) ((uint32_t)((((uint8_t *)x)[0] << 16) | (((uint8_t *)x)[1] << 8) | ((uint8_t *)x)[2]))
                                                
/**
 * @brief Load a 16-bit big-endian value from memory.
 *
 * @param x Pointer to at least 2 bytes of readable memory.
 *
 * @return 16-bit value interpreted as big-endian.
 */
#define LOAD_BE_16(x) ((uint16_t)((((uint8_t *)x)[0] << 8) | ((uint8_t *)x)[1]))
                                                
/** @} */

/** @name Little-endian load helpers
 *  @brief Load integer values from memory in little-endian order.
 *  @{
 */

/**
 * @brief Load a 32-bit little-endian value from memory.
 *
 * @param x Pointer to at least 4 bytes of readable memory.
 *
 * @return 32-bit value interpreted as little-endian.
 */
#define LOAD_LE_32(x) ((uint32_t)((((uint8_t *)x)[3] << 24) | (((uint8_t *)x)[2] << 16) | (((uint8_t *)x)[1] << 8) | ((uint8_t *)x)[0]))
                                                
/**
 * @brief Load a 16-bit little-endian value from memory.
 *
 * @param x Pointer to at least 2 bytes of readable memory.
 *
 * @return 16-bit value interpreted as little-endian.
 */
#define LOAD_LE_16(x) ((uint16_t)((((uint8_t *)x)[1] << 8) | ((uint8_t *)x)[0]))
                                                
/** @} */

/**
 * @brief Swap low and high bytes of a 16-bit value.
 *
 * @param x 16-bit value to be byte-swapped.
 *
 * @return Value with byte order reversed.
 */
#define SWAP_BYTE(x) ((((uint16_t)(x) >> 8) & 0xff) | (((uint16_t)(x) & 0xff) << 8))

/**
 * @brief Safely free a pointer and set it to NULL.
 *
 * If @p buf is non-NULL, it is freed and then set to NULL.
 *
 * @param buf Pointer variable to be freed.
 */
#define SAFE_RELEASE(buf)                   do {                                \
                                                if(buf) free(buf);              \
                                                buf = NULL;                     \
                                            } while(0)

/**
 * @brief Assert on allocation failure and halt execution.
 *
 * Prints a message and loops forever. Intended for fatal allocation failures.
 *
 * @param msg Additional message string to print.
 */
#define ASSERT_ALLOC_FAIL(msg)              do{debug_print("memory failed: "); debug_print(msg); while(1);} while(0)


//uint8_t crc8_citt(const void *data, uint32_t size);
//uint8_t crc8_citt_byte(uint8_t start, uint8_t new);

/**
 * @brief Compute CRC-16/KERMIT over a buffer.
 *
 * @param[in] buf      Pointer to input buffer.
 * @param[in] length   Number of bytes in @p buf.
 * @param[in] startVal Initial CRC value.
 *
 * @return Computed 16-bit CRC.
 */
uint16_t crc16_kermit(uint8_t *buf, int length, uint16_t startVal);

/**
 * @brief Reverse bits within the given number of bytes.
 *
 * Reverses the bit order of the least significant @p bytes of @p val.
 *
 * @param[in] val   Input value.
 * @param[in] bytes Number of bytes (1–4) to be bit-reversed.
 *
 * @return Value with bits reversed within the specified byte width.
 */
uint32_t bit_reverse(uint32_t val, uint8_t bytes);

/**
 * @brief Initialize pseudo-random number generator.
 *
 * @param[in] x Seed value.
 */
void init_rand(uint32_t x);

/**
 * @brief Generate a pseudo-random number.
 *
 * @return Pseudo-random 32-bit value.
 */
uint32_t rand_cmwc(void);

/**
 * @brief Debug printing macro.
 *
 * Wraps @c printf and @c fflush(stdout). Enabled unconditionally.
 *
 * @param ... Format string and arguments (printf-style).
 */
#define debug_print(...)                                                        \
    do {                                                                        \
        if (1) {                                                                \
            printf(__VA_ARGS__);                                                \
            fflush(stdout);                                                     \
        }                                                                       \
    } while (0)

/**
 * @brief Dump a buffer in human-readable form.
 *
 * Typically prints the buffer content as hex bytes.
 *
 * @param[in] buffer Pointer to buffer.
 * @param[in] len    Buffer length in bytes.
 */
void dump_buffer(unsigned char *buffer, int len);

/**
 * @brief Print an integer array.
 *
 * @param[in] arr Pointer to array of integers.
 * @param[in] n   Number of elements in the array.
 */
void print_array(int arr[], int n);

/**
 * @brief Convert big-endian 32-bit words to little-endian order.
 *
 * Copies @p byte_len bytes from @p src to @p dst while swapping
 * the byte order of each 32-bit word.
 *
 * @param[out] dst      Destination buffer.
 * @param[in]  src      Source buffer containing big-endian words.
 * @param[in]  byte_len Number of bytes to process (multiple of 4).
 */
void wise_swap_be32_to_le32(uint8_t *dst, const uint8_t *src, uint32_t byte_len);

/** @} */ /* end of WISE_UTIL group */

#endif /* __UTIL_H */

/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_basic_types.h
 * @brief Basic type definitions and macros for the system.
 *
 * This header file defines fundamental types, macros, and utility functions
 * used throughout the system. It includes type definitions for unsigned and
 * signed integers of various sizes, as well as bit manipulation macros. The
 * file also provides utilities for accessing and manipulating hardware I/O
 * ports and basic constants for system configurations.
 */
#ifndef __TYPES_H_
#define __TYPES_H_

#include <stdint.h>

/** @def TRUE
 *  @brief Boolean true value.
 */
#define TRUE (1UL)

/** @def FALSE
 *  @brief Boolean false value.
 */
#define FALSE (0UL)

/** @def ENABLE
 *  @brief Value indicating an enabled state.
 */
#define ENABLE (1UL)

/** @def DISABLE
 *  @brief Value indicating a disabled state.
 */
#define DISABLE (0UL)

/** @def NONE
 *  @brief Value indicating no selection or option.
 */
#define NONE (0UL)

#ifndef NULL
/** @def NULL
 *  @brief Null pointer constant.
 */
#define NULL ((void *)0)
#endif

/** @def BIT
 *  @brief Creates a bitmask for a given bit position.
 *  @param x Bit position.
 *  @return Bitmask for the specified bit position.
 */
#define BIT(x) (1UL << (x))

/**
 * @def BIT0
 * @brief Bitmask definition for bit position 0.
 */
#define BIT0 (0x00000001UL)

/**
 * @def BIT1
 * @brief Bitmask definition for bit position 1.
 */
#define BIT1 (0x00000002UL)

/**
 * @def BIT2
 * @brief Bitmask definition for bit position 2.
 */
#define BIT2 (0x00000004UL)

/**
 * @def BIT3
 * @brief Bitmask definition for bit position 3.
 */
#define BIT3 (0x00000008UL)

/**
 * @def BIT4
 * @brief Bitmask definition for bit position 4.
 */
#define BIT4 (0x00000010UL)

/**
 * @def BIT5
 * @brief Bitmask definition for bit position 5.
 */
#define BIT5 (0x00000020UL)

/**
 * @def BIT6
 * @brief Bitmask definition for bit position 6.
 */
#define BIT6 (0x00000040UL)

/**
 * @def BIT7
 * @brief Bitmask definition for bit position 7.
 */
#define BIT7 (0x00000080UL)

/**
 * @def BIT8
 * @brief Bitmask definition for bit position 8.
 */
#define BIT8 (0x00000100UL)

/**
 * @def BIT9
 * @brief Bitmask definition for bit position 9.
 */
#define BIT9 (0x00000200UL)

/**
 * @def BIT10
 * @brief Bitmask definition for bit position 10.
 */
#define BIT10 (0x00000400UL)

/**
 * @def BIT11
 * @brief Bitmask definition for bit position 11.
 */
#define BIT11 (0x00000800UL)

/**
 * @def BIT12
 * @brief Bitmask definition for bit position 12.
 */
#define BIT12 (0x00001000UL)

/**
 * @def BIT13
 * @brief Bitmask definition for bit position 13.
 */
#define BIT13 (0x00002000UL)

/**
 * @def BIT14
 * @brief Bitmask definition for bit position 14.
 */
#define BIT14 (0x00004000UL)

/**
 * @def BIT15
 * @brief Bitmask definition for bit position 15.
 */
#define BIT15 (0x00008000UL)

/**
 * @def BIT16
 * @brief Bitmask definition for bit position 16.
 */
#define BIT16 (0x00010000UL)

/**
 * @def BIT17
 * @brief Bitmask definition for bit position 17.
 */
#define BIT17 (0x00020000UL)

/**
 * @def BIT18
 * @brief Bitmask definition for bit position 18.
 */
#define BIT18 (0x00040000UL)

/**
 * @def BIT19
 * @brief Bitmask definition for bit position 19.
 */
#define BIT19 (0x00080000UL)

/**
 * @def BIT20
 * @brief Bitmask definition for bit position 20.
 */
#define BIT20 (0x00100000UL)

/**
 * @def BIT21
 * @brief Bitmask definition for bit position 21.
 */
#define BIT21 (0x00200000UL)

/**
 * @def BIT22
 * @brief Bitmask definition for bit position 22.
 */
#define BIT22 (0x00400000UL)

/**
 * @def BIT23
 * @brief Bitmask definition for bit position 23.
 */
#define BIT23 (0x00800000UL)

/**
 * @def BIT24
 * @brief Bitmask definition for bit position 24.
 */
#define BIT24 (0x01000000UL)

/**
 * @def BIT25
 * @brief Bitmask definition for bit position 25.
 */
#define BIT25 (0x02000000UL)

/**
 * @def BIT26
 * @brief Bitmask definition for bit position 26.
 */
#define BIT26 (0x04000000UL)

/**
 * @def BIT27
 * @brief Bitmask definition for bit position 27.
 */
#define BIT27 (0x08000000UL)

/**
 * @def BIT28
 * @brief Bitmask definition for bit position 28.
 */
#define BIT28 (0x10000000UL)

/**
 * @def BIT29
 * @brief Bitmask definition for bit position 29.
 */
#define BIT29 (0x20000000UL)

/**
 * @def BIT30
 * @brief Bitmask definition for bit position 30.
 */
#define BIT30 (0x40000000UL)

/**
 * @def BIT31
 * @brief Bitmask definition for bit position 31.
 */
#define BIT31 (0x80000000UL)

/** @def BYTE0_MASK
 *  @brief Mask to extract byte 0 (bits 0~7) from a 32-bit integer.
 */
#define BYTE0_MASK (0x000000FFUL)

/** @def BYTE1_MASK
 *  @brief Mask to extract byte 1 (bits 8~15) from a 32-bit integer.
 */
#define BYTE1_MASK (0x0000FF00UL)

/** @def BYTE2_MASK
 *  @brief Mask to extract byte 2 (bits 16~23) from a 32-bit integer.
 */
#define BYTE2_MASK (0x00FF0000UL)

/** @def BYTE3_MASK
 *  @brief Mask to extract byte 3 (bits 24~31) from a 32-bit integer.
 */
#define BYTE3_MASK (0xFF000000UL)

/** \def GET_BYTE0
 *  @brief Extract byte 0 from a 32-bit integer.
 *  @param u32Param The 32-bit integer.
 *  @return The value of byte 0.
 */
#define GET_BYTE0(u32Param) (((u32Param) & BYTE0_MASK))

/** \def GET_BYTE1
 *  @brief Extract byte 1 from a 32-bit integer.
 *  @param u32Param The 32-bit integer.
 *  @return The value of byte 1.
 */
#define GET_BYTE1(u32Param) (((u32Param) & BYTE1_MASK) >> 8)

/** \def GET_BYTE2
 *  @brief Extract byte 2 from a 32-bit integer.
 *  @param u32Param The 32-bit integer.
 *  @return The value of byte 2.
 */
#define GET_BYTE2(u32Param) (((u32Param) & BYTE2_MASK) >> 16)

/** \def GET_BYTE3
 *  @brief Extract byte 3 from a 32-bit integer.
 *  @param u32Param The 32-bit integer.
 *  @return The value of byte 3.
 */
#define GET_BYTE3(u32Param) (((u32Param) & BYTE3_MASK) >> 24)

/** @} */ /* end of group IO_ROUTINE */

/** @def TRANS_TO_M
 *  @brief Converts a value to micro-units.
 *  @param data The value to be converted.
 *  @return The value converted to micro-units.
 */
#define TRANS_TO_M(data) (data * 1000000)

/** @def TRANS_TO_K
 *  @brief Converts a value to kilo-units.
 *  @param data The value to be converted.
 *  @return The value converted to kilo-units.
 */
#define TRANS_TO_K(data) (data * 1000)

#ifndef ARRAY_SIZE
/**
 * \def ARRAY_SIZE
 * @brief Computes the number of elements in an array.
 *
 * This macro calculates the number of elements in a statically allocated array.
 * It is typically used to determine the size of the array in terms of the
 * number of elements it contains.
 *
 * @param arr The array to compute the size of.
 *
 * @c ARRAY_SIZE(arr) will expand to the number of elements in the array `arr`.
 *
 * @note This macro assumes that `arr` is a statically allocated array and not
 * a pointer.
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

/**
 * \def time_diff
 * @brief Computes the difference between 2 time values.
 */
#define time_diff(start, end) ((uint32_t)((uint32_t)end - (uint32_t)start))

/**
 * @brief Set a 8-bit unsigned value to specified I/O port
 * @param[in] addr Port address to set 8-bit data
 * @param[in] value Value to write to I/O port
 * @return  None
 * @note The output port must be 8-bit aligned
 */
#define REG_W8(addr, value) *((volatile uint8_t *)(addr)) = (value)

/**
 * @brief Get a 8-bit unsigned value from specified I/O port
 * @param[in] addr Port address to get 8-bit data from
 * @return  8-bit unsigned value stored in specified I/O port
 * @note The input port must be 8-bit aligned
 */
#define REG_R8(addr) (*((volatile uint8_t *)(addr)))

/**
 * @brief Set a 16-bit unsigned value to specified I/O port
 * @param[in] addr Port address to set 16-bit data
 * @param[in] value Value to write to I/O port
 * @return  None
 * @note The output port must be 16-bit aligned
 */
#define REG_W16(addr, value) *((volatile uuint16_t *)(addr)) = (value)

/**
 * @brief Get a 16-bit unsigned value from specified I/O port
 * @param[in] addr Port address to get 16-bit data from
 * @return  16-bit unsigned value stored in specified I/O port
 * @note The input port must be 16-bit aligned
 */
#define REG_R16(addr) (*((volatile uint16_t *)(addr)))

/**
 * @brief Set a 32-bit unsigned value to specified I/O port
 * @param[in] addr Port address to set 32-bit data
 * @param[in] value Value to write to I/O port
 * @return  None
 * @note The output port must be 32-bit aligned
 */
#define REG_W32(addr, value) *((volatile uint32_t *)(addr)) = (value)

/**
 * @brief Get a 32-bit unsigned value from specified I/O port
 * @param[in] addr Port address to get 32-bit data from
 * @return  32-bit unsigned value stored in specified I/O port
 * @note The input port must be 32-bit aligned
 */
#define REG_R32(addr) (*((volatile uint32_t *)(addr)))

typedef int32_t HAL_STATUS;

#define HAL_NO_ERR (0) //< Operation succeeded
#define HAL_ERR (-1)   //< Operation failed

#define MIN(a, b) ((a) < (b) ? (a) : (b))
enum {
    INT_DISABLE = 0,
    INT_ENABLE  = 1,
    INT_INVALID = 2,
};

/**
 * @brief Callback function type definition.
 */
typedef void (*HAL_ISR_CALLBACK)(void *userData);
typedef void (*CALLBACK_T)(void *context, uint8_t idx);
typedef struct {
    CALLBACK_T callback;
    void *context;
} CALLBACK_ENTRY_T;

typedef void (*EVT_CALLBACK_T)(uint8_t idx, uint32_t event, void *context);
typedef struct {
    EVT_CALLBACK_T callback;
    void *context;
} EVT_CALLBACK_ENTRY_T;

#endif /* _WISE_BASIC_TYPES_H_ */

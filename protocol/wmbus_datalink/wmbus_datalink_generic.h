#ifndef __BLE_GENERIC_H__
#define __BLE_GENERIC_H__

#include <stdint.h>
#include <stdbool.h>

/// Fixed-width type indicating the needed alignment for RX and TX FIFOs.
#define FIFO_ALIGNMENT_TYPE uint32_t

/// Alignment that is needed for the RX and TX FIFOs.
#define FIFO_ALIGNMENT (sizeof(FIFO_ALIGNMENT_TYPE))

// Capacity of the TX FIFO
// <128=> 128
// <256=> 256
// <512=> 512
// <1024=> 1024
// <2048=> 2048
// <4096=> 4096
// <i> Default: 256
#define TX_FIFO_SIZE  512

// Capacity of the RX FIFO
// <128=> 128
// <256=> 256
// <512=> 512
// <1024=> 1024
// <2048=> 2048
// <4096=> 4096
// <i> Default: 256
#define RX_FIFO_SIZE  512

#endif


/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_trng_api.h
 * @brief True Random Number Generator (TRNG) driver APIs.
 *
 * @ingroup WISE_TRNG
 *
 * This header provides the interface for initializing, enabling, disabling,
 * and generating true random numbers using the hardware TRNG module.
 */


#ifndef __WISE_TRNG_API_H_
#define __WISE_TRNG_API_H_

#include "wise_core.h"
#include "api/wise_tick_api.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_pmu.h"
#include "hal_intf_trng.h"
#include "types.h"

/**
 * @defgroup WISE_TRNG WISE TRNG APIs
 * @ingroup WISE_CORE_API
 * @brief TRNG initialization and random number generation.
 * @{
 */

/** @brief Fixed block size returned by TRNG per hardware output (bytes). */
#define TRNG_BLOCK_SIZE 24 // bytes

/* ------------------------------------------------------------------------- */
/*                               API Functions                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Initialize the TRNG hardware.
 *
 * Must be called before enabling or generating random numbers. This function
 * configures TRNG registers to default operational settings.
 */
void wise_trng_init(void);

/**
 * @brief Enable the TRNG module.
 *
 * Activates the TRNG hardware so that random numbers can be generated.
 * Must be called after ::wise_trng_init().
 */
void wise_trng_enable(void);

/**
 * @brief Disable the TRNG module.
 *
 * Turns off the TRNG hardware. Call this when random number generation
 * is no longer required to reduce power consumption.
 */
void wise_trng_disable(void);

/**
 * @brief Generate random bytes.
 *
 * Produces @p len random bytes and writes them into @p buffer.
 * The buffer must be allocated by the caller with sufficient size.
 *
 * @param[out] buffer Pointer to the buffer where generated random bytes
 *                    will be stored.
 * @param[in]  len    Number of random bytes requested.
 */
void wise_trng_generate(uint8_t *buffer, uint32_t len);

/* ------------------------------------------------------------------------- */
/*                     (Optional Future Interface Placeholder)               */
/* ------------------------------------------------------------------------- */
/*
 * @brief Retrieve EHR (Entropy Harvesting Register) data from TRNG.
 *
 * @param[out] ehr_data Pointer to variable receiving EHR contents.
 *
 * void wise_trng_get_ehr_data(uint32_t *ehr_data);
 */


/** @} */ /* end of WISE_TRNG group */

#endif /* __WISE_TRNG_API_H_ */

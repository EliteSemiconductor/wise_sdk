/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_crypto_api.h
 * @brief Hardware crypto accelerator APIs (AES, SHA).
 *
 * @ingroup WISE_CRYPTO
 *
 * This header provides APIs to configure and use the hardware
 * cryptographic engine, including AES (ECB/CBC/CTR/CBC-MAC/CCM)
 * and SHA-224/SHA-256.
 */

#ifndef _WISE_CRYPTO_API_H_
#define _WISE_CRYPTO_API_H_

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "types.h"
#include "hal_intf_crypto.h"
#include "hal_intf_dma.h"

/**
 * @defgroup WISE_CRYPTO WISE Crypto APIs
 * @ingroup WISE_CORE_API
 * @brief Hardware crypto accelerator configuration and data processing.
 * @{
 */


/**
 * @enum AES_DIR_T
 * @brief AES operation direction.
 *
 * @ingroup WISE_CRYPTO
 */
typedef enum {
    AES_ENCRYPT,    /**< Encrypt operation. */
    AES_DECRYPT,    /**< Decrypt operation. */
} AES_DIR_T;

/**
 * @enum AES_KEY_SOURCE_T
 * @brief AES key source selection.
 *
 * @ingroup WISE_CRYPTO
 */
typedef enum {
    AES_KEY_SRC_FROM_EFUSE = 0, /**< Use key stored in eFuse/keystore. */
    AES_KEY_SRC_FROM_USER  = 1, /**< Use key provided by software. */
} AES_KEY_SOURCE_T;

/**
 * @enum AES_MODE_T
 * @brief AES operation mode.
 *
 * @ingroup WISE_CRYPTO
 */
typedef enum {
    AES_MODE_ECB,     /**< Electronic Codebook mode. */
    AES_MODE_CBC,     /**< Cipher Block Chaining mode. */
    AES_MODE_CTR,     /**< Counter mode. */
    AES_MODE_CBC_MAC, /**< CBC-MAC authentication mode. */
    AES_MODE_CCM,     /**< Counter with CBC-MAC (CCM) authenticated encryption. */
} AES_MODE_T;

/**
 * @enum AES_TRIG_MODE_T
 * @brief AES DMA trigger modes.
 *
 * Used internally to control the DMA channel that feeds or reads the AES engine.
 *
 * @ingroup WISE_CRYPTO
 */
typedef enum {
    AES_DMA_DATA_TRIG,      /**< Trigger by AES data input/output. */
    AES_DMA_AUTH_TRIG,      /**< Trigger by authentication data path. */
    AES_DMA_WRITE_KEY_TRIG, /**< Trigger when writing keys to AES. */
    AES_DMA_READ_KEY_TRIG   /**< Trigger when reading keys from AES. */
} AES_TRIG_MODE_T;

/**
 * @enum AES_KEY_SIZE_T
 * @brief AES key size selection.
 *
 * @ingroup WISE_CRYPTO
 */
typedef enum {
    AES_KEY_128, /**< 128-bit AES key. */
    AES_KEY_192, /**< 192-bit AES key. */
    AES_KEY_256, /**< 256-bit AES key. */
} AES_KEY_SIZE_T;

/**
 * @enum AES_SWAP_T
 * @brief Input/Output swap control for AES data.
 *
 * @ingroup WISE_CRYPTO
 */
typedef enum {
    AES_SWAP_NONE   = 0, /**< No swap applied. */
    AES_SWAP_INPUT  = 1, /**< Swap input data only. */
    AES_SWAP_OUTPUT = 2, /**< Swap output data only. */
    AES_SWAP_IN_OUT = 3, /**< Swap both input and output data. */
} AES_SWAP_T;

/**
 * @struct WISE_AES_KEY_CONFIG_T
 * @brief AES key and IV/Counter configuration.
 *
 * Describes key source, key data or index, swap mode, and initial
 * IV/counter where required by the AES mode.
 *
 * @ingroup WISE_CRYPTO
 */
typedef struct {
    AES_KEY_SIZE_T key_size;  /**< AES key size selection. */
    AES_KEY_SOURCE_T key_src; /**< Source of key material. */
    const uint8_t *key_bytes; /**< Pointer to key bytes (if @ref AES_KEY_SRC_FROM_USER), NULL otherwise. */
    uint32_t key_num;         /**< Keystore key index (when using stored key). */
    AES_SWAP_T swap_mode;     /**< Data swap behavior for AES engine. */
    const uint8_t *iv_or_cnt; /**< IV or counter value for modes that require it. */
} WISE_AES_KEY_CONFIG_T;

/**
 * @struct WISE_AES_DATA_T
 * @brief AES input/output buffer description.
 *
 * Describes the buffer for AES input and output data.
 *
 * @ingroup WISE_CRYPTO
 */
typedef struct {
    const uint8_t *input;       /**< Pointer to input buffer. */
    uint8_t *output;            /**< Pointer to output buffer. */
    uint32_t length;            /**< Data length in bytes (must be multiple of 16 for block modes). */
    const uint8_t *iv_or_cnt;   /**< Optional IV or counter (may override key config). */
} WISE_AES_DATA_T;

/**
 * @struct WISE_AES_CCM_EXTRA_T
 * @brief Additional CCM parameters.
 *
 * Contains nonce and additional authenticated data used by AES-CCM.
 *
 * @ingroup WISE_CRYPTO
 */
typedef struct {
    const uint8_t *nonce;       /**< Pointer to CCM nonce. */
    uint8_t nonce_len;          /**< Length of nonce in bytes. */
    const uint8_t *auth_data;   /**< Pointer to additional authenticated data (AAD). */
    uint32_t auth_len;          /**< Length of AAD in bytes. */
    uint8_t tag_len;            /**< Desired authentication tag length in bytes. */
} WISE_AES_CCM_EXTRA_T;

/**
 * @struct WISE_AES_CONFIG_CTX_T
 * @brief AES configuration context for a single operation.
 *
 * Encapsulates mode, direction, and data buffers for one AES run.
 *
 * @ingroup WISE_CRYPTO
 */
typedef struct {
    AES_MODE_T aes_mode;        /**< AES mode of operation. */
    AES_DIR_T aes_direction;    /**< Encrypt or decrypt. */
    WISE_AES_DATA_T data_cfg;   /**< Data buffer configuration. */
} WISE_AES_CONFIG_CTX_T;

/**
 * @struct WISE_AES_CCM_CTX_T
 * @brief AES-CCM context.
 *
 * Describes parameters and state for AES-CCM authentication and encryption.
 *
 * @ingroup WISE_CRYPTO
 */
typedef struct {
    WISE_AES_KEY_CONFIG_T key_cfg;  /**< AES key configuration. */
    WISE_AES_CCM_EXTRA_T ccm_cfg;   /**< CCM-specific nonce and AAD. */
    uint32_t total_payload_len;     /**< Total payload length for CCM processing. */
    bool is_encrypt;                /**< true for encrypt+tag, false for decrypt+verify. */
    HAL_DMA_DATA_CFG_T auth_cfg;    /**< DMA configuration for authentication path. */
    bool has_auth;                  /**< Indicates if authentication data is present. */
} WISE_AES_CCM_CTX_T;

// ===== Common API =====
WISE_STATUS wise_crypto_init();
void wise_crypto_deinit();

// ===== AES =====
/**
 * @brief Initialize the crypto subsystem.
 *
 * Must be called before using any other crypto APIs.
 *
 * @retval ::WISE_SUCCESS Crypto engine initialized successfully.
 * @retval ::WISE_FAIL    Initialization failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_crypto_init();

/**
 * @brief Deinitialize the crypto subsystem.
 *
 * Releases internal resources used by the crypto engine.
 *
 * @ingroup WISE_CRYPTO
 */
void wise_crypto_deinit(void);

/* ===== AES ===== */

/**
 * @brief Configure AES key and IV/counter.
 *
 * Programs key material and related parameters into the hardware AES engine.
 *
 * @param[in] key_cfg Pointer to key configuration structure.
 *
 * @retval ::WISE_SUCCESS Key configuration applied successfully.
 * @retval ::WISE_FAIL    Configuration failed (invalid parameters or HW error).
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_key_config(const WISE_AES_KEY_CONFIG_T *key_cfg);

/**
 * @brief Execute a generic AES operation.
 *
 * Uses the previously configured key and the mode/direction specified
 * in the context to process the provided data.
 *
 * @param[in,out] ctx Pointer to AES configuration context.
 *
 * @retval ::WISE_SUCCESS Operation completed successfully.
 * @retval ::WISE_FAIL    Operation failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_exec(WISE_AES_CONFIG_CTX_T *ctx);

/**
 * @brief Configure AES-CCM context.
 *
 * Programs key, nonce, AAD, and other CCM parameters into the hardware.
 *
 * @param[in,out] ctx Pointer to AES-CCM context structure.
 *
 * @retval ::WISE_SUCCESS Configuration successful.
 * @retval ::WISE_FAIL    Configuration failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_ccm_configure(WISE_AES_CCM_CTX_T *ctx);

/**
 * @brief Execute AES-CCM with pre-configured context.
 *
 * @param[in,out] ctx  Pointer to initialized AES-CCM context.
 * @param[in]     data Pointer to data buffer description.
 *
 * @retval ::WISE_SUCCESS Operation completed successfully.
 * @retval ::WISE_FAIL    Operation or authentication failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_ccm_exec(WISE_AES_CCM_CTX_T *ctx, const WISE_AES_DATA_T *data);

/**
 * @brief Perform AES-ECB encryption.
 *
 * One-shot helper that configures the key and runs ECB encryption.
 *
 * @param[in]  key_cfg Pointer to AES key configuration.
 * @param[in]  data    Pointer to AES data buffers.
 *
 * @retval ::WISE_SUCCESS Encryption completed successfully.
 * @retval ::WISE_FAIL    Encryption failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_ecb_encrypt(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data);

/**
 * @brief Perform AES-ECB decryption.
 *
 * @param[in]  key_cfg Pointer to AES key configuration.
 * @param[in]  data    Pointer to AES data buffers.
 *
 * @retval ::WISE_SUCCESS Decryption completed successfully.
 * @retval ::WISE_FAIL    Decryption failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_ecb_decrypt(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data);

/**
 * @brief Perform AES-CBC encryption.
 *
 * @param[in]  key_cfg Pointer to AES key configuration (includes IV).
 * @param[in]  data    Pointer to AES data buffers.
 *
 * @retval ::WISE_SUCCESS Encryption completed successfully.
 * @retval ::WISE_FAIL    Encryption failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_cbc_encrypt(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data);

/**
 * @brief Perform AES-CBC decryption.
 *
 * @param[in]  key_cfg Pointer to AES key configuration (includes IV).
 * @param[in]  data    Pointer to AES data buffers.
 *
 * @retval ::WISE_SUCCESS Decryption completed successfully.
 * @retval ::WISE_FAIL    Decryption failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_cbc_decrypt(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data);

/**
 * @brief Perform AES-CTR encryption.
 *
 * The same function may be used for decryption in CTR mode.
 *
 * @param[in]  key_cfg Pointer to AES key configuration (includes counter).
 * @param[in]  data    Pointer to AES data buffers.
 *
 * @retval ::WISE_SUCCESS Operation completed successfully.
 * @retval ::WISE_FAIL    Operation failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_ctr_encrypt(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data);

/**
 * @brief Perform AES-CTR decryption.
 *
 * Provided for API symmetry. Internally equivalent to encrypt in CTR mode.
 *
 * @param[in]  key_cfg Pointer to AES key configuration (includes counter).
 * @param[in]  data    Pointer to AES data buffers.
 *
 * @retval ::WISE_SUCCESS Operation completed successfully.
 * @retval ::WISE_FAIL    Operation failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_ctr_decrypt(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data);

/**
 * @brief Generate CBC-MAC tag using AES-CBC.
 *
 * @param[in]  key_cfg Pointer to AES key configuration.
 * @param[in]  data    Pointer to data over which MAC is computed.
 *
 * @retval ::WISE_SUCCESS MAC generation completed successfully.
 * @retval ::WISE_FAIL    MAC generation failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_cbcmac_generate(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data);

/**
 * @brief Perform AES-CCM authenticated encryption.
 *
 * One-shot helper that configures key, CCM parameters and encrypts payload.
 *
 * @param[in]  key_cfg Pointer to AES key configuration.
 * @param[in]  data    Pointer to AES data buffers.
 * @param[in]  ccm     Pointer to CCM extra parameters.
 *
 * @retval ::WISE_SUCCESS Encryption and tag generation succeeded.
 * @retval ::WISE_FAIL    Operation failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_ccm_encrypt(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data, const WISE_AES_CCM_EXTRA_T *ccm);

/**
 * @brief Perform AES-CCM authenticated decryption.
 *
 * Decrypts payload and verifies authentication tag.
 *
 * @param[in]  key_cfg Pointer to AES key configuration.
 * @param[in]  data    Pointer to AES data buffers.
 * @param[in]  ccm     Pointer to CCM extra parameters.
 *
 * @retval ::WISE_SUCCESS Decryption and tag verification succeeded.
 * @retval ::WISE_FAIL    Authentication failed or operation error.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_ccm_decrypt(const WISE_AES_KEY_CONFIG_T *key_cfg, const WISE_AES_DATA_T *data, const WISE_AES_CCM_EXTRA_T *ccm);

/**
 * @brief Read AES authentication tags.
 *
 * Reads up to four 32-bit words containing generated tags (for example
 * from AES-CCM or CBC-MAC operations).
 *
 * @param[out] tags Array of 4 @c uint32_t that receives tag words.
 *
 * @retval ::WISE_SUCCESS Tags read successfully.
 * @retval ::WISE_FAIL    Read failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_aes_read_tags(uint32_t tags[4]);

// ===== SHA =====
/**
 * @brief SHA-224 digest length in bytes.
 *
 * @ingroup WISE_CRYPTO
 */
#define SHA_224_OUTPUT_LEN 28

/**
 * @brief SHA-256 digest length in bytes.
 *
 * @ingroup WISE_CRYPTO
 */
#define SHA_256_OUTPUT_LEN 32

/**
 * @enum E_SHA_MODE_T
 * @brief SHA operation mode selection.
 *
 * @ingroup WISE_CRYPTO
 */
typedef enum {
    E_SHA_MODE_224 = 0, /**< SHA-224 mode. */
    E_SHA_MODE_256,     /**< SHA-256 mode. */
} E_SHA_MODE_T;

/**
 * @brief Generate a SHA-224 or SHA-256 message digest.
 *
 * @param[in]  mode      SHA mode (224 or 256).
 * @param[in]  dataAddr  Start address of input data in memory.
 * @param[in]  dataLen   Length of input data in bytes.
 * @param[out] outputSha Pointer to output buffer where digest is stored.
 *                       Must be at least ::SHA_224_OUTPUT_LEN or
 *                       ::SHA_256_OUTPUT_LEN bytes, depending on @p mode.
 *
 * @retval ::WISE_SUCCESS Digest generated successfully.
 * @retval ::WISE_FAIL    Generation failed.
 *
 * @ingroup WISE_CRYPTO
 */
WISE_STATUS wise_sha_generate(E_SHA_MODE_T mode, uint32_t dataAddr, uint32_t dataLen, uint8_t *outputSha);


/** @} */ /* end of WISE_CRYPTO group */

#endif /* _WISE_CRYPTO_API_H_ */

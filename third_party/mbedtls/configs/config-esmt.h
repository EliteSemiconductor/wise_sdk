/**
 * \file config-ccm-psk-tls1_2.h
 *
 * \brief Minimal configuration for TLS 1.2 with PSK and AES-CCM ciphersuites
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*
 * Minimal configuration for TLS 1.2 with PSK and AES-CCM ciphersuites
 *
 * Distinguishing features:
 * - Optimized for small code size, low bandwidth (on a reliable transport),
 *   and low RAM usage.
 * - No asymmetric cryptography (no certificates, no Diffie-Hellman key
 *   exchange).
 * - Fully modern and secure (provided the pre-shared keys are generated and
 *   stored securely).
 * - Very low record overhead with CCM-8.
 *
 * See README.txt for usage instructions.
 */

/* System support */
//#define MBEDTLS_HAVE_TIME /* Optionally used in Hello messages */
/* Other MBEDTLS_HAVE_XXX flags irrelevant for this configuration */

/* Mbed TLS modules */
#define MBEDTLS_AES_C
#define MBEDTLS_CCM_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_MD_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CMAC_C

/* TLS protocol feature support */
#define MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
#define MBEDTLS_SSL_PROTO_TLS1_2

/*
 * Use only CCM_8 ciphersuites, and
 * save ROM and a few bytes of RAM by specifying our own ciphersuite list
 */
#define MBEDTLS_SSL_CIPHERSUITES                        \
    MBEDTLS_TLS_PSK_WITH_AES_256_CCM_8,             \
    MBEDTLS_TLS_PSK_WITH_AES_128_CCM_8

/*
 * Save RAM at the expense of interoperability: do this only if you control
 * both ends of the connection!  (See comments in "mbedtls/ssl.h".)
 * The optimal size here depends on the typical size of records.
 */
#define MBEDTLS_SSL_IN_CONTENT_LEN              768
#define MBEDTLS_SSL_OUT_CONTENT_LEN             768

/* Save RAM at the expense of ROM */
#define MBEDTLS_AES_ROM_TABLES

/* Save some RAM by adjusting to your exact needs */
#define MBEDTLS_PSK_MAX_LEN    16 /* 128-bits keys are generally enough */

/*
 * You should adjust this to the exact number of sources you're using: default
 * is the "platform_entropy_poll" source, but you may want to add other ones
 * Minimum is 2 for the entropy test suite.
 */
#define MBEDTLS_ENTROPY_MAX_SOURCES 2

/* These defines are present so that the config modifying scripts can enable
 * them during tests/scripts/test-ref-configs.pl */
//#define MBEDTLS_USE_PSA_CRYPTO
//#define MBEDTLS_PSA_CRYPTO_C

/* Error messages and TLS debugging traces
 * (huge code size increase, needed for tests/ssl-opt.sh) */
//#define MBEDTLS_DEBUG_C
//#define MBEDTLS_ERROR_C


//RSA related
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_RSA_C
#define MBEDTLS_ENTROPY_C
//#define MBEDTLS_FS_IO
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V15           1
#define MBEDTLS_PKCS1_V21           1
#define MBEDTLS_RSA_PKCS_V15        1
#define MBEDTLS_RSA_PKCS_V21        1

#define MBEDTLS_ENTROPY_C
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
#define MBEDTLS_GENPRIME

#define MBEDTLS_RSA_GEN_KEY_MIN_BITS    128
#define MBEDTLS_PEM_WRITE_C
#define MBEDTLS_BASE64_C

//PSA crypto
#define MBEDTLS_PSA_CRYPTO_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECP_C
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
/*
#define MBEDTLS_ECP_DP_BP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
#define MBEDTLS_ECP_DP_CURVE25519_ENABLED
*/
#define MBEDTLS_ECDSA_C

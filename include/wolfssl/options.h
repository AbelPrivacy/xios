/* wolfssl options.h
 * generated from configure options
 *
 * Copyright (C) 2006-2024 wolfSSL Inc.
 *
 * This file is part of wolfSSL. (formerly known as CyaSSL)
 *
 */

#ifdef WOLFSSL_NO_OPTIONS_H
/* options.h inhibited by configuration */
#elif !defined(WOLFSSL_OPTIONS_H)
#define WOLFSSL_OPTIONS_H


#ifdef __cplusplus
extern "C" {
#endif

#undef  WOLFSSL_HAVE_ATOMIC_H
#define WOLFSSL_HAVE_ATOMIC_H

#undef  WOLFSSL_HAVE_ASSERT_H
#define WOLFSSL_HAVE_ASSERT_H

#undef  WOLFSSL_EXPERIMENTAL_SETTINGS
#define WOLFSSL_EXPERIMENTAL_SETTINGS

#undef  HAVE_C___ATOMIC
#define HAVE_C___ATOMIC 1

#undef  HAVE_THREAD_LS
#define HAVE_THREAD_LS

#undef  NO_DO178
#define NO_DO178

#undef  WOLFSSL_ASN_TEMPLATE
#define WOLFSSL_ASN_TEMPLATE

#undef  WOLFSSL_HAVE_MLKEM
#define WOLFSSL_HAVE_MLKEM

#undef  WOLFSSL_WC_MLKEM
#define WOLFSSL_WC_MLKEM

#undef  HAVE_DILITHIUM
#define HAVE_DILITHIUM

#undef  WOLFSSL_WC_DILITHIUM
#define WOLFSSL_WC_DILITHIUM

#undef  WOLFSSL_HAVE_XMSS
#define WOLFSSL_HAVE_XMSS

#undef  WOLFSSL_WC_XMSS
#define WOLFSSL_WC_XMSS

#undef  SINGLE_THREADED
#define SINGLE_THREADED

#undef  WOLFSSL_DTLS
#define WOLFSSL_DTLS

#undef  TFM_TIMING_RESISTANT
#define TFM_TIMING_RESISTANT

#undef  ECC_TIMING_RESISTANT
#define ECC_TIMING_RESISTANT

#undef  WC_RSA_BLINDING
#define WC_RSA_BLINDING

#undef  WOLFSSL_ARMASM_NO_HW_CRYPTO
#define WOLFSSL_ARMASM_NO_HW_CRYPTO

#undef  WOLFSSL_USE_ALIGN
#define WOLFSSL_USE_ALIGN

#undef  WOLFSSL_SHA224
#define WOLFSSL_SHA224

#undef  WOLFSSL_SHA512
#define WOLFSSL_SHA512

#undef  WOLFSSL_SHA384
#define WOLFSSL_SHA384

#undef  HAVE_HKDF
#define HAVE_HKDF

#undef  NO_DSA
#define NO_DSA

#undef  HAVE_ECC
#define HAVE_ECC

#undef  TFM_ECC256
#define TFM_ECC256

#undef  ECC_SHAMIR
#define ECC_SHAMIR

#undef  ECC_MIN_KEY_SZ
#define ECC_MIN_KEY_SZ 224

#undef  HAVE_CURVE25519
#define HAVE_CURVE25519

#undef  NO_OLD_TLS
#define NO_OLD_TLS

#undef  WC_RSA_PSS
#define WC_RSA_PSS

#undef  WOLFSSL_PSS_LONG_SALT
#define WOLFSSL_PSS_LONG_SALT

#undef  WOLFSSL_ASN_PRINT
#define WOLFSSL_ASN_PRINT

#undef  WOLFSSL_SHA3
#define WOLFSSL_SHA3

#undef  WOLFSSL_SHAKE128
#define WOLFSSL_SHAKE128

#undef  WOLFSSL_SHAKE256
#define WOLFSSL_SHAKE256

#undef  HAVE_POLY1305
#define HAVE_POLY1305

#undef  HAVE_CHACHA
#define HAVE_CHACHA

#undef  HAVE_HASHDRBG
#define HAVE_HASHDRBG

#undef  HAVE_TLS_EXTENSIONS
#define HAVE_TLS_EXTENSIONS

#undef  HAVE_SNI
#define HAVE_SNI

#undef  HAVE_TLS_EXTENSIONS
#define HAVE_TLS_EXTENSIONS

#undef  HAVE_ALPN
#define HAVE_ALPN

#undef  HAVE_TLS_EXTENSIONS
#define HAVE_TLS_EXTENSIONS

#undef  HAVE_SUPPORTED_CURVES
#define HAVE_SUPPORTED_CURVES

#undef  HAVE_FFDHE_2048
#define HAVE_FFDHE_2048

#undef  HAVE_SUPPORTED_CURVES
#define HAVE_SUPPORTED_CURVES

#undef  WOLFSSL_TLS13
#define WOLFSSL_TLS13

#undef  HAVE_TLS_EXTENSIONS
#define HAVE_TLS_EXTENSIONS

#undef  HAVE_TLS_EXTENSIONS
#define HAVE_TLS_EXTENSIONS

#undef  HAVE_SESSION_TICKET
#define HAVE_SESSION_TICKET

#undef  HAVE_EXTENDED_MASTER
#define HAVE_EXTENDED_MASTER

#undef  NO_RC4
#define NO_RC4

#undef  HAVE_ENCRYPT_THEN_MAC
#define HAVE_ENCRYPT_THEN_MAC

#undef  NO_PSK
#define NO_PSK

#undef  NO_MD4
#define NO_MD4

#undef  WOLFSSL_ENCRYPTED_KEYS
#define WOLFSSL_ENCRYPTED_KEYS

#undef  WOLFSSL_HAVE_SP_RSA
#define WOLFSSL_HAVE_SP_RSA

#undef  WOLFSSL_HAVE_SP_DH
#define WOLFSSL_HAVE_SP_DH

#undef  WOLFSSL_SP_4096
#define WOLFSSL_SP_4096

#undef  WOLFSSL_SP_LARGE_CODE
#define WOLFSSL_SP_LARGE_CODE

#undef  WOLFSSL_HAVE_SP_ECC
#define WOLFSSL_HAVE_SP_ECC

#undef  HAVE_ECC384
#define HAVE_ECC384

#undef  WOLFSSL_SP_384
#define WOLFSSL_SP_384

#undef  HAVE_ECC521
#define HAVE_ECC521

#undef  WOLFSSL_SP_521
#define WOLFSSL_SP_521

#undef  WOLFSSL_SP_MATH_ALL
#define WOLFSSL_SP_MATH_ALL

#undef  WOLFSSL_SP_ARM64
#define WOLFSSL_SP_ARM64

#undef  WOLFSSL_AARCH64_BUILD
#define WOLFSSL_AARCH64_BUILD

#undef  WOLFSSL_SP_ASM
#define WOLFSSL_SP_ASM

#undef  WOLFSSL_SP_ARM64_ASM
#define WOLFSSL_SP_ARM64_ASM

#undef  WC_NO_ASYNC_THREADING
#define WC_NO_ASYNC_THREADING

#undef  HAVE_DH_DEFAULT_PARAMS
#define HAVE_DH_DEFAULT_PARAMS

#undef  WOLFSSL_SYS_CA_CERTS
#define WOLFSSL_SYS_CA_CERTS

#undef  WOLFSSL_DUAL_ALG_CERTS
#define WOLFSSL_DUAL_ALG_CERTS

#undef  WOLFSSL_KEY_GEN
#define WOLFSSL_KEY_GEN

#undef  WOLFSSL_CERT_GEN
#define WOLFSSL_CERT_GEN

#undef  OPENSSL_EXTRA
#define OPENSSL_EXTRA

#undef  NO_DES3
#define NO_DES3

#undef  NO_DES3_TLS_SUITES
#define NO_DES3_TLS_SUITES

#undef  GCM_TABLE_4BIT
#define GCM_TABLE_4BIT

#undef  HAVE_AESGCM
#define HAVE_AESGCM

#undef  HAVE_TLS_EXTENSIONS
#define HAVE_TLS_EXTENSIONS

#undef  HAVE_SERVER_RENEGOTIATION_INFO
#define HAVE_SERVER_RENEGOTIATION_INFO

#undef  HAVE___UINT128_T
#define HAVE___UINT128_T 1

#undef  WOLFSSL_TEST_STATIC_BUILD
#define WOLFSSL_TEST_STATIC_BUILD

#undef  HAVE_WC_INTROSPECTION
#define HAVE_WC_INTROSPECTION


#ifdef __cplusplus
}
#endif


#endif /* WOLFSSL_OPTIONS_H */


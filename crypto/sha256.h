/**
 *
 * File Name: sha256.h
 * Title    : SHA256 library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-09-08
 * Modified : 
 * Revised  : 
 * Version  : 0.1.0.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_CRYPTO_SHA256_H
#define LIBAVR_CRYPTO_SHA256_H

#include <stdint.h>

#define SHA256_HASH_LEN     32

typedef struct sha256_ctx {
    uint32_t hash_cur[8];
    uint32_t hash_last[8];
} sha256_ctx_t;

extern sha256_ctx_t *sha256_init(void);
extern void sha256_free(sha256_ctx_t *sha256);
extern void sha256_transform(sha256_ctx_t *sha256, uint8_t msg[64]);
extern sha256_ctx_t *sha256_sum(uint8_t *buf, int len);
extern void sha256_get_hash(sha256_ctx_t *sha256, uint8_t *buf);
extern int sha256_equal(sha256_ctx_t *sha1, sha256_ctx_t *sha2);

#endif

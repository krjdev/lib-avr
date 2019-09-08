/**
 *
 * File Name: sha256.c
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

#include <stdlib.h>
#include <string.h>

#include "sha256.h"

#define ROR32(val, num)     (((val) >> (num)) | ((val) << (32 - (num))))
#define SHR32(val, num)     ((val) >> (num))

#define CH(x, y, z)         (((x) & (y)) ^ ((~(x)) & (z)))
#define MAJ(x, y, z)        (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SUM0(x)             (ROR32((x), 2) ^ ROR32((x), 13) ^ ROR32((x), 22))
#define SUM1(x)             (ROR32((x), 6) ^ ROR32((x), 11) ^ ROR32((x), 25))
#define SIG0(x)             (ROR32((x), 7) ^ ROR32((x), 18) ^ SHR32((x), 3))
#define SIG1(x)             (ROR32((x), 17) ^ ROR32((x), 19) ^ SHR32((x), 10))

#define SHA256_INIT_H1      0x6A09E667
#define SHA256_INIT_H2      0xBB67AE85
#define SHA256_INIT_H3      0x3C6EF372
#define SHA256_INIT_H4      0xA54FF53A
#define SHA256_INIT_H5      0x510E527F
#define SHA256_INIT_H6      0x9B05688C
#define SHA256_INIT_H7      0x1F83D9AB
#define SHA256_INIT_H8      0x5BE0CD19

static const uint32_t k[64] = { 0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 
                                0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5, 
                                0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 
                                0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174, 
                                0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 
                                0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA, 
                                0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 
                                0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967, 
                                0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 
                                0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85, 
                                0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 
                                0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070, 
                                0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 
                                0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3, 
                                0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 
                                0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2 };

sha256_ctx_t *sha256_init(void)
{
    sha256_ctx_t *sha256;
    
    sha256 = (sha256_ctx_t *) malloc(sizeof(sha256_ctx_t));
    
    if (!sha256)
        return NULL;
    
    sha256->hash_cur[0] = SHA256_INIT_H1;
    sha256->hash_cur[1] = SHA256_INIT_H2;
    sha256->hash_cur[2] = SHA256_INIT_H3;
    sha256->hash_cur[3] = SHA256_INIT_H4;
    sha256->hash_cur[4] = SHA256_INIT_H5;
    sha256->hash_cur[5] = SHA256_INIT_H6;
    sha256->hash_cur[6] = SHA256_INIT_H7;
    sha256->hash_cur[7] = SHA256_INIT_H8;
    sha256->hash_last[0] = SHA256_INIT_H1;
    sha256->hash_last[1] = SHA256_INIT_H2;
    sha256->hash_last[2] = SHA256_INIT_H3;
    sha256->hash_last[3] = SHA256_INIT_H4;
    sha256->hash_last[4] = SHA256_INIT_H5;
    sha256->hash_last[5] = SHA256_INIT_H6;
    sha256->hash_last[6] = SHA256_INIT_H7;
    sha256->hash_last[7] = SHA256_INIT_H8;
    return sha256;
}

void sha256_free(sha256_ctx_t *sha256)
{
    if (!sha256)
        return;
    
    free(sha256);
}

void sha256_transform(sha256_ctx_t *sha256, uint8_t msg[64])
{
    int i;
    int j = 0;
    uint32_t t1;
    uint32_t t2;
    uint32_t w[64];
    
    if (!sha256)
        return;
    
    for (i = 0; i < 16; i++) {
        w[i] = (uint32_t) msg[j++] << 24;
        w[i] |= (uint32_t) msg[j++] << 16;
        w[i] |= (uint32_t) msg[j++] << 8;
        w[i] |= (uint32_t) msg[j++];
    }
    
    for (i = 16; i < 64; i++) {
        w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
    }
    
    for (i = 0; i < 64; i++) {
        t1 = sha256->hash_cur[7] + 
             SUM1(sha256->hash_cur[4]) + 
             CH(sha256->hash_cur[4], sha256->hash_cur[5], sha256->hash_cur[6]) + 
             k[i] + 
             w[i];
        t2 = SUM0(sha256->hash_cur[0]) + 
             MAJ(sha256->hash_cur[0], sha256->hash_cur[1], sha256->hash_cur[2]);
        sha256->hash_cur[7] = sha256->hash_cur[6];
        sha256->hash_cur[6] = sha256->hash_cur[5];
        sha256->hash_cur[5] = sha256->hash_cur[4];
        sha256->hash_cur[4] = sha256->hash_cur[3] + t1;
        sha256->hash_cur[3] = sha256->hash_cur[2];
        sha256->hash_cur[2] = sha256->hash_cur[1];
        sha256->hash_cur[1] = sha256->hash_cur[0];
        sha256->hash_cur[0] = t1 + t2;
    }
    
    sha256->hash_cur[0] += sha256->hash_last[0];
    sha256->hash_cur[1] += sha256->hash_last[1];
    sha256->hash_cur[2] += sha256->hash_last[2];
    sha256->hash_cur[3] += sha256->hash_last[3];
    sha256->hash_cur[4] += sha256->hash_last[4];
    sha256->hash_cur[5] += sha256->hash_last[5];
    sha256->hash_cur[6] += sha256->hash_last[6];
    sha256->hash_cur[7] += sha256->hash_last[7];
    
    sha256->hash_last[0] = sha256->hash_cur[0];
    sha256->hash_last[1] = sha256->hash_cur[1];
    sha256->hash_last[2] = sha256->hash_cur[2];
    sha256->hash_last[3] = sha256->hash_cur[3];
    sha256->hash_last[4] = sha256->hash_cur[4];
    sha256->hash_last[5] = sha256->hash_cur[5];
    sha256->hash_last[6] = sha256->hash_cur[6];
    sha256->hash_last[7] = sha256->hash_cur[7];
}

sha256_ctx_t *sha256_sum(uint8_t *buf, int len)
{
    int i;
    int p = 0;
    sha256_ctx_t *sha256;
    uint8_t msg[64];
    uint64_t len_bit;
    int num_blocks;
    int mod;
    
    sha256 = sha256_init();
    
    if (!sha256)
        return NULL;
    
    num_blocks = len / 64;
    mod = len % 64;
    
    if (num_blocks) {
        for (i = 0; i < num_blocks; i++) {
            memcpy(msg, &buf[p], 64);
            p += 64;
            sha256_transform(sha256, msg);
        }
    }
    
    if (mod < 56) {
        memset(msg, 0, 64);
        memcpy(msg, &buf[p], mod);
        msg[mod] = 0x80;
    } else if (mod == 0){
        memset(msg, 0, 64);
        msg[0] = 0x80;
    } else {
        memset(msg, 0, 64);
        memcpy(msg, &buf[p], mod);
        msg[mod] = 0x80;
        sha256_transform(sha256, msg);
        memset(msg, 0, 64);
    }
    
    len_bit = len * 8;
    msg[56] = (uint8_t) (len_bit >> 56) & 0xFF;
    msg[57] = (uint8_t) (len_bit >> 48) & 0xFF;
    msg[58] = (uint8_t) (len_bit >> 40) & 0xFF;
    msg[59] = (uint8_t) (len_bit >> 32) & 0xFF;
    msg[60] = (uint8_t) (len_bit >> 24) & 0xFF;
    msg[61] = (uint8_t) (len_bit >> 16) & 0xFF;
    msg[62] = (uint8_t) (len_bit >> 8) & 0xFF;
    msg[63] = (uint8_t) len_bit & 0xFF;
    sha256_transform(sha256, msg);
    return sha256;
}

void sha256_get_hash(sha256_ctx_t *sha256, uint8_t *buf)
{
    if (!sha256)
        return;
    
    if (!buf)
        return;
    
    buf[0] = (uint8_t) (sha256->hash_cur[0] >> 24) & 0xFF;
    buf[1] = (uint8_t) (sha256->hash_cur[0] >> 16) & 0xFF;
    buf[2] = (uint8_t) (sha256->hash_cur[0] >> 8) & 0xFF;
    buf[3] = (uint8_t) sha256->hash_cur[0] & 0xFF;
    buf[4] = (uint8_t) (sha256->hash_cur[1] >> 24) & 0xFF;
    buf[5] = (uint8_t) (sha256->hash_cur[1] >> 16) & 0xFF;
    buf[6] = (uint8_t) (sha256->hash_cur[1] >> 8) & 0xFF;
    buf[7] = (uint8_t) sha256->hash_cur[1] & 0xFF;
    buf[8] = (uint8_t) (sha256->hash_cur[2] >> 24) & 0xFF;
    buf[9] = (uint8_t) (sha256->hash_cur[2] >> 16) & 0xFF;
    buf[10] = (uint8_t) (sha256->hash_cur[2] >> 8) & 0xFF;
    buf[11] = (uint8_t) sha256->hash_cur[2] & 0xFF;
    buf[12] = (uint8_t) (sha256->hash_cur[3] >> 24) & 0xFF;
    buf[13] = (uint8_t) (sha256->hash_cur[3] >> 16) & 0xFF;
    buf[14] = (uint8_t) (sha256->hash_cur[3] >> 8) & 0xFF;
    buf[15] = (uint8_t) sha256->hash_cur[3] & 0xFF;
    buf[16] = (uint8_t) (sha256->hash_cur[4] >> 24) & 0xFF;
    buf[17] = (uint8_t) (sha256->hash_cur[4] >> 16) & 0xFF;
    buf[18] = (uint8_t) (sha256->hash_cur[4] >> 8) & 0xFF;
    buf[19] = (uint8_t) sha256->hash_cur[4] & 0xFF;
    buf[20] = (uint8_t) (sha256->hash_cur[5] >> 24) & 0xFF;
    buf[21] = (uint8_t) (sha256->hash_cur[5] >> 16) & 0xFF;
    buf[22] = (uint8_t) (sha256->hash_cur[5] >> 8) & 0xFF;
    buf[23] = (uint8_t) sha256->hash_cur[5] & 0xFF;
    buf[24] = (uint8_t) (sha256->hash_cur[6] >> 24) & 0xFF;
    buf[25] = (uint8_t) (sha256->hash_cur[6] >> 16) & 0xFF;
    buf[26] = (uint8_t) (sha256->hash_cur[6] >> 8) & 0xFF;
    buf[27] = (uint8_t) sha256->hash_cur[6] & 0xFF;
    buf[28] = (uint8_t) (sha256->hash_cur[7] >> 24) & 0xFF;
    buf[29] = (uint8_t) (sha256->hash_cur[7] >> 16) & 0xFF;
    buf[30] = (uint8_t) (sha256->hash_cur[7] >> 8) & 0xFF;
    buf[31] = (uint8_t) sha256->hash_cur[7] & 0xFF;
}

int sha256_equal(sha256_ctx_t *sha1, sha256_ctx_t *sha2)
{
    if (!sha1)
        return -1;
    
    if (!sha2)
        return -1;
    
    if ((sha1->hash_cur[0] == sha2->hash_cur[0]) && 
        (sha1->hash_cur[1] == sha2->hash_cur[1]) && 
        (sha1->hash_cur[2] == sha2->hash_cur[2]) && 
        (sha1->hash_cur[3] == sha2->hash_cur[3]) && 
        (sha1->hash_cur[4] == sha2->hash_cur[4]) && 
        (sha1->hash_cur[5] == sha2->hash_cur[5]) && 
        (sha1->hash_cur[6] == sha2->hash_cur[6]) && 
        (sha1->hash_cur[7] == sha2->hash_cur[7]))
        return 1;
    
    return 0;
}

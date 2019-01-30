/**
 *
 * File Name: ipv4.h
 * Title    : IPv4 definitions and helper functions header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
 * Modified : 2019-01-30
 * Revised  : 
 * Version  : 0.2.1.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_NET_IPV4_H
#define LIBAVR_NET_IPV4_H

#include <stdint.h>

#define IPV4_FLAG_DF    1
#define IPV4_FLAG_MF    2

#define IPV4_PROT_TCP   6
#define IPV4_PROT_UDP   17

typedef struct ipv4_addr {
    uint8_t ia_byte0;
    uint8_t ia_byte1;
    uint8_t ia_byte2;
    uint8_t ia_byte3;
} ipv4_addr_t;

typedef struct ipv4_range {
    ipv4_addr_t ir_ip;
    uint8_t ir_suf;
} ipv4_range_t;

typedef struct ipv4_hdr {
    unsigned char ih_ver : 4;
    unsigned char ih_ihl : 4;
    unsigned char ih_dscp : 6;
    unsigned char ih_ecn : 2;
    uint16_t ih_tlen;
    uint16_t ih_id;
    unsigned char ih_flag : 3;
    unsigned int ih_foff : 13;
    uint8_t ih_ttl;
    uint8_t ih_prot;
    uint16_t ih_chk;
    ipv4_addr_t ih_src;
    ipv4_addr_t ih_dst;
} ipv4_hdr_t;

ipv4_range_t rfc6890[] = {
    { { 0, 0, 0, 0 }, 8 }, 
    { { 10, 0, 0, 0 }, 8 }, 
    { { 100, 64, 0, 0 }, 10 }, 
    { { 127, 0, 0, 0 }, 8 }, 
    { { 169, 254, 0, 0 }, 16 }, 
    { { 172, 16, 0, 0 }, 12 }, 
    { { 192, 0, 0, 0 }, 24 }, 
    { { 192, 0, 0, 0 }, 29 }, 
    { { 192, 0, 2, 0 }, 24 }, 
    { { 192, 88, 99, 0 }, 24 }, 
    { { 192, 168, 0, 0 }, 16 }, 
    { { 198, 18, 0, 0 }, 15 },
    { { 198, 51, 100, 0 }, 24 }, 
    { { 203, 0, 113, 0 }, 24 }, 
    { { 224, 0, 0, 0 }, 4 }, 
    { { 240, 0, 0, 0 }, 4 },
    { { 255, 255, 255, 255 }, 32 }
};

extern int ipv4_aton(const char *str, ipv4_addr_t *ip);
extern int ipv4_ntoa(ipv4_addr_t *ip, char *str);
extern int ipv4_equal(ipv4_addr_t *ip1, ipv4_addr_t *ip2);
extern int ipv4_acpy(ipv4_addr_t *dst, ipv4_addr_t *src);
extern int ipv4_is_broadcast(ipv4_addr_t *ip);

#endif

/**
 *
 * File Name: ipv4.h
 * Title    : IPv4 definitions and helper functions header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
 * Modified : 2019-01-30
 * Revised  : 
 * Version  : 0.2.0.0
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

typedef struct ipv4_addr {
    uint8_t ia_byte0;
    uint8_t ia_byte1;
    uint8_t ia_byte2;
    uint8_t ia_byte3;
} ipv4_addr_t;

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

extern int ipv4_aton(const char *str, ipv4_addr_t *ip);
extern int ipv4_ntoa(ipv4_addr_t *ip, char *str);

#endif

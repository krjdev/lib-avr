/**
 *
 * File Name: ipv4.h
 * Title    : IPv4 definitions and helper functions header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
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

#ifndef LIBAVR_NET_IPV4_H
#define LIBAVR_NET_IPV4_H

#include <stdint.h>

typedef struct ipv4_addr {
    uint8_t ia_byte0;
    uint8_t ia_byte1;
    uint8_t ia_byte2;
    uint8_t ia_byte3;
} ipv4_addr_t;

extern ipv4_addr_t *ipv4_str_to_ip(const char *str);
extern char *ipv4_ip_to_str(ipv4_addr_t *ip);

#endif

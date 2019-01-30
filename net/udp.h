/**
 *
 * File Name: udp.h
 * Title    : UDP definitions and helper functions header
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-01-30
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

#ifndef LIBAVR_NET_UDP_H
#define LIBAVR_NET_UDP_H

#include <stdint.h>

typedef struct udp_hdr {
    uint16_t uh_srcp;   /* Source port */
    uint16_t uh_dstp;   /* Destination port */
    uint16_t uh_len;    /* Length (UDP header and data) */
    uint16_t uh_chk;    /* Checksum */
} udp_hdr_t;

extern int udp_set_port_src(udp_hdr_t *udp, uint16_t src);
extern int udp_set_port_dst(udp_hdr_t *udp, uint16_t dst);
extern int udp_set_len(udp_hdr_t *udp, uint16_t len);
extern int udp_set_chk(udp_hdr_t *udp, uint16_t chk);
extern int udp_get_port_src(udp_hdr_t *udp, uint16_t *src);
extern int udp_get_port_dst(udp_hdr_t *udp, uint16_t *dst);
extern int udp_get_len(udp_hdr_t *udp, uint16_t *len);
extern int udp_get_chk(udp_hdr_t *udp, uint16_t *chk);

#endif

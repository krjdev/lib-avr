/**
 *
 * File Name: udp.h
 * Title    : UDP library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-01-30
 * Modified : 2019-08-09
 * Revised  : 
 * Version  : 0.3.0.0
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

#include "ipv4.h"

#define UDP_ERROR_SUCCESS       0
#define UDP_ERROR_INVAL         1
#define UDP_ERROR_NOMEM         2
#define UDP_ERROR_CHKSUM        3
#define UDP_ERROR_UNKNOWN       4
#define UDP_ERROR_INTERNAL      5

typedef struct udp_hdr {
    uint16_t uh_srcp;   /* Source port */
    uint16_t uh_dstp;   /* Destination port */
    uint16_t uh_len;    /* Length (UDP header and data) */
    uint16_t uh_chk;    /* Checksum */
} udp_hdr_t;

typedef struct udp_packet {
    udp_hdr_t up_hdr;
    uint8_t *up_payload_buf;
    int up_payload_len;
} udp_packet_t;

extern int udp_pkt_set_srcp(udp_packet_t *udp, uint16_t srcp);
extern int udp_pkt_set_dstp(udp_packet_t *udp, uint16_t dstp);
extern int udp_pkt_set_payload(udp_packet_t *udp, uint8_t *buf, int len);
extern int udp_pkt_get_srcp(udp_packet_t *udp, uint16_t *srcp);
extern int udp_pkt_get_dstp(udp_packet_t *udp, uint16_t *dstp);
extern int udp_pkt_get_payload_len(udp_packet_t *udp);
extern int udp_pkt_get_payload(udp_packet_t *udp, uint8_t **buf);
extern int udp_pkt_get_len(udp_packet_t *udp);
extern int udp_pkt_create(uint16_t srcp, uint16_t dstp, 
                          uint8_t *buf, int len, 
                          udp_packet_t *udp);
extern int udp_pkt_free(udp_packet_t *udp);
extern int udp_pkt_to_ip(udp_packet_t *udp, ipv4_packet_t *ip_udp);
extern int udp_ip_to_pkt(ipv4_packet_t *ip_udp, udp_packet_t *udp);
extern int udp_get_last_error(void);

#endif

/**
 *
 * File Name: ipv6.h
 * Title    : IPv6 library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-08-04
 * Modified : 2019-08-05
 * Revised  : 
 * Version  : 0.1.1.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_NET_IPV6_H
#define LIBAVR_NET_IPV6_H

#include <stdint.h>

#define IPV6_NHDR_TCP   6
#define IPV6_NHDR_UDP   17

typedef struct ipv6_addr {
    uint8_t ia_byte00;
    uint8_t ia_byte01;
    uint8_t ia_byte02;
    uint8_t ia_byte03;
    uint8_t ia_byte04;
    uint8_t ia_byte05;
    uint8_t ia_byte06;
    uint8_t ia_byte07;
    uint8_t ia_byte08;
    uint8_t ia_byte09;
    uint8_t ia_byte10;
    uint8_t ia_byte11;
    uint8_t ia_byte12;
    uint8_t ia_byte13;
    uint8_t ia_byte14;
    uint8_t ia_byte15;
} ipv6_addr_t;

typedef struct ipv6_hdr {
    unsigned char ih_ver : 4;
    unsigned char ih_ecn : 2;
    unsigned char ih_dscp : 6;
    uint32_t ih_flow : 20;
    uint16_t ih_plen;
    uint8_t ih_nhdr;
    uint8_t ih_hopl;
    ipv6_addr_t ih_src;
    ipv6_addr_t ih_dst;
} ipv6_hdr_t;

typedef struct ipv6_packet {
    ipv6_hdr_t ip_hdr;
    uint8_t *ip_payload_buf;
    int ip_payload_len;
} ipv6_packet_t;

extern int ipv6_addr_equal(ipv6_addr_t *ia1, ipv6_addr_t *ia2);
extern int ipv6_addr_cpy(ipv6_addr_t *ia_dst, ipv6_addr_t *ia_src);
extern int ipv6_pkt_set_flow(ipv6_packet_t *ip, uint32_t flow);
extern int ipv6_pkt_set_nhdr(ipv6_packet_t *ip, uint8_t nhdr);
extern int ipv6_pkt_set_hopl(ipv6_packet_t *ip, uint8_t hopl);
extern int ipv6_pkt_set_src(ipv6_packet_t *ip, ipv6_addr_t *src);
extern int ipv6_pkt_set_dst(ipv6_packet_t *ip, ipv6_addr_t *dst);
extern int ipv6_pkt_set_payload(ipv6_packet_t *ip, uint8_t *buf, int len);
extern int ipv6_pkt_get_flow(ipv6_packet_t *ip, uint32_t *flow);
extern int ipv6_pkt_get_nhdr(ipv6_packet_t *ip, uint8_t *nhdr);
extern int ipv6_pkt_get_hopl(ipv6_packet_t *ip, uint8_t *hopl);
extern int ipv6_pkt_get_src(ipv6_packet_t *ip, ipv6_addr_t *src);
extern int ipv6_pkt_get_dst(ipv6_packet_t *ip, ipv6_addr_t *dst);
extern int ipv6_pkt_get_payload_len(ipv6_packet_t *ip);
extern int ipv6_pkt_get_payload(ipv6_packet_t *ip, uint8_t **buf);
extern int ipv6_pkt_create_empty(ipv6_packet_t *ip);
extern int ipv6_pkt_free(ipv6_packet_t *ip);
extern int ipv6_pkt_get_len(ipv6_packet_t *ip);
extern int ipv6_buf_to_pkt(uint8_t *buf, int len, ipv6_packet_t *ip);
extern int ipv6_pkt_to_buf(ipv6_packet_t *ip, uint8_t *buf);

#endif

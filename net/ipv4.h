/**
 *
 * File Name: ipv4.h
 * Title    : IPv4 library
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
 * Modified : 2019-02-11
 * Revised  : 
 * Version  : 0.4.1.0
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

#define IPV4_PROT_ICMP  1
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

typedef struct ipv4_packet {
    ipv4_hdr_t ip_hdr;
    uint8_t *ip_options_buf;
    int ip_options_len;
    uint8_t *ip_payload_buf;
    int ip_payload_len;
} ipv4_packet_t;

extern int ipv4_addr_aton(const char *str, ipv4_addr_t *ip);
extern int ipv4_addr_ntoa(ipv4_addr_t *ip, char *str);
extern int ipv4_addr_equal(ipv4_addr_t *ip1, ipv4_addr_t *ip2);
extern int ipv4_addr_cpy(ipv4_addr_t *dst, ipv4_addr_t *src);
extern int ipv4_addr_is_broadcast(ipv4_addr_t *ip);
extern int ipv4_pkt_create_empty(ipv4_packet_t *ip);
extern int ipv4_pkt_free(ipv4_packet_t *ip);
extern int ipv4_pkt_set_id(ipv4_packet_t *ip, uint16_t id);
extern int ipv4_pkt_set_ttl(ipv4_packet_t *ip, uint8_t ttl);
extern int ipv4_pkt_set_prot(ipv4_packet_t *ip, uint8_t prot);
extern int ipv4_pkt_set_src(ipv4_packet_t *ip, ipv4_addr_t *src);
extern int ipv4_pkt_set_dst(ipv4_packet_t *ip, ipv4_addr_t *dst);
extern int ipv4_pkt_set_options(ipv4_packet_t *ip, uint8_t *buf, int len);
extern int ipv4_pkt_set_payload(ipv4_packet_t *ip, uint8_t *buf, int len);
extern int ipv4_pkt_get_id(ipv4_packet_t *ip, uint16_t *id);
extern int ipv4_pkt_get_ttl(ipv4_packet_t *ip, uint8_t *ttl);
extern int ipv4_pkt_get_prot(ipv4_packet_t *ip, uint8_t *prot);
extern int ipv4_pkt_get_src(ipv4_packet_t *ip, ipv4_addr_t *src);
extern int ipv4_pkt_get_dst(ipv4_packet_t *ip, ipv4_addr_t *dst);
extern int ipv4_pkt_get_options_len(ipv4_packet_t *ip);
extern int ipv4_pkt_get_options(ipv4_packet_t *ip, uint8_t **buf);
extern int ipv4_pkt_get_payload_len(ipv4_packet_t *ip);
extern int ipv4_pkt_get_payload(ipv4_packet_t *ip, uint8_t **buf);
extern int ipv4_pkt_get_len(ipv4_packet_t *ip);
extern int ipv4_buf_to_pkt(uint8_t *buf, int len, ipv4_packet_t *ip);
extern int ipv4_pkt_to_buf(ipv4_packet_t *ip, uint8_t *buf);

#endif

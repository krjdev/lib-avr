/**
 *
 * File Name: icmp.h
 * Title    : ICMP library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-02-09
 * Modified : 2019-06-22
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

#ifndef LIBAVR_NET_ICMP_H
#define LIBAVR_NET_ICMP_H

#include <stdint.h>

#define ICMP_TYPE_ECHOREP   0
#define ICMP_TYPE_UNREACH   3
#define ICMP_TYPE_ECHOREQ   8

typedef struct icmp_hdr {
    uint8_t ih_type;
    uint8_t ih_code;
    uint16_t ih_chk;
    uint8_t ih_rest[4];
} icmp_hdr_t;

typedef struct icmp_packet {
    icmp_hdr_t ip_hdr;
    uint8_t *ip_payload_buf;
    int ip_payload_len;
} icmp_packet_t;

extern int icmp_pkt_set_type(icmp_packet_t *icmp, uint8_t type);
extern int icmp_pkt_set_code(icmp_packet_t *icmp, uint8_t code);
extern int icmp_pkt_set_rest(icmp_packet_t *icmp, uint8_t *rest, int len);
extern int icmp_pkt_set_payload(icmp_packet_t *icmp, uint8_t *buf, int len);
extern int icmp_pkt_get_type(icmp_packet_t *icmp, uint8_t *type);
extern int icmp_pkt_get_code(icmp_packet_t *icmp, uint8_t *code);
extern int icmp_pkt_get_rest(icmp_packet_t *icmp, uint8_t *rest);
extern int icmp_pkt_get_payload_len(icmp_packet_t *icmp);
extern int icmp_pkt_get_payload(icmp_packet_t *icmp, uint8_t **buf;
extern int icmp_pkt_get_len(icmp_packet_t *icmp);
extern int icmp_buf_to_pkt(uint8_t *buf, int len, icmp_packet_t *icmp);
extern int icmp_pkt_to_buf(icmp_packet_t *icmp, uint8_t *buf);
extern int icmp_create_echo_reply(icmp_packet_t *icmp_in, icmp_packet_t *icmp_out);
extern int icmp_pkt_free(icmp_packet_t *icmp);

#endif

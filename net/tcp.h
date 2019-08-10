/**
 *
 * File Name: tcp.h
 * Title    : TCP library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-08-09
 * Modified : 2019-08-10
 * Revised  : 
 * Version  : 0.1.0.1
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_NET_TCP_H
#define LIBAVR_NET_TCP_H

#include <stdint.h>

#include "ipv4.h"

#define TCP_ERROR_SUCCESS       0
#define TCP_ERROR_INVAL         1
#define TCP_ERROR_NOMEM         2
#define TCP_ERROR_CHKSUM        3
#define TCP_ERROR_UNKNOWN       4
#define TCP_ERROR_INTERNAL      5

#define TCP_FLAG_FIN            0x01
#define TCP_FLAG_SYN            0x02
#define TCP_FLAG_RST            0x04
#define TCP_FLAG_PSH            0x08
#define TCP_FLAG_ACK            0x10
#define TCP_FLAG_URG            0x20
#define TCP_FLAG_ECE            0x40
#define TCP_FLAG_CWR            0x80

typedef struct tcp_hdr {
    uint16_t th_srcp;
    uint16_t th_dstp;
    uint32_t th_seqn;
    uint32_t th_ackn;
    uint8_t th_off : 4;
    uint8_t th_res : 4;
    uint8_t th_flags;
    uint16_t th_win;
    uint16_t th_chk;
    uint16_t th_urgp;
} tcp_hdr_t;

typedef struct tcp_packet {
    tcp_hdr_t tp_hdr;
    uint8_t *tp_options_buf;
    int tp_options_len;
    uint8_t *tp_payload_buf;
    int tp_payload_len;
} tcp_packet_t;

extern int tcp_pkt_set_srcp(tcp_packet_t *tcp, uint16_t srcp);
extern int tcp_pkt_set_dstp(tcp_packet_t *tcp, uint16_t dstp);
extern int tcp_pkt_set_seqn(tcp_packet_t *tcp, uint32_t seqn);
extern int tcp_pkt_set_ackn(tcp_packet_t *tcp, uint32_t ackn);
extern int tcp_pkt_set_flags(tcp_packet_t *tcp, uint8_t flags);
extern int tcp_pkt_set_win(tcp_packet_t *tcp, uint16_t win);
extern int tcp_pkt_set_urgp(tcp_packet_t *tcp, uint16_t urgp);
extern int tcp_pkt_set_options(tcp_packet_t *tcp, uint8_t *buf, int len);
extern int tcp_pkt_set_payload(tcp_packet_t *tcp, uint8_t *buf, int len);
extern int tcp_pkt_get_srcp(tcp_packet_t *tcp, uint16_t *srcp);
extern int tcp_pkt_get_dstp(tcp_packet_t *tcp, uint16_t *dstp);
extern int tcp_pkt_get_seqn(tcp_packet_t *tcp, uint32_t *seqn);
extern int tcp_pkt_get_ackn(tcp_packet_t *tcp, uint32_t *ackn);
extern int tcp_pkt_get_flags(tcp_packet_t *tcp, uint8_t *flags);
extern int tcp_pkt_get_win(tcp_packet_t *tcp, uint16_t *win);
extern int tcp_pkt_get_urgp(tcp_packet_t *tcp, uint16_t *urgp);
extern int tcp_pkt_get_options(tcp_packet_t *tcp, uint8_t **buf);
extern int tcp_pkt_get_options_len(tcp_packet_t *tcp);
extern int tcp_pkt_get_payload(tcp_packet_t *tcp, uint8_t **buf);
extern int tcp_pkt_get_payload_len(tcp_packet_t *tcp);
extern int tcp_pkt_get_len(tcp_packet_t *tcp);
extern int tcp_pkt_create(uint16_t srcp, uint16_t dstp, 
                          uint32_t seqn, uint32_t ackn, 
                          uint16_t win, uint8_t flags, 
                          uint16_t urgp, 
                          tcp_packet_t *tcp);
extern int tcp_pkt_free(tcp_packet_t *tcp);
extern int tcp_ip_to_pkt(ipv4_packet_t *ip, tcp_packet_t *tcp);
extern int tcp_pkt_to_ip(tcp_packet_t *tcp, ipv4_packet_t *ip);
extern int tcp_get_last_error(void);

#endif

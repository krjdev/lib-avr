/**
 *
 * File Name: arp.h
 * Title    : ARP library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-01-30
 * Modified : 2019-08-10
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

#ifndef LIBAVR_NET_ARP_H
#define LIBAVR_NET_ARP_H

#include <stdint.h>

#include "ethernet.h"
#include "ipv4.h"

#define ARP_ERROR_SUCCESS   0
#define ARP_ERROR_INVAL     1
#define ARP_ERROR_UNKNOWN   2

#define ARP_OPER_QUERY      1
#define ARP_OPER_ANSWE      2

typedef struct arp_packet {
    uint16_t ap_htype;  /* Hardware address type */
    uint16_t ap_ptype;  /* Protocol address type */
    uint8_t ap_hlen;    /* Hardware address length */
    uint8_t ap_plen;    /* Protocol address length */
    uint16_t ap_oper;   /* ARP operation */
    mac_addr_t ap_sha;  /* Source MAC address */
    ipv4_addr_t ap_spa; /* Source IPv4 address */
    mac_addr_t ap_tha;  /* Destination MAC address */
    ipv4_addr_t ap_tpa; /* Destination IPv4 address */
    
} arp_packet_t;

extern int arp_init(mac_addr_t *src_mac, ipv4_addr_t *src_ip);
extern int arp_pkt_set_oper(arp_packet_t *arp, uint16_t oper);
extern int arp_pkt_set_sha(arp_packet_t *arp, mac_addr_t *mac);
extern int arp_pkt_set_spa(arp_packet_t *arp, ipv4_addr_t *ip);
extern int arp_pkt_set_tha(arp_packet_t *arp, mac_addr_t *mac);
extern int arp_pkt_set_tpa(arp_packet_t *arp, ipv4_addr_t *ip);
extern int arp_pkt_get_oper(arp_packet_t *arp, uint16_t *oper);
extern int arp_pkt_get_sha(arp_packet_t *arp, mac_addr_t *mac);
extern int arp_pkt_get_spa(arp_packet_t *arp, ipv4_addr_t *ip);
extern int arp_pkt_get_tha(arp_packet_t *arp, mac_addr_t *mac);
extern int arp_pkt_get_tpa(arp_packet_t *arp, ipv4_addr_t *ip);
extern int arp_pkt_get_len(arp_packet_t *arp);
extern int arp_buf_to_pkt(uint8_t *buf, int len, arp_packet_t *arp);
extern int arp_pkt_to_buf(arp_packet_t *arp, uint8_t *buf);
extern int arp_pkt_is_valid(arp_packet_t *arp);
extern int arp_pkt_is_query(arp_packet_t *arp);
extern int arp_pkt_create(arp_packet_t *arp);
extern int arp_pkt_create_probe(arp_packet_t *arp);
extern int arp_pkt_create_query(ipv4_addr_t *dst_ip, arp_packet_t *arp);
extern int arp_pkt_create_answer(arp_packet_t *arp_in, arp_packet_t *arp_out);
extern int arp_get_last_error(void);

#endif

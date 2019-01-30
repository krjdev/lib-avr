/**
 *
 * File Name: arp.h
 * Title    : ARP definitions and helper functions header
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

#ifndef LIBAVR_NET_ARP_H
#define LIBAVR_NET_ARP_H

#include <stdint.h>

#include "ethernet.h"
#include "ipv4.h"

#define ARP_HTYPE_ETHERNET  0x0001
#define ARP_PTYPE_IPV4      0x0800
#define ARP_HLEN_ETHERNET   6
#define ARP_PLEN_IPV4       4
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
extern int arp_pkt_create(arp_packet_t *arp);
extern int arp_pkt_valid(arp_packet_t *arp);

#endif

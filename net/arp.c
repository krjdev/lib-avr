/**
 *
 * File Name: arp.c
 * Title    : ARP definitions and helper functions source
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

#define _HIGH16(val)   ((uint8_t) (((val) & 0xFF00) >> 8))
#define _LOW16(val)    ((uint8_t) ((val) & 0x00FF))

#include "arp.h"

int arp_pkt_set_oper(arp_packet_t *arp, uint16_t oper)
{
    if (!arp)
        return -1;
    
    arp->ap_oper = oper;
    return 0;
}

int arp_pkt_set_sha(arp_packet_t *arp, mac_addr_t *mac)
{
    if (!arp)
        return -1;
    
    if (!mac)
        return -1;
    
    ethernet_mcpy(&arp->ap_sha, mac);
    return 0;
}

int arp_pkt_set_spa(arp_packet_t *arp, ipv4_addr_t *ip)
{
    if (!arp)
        return -1;
    
    if (!ip)
        return -1;
    
    ipv4_acpy(&arp->ap_spa, ip);
    return 0;
}

int arp_pkt_set_tha(arp_packet_t *arp, mac_addr_t *mac)
{
    if (!arp)
        return -1;
    
    if (!mac)
        return -1;
    
    ethernet_mcpy(&arp->ap_tha, mac);
    return 0;
}

int arp_pkt_set_tpa(arp_packet_t *arp, ipv4_addr_t *ip)
{
    if (!arp)
        return -1;
    
    if (!ip)
        return -1;
    
    ipv4_acpy(&arp->ap_tpa, ip);
    return 0;
}

int arp_pkt_get_oper(arp_packet_t *arp, uint16_t *oper)
{
    if (!arp)
        return -1;
    
    (*oper) = arp->ap_oper;
    return 0;
}

int arp_pkt_get_sha(arp_packet_t *arp, mac_addr_t *mac)
{
    if (!arp)
        return -1;
    
    if (!mac)
        return -1;
    
    ethernet_mcpy(mac, &arp->ap_sha);
    return 0;
}

int arp_pkt_get_spa(arp_packet_t *arp, ipv4_addr_t *ip)
{
    if (!arp)
        return -1;
    
    if (!ip)
        return -1;
    
    ipv4_acpy(ip, &arp->ap_spa);
    return 0;
}

int arp_pkt_get_tha(arp_packet_t *arp, mac_addr_t *mac)
{
    if (!arp)
        return -1;
    
    if (!mac)
        return -1;
    
    ethernet_mcpy(mac, &arp->ap_tha);
    return 0;
}

int arp_pkt_get_tpa(arp_packet_t *arp, ipv4_addr_t *ip)
{
    if (!arp)
        return -1;
    
    if (!ip)
        return -1;
    
    ipv4_acpy(ip, &arp->ap_tpa);
    return 0;
}

int arp_pkt_get_len(arp_packet_t *arp)
{
    if (!arp)
        return -1;
    
    return 28;
}

int arp_buf_to_pkt(uint8_t *buf, int len, arp_packet_t *arp)
{
    int i = 0;
    uint16_t tmp;
    
    if (!buf)
        return -1;
    
    if (len != 28)
        return -1;
    
    if (!arp)
        return -1;
    
    /* Hardware address type */
    tmp = (buf[i++] << 8);
    tmp |= buf[i++];
    arp->ap_htype = tmp;
    /* Protocol address type */
    tmp = (buf[i++] << 8);
    tmp |= buf[i++];
    arp->ap_ptype = tmp;
    /* Hardware address length */
    arp->ap_hlen = buf[i++];
    /* Hardware address length */
    arp->ap_plen = buf[i++];
    /* ARP operation */
    tmp = (buf[i++] << 8);
    tmp |= buf[i++];
    arp->ap_oper = tmp;
    /* Source hardware address */
    arp->ap_sha.ma_byte0 = buf[i++];
    arp->ap_sha.ma_byte1 = buf[i++];
    arp->ap_sha.ma_byte2 = buf[i++];
    arp->ap_sha.ma_byte3 = buf[i++];
    arp->ap_sha.ma_byte4 = buf[i++];
    arp->ap_sha.ma_byte5 = buf[i++];
    /* Source protocol address */
    arp->ap_spa.ia_byte0 = buf[i++];
    arp->ap_spa.ia_byte1 = buf[i++];
    arp->ap_spa.ia_byte2 = buf[i++];
    arp->ap_spa.ia_byte3 = buf[i++];
    /* Destination hardware address */
    arp->ap_tha.ma_byte0 = buf[i++];
    arp->ap_tha.ma_byte1 = buf[i++];
    arp->ap_tha.ma_byte2 = buf[i++];
    arp->ap_tha.ma_byte3 = buf[i++];
    arp->ap_tha.ma_byte4 = buf[i++];
    arp->ap_tha.ma_byte5 = buf[i++];
    /* Destination protocol address */
    arp->ap_tpa.ia_byte0 = buf[i++];
    arp->ap_tpa.ia_byte1 = buf[i++];
    arp->ap_tpa.ia_byte2 = buf[i++];
    arp->ap_tpa.ia_byte3 = buf[i];
    return 0;
}

int arp_pkt_to_buf(arp_packet_t *arp, uint8_t *buf)
{
    int i = 0;
    
    if (!arp)
        return -1;
    
    if (!buf)
        return -1;
    
    /* Hardware address type */
    buf[i++] = _HIGH16(arp->ap_htype);
    buf[i++] = _LOW16(arp->ap_htype);
    /* Protocol address type */
    buf[i++] = _HIGH16(arp->ap_ptype);
    buf[i++] = _LOW16(arp->ap_ptype);
    /* Hardware address length */
    buf[i++] = arp->ap_hlen;
    /* Protocol address length */
    buf[i++] = arp->ap_plen;
    /* ARP operation */
    buf[i++] = _HIGH16(arp->ap_oper);
    buf[i++] = _LOW16(arp->ap_oper);
    /* Source hardware address */
    buf[i++] = arp->ap_sha.ma_byte0;
    buf[i++] = arp->ap_sha.ma_byte1;
    buf[i++] = arp->ap_sha.ma_byte2;
    buf[i++] = arp->ap_sha.ma_byte3;
    buf[i++] = arp->ap_sha.ma_byte4;
    buf[i++] = arp->ap_sha.ma_byte5;
    /* Source protocol address */
    buf[i++] = arp->ap_spa.ia_byte0;
    buf[i++] = arp->ap_spa.ia_byte1;
    buf[i++] = arp->ap_spa.ia_byte2;
    buf[i++] = arp->ap_spa.ia_byte3;
    /* Destination hardware address */
    buf[i++] = arp->ap_tha.ma_byte0;
    buf[i++] = arp->ap_tha.ma_byte1;
    buf[i++] = arp->ap_tha.ma_byte2;
    buf[i++] = arp->ap_tha.ma_byte3;
    buf[i++] = arp->ap_tha.ma_byte4;
    buf[i++] = arp->ap_tha.ma_byte5;
    /* Destination protocol address */
    buf[i++] = arp->ap_tpa.ia_byte0;
    buf[i++] = arp->ap_tpa.ia_byte1;
    buf[i++] = arp->ap_tpa.ia_byte2;
    buf[i] = arp->ap_tpa.ia_byte3;
    return 0;
}

int arp_pkt_create(arp_packet_t *arp)
{
    if (!arp)
        return -1;
    
    arp->ap_htype = ARP_HTYPE_ETHERNET;
    arp->ap_ptype = ARP_PTYPE_IPV4;
    arp->ap_hlen = ARP_HLEN_ETHERNET;
    arp->ap_plen = ARP_PLEN_IPV4;
    return 0;
}

int arp_pkt_valid(arp_packet_t *arp)
{
    if (!arp)
        return -1;
    
    if ((arp->ap_htype == ARP_HTYPE_ETHERNET) && 
        (arp->ap_ptype == ARP_PTYPE_IPV4) && 
        (arp->ap_hlen == ARP_HLEN_ETHERNET) && 
        (arp->ap_plen == ARP_PLEN_IPV4))
        return 1;
    
    return 0;
}

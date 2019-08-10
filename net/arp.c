/**
 *
 * File Name: arp.c
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

#define ARP_PKT_LEN         28

#define ARP_HTYPE_ETHERNET  0x0001
#define ARP_PTYPE_IPV4      0x0800
#define ARP_HLEN_ETHERNET   6
#define ARP_PLEN_IPV4       4

#define HI16(val)       ((uint8_t) (((val) & 0xFF00) >> 8))
#define LO16(val)       ((uint8_t) ((val) & 0x00FF))

#include "arp.h"

static int error = ARP_ERROR_SUCCESS;
static mac_addr_t me_mac;
static ipv4_addr_t me_ip;

int arp_init(mac_addr_t *src_mac, ipv4_addr_t *src_ip)
{
    if (!src_mac) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!src_ip) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ethernet_addr_cpy(&me_mac, src_mac);
    ipv4_addr_cpy(&me_ip, src_ip);
    return 0;
}

int arp_pkt_set_oper(arp_packet_t *arp, uint16_t oper)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    arp->ap_oper = oper;
    return 0;
}

int arp_pkt_set_sha(arp_packet_t *arp, mac_addr_t *mac)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ethernet_addr_cpy(&arp->ap_sha, mac);
    return 0;
}

int arp_pkt_set_spa(arp_packet_t *arp, ipv4_addr_t *ip)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!ip) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ipv4_addr_cpy(&arp->ap_spa, ip);
    return 0;
}

int arp_pkt_set_tha(arp_packet_t *arp, mac_addr_t *mac)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ethernet_addr_cpy(&arp->ap_tha, mac);
    return 0;
}

int arp_pkt_set_tpa(arp_packet_t *arp, ipv4_addr_t *ip)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!ip) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ipv4_addr_cpy(&arp->ap_tpa, ip);
    return 0;
}
// TODO
int arp_pkt_get_oper(arp_packet_t *arp, uint16_t *oper)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    (*oper) = arp->ap_oper;
    return 0;
}

int arp_pkt_get_sha(arp_packet_t *arp, mac_addr_t *mac)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ethernet_addr_cpy(mac, &arp->ap_sha);
    return 0;
}

int arp_pkt_get_spa(arp_packet_t *arp, ipv4_addr_t *ip)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!ip) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ipv4_addr_cpy(ip, &arp->ap_spa);
    return 0;
}

int arp_pkt_get_tha(arp_packet_t *arp, mac_addr_t *mac)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ethernet_addr_cpy(mac, &arp->ap_tha);
    return 0;
}

int arp_pkt_get_tpa(arp_packet_t *arp, ipv4_addr_t *ip)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!ip) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    ipv4_addr_cpy(ip, &arp->ap_tpa);
    return 0;
}

int arp_pkt_get_len(arp_packet_t *arp)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    return ARP_PKT_LEN;
}

int arp_buf_to_pkt(uint8_t *buf, int len, arp_packet_t *arp)
{
    int i = 0;
    uint16_t tmp;
    
    if (!buf) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (len != ARP_PKT_LEN) {
        error = ARP_ERROR_UNKNOWN;
        return -1;
    }
    
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
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
    
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!buf) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    /* Hardware address type */
    buf[i++] = HI16(arp->ap_htype);
    buf[i++] = LO16(arp->ap_htype);
    /* Protocol address type */
    buf[i++] = HI16(arp->ap_ptype);
    buf[i++] = LO16(arp->ap_ptype);
    /* Hardware address length */
    buf[i++] = arp->ap_hlen;
    /* Protocol address length */
    buf[i++] = arp->ap_plen;
    /* ARP operation */
    buf[i++] = HI16(arp->ap_oper);
    buf[i++] = LO16(arp->ap_oper);
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

int arp_pkt_is_valid(arp_packet_t *arp)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if ((arp->ap_htype == ARP_HTYPE_ETHERNET) && 
        (arp->ap_ptype == ARP_PTYPE_IPV4) && 
        (arp->ap_hlen == ARP_HLEN_ETHERNET) && 
        (arp->ap_plen == ARP_PLEN_IPV4))
        return 1;
    
    return 0;
}

int arp_pkt_is_query(arp_packet_t *arp)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (arp->ap_oper == ARP_OPER_QUERY)
        return 1;
    
    return 0;
}

int arp_pkt_create(arp_packet_t *arp)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    arp->ap_htype = ARP_HTYPE_ETHERNET;
    arp->ap_ptype = ARP_PTYPE_IPV4;
    arp->ap_hlen = ARP_HLEN_ETHERNET;
    arp->ap_plen = ARP_PLEN_IPV4;
    return 0;
}

int arp_pkt_create_probe(arp_packet_t *arp)
{
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    arp->ap_htype = ARP_HTYPE_ETHERNET;
    arp->ap_ptype = ARP_PTYPE_IPV4;
    arp->ap_hlen = ARP_HLEN_ETHERNET;
    arp->ap_plen = ARP_PLEN_IPV4;
    arp->ap_oper = ARP_OPER_QUERY;
    ethernet_addr_cpy(&arp->ap_sha, &me_mac);
    arp->ap_spa.ia_byte0 = 0;
    arp->ap_spa.ia_byte1 = 0;
    arp->ap_spa.ia_byte2 = 0;
    arp->ap_spa.ia_byte3 = 0;
    arp->ap_tha.ma_byte0 = 0x00;
    arp->ap_tha.ma_byte1 = 0x00;
    arp->ap_tha.ma_byte2 = 0x00;
    arp->ap_tha.ma_byte3 = 0x00;
    arp->ap_tha.ma_byte4 = 0x00;
    arp->ap_tha.ma_byte5 = 0x00;
    ipv4_addr_cpy(&arp->ap_tpa, &me_ip);
    return 0;
    
}

int arp_pkt_create_query(ipv4_addr_t *dst_ip, arp_packet_t *arp)
{
    if (!dst_ip) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!arp) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    arp->ap_htype = ARP_HTYPE_ETHERNET;
    arp->ap_ptype = ARP_PTYPE_IPV4;
    arp->ap_hlen = ARP_HLEN_ETHERNET;
    arp->ap_plen = ARP_PLEN_IPV4;
    arp->ap_oper = ARP_OPER_QUERY;
    ethernet_addr_cpy(&arp->ap_sha, &me_mac);
    ipv4_addr_cpy(&arp->ap_spa, &me_ip);
    arp->ap_tha.ma_byte0 = 0x00;
    arp->ap_tha.ma_byte1 = 0x00;
    arp->ap_tha.ma_byte2 = 0x00;
    arp->ap_tha.ma_byte3 = 0x00;
    arp->ap_tha.ma_byte4 = 0x00;
    arp->ap_tha.ma_byte5 = 0x00;
    ipv4_addr_cpy(&arp->ap_tpa, dst_ip);
    return 0;
}

int arp_pkt_create_answer(arp_packet_t *arp_in, arp_packet_t *arp_out)
{
    if (!arp_in) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    if (!arp_out) {
        error = ARP_ERROR_INVAL;
        return -1;
    }
    
    arp_out->ap_htype = ARP_HTYPE_ETHERNET;
    arp_out->ap_ptype = ARP_PTYPE_IPV4;
    arp_out->ap_hlen = ARP_HLEN_ETHERNET;
    arp_out->ap_plen = ARP_PLEN_IPV4;
    arp_out->ap_oper = ARP_OPER_ANSWE;
    ethernet_addr_cpy(&arp_out->ap_sha, &me_mac);
    ipv4_addr_cpy(&arp_out->ap_spa, &me_ip);
    ethernet_addr_cpy(&arp_out->ap_tha, &arp_in->ap_sha);
    ipv4_addr_cpy(&arp_out->ap_tpa, &arp_in->ap_tpa);
    return 0;
}

int arp_get_last_error(void)
{
    int err;
    
    err = error;
    error = ARP_ERROR_SUCCESS;
    return err;
}

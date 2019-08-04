/**
 *
 * File Name: ipv6.c
 * Title    : IPv6 library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-08-04
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

#include <stdlib.h>
#include <string.h>

#include "ipv6.h"

#define IPV6_HDR_LEN    40

#define HI16(val)       ((uint8_t) (((val) & 0xFF00) >> 8))
#define LO16(val)       ((uint8_t) ((val) & 0x00FF))

int ipv6_addr_equal(ipv6_addr_t *ia1, ipv6_addr_t *ia2)
{
    if (!ia1)
        return -1;
    
    if (!ia2)
        return -1;
    
    if ((ia1->ia_byte00 == ia2->ia_byte00) && 
        (ia1->ia_byte01 == ia2->ia_byte01) && 
        (ia1->ia_byte02 == ia2->ia_byte02) && 
        (ia1->ia_byte03 == ia2->ia_byte03) && 
        (ia1->ia_byte04 == ia2->ia_byte04) && 
        (ia1->ia_byte05 == ia2->ia_byte05) && 
        (ia1->ia_byte06 == ia2->ia_byte06) && 
        (ia1->ia_byte07 == ia2->ia_byte07) && 
        (ia1->ia_byte08 == ia2->ia_byte08) && 
        (ia1->ia_byte09 == ia2->ia_byte09) && 
        (ia1->ia_byte10 == ia2->ia_byte10) && 
        (ia1->ia_byte11 == ia2->ia_byte11) && 
        (ia1->ia_byte12 == ia2->ia_byte12) && 
        (ia1->ia_byte13 == ia2->ia_byte13) && 
        (ia1->ia_byte14 == ia2->ia_byte14) && 
        (ia1->ia_byte15 == ia2->ia_byte15))
        return 1;
    
    return 0;
}

int ipv6_addr_cpy(ipv6_addr_t *ia_dst, ipv6_addr_t *ia_src)
{
    if (!ia_dst)
        return -1;
    
    if (!ia_src)
        return -1;
    
    ia_dst->ia_byte00 = ia_src->ia_byte00;
    ia_dst->ia_byte00 = ia_src->ia_byte01;
    ia_dst->ia_byte00 = ia_src->ia_byte02;
    ia_dst->ia_byte00 = ia_src->ia_byte03;
    ia_dst->ia_byte00 = ia_src->ia_byte04;
    ia_dst->ia_byte00 = ia_src->ia_byte05;
    ia_dst->ia_byte00 = ia_src->ia_byte06;
    ia_dst->ia_byte00 = ia_src->ia_byte07;
    ia_dst->ia_byte00 = ia_src->ia_byte08;
    ia_dst->ia_byte00 = ia_src->ia_byte09;
    ia_dst->ia_byte00 = ia_src->ia_byte10;
    ia_dst->ia_byte00 = ia_src->ia_byte11;
    ia_dst->ia_byte00 = ia_src->ia_byte12;
    ia_dst->ia_byte00 = ia_src->ia_byte13;
    ia_dst->ia_byte00 = ia_src->ia_byte14;
    ia_dst->ia_byte00 = ia_src->ia_byte15;
    return 0;
}

int ipv6_pkt_set_flow(ipv6_packet_t *ip, uint32_t flow)
{
    if (!ip)
        return -1;
    
    ip->ip_hdr.ih_flow = 0xFFFFF & flow;
    return 0;
}

int ipv6_pkt_set_nhdr(ipv6_packet_t *ip, uint8_t nhdr)
{
    if (!ip)
        return -1;
    
    ip->ip_hdr.ih_nhdr = nhdr;
    return 0;
}

int ipv6_pkt_set_hopl(ipv6_packet_t *ip, uint8_t hopl)
{
    if (!ip)
        return -1;
    
    ip->ip_hdr.ih_hopl = hopl;
    return 0;
}

int ipv6_pkt_set_src(ipv6_packet_t *ip, ipv6_addr_t *src)
{
    if (!ip)
        return -1;
    
    ipv6_addr_cpy(&ip->ip_hdr.ih_src, src);
    return 0;
}

int ipv6_pkt_set_dst(ipv6_packet_t *ip, ipv6_addr_t *dst)
{
    if (!ip)
        return -1;
    
    ipv6_addr_cpy(&ip->ip_hdr.ih_dst, dst);
    return 0;
}

int ipv6_pkt_set_payload(ipv6_packet_t *ip, uint8_t *buf, int len)
{
    if (!ip)
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    ip->ip_hdr.ih_plen = len;
    ip->ip_payload_len = len;
    memcpy(ip->ip_payload_buf, buf, ip->ip_payload_len); 
    return 0;
}

int ipv6_pkt_get_flow(ipv6_packet_t *ip, uint32_t *flow)
{
    if (!ip)
        return -1;
    
    (*flow) = ip->ip_hdr.ih_flow;
    return 0;
}

int ipv6_pkt_get_nhdr(ipv6_packet_t *ip, uint8_t *nhdr)
{
    if (!ip)
        return -1;
    
    (*nhdr) = ip->ip_hdr.ih_nhdr;
    return 0;
}

int ipv6_pkt_get_hopl(ipv6_packet_t *ip, uint8_t *hopl)
{
    if (!ip)
        return -1;
    
    (*hopl) = ip->ip_hdr.ih_hopl;
    return 0;
}

int ipv6_pkt_get_src(ipv6_packet_t *ip, ipv6_addr_t *src)
{
    if (!ip)
        return -1;
    
    ipv6_addr_cpy(src, &ip->ip_hdr.ih_src);
    return 0;
}

int ipv6_pkt_get_dst(ipv6_packet_t *ip, ipv6_addr_t *dst)
{
    if (!ip)
        return -1;
    
    ipv6_addr_cpy(dst, &ip->ip_hdr.ih_dst);
    return 0;
}

int ipv6_pkt_get_payload_len(ipv6_packet_t *ip)
{
    if (!ip)
        return -1;
    
    return ip->ip_payload_len;
}

int ipv6_pkt_get_payload(ipv6_packet_t *ip, uint8_t **buf)
{
    if (!ip)
        return -1;
    
    (*buf) = ip->ip_payload_buf;
    return 0;
}

int ipv6_pkt_create_empty(ipv6_packet_t *ip)
{
    if (!ip)
        return -1;
    
    ip->ip_hdr.ih_ver = 6;
    ip->ip_hdr.ih_ecn = 0;
    ip->ip_hdr.ih_dscp = 0;
    return 0;
}

int ipv6_pkt_free(ipv6_packet_t *ip)
{
    if (!ip)
        return -1;
    
    if (ip->ip_payload_buf) {
        free(ip->ip_payload_buf);
        ip->ip_payload_buf = NULL;
        ip->ip_payload_len = 0;
    }
    
    return 0;
}

int ipv6_pkt_get_len(ipv6_packet_t *ip)
{
    if (!ip)
        return -1;
    
    return (IPV6_HDR_LEN + ip->ip_payload_len);
}

int ipv6_buf_to_pkt(uint8_t *buf, int len, ipv6_packet_t *ip)
{
    int i = 0;
    uint8_t *p = NULL;
    int plen;
    uint8_t tmp8;
    uint16_t tmp16;
    uint32_t tmp32;
    
    if (!buf)
        return -1;
    
    if (len < IPV6_HDR_LEN)
        return -1;
    
    if (!ip)
        return -1;
    
    /* Header */
    tmp8 = buf[i++];
    ip->ip_hdr.ih_ver = (tmp8 >> 4);
    
    if (ip->ip_hdr.ih_ver != 6)
        return -1;
    
    ip->ip_hdr.ih_ecn = ((tmp8 >> 2) & 0x03);
    tmp32 = buf[i++];
    ip->ip_hdr.ih_dscp = (((tmp8 << 4) & 0x30) | ((uint8_t) tmp32 >> 4));
    tmp32 = ((tmp32 << 16) & 0x0F0000);
    tmp8 = buf[i++];
    tmp32 |= ((uint32_t) tmp8 << 8);
    tmp8 = buf[i++];
    tmp32 |= tmp8;
    ip->ip_hdr.ih_flow = tmp32;
    tmp16 = ((uint16_t) buf[i++] << 8);
    tmp16 |= buf[i++];
    
    if (tmp16 != (len - IPV6_HDR_LEN))
        return -1;
    
    ip->ip_hdr.ih_plen = tmp16;
    ip->ip_hdr.ih_nhdr = buf[i++];
    ip->ip_hdr.ih_hopl = buf[i++];
    ip->ip_hdr.ih_src.ia_byte00 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte01 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte02 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte03 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte04 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte05 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte06 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte07 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte08 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte09 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte10 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte11 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte12 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte13 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte14 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte15 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte00 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte01 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte02 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte03 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte04 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte05 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte06 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte07 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte08 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte09 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte10 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte11 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte12 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte13 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte14 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte15 = buf[i++];
    
    /* Payload */
    if (len > IPV6_HDR_LEN) {
        plen = len - IPV6_HDR_LEN;
        p = (uint8_t *) malloc(plen);
        
        if (!p)
            return -1;
        
        memcpy(p, &buf[i], plen);
        ip->ip_payload_buf = p;
        ip->ip_payload_len = plen;
    }
    
    return 0;
}

int ipv6_pkt_to_buf(ipv6_packet_t *ip, uint8_t *buf)
{
    int i = 0;
    
    if (ip)
        return -1;
    
    if (buf)
        return -1;
    
    /* Header */
    buf[i++] = ((ip->ip_hdr.ih_ver << 4) | (ip->ip_hdr.ih_ecn << 2) | (ip->ip_hdr.ih_dscp >> 4));
    buf[i++] = ((ip->ip_hdr.ih_dscp << 6) | (ip->ip_hdr.ih_flow >> 16));
    buf[i++] = ((ip->ip_hdr.ih_flow >> 8) & 0xFF);
    buf[i++] = (ip->ip_hdr.ih_flow & 0xFF);
    buf[i++] = HI16(ip->ip_hdr.ih_plen);
    buf[i++] = LO16(ip->ip_hdr.ih_plen);
    buf[i++] = ip->ip_hdr.ih_nhdr;
    buf[i++] = ip->ip_hdr.ih_hopl;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte00;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte01;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte02;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte03;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte04;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte05;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte06;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte07;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte08;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte09;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte10;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte11;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte12;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte13;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte14;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte15;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte00;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte01;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte02;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte03;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte04;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte05;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte06;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte07;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte08;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte09;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte10;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte11;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte12;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte13;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte14;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte15;
    
    /* Payload */
    if (ip->ip_payload_len > 0) {
        if (!ip->ip_payload_buf)
            return -1;
        
        memcpy(&buf[i], ip->ip_payload_buf, ip->ip_payload_len);
    }
    
    return 0;
}

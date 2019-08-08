/**
 *
 * File Name: udp.c
 * Title    : UDP library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-01-30
 * Modified : 2019-08-09
 * Revised  : 
 * Version  : 0.2.0.2
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
#include <stdio.h>

#include "udp.h"

#define _HIGH16(val)    ((uint8_t) (((val) & 0xFF00) >> 8))
#define _LOW16(val)     ((uint8_t) ((val) & 0x00FF))

static uint32_t udp_sum(udp_packet_t *udp)
{
    int i = 0;
    int j;
    uint32_t sum = 0;
    uint16_t tmp;
    
    if (!udp)
        return 0;
    
    sum += udp->up_hdr.uh_srcp;
    sum += udp->up_hdr.uh_dstp;
    sum += udp->up_hdr.uh_len;
    
    if (udp->up_payload_len > 0) {
        if (!udp->up_payload_buf)
            return 0;
        
        for (j = 0; j < udp->up_payload_len / 2; j++ ) {
            tmp = ((uint16_t) udp->up_payload_buf[i++] << 8);
            tmp |= udp->up_payload_buf[i++];
            sum += tmp;
        }
        
        if (udp->up_payload_len & 1) {
            tmp = ((uint16_t) udp->up_payload_buf[i] << 8);
            sum += tmp;
        }
    }
    
    return sum;
}

int udp_pkt_set_srcp(udp_packet_t *udp, uint16_t srcp)
{
    if (!udp)
        return -1;
    
    udp->up_hdr.uh_srcp = srcp;
    return 0;
}

int udp_pkt_set_dstp(udp_packet_t *udp, uint16_t dstp)
{
    if (!udp)
        return -1;
    
    udp->up_hdr.uh_dstp = dstp;
    return 0;
}

int udp_pkt_set_payload(udp_packet_t *udp, uint8_t *buf, int len)
{
    uint8_t *p;
    
    if (!udp)
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    p = (uint8_t *) malloc(len);
    
    if (!p)
        return -1;
    
    memcpy(p, buf, len);
    udp->up_hdr.uh_len += len;
    udp->up_payload_buf = p;
    udp->up_payload_len = len;
    return 0;
}

int udp_pkt_get_srcp(udp_packet_t *udp, uint16_t *srcp)
{
    if (!udp)
        return -1;
    
    (*srcp) = udp->up_hdr.uh_srcp;
    return 0;
}

int udp_pkt_get_dstp(udp_packet_t *udp, uint16_t *dstp)
{
    if (!udp)
        return -1;
    
    (*dstp) = udp->up_hdr.uh_dstp;
    return 0;
}

int udp_pkt_get_payload_len(udp_packet_t *udp)
{
    if (!udp)
        return -1;
    
    return udp->up_payload_len;
}

int udp_pkt_get_payload(udp_packet_t *udp, uint8_t **buf)
{
    if (!udp)
        return -1;
    
    if (!buf)
        return -1;
    
    (*buf) = udp->up_payload_buf;
    return 0;
}

int udp_pkt_get_len(udp_packet_t *udp)
{
    if (!udp)
        return -1;
    
    return udp->up_hdr.uh_len;
}

int udp_pkt_create(uint16_t srcp, uint16_t dstp, 
                   uint8_t *buf, int len, 
                   udp_packet_t *udp)
{
    if (!buf)
        return -1;
    
    if (!udp)
        return -1;
    
    udp->up_hdr.uh_srcp = srcp;
    udp->up_hdr.uh_dstp = dstp;
    udp->up_hdr.uh_len = 8;
    udp_pkt_set_payload(udp, buf, len);
    return 0;
}

int udp_pkt_free(udp_packet_t *udp)
{
    if (!udp)
        return -1;
    
    if (udp->up_payload_len > 0) {
        if (!udp->up_payload_buf)
            return -1;
        
        free(udp->up_payload_buf);
    }
    
    return 0;
}

int udp_pkt_to_ip(udp_packet_t *udp, ipv4_packet_t *ip_udp)
{
    uint32_t sum = 0;
    uint16_t tmp;
    uint16_t carry;
    uint8_t *p;
    uint8_t *p_payl;
    int i = 0;
    
    if (!udp)
        return -1;
    
    if (!ip_udp)
        return -1;
    
    tmp = ((uint16_t) ip_udp->ip_hdr.ih_src.ia_byte0 << 8);
    tmp |= (uint16_t) ip_udp->ip_hdr.ih_src.ia_byte1;
    sum += tmp;
    tmp = ((uint16_t) ip_udp->ip_hdr.ih_src.ia_byte2 << 8);
    tmp |= (uint16_t) ip_udp->ip_hdr.ih_src.ia_byte3;
    sum += tmp;
    tmp = ((uint16_t) ip_udp->ip_hdr.ih_dst.ia_byte0 << 8);
    tmp |= (uint16_t) ip_udp->ip_hdr.ih_dst.ia_byte1;
    sum += tmp;
    tmp = ((uint16_t) ip_udp->ip_hdr.ih_dst.ia_byte2 << 8);
    tmp |= (uint16_t) ip_udp->ip_hdr.ih_dst.ia_byte3;
    sum += tmp;
    tmp = ip_udp->ip_hdr.ih_prot;
    sum += tmp;
    sum += udp_sum(udp);
    carry = (uint16_t) (sum >> 16);
    sum += carry;
    sum = ~sum;
    udp->up_hdr.uh_chk = (uint16_t) sum;
    p = (uint8_t *) malloc(udp->up_hdr.uh_len);
    
    if (!p)
        return -1;
    
    p[i++] = _HIGH16(udp->up_hdr.uh_srcp);
    p[i++] = _LOW16(udp->up_hdr.uh_srcp);
    p[i++] = _HIGH16(udp->up_hdr.uh_dstp);
    p[i++] = _LOW16(udp->up_hdr.uh_dstp);
    p[i++] = _HIGH16(udp->up_hdr.uh_len);
    p[i++] = _LOW16(udp->up_hdr.uh_len);
    p[i++] = _HIGH16(udp->up_hdr.uh_chk);
    p[i++] = _LOW16(udp->up_hdr.uh_chk);
    
    if (udp_pkt_get_payload(udp, &p_payl) == -1) {
        free(p);
        return -1;
    }
    
    memcpy(&p[i], p_payl, udp_pkt_get_payload_len(udp));
    
    if (ipv4_pkt_set_payload(ip_udp, p, udp->up_hdr.uh_len) == -1) {
        free(p);
        return -1;
    }
    
    free(p);
    return 0;
}

int udp_ip_to_pkt(ipv4_packet_t *ip_udp, udp_packet_t *udp)
{
    uint8_t *p;
    uint32_t sum = 0;
    uint16_t tmp;
    uint16_t carry;
    uint16_t chk;
    int i = 0;
    int len;
    
    if (!ip_udp)
        return -1;
    
    if (!udp)
        return -1;
    
    tmp = ((uint16_t) ip_udp->ip_hdr.ih_src.ia_byte0 << 8);
    tmp |= (uint16_t) ip_udp->ip_hdr.ih_src.ia_byte1;
    sum += tmp;
    tmp = ((uint16_t) ip_udp->ip_hdr.ih_src.ia_byte2 << 8);
    tmp |= (uint16_t) ip_udp->ip_hdr.ih_src.ia_byte3;
    sum += tmp;
    tmp = ((uint16_t) ip_udp->ip_hdr.ih_dst.ia_byte0 << 8);
    tmp |= (uint16_t) ip_udp->ip_hdr.ih_dst.ia_byte1;
    sum += tmp;
    tmp = ((uint16_t) ip_udp->ip_hdr.ih_dst.ia_byte2 << 8);
    tmp |= (uint16_t) ip_udp->ip_hdr.ih_dst.ia_byte3;
    sum += tmp;
    tmp = ip_udp->ip_hdr.ih_prot;
    sum += tmp;
    len = ipv4_pkt_get_payload_len(ip_udp);
    
    if (len < 8)
        return -1;
    
    ipv4_pkt_get_payload(ip_udp, &p);
    udp->up_hdr.uh_srcp = ((uint16_t) p[i++] << 8);
    udp->up_hdr.uh_srcp |= (uint16_t) p[i++];
    udp->up_hdr.uh_dstp = ((uint16_t) p[i++] << 8);
    udp->up_hdr.uh_dstp |= (uint16_t) p[i++];
    udp->up_hdr.uh_len = ((uint16_t) p[i++] << 8);
    udp->up_hdr.uh_len |= (uint16_t) p[i++];
    udp->up_hdr.uh_chk = ((uint16_t) p[i++] << 8);
    udp->up_hdr.uh_chk |= (uint16_t) p[i++];
    udp_pkt_set_payload(udp, &p[i], len);
    sum += udp_sum(udp);
    carry = (uint16_t) (sum >> 16);
    sum += carry;
    sum = ~sum;
    chk = (uint16_t) sum;
    
    if (udp->up_hdr.uh_chk != chk) {
        udp_pkt_free(udp);
        return -1;
    }
    
    return 0;
}

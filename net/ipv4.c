/**
 *
 * File Name: ipv4.c
 * Title    : IPv4 library
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
 * Modified : 2019-06-22
 * Revised  : 
 * Version  : 0.5.0.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ipv4.h"

#define FLAG_DF         1
#define FLAG_MF         2

#define _HIGH16(val)    ((uint8_t) (((val) & 0xFF00) >> 8))
#define _LOW16(val)     ((uint8_t) ((val) & 0x00FF))

ipv4_range_t rfc6890[] = {
    { { 0, 0, 0, 0 }, 8 }, 
    { { 10, 0, 0, 0 }, 8 }, 
    { { 100, 64, 0, 0 }, 10 }, 
    { { 127, 0, 0, 0 }, 8 }, 
    { { 169, 254, 0, 0 }, 16 }, 
    { { 172, 16, 0, 0 }, 12 }, 
    { { 192, 0, 0, 0 }, 24 }, 
    { { 192, 0, 0, 0 }, 29 }, 
    { { 192, 0, 2, 0 }, 24 }, 
    { { 192, 88, 99, 0 }, 24 }, 
    { { 192, 168, 0, 0 }, 16 }, 
    { { 198, 18, 0, 0 }, 15 },
    { { 198, 51, 100, 0 }, 24 }, 
    { { 203, 0, 113, 0 }, 24 }, 
    { { 224, 0, 0, 0 }, 4 }, 
    { { 240, 0, 0, 0 }, 4 },
    { { 255, 255, 255, 255 }, 32 }
};

static uint32_t pkt_hdr_sum(ipv4_packet_t *ip)
{
    int i = 0;
    int j;
    uint32_t sum = 0;
    uint16_t tmp;
    
    if (!ip)
        return 0;
    
    tmp = ((uint16_t) ((ip->ip_hdr.ih_ver << 4) | ip->ip_hdr.ih_ihl) << 8);
    tmp |= (uint16_t) ((ip->ip_hdr.ih_dscp << 2) | ip->ip_hdr.ih_ecn);
    sum += tmp;
    tmp = (uint16_t) (_HIGH16(ip->ip_hdr.ih_tlen) << 8);
    tmp |= (uint16_t) _LOW16(ip->ip_hdr.ih_tlen);
    sum += tmp;
    tmp = (uint16_t) (_HIGH16(ip->ip_hdr.ih_id) << 8);
    tmp |= (uint16_t) _LOW16(ip->ip_hdr.ih_id);
    sum += tmp;
    tmp = ((((uint16_t) ip->ip_hdr.ih_flag << 13)) | (uint16_t) ip->ip_hdr.ih_foff);
    sum += tmp;
    tmp = ((uint16_t) ip->ip_hdr.ih_ttl << 8);
    tmp |= (uint16_t) ip->ip_hdr.ih_prot;
    sum += tmp;
    tmp = ((uint16_t) ip->ip_hdr.ih_src.ia_byte0 << 8);
    tmp |= (uint16_t) ip->ip_hdr.ih_src.ia_byte1;
    sum += tmp;
    tmp = ((uint16_t) ip->ip_hdr.ih_src.ia_byte2 << 8);
    tmp |= (uint16_t) ip->ip_hdr.ih_src.ia_byte3;
    sum += tmp;
    tmp = ((uint16_t) ip->ip_hdr.ih_dst.ia_byte0 << 8);
    tmp |= (uint16_t) ip->ip_hdr.ih_dst.ia_byte1;
    sum += tmp;
    tmp = ((uint16_t) ip->ip_hdr.ih_dst.ia_byte2 << 8);
    tmp |= (uint16_t) ip->ip_hdr.ih_dst.ia_byte3;
    sum += tmp;
    
    if (ip->ip_options_len > 0) {
        if (!ip->ip_options_buf)
            return 0;
        
        for (j = 0; j < ip->ip_options_len / 2; j++ ) {
            tmp = ((uint16_t) ip->ip_options_buf[i++] << 8);
            tmp |= ip->ip_options_buf[i++];
            sum += tmp;
        }
    }
    
    return sum;
}

static int pkt_hdr_append_checksum(ipv4_packet_t *ip)
{
    uint32_t sum;
    uint16_t carry;
    
    if (!ip)
        return -1;
    
    sum = pkt_hdr_sum(ip);
    carry = (uint16_t) (sum >> 16);
    sum += carry;
    sum = ~sum;
    ip->ip_hdr.ih_chk = (uint16_t) sum;
    return 0;
}

static int pkt_hdr_verify_checksum(ipv4_packet_t *ip)
{
    uint32_t sum;
    uint16_t carry;
    uint16_t chk;
    
    if (!ip)
        return -1;
    
    sum = pkt_hdr_sum(ip);
    carry = (uint16_t) (sum >> 16);
    sum += carry;
    chk = (uint16_t) ~sum;
    
    if (ip->ip_hdr.ih_chk == chk)
        return 1;
    
    return 0;
}

int ipv4_addr_aton(const char *str, ipv4_addr_t *ia)
{
    char tmp[4];
    int i = 0;
    int start, end, cnt;
    
    if (!str)
        return -1;
    
    if (!ia)
        return -1;
    
    if (strlen(str) > 15 || strlen(str) < 7)
        return -1;
    
    start = i;
    cnt = i;
    
    while (str[i] != '\0') {
        if (isdigit(str[i]))
            i++;
        else {
            end = i;
            cnt++;
            i++;
            memcpy(&tmp, &str[start], (end - start));
            tmp[end - start] = '\0';
            start = i;
            
            switch (cnt) {
            case 1:
                sscanf(tmp, "%hhu", &ia->ia_byte0);
                break;
            case 2:
                sscanf(tmp, "%hhu", &ia->ia_byte1);
                break;
            case 3:
                sscanf(tmp, "%hhu", &ia->ia_byte2);
                break;
            default:
                break;
            }
        }
    }
    
    end = i;
    memcpy(&tmp, &str[start], (end - start));
    tmp[end - start] = '\0';
    sscanf(tmp, "%hhu", &ia->ia_byte3);
    return 0;
}

int ipv4_addr_ntoa(ipv4_addr_t *ia, char *str)
{
    if (!ia)
        return -1;
    
    if (!str)
        return -1;
    
    sprintf(str, "%hhu.%hhu.%hhu.%hhu", ia->ia_byte0, ia->ia_byte1, 
            ia->ia_byte2, ia->ia_byte3);
    return 0;
}

int ipv4_addr_equal(ipv4_addr_t *ia1, ipv4_addr_t *ia2)
{
    if (!ia1)
        return -1;
    
    if (!ia2)
        return -1;
    
    if ((ia1->ia_byte0 == ia2->ia_byte0) && 
        (ia1->ia_byte1 == ia2->ia_byte1) && 
        (ia1->ia_byte2 == ia2->ia_byte2) && 
        (ia1->ia_byte3 == ia2->ia_byte3))
        return 1;
    
    return 0;
}

int ipv4_addr_cpy(ipv4_addr_t *ia_dst, ipv4_addr_t *ia_src)
{
    if (!ia_dst)
        return -1;
    
    if (!ia_src)
        return -1;
    
    ia_dst->ia_byte0 = ia_src->ia_byte0;
    ia_dst->ia_byte1 = ia_src->ia_byte1;
    ia_dst->ia_byte2 = ia_src->ia_byte2;
    ia_dst->ia_byte3 = ia_src->ia_byte3;
    return 0;
}

int ipv4_addr_is_broadcast(ipv4_addr_t *ia)
{
    if (!ia)
        return -1;
    
    if ((ia->ia_byte0 == 0xFF) && 
        (ia->ia_byte1 == 0xFF) && 
        (ia->ia_byte2 == 0xFF) && 
        (ia->ia_byte3 == 0xFF))
        return 1;
    
    return 0;
}

int ipv4_addr_is_localhost(ipv4_addr_t *ia)
{
    if (!ia)
        return -1;
    
    if ((ia->ia_byte0 == 127) && 
        (ia->ia_byte1 == 0) && 
        (ia->ia_byte2 == 0) && 
        (ia->ia_byte3 == 1))
        return 1;
    
    return 0;
}

int ipv4_pkt_create_empty(ipv4_packet_t *ip)
{
    if (!ip)
        return -1;
    
    ip->ip_hdr.ih_ver = 4;
    ip->ip_hdr.ih_ihl = 5;
    ip->ip_hdr.ih_dscp = 0;
    ip->ip_hdr.ih_ecn = 0;
    ip->ip_hdr.ih_tlen = 20;
    ip->ip_hdr.ih_id = 0;
    ip->ip_hdr.ih_flag = (1 << FLAG_DF);
    ip->ip_hdr.ih_foff = 0;
    ip->ip_hdr.ih_ttl = 64;
    ip->ip_hdr.ih_prot = 0;
    ip->ip_options_buf = NULL;
    ip->ip_options_len = 0;
    ip->ip_payload_buf = NULL;
    ip->ip_payload_len = 0;
    pkt_hdr_append_checksum(ip);
    return 0;
}

int ipv4_pkt_free(ipv4_packet_t *ip)
{
    if (!ip)
        return -1;
    
    if ((ip->ip_options_len > 0) && (ip->ip_options_buf != NULL)) {
        free(ip->ip_options_buf);
        ip->ip_options_len = 0;
    }
    
    if ((ip->ip_payload_len > 0) && (ip->ip_payload_buf != NULL)) {
        free(ip->ip_payload_buf);
        ip->ip_payload_len = 0;
    }
    
    return 0;
}

int ipv4_pkt_set_id(ipv4_packet_t *ip, uint16_t id)
{
    if (!ip)
        return -1;
    
    ip->ip_hdr.ih_id = id;
    pkt_hdr_append_checksum(ip);
    return 0;
}

int ipv4_pkt_set_ttl(ipv4_packet_t *ip, uint8_t ttl)
{
    if (!ip)
        return -1;
    
    ip->ip_hdr.ih_ttl = ttl;
    pkt_hdr_append_checksum(ip);
    return 0;
}

int ipv4_pkt_set_prot(ipv4_packet_t *ip, uint8_t prot)
{
    if (!ip)
        return -1;
    
    ip->ip_hdr.ih_prot = prot;
    pkt_hdr_append_checksum(ip);
    return 0;
}

int ipv4_pkt_set_src(ipv4_packet_t *ip, ipv4_addr_t *src)
{
    if (!ip)
        return -1;
    
    ipv4_addr_cpy(&ip->ip_hdr.ih_src, src);
    pkt_hdr_append_checksum(ip);
    return 0;
}

int ipv4_pkt_set_dst(ipv4_packet_t *ip, ipv4_addr_t *dst)
{
    if (!ip)
        return -1;
    
    ipv4_addr_cpy(&ip->ip_hdr.ih_dst, dst);
    pkt_hdr_append_checksum(ip);
    return 0;
}

int ipv4_pkt_set_options(ipv4_packet_t *ip, uint8_t *buf, int len)
{
    uint8_t *p;
    
    if (!ip)
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (len % 4)
        return -1;
    
    p = (uint8_t *) malloc(len);
    
    if (!p)
        return -1;
    
    memcpy(p, buf, len);
    ip->ip_hdr.ih_ihl += len / 4;
    ip->ip_options_len = len;
    ip->ip_options_buf = p;
    ip->ip_hdr.ih_tlen += len;
    pkt_hdr_append_checksum(ip);
    return 0;
}

int ipv4_pkt_set_payload(ipv4_packet_t *ip, uint8_t *buf, int len)
{
    uint8_t *p;
    
    if (!ip)
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    p = (uint8_t *) malloc(len);
    
    if (!p)
        return -1;
    
    memcpy(p, buf, len);
    ip->ip_payload_len = len;
    ip->ip_payload_buf = p;
    ip->ip_hdr.ih_tlen += len;
    pkt_hdr_append_checksum(ip);
    return 0;
}

int ipv4_pkt_get_id(ipv4_packet_t *ip, uint16_t *id)
{
    if (!ip)
        return -1;
    
    (*id) = ip->ip_hdr.ih_id;
    return 0;
}

int ipv4_pkt_get_ttl(ipv4_packet_t *ip, uint8_t *ttl)
{
    if (!ip)
        return -1;
    
    (*ttl) = ip->ip_hdr.ih_ttl;
    return 0;
}

int ipv4_pkt_get_prot(ipv4_packet_t *ip, uint8_t *prot)
{
    if (!ip)
        return -1;
    
    (*prot) = ip->ip_hdr.ih_prot;
    return 0;
}

int ipv4_pkt_get_src(ipv4_packet_t *ip, ipv4_addr_t *src)
{
    if (!ip)
        return -1;
    
    ipv4_addr_cpy(src, &ip->ip_hdr.ih_src);
    return 0;
}

int ipv4_pkt_get_dst(ipv4_packet_t *ip, ipv4_addr_t *dst)
{
    if (!ip)
        return -1;
    
    ipv4_addr_cpy(dst, &ip->ip_hdr.ih_dst);
    return 0;
}

int ipv4_pkt_get_options_len(ipv4_packet_t *ip)
{
    if (!ip)
        return -1;
    
    return ip->ip_options_len;
}

int ipv4_pkt_get_options(ipv4_packet_t *ip, uint8_t **buf)
{
    if (!ip)
        return -1;
    
    if (!buf)
        return -1;
    
    (*buf) = ip->ip_options_buf;
    return 0;
}

int ipv4_pkt_get_payload_len(ipv4_packet_t *ip)
{
    if (!ip)
        return -1;
    
    return ip->ip_payload_len;
}

int ipv4_pkt_get_payload(ipv4_packet_t *ip, uint8_t **buf)
{
    if (!ip)
        return -1;
    
    if (!buf)
        return -1;
    
    (*buf) = ip->ip_payload_buf;
    return 0;
}

int ipv4_pkt_get_len(ipv4_packet_t *ip)
{
    if (!ip)
        return -1;
    
    return ip->ip_hdr.ih_tlen;
}

int ipv4_buf_to_pkt(uint8_t *buf, int len, ipv4_packet_t *ip)
{
    int i = 0;
    int len_total = 0;
    int len_opt = 0;
    uint16_t tmp;
    uint8_t *p_options = NULL;
    uint8_t *p_payload = NULL;
    
    if (!buf)
        return -1;
    
    if (len < 20)
        return -1;
    
    if (!ip)
        return -1;
    
    /* Header */
    ip->ip_hdr.ih_ver = (buf[i] >> 4);
    ip->ip_hdr.ih_ihl = buf[i++] & 0x0F;
    ip->ip_hdr.ih_dscp = (buf[i] >> 6);
    ip->ip_hdr.ih_ecn = buf[i++] & 0x03;
    
    if (ip->ip_hdr.ih_ihl < 5)
        return -1;
    else if (ip->ip_hdr.ih_ihl > 5)
        len_opt = (ip->ip_hdr.ih_ihl - 5) * 4;
    
    tmp = ((uint16_t) buf[i++] << 8);
    tmp |= buf[i++];
    
    len_total = tmp;
    
    if (len_total != len)
        return -1;
    
    ip->ip_hdr.ih_tlen = tmp;
    tmp = ((uint16_t) buf[i++] << 8);
    tmp |= buf[i++];
    ip->ip_hdr.ih_id = tmp;
    ip->ip_hdr.ih_flag = buf[i] >> 5;
    tmp = (((uint16_t) (buf[i++] & 0x1F)) << 8);
    tmp |= buf[i++];
    ip->ip_hdr.ih_foff = tmp;
    ip->ip_hdr.ih_ttl = buf[i++];
    ip->ip_hdr.ih_prot = buf[i++];
    tmp = ((uint16_t) buf[i++] << 8);
    tmp |= buf[i++];
    ip->ip_hdr.ih_chk = tmp;
    ip->ip_hdr.ih_src.ia_byte0 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte1 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte2 = buf[i++];
    ip->ip_hdr.ih_src.ia_byte3 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte0 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte1 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte2 = buf[i++];
    ip->ip_hdr.ih_dst.ia_byte3 = buf[i++];
    
    /* Header options */
    if (len_opt > 0) {
        p_options = (uint8_t *) malloc(len_opt);
        
        if (!p_options)
            return -1;
        
        ip->ip_options_buf = p_options;
        ip->ip_options_len = len_opt;
        memcpy(ip->ip_options_buf, &buf[i], len_opt);
        i += len_opt;
    } else
        ip->ip_options_len = 0;
    
    if (pkt_hdr_verify_checksum(ip) != 1) {
        if (ip->ip_options_buf)
            free(ip->ip_options_buf);
        return -1;
    }
    
    /* Payload */
    if ((len_total - len_opt - 20) > 0) {
        p_payload = (uint8_t *) malloc(len_total - len_opt - 20);
        
        if (!p_payload)
            return -1;
        
        ip->ip_payload_buf = p_payload;
        ip->ip_payload_len = (len_total - len_opt - 20);
        memcpy(ip->ip_payload_buf, &buf[i], ip->ip_payload_len);
    }
    
    return 0;
}

int ipv4_pkt_to_buf(ipv4_packet_t *ip, uint8_t *buf)
{
    int i = 0;
    uint16_t tmp;
    
    if (!ip)
        return -1;
    
    if (!buf)
        return -1;
    
    /* Header */
    buf[i++] = (ip->ip_hdr.ih_ver << 4) | ip->ip_hdr.ih_ihl;
    buf[i++] = (ip->ip_hdr.ih_dscp << 2) | ip->ip_hdr.ih_ecn;
    buf[i++] = _HIGH16(ip->ip_hdr.ih_tlen);
    buf[i++] = _LOW16(ip->ip_hdr.ih_tlen);
    buf[i++] = _HIGH16(ip->ip_hdr.ih_id);
    buf[i++] = _LOW16(ip->ip_hdr.ih_id);
    tmp = ((((uint16_t) ip->ip_hdr.ih_flag << 13)) | (uint16_t) ip->ip_hdr.ih_foff);
    buf[i++] = _HIGH16(tmp);
    buf[i++] = _LOW16(tmp);
    buf[i++] = ip->ip_hdr.ih_ttl;
    buf[i++] = ip->ip_hdr.ih_prot;
    buf[i++] = _HIGH16(ip->ip_hdr.ih_chk);
    buf[i++] = _LOW16(ip->ip_hdr.ih_chk);
    buf[i++] = ip->ip_hdr.ih_src.ia_byte0;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte1;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte2;
    buf[i++] = ip->ip_hdr.ih_src.ia_byte3;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte0;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte1;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte2;
    buf[i++] = ip->ip_hdr.ih_dst.ia_byte3;
    
    /* Header options */
    if (ip->ip_options_len > 0) {
        if (!ip->ip_options_buf)
            return -1;
        
        memcpy(&buf[i], ip->ip_options_buf, ip->ip_options_len);
        i += ip->ip_options_len;
    }
    
    /* Payload/Data */
    if (ip->ip_payload_len > 0) {
        if (!ip->ip_payload_buf)
            return -1;
        
        memcpy(&buf[i], ip->ip_payload_buf, ip->ip_payload_len);
    }
    
    return 0;
}

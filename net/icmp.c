/**
 *
 * File Name: icmp.c
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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "icmp.h"

#define _HIGH(val)      ((uint8_t) (((val) & 0xFF00) >> 8))
#define _LOW(val)       ((uint8_t) ((val) & 0x00FF))

static uint32_t pkt_sum(icmp_packet_t *icmp)
{
    int i = 0;
    int j;
    uint32_t sum = 0;
    uint16_t tmp;
    
    sum += ((uint16_t) icmp->ip_hdr.ih_type << 8) | icmp->ip_hdr.ih_code;
    sum += ((uint16_t) icmp->ip_hdr.ih_rest[0] << 8) | icmp->ip_hdr.ih_rest[1];
    sum += ((uint16_t) icmp->ip_hdr.ih_rest[2] << 8) | icmp->ip_hdr.ih_rest[3];
    
    if (icmp->ip_payload_len > 0) {
        if (!icmp->ip_payload_buf)
            return 0;
        
        for (j = 0; j < icmp->ip_payload_len / 2; j++ ) {
            tmp = ((uint16_t) icmp->ip_payload_buf[i++] << 8);
            tmp |= icmp->ip_payload_buf[i++];
            sum += tmp;
        }
    }
    
    return sum;
}

static int pkt_append_checksum(icmp_packet_t *icmp)
{
    uint32_t sum;
    uint16_t carry;
    
    sum = pkt_sum(icmp);
    carry = (uint16_t) (sum >> 16);
    sum += carry;
    sum = ~sum;
    icmp->ip_hdr.ih_chk = (uint16_t) sum;
    return 0;
}

static int pkt_verify_checksum(icmp_packet_t *icmp)
{
    uint32_t sum;
    uint16_t carry;
    uint16_t chksum;
    
    sum = pkt_sum(icmp);
    carry = (uint16_t) (sum >> 16);
    sum += carry;
    sum = ~sum;
    chksum = (uint16_t) sum;
        
    if (icmp->ip_hdr.ih_chk == chksum)
        return 1;
    
    return 0;
}

int icmp_pkt_set_type(icmp_packet_t *icmp, uint8_t type)
{
    if (!icmp)
        return -1;
    
    icmp->ip_hdr.ih_type = type;
    pkt_append_checksum(icmp);
    return 0;
}

int icmp_pkt_set_code(icmp_packet_t *icmp, uint8_t code)
{
    if (!icmp)
        return -1;
    
    icmp->ip_hdr.ih_code = code;
    pkt_append_checksum(icmp);
    return 0;
}

int icmp_pkt_set_rest(icmp_packet_t *icmp, uint8_t *rest, int len)
{
    if (!icmp)
        return -1;
    
    if (!rest)
        return -1;
    
    if (len != 4)
        return -1;
    
    icmp->ip_hdr.ih_rest[0] = rest[0];
    icmp->ip_hdr.ih_rest[1] = rest[1];
    icmp->ip_hdr.ih_rest[2] = rest[2];
    icmp->ip_hdr.ih_rest[3] = rest[3];
    pkt_append_checksum(icmp);
    return 0;
}

int icmp_pkt_set_payload(icmp_packet_t *icmp, uint8_t *buf, int len)
{
    uint8_t *p;
    
    if (!icmp)
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    p = (uint8_t *) malloc(len);
    
    if (!p)
        return -1;
    
    memcpy(p, buf, len);
    icmp->ip_payload_buf = p;
    icmp->ip_payload_len = len;
    pkt_append_checksum(icmp);
    return 0;
}

int icmp_pkt_get_type(icmp_packet_t *icmp, uint8_t *type)
{
    if (!icmp)
        return -1;
    
    if (!type)
        return -1;
    
    (*type) = icmp->ip_hdr.ih_type;
    return 0;
}

int icmp_pkt_get_code(icmp_packet_t *icmp, uint8_t *code)
{
    if (!icmp)
        return -1;
    
    if (!code)
        return -1;
    
    (*code) = icmp->ip_hdr.ih_code;
    return 0;
}

int icmp_pkt_get_rest(icmp_packet_t *icmp, uint8_t *rest)
{
    if (!icmp)
        return -1;
    
    if (!rest)
        return -1;
    
    memcpy(rest, &icmp->ip_hdr.ih_rest, 4);
    return 0;
}

int icmp_pkt_get_payload_len(icmp_packet_t *icmp)
{
    if (!icmp)
        return -1;
    
    return icmp->ip_payload_len;
}

int icmp_pkt_get_payload(icmp_packet_t *icmp, uint8_t **buf)
{
    if (!icmp)
        return -1;
    
    if (!buf)
        return -1;
    
    (*buf) = icmp->ip_payload_buf;
    return 0;
}

int icmp_pkt_get_len(icmp_packet_t *icmp)
{
    if (!icmp)
        return -1;
    
    return (8 + icmp->ip_payload_len);
}

int icmp_buf_to_pkt(uint8_t *buf, int len, icmp_packet_t *icmp)
{
    int i = 0;
    uint16_t tmp;
    uint8_t *p;
    
    if (!buf)
        return -1;
    
    if (len < 8)
        return -1;
    
    if (!icmp)
        return -1;
    
    icmp->ip_hdr.ih_type = buf[i++];
    icmp->ip_hdr.ih_code = buf[i++];
    tmp = ((uint16_t) buf[i++] << 8);
    tmp |= buf[i++];
    icmp->ip_hdr.ih_chk = tmp;
    icmp->ip_hdr.ih_rest[0] = buf[i++];
    icmp->ip_hdr.ih_rest[1] = buf[i++];
    icmp->ip_hdr.ih_rest[2] = buf[i++];
    icmp->ip_hdr.ih_rest[3] = buf[i++];
    
    if ((len - 8) > 0) {
        p = (uint8_t *) malloc(len - 8);
        
        if (!p)
            return -1;
        
        memcpy(p, &buf[i], len - 8);
        icmp->ip_payload_buf = p;
        icmp->ip_payload_len = len - 8;
    }
    
    if (pkt_verify_checksum(icmp) > 0)
        return 0;
    else
        return -1;
}

int icmp_pkt_to_buf(icmp_packet_t *icmp, uint8_t *buf)
{
    int i = 0;
    
    if (!icmp)
        return -1;
    
    if (!buf)
        return -1;
    
    buf[i++] = icmp->ip_hdr.ih_type;
    buf[i++] = icmp->ip_hdr.ih_code;
    buf[i++] = _HIGH(icmp->ip_hdr.ih_chk);
    buf[i++] = _LOW(icmp->ip_hdr.ih_chk);
    buf[i++] = icmp->ip_hdr.ih_rest[0];
    buf[i++] = icmp->ip_hdr.ih_rest[1];
    buf[i++] = icmp->ip_hdr.ih_rest[2];
    buf[i++] = icmp->ip_hdr.ih_rest[3];
    
    if (icmp->ip_payload_len > 0) {
        if (!icmp->ip_payload_buf)
            return -1;
        
        memcpy(&buf[i], icmp->ip_payload_buf, icmp->ip_payload_len);
    }
    
    return 0;
}

int icmp_create_echo_reply(icmp_packet_t *icmp_in, icmp_packet_t *icmp_out)
{
    uint8_t *p;
    
    if (icmp_in->ip_hdr.ih_type != ICMP_TYPE_ECHOREQ)
        return -1;
    
    icmp_out->ip_hdr.ih_type = ICMP_TYPE_ECHOREP;
    icmp_out->ip_hdr.ih_code = 0;
    icmp_out->ip_hdr.ih_rest[0] = icmp_in->ip_hdr.ih_rest[0];
    icmp_out->ip_hdr.ih_rest[1] = icmp_in->ip_hdr.ih_rest[1];
    icmp_out->ip_hdr.ih_rest[2] = icmp_in->ip_hdr.ih_rest[2];
    icmp_out->ip_hdr.ih_rest[3] = icmp_in->ip_hdr.ih_rest[3];
    p = (uint8_t *) malloc(icmp_in->ip_payload_len);
    
    if (!p)
        return -1;
    
    memcpy(p, icmp_in->ip_payload_buf, icmp_in->ip_payload_len);
    icmp_out->ip_payload_buf = p;
    icmp_out->ip_payload_len = icmp_in->ip_payload_len;
    pkt_append_checksum(icmp_out);
    return 0;
}

int icmp_pkt_free(icmp_packet_t *icmp)
{
    if (!icmp)
        return -1;
    
    if (icmp->ip_payload_buf) {
        free(icmp->ip_payload_buf);
        icmp->ip_payload_len = 0;
    }
    
    return 0;
}

/**
 *
 * File Name: tcp.c
 * Title    : TCP library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-08-09
 * Modified : 2019-08-11
 * Revised  : 
 * Version  : 0.1.0.2
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

#include "tcp.h"

#define TCP_HDR_LEN     20

#define HI16(val)       ((uint8_t) (((val) & 0xFF00) >> 8))
#define LO16(val)       ((uint8_t) ((val) & 0x00FF))

static int error = TCP_ERROR_SUCCESS;

static uint32_t ipv4_sum(ipv4_packet_t *ip)
{
    uint32_t sum = 0;
    uint16_t tmp;
    
    if (!ip)
        return 0;
    
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
    sum += ip->ip_hdr.ih_prot;
    sum += (ip->ip_hdr.ih_tlen - (ip->ip_hdr.ih_ihl * 4));
    return sum;
}

static uint32_t tcp_sum(tcp_packet_t *tcp)
{
    int i = 0;
    int j;
    uint32_t sum = 0;
    uint16_t tmp;
    
    if (!tcp)
        return 0;
    
    sum += tcp->tp_hdr.th_srcp;
    sum += tcp->tp_hdr.th_dstp;
    sum += (tcp->tp_hdr.th_seqn >> 16);
    sum += (tcp->tp_hdr.th_seqn & 0xFFFF);
    sum += (tcp->tp_hdr.th_ackn >> 16);
    sum += (tcp->tp_hdr.th_ackn & 0xFFFF);
    tmp = tcp->tp_hdr.th_off << 12;
    tmp |= tcp->tp_hdr.th_res << 8;
    tmp |= tcp->tp_hdr.th_flags;
    sum += tmp;
    sum += tcp->tp_hdr.th_win;
    sum += tcp->tp_hdr.th_urgp;
    
    if (tcp->tp_options_len > 0) {
        if (!tcp->tp_options_buf)
            return 0;
        
        for (j = 0; j < tcp->tp_options_len / 2; j++ ) {
            tmp = ((uint16_t) tcp->tp_options_buf[i++] << 8);
            tmp |= tcp->tp_options_buf[i++];
            sum += tmp;
        }
        
        if (tcp->tp_options_len & 1) {
            tmp = ((uint16_t) tcp->tp_options_buf[i] << 8);
            sum += tmp;
        }
    }
    
    i = 0;
    
    if (tcp->tp_payload_len > 0) {
        if (!tcp->tp_payload_buf)
            return 0;
        
        for (j = 0; j < tcp->tp_payload_len / 2; j++ ) {
            tmp = ((uint16_t) tcp->tp_payload_buf[i++] << 8);
            tmp |= tcp->tp_payload_buf[i++];
            sum += tmp;
        }
        
        if (tcp->tp_payload_len & 1) {
            tmp = ((uint16_t) tcp->tp_payload_buf[i] << 8);
            sum += tmp;
        }
    }
    
    return sum;
}

int tcp_pkt_set_srcp(tcp_packet_t *tcp, uint16_t srcp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    tcp->tp_hdr.th_srcp = srcp;
    return 0;
}

int tcp_pkt_set_dstp(tcp_packet_t *tcp, uint16_t dstp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    tcp->tp_hdr.th_dstp = dstp;
    return 0;
}

int tcp_pkt_set_seqn(tcp_packet_t *tcp, uint32_t seqn)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    tcp->tp_hdr.th_seqn = seqn;
    return 0;
}

int tcp_pkt_set_ackn(tcp_packet_t *tcp, uint32_t ackn)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    tcp->tp_hdr.th_ackn = ackn;
    return 0;
}

int tcp_pkt_set_flags(tcp_packet_t *tcp, uint8_t flags)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    tcp->tp_hdr.th_flags = flags;
    return 0;
}

int tcp_pkt_set_win(tcp_packet_t *tcp, uint16_t win)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    tcp->tp_hdr.th_win = win;
    return 0;
}

int tcp_pkt_set_urgp(tcp_packet_t *tcp, uint16_t urgp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    tcp->tp_hdr.th_urgp = urgp;
    return 0;
}

int tcp_pkt_set_options(tcp_packet_t *tcp, uint8_t *buf, int len)
{
    uint8_t *p;
    int padding;
    
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!buf) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (len < 1) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (len > 4)
        padding = len % 4;
    else
        padding = 4 - len;
    
    p = (uint8_t *) malloc(len + padding);
    
    if (!p) {
        error = TCP_ERROR_NOMEM;
        return -1;
    }
    
    tcp->tp_hdr.th_off += ((len / 4) + 1);
    tcp->tp_options_buf = p;
    tcp->tp_options_len = len + padding;
    memcpy(tcp->tp_options_buf, buf, len);
    
    if (padding)
        memset(&tcp->tp_options_buf[len], 0, padding);
    
    return 0;
}

int tcp_pkt_set_payload(tcp_packet_t *tcp, uint8_t *buf, int len)
{
    uint8_t *p;
    
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!buf) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (len < 1) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    p = (uint8_t *) malloc(len);
    
    if (!p) {
        error = TCP_ERROR_NOMEM;
        return -1;
    }
    
    tcp->tp_payload_buf = p;
    tcp->tp_payload_len = len;
    memcpy(tcp->tp_payload_buf, buf, len);
    return 0;
}

int tcp_pkt_get_srcp(tcp_packet_t *tcp, uint16_t *srcp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!srcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*srcp) = tcp->tp_hdr.th_srcp;
    return 0;
}

int tcp_pkt_get_dstp(tcp_packet_t *tcp, uint16_t *dstp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!dstp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*dstp) = tcp->tp_hdr.th_dstp;
    return 0;
}

int tcp_pkt_get_seqn(tcp_packet_t *tcp, uint32_t *seqn)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!seqn) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*seqn) = tcp->tp_hdr.th_seqn;
    return 0;
}

int tcp_pkt_get_ackn(tcp_packet_t *tcp, uint32_t *ackn)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!ackn) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*ackn) = tcp->tp_hdr.th_ackn;
    return 0;
}

int tcp_pkt_get_flags(tcp_packet_t *tcp, uint8_t *flags)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!flags) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*flags) = tcp->tp_hdr.th_flags;
    return 0;
}

int tcp_pkt_get_win(tcp_packet_t *tcp, uint16_t *win)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!win) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*win) = tcp->tp_hdr.th_win;
    return 0;
}

int tcp_pkt_get_urgp(tcp_packet_t *tcp, uint16_t *urgp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!urgp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*urgp) = tcp->tp_hdr.th_urgp;
    return 0;
}

int tcp_pkt_get_options(tcp_packet_t *tcp, uint8_t **buf)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!buf) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*buf) = tcp->tp_options_buf;
    return 0;
}

int tcp_pkt_get_options_len(tcp_packet_t *tcp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    return tcp->tp_options_len;
}

int tcp_pkt_get_payload(tcp_packet_t *tcp, uint8_t **buf)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!buf) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    (*buf) = tcp->tp_payload_buf;
    return 0;
}

int tcp_pkt_get_payload_len(tcp_packet_t *tcp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    return tcp->tp_payload_len;
}

int tcp_pkt_create(uint16_t srcp, uint16_t dstp, 
                   uint32_t seqn, uint32_t ackn, 
                   uint16_t win, uint8_t flags, 
                   uint16_t urgp, 
                   tcp_packet_t *tcp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    tcp->tp_hdr.th_srcp = srcp;
    tcp->tp_hdr.th_dstp = dstp;
    tcp->tp_hdr.th_seqn = seqn;
    tcp->tp_hdr.th_ackn = ackn;
    tcp->tp_hdr.th_flags = flags;
    tcp->tp_hdr.th_urgp = urgp;
    tcp->tp_hdr.th_off = (TCP_HDR_LEN / 4);
    tcp->tp_hdr.th_res = 0;
    return 0;
}

int tcp_pkt_get_len(tcp_packet_t *tcp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    return (TCP_HDR_LEN + tcp->tp_options_len + tcp->tp_payload_len);
}

int tcp_pkt_free(tcp_packet_t *tcp)
{
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (tcp->tp_options_len > 0) {
        if (tcp->tp_options_buf) {
            free(tcp->tp_options_buf);
            tcp->tp_options_len = 0;
            tcp->tp_options_buf = NULL;
        } else {
            error = TCP_ERROR_INTERNAL;
            return -1;
        }
    }
    
    if (tcp->tp_payload_len > 0) {
        if (tcp->tp_payload_buf) {
            free(tcp->tp_payload_buf);
            tcp->tp_payload_len = 0;
            tcp->tp_payload_buf = NULL;
        } else {
            error = TCP_ERROR_INTERNAL;
            return -1;
        }
    }
    
    return 0;
}

int tcp_ip_to_pkt(ipv4_packet_t *ip_tcp, tcp_packet_t *tcp)
{
    int i = 0;
    uint32_t sum;
    uint16_t carry;
    uint16_t chk;
    uint8_t *p;
    uint8_t *p_opt;
    uint8_t *p_pay;
    int len;
    int opt_len = 0;
    
    if (!ip_tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    sum = ipv4_sum(ip_tcp);
    len = ipv4_pkt_get_payload_len(ip_tcp);
    
    if (len < TCP_HDR_LEN) {
        error = TCP_ERROR_UNKNOWN;
        return -1;
    }
    
    ipv4_pkt_get_payload(ip_tcp, &p);
    tcp->tp_hdr.th_srcp = ((uint16_t) p[i++] << 8);
    tcp->tp_hdr.th_srcp |= (uint16_t) p[i++];
    tcp->tp_hdr.th_dstp = ((uint16_t) p[i++] << 8);
    tcp->tp_hdr.th_dstp |= (uint16_t) p[i++];
    tcp->tp_hdr.th_seqn = ((uint32_t) p[i++] << 24);
    tcp->tp_hdr.th_seqn |= ((uint32_t) p[i++] << 16);
    tcp->tp_hdr.th_seqn |= ((uint32_t) p[i++] << 8);
    tcp->tp_hdr.th_seqn |= (uint32_t) p[i++];
    tcp->tp_hdr.th_ackn = ((uint32_t) p[i++] << 24);
    tcp->tp_hdr.th_ackn |= ((uint32_t) p[i++] << 16);
    tcp->tp_hdr.th_ackn |= ((uint32_t) p[i++] << 8);
    tcp->tp_hdr.th_ackn |= (uint32_t) p[i++];
    tcp->tp_hdr.th_off = (p[i] >> 4);
    tcp->tp_hdr.th_res = (p[i++] & 0x0F);
    tcp->tp_hdr.th_flags = p[i++];
    tcp->tp_hdr.th_win = ((uint16_t) p[i++] << 8);
    tcp->tp_hdr.th_win |= (uint16_t) p[i++];
    tcp->tp_hdr.th_chk = ((uint16_t) p[i++] << 8);
    tcp->tp_hdr.th_chk |= (uint16_t) p[i++];
    tcp->tp_hdr.th_urgp = ((uint16_t) p[i++] << 8);
    tcp->tp_hdr.th_urgp |= (uint16_t) p[i++];
    
    if (tcp->tp_hdr.th_off > (TCP_HDR_LEN / 4)) {
        opt_len = (tcp->tp_hdr.th_off * 4) - TCP_HDR_LEN;
        
        p_opt = (uint8_t *) malloc(opt_len);
        
        if (!p_opt) {
            error = TCP_ERROR_NOMEM;
            return -1;
        }
        
        tcp->tp_options_buf = p_opt;
        tcp->tp_options_len = opt_len;
        memcpy(tcp->tp_options_buf, &p[i], tcp->tp_options_len);
        i += opt_len;
    }
    
    if (len > (TCP_HDR_LEN + opt_len)) {
        p_pay = (uint8_t *) malloc(len - TCP_HDR_LEN - opt_len);
        
        if (!p_pay) {
            if (tcp->tp_options_buf)
                free(tcp->tp_options_buf);
            
            error = TCP_ERROR_NOMEM;
            return -1;
        }
        
        tcp->tp_payload_buf = p_pay;
        tcp->tp_payload_len = len - TCP_HDR_LEN - opt_len;
        memcpy(tcp->tp_payload_buf, &p[i], tcp->tp_payload_len);
    }
    
    sum += tcp_sum(tcp);
    carry = (uint16_t) (sum >> 16);
    sum = ((sum & 0xFFFF) + carry);
    sum = ~sum;
    chk = (uint16_t) sum;
    
    if (tcp->tp_hdr.th_chk != chk) {
        tcp_pkt_free(tcp);
        error = TCP_ERROR_CHKSUM;
        return -1;
    }
    
    return 0;
}

int tcp_pkt_to_ip(tcp_packet_t *tcp, ipv4_packet_t *ip_tcp)
{
    int i = 0;
    uint32_t sum;
    uint16_t carry;
    uint8_t *p;
    int len;
    
    if (!tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    if (!ip_tcp) {
        error = TCP_ERROR_INVAL;
        return -1;
    }
    
    sum = ipv4_sum(ip_tcp);
    sum += tcp_sum(tcp);
    carry = (uint16_t) (sum >> 16);
    sum = ((sum & 0xFFFF) + carry);
    sum = ~sum;
    tcp->tp_hdr.th_chk = (uint16_t) sum;
    len = tcp_pkt_get_len(tcp);
    p = (uint8_t *) malloc(len);
    
    if (!p) {
        error = TCP_ERROR_NOMEM;
        return -1;
    }
    
    p[i++] = HI16(tcp->tp_hdr.th_srcp);
    p[i++] = LO16(tcp->tp_hdr.th_srcp);
    p[i++] = HI16(tcp->tp_hdr.th_dstp);
    p[i++] = LO16(tcp->tp_hdr.th_dstp);
    p[i++] = (uint8_t) ((tcp->tp_hdr.th_seqn & 0xFF000000) >> 24);
    p[i++] = (uint8_t) ((tcp->tp_hdr.th_seqn & 0x00FF0000) >> 16);
    p[i++] = (uint8_t) ((tcp->tp_hdr.th_seqn & 0x0000FF00) >> 8);
    p[i++] = (uint8_t) (tcp->tp_hdr.th_seqn & 0x000000FF);
    p[i++] = (uint8_t) ((tcp->tp_hdr.th_ackn & 0xFF000000) >> 24);
    p[i++] = (uint8_t) ((tcp->tp_hdr.th_ackn & 0x00FF0000) >> 16);
    p[i++] = (uint8_t) ((tcp->tp_hdr.th_ackn & 0x0000FF00) >> 8);
    p[i++] = (uint8_t) (tcp->tp_hdr.th_ackn & 0x000000FF);
    p[i] = ((uint8_t) tcp->tp_hdr.th_off << 4);
    p[i++] |= (uint8_t) tcp->tp_hdr.th_res;
    p[i++] = tcp->tp_hdr.th_flags;
    p[i++] = HI16(tcp->tp_hdr.th_win);
    p[i++] = LO16(tcp->tp_hdr.th_win);
    p[i++] = HI16(tcp->tp_hdr.th_chk);
    p[i++] = LO16(tcp->tp_hdr.th_chk);
    p[i++] = HI16(tcp->tp_hdr.th_urgp);
    p[i++] = LO16(tcp->tp_hdr.th_urgp);
    
    if (tcp->tp_options_len > 0) {
        if (!tcp->tp_options_buf) {
            free(p);
            error = TCP_ERROR_INTERNAL;
            return -1;
        }
        
        memcpy(&p[i], tcp->tp_options_buf, tcp->tp_options_len);
        i += tcp->tp_options_len;
    }
    
    if (tcp->tp_payload_len > 0) {
        if (!tcp->tp_payload_buf) {
            free(p);
            error = TCP_ERROR_INTERNAL;
            return -1;
        }
        
        memcpy(&p[i], tcp->tp_payload_buf, tcp->tp_payload_len);
    }
    
    if (ipv4_pkt_set_payload(ip_tcp, p, len) == -1) {
        error = TCP_ERROR_INTERNAL;
        free(p);
        return -1;
    }
    
    free(p);
    return 0;
}

int tcp_get_last_error(void)
{
    int err;
    
    err = error;
    error = TCP_ERROR_SUCCESS;
    return err;
}

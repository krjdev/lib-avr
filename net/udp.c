/**
 *
 * File Name: udp.c
 * Title    : UDP definitions and helper functions source
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

#include "udp.h"

int udp_set_port_src(udp_hdr_t *udp, uint16_t src)
{
    if (!udp)
        return -1;
    
    udp->uh_srcp = src;
    return 0;
}

int udp_set_port_dst(udp_hdr_t *udp, uint16_t dst)
{
    if (!udp)
        return -1;
    
    udp->uh_dstp = dst;
    return 0;
}

int udp_set_len(udp_hdr_t *udp, uint16_t len)
{
    if (!udp)
        return -1;
    
    udp->uh_len = len;
    return 0;
}

int udp_set_chk(udp_hdr_t *udp, uint16_t chk)
{
    if (!udp)
        return -1;
    
    udp->uh_chk = chk;
    return 0;
}

int udp_get_port_src(udp_hdr_t *udp, uint16_t *src)
{
    if (!udp)
        return -1;
    
    (*src) = udp->uh_srcp;
    return 0;
}

int udp_get_port_dst(udp_hdr_t *udp, uint16_t *dst)
{
    if (!udp)
        return -1;
    
    (*dst) = udp->uh_dstp;
    return 0;
}

int udp_get_len(udp_hdr_t *udp, uint16_t *len)
{
    if (!udp)
        return -1;
    
    (*len) = udp->uh_len;
    return 0;
}

int udp_get_chk(udp_hdr_t *udp, uint16_t *chk)
{
    if (!udp)
        return -1;
    
    (*chk) = udp->uh_chk;
    return 0;
}

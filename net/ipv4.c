/**
 *
 * File Name: ipv4.c
 * Title    : IPv4 definitions and helper functions source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
 * Modified : 2019-02-04
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

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ipv4.h"

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

int ipv4_addr_aton(const char *str, ipv4_addr_t *ip)
{
    char tmp[4];
    int i = 0;
    int start, end, cnt;
    
    if (!str)
        return -1;
    
    if (!ip)
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
                sscanf(tmp, "%hhu", &ip->ia_byte0);
                break;
            case 2:
                sscanf(tmp, "%hhu", &ip->ia_byte1);
                break;
            case 3:
                sscanf(tmp, "%hhu", &ip->ia_byte2);
                break;
            default:
                break;
            }
        }
    }
    
    end = i;
    memcpy(&tmp, &str[start], (end - start));
    tmp[end - start] = '\0';
    sscanf(tmp, "%hhu", &ip->ia_byte3);
    return 0;
}

int ipv4_addr_ntoa(ipv4_addr_t *ip, char *str)
{
    if (!ip)
        return -1;
    
    if (!str)
        return -1;
    
    sprintf(str, "%hhu.%hhu.%hhu.%hhu", ip->ia_byte0, ip->ia_byte1, 
            ip->ia_byte2, ip->ia_byte3);
    return 0;
}

int ipv4_addr_equal(ipv4_addr_t *ip1, ipv4_addr_t *ip2)
{
    if (!ip1)
        return -1;
    
    if (!ip2)
        return -1;
    
    if ((ip1->ia_byte0 == ip2->ia_byte0) && 
        (ip1->ia_byte1 == ip2->ia_byte1) && 
        (ip1->ia_byte2 == ip2->ia_byte2) && 
        (ip1->ia_byte3 == ip2->ia_byte3))
        return 1;
    
    return 0;
}

int ipv4_addr_cpy(ipv4_addr_t *dst, ipv4_addr_t *src)
{
    if (!dst)
        return -1;
    
    if (!src)
        return -1;
    
    dst->ia_byte0 = src->ia_byte0;
    dst->ia_byte1 = src->ia_byte1;
    dst->ia_byte2 = src->ia_byte2;
    dst->ia_byte3 = src->ia_byte3;
    return 0;
}

int ipv4_addr_is_broadcast(ipv4_addr_t *ip)
{
    if (!ip)
        return -1;
    
    if ((ip->ia_byte0 == 0xFF) && 
        (ip->ia_byte1 == 0xFF) && 
        (ip->ia_byte2 == 0xFF) && 
        (ip->ia_byte3 == 0xFF))
        return 1;
    
    return 0;
}

int ipv4_calc_checksum(uint8_t *buf, int len, uint16_t *chksum)
{
    int i, j;
    uint16_t tmp;
    uint32_t sum = 0;
    uint8_t carry;
    
    if (!buf)
        return -1;
    
    if ((len % 2) != 0)
        return -1;
    
    for (i = 0, j = 0; i < (len / 2); i++) {
        tmp = ((uint16_t) buf[j++] << 8);
        tmp |= (uint16_t) buf[j++];
        sum += tmp;
    }
    
    carry = (sum >> 16);
    sum += carry;
    sum = ~sum;
    (*chksum) = (uint16_t) sum;
    return 0;
}

/**
 *
 * File Name: ipv4.c
 * Title    : IPv4 definitions and helper functions source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
 * Modified : 2019-01-30
 * Revised  : 
 * Version  : 0.2.1.0
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

int ipv4_aton(const char *str, ipv4_addr_t *ip)
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
            memcpy(&tmp[0], &str[start], (end - start));
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
    sscanf(tmp, "%hhu", &ip->ia_byte3);
    return 0;
}

int ipv4_ntoa(ipv4_addr_t *ip, char *str)
{
    if (!ip)
        return -1;
    
    if (!str)
        return -1;
    
    sprintf(str, "%hhu.%hhu.%hhu.%hhu", ip->ia_byte0, ip->ia_byte1, 
            ip->ia_byte2, ip->ia_byte3);
    return 0;
}

int ipv4_equal(ipv4_addr_t *ip1, ipv4_addr_t *ip2)
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

int ipv4_acpy(ipv4_addr_t *dst, ipv4_addr_t *src)
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

int ipv4_is_broadcast(ipv4_addr_t *ip)
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

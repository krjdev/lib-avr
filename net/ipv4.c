/**
 *
 * File Name: ipv4.h
 * Title    : IPv4 definitions and helper functions source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
 * Modified : 2019-01-30
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

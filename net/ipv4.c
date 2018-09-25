/**
 *
 * File Name: ipv4.h
 * Title    : IPv4 definitions and helper functions source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ipv4.h"

ipv4_addr_t *ipv4_str_to_ip(const char *str)
{
    ipv4_addr_t *p;
    char tmp[4];
    int i = 0;
    int start, end, cnt;
    
    if (!str)
        return NULL;
    
    if (strlen(str) > 15 || strlen(str) < 7)
        return NULL;
    
    p = (ipv4_addr_t *) malloc(sizeof(ipv4_addr_t));
    
    if (!p)
        return NULL;
    
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
                sscanf(tmp, "%hhu", &p->ia_byte0);
                break;
            case 2:
                sscanf(tmp, "%hhu", &p->ia_byte1);
                break;
            case 3:
                sscanf(tmp, "%hhu", &p->ia_byte2);
                break;
            default:
                break;
            }
        }
    }
    
    end = i;
    memcpy(&tmp, &str[start], (end - start));
    sscanf(tmp, "%hhu", &p->ia_byte3);    
    return p;
}

char *ipv4_ip_to_str(ipv4_addr_t *ip)
{
    char *p;
    
    if (!ip)
        return NULL;
    
    p = (char *) malloc(16);
    
    if (!p)
        return NULL;
    
    sprintf(p, "%hhu.%hhu.%hhu.%hhu", ip->ia_byte0, ip->ia_byte1, 
            ip->ia_byte2, ip->ia_byte3);
    
    return p;
}

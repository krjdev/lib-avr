/**
 *
 * File Name: crc32_ethernet.c
 * Title    : CRC32 calculation for Ethernet (FCS) library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-05-29
 * Modified : 
 * Revised  : 
 * Version  : 0.1.0.0
 * License  : 
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include "crc32_ethernet.h"

#define CRC_POLY    0xEDB88320

uint32_t crc32_calc(uint8_t *data, int len)
{
    int i, j;
    uint32_t crc;
    
    if (!data)
        return 0;
    
    if (len < 1)
        return 0;
    
    crc = 0xFFFFFFFF;
    
    for (j = 0; j < len; j++) {
        crc ^= data[j];
        
        for (i = 0; i < 8; i++) {
             crc = (crc & 1) ? ((crc >> 1) ^ CRC_POLY) : (crc >> 1);
        }
    }
    
    return (crc ^ 0xFFFFFFFF);
}

int crc32_check(uint8_t *data, int len, uint32_t crc)
{
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (crc32_calc(data, len) == crc)
        return 1;
    
    return 0;
}

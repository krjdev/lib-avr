/**
 *
 * File Name: crc7.c
 * Title    : CRC calculation for MMC/SD cards
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-04-14
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

#include "crc7.h"
 
#define CRC_POLY    0x89

int crc7_calc(uint8_t *data, int len)
{
    int i, j;
    uint8_t crc;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    crc = 0x00;
    
    for (j = 0; j < len; j++) {
        crc ^= data[j];
        
        for (i = 0; i < 8; i++) {
            crc = (crc & 0x80) ? ((crc << 1) ^ (CRC_POLY << 1)) : (crc << 1);
        }
    }
    
    crc >>= 1;
    return ((crc << 1) + 1);
}

int crc7_check(uint8_t *data, int len, uint8_t crc)
{
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (crc7_calc(data, len) == crc)
        return 1;
    
    return 0;
}

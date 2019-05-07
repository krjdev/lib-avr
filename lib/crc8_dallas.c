/**
 *
 * File Name: crc_dallas.c
 * Title    : CRC calculation for Dallas/Maxim 1-Wire devices source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-11-29
 * Modified : 2019-05-07
 * Revised  : 
 * Version  : 0.1.1.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include "crc8_dallas.h"

#define CRC_POLY    0x8C

int crc8_dallas_calc(uint8_t *data, int len)
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
            crc = (crc & 0x01) ? ((crc >> 1) ^ CRC_POLY) : (crc >> 1);
        }
    }
    
    return (int) crc;
}

int crc8_dallas_check(uint8_t *data, int len, uint8_t crc)
{
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (crc8_dallas_calc(data, len) == crc)
        return 1;
    
    return 0;
}

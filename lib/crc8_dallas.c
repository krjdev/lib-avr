/**
 *
 * File Name: crc_dallas.c
 * Title    : CRC calculation for Dallas/Maxim 1-Wire devices source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-11-29
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

#include "crc8_dallas.h"

#define CRC_POLY    0x8C

int crc8_dallas_calc(uint8_t *data, int len)
{
    int i, j, carry;
    uint8_t tmp, rem, crc;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    crc = 0x00;
    
    for (j = 0; j < len; j++) {
        tmp = data[j];
        rem = tmp;
        
        for (i = 0; i < 8; i++) {
            rem ^= crc;
            crc >>= 1;
            tmp >>= 1;
            carry = rem & 0x01;
            
            if (carry)
                crc ^= CRC_POLY;
            
            rem = tmp;
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

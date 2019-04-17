/**
 *
 * File Name: crc16_ccitt.c
 * Title    : CRC16-CCITT calculation library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-04-17
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

#include "crc16_ccitt.h"

#define CRC_POLY    0x1021

uint16_t crc16_ccitt_calc(uint8_t *data, int len)
{
    int i, j;
    uint16_t crc;
    
    if (!data)
        return 0;
    
    if (len < 1)
        return 0;
    
    crc = 0x0000;
    
    for (j = 0; j < len; j++) {
        crc ^= ((uint16_t) data[j] << 8);
        
        for (i = 0; i < 8; i++) {
            crc = (crc & 0x8000) ? ((crc << 1) ^ CRC_POLY) : (crc << 1);
        }
    }
    
    return crc;
}

int crc16_ccitt_check(uint8_t *data, int len, uint16_t crc)
{
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (crc16_ccitt_calc(data, len) == crc)
        return 1;
    
    return 0;
}

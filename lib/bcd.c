/**
 *
 * File Name: bcd.c
 * Title    : BCD (Binary Coded Data) conversion functions source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-12-04
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

#include "bcd.h"

uint8_t bcd_to_int(uint8_t bcd)
{
    uint8_t nib_lo;
    uint8_t nib_hi;
    uint8_t ret;
    
    nib_lo = bcd & 0x0F;
    nib_hi = (bcd & 0xF0) >> 4;
    
    ret = (nib_hi * 10) + nib_lo;
    return ret;
}

uint8_t int_to_bcd(uint8_t i)
{
    uint8_t nib_lo;
    uint8_t nib_hi;
    uint8_t ret;
    
    nib_hi = i / 10;
    nib_lo = i % 10;
    ret = (nib_hi << 4) | nib_lo;
    return ret;    
}

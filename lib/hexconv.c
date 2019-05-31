/**
 *
 * File Name: hexconv.c
 * Title    : HEX conversion library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-05-31
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

#include <stdlib.h>

#include "hexconv.h"

char *uint8_to_hex(uint8_t data)
{
    uint8_t lo_nib;
    uint8_t hi_nib;
    char *hex;
    
    hex = (char *) malloc(3);
    
    if (!hex)
        return NULL;
    
    lo_nib = (data & 0x0F);
    hi_nib = ((data & 0xF0) >> 4);
    
    if (lo_nib > 9)
        lo_nib += 55;
    else
        lo_nib += 48;
    
    if (hi_nib > 9)
        hi_nib += 55;
    else
       hi_nib += 48;
    
    hex[0] = (char) hi_nib;
    hex[1] = (char) lo_nib;
    hex[2] = '\0';
    return hex;
}

uint8_t hex_to_uint8(const char *hex)
{
    uint8_t lo_nib;
    uint8_t hi_nib;
    
    hi_nib = (uint8_t) hex[0];
    lo_nib = (uint8_t) hex[1];
    
    if ((lo_nib >= 48) && (lo_nib <= 57))
        lo_nib -= 48;
    else
        lo_nib -= 55;
    
    if ((hi_nib >= 48) && (hi_nib <= 57))
        hi_nib -= 48;
    else
        hi_nib -= 55;
    
    return ((hi_nib << 4) | lo_nib);
    
}

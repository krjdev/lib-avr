/**
 *
 * File Name: endian.c
 * Title    : Endianness (Byte Order) library
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

#include "endian.h"
 
uint16_t buf_to_uint16_le(uint8_t *buf)
{
    uint16_t tmp;
    
    if (!buf)
        return 0;
    
    tmp = (uint16_t) buf[0];
    tmp |= ((uint16_t) buf[1] << 8);
    return tmp;
}

uint32_t buf_to_uint32_le(uint8_t *buf)
{
    uint32_t tmp;
    
    if (!buf)
        return 0;
    
    tmp = (uint32_t) buf[0];
    tmp |= ((uint32_t) buf[1] << 8);
    tmp |= ((uint32_t) buf[2] << 16);
    tmp |= ((uint32_t) buf[3] << 24);
    return tmp;
}

uint64_t buf_to_uint64_le(uint8_t *buf)
{
    uint64_t tmp;
    
    if (!buf)
        return 0;
    
    tmp = (uint64_t) buf[0];
    tmp |= ((uint64_t) buf[1] << 8);
    tmp |= ((uint64_t) buf[2] << 16);
    tmp |= ((uint64_t) buf[3] << 24);
    tmp |= ((uint64_t) buf[4] << 32);
    tmp |= ((uint64_t) buf[5] << 40);
    tmp |= ((uint64_t) buf[6] << 48);
    tmp |= ((uint64_t) buf[7] << 56);
    return tmp;
}

uint16_t buf_to_uint16_be(uint8_t *buf)
{
    uint16_t tmp;
    
    if (!buf)
        return 0;
    
    tmp = ((uint16_t) buf[0] << 8);
    tmp |= (uint16_t) buf[1];
    return tmp;
}

uint32_t buf_to_uint32_be(uint8_t *buf)
{
    uint32_t tmp;
    
    if (!buf)
        return 0;
    
    tmp = ((uint32_t) buf[0] << 24);
    tmp |= ((uint32_t) buf[1] << 16);
    tmp |= ((uint32_t) buf[2] << 8);
    tmp |= (uint32_t) buf[3];
    return tmp;
}

uint64_t buf_to_uint64_be(uint8_t *buf)
{
    uint64_t tmp;
    
    if (!buf)
        return 0;
    
    tmp = ((uint64_t) buf[0] << 56);
    tmp |= ((uint64_t) buf[1] << 48);
    tmp |= ((uint64_t) buf[2] << 40);
    tmp |= ((uint64_t) buf[3] << 32);
    tmp |= ((uint64_t) buf[4] << 24);
    tmp |= ((uint64_t) buf[5] << 16);
    tmp |= ((uint64_t) buf[6] << 8);
    tmp |= (uint64_t) buf[7];
    return tmp;
}

void uint16_to_buf_le(uint16_t val, uint8_t *buf)
{
    if (!buf)
        return;
    
    buf[0] = (uint8_t) (val & 0x00FF);
    buf[1] = (uint8_t) ((val & 0xFF00) >> 8);
}

void uint32_to_buf_le(uint32_t val, uint8_t *buf)
{
    if (!buf)
        return;
    
    buf[0] = (uint8_t) (val & 0x000000FF);
    buf[1] = (uint8_t) ((val & 0x0000FF00) >> 8);
    buf[2] = (uint8_t) ((val & 0x00FF0000) >> 16);
    buf[3] = (uint8_t) ((val & 0xFF000000) >> 24);
}

void uint64_to_buf_le(uint64_t val, uint8_t *buf)
{
    if (!buf)
        return;
    
    buf[0] = (uint8_t) (val & 0x00000000000000FF);
    buf[1] = (uint8_t) ((val & 0x000000000000FF00) >> 8);
    buf[2] = (uint8_t) ((val & 0x0000000000FF0000) >> 16);
    buf[3] = (uint8_t) ((val & 0x00000000FF000000) >> 24);
    buf[4] = (uint8_t) ((val & 0x000000FF00000000) >> 32);
    buf[5] = (uint8_t) ((val & 0x0000FF0000000000) >> 40);
    buf[6] = (uint8_t) ((val & 0x00FF000000000000) >> 48);
    buf[7] = (uint8_t) ((val & 0xFF00000000000000) >> 56);
}

void uint16_to_buf_be(uint16_t val, uint8_t *buf)
{
    if (!buf)
        return;
    
    buf[1] = (uint8_t) (val & 0x00FF);
    buf[0] = (uint8_t) ((val & 0xFF00) >> 8);
}

void uint32_to_buf_be(uint32_t val, uint8_t *buf)
{
    if (!buf)
        return;
    
    buf[3] = (uint8_t) (val & 0x000000FF);
    buf[2] = (uint8_t) ((val & 0x0000FF00) >> 8);
    buf[1] = (uint8_t) ((val & 0x00FF0000) >> 16);
    buf[0] = (uint8_t) ((val & 0xFF000000) >> 24);
}

void uint64_to_buf_be(uint64_t val, uint8_t *buf)
{
    if (!buf)
        return;
    
    buf[7] = (uint8_t) (val & 0x00000000000000FF);
    buf[6] = (uint8_t) ((val & 0x000000000000FF00) >> 8);
    buf[5] = (uint8_t) ((val & 0x0000000000FF0000) >> 16);
    buf[4] = (uint8_t) ((val & 0x00000000FF000000) >> 24);
    buf[3] = (uint8_t) ((val & 0x000000FF00000000) >> 32);
    buf[2] = (uint8_t) ((val & 0x0000FF0000000000) >> 40);
    buf[1] = (uint8_t) ((val & 0x00FF000000000000) >> 48);
    buf[0] = (uint8_t) ((val & 0xFF00000000000000) >> 56);
}

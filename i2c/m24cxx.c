/**
 *
 * File Name: m24cxx.c
 * Title    : STmicroelectronics M24C01 - M24C64 I2C EEPROM source header
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

#include <stdlib.h>

#include "i2c.h"
#include "m24cxx.h"

#define M24CXX_ADDR            0x50

#define M24C01_16_SUBTYPE       0
#define M24C32_64_SUBTYPE       1

#define M24C01_16_PAGE_SIZE    16
#define M24C32_64_PAGE_SIZE    32

#define M24C01_SIZE             16
#define M24C02_SIZE             32
#define M24C04_SIZE             64
#define M24C08_SIZE             128
#define M24C16_SIZE             256
#define M24C32_SIZE             512
#define M24C64_SIZE             1024

#define _HIGH(u16)              ((uint8_t) (((u16) & 0xFF00) >> 8))
#define _LOW(u16)               ((uint8_t) ((u16) & 0x00FF))

void mc24cxx_init(void)
{
    i2c_master_init(100000);
}

int m24cxx_write(int type, uint8_t subaddr, uint16_t addr, uint8_t *buf, int len)
{
    uint8_t i2c_addr;
    int subtype;
    int page;
    int size;
    int page_cnt;
    int rest;
    int ret;
    int i;
    uint8_t addr_buf[2];
    
    i2c_addr = M24CXX_ADDR | subaddr;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    switch (type) {
    case TYPE_M24C01:
        subtype = M24C01_16_SUBTYPE;
        page = M24C01_16_PAGE_SIZE;
        size = M24C01_SIZE;
        break;
    case TYPE_M24C02:
        subtype = M24C01_16_SUBTYPE;
        page = M24C01_16_PAGE_SIZE;
        size = M24C02_SIZE;
        break;
    case TYPE_M24C04:
        subtype = M24C01_16_SUBTYPE;
        page = M24C01_16_PAGE_SIZE;
        size = M24C04_SIZE;
        break;
    case TYPE_M24C08:
        subtype = M24C01_16_SUBTYPE;
        page = M24C01_16_PAGE_SIZE;
        size = M24C08_SIZE;
        break;
    case TYPE_M24C16:
        subtype = M24C01_16_SUBTYPE;
        page = M24C01_16_PAGE_SIZE;
        size = M24C01_SIZE;
        break;
    case TYPE_M24C32:
        subtype = M24C32_64_SUBTYPE;
        page = M24C32_64_PAGE_SIZE;
        size = M24C32_SIZE;
        break;
    case TYPE_M24C64:
        subtype = M24C32_64_SUBTYPE;
        page = M24C32_64_PAGE_SIZE;
        size = M24C64_SIZE;
        break;
    default:
        return -1;
    }
    
    if (addr > (size - 1))
        return -1;
    
    if ((addr + len) > size)
        return -1;
    
    if (subtype == M24C01_16_SUBTYPE) {
        addr_buf[0] = _LOW(addr);
        ret = i2c_master_send(I2C_OPT_NOSTOP, i2c_addr, addr_buf, 1, NULL, 0);
        
        if (ret == -1)
            return -1;
    } else {
        addr_buf[0] = _HIGH(addr);
        addr_buf[1] = _LOW(addr);
        ret = i2c_master_send(I2C_OPT_NOSTOP, i2c_addr, addr_buf, 2, NULL, 0);
        
        if (ret == -1)
            return -1;
    }
    
    for (i = 0; i < len; i++) {
        ret = i2c_master_send(I2C_OPT_NORMAL, i2c_addr, NULL, 0, buf, len);
        
        if (ret == -1)
            return -1;
    }
    
    return 0;
}

int m24cxx_read(int type, uint8_t subaddr, uint16_t addr, uint8_t *buf, int len)
{
    uint8_t i2c_addr;
    int subtype;
    int size;
    int i;
    int ret;
    uint8_t addr_buf[2];
    
    i2c_addr = M24CXX_ADDR | subaddr;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    switch (type) {
    case TYPE_M24C01:
        subtype = M24C01_16_SUBTYPE;
        size = M24C01_SIZE;
        break;
    case TYPE_M24C02:
        subtype = M24C01_16_SUBTYPE;
        size = M24C02_SIZE;
        break;
    case TYPE_M24C04:
        subtype = M24C01_16_SUBTYPE;
        size = M24C04_SIZE;
        break;
    case TYPE_M24C08:
        subtype = M24C01_16_SUBTYPE;
        size = M24C08_SIZE;
        break;
    case TYPE_M24C16:
        subtype = M24C01_16_SUBTYPE;
        size = M24C01_SIZE;
        break;
    case TYPE_M24C32:
        subtype = M24C32_64_SUBTYPE;
        size = M24C32_SIZE;
        break;
    case TYPE_M24C64:
        subtype = M24C32_64_SUBTYPE;
        size = M24C64_SIZE;
        break;
    default:
        return -1;
    }
    
    if (addr > (size - 1))
        return -1;
    
    if ((addr + len) > size)
        return -1;
    
    if (subtype == M24C01_16_SUBTYPE) {
        addr_buf[0] = _LOW(addr);
        ret = i2c_master_send(I2C_OPT_NOSTOP, i2c_addr, addr_buf, 1, NULL, 0);
        
        if (ret == -1)
            return -1;
    } else {
        addr_buf[0] = _HIGH(addr);
        addr_buf[1] = _LOW(addr);
        ret = i2c_master_send(I2C_OPT_NOSTOP, i2c_addr, addr_buf, 2, NULL, 0);
        
        if (ret == -1)
            return -1;
    }
    
    for (i = 0; i < len; i++) {
        ret = i2c_master_recv(I2C_OPT_NORMAL, i2c_addr, NULL, 0, buf, len);
        
        if (ret == -1)
            return -1;
    }
    
    return 0;
}

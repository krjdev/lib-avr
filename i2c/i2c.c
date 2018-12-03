/**
 *
 * File Name: i2c.c
 * Title    : I2C interface library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-21
 * Modified : 2018-12-03
 * Revised  : 
 * Version  : 0.2.0.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>

#include "i2c.h"

uint8_t err = 0xFF;

static int i2c_start(void)
{
    TWCR |= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT)))
        ;
    
    if ((TWSR & 0xF8) != 0x08) {
        err = TWSR;
        return -1;
    }
    
    return 0;
}

static int i2c_addr_send(uint8_t addr)
{
    TWDR = addr << 1;
    TWCR |= (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT)))
        ;
    
    if ((TWSR & 0xF8) != 0x18) {
        err = TWSR;
        return -1;
    }
    
    return 0;
}

static int i2c_addr_recv(uint8_t addr)
{
    TWDR = (addr << 1) | 1;
    TWCR |= (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT)))
        ;
    
    if ((TWSR & 0xF8) != 0x40) {
        err = TWSR;
        return -1;
    }
    
    return 0;
}

static int i2c_data_send(uint8_t data)
{
    TWDR = data;
    TWCR |= (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT)))
        ;
    
    if ((TWSR & 0xF8) != 0x28) {
        err = TWSR;
        return -1;
    }
    
    return 0;
}

static int i2c_data_recv(uint8_t *data, int len)
{
    if (len < 1)
        return -1;
    
    if (len > 1)
        TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    else
        TWCR |= (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT)))
        ;
    
    if (len > 1) {
        if ((TWSR & 0xF8) != 0x50) {
            err = TWSR;
            return -1;
        }
    } else {
        if ((TWSR & 0xF8) != 0x58) {
            err = TWSR;
            return -1;
        }
    }
    
    (*data) = TWDR;
    return 0;
}

static void i2c_stop(void)
{
    TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    _delay_us(10);
}

int i2c_master_init(uint32_t speed)
{
    if (speed > 400000)
        return -1;
    
    TWBR = ((((F_CPU / speed) / 1) - 16 ) / 2);
    return 0;
}

int i2c_master_send(int opt, 
                    uint8_t addr, 
                    uint8_t *cmd, 
                    int cmd_len, 
                    uint8_t *data, 
                    int data_len)
{
    int ret;
    int i;

    if (cmd_len > 0 && cmd == NULL)
        return -1;
    
    if (data_len > 0 && data == NULL)
        return -1;
    
    ret = i2c_start();
    
    if (ret == -1)
        return -1;
    
    ret = i2c_addr_send(addr);
    
    if (ret == -1)
        return -1;
    
    if (cmd_len > 0) {
        for (i = 0; i < cmd_len; i++) {
            ret = i2c_data_send(cmd[i]);
            
            if (ret == -1)
                return -1;
        }
    }
    
    if (data_len > 0) {
        for (i = 0; i < data_len; i++) {
            ret = i2c_data_send(data[i]);
            
            if (ret == -1)
                return -1;
        }
    }
    
    if (opt == I2C_OPT_NORMAL)
        i2c_stop();
    
    return 0;
}

int i2c_master_recv(int opt, 
                    uint8_t addr, 
                    uint8_t *cmd, 
                    int cmd_len, 
                    uint8_t *data, 
                    int data_len)
{
    int ret;
    int i;
    
    if (cmd_len > 0 && cmd == NULL)
        return -1;
    
    if (data_len > 0 && data == NULL)
        return -1;
    
    ret = i2c_start();
    
    if (ret == -1)
        return -1;
    
    ret = i2c_addr_recv(addr);
    
    if (ret == -1)
        return -1;
    
    if (cmd_len > 0) {
        for (i = 0; i < cmd_len; i++) {
            ret = i2c_data_send(cmd[i]);
            
            if (ret == -1)
                return -1;
        }
    }
    
    if (data_len > 0) {
        for (i = 0; i < data_len; i++) {
            ret = i2c_data_recv(&data[i], data_len);
            
            if (ret == -1)
                return -1;
        }
    }
    
    if (opt == I2C_OPT_NORMAL)
        i2c_stop();
    
    return 0;
}

int i2c_get_error(void)
{
    return err;
}

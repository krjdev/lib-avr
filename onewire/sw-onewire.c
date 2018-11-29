/**
 *
 * File Name: sw-onewire.h
 * Title    : Software based (bitbang) 1-Wire library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-28
 * Modified : 2018-11-29
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

#include <util/delay.h>

#include "sw-onewire.h"

void onewire_init(void)
{
    DQ_TX_CONFIG;
    DQ_TX_HIGH;
}

int onewire_reset(void)
{
    int ret;
    
    DQ_TX_LOW;
    _delay_us(480);
    DQ_TX_HIGH;
    _delay_us(70);
    
    if (!(DQ_RX_PORT & DQ_RX_PIN))
        ret = 1;
    else
        ret = 0;
            
    _delay_us(420);
    return ret;
}


int onewire_send(uint8_t *data, int len)
{
    int i, j;
    uint8_t tmp;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    for (j = 0; j < len; j++) {
        tmp = 0x01;
        
        for (i = 0; i < 8; i++) {
            if (data[j] & tmp) {
                DQ_TX_LOW;
                _delay_us(1);
                DQ_TX_HIGH;
                _delay_us(80);
            } else {
                DQ_TX_LOW;
                _delay_us(80);
                DQ_TX_HIGH;
                _delay_us(1);
            }
        
            _delay_us(100); 
            tmp <<= 1;
        }
    }
    
    return 0;
}

int onewire_recv(uint8_t *data, int len)
{
    int i, j;
    uint8_t tmp;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    for (j = 0; j < len; j++) {
        tmp = 0x00;
        
        for (i = 0; i < 8; i++) {
            DQ_TX_LOW;
            DQ_TX_HIGH;
            _delay_us(15);
            
            if (DQ_RX_PORT & DQ_RX_PIN)
                tmp |= (1 << i);
            
            _delay_us(100);
        }
        
        _delay_us(100);
        *(data + j) = tmp;
    }
    
    return 0;
}

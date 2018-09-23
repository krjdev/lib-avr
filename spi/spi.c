/**
 *
 * File Name: spi.c
 * Title    : SPI interface library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-22
 * Modified : 
 * Revised  : 
 * Version  : 0.1.0.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel ATmega2560
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <avr/io.h>

#include "spi.h"

void spi_master_init(void)
{
    /* set MOSI, SCK and SS as output */
    DDRB = (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
    
    /* enable SPI, set master */
    SPCR = (1 << SPE) | (1 << MSTR);
}

void spi_master_send(uint8_t *data, int len)
{
    int i;
    
    if (!data)
        return;
    
    if (len < 1)
        return;
    
    for (i = 0; i < len; i++) {
        SPDR = data[i];
        
        while (!(SPSR & (1 << SPIF)))
            ;
    }
}

void spi_master_recv(uint8_t *data, int len)
{
    int i;
    
    if (!data)
        return;
    
    if (len < 1)
        return;
    
    for (i = 0; i < len; i++) {
        /* send dummy */
        SPDR = 0xFF;
        
        while (!(SPSR & (1 << SPIF)))
            ;
        
        data[i] = SPDR;
    }
}

/**
 *
 * File Name: spi.c
 * Title    : SPI interface library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-22
 * Modified : 2018-09-24
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

#include "spi.h"

void spi_master_init(int speed)
{
    /* set MOSI, SCK and SS as output */
    SPI_PORT |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
    
    /* enable SPI, set master */
    SPCR |= (1 << SPE) | (1 << MSTR);

    /* set speed */
    switch (speed) {
    case SPI_FOSC_2:
        SPSR |= (1 << SPI2X);
        break;
    case SPI_FOSC_4:
        break;
    case SPI_FOSC_8:
        SPSR |= (1 << SPI2X);
        break;
    case SPI_FOSC_16:
        SPCR |= (1 << SPR0);
        break;
    case SPI_FOSC_32:
        SPSR |= (1 << SPI2X);
        break;
    case SPI_FOSC_64:
        SPCR |= (1 << SPR1);
        break;
    case SPI_FOSC_128:
        SPCR |= (1 << SPR0) | (1 << SPR1);
        break;
    default:
        return;
    }
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

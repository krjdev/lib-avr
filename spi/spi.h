/**
 *
 * File Name: spi.h
 * Title    : SPI interface library header
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

#ifndef LIBAVR_SPI_SPI_H
#define LIBAVR_SPI_SPI_H

#include <stdint.h>
#include <avr/io.h>

#define SPI_PORT    DDRB
#define SPI_MOSI    DDB2
#define SPI_SCK     DDB1
#define SPI_SS      DDB0

extern void spi_master_init(void);
extern void spi_master_send(uint8_t *data, int len);
extern void spi_master_recv(uint8_t *data, int len);

#endif

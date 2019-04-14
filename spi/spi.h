/**
 *
 * File Name: spi.h
 * Title    : SPI interface library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-22
 * Modified : 2019-04-14
 * Revised  : 
 * Version  : 0.2.1.0
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

#define SPI_PORT        DDRB
#define SPI_MOSI        DDB2
#define SPI_SCK         DDB1
#define SPI_SS          DDB0

/* Speed */
#define SPI_FOSC_2      0
#define SPI_FOSC_4      1
#define SPI_FOSC_8      2
#define SPI_FOSC_16     3
#define SPI_FOSC_32     4
#define SPI_FOSC_64     5
#define SPI_FOSC_128    6

/* SPI mode */
#define SPI_MODE_0      0
#define SPI_MODE_1      1
#define SPI_MODE_2      2
#define SPI_MODE_3      3

/* SPI data order */
#define SPI_ORDER_MSB   0
#define SPI_ORDER_LSB   1

extern void spi_master_init(int mode, int speed, int order);
extern void spi_master_send(uint8_t *data, int len);
extern void spi_master_recv(uint8_t *data, int len);

#endif

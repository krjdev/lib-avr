/**
 *
 * File Name: spi.h
 * Title    : SPI interface library header
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

#include <stdint.h>

#ifndef LIBAVR_SPI_H
#define LIBAVR_SPI_H

extern void spi_master_init(void);
extern void spi_master_send(uint8_t *data, int len);
extern void spi_master_recv(uint8_t *data, int len);

#endif

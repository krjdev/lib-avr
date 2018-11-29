/**
 *
 * File Name: crc_dallas.h
 * Title    : CRC calculation for Dallas/Maxim 1-Wire devices header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-11-29
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

#ifndef LIBAVR_LIB_CRC_DALLAS_H
#define LIBAVR_LIB_CRC_DALLAS_H

#include <stdint.h>

extern int crc8_dallas_calc(uint8_t *data, int len);
extern int crc8_dallas_check(uint8_t *data, int len, uint8_t crc);

#endif

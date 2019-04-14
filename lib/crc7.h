/**
 *
 * File Name: crc7.h
 * Title    : CRC calculation for MMC/SD cards
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-04-14
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

#ifndef LIBAVR_LIB_CRC7_H
#define LIBAVR_LIB_CRC7_H

#include <stdint.h>

extern int crc7_calc(uint8_t *data, int len);
extern int crc7_check(uint8_t *data, int len, uint8_t crc);

#endif

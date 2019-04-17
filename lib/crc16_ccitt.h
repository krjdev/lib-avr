/**
 *
 * File Name: crc16_ccitt.h
 * Title    : CRC16-CCITT calculation library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-04-17
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

#ifndef LIBAVR_LIB_CRC16_CCITT_H
#define LIBAVR_LIB_CRC16_CCITT_H

#include <stdint.h>

extern uint16_t crc16_ccitt_calc(uint8_t *data, int len);
extern int crc16_ccitt_check(uint8_t *data, int len, uint16_t crc);

#endif

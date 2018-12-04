/**
 *
 * File Name: bcd.h
 * Title    : BCD (Binary Coded Data) conversion functions header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-12-04
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

#ifndef LIBAVR_LIB_BCD_H
#define LIBAVR_LIB_BCD_H

#include <stdint.h>

extern uint8_t bcd_to_int(uint8_t bcd);
extern uint8_t int_to_bcd(uint8_t i);

#endif

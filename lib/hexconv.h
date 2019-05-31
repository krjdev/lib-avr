/**
 *
 * File Name: hexconv.h
 * Title    : HEX conversion library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-05-31
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

#ifndef LIBAVR_LIB_HEXCONV_H
#define LIBAVR_LIB_HEXCONV_H

#include <stdint.h>

extern char *uint8_to_hex(uint8_t data);
extern uint8_t hex_to_uint8(const char *hex);

#endif

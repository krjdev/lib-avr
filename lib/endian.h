/**
 *
 * File Name: endian.h
 * Title    : Endianness (Byte Order) library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-05-29
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

#ifndef LIBAVR_LIB_ENDIAN_H
#define LIBAVR_LIB_ENDIAN_H

#include <stdint.h>

extern uint16_t buf_to_uint16_le(uint8_t *buf);
extern uint32_t buf_to_uint32_le(uint8_t *buf);
extern uint64_t buf_to_uint64_le(uint8_t *buf);
extern uint16_t buf_to_uint16_be(uint8_t *buf);
extern uint32_t buf_to_uint32_be(uint8_t *buf);
extern uint64_t buf_to_uint64_be(uint8_t *buf);
extern void uint16_to_buf_le(uint16_t val, uint8_t *buf);
extern void uint32_to_buf_le(uint32_t val, uint8_t *buf);
extern void uint64_to_buf_le(uint64_t val, uint8_t *buf);
extern void uint16_to_buf_be(uint16_t val, uint8_t *buf);
extern void uint32_to_buf_be(uint32_t val, uint8_t *buf);
extern void uint64_to_buf_be(uint64_t val, uint8_t *buf);

#endif

/**
 *
 * File Name: crc32_ethernet.h
 * Title    : CRC32 calculation for Ethernet (FCS) library
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

#ifndef LIBAVR_LIB_CRC32_ETHERNET_H
#define LIBAVR_LIB_CRC32_ETHERNET_H

#include <stdint.h>

extern uint32_t crc32_calc(uint8_t *data, int len);
extern int crc32_check(uint8_t *data, int len, uint32_t crc);

#endif

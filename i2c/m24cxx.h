/**
 *
 * File Name: m24cxx.h
 * Title    : STmicroelectronics M24C01 - M24C64 I2C EEPROM driver header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-12-04
 * Modified : 2019-04-04
 * Revised  : 
 * Version  : 0.1.1.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdint.h>

#ifndef LIBAVR_I2C_M24CXX_H
#define LIBAVR_I2C_M24CXX_H

#define TYPE_M24C01         0
#define TYPE_M24C02         1
#define TYPE_M24C04         2
#define TYPE_M24C08         3
#define TYPE_M24C16         4
#define TYPE_M24C32         5
#define TYPE_M24C64         6

extern void m24cxx_init(void);
extern int m24cxx_write(int type, uint8_t subaddr, uint16_t addr, uint8_t *buf, int len);
extern int m24cxx_read(int type, uint8_t subaddr, uint16_t addr, uint8_t *buf, int len);

#endif

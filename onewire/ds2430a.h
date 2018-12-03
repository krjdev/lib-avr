/**
 *
 * File Name: ds2430a.h
 * Title    : Dallas/Maxim DS2430A library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-12-01
 * Modified : 2018-12-03
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

#ifndef LIBAVR_ONEWIRE_DS2430A_H
#define LIBAVR_ONEWIRE_DS2430A_H

#include <stdint.h>

extern void ds2430a_init(void);
extern int ds2430a_write_memory(uint8_t addr, uint8_t *buf, int len);
extern int ds2430a_read_memory(uint8_t addr, uint8_t *buf, int len);

#endif

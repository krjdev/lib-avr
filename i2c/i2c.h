/**
 *
 * File Name: i2c.h
 * Title    : I2C interface library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-21
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

#include <stdint.h>

#ifndef LIBAVR_I2C_I2C_H
#define LIBAVR_I2C_I2C_H

extern int i2c_master_init(uint32_t speed);
extern int i2c_master_send(uint8_t addr, uint8_t *data, int len);
extern int i2c_master_recv(uint8_t addr, uint8_t *data, int len);
extern int i2c_get_error(void);

#endif

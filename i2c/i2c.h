/**
 *
 * File Name: i2c.h
 * Title    : I2C interface library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-21
 * Modified : 2019-03-24
 * Revised  : 
 * Version  : 0.2.1.0
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

#define I2C_OPT_NORMAL      0
#define I2C_OPT_NOSTOP      1

extern int i2c_master_init(uint32_t speed);
extern int i2c_master_send(int opt, 
                           uint8_t addr, 
                           uint8_t *cmd, 
                           int cmd_len, 
                           uint8_t *data, 
                           int data_len);
extern int i2c_master_recv(int opt, 
                           uint8_t addr, 
                           uint8_t *cmd, 
                           int cmd_len, 
                           uint8_t *data, 
                           int data_len);
extern int i2c_get_error(void);

#endif

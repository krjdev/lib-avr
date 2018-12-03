/**
 *
 * File Name: pcf8591.h
 * Title    : NXP PCF8591 library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-12-03
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

#ifndef LIBAVR_I2C_PCF8591_H
#define LIBAVR_I2C_PCF8591_H

#define CONFIG_ADC0         0 /* Four single-ended inputs */
#define CONFIG_ADC1         1 /* Three differential inputs */
#define CONFIG_ADC2         2 /* Two single-ended and one differential input */
#define CONFIG_ADC3         3 /* Two differential inputs */

#define CHANNEL_ADC0        0
#define CHANNEL_ADC1        1
#define CHANNEL_ADC2        2
#define CHANNEL_ADC3        3

extern void pcf8591_init(void);
extern int pcf8591_get_adc(uint8_t subaddr, int config, int channel, uint8_t *value);
extern int pcf8591_set_dac(uint8_t subaddr, uint8_t *value);

#endif

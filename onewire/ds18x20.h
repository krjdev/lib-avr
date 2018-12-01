/**
 *
 * File Name: ds18x20.h
 * Title    : Dallas/Maxim DS18S20 and DS18B20 library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-11-30
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

#ifndef LIBAVR_ONEWIRE_DS18X20_H
#define LIBAVR_ONEWIRE_DS18X20_H

#include <stdint.h>

#define RESOLUTION_9BIT     9
#define RESOLUTION_12BIT    12

typedef struct ds18x20_temp {
    uint8_t dt_templ;
    uint8_t dt_temph;
    uint8_t dt_cr;
    uint8_t dt_cpc;
} ds18x20_temp_t;

extern void ds18x20_init(int res);
extern ds18x20_temp_t *ds18x20_get_temp_data(void);

#endif

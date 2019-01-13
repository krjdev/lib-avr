/**
 *
 * File Name: ds18x20.h
 * Title    : Dallas/Maxim DS18S20 and DS18B20 library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-11-30
 * Modified : 2019-01-13
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

#ifndef LIBAVR_ONEWIRE_DS18X20_H
#define LIBAVR_ONEWIRE_DS18X20_H

#include <stdint.h>

#include "sw-onewire.h"

#define TYPE_DS18S20        0
#define TYPE_DS18B20        1

#define RESOLUTION_9BIT     9
#define RESOLUTION_10BIT    10 /* DS18B20 only */
#define RESOLUTION_11BIT    11 /* DS18B20 only */
#define RESOLUTION_12BIT    12

extern void ds18x20_init(void);
extern int ds18x20_read_rom(int type, ow_rom_t *rom);
extern int ds18x20_search_rom(int type, ow_rom_t *roms, int num);
extern int ds18x20_search_alarm(int type, ow_rom_t *roms, int num);
extern int ds18x20_convert(ow_rom_t *rom);
extern int ds18x20_get_temp(ow_rom_t *rom, int type, int res, float *temp);
extern int ds18x20_set_resolution(ow_rom_t *rom, int res); /* DS18B20 only */
extern int ds18x20_set_alarm(ow_rom_t *rom, int type, int8_t temp_high, int8_t temp_low);

#endif

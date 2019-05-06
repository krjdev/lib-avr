/**
 *
 * File Name: pcf8563.h
 * Title    : NXP PCF8563 library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-04-04
 * Modified : 2019-05-06
 * Revised  : 
 * Version  : 0.1.0.1
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdint.h>

#include "../lib/date.h"
#include "../lib/time.h"

#ifndef LIBAVR_I2C_PCF8563_H
#define LIBAVR_I2C_PCF8563_H

#define FREQ_4096HZ     0
#define FREQ_64HZ       1
#define FREQ_1HZ        2
#define FREQ_1DIV60HZ   3

#define ALARM_MIN       0x01
#define ALARM_HR        0x02
#define ALARM_DAY       0x04
#define ALARM_WDAY      0x08

extern void pcf8563_init(void);
extern int pcf8563_set_time(time_t *time);
extern int pcf8563_get_time(time_t *time);
extern int pcf8563_set_date(date_t *date);
extern int pcf8563_get_date(date_t *date);
extern int pcf8563_set_alarm(time_t *time, date_t *date, uint8_t flags);
extern int pcf8563_check_alarm(void);
extern int pcf8563_set_timer(int freq, uint8_t value);
extern int pcf8563_check_timer(void);
extern int pcf8563_set_clkout(int freq, int enable);

#endif

/**
 *
 * File Name: time.h
 * Title    : Time library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-03-22
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

#ifndef LIBAVR_LIB_TIME_H
#define LIBAVR_LIB_TIME_H

#include <stdint.h>

typedef struct time {
    uint8_t t_hr;
    uint8_t t_min;
    uint8_t t_sec;
    uint16_t t_msec;
} time_t;

extern int time_set(time_t *time, uint8_t hr, uint8_t min, uint8_t sec, uint16_t msec);
extern int time_set_hour(time_t *time, uint8_t hr);
extern int time_set_min(time_t *time, uint8_t min);
extern int time_set_sec(time_t *time, uint8_t sec);
extern int time_set_msec(time_t *time, uint16_t msec);
extern int time_get(time_t *time, uint8_t *hr, uint8_t *min, uint8_t *sec, uint16_t *msec);
extern int time_get_hour(time_t *time, uint8_t *hr);
extern int time_get_min(time_t *time, uint8_t *min);
extern int time_get_sec(time_t *time, uint8_t *sec);
extern int time_get_msec(time_t *time, uint16_t *msec);


#endif

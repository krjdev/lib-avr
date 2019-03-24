/**
 *
 * File Name: date.h
 * Title    : Date library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-03-24
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

#ifndef LIBAVR_LIB_DATE_H
#define LIBAVR_LIB_DATE_H

#include <stdint.h>

enum WEEKDAY {
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT,
    SUN
};

enum MONTH {
    JAN,
    FEB,
    MAR,
    APR,
    MAY,
    JUN,
    JUL,
    AUG,
    SEP,
    OCT,
    NOV,
    DEC
};

typedef struct date {
    uint8_t d_day;
    uint8_t d_weekday;
    uint8_t d_month;
    uint16_t d_year;
} date_t;

extern int date_set(date_t *date, uint8_t day, uint8_t wday, uint8_t mon, uint16_t year);
extern int date_set_day(date_t *date, uint8_t day);
extern int date_set_wday(date_t *date, uint8_t wday);
extern int date_set_month(date_t *date, uint8_t mon);
extern int date_set_year(date_t *date, uint16_t year);
extern int date_get(date_t *date, uint8_t *day, uint8_t *wday, uint8_t *mon, uint16_t *year);
extern int date_get_day(date_t *date, uint8_t *day);
extern int date_get_wday(date_t *date, uint8_t *wday);
extern int date_get_month(date_t *date, uint8_t *mon);
extern int date_get_year(date_t *date, uint16_t *year);


#endif

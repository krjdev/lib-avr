/**
 *
 * File Name: date.c
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

#include "date.h"

int date_set(date_t *date, uint8_t day, uint8_t wday, uint8_t mon, uint16_t year)
{
    if (!date)
        return -1;
    
    if (day < 1 && day > 31)
        return -1;
    
    switch (wday) {
    case MON:
        break;
    case TUE:
        break;
    case WED:
        break;
    case THU:
        break;
    case FRI:
        break;
    case SAT:
        break;
    case SUN:
        break;
    default:
        return -1;
    }
    
    switch (mon) {
    case JAN:
        break;
    case FEB:
        break;
    case MAR:
        break;
    case APR:
        break;
    case MAY:
        break;
    case JUN:
        break;
    case JUL:
        break;
    case AUG:
        break;
    case SEP:
        break;
    case OCT:
        break;
    case NOV:
        break;
    case DEC:
        break;
    default:
        return -1;
    }
    
    date->d_day = day;
    date->d_weekday = wday;
    date->d_month = mon;
    date->d_year = year;
    return 0;
}

int date_set_day(date_t *date, uint8_t day)
{
    if (!date)
        return -1;
    
    if (day < 1 && day > 31)
        return -1;
    
    date->d_day = day;
    return 0;
}

int date_set_wday(date_t *date, uint8_t wday)
{
    if (!date)
        return -1;
    
    switch (wday) {
    case MON:
        break;
    case TUE:
        break;
    case WED:
        break;
    case THU:
        break;
    case FRI:
        break;
    case SAT:
        break;
    case SUN:
        break;
    default:
        return -1;
    }
    
    date->d_weekday = wday;
    return 0;
}

int date_set_month(date_t *date, uint8_t mon)
{
    if (!date)
        return -1;
    
    switch (mon) {
    case JAN:
        break;
    case FEB:
        break;
    case MAR:
        break;
    case APR:
        break;
    case MAY:
        break;
    case JUN:
        break;
    case JUL:
        break;
    case AUG:
        break;
    case SEP:
        break;
    case OCT:
        break;
    case NOV:
        break;
    case DEC:
        break;
    default:
        return -1;
    }
    
    date->d_month = mon;
    return 0;
}

int date_set_year(date_t *date, uint16_t year)
{
    if (!date)
        return -1;
    
    date->d_year = year;
    return 0;
}

int date_get(date_t *date, uint8_t *day, uint8_t *wday, uint8_t *mon, uint16_t *year)
{
    if (!date)
        return -1;
    
    (*day) = date->d_day;
    (*wday) = date->d_weekday;
    (*mon) = date->d_month;
    (*year) = date->d_year;
    return 0;
}

int date_get_day(date_t *date, uint8_t *day)
{
    if (!date)
        return -1;
    
    (*day) = date->d_day;
    return 0;
}

int date_get_wday(date_t *date, uint8_t *wday)
{
    if (!date)
        return -1;
    
    (*wday) = date->d_weekday;
    return 0;
}

int date_get_month(date_t *date, uint8_t *mon)
{
    if (!date)
        return -1;
    
    (*mon) = date->d_month;
    return 0;
}

int date_get_year(date_t *date, uint16_t *year)
{
    if (!date)
        return -1;
    
    (*year) = date->d_year;
    return 0;
}

/**
 *
 * File Name: time.c
 * Title    : Time library source
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

#include "time.h"

int time_set(time_t *time, uint8_t hr, uint8_t min, uint8_t sec, uint16_t msec)
{
    if (!time)
        return -1;
    
    if (hr > 23)
        return -1;
    
    if (min > 59)
        return -1;
    
    if (sec > 59)
        return -1;
    
    if (msec > 999)
        return -1;
    
    time->t_hr = hr;
    time->t_min = min;
    time->t_sec = sec;
    time->t_msec = msec;
    return 0;
}

int time_set_hour(time_t *time, uint8_t hr)
{
    if (!time)
        return -1;
    
    if (hr > 23)
        return -1;
    
    time->t_hr = hr;
    return 0;
}

int time_set_min(time_t *time, uint8_t min)
{
    if (!time)
        return -1;
    
    if (min > 59)
        return -1;
    
    time->t_min = min;
    return 0;
}

int time_set_sec(time_t *time, uint8_t sec)
{
    if (!time)
        return -1;
    
    if (sec > 59)
        return -1;
    
    time->t_sec = sec;
    return 0;
}

int time_set_msec(time_t *time, uint16_t msec)
{
    if (!time)
        return -1;
    
    if (msec > 999)
        return -1;
    
    time->t_msec = msec;
    return 0;
}

int time_get(time_t *time, uint8_t *hr, uint8_t *min, uint8_t *sec, uint16_t *msec)
{
    if (!time)
        return -1;
    
    (*hr) = time->t_hr;
    (*min) = time->t_min;
    (*sec) = time->t_sec;
    (*msec) = time->t_msec;
    return 0;
}

int time_get_hour(time_t *time, uint8_t *hr)
{
    if (!time)
        return -1;
    
    (*hr) = time->t_hr;
    return 0;
}

int time_get_min(time_t *time, uint8_t *min)
{
    if (!time)
        return -1;
    
    (*min) = time->t_min;
    return 0;
}

int time_get_sec(time_t *time, uint8_t *sec)
{
    if (!time)
        return -1;
    
    (*sec) = time->t_sec;
    return 0;
}

int time_get_msec(time_t *time, uint16_t *msec)
{
    if (!time)
        return -1;
    
    (*msec) = time->t_msec;
    return 0;
}

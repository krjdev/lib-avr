/**
 *
 * File Name: pcf8563.c
 * Title    : NXP PCF8563 library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-04-04
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

#include "i2c.h"
#include "../lib/bcd.h"
#include "pcf8563.h"

#define _ISCLR(reg, bit)            (((reg) | (1 << (bit))) ^ (reg))
#define _ISSET(reg, bit)            ((reg) & (1 << (bit)))

#define PCF8563_ADDR                0x51

#define PCF8563_REG_CONTROL1        0x00

    #define PCF8563_REG_CONTROL1_TEST1      7
    #define PCF8563_REG_CONTROL1_STOP       5
    #define PCF8563_REG_CONTROL1_TESTC      3

#define PCF8563_REG_CONTROL2        0x01

    #define PCF8563_REG_CONTROL2_TITP       4
    #define PCF8563_REG_CONTROL2_AF         3
    #define PCF8563_REG_CONTROL2_TF         2
    #define PCF8563_REG_CONTROL2_AIE        1
    #define PCF8563_REG_CONTROL2_TIE        0

#define PCF8563_REG_SECONDS         0x02

    #define PCF8563_REG_SECONDS_VL          7

#define PCF8563_REG_MINUTES         0x03
#define PCF8563_REG_HOURS           0x04
#define PCF8563_REG_DAYS            0x05
#define PCF8563_REG_WEEKDAYS        0x06

    #define PCF8563_WEEKDAY_SUN             0
    #define PCF8563_WEEKDAY_MON             1
    #define PCF8563_WEEKDAY_TUE             2
    #define PCF8563_WEEKDAY_WED             3
    #define PCF8563_WEEKDAY_THU             4
    #define PCF8563_WEEKDAY_FRI             5
    #define PCF8563_WEEKDAY_SAT             6

#define PCF8563_REG_MONTHS          0x07

    #define PCF8563_REG_MONTHS_C            7
    
    #define PCF8563_MONTH_JAN               1
    #define PCF8563_MONTH_FEB               2
    #define PCF8563_MONTH_MAR               3
    #define PCF8563_MONTH_APR               4
    #define PCF8563_MONTH_MAY               5
    #define PCF8563_MONTH_JUN               6
    #define PCF8563_MONTH_JUL               7
    #define PCF8563_MONTH_AUG               8
    #define PCF8563_MONTH_SEP               9
    #define PCF8563_MONTH_OCT               10
    #define PCF8563_MONTH_NOV               11
    #define PCF8563_MONTH_DEC               12

#define PCF8563_REG_YEARS           0x08
#define PCF8563_REG_ALARM_MINUTE    0x09

    #define PCF8563_REG_ALARM_MINUTE_AE     7

#define PCF8563_REG_ALARM_HOUR      0x0A

    #define PCF8563_REG_ALARM_HOUR_AE       7

#define PCF8563_REG_ALARM_DAY       0x0B

    #define PCF8563_REG_ALARM_DAY_AE        7

#define PCF8563_REG_ALARM_WEEKDAY   0x0C

    #define PCF8563_REG_ALARM_WEEKDAY_AE    7

#define PCF8563_REG_CLKOUT          0x0D

    #define PCF8563_REG_CLKOUT_FE           7

#define PCF8563_REG_TIMERCTRL       0x0E

    #define PCF8563_REG_TIMERCTRL_TE        7

#define PCF8563_REG_TIMER           0x0F

inline int dec_lsh(int x, int y)
{
    int i;
    int tmp = x;
    
    if (y == 1)
        return tmp;
    
    for (i = 1; i < y; i++)
        tmp *= 10;
    
    return tmp;
}

static int dec_cat(int val, int num)
{
    int tmp = val;
    int cnt = 0;
    int i;
    int a;
    
    while (tmp != 0) {
        tmp /= 10;
        cnt++;
    }
    
    tmp = val;
    
    for (i = cnt; i > num; i--) {
        a = (tmp / dec_lsh(1, i));
        
        if (a != 0)
            tmp = (tmp - (a * dec_lsh(1, i)));
    }
    
    return tmp;
}

void pcf8563_init(void)
{
    i2c_master_init(100000);
}

int pcf8563_set_time(time_t *time)
{
    uint8_t cmd;
    uint8_t val[3];
    
    cmd = PCF8563_REG_SECONDS;
    val[0] = int_to_bcd(time->t_sec);
    val[1] = int_to_bcd(time->t_min);
    val[2] = int_to_bcd(time->t_hr);
    i2c_master_send(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 3);
    return 0;
}

int pcf8563_get_time(time_t *time)
{
    uint8_t cmd;
    uint8_t val[3];
    
    cmd = PCF8563_REG_SECONDS;
    i2c_master_recv(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 3);
    
    if (_ISSET(val[0], PCF8563_REG_SECONDS_VL))
        return -1;
    
    time->t_sec = bcd_to_int(val[0]);
    time->t_min = bcd_to_int(val[1]);
    time->t_hr = bcd_to_int(val[2]);
    return 0;
}

int pcf8563_set_date(date_t *date)
{
    uint8_t cmd;
    uint8_t val[4];
    
    cmd = PCF8563_REG_DAYS;
    val[0] = int_to_bcd(date->d_day);
    
    switch (date->d_weekday) {
    case MON:
        val[1] = PCF8563_WEEKDAY_MON;
        break;
    case TUE:
        val[1] = PCF8563_WEEKDAY_TUE;
        break;
    case WED:
        val[1] = PCF8563_WEEKDAY_WED;
        break;
    case THU:
        val[1] = PCF8563_WEEKDAY_THU;
        break;
    case FRI:
        val[1] = PCF8563_WEEKDAY_FRI;
        break;
    case SAT:
        val[1] = PCF8563_WEEKDAY_SAT;
        break;
    case SUN:
        val[1] = PCF8563_WEEKDAY_SUN;
        break;
    default:
        return -1;
    }
    
    switch (date->d_month) {
    case JAN:
        val[2] = PCF8563_MONTH_JAN;
        break;
    case FEB:
        val[2] = PCF8563_MONTH_FEB;
        break;
    case MAR:
        val[2] = PCF8563_MONTH_MAR;
        break;
    case APR:
        val[2] = PCF8563_MONTH_APR;
        break;
    case MAY:
        val[2] = PCF8563_MONTH_MAY;
        break;
    case JUN:
        val[2] = PCF8563_MONTH_JUN;
        break;
    case JUL:
        val[2] = PCF8563_MONTH_JUL;
        break;
    case AUG:
        val[2] = PCF8563_MONTH_AUG;
        break;
    case SEP:
        val[2] = PCF8563_MONTH_SEP;
        break;
    case OCT:
        val[2] = PCF8563_MONTH_OCT;
        break;
    case NOV:
        val[2] = PCF8563_MONTH_NOV;
        break;
    case DEC:
        val[2] = PCF8563_MONTH_DEC;
        break;
    default:
        return -1;
    }
    
    val[3] = int_to_bcd(dec_cat(date->d_year, 2));
    i2c_master_send(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 4);
    return 0;
}

int pcf8563_get_date(date_t *date)
{
    uint8_t cmd;
    uint8_t val[4];
    
    cmd = PCF8563_REG_DAYS;
    i2c_master_recv(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 4);
    date->d_day = bcd_to_int(val[0]);
    
    switch (val[1]) {
    case PCF8563_WEEKDAY_MON:
        date->d_weekday = MON;
        break;
    case PCF8563_WEEKDAY_TUE:
        date->d_weekday = TUE;
        break;
    case PCF8563_WEEKDAY_WED:
        date->d_weekday = WED;
        break;
    case PCF8563_WEEKDAY_THU:
        date->d_weekday = THU;
        break;
    case PCF8563_WEEKDAY_FRI:
        date->d_weekday = FRI;
        break;
    case PCF8563_WEEKDAY_SAT:
        date->d_weekday = SAT;
        break;
    case PCF8563_WEEKDAY_SUN:
        date->d_weekday = SUN;
        break;
    default:
        return -1;
    }
    
    switch (val[2]) {
    case PCF8563_MONTH_JAN:
        date->d_month = JAN;
        break;
    case PCF8563_MONTH_FEB:
        date->d_month = FEB;
        break;
    case PCF8563_MONTH_MAR:
        date->d_month = MAR;
        break;
    case PCF8563_MONTH_APR:
        date->d_month = APR;
        break;
    case PCF8563_MONTH_MAY:
        date->d_month = MAY;
        break;
    case PCF8563_MONTH_JUN:
        date->d_month = JUN;
        break;
    case PCF8563_MONTH_JUL:
        date->d_month = JUL;
        break;
    case PCF8563_MONTH_AUG:
        date->d_month = AUG;
        break;
    case PCF8563_MONTH_SEP:
        date->d_month = SEP;
        break;
    case PCF8563_MONTH_OCT:
        date->d_month = OCT;
        break;
    case PCF8563_MONTH_NOV:
        date->d_month = NOV;
        break;
    case PCF8563_MONTH_DEC:
        date->d_month = DEC;
        break;
    default:
        return -1;
    }
    
    date->d_year = 2000 + bcd_to_int(val[3]);
    return 0;
}

int pcf8563_set_alarm(time_t *time, date_t *date, uint8_t flags)
{
    uint8_t cmd;
    uint8_t val[4];
    
    cmd = PCF8563_REG_ALARM_MINUTE;
    val[0] = int_to_bcd(time->t_min);
    val[1] = int_to_bcd(time->t_hr);
    val[2] = int_to_bcd(date_>d_day);
    
    switch (date->d_weekday) {
    case MON:
        val[3] = PCF8563_WEEKDAY_MON;
        break;
    case TUE:
        val[3] = PCF8563_WEEKDAY_TUE;
        break;
    case WED:
        val[3] = PCF8563_WEEKDAY_WED;
        break;
    case THU:
        val[3] = PCF8563_WEEKDAY_THU;
        break;
    case FRI:
        val[3] = PCF8563_WEEKDAY_FRI;
        break;
    case SAT:
        val[3] = PCF8563_WEEKDAY_SAT;
        break;
    case SUN:
        val[3] = PCF8563_WEEKDAY_SUN;
        break;
    default:
        return -1;
    }
    
    if (flags & ALARM_MIN)
        val[0] |= PCF8563_REG_ALARM_MINUTE_AE;
    
    if (flags & ALARM_HR)
        val[1] |= PCF8563_REG_ALARM_HOUR_AE;
    
    if (flags & ALARM_DAY)
        val[2] |= PCF8563_REG_ALARM_DAY_AE;
    
    if (flags & ALARM_WDAY)
        val[3] |= PCF8563_REG_ALARM_WEEKDAY_AE;
    
    i2c_master_send(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 4);
    return 0;
}

int pcf8563_check_alarm(void)
{
    uint8_t cmd;
    uint8_t val[1];
    
    cmd = PCF8563_REG_CONTROL2;
    i2c_master_recv(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 1);
    
    if (_ISSET(val[0], PCF8563_REG_CONTROL2_AF))
        return 1;
    else
        return 0;
}

int pcf8563_set_timer(int freq, uint8_t value)
{
    uint8_t cmd;
    uint8_t val[2];
    
    cmd = PCF8563_REG_TIMERCTRL;
    
    switch (freq) {
    case FREQ_4096HZ:
        val[0] = (1 << PCF8563_REG_TIMERCTRL_TE) | FREQ_4096HZ;
        break;
    case FREQ_64HZ:
        val[0] = (1 << PCF8563_REG_TIMERCTRL_TE) | FREQ_64HZ;
        break;
    case FREQ_1HZ:
        val[0] = (1 << PCF8563_REG_TIMERCTRL_TE) | FREQ_1HZ;
        break;
    case FREQ_1DIV60HZ:
        val[0] = (1 << PCF8563_REG_TIMERCTRL_TE) | FREQ_1DIV60HZ;
        break;
    default:
        return -1;
    }
    
    val[1] = value;
    i2c_master_send(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 2);
    return 0;
}

int pcf8563_check_timer(void)
{
    uint8_t cmd;
    uint8_t val[1];
    
    cmd = PCF8563_REG_CONTROL2;
    i2c_master_recv(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 1);
    
    if (_ISSET(val[0], PCF8563_REG_CONTROL2_TF))
        return 1;
    else
        return 0;
}

int pcf8563_set_clkout(int freq, int enable)
{
    uint8_t cmd;
    uint8_t val[1];
    
    cmd = PCF8563_REG_CLKOUT;
    
    switch (freq) {
    case FREQ_4096HZ:
        val[0] = (1 << PCF8563_REG_CLKOUT_FE) | FREQ_4096HZ;
        break;
    case FREQ_64HZ:
        val[0] = (1 << PCF8563_REG_CLKOUT_FE) | FREQ_64HZ;
        break;
    case FREQ_1HZ:
        val[0] = (1 << PCF8563_REG_CLKOUT_FE) | FREQ_1HZ;
        break;
    case FREQ_1DIV60HZ:
        val[0] = (1 << PCF8563_REG_CLKOUT_FE) | FREQ_1DIV60HZ;
        break;
    default:
        return -1;
    }
    
    i2c_master_send(I2C_OPT_NORMAL, PCF8563_ADDR, &cmd, 1, val, 1);
    return 0;
}

/**
 *
 * File Name: ds18x20.h
 * Title    : Dallas/Maxim DS18S20 and DS18B20 library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-11-30
 * Modified : 2018-01-12
 * Revised  : 
 * Version  : 0.2.0.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "../lib/crc8_dallas.h"
#include "ds18x20.h"

/* Commands */
#define CMD_CONVERT_T           0x44
#define CMD_SCRATCHPAD_READ     0xBE
#define CMD_SCRATCHPAD_WRITE    0x4E
#define CMD_SCRATCHPAD_COPY     0x48
#define CMD_RECALL_E2           0xB8
#define CMD_POWERSUP_READ       0xB4

/* Family code */
#define FAMILY_DS18S20          0x10
#define FAMILY_DS18B20          0x28

/* Scratchpad */
#define REG_TEMPL               0
#define REG_TEMPH               1
#define REG_TH                  2
#define REG_TL                  3
#define REG_CONFIG              4 /* DS18B20 only */
#define REG_COUNTRE             6 /* DS18S20 only */
#define REG_COUNTPC             7 /* DS18S20 only */
#define REG_CRC                 8

/* Temperature conversion time (milliseconds) */
#define CONVTIME_9BIT           95
#define CONVTIME_10BIT          190
#define CONVTIME_11BIT          375
#define CONVTIME_12BIT          750 /* DS18S20 needs always this time */

static void write_scratchpad(int type, uint8_t *buf)
{
    uint8_t cmd[4];
    
    if (!buf)
        return;

    cmd[0] = CMD_SCRATCHPAD_WRITE;
    cmd[1] = buf[0];
    cmd[2] = buf[1];
    
    if (type == TYPE_DS18S20) 
        onewire_send(cmd, 3);
    else {
        cmd[3] = buf[2];
        onewire_send(cmd, 4);
    }
}

static void read_scratchpad(uint8_t *buf)
{
    uint8_t cmd;
    uint8_t tmp[9];
    
    if (!buf)
        return;
    
    cmd = CMD_SCRATCHPAD_READ;
    onewire_send(&cmd, 1);
    onewire_recv(tmp, 9);
    memcpy(buf, tmp, 9);
}

static void copy_scratchpad(void)
{
    uint8_t cmd;
    
    cmd = CMD_SCRATCHPAD_COPY;
    onewire_send(&cmd, 1);
}

void ds18x20_init(void)
{
    onewire_init();
}

int ds18x20_read_rom(int type, ow_rom_t *rom)
{
    if (onewire_read_rom(rom) == -1)
        return -1;
    
    return 0;
}

int ds18x20_search_rom(int type, ow_rom_t *roms, int num)
{
    int ret;
    
    switch (type) {
    case TYPE_DS18S20:
        ret = onewire_search_family(TYPE_SEARCH_ALL, FAMILY_DS18S20, roms, num);
        break;
    case TYPE_DS18B20:
        ret = onewire_search_family(TYPE_SEARCH_ALL, FAMILY_DS18B20, roms, num);
        break;
    default:
        return -1;
    }
    
    return ret;
}

int ds18x20_search_alarm(int type, ow_rom_t *roms, int num)
{
    int ret;
    
    switch (type) {
    case TYPE_DS18S20:
        ret = onewire_search_family(TYPE_SEARCH_ALARM, FAMILY_DS18S20, roms, num);
        break;
    case TYPE_DS18B20:
        ret = onewire_search_family(TYPE_SEARCH_ALARM, FAMILY_DS18B20, roms, num);
        break;
    default:
        return -1;
    }
    
    return ret;
}

int ds18x20_convert(ow_rom_t *rom)
{
    uint8_t cmd;
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    cmd = CMD_CONVERT_T;
    onewire_send(&cmd, 1);
    _delay_ms(CONVTIME_12BIT);
    return 0;
}

int ds18x20_get_temp(ow_rom_t *rom, int type, int res, float *temp)
{
    uint8_t buf[9];
    uint16_t temp_raw;
    float temp_ret;
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    read_scratchpad(buf);
    
    if (!crc8_dallas_check(buf, 8, buf[REG_CRC]))
        return -1;
    
    temp_raw = (buf[REG_TEMPH] << 8);
    temp_raw |= buf[REG_TEMPL];
    
    switch (res) {
    case RESOLUTION_9BIT:
        if (type == TYPE_DS18S20)
            temp_ret = (float) temp_raw / 2.0;
        else {
            temp_raw &= 0xFFF8;
            temp_ret = (float) temp_raw / 16.0;
        }
        break;
    case RESOLUTION_10BIT:
        if (type == TYPE_DS18S20)
            return -1;
        else {
            temp_raw &= 0xFFFC;
            temp_ret = (float) temp_raw / 16.0;
        }
        break;
    case RESOLUTION_11BIT:
        if (type == TYPE_DS18S20)
            return -1;
        else {
            temp_raw &= 0xFFFE;
            temp_ret = (float) temp_raw / 16.0;
        }
        break;
    case RESOLUTION_12BIT:
        if (type == TYPE_DS18S20)
            temp_ret = (float) temp_raw - 0.25 + 
                       ((buf[REG_COUNTPC] - buf[REG_COUNTRE]) / 
                       buf[REG_COUNTPC]);
        else
            temp_ret = (float) temp_raw / 16.0;
        break;
    default:
        return -1;
    }
    
    (*temp) = temp_ret;
    return 0;
}

int ds18x20_set_resolution(ow_rom_t *rom, int res)
{
    uint8_t buf[9];
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    read_scratchpad(buf);
    
    switch (res) {
    case RESOLUTION_9BIT:
        buf[2] = 0x1F;
        break;
    case RESOLUTION_10BIT:
        buf[2] = 0x3F;
        break;
    case RESOLUTION_11BIT:
        buf[2] = 0x5F;
        break;
    case RESOLUTION_12BIT:
        buf[2] = 0x7F;
        break;
    default:
        return -1;
    }
    
    write_scratchpad(TYPE_DS18B20, buf);
    copy_scratchpad();
    _delay_ms(20);
    return 0;
}

int ds18x20_set_alarm(ow_rom_t *rom, int type, int8_t temp_high, int8_t temp_low)
{
    uint8_t buf[9];
    uint8_t tmp;
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    if (type == TYPE_DS18S20) {
        buf[0] = temp_high;
        buf[1] = temp_low;
        write_scratchpad(TYPE_DS18S20, buf);
    } else {
        read_scratchpad(buf);
        tmp = buf[REG_CONFIG];
        buf[0] = temp_high;
        buf[1] = temp_low;
        buf[2] = tmp;
        write_scratchpad(TYPE_DS18B20, buf);
    }
    
    copy_scratchpad();
    _delay_ms(20);
    return 0;
}

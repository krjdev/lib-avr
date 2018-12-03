/**
 *
 * File Name: ds18x20.h
 * Title    : Dallas/Maxim DS18S20 and DS18B20 library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-11-30
 * Modified : 2018-12-03
 * Revised  : 
 * Version  : 0.1.2.0 - Alpha
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

#include "sw-onewire.h"
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
#define REG_CONFIG              4 /* Only on DS18B20 available */
#define REG_COUNTRE             6 /* Only on DS18S20 available */
#define REG_COUNTPC             7 /* Only on DS18S20 available */
#define REG_CRC                 8

/* Temperature conversion time (milliseconds) */
#define CONVTIME_9BIT           95
#define CONVTIME_10BIT          190
#define CONVTIME_11BIT          375
#define CONVTIME_12BIT          750 /* DS18S20 needs always this time */

int type = 0;
int resolution = 0;

static void write_scratchpad(uint8_t *buf)
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
    memcpy(&buf, tmp, 9);
}

static void copy_scratchpad(void)
{
    uint8_t cmd;
    
    cmd = CMD_SCRATCHPAD_COPY;
    onewire_send(&cmd, 1);
}

void ds18x20_init(int typ, int res)
{
    switch (typ) {
    case TYPE_DS18S20:
    {
        switch (res) {
        case RESOLUTION_9BIT:
            resolution = RESOLUTION_9BIT;
            break;
        case RESOLUTION_12BIT:
            resolution = RESOLUTION_12BIT;
        default:
            return;
        }
        
        break;
    }
    case TYPE_DS18B20:
    {
        switch (res) {
        case RESOLUTION_9BIT:
            resolution = RESOLUTION_9BIT;
            break;
        case RESOLUTION_10BIT:
            resolution = RESOLUTION_10BIT;
            break;
        case RESOLUTION_11BIT:
            resolution = RESOLUTION_11BIT;
            break;
        case RESOLUTION_12BIT:
            resolution = RESOLUTION_12BIT;
        default:
            return;
        }
        
        break;
    }
    default:
        return;
    }
    
    type = typ;
    onewire_init();
}

int ds18x20_get_temp_data(ds18x20_temp_t *dt)
{
    uint8_t cmd;
    uint8_t data[9];
    uint8_t family;
    ow_id_t owid;
    
    if (onewire_read_rom(&owid) == -1)
        return -1;
    
    onewire_get_family(&owid, &family);
    
    cmd = CMD_CONVERT_T;
    onewire_send(&cmd, 1);
    
    if (family == FAMILY_DS18S20)
        _delay_ms(CONVTIME_12BIT);
    else {
        switch (resolution) {
        case RESOLUTION_9BIT:
            _delay_ms(CONVTIME_9BIT);
            break;
        case RESOLUTION_10BIT:
            _delay_ms(CONVTIME_10BIT);
            break;
        case RESOLUTION_11BIT:
            _delay_ms(CONVTIME_11BIT);
            break;
        case RESOLUTION_12BIT:
            _delay_ms(CONVTIME_12BIT);
        default:
            return -1;
        }
    }
    
    if (onewire_read_rom(&owid) == -1)
        return -1;
    
    read_scratchpad(data);
    
    if (!crc8_dallas_check(data, 8, data[REG_CRC]))
        return -1;
    
    dt->dt_templ = data[REG_TEMPL];
    dt->dt_temph = data[REG_TEMPH];
    dt->dt_cr = data[REG_COUNTRE];
    dt->dt_cpc = data[REG_COUNTPC];
    return 0;
}

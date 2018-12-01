/**
 *
 * File Name: ds18x20.h
 * Title    : Dallas/Maxim DS18S20 and DS18B20 library source
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

#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "sw-onewire.h"
#include "../lib/crc8_dallas.h"
#include "ds18x20.h"

#define CMD_CONVERT_T           0x44
#define CMD_SCRATCHPAD_READ     0xBE
#define CMD_SCRATCHPAD_WRITE    0x4E
#define CMD_SCRATCHPAD_COPY     0x48
#define CMD_RECALL_E2           0xB8
#define CMD_POWERSUP_READ       0xB4

#define REG_TEMPL               0
#define REG_TEMPH               1
#define REG_TH                  2
#define REG_TL                  3
#define REG_COUNTRE             6
#define REG_COUNTPC             7
#define REG_CRC                 8

#define FAMILY_DS18S20          0x10

int res = 0;

static void write_scratchpad(uint8_t *buf)
{
    uint8_t cmd[3];
    
    if (!buf)
        return;

    cmd[0] = CMD_SCRATCHPAD_WRITE;
    cmd[1] = buf[0];
    cmd[2] = buf[1];
    onewire_send(cmd, 3);
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

void ds18x20_init(int res)
{
    switch (res) {
    case RESOLUTION_9BIT:
        res = RESOLUTION_9BIT;
        break;
    case RESOLUTION_12BIT:
        res = RESOLUTION_12BIT;
    default:
        return;
    }
    
    onewire_init();
}

ds18x20_temp_t *ds18x20_get_temp_data(void)
{
    uint8_t cmd;
    uint8_t data[9];
    uint8_t family;
    onewire_id_t *oi;
    ds18x20_temp_t *p;
    
    oi = onewire_read_rom();
    onewire_get_family(oi, &family);
    free(oi);
    
    if (family != FAMILY_DS18S20)
        return NULL;
    
    cmd = CMD_CONVERT_T;
    onewire_send(&cmd, 1);
    _delay_ms(750);
    oi = onewire_read_rom();
    free(oi);
    read_scratchpad(data);
    
    if (!crc8_dallas_check(data, 8, data[REG_CRC]))
        return NULL;
    
    p = (ds18x20_temp_t *) malloc(sizeof(ds18x20_temp_t));
    
    if (!p)
        return NULL;
    
    p->dt_templ = data[REG_TEMPL];
    p->dt_temph = data[REG_TEMPH];
    p->dt_cr = data[REG_COUNTRE];
    p->dt_cpc = data[REG_COUNTPC];
    return p;
}

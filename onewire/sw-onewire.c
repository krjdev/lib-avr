/**
 *
 * File Name: sw-onewire.c
 * Title    : Software based (bitbang) 1-Wire library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-28
 * Modified : 2019-01-06
 * Revised  : 
 * Version  : 0.3.0.0
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

#define _ISCLR(reg, bit)            (((reg) | (1 << (bit))) ^ (reg))
#define _ISSET(reg, bit)            ((reg) & (1 << (bit)))

#define ONEWIRE_CMD_ROM_SEARCH      0xF0
#define ONEWIRE_CMD_ROM_READ        0x33
#define ONEWIRE_CMD_ROM_MATCH       0x55
#define ONEWIRE_CMD_ROM_SKIP        0xCC
#define ONEWIRE_CMD_ALARM_SEARCH    0xEC

static void write_bit(uint8_t val)
{
    if (val) {
        DQ_TX_LOW;
        _delay_us(1);
        DQ_TX_HIGH;
        _delay_us(80);
    } else {
        DQ_TX_LOW;
        _delay_us(80);
        DQ_TX_HIGH;
        _delay_us(1);
    }
    
    _delay_us(100);
}

static uint8_t read_bit(void)
{
    uint8_t ret;
    
    DQ_TX_LOW;
    _delay_us(1);
    DQ_TX_HIGH;
    _delay_us(15);
    
    if (DQ_RX_PORT & DQ_RX_PIN)
        ret = 1;
    else
        ret = 0;
    
    _delay_us(100);
    return ret;
}

static void rom_set_bit(ow_rom_t *rom, uint8_t pos, uint8_t val)
{
    if (pos >= 0 && pos <= 7) {
        if (val)
            rom->or_family |= (1 << pos);
    } else if (pos >= 8 && pos <= 15) {
        if (val)
            rom->or_serial[0] |= (1 << (pos - 8));
    } else if (pos >= 16 && pos <= 23) {
        if (val)
            rom->or_serial[1] |= (1 << (pos - 16));
    } else if (pos >= 24 && pos <= 31) {
        if (val)
            rom->or_serial[2] |= (1 << (pos - 24));
    } else if (pos >= 32 && pos <= 39) {
        if (val)
            rom->or_serial[3] |= (1 << (pos - 32));
    } else if (pos >= 40 && pos <= 47) {
        if (val)
            rom->or_serial[4] |= (1 << (pos - 40));
    } else if (pos >= 48 && pos <= 55) {
        if (val)
            rom->or_serial[5] |= (1 << (pos - 48));
    } else {
        if (val)
            rom->or_crc |= (1 << (pos - 56));
    }
}

void onewire_init(void)
{
    DQ_TX_CONFIG;
    DQ_TX_HIGH;
}

int onewire_reset(void)
{
    int ret;
    
    DQ_TX_LOW;
    _delay_us(480);
    DQ_TX_HIGH;
    _delay_us(70);
    
    if (!(DQ_RX_PORT & DQ_RX_PIN))
        ret = 1;
    else
        ret = 0;
            
    _delay_us(420);
    return ret;
}


int onewire_send(uint8_t *data, int len)
{
    int i, j;
    uint8_t tmp;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    for (j = 0; j < len; j++) {
        tmp = 0x01;
        
        for (i = 0; i < 8; i++) {
            if (data[j] & tmp) {
                DQ_TX_LOW;
                _delay_us(1);
                DQ_TX_HIGH;
                _delay_us(80);
            } else {
                DQ_TX_LOW;
                _delay_us(80);
                DQ_TX_HIGH;
                _delay_us(1);
            }
        
            _delay_us(100);
            tmp <<= 1;
        }
    }
    
    return 0;
}

int onewire_recv(uint8_t *data, int len)
{
    int i, j;
    uint8_t tmp;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    for (j = 0; j < len; j++) {
        tmp = 0x00;
        
        for (i = 0; i < 8; i++) {
            DQ_TX_LOW;
            DQ_TX_HIGH;
            _delay_us(15);
            
            if (DQ_RX_PORT & DQ_RX_PIN)
                tmp |= (1 << i);
            
            _delay_us(100);
        }
        
        _delay_us(100);
        *(data + j) = tmp;
    }
    
    return 0;
}

int onewire_read_rom(ow_rom_t *rom)
{
    uint8_t cmd;
    uint8_t buf[8];
    
    if (!onewire_reset())
        return -1;
    
    cmd = ONEWIRE_CMD_ROM_READ;
    onewire_send(&cmd, 1);
    onewire_recv(buf, 8);
    
    if (!crc8_dallas_check(buf, 7, buf[7]))
        return -1;
    
    rom->or_family = buf[0];
    memcpy(&(rom->or_serial[0]), &buf[1], 6);
    rom->or_crc = buf[7];
    return 0;
}

int onewire_search_rom(int type, ow_rom_t *roms, int num)
{
    int rom_id;
    int rom_cnt;
    uint8_t cmd;
    uint8_t r_bit;
    uint8_t r_bit_c;
    int bit_pos;
    int dis;
    int dir;
    int not;
    int oth;
    int rep_o;
    int rep_i;
    int first;
    
    if ((roms == NULL) && (num > 0))
        return -1;
    
    rom_cnt = 0;
    rep_o = 1;
    first = 1;
    
    while (rep_o) {
        if (!onewire_reset()) {
            return 0;
        }
    
        if (first) {
            rom_id = 0;
            if (rom_id < num)
                memset(&roms[rom_id], 0, sizeof(ow_rom_t));
            oth = 0;
        } else {
            rom_id++;
            
            if (rom_id < num)
                memset(&roms[rom_id], 0, sizeof(ow_rom_t));
        }
        
        first = 0;
        rep_i = 1;
        
        if (type == TYPE_SEARCH_ALL)
            cmd = ONEWIRE_CMD_ROM_SEARCH;
        else
            cmd = ONEWIRE_CMD_ALARM_SEARCH;
        
        onewire_send(&cmd, 1);
        bit_pos = 0;
        not = 0;
        
        while (rep_i) {
            r_bit = read_bit();
            r_bit_c = read_bit();
            dis = 0;
            
            if (r_bit && !r_bit_c) {
                if (rom_id < num)
                    rom_set_bit(&roms[rom_id], bit_pos, 1);
            } else if (!r_bit && r_bit_c) {
                if (rom_id < num)
                    rom_set_bit(&roms[rom_id], bit_pos, 0);
            } else if (!r_bit && !r_bit_c) { 
                if (!oth) {
                    if (rom_id < num)
                        rom_set_bit(&roms[rom_id], bit_pos, 1);
                    dir = 1;
                    oth = 1;
                } else {
                    if (rom_id < num)
                        rom_set_bit(&roms[rom_id], bit_pos, 0);
                    dir = 0;
                    oth = 0;
                }
                
                rep_o = 1;
                dis = 1;
            } else {
                not = 1;
                rep_o = 1;
                dir = 0;
            }
            
            if (!not) {
                if (!dis) {
                    write_bit(r_bit);
                } else {
                    if (dir)
                        write_bit(1);
                    else
                        write_bit(0);
                }
                
                if (bit_pos == 63) {
                    rep_i = 0;
                    
                    if (!oth)
                        rep_o = 0;
                    
                    rom_cnt++;
                } else
                    bit_pos++;
            } else {
                not = 0;
                rep_i = 0;
            }
        }
    }
    
    return rom_cnt;
}

int onewire_search_family(int type, uint8_t family, ow_rom_t *roms, int num)
{
    int rom_id;
    int rom_cnt;
    uint8_t cmd;
    uint8_t r_bit;
    uint8_t r_bit_c;
    int bit_pos;
    int dis;
    int dir;
    int not;
    int oth;
    int rep_o;
    int rep_i;
    int first;
    int i;
    
    if ((roms == NULL) && (num > 0))
        return -1;
    
    rom_cnt = 0;
    rep_o = 1;
    first = 1;
    
    while (rep_o) {
        if (!onewire_reset()) {
            return 0;
        }
    
        if (first) {
            rom_id = 0;
            if (rom_id < num)
                memset(&roms[rom_id], 0, sizeof(ow_rom_t));
            oth = 0;
        } else {
            rom_id++;
            
            if (rom_id < num)
                memset(&roms[rom_id], 0, sizeof(ow_rom_t));
        }
        
        first = 0;
        rep_i = 1;
        
        if (type == TYPE_SEARCH_ALL)
            cmd = ONEWIRE_CMD_ROM_SEARCH;
        else
            cmd = ONEWIRE_CMD_ALARM_SEARCH;
        
        onewire_send(&cmd, 1);
        bit_pos = 0;
        not = 0;
        
        for (i = 0; i < 8; i++) {
            r_bit = read_bit();
            r_bit_c = read_bit();
            
            if (r_bit && !r_bit_c && _ISSET(family, i)) {
                if (rom_id < num)
                    rom_set_bit(&roms[rom_id], bit_pos, 1);
                
                write_bit(r_bit);
            } else if (!r_bit && r_bit_c && _ISCLR(family, i)) {
                if (rom_id < num)
                    rom_set_bit(&roms[rom_id], bit_pos, 0);
                
                write_bit(r_bit);
            } else if (!r_bit && !r_bit_c) { 
                if (_ISSET(family, i)) {
                    if (rom_id < num)
                        rom_set_bit(&roms[rom_id], bit_pos, 1);
                    
                    write_bit(1);
                } else {
                    if (rom_id < num)
                        rom_set_bit(&roms[rom_id], bit_pos, 0);
                    
                    write_bit(0);
                }
            } else {
                rep_o = 0;
                rep_i = 0;
            }
            
            bit_pos++;
        }
        
        while (rep_i) {
            r_bit = read_bit();
            r_bit_c = read_bit();
            dis = 0;
            
            if (r_bit && !r_bit_c) {
                if (rom_id < num)
                    rom_set_bit(&roms[rom_id], bit_pos, 1);
            } else if (!r_bit && r_bit_c) {
                if (rom_id < num)
                    rom_set_bit(&roms[rom_id], bit_pos, 0);
            } else if (!r_bit && !r_bit_c) { 
                if (!oth) {
                    if (rom_id < num)
                        rom_set_bit(&roms[rom_id], bit_pos, 1);
                    dir = 1;
                    oth = 1;
                } else {
                    if (rom_id < num)
                        rom_set_bit(&roms[rom_id], bit_pos, 0);
                    dir = 0;
                    oth = 0;
                }
                
                rep_o = 1;
                dis = 1;
            } else {
                not = 1;
                rep_o = 1;
                dir = 0;
            }
            
            if (!not) {
                if (!dis) {
                    write_bit(r_bit);
                } else {
                    if (dir)
                        write_bit(1);
                    else
                        write_bit(0);
                }
                
                if (bit_pos == 63) {
                    rep_i = 0;
                    
                    if (!oth)
                        rep_o = 0;
                    
                    rom_cnt++;
                } else
                    bit_pos++;
            } else {
                not = 0;
                rep_i = 0;
            }
        }
    }
    
    return rom_cnt;
}

int onewire_match_rom(ow_rom_t *rom)
{
    uint8_t cmd[9];
    
    if (!rom)
        return -1;
    
    if (!onewire_reset())
        return -1;
    
    cmd[0] = ONEWIRE_CMD_ROM_MATCH;
    cmd[1] = rom->or_family;
    memcpy(&cmd[2], rom->or_serial, 6);
    cmd[8] = rom->or_crc;
    
    onewire_send(cmd, 9);
    return 0;
}

int onewire_skip_rom(void)
{
    uint8_t cmd;
    
    if (!onewire_reset())
        return -1;
    
    cmd = ONEWIRE_CMD_ROM_SKIP;
    onewire_send(&cmd, 1);
    return 0;
}

int onewire_get_family(ow_rom_t *rom, uint8_t *family)
{
    if (!rom)
        return -1;
    
    if (!family)
        return -1;
    
    (*family) = rom->or_family;
    return 0;
}

int onewire_get_serial(ow_rom_t *rom, uint8_t *buf)
{
    int i, j;
    
    if (!rom)
        return -1;
    
    if (!buf)
        return -1;
    
    for (i = 0, j = 5; i < 6; i++, j--)
        buf[i] = rom->or_serial[j];
    
    return 0;
}

int onewire_get_crc(ow_rom_t *rom, uint8_t *crc)
{
    if (!rom)
        return -1;
    
    if (!crc)
        return -1;
    
    (*crc) = rom->or_crc;
    return 0;
}

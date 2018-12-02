/**
 *
 * File Name: sw-onewire.c
 * Title    : Software based (bitbang) 1-Wire library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-28
 * Modified : 2018-12-02
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

#include "sw-onewire.h"
#include "../lib/crc8_dallas.h"

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

static void owid_set_bit(ow_id_t *owid, uint8_t pos, uint8_t val)
{
    if (pos >= 0 && pos <= 7) {
        if (val)
            owid->oi_family |= (1 << pos);
    } else if (pos >= 8 && pos <= 15) {
        if (val)
            owid->oi_serial[0] |= (1 << (pos - 8));
    } else if (pos >= 16 && pos <= 23) {
        if (val)
            owid->oi_serial[1] |= (1 << (pos - 16));
    } else if (pos >= 24 && pos <= 31) {
        if (val)
            owid->oi_serial[2] |= (1 << (pos - 24));
    } else if (pos >= 32 && pos <= 39) {
        if (val)
            owid->oi_serial[3] |= (1 << (pos - 32));
    } else if (pos >= 40 && pos <= 47) {
        if (val)
            owid->oi_serial[4] |= (1 << (pos - 40));
    } else if (pos >= 48 && pos <= 55) {
        if (val)
            owid->oi_serial[5] |= (1 << (pos - 48));
    } else {
        if (val)
            owid->oi_crc |= (1 << (pos - 56));
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

int onewire_read_rom(ow_id_t *owid)
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
    
    owid->oi_family = buf[0];
    memcpy(&(owid->oi_serial[0]), &buf[1], 6);
    owid->oi_crc = buf[7];
    return 0;
}

int onewire_search_rom(ow_id_t **owids, int len)
{
    int owids_cnt;
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
    ow_id_t *p_owid, *p_owid_n;
    
    rep_o = 1;
    first = 1;
    
    while (rep_o) {
        if (!onewire_reset()) {
            return -1;
        }
    
        if (first) {
            owids_cnt = 0;
            
            p_owid = (ow_id_t *) malloc(sizeof(ow_id_t));
            
            if (!p_owid) {
                return -1;
            }
            
            memset(p_owid, 0, sizeof(ow_id_t));
            owids[owids_cnt] = p_owid;
            owids_cnt++;
            oth = 0;
        } else
            p_owid = p_owid_n;
        
        first = 0;
        rep_i = 1;
        
        cmd = ONEWIRE_CMD_ROM_SEARCH;
        onewire_send(&cmd, 1);
        bit_pos = 0;
        not = 0;
        
        while (rep_i) {
            r_bit = read_bit();
            r_bit_c = read_bit();
            dis = 0;
            
            if (r_bit && !r_bit_c) {
                owid_set_bit(p_owid, bit_pos, 1);
            } else if (!r_bit && r_bit_c) {
                owid_set_bit(p_owid, bit_pos, 0);
            } else if (!r_bit && !r_bit_c) { 
                if (!oth) {
                    owid_set_bit(p_owid, bit_pos, 1);
                    dir = 1;
                    p_owid_n = (ow_id_t *) malloc(sizeof(ow_id_t));
                    
                    if (!p_owid_n) {
                        return -1;
                    }
                    
                    memset(p_owid_n, 0, sizeof(ow_id_t));
                    owids[owids_cnt] = p_owid_n;
                    owids_cnt++;
                    oth = 1;
                } else {
                    oth = 0;
                    owid_set_bit(p_owid, bit_pos, 0);
                    dir = 0;
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
                } else
                    bit_pos++;
            } else {
                not = 0;
                rep_i = 0;
            }
        }
    }
    
    return owids_cnt;
}

int onewire_get_family(ow_id_t *owid, uint8_t *family)
{
    if (!owid)
        return -1;
    
    if (!family)
        return -1;
    
    (*family) = owid->oi_family;
    return 0;
}

int onewire_get_serial(ow_id_t *owid, uint8_t *buf)
{
    if (!owid)
        return -1;
    
    if (!buf)
        return -1;
    
    memcpy(buf, owid->oi_serial, 6);
    return 0;
}

int onewire_get_crc(ow_id_t *owid, uint8_t *crc)
{
    if (!owid)
        return -1;
    
    if (!crc)
        return -1;
    
    (*crc) = owid->oi_crc;
    return 0;
}

/**
 *
 * File Name: ds2430a.c
 * Title    : Dallas/Maxim DS2430A library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-12-01
 * Modified : 2019-01-13
 * Revised  : 
 * Version  : 0.2.1.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */
#include <util/delay.h>

#include "../lib/crc8_dallas.h"
#include "ds2430a.h"

/* Commands */
#define CMD_SCRATCHPAD_READ     0xAA
#define CMD_SCRATCHPAD_WRITE    0x0F
#define CMD_SCRATCHPAD_COPY     0x55
#define CMD_MEMORY_READ         0xF0
#define CMD_APPREG_WRITE        0x99
#define CMD_APPREG_READ         0xC3
#define CMD_APPREG_COPY_LOCK    0x5A
#define CMD_STATUSREG_READ      0x66

#define VAL_KEY_COPY            0xA5
#define VAL_KEY_STATUS          0x00

/* Family code */
#define FAMILY_DS2430A          0x14

#define MEMORY_SIZE             32
#define APPREG_SIZE             8

static void write_scratchpad(uint8_t addr, uint8_t *buf, int len)
{
    uint8_t cmd[2];
    
    if (!buf)
        return;
    
    if (len < 1)
        return;

    cmd[0] = CMD_SCRATCHPAD_WRITE;
    cmd[1] = addr;
    
    onewire_send(cmd, 2);
    onewire_send(buf, len);
    onewire_reset();
}

static void read_scratchpad(uint8_t addr, uint8_t *buf, int len)
{
    uint8_t cmd[2];
     
    if (!buf)
        return;
    
    if (len < 1)
        return;
    
    cmd[0] = CMD_SCRATCHPAD_READ;
    cmd[1] = addr;
    onewire_send(cmd, 2);
    onewire_recv(buf, len);
    onewire_reset();
}

static void copy_scratchpad(void)
{
    uint8_t cmd[2];
    
    cmd[0] = CMD_SCRATCHPAD_COPY;
    cmd[1] = VAL_KEY_COPY;
    onewire_send(cmd, 2);
    _delay_ms(10);
    onewire_reset();
}

static void read_memory(uint8_t addr, uint8_t *buf, int len)
{
    uint8_t cmd[2];
    
    if (len < 1)
        return;
    
    if (len == MEMORY_SIZE) {
        cmd[0] = CMD_MEMORY_READ;
        onewire_send(cmd, 1);
    } else {
        cmd[0] = CMD_MEMORY_READ;
        cmd[1] = addr;
        onewire_send(cmd, 2);
    }
    
    onewire_recv(buf, len);
    onewire_reset();
}

static void read_status(uint8_t *status)
{
    uint8_t cmd[2];
    uint8_t tmp;
    
    cmd[0] = CMD_STATUSREG_READ;
    cmd[1] = VAL_KEY_STATUS;
    onewire_send(cmd, 2);
    onewire_recv(&tmp, 1);
    (*status) = tmp;
    onewire_reset();
}

static void write_appreg(uint8_t addr, uint8_t *buf, int len)
{
    uint8_t cmd[2];
    
    if (!buf)
        return;
    
    if (len < 1)
        return;

    cmd[0] = CMD_APPREG_WRITE;
    cmd[1] = addr;
    
    onewire_send(cmd, 2);
    onewire_send(buf, len);
    onewire_reset();
}

static void read_appreg(uint8_t addr, uint8_t *buf, int len)
{
    uint8_t cmd[2];
     
    if (!buf)
        return;
    
    if (len < 1)
        return;
    
    cmd[0] = CMD_APPREG_READ;
    cmd[1] = addr;
    onewire_send(cmd, 2);
    onewire_recv(buf, len);
    onewire_reset();
}

static void copy_lock_appreg(void)
{
    uint8_t cmd[2];
    
    cmd[0] = CMD_APPREG_COPY_LOCK;
    cmd[1] = VAL_KEY_COPY;
    onewire_send(cmd, 2);
    onewire_reset();
}

void ds2430a_init(void)
{
    onewire_init();
}

int ds2430a_read_rom(ow_rom_t *rom)
{
    uint8_t family;
    
    if (!rom)
        return -1;
    
    if (onewire_read_rom(rom) == -1)
        return 0;
    
    onewire_get_family(rom, &family);
    
    if (family == FAMILY_DS2430A)
        return 1;
    
    return 0;
}

int ds2430a_search_rom(ow_rom_t *roms, int num)
{
    return onewire_search_family(TYPE_SEARCH_ALL, FAMILY_DS2430A, roms, num);
}

int ds2430a_write_memory(ow_rom_t *rom, uint8_t addr, uint8_t *buf, int len)
{
    if (addr > (MEMORY_SIZE - 1))
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (len > (MEMORY_SIZE - addr))
        return -1;
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    write_scratchpad(addr, buf, len);
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    copy_scratchpad();
    return 0;
}

int ds2430a_read_memory(ow_rom_t *rom, uint8_t addr, uint8_t *buf, int len)
{
    if (addr > (MEMORY_SIZE - 1))
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (len > (MEMORY_SIZE - addr))
        return -1;
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    read_memory(addr, buf, len);
    return 0;
}

int ds2430a_write_app_reg(ow_rom_t *rom, uint8_t addr, uint8_t *buf, int len)
{
    if (addr > (APPREG_SIZE - 1))
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (len > (APPREG_SIZE - addr))
        return -1;
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    write_appreg(addr, buf, len);
    return 0;
}

int ds2430a_read_app_reg(ow_rom_t *rom, uint8_t addr, uint8_t *buf, int len)
{
    if (addr > (APPREG_SIZE - 1))
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1)
        return -1;
    
    if (len > (APPREG_SIZE - addr))
        return -1;
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    read_appreg(addr, buf, len);
    return 0;
}

int ds2430a_lock_app_reg(ow_rom_t *rom)
{
    uint8_t status;
    
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
    
    read_status(&status);
    
    if (status != 0xFF)
        return -1;
    
    copy_lock_appreg();
    _delay_ms(10);
    return 0;
}

int ds2430a_read_status(ow_rom_t *rom, uint8_t *status)
{
    if (rom) {
        if (onewire_match_rom(rom) == -1)
            return -1;
    } else
        if (onewire_skip_rom() == -1)
            return -1;
        
    if (!status)
        return -1;
    
    read_status(status);
    return 0;
}

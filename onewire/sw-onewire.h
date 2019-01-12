/**
 *
 * File Name: sw-onewire.h
 * Title    : Software based (bitbang) 1-Wire library header
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

#ifndef LIBAVR_ONEWIRE_SW_ONEWIRE_H
#define LIBAVR_ONEWIRE_SW_ONEWIRE_H

#include <stdint.h>
#include <avr/io.h>

#define DQ_TX_CONFIG                (DDRH = (1 << PH0))
#define DQ_TX_HIGH                  (PORTH |= (1 << PH0))
#define DQ_TX_LOW                   (PORTH &= ~(1 << PH0))
#define DQ_RX_PORT                  PINH
#define DQ_RX_PIN                   (1 << PINH1)

#define TYPE_SEARCH_ALL             0
#define TYPE_SEARCH_ALARM           1

typedef struct ow_rom {
    uint8_t or_family;
    uint8_t or_serial[6];
    uint8_t or_crc;
} ow_rom_t;

extern void onewire_init(void);
extern int onewire_reset(void);
extern int onewire_send(uint8_t *data, int len);
extern int onewire_recv(uint8_t *data, int len);
extern int onewire_read_rom(ow_rom_t *rom);
extern int onewire_search_rom(int type, ow_rom_t *roms, int num);
extern int onewire_search_family(int type, uint8_t family, ow_rom_t *roms, int num);
extern int onewire_match_rom(ow_rom_t *rom);
extern int onewire_skip_rom(void);
extern int onewire_get_family(ow_rom_t *rom, uint8_t *family);
extern int onewire_get_serial(ow_rom_t *rom, uint8_t *buf);
extern int onewire_get_crc(ow_rom_t *rom, uint8_t *crc);

#endif

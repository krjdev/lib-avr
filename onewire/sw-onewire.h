/**
 *
 * File Name: sw-onewire.h
 * Title    : Software based (bitbang) 1-Wire library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-28
 * Modified : 2018-12-01
 * Revised  : 
 * Version  : 0.1.2.0
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

#define ONEWIRE_CMD_ROM_SEARCH      0xF0
#define ONEWIRE_CMD_ROM_READ        0x33
#define ONEWIRE_CMD_ROM_MATCH       0x55
#define ONEWIRE_CMD_ROM_SKIP        0xCC
#define ONEWIRE_CMD_ALARM_SEARCH    0xEC

typedef struct onewire_id {
    uint8_t oi_family;
    uint8_t oi_serial[6];
    uint8_t oi_crc;
} onewire_id_t;

extern void onewire_init(void);
extern int onewire_reset(void);
extern int onewire_send(uint8_t *data, int len);
extern int onewire_recv(uint8_t *data, int len);
extern onewire_id_t *onewire_read_rom(void);
extern int onewire_get_family(onewire_id_t *oid, uint8_t *family);
extern int onewire_get_serial(onewire_id_t *oid, uint8_t *buf);

#endif

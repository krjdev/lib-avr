/**
 *
 * File Name: enc28j60.h
 * Title    : SPI device Microchip ENC28J60 Ethernet controller library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-22
 * Modified : 2019-02-06
 * Revised  : 
 * Version  : 0.4.1.1
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_SPI_ENC28J60_H
#define LIBAVR_SPI_ENC28J60_H

#include <stdint.h>
#include <avr/io.h>

#include "../net/ethernet.h"
#include "../net/nic.h"

/* ENC28J60 Chip select */
#define ENC28J60_CSCONFIG   (DDRJ |= (1 << PJ0))
#define ENC28J60_ENABLE     (PORTJ &= ~(1 << PJ0))
#define ENC28J60_DISABLE    (PORTJ |= (1 << PJ0))

/* Duplex Mode */
#define MODE_FDPX           0
#define MODE_HDPX           1

/* Error codes */
#define ERR_NOERR           0
#define ERR_NOMEM           1
#define ERR_INVAL           2
#define ERR_TIMEO           3
#define ERR_INTER           4
#define ERR_ETLIB           5
#define ERR_FRMIN           6
#define ERR_TXABT           7
#define ERR_FRMTB           8
#define ERR_TXERR           9

struct enc28j60_regs {
    int mode;
    uint8_t eie;
    uint8_t eir;
    uint8_t estat;
    uint8_t econ1;
    uint8_t econ2;
    uint8_t erxstl;
    uint8_t erxsth;
    uint8_t erxrdptl;
    uint8_t erxrdpth;
    uint8_t erxndl;
    uint8_t erxndh;
    uint8_t etxstl;
    uint8_t etxsth;
    uint8_t erxfcon;
    uint8_t epktcnt;
    uint8_t macon1;
    uint8_t macon3;
    uint8_t macon4;
    uint8_t mamxfll;
    uint8_t mamxflh;
    uint8_t mabbipg;
    uint8_t maipgl;
    uint8_t maipgh;
    uint16_t phcon1;
};

extern int enc28j60_init(int mode, mac_addr_t *addr);
extern int enc28j60_set_mac(mac_addr_t *addr);
extern int enc28j60_get_mac(mac_addr_t *addr);
extern int enc28j60_send(eth_frame_t *frame);
extern int enc28j60_recv(eth_frame_t *frame);
extern int enc28j60_is_link_up(void);
extern int enc28j60_get_free_rx_space(void);
extern int enc28j60_get_last_error(void);
extern char *enc28j60_get_ver(void);
extern char *enc28j60_get_rev(void);
extern nic_stats_t enc28j60_get_stats(void);
extern struct enc28j60_regs enc28j60_dump_regs(void);

#endif

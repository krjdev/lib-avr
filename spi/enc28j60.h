/**
 *
 * File Name: enc28j60.h
 * Title    : SPI device Microchip ENC28J60 Ethernet controller library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-22
 * Modified : 2019-02-04
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

#ifndef LIBAVR_SPI_ENC28J60_H
#define LIBAVR_SPI_ENC28J60_H

#include <stdint.h>
#include <avr/io.h>

#include "../net/ethernet.h"

/* ENC28J60 Chip select */
#define ENC28J60_CSCONFIG   (DDRJ |= (1 << PJ0))
#define ENC28J60_ENABLE     (PORTJ &= ~(1 << PJ0))
#define ENC28J60_DISABLE    (PORTJ |= (1 << PJ0))

/* Duplex Mode */
#define MODE_FDPX           0
#define MODE_HDPX           1

/* Error codes */
#define ERROR_NO            0
#define ERROR_NOMEM         1
#define ERROR_INVAL         2
#define ERROR_TIMEOUT       3
#define ERROR_INTERNAL      4
#define ERROR_RX_NOFRM      5
#define ERROR_RX_FRMERR     6
#define ERROR_RX_FRMTOBIG   7
#define ERROR_TX_FRMTOBIG   8
#define ERROR_TX_FRMERR     9
#define ERROR_TX_ABORT      10
#define ERROR_ENC28J60      11
#define ERROR_ETHLIB        12

extern int enc28j60_init(int mode, mac_addr_t *addr);
extern int enc28j60_send(eth_frame_t *frame);
extern int enc28j60_recv(eth_frame_t *frame);
extern int enc28j60_set_mac(mac_addr_t *addr);
extern int enc28j60_get_mac(mac_addr_t *addr);
extern int enc28j60_is_link_up(void);
extern uint32_t enc28j60_get_frame_count_rx(void);
extern uint32_t enc28j60_get_frame_count_tx(void);
extern uint32_t enc28j60_get_frame_count_err(void);
extern int enc28j60_get_free_rx_space(void);
extern char *enc28j60_get_version(void);
extern char *enc28j60_get_chip_revision(void);
extern int enc28j60_get_last_error(void);

#endif

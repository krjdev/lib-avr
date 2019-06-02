/**
 *
 * File Name: nic.h
 * Title    : Generic NIC wrapper library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-01-08
 * Modified : 2019-06-02
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

#ifndef LIBAVR_NET_NIC_H
#define LIBAVR_NET_NIC_H

#include <stdint.h>

#include "ethernet.h"

/* Ethernet devices */
#define NIC_DEVICE_ENC28J60

/* Error codes */
#define NIC_ERROR_SUCCESS       0
#define NIC_ERROR_INVAL         1
#define NIC_ERROR_NOMEM         2
#define NIC_ERROR_ETHLIB        3
#define NIC_ERROR_DRIVER        4
#define NIC_ERROR_TX_TOBIG      5
#define NIC_ERROR_RX_TOBIG      6
#define NIC_ERROR_RX_CRC        7

typedef struct nic_stats {
    uint32_t tx_frm;
    uint32_t rx_frm;
    uint32_t tx_byt;
    uint32_t rx_byt;
    uint16_t tx_err;
    uint16_t rx_err;
} nic_stats_t;

extern int nic_init(mac_addr_t *addr);
extern int nic_is_link_up(void);
extern int nic_send(eth_frame_t *frame);
extern int nic_recv(eth_frame_t *frame);
extern char *nic_get_driver_name(void);
extern char *nic_get_driver_vers(void);
extern nic_stats_t nic_get_stats(void);
extern int nic_get_last_error(void);

#endif

/**
 *
 * File Name: nic.h
 * Title    : Generic NIC header
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-01-08
 * Modified : 
 * Revised  : 
 * Version  : 0.0.1.0
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

typedef struct nic_stats {
    uint32_t tx_frm;
    uint32_t rx_frm;
    uint32_t tx_byt;
    uint32_t rx_byt;
    uint16_t tx_err;
    uint16_t rx_err;
} nic_stats_t;

#endif

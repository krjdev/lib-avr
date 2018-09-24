/**
 *
 * File Name: ethernet.h
 * Title    : Ethernet (IEEE 802.3) definitions and helper header
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
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

#ifndef LIBAVR_NET_ETHERNET_H
#define LIBAVR_NET_ETHERNET_H

#include <stdint.h>

typedef struct mac_addr {
    uint8_t ma_byte0;
    uint8_t ma_byte1;
    uint8_t ma_byte2;
    uint8_t ma_byte3;
    uint8_t ma_byte4;
    uint8_t ma_byte5;
} mac_addr_t;

typedef struct eth_frame {
    mac_addr_t ef_dst;
    mac_addr_t ef_src;
    uint16_t ef_type;
    uint8_t *ef_payload_buf;
    int ef_payload_len;
} eth_frame_t;

extern mac_addr_t *ethernet_str_to_mac(const char *str);
extern char *ethernet_mac_to_str(mac_addr_t *mac);
extern int ethernet_frame_set_dst(eth_frame_t *frame, const char *addr);
extern int ethernet_frame_get_dst(eth_frame_t *frame, char **str);
extern int ethernet_frame_set_src(eth_frame_t *frame, const char *addr);
extern int ethernet_frame_get_src(eth_frame_t *frame, char **str);
extern int ethernet_frame_set_type(eth_frame_t *frame, uint16_t type);
extern int ethernet_frame_get_type(eth_frame_t *frame, uint16_t *type);
extern int ethernet_frame_set_payload(eth_frame_t *frame, uint8_t *buffer, int length);
extern int ethernet_frame_get_payload(eth_frame_t *frame, uint8_t **buffer, int *length);
extern int ethernet_buffer_to_frame(uint8_t *buffer, int length, eth_frame_t **frame);
extern int ethernet_frame_to_buffer(eth_frame_t *frame, uint8_t **buffer, int *length);

#endif

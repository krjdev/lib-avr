/**
 *
 * File Name: ethernet.h
 * Title    : Ethernet (IEEE 802.3) library
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
 * Modified : 2019-06-01
 * Revised  : 
 * Version  : 0.5.0.0
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

#define ETHERNET_MAX_FRAME_SIZE     1518

#define ETHERNET_TYPE_IPV4          0x0800
#define ETHERNET_TYPE_IPV6          0x86DD
#define ETHERNET_TYPE_ARP           0x0806

#define ETHERNET_ERROR_SUCCESS      0
#define ETHERNET_ERROR_INVAL        1
#define ETHERNET_ERROR_NOMEM        2
#define ETHERNET_ERROR_INVFRAME     3
#define ETHERNET_ERROR_CRC          4
#define ETHERNET_ERROR_INTERNAL     5

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

extern void ethernet_crc_enable(void);
extern void ethernet_crc_disable(void);
extern int ethernet_addr_aton(const char *str, mac_addr_t *mac);
extern int ethernet_addr_ntoa(mac_addr_t *mac, char *str);
extern int ethernet_addr_broadcast(mac_addr_t *mac);
extern int ethernet_addr_cpy(mac_addr_t *dst, mac_addr_t *src);
extern int ethernet_addr_equal(mac_addr_t *mac1, mac_addr_t *mac2);
extern int ethernet_addr_is_broadcast(mac_addr_t *mac);
extern int ethernet_frame_set_dst(eth_frame_t *frame, mac_addr_t *mac);
extern int ethernet_frame_set_src(eth_frame_t *frame, mac_addr_t *mac);
extern int ethernet_frame_set_type(eth_frame_t *frame, uint16_t type);
extern int ethernet_frame_set_payload(eth_frame_t *frame, uint8_t *buf, int len);
extern int ethernet_frame_get_dst(eth_frame_t *frame, mac_addr_t *mac);
extern int ethernet_frame_get_src(eth_frame_t *frame, mac_addr_t *mac);
extern int ethernet_frame_get_type(eth_frame_t *frame, uint16_t *type);
extern int ethernet_frame_get_payload_len(eth_frame_t *frame);
extern int ethernet_frame_get_payload(eth_frame_t *frame, uint8_t **buf);
extern int ethernet_frame_get_len(eth_frame_t *frame);
extern int ethernet_frame_payload_free(eth_frame_t *frame);
extern int ethernet_buf_to_frm(uint8_t *buf, int len, eth_frame_t *frame);
extern int ethernet_frm_to_buf(eth_frame_t *frame, uint8_t *buf);
extern int ethernet_get_last_error(void);

#endif

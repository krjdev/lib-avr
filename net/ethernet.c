/**
 *
 * File Name: ethernet.c
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

#define HI16(val)       ((uint8_t) (((val) & 0xFF00) >> 8))
#define LO16(val)       ((uint8_t) ((val) & 0x00FF))

#define HDR_LEN         14

#include <stdlib.h>
#include <string.h>

#include "../lib/crc32_ethernet.h"
#include "../lib/endian.h"
#include "../lib/hexconv.h"
#include "ethernet.h"

static int error = ETHERNET_ERROR_SUCCESS;
static int crc_enable = 0;

void ethernet_crc_enable(void)
{
    crc_enable = 1;
}

void ethernet_crc_disable(void)
{
    crc_enable = 0;
}

int ethernet_addr_aton(const char *str, mac_addr_t *mac)
{
    int i = 0;
    char tmp[3];
    
    if (!str) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (strlen(str) != 17) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    mac->ma_byte0 = hex_to_uint8(tmp);
    i += 3;
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    mac->ma_byte1 = hex_to_uint8(tmp);
    i += 3;
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    mac->ma_byte2 = hex_to_uint8(tmp);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    mac->ma_byte3 = hex_to_uint8(tmp);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    mac->ma_byte4 = hex_to_uint8(tmp);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    mac->ma_byte5 = hex_to_uint8(tmp);
    return 0;
}

int ethernet_addr_ntoa(mac_addr_t *mac, char *str)
{
    char *hex;
    
    if (!mac) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!str) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    str[0] = '\0';
    hex = uint8_to_hex(mac->ma_byte0);
    strcat(str, hex);
    strcat(str, ":");
    free(hex);
    hex = uint8_to_hex(mac->ma_byte1);
    strcat(str, hex);
    strcat(str, ":");
    free(hex);
    hex = uint8_to_hex(mac->ma_byte2);
    strcat(str, hex);
    strcat(str, ":");
    free(hex);
    hex = uint8_to_hex(mac->ma_byte3);
    strcat(str, hex);
    strcat(str, ":");
    free(hex);
    hex = uint8_to_hex(mac->ma_byte4);
    strcat(str, hex);
    strcat(str, ":");
    free(hex);
    hex = uint8_to_hex(mac->ma_byte5);
    strcat(str, hex);
    free(hex);
    return 0;
}

int ethernet_addr_broadcast(mac_addr_t *mac)
{
    if (!mac) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    mac->ma_byte0 = 0xFF;
    mac->ma_byte1 = 0xFF;
    mac->ma_byte2 = 0xFF;
    mac->ma_byte3 = 0xFF;
    mac->ma_byte4 = 0xFF;
    mac->ma_byte5 = 0xFF;
    return 0;
}

int ethernet_addr_cpy(mac_addr_t *dst, mac_addr_t *src)
{
    if (!dst) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!src) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    dst->ma_byte0 = src->ma_byte0;
    dst->ma_byte1 = src->ma_byte1;
    dst->ma_byte2 = src->ma_byte2;
    dst->ma_byte3 = src->ma_byte3;
    dst->ma_byte4 = src->ma_byte4;
    dst->ma_byte5 = src->ma_byte5;
    return 0;
}

int ethernet_addr_equal(mac_addr_t *mac1, mac_addr_t *mac2)
{
    if (!mac1) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!mac2) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if ((mac1->ma_byte0 == mac2->ma_byte0) && 
        (mac1->ma_byte1 == mac2->ma_byte1) && 
        (mac1->ma_byte2 == mac2->ma_byte2) && 
        (mac1->ma_byte3 == mac2->ma_byte3) && 
        (mac1->ma_byte4 == mac2->ma_byte4) && 
        (mac1->ma_byte5 == mac2->ma_byte5))
        return 1;
    
    return 0;
}

int ethernet_addr_is_broadcast(mac_addr_t *mac)
{
    if (!mac) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if ((mac->ma_byte0 == 0xFF) && 
        (mac->ma_byte1 == 0xFF) && 
        (mac->ma_byte2 == 0xFF) && 
        (mac->ma_byte3 == 0xFF) && 
        (mac->ma_byte4 == 0xFF) && 
        (mac->ma_byte5 == 0xFF))
        return 1;
    
    return 0;
}

int ethernet_frame_set_dst(eth_frame_t *frame, mac_addr_t *mac)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    ethernet_addr_cpy(&frame->ef_dst, mac);
    return 0;
}

int ethernet_frame_set_src(eth_frame_t *frame, mac_addr_t *mac)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    ethernet_addr_cpy(&frame->ef_src, mac);
    return 0;
}

int ethernet_frame_set_type(eth_frame_t *frame, uint16_t type)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    frame->ef_type = type;
    return 0;
}

int ethernet_frame_set_payload(eth_frame_t *frame, uint8_t *buf, int len)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if ((len > 0 ) && (buf != NULL)) {
        frame->ef_payload_buf = buf;
        frame->ef_payload_len = len;
    } else {
        frame->ef_payload_buf = NULL;
        frame->ef_payload_len = 0;
    }
    
    return 0;
}

int ethernet_frame_get_dst(eth_frame_t *frame, mac_addr_t *mac)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    ethernet_addr_cpy(mac, &frame->ef_dst);
    return 0;
}

int ethernet_frame_get_src(eth_frame_t *frame, mac_addr_t *mac)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!mac) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }

    ethernet_addr_cpy(mac, &frame->ef_src);
    return 0;
}

int ethernet_frame_get_type(eth_frame_t *frame, uint16_t *type)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!type) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    (*type) = frame->ef_type;
    return 0;
}

int ethernet_frame_get_payload_len(eth_frame_t *frame)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    return frame->ef_payload_len;
}

int ethernet_frame_get_payload(eth_frame_t *frame, uint8_t **buf)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!buf) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    (*buf) = frame->ef_payload_buf;
    return 0;
}

int ethernet_frame_get_len(eth_frame_t *frame)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (crc_enable)
        return (HDR_LEN + frame->ef_payload_len + 4);
    else
        return (HDR_LEN + frame->ef_payload_len);
}

int ethernet_frame_payload_free(eth_frame_t *frame)
{
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (frame->ef_payload_len > 0) {
        if (frame->ef_payload_buf)
            free(frame->ef_payload_buf);
        else {
            error = ETHERNET_ERROR_INTERNAL;
            return -1;
        }
        
        frame->ef_payload_len = 0;
    }
    
    return 0;
}

int ethernet_buf_to_frm(uint8_t *buf, int len, eth_frame_t *frame)
{
    uint32_t crc;
    uint8_t *p;
    int i = 0;
    
    if (!buf) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (len < HDR_LEN) {
        error = ETHERNET_ERROR_INVFRAME;
        return -1;
    }
    
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (crc_enable) {
        crc = buf_to_uint32_le(&buf[len - 4]);
        
        if (crc32_check(buf, len - 4, crc) != 1) {
            error = ETHERNET_ERROR_CRC;
            return -1;
        }
    }
    
    /* Destination address */
    frame->ef_dst.ma_byte0 = buf[i++];
    frame->ef_dst.ma_byte1 = buf[i++];
    frame->ef_dst.ma_byte2 = buf[i++];
    frame->ef_dst.ma_byte3 = buf[i++];
    frame->ef_dst.ma_byte4 = buf[i++];
    frame->ef_dst.ma_byte5 = buf[i++];
    
    /* Source address */
    frame->ef_src.ma_byte0 = buf[i++];
    frame->ef_src.ma_byte1 = buf[i++];
    frame->ef_src.ma_byte2 = buf[i++];
    frame->ef_src.ma_byte3 = buf[i++];
    frame->ef_src.ma_byte4 = buf[i++];
    frame->ef_src.ma_byte5 = buf[i++];
    
    /* Type */
    frame->ef_type = buf_to_uint16_be(&buf[i]);
    i += 2;
    
    /* Payload/Data */
    if (len > HDR_LEN) {
        if (crc_enable)
            p = (uint8_t *) malloc(len - HDR_LEN - 4);
        else
            p = (uint8_t *) malloc(len - HDR_LEN);
        
        if (!p) {
            error = ETHERNET_ERROR_NOMEM;
            return -1;
        }
    
        frame->ef_payload_buf = p;
        
        if (crc_enable)
            frame->ef_payload_len = (len - HDR_LEN - 4);
        else
            frame->ef_payload_len = (len - HDR_LEN);
        
        memcpy(frame->ef_payload_buf, &buf[i], frame->ef_payload_len);
    }
    
    return 0;
}

int ethernet_frm_to_buf(eth_frame_t *frame, uint8_t *buf)
{
    int i = 0;
    uint32_t crc;
    
    if (!frame) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    if (!buf) {
        error = ETHERNET_ERROR_INVAL;
        return -1;
    }
    
    /* Destination address */
    buf[i++] = frame->ef_dst.ma_byte0;
    buf[i++] = frame->ef_dst.ma_byte1;
    buf[i++] = frame->ef_dst.ma_byte2;
    buf[i++] = frame->ef_dst.ma_byte3;
    buf[i++] = frame->ef_dst.ma_byte4;
    buf[i++] = frame->ef_dst.ma_byte5;
    
    /* Source address */
    buf[i++] = frame->ef_src.ma_byte0;
    buf[i++] = frame->ef_src.ma_byte1;
    buf[i++] = frame->ef_src.ma_byte2;
    buf[i++] = frame->ef_src.ma_byte3;
    buf[i++] = frame->ef_src.ma_byte4;
    buf[i++] = frame->ef_src.ma_byte5;

    /* Type */
    buf[i++] = HI16(frame->ef_type);
    buf[i++] = LO16(frame->ef_type);
    
    /* Payload/Data */
    if (frame->ef_payload_len > 0) {
        if (!frame->ef_payload_buf) {
            error = ETHERNET_ERROR_INTERNAL;
            return -1;
        }
        
        memcpy(&buf[i], frame->ef_payload_buf, frame->ef_payload_len);
    }
    
    if (crc_enable) {
        crc = crc32_calc(buf, frame->ef_payload_len +  HDR_LEN);
        uint32_to_buf_le(crc, &buf[frame->ef_payload_len +  HDR_LEN]);
    }
    
    return 0;
}

int ethernet_get_last_error(void)
{
    int ret;
    
    ret = error;
    error = ETHERNET_ERROR_SUCCESS;
    return ret;
}

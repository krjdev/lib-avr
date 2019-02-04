/**
 *
 * File Name: ethernet.c
 * Title    : Ethernet (IEEE 802.3) definitions and helper functions source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-24
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

#define HIGH16(val)   ((uint8_t) (((val) & 0xFF00) >> 8))
#define LOW16(val)    ((uint8_t) ((val) & 0x00FF))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ethernet.h"

int ethernet_addr_aton(const char *str, mac_addr_t *mac)
{
    int i = 0;
    char tmp[3];
    
    if (!str)
        return -1;
    
    if (!mac)
        return -1;
    
    if (strlen(str) != 17)
        return -1;
    
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &mac->ma_byte0);
    i += 3;
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &mac->ma_byte1);
    i += 3;
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &mac->ma_byte2);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &mac->ma_byte3);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &mac->ma_byte4);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &mac->ma_byte5);
    return 0;
}

int ethernet_addr_ntoa(mac_addr_t *mac, char *str)
{
    if (!mac)
        return -1;
    
    if (!str)
        return -1;
    
    sprintf(str, "%02X:%02X:%02X:%02X:%02X:%02X", mac->ma_byte0, mac->ma_byte1, 
            mac->ma_byte2, mac->ma_byte3, mac->ma_byte4, mac->ma_byte5);
    return 0;
}

int ethernet_addr_cpy(mac_addr_t *dst, mac_addr_t *src)
{
    if (!dst)
        return -1;
    
    if (!src)
        return -1;
    
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
    if (!mac1)
        return -1;
    
    if (!mac2)
        return -1;
    
    if ((mac1->ma_byte0 == mac2->ma_byte0) && 
        (mac1->ma_byte1 == mac2->ma_byte1) && 
        (mac1->ma_byte2 == mac2->ma_byte2) && 
        (mac1->ma_byte3 == mac2->ma_byte3) && 
        (mac1->ma_byte4 == mac2->ma_byte4) && 
        (mac1->ma_byte5 == mac2->ma_byte5))
        return 1;
    
    return 0;
}

int ethernet_frame_set_dst(eth_frame_t *frame, mac_addr_t *mac)
{
    if (!frame)
        return -1;
    
    if (!mac)
        return -1;
    
    ethernet_addr_cpy(&frame->ef_dst, mac);
    return 0;
}

int ethernet_frame_set_src(eth_frame_t *frame, mac_addr_t *mac)
{
    if (!frame)
        return -1;
    
    if (!mac)
        return -1;
    
    ethernet_addr_cpy(&frame->ef_src, mac);
    return 0;
}

int ethernet_frame_set_type(eth_frame_t *frame, uint16_t type)
{
    if (!frame)
        return -1;
    
    frame->ef_type = type;
    return 0;
}

int ethernet_frame_set_payload(eth_frame_t *frame, uint8_t *buf, int len)
{
    if (!frame)
        return -1;
    
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
    if (!frame)
        return -1;
    
    if (!mac)
        return -1;
    
    ethernet_addr_cpy(mac, &frame->ef_dst);
    return 0;
}

int ethernet_frame_get_src(eth_frame_t *frame, mac_addr_t *mac)
{
    if (!frame)
        return -1;
    
    if (!mac)
        return -1;

    ethernet_addr_cpy(mac, &frame->ef_src);
    return 0;
}

int ethernet_frame_get_type(eth_frame_t *frame, uint16_t *type)
{
    if (!frame)
        return -1;
    
    if (!type)
        return -1;
    
    (*type) = frame->ef_type;
    return 0;
}

int ethernet_frame_get_payload_len(eth_frame_t *frame)
{
    if (!frame)
        return -1;
    
    return frame->ef_payload_len;
}

int ethernet_frame_get_payload(eth_frame_t *frame, uint8_t **buf)
{
    if (!frame)
        return -1;
    
    if (!buf)
        return -1;
    
    (*buf) = frame->ef_payload_buf;
    return 0;
}

int ethernet_frame_get_len(eth_frame_t *frame)
{
    if (!frame)
        return -1;
    
    return (14 + frame->ef_payload_len);
}

int ethernet_frame_payload_free(eth_frame_t *frame)
{
    if (!frame)
        return -1;
    
    if (frame->ef_payload_len > 0) {
        if (frame->ef_payload_buf)
            free(frame->ef_payload_buf);
        else
            return -1;
        
        frame->ef_payload_len = 0;
    }
    
    return 0;
}

int ethernet_buf_to_frm(uint8_t *buf, int len, eth_frame_t *frame)
{
    uint16_t type;
    uint8_t *p;
    int i = 0;
    
    if (!buf)
        return -1;
    
    if (len < 14)
        return -1;
    
    if (!frame)
        return -1;
    
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
    type = (buf[i++] << 8);
    type |= buf[i++];
    frame->ef_type = type;
    
    /* Payload/Data */
    if (len > 14) {
        p = (uint8_t *) malloc(len - 14);
        
        if (!p)
            return -1;
    
        frame->ef_payload_buf = p;
        frame->ef_payload_len = (len - 14);
        memcpy(frame->ef_payload_buf, &buf[i], frame->ef_payload_len);
    }
    
    return 0;
}

int ethernet_frm_to_buf(eth_frame_t *frame, uint8_t *buf)
{
    int i = 0;
    int j, k;
    
    if (!frame)
        return -1;
    
    if (!buf)
        return -1;
    
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
    buf[i++] = HIGH16(frame->ef_type);
    buf[i++] = LOW16(frame->ef_type);
    
    /* Payload/Data */
    if (frame->ef_payload_len > 0) {
        if (!frame->ef_payload_buf)
            return -1;
        
        for (j = i, k = 0; j < (frame->ef_payload_len + i); j++, k++)
            buf[j] = frame->ef_payload_buf[k];
    }
    
    return 0;
}

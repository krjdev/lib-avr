/**
 *
 * File Name: ethernet.c
 * Title    : Ethernet (IEEE 802.3) definitions and helper source
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

#define HIGH16(val)   ((uint8_t) (((val) & 0xFF00) >> 8))
#define LOW16(val)    ((uint8_t) ((val) & 0x00FF))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ethernet.h"

mac_addr_t *ethernet_str_to_mac(const char *str)
{
    mac_addr_t *p;
    int i = 0;
    char tmp[3];
    
    if (!str)
        return NULL;
    
    if (strlen(str) != 17)
        return NULL;
    
    p = (mac_addr_t *) malloc(sizeof(mac_addr_t));
    
    if (!p)
        return NULL;
    
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &p->ma_byte0);
    i += 3;
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &p->ma_byte1);
    i += 3;
    memcpy(&tmp[0], &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &p->ma_byte2);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &p->ma_byte3);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &p->ma_byte4);
    i += 3;
    memcpy(tmp, &str[i], 2);
    tmp[2] = '\0';
    sscanf(tmp, "%hhx", &p->ma_byte5);
    return p;
}

char *ethernet_mac_to_str(mac_addr_t *mac)
{
    char *p;
    
    if (!mac)
        return NULL;
    
    p = (char *) malloc(18);
    
    if (!p)
        return NULL;
    
    sprintf(p, "%02X:%02X:%02X:%02X:%02X:%02X", mac->ma_byte0, mac->ma_byte1, 
            mac->ma_byte2, mac->ma_byte3, mac->ma_byte4, mac->ma_byte5);
    return p;
}

int ethernet_frame_set_dst(eth_frame_t *frame, const char *addr)
{
    mac_addr_t *p;
    
    if (!frame)
        return -1;
    
    if (!addr)
        return -1;
    
    p = ethernet_str_to_mac(addr);
    
    if (!p)
        return -1;
    
    frame->ef_dst.ma_byte0 = p->ma_byte0;
    frame->ef_dst.ma_byte1 = p->ma_byte1;
    frame->ef_dst.ma_byte2 = p->ma_byte2;
    frame->ef_dst.ma_byte3 = p->ma_byte3;
    frame->ef_dst.ma_byte4 = p->ma_byte4;
    frame->ef_dst.ma_byte5 = p->ma_byte5;
    free(p);
    return 0;
}

int ethernet_frame_get_dst(eth_frame_t *frame, char **str)
{
    char *p;
    
    if (!frame)
        return -1;
    
    if (!str)
        return -1;
    
    p = ethernet_mac_to_str(&frame->ef_dst);
    
    if (!p)
        return -1;
    
    (*str) = p;
    return 0;
}

int ethernet_frame_set_src(eth_frame_t *frame, const char *addr)
{
    mac_addr_t *p;
    
    if (!frame)
        return -1;
    
    if (!addr)
        return -1;
    
    p = ethernet_str_to_mac(addr);
    
    if (!p)
        return -1;
    
    frame->ef_src.ma_byte0 = p->ma_byte0;
    frame->ef_src.ma_byte1 = p->ma_byte1;
    frame->ef_src.ma_byte2 = p->ma_byte2;
    frame->ef_src.ma_byte3 = p->ma_byte3;
    frame->ef_src.ma_byte4 = p->ma_byte4;
    frame->ef_src.ma_byte5 = p->ma_byte5;
    free(p);
    return 0;
}

int ethernet_frame_get_src(eth_frame_t *frame, char **str)
{
    char *p;
    
    if (!frame)
        return -1;
    
    if (!str)
        return -1;
    
    p = ethernet_mac_to_str(&frame->ef_src);
    
    if (!p)
        return -1;
    
    (*str) = p;
    return 0;
}

int ethernet_frame_set_type(eth_frame_t *frame, uint16_t type)
{
    if (!frame)
        return -1;
    
    frame->ef_type = type;
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

int ethernet_frame_set_payload(eth_frame_t *frame, uint8_t *buffer, int length)
{
    if (!frame)
        return -1;
    
    if (!buffer)
        return -1;
    
    if (length < 1)
        return -1;
    
    frame->ef_payload_buf = buffer;
    frame->ef_payload_len = length;
    return 0;
}

int ethernet_frame_get_payload(eth_frame_t *frame, uint8_t **buffer, int *length)
{
    if (!frame)
        return -1;
    
    if (!buffer)
        return -1;
    
    if (!length)
        return -1;
    
    (*buffer) = frame->ef_payload_buf;
    (*length) = frame->ef_payload_len;
    return 0;
}

int ethernet_buffer_to_frame(uint8_t *buffer, int length, eth_frame_t **frame)
{
    eth_frame_t *p_f = NULL;
    uint8_t *p = NULL;
    uint16_t type;
    int i = 0;
    
    if (!buffer)
        return -1;
    
    if (!frame)
        return -1;
    
    p_f = (eth_frame_t *) malloc(sizeof(eth_frame_t));
    
    if (!p_f)
        return -1;
    
    p = (uint8_t *) malloc(length - 14);
    
    if (!p) {
        free(p_f);
        return -1;
    }
    
    /* Destination address */
    p_f->ef_dst.ma_byte0 = buffer[i++];
    p_f->ef_dst.ma_byte1 = buffer[i++];
    p_f->ef_dst.ma_byte2 = buffer[i++];
    p_f->ef_dst.ma_byte3 = buffer[i++];
    p_f->ef_dst.ma_byte4 = buffer[i++];
    p_f->ef_dst.ma_byte5 = buffer[i++];
    
    /* Source address */
    p_f->ef_src.ma_byte0 = buffer[i++];
    p_f->ef_src.ma_byte1 = buffer[i++];
    p_f->ef_src.ma_byte2 = buffer[i++];
    p_f->ef_src.ma_byte3 = buffer[i++];
    p_f->ef_src.ma_byte4 = buffer[i++];
    p_f->ef_src.ma_byte5 = buffer[i++];
    
    /* Type */
    type = (buffer[i++] << 8);
    type |= buffer[i++];
    p_f->ef_type = type;
    
    /* Payload/Data */
    p_f->ef_payload_buf = p;
    p_f->ef_payload_len = (length - 14);
    memcpy(p_f->ef_payload_buf, &buffer[i], p_f->ef_payload_len);
    
    (*frame) = p_f;
    return 0;
}

int ethernet_frame_to_buffer(eth_frame_t *frame, uint8_t **buffer, int *length)
{
    uint8_t *p;
    int len;
    int i = 0;
    int j, k;
    
    if (!frame)
        return -1;
    
    if (!buffer)
        return -1;
    
    if (!length)
        return -1;
    
    len = frame->ef_payload_len;
    len += sizeof(frame->ef_dst);
    len += sizeof(frame->ef_src);
    len += sizeof(frame->ef_type);

    p = (uint8_t *) malloc(len);
    
    if (!p)
        return -1;
    
    /* Destination address */
    p[i++] = frame->ef_dst.ma_byte0;
    p[i++] = frame->ef_dst.ma_byte1;
    p[i++] = frame->ef_dst.ma_byte2;
    p[i++] = frame->ef_dst.ma_byte3;
    p[i++] = frame->ef_dst.ma_byte4;
    p[i++] = frame->ef_dst.ma_byte5;
    
    /* Source address */
    p[i++] = frame->ef_src.ma_byte0;
    p[i++] = frame->ef_src.ma_byte1;
    p[i++] = frame->ef_src.ma_byte2;
    p[i++] = frame->ef_src.ma_byte3;
    p[i++] = frame->ef_src.ma_byte4;
    p[i++] = frame->ef_src.ma_byte5;

    /* Type */
    p[i++] = HIGH16(frame->ef_type);
    p[i++] = LOW16(frame->ef_type);
    
    /* Payload/Data */
    for (j = i, k = 0; j < (frame->ef_payload_len + i); j++, k++)
        p[j] = frame->ef_payload_buf[k];
    
    (*buffer) = p;
    (*length) = len;
    return 0;
}

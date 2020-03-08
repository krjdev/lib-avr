/**
 *
 * File Name: can.c
 * Title    : CAN Library
 * Project  : lib-avr
 * Author   : Copyright (C) 2020 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2020-03-08
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

#include <string.h>

#include "can.h"

int can_frame_set_sid(can_frame_t *frame, uint16_t sid)
{
    if (!frame)
        return -1;
    
    switch (frame->f_type) {
    case CAN_TYPE_DATA_STD:
        frame->f_d_std.sid = sid;
        break;
    case CAN_TYPE_DATA_EXT:
        frame->f_d_ext.sid = sid;
        break;
    case CAN_TYPE_REMOTE_STD:
        frame->f_r_std.sid = sid;
        break;
    case CAN_TYPE_REMOTE_EXT:
        frame->f_r_ext.sid = sid;
        break;
    default:
        return -1;
    }
    
    return 0;
}

int can_frame_set_eid(can_frame_t *frame, uint32_t eid)
{
    if (!frame)
        return -1;
    
    switch (frame->f_type) {
    case CAN_TYPE_DATA_EXT:
        frame->f_d_ext.eid = eid;
        break;
    case CAN_TYPE_REMOTE_EXT:
        frame->f_r_ext.eid = eid;
        break;
    default:
        return -1;
    }
    
    return 0;
}

int can_frame_set_data(can_frame_t *frame, uint8_t *buf, int len)
{
    if (!frame)
        return -1;
    
    if (!buf)
        return -1;
    
    if (len < 1 || len > CAN_DATA_LENMAX)
        return -1;
    
    switch (frame->f_type) {
    case CAN_TYPE_DATA_STD:
        memcpy(frame->f_d_std.data, buf, len);
        frame->f_d_std.dlen = len;
        break;
    case CAN_TYPE_DATA_EXT:
        memcpy(frame->f_d_ext.data, buf, len);
        frame->f_d_ext.dlen = len;
        break;
    default:
        return -1;
    }
    
    return 0;
}

int can_frame_set_type(can_frame_t *frame, int type)
{
    if (!frame)
        return -1;
    
    frame->f_type = type;
    return 0;
}

int can_frame_get_type(can_frame_t *frame, int *type)
{
    if (!frame)
        return -1;
    
    (*type) = frame->f_type;
    return 0;
}

int can_frame_get_sid(can_frame_t *frame, uint16_t *sid)
{
    if (!frame)
        return -1;
    
    if (!sid)
        return -1;
    
    switch (frame->f_type) {
    case CAN_TYPE_DATA_STD:
        (*sid) = frame->f_d_std.sid;
        break;
    case CAN_TYPE_DATA_EXT:
        (*sid) = frame->f_d_ext.sid;
        break;
    case CAN_TYPE_REMOTE_STD:
        (*sid) = frame->f_r_std.sid;
        break;
    case CAN_TYPE_REMOTE_EXT:
        (*sid) = frame->f_r_ext.sid;
        break;
    default:
        return -1;
    }
    
    return 0;
}

int can_frame_get_eid(can_frame_t *frame, uint32_t *eid)
{
    if (!frame)
        return -1;
    
    if (!eid)
        return -1;
    
    switch (frame->f_type) {
    case CAN_TYPE_DATA_EXT:
        (*eid) = frame->f_d_ext.eid;
        break;
    case CAN_TYPE_REMOTE_EXT:
        (*eid) = frame->f_r_ext.eid;
        break;
    default:
        return -1;
    }
    
    return 0;
}

int can_frame_get_data_len(can_frame_t *frame, int *len)
{
    if (!frame)
        return -1;
    
    if (!len)
        return -1;
    
    switch (frame->f_type) {
    case CAN_TYPE_DATA_STD:
        (*len) = frame->f_d_std.dlen;
        break;
    case CAN_TYPE_DATA_EXT:
        (*len) = frame->f_d_ext.dlen;
        break;
    default:
        return -1;
    }
    
    return 0;
}

int can_frame_get_data(can_frame_t *frame, uint8_t *buf)
{
    if (!frame)
        return -1;
    
    if (!buf)
        return -1;
    
    switch (frame->f_type) {
    case CAN_TYPE_DATA_STD:
        memcpy(buf, frame->f_d_std.data, frame->f_d_std.dlen);
        break;
    case CAN_TYPE_DATA_EXT:
        memcpy(buf, frame->f_d_ext.data, frame->f_d_ext.dlen);
        break;
    default:
        return -1;
    }
    
    return 0;
}

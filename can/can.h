/**
 *
 * File Name: can.h
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

#ifndef LIBAVR_CAN_CAN_H
#define LIBAVR_CAN_CAN_H

#include <stdint.h>

#define CAN_DATA_LENMAX         8

#define CAN_TYPE_DATA_STD       0
#define CAN_TYPE_DATA_EXT       1
#define CAN_TYPE_REMOTE_STD     2
#define CAN_TYPE_REMOTE_EXT     3

/* CAN Standard Data Frame */
struct can_frame_data_std
{
    uint16_t sid;
    uint8_t dlen;
    uint8_t data[CAN_DATA_LENMAX];
};

/* CAN Extended Data Frame */
struct can_frame_data_ext
{
    uint16_t sid;
    uint32_t eid;
    uint8_t dlen;
    uint8_t data[CAN_DATA_LENMAX];
};

/* CAN Standard Remote Frame */
struct can_frame_remote_std
{
    uint16_t sid;
};

/* CAN Extended Remote Frame */
struct can_frame_remote_ext
{
    uint16_t sid;
    uint32_t eid;
};

typedef struct can_frame
{
    int f_type;
    
    union {
        struct can_frame_data_std f_d_std;
        struct can_frame_data_ext f_d_ext;
        struct can_frame_remote_std f_r_std;
        struct can_frame_remote_ext f_r_ext;
    };
} can_frame_t;

extern int can_frame_set_sid(can_frame_t *frame, uint16_t sid);
extern int can_frame_set_eid(can_frame_t *frame, uint32_t eid);
extern int can_frame_set_data(can_frame_t *frame, uint8_t *buf, int len);
extern int can_frame_set_type(can_frame_t *frame, int type);
extern int can_frame_get_type(can_frame_t *frame, int *type);
extern int can_frame_get_sid(can_frame_t *frame, uint16_t *sid);
extern int can_frame_get_eid(can_frame_t *frame, uint32_t *eid);
extern int can_frame_get_data_len(can_frame_t *frame, int *len);
extern int can_frame_get_data(can_frame_t *frame, uint8_t *buf);

#endif

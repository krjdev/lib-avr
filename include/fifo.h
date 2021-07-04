/**
 *
 * File Name: include/fifo.h
 * Title    : FIFO buffer header
 * Project  : lib-avr
 * Author   : Copyright (C) 2019-2021 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-02-02
 * Modified : 2021-07-05
 * Revised  : 
 * Version  : 0.1.0.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Microchip AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_FIFO_H
#define LIBAVR_FIFO_H

#include <stdint.h>

typedef struct fifo {
    int f_len;
    int f_num;
    uint8_t *f_p;
    int f_idxr;
    int f_idxw;
} fifo_t;

extern fifo_t *fifo_init(int len);
extern void fifo_free(fifo_t *fifo);
extern int fifo_enqueue(fifo_t *fifo, uint8_t data);
extern int fifo_dequeue(fifo_t *fifo, uint8_t *data);
extern int fifo_get_len(fifo_t *fifo);
extern int fifo_get_num(fifo_t *fifo);

#endif

/**
 *
 * File Name: lifo.h
 * Title    : LIFO (Stack) buffer library header
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-02-04
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

#ifndef LIBAVR_LIB_LIFO_H
#define LIBAVR_LIB_LIFO_H

#include <stdint.h>

typedef struct lifo {
    int l_len;
    int l_num;
    uint8_t *l_p;
    int l_idxwr;
} lifo_t;

extern lifo_t *lifo_init(int len);
extern void lifo_free(lifo_t *lifo);
extern int lifo_push(lifo_t *lifo, uint8_t data);
extern int lifo_pop(lifo_t *lifo, uint8_t *data);
extern int lifo_get_len(lifo_t *lifo);
extern int lifo_get_num(lifo_t *lifo);

#endif

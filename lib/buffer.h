/**
 *
 * File Name: buffer.h
 * Title    : Buffer library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-08-12
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

#ifndef LIBAVR_LIB_BUFFER_H
#define LIBAVR_LIB_BUFFER_H

#include <stdint.h>

#define BUFFER_ERROR_SUCCESS    0
#define BUFFER_ERROR_EMPTY      1
#define BUFFER_ERROR_FULL       2
#define BUFFER_ERROR_INTERNAL   3
#define BUFFER_ERROR_INVAL      4
#define BUFFER_ERROR_NOMEM      5
#define BUFFER_ERROR_SPACE      6
#define BUFFER_ERROR_TOOFEW     7

typedef struct buffer {
    int b_len;
    int b_num;
    uint8_t *b_p;
    int b_idxr;
    int b_idxw;
} buffer_t;

extern buffer_t *buffer_init(int len);
extern void buffer_close(buffer_t *buf);
extern int buffer_wr(buffer_t *buf, uint8_t *data, int len);
extern int buffer_rd(buffer_t *buf, uint8_t *data, int len);
extern int buffer_get_len(buffer_t *buf);
extern int buffer_get_num(buffer_t *buf);
extern int buffer_get_free(buffer_t *buf);
extern int buffer_get_last_error(void);

#endif

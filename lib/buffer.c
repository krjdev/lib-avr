/**
 *
 * File Name: buffer.c
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

#include <stdlib.h>
#include <string.h>

#include "buffer.h"

static int error = BUFFER_ERROR_SUCCESS;

buffer_t *buffer_init(int len)
{
    buffer_t *b;
    uint8_t *p;
    
    if (len < 1) {
        error = BUFFER_ERROR_INVAL;
        return NULL;
    }
    
    p = (uint8_t *) malloc(len);
    
    if (!p) {
        error = BUFFER_ERROR_NOMEM;
        return NULL;
    }
    
    b = (buffer_t *) malloc(sizeof(buffer_t));
    
    if (!b) {
        free(p);
        error = BUFFER_ERROR_NOMEM;
        return NULL;
    }
    
    b->b_len = len;
    b->b_num = 0;
    b->b_p = p;
    b->b_idxw = 0;
    b->b_idxr = -1;
    return b;
}

void buffer_free(buffer_t *buf)
{
    if (!buf) {
        error = BUFFER_ERROR_INVAL;
        return;
    }
    
    if (buf->b_p)
        free(buf->b_p);
    
    buf->b_p = NULL;
    free(buf);
}

int buffer_wr(buffer_t *buf, uint8_t *data, int len)
{
    int i;
    
    if (!buf) {
        error = BUFFER_ERROR_INVAL;
        return -1;
    }
    
    if (!buf->b_p) {
        error = BUFFER_ERROR_INTERNAL;
        return -1;
    }
    
    if (!data) {
        error = BUFFER_ERROR_INVAL;
        return -1;
    }
    
    if (len < 1) {
        error = BUFFER_ERROR_INVAL;
        return -1;
    }
    
    if (len > (buf->b_len - buf->b_num)) {
        error = BUFFER_ERROR_SPACE;
        return -1;
    }
    
    for (i = 0; i < len; i++) {
        if (buf->b_num == buf->b_len) {
            error = BUFFER_ERROR_FULL;
            return -1;
        } else if (buf->b_num < buf->b_len) {
            buf->b_p[buf->b_idxw] = data[i];
            buf->b_num++;
        
            if (buf->b_idxw < (buf->b_len - 1))
                buf->b_idxw++;
            else
                buf->b_idxw = 0;
            
            if (buf->b_idxr == -1)
                buf->b_idxr = 0;
        } else {
            error = BUFFER_ERROR_INTERNAL;
            return -1;
        }
    }
    
    return len;
}

int buffer_rd(buffer_t *buf, uint8_t *data, int len)
{
    int i;
    
    if (!buf) {
        error = BUFFER_ERROR_INVAL;
        return -1;
    }
    
    if (!buf->b_p) {
        error = BUFFER_ERROR_INTERNAL;
        return -1;
    }
    
    if (!data) {
        error = BUFFER_ERROR_INVAL;
        return -1;
    }
    
    if (len > buf->b_num) {
        error = BUFFER_ERROR_TOOFEW;
        return -1;
    }
    
    for (i = 0; i < len; i++) {
        if (buf->b_num > 0) {
            data[i] = buf->b_p[buf->b_idxr];
            buf->b_num--;
            
            if (buf->b_idxr < (buf->b_len - 1))
                buf->b_idxr++;
            else
                buf->b_idxr = 0;
        } else {
            error = BUFFER_ERROR_EMPTY;
            return -1;
        }
    }
    
    return len;
}

int buffer_get_len(buffer_t *buf)
{
    if (!buf) {
        error = BUFFER_ERROR_INVAL;
        return -1;
    }
    
    return buf->b_len;
}

int buffer_get_num(buffer_t *buf)
{
    if (!buf) {
        error = BUFFER_ERROR_INVAL;
        return -1;
    }
    
    return buf->b_num;
}

int buffer_get_free(buffer_t *buf)
{
    if (!buf) {
        error = BUFFER_ERROR_INVAL;
        return -1;
    }
    
    return (buf->b_len - buf->b_num);
}

int buffer_get_last_error(void)
{
    int err;
    
    err = error;
    error = BUFFER_ERROR_SUCCESS;
    return err;
}

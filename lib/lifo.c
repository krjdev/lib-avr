/**
 *
 * File Name: lifo.c
 * Title    : LIFO (Stack) buffer library source
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

#include <stdlib.h>

#include "lifo.h"

lifo_t *lifo_init(int len)
{
    lifo_t *l;
    uint8_t *p;
    
    if (len < 1)
        return NULL;
    
    p = (uint8_t *) malloc(len);
    
    if (!p)
        return NULL;
    
    l = (lifo_t *) malloc(sizeof(lifo_t));
    
    if (!l) {
        free(p);
        return NULL;
    }
    
    l->l_len = len;
    l->l_p = p;
    l->l_idxwr = 0;
    return l;
}

void lifo_free(lifo_t *lifo)
{
    if (!lifo)
        return;
    
    if (lifo->l_p)
        free(lifo->l_p);
    
    lifo->l_p = NULL;
    free(lifo);
}

int lifo_push(lifo_t *lifo, uint8_t data)
{
    if (!lifo)
        return -1;
    
    if (!lifo->l_p)
        return -1;
    
    if ((lifo->l_idxwr + 1) < lifo->l_len)
        lifo->l_p[lifo->l_idxwr++] = data;
    else
        return -1;
    
    return 0;
}

int lifo_pop(lifo_t *lifo, uint8_t *data)
{
    if (!lifo)
        return -1;
    
    if (!data)
        return -1;
    
    if (!lifo->l_p)
        return -1;
    
    if ((lifo->l_idxwr + 1) > 0) {
        (*data) = lifo->l_p[lifo->l_idxwr - 1];
        lifo->l_idxwr--;
    } else
        return -1;
    
    return 0;
}

int lifo_get_len(lifo_t *lifo)
{
    if (!lifo)
        return -1;
    
    return lifo->l_len;
}

int lifo_get_num(lifo_t *lifo)
{
    if (!lifo)
        return -1;
    
    return lifo->l_idxwr;
}

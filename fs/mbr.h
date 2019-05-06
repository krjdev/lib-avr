/**
 *
 * File Name: mbr.h
 * Title    : MBR (Master Boot Record) library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-05-05
 * Modified : 2019-05-06
 * Revised  : 
 * Version  : 0.1.0.1
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_FS_MBR_H
#define LIBAVR_FS_MBR_H

#include <stdint.h>

typedef struct part {
    uint8_t boot;
//    uint8_t chs_start[3];
    uint8_t type;
//    uint8_t chs_end[3];
    uint32_t lba_start;
    uint32_t lba_size;
} part_t;

typedef struct mbr {
//    uint8_t bootc[440];
//    uint32_t sig;
//    uint16_t res;
    part_t part_tbl[4];
//    uint16_t magic;
} mbr_t;

extern mbr_t *mbr_open(uint8_t *buf, int len);
extern void mbr_close(mbr_t *mbr); 
extern int mbr_part_get_num(mbr_t *mbr);
extern int mbr_part_is_boot(mbr_t *mbr, int part);
extern uint8_t mbr_part_get_type(mbr_t *mbr, int part);
extern uint32_t mbr_part_get_lba_start(mbr_t *mbr, int part);
extern uint32_t mbr_part_get_lba_size(mbr_t *mbr, int part);

#endif

/**
 *
 * File Name: mbr.h
 * Title    : MBR (Master Boot Record) library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-05-05
 * Modified : 2019-11-09
 * Revised  : 
 * Version  : 0.2.0.0
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

#define MBR_START               0x00000000
#define MBR_LEN                 512

#define MBR_PART_TYPE_EMPTY     0x00    /* Empty partition */
#define MBR_PART_TYPE_FAT12     0x01    /* FAT12 partition */
#define MBR_PART_TYPE_FAT16S32  0x04    /* FAT16 < 32MiB partition */
#define MBR_PART_TYPE_EXTENDED  0x05    /* Extented partition */
#define MBR_PART_TYPE_FAT16G32  0x06    /* FAT16 > 32MiB partition */
#define MBR_PART_TYPE_NTFS      0x07    /* NTFS partition */
#define MBR_PART_TYPE_FAT32     0x0B    /* FAT32 partition */
#define MBR_PART_TYPE_FAT32L    0x0C    /* FAT32 partition (LBA mode) */
#define MBR_PART_TYPE_FAT16L    0x0E    /* FAT16 partition (LBA mode) */
#define MBR_PART_TYPE_EXTL      0x0F    /* Extented partition (LBA mode) */
#define MBR_PART_TYPE_OEM       0x12    /* OEM partition */
#define MBR_PART_TYPE_WINRE     0x27    /* Windows RE hidden partition */
#define MBR_PART_TYPE_DYNAMIC   0x42    /* Dynamic volume partition */
#define MBR_PART_TYPE_LINUXSWP  0x82    /* Linux Swap partition */
#define MBR_PART_TYPE_LINUXNAT  0x83    /* Linux Native partition */
#define MBR_PART_TYPE_LINUXLVM  0x8E    /* Linux LVM partition */
#define MBR_PART_TYPE_FREEBSD   0xA5    /* FreeBSD partition */
#define MBR_PART_TYPE_OPENBSD   0xA6    /* OpenBSD partition */
#define MBR_PART_TYPE_NETBSD    0xA9    /* NetBSD partition */
#define MBR_PART_TYPE_LMBR      0xEE    /* Legacy MBR partition */
#define MBR_PART_TYPE_EFI       0xEF    /* EFI partition */

#define MBR_ERROR_SUCCESS       0
#define MBR_ERROR_INVAL         1
#define MBR_ERROR_NOMEM         2
#define MBR_ERROR_MAGIC         3

typedef struct part {
    uint8_t boot;
//     uint8_t chs_start[3];
    uint8_t type;
//     uint8_t chs_end[3];
    uint32_t lba_start;
    uint32_t lba_size;
} part_t;

typedef struct mbr {
//     uint8_t bootc[440];
    uint8_t sig[4];
//     uint8_t res[2];
    part_t part_tbl[4];
//     uint8_t magic[2];
} mbr_t;

extern mbr_t *mbr_open(uint8_t *buf, int len);
extern void mbr_close(mbr_t *mbr);
extern int mbr_get_sig(mbr_t *mbr, uint32_t *sig);
extern int mbr_part_get_num(mbr_t *mbr, int *num);
extern int mbr_part_is_bootable(mbr_t *mbr, int part, int *bootable);
extern int mbr_part_get_type(mbr_t *mbr, int part, uint8_t *type);
extern int mbr_part_get_lba_start(mbr_t *mbr, int part, uint32_t *lba_start);
extern int mbr_part_get_lba_end(mbr_t *mbr, int part, uint32_t *lba_end);
extern int mbr_get_last_error(void);

#endif

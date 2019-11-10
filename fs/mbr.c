/**
 *
 * File Name: mbr.c
 * Title    : MBR (Master Boot Record) library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-05-05
 * Modified : 2019-11-10
 * Revised  : 
 * Version  : 0.2.0.1
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdlib.h>

#include "mbr.h"

static int error = MBR_ERROR_SUCCESS;

mbr_t *mbr_open(uint8_t *buf, int len)
{
    mbr_t *p;
    uint32_t tmp;
    
    if (!buf) {
        error = MBR_ERROR_INVAL;
        return NULL;
    }
    
    if (len != 512) {
        error = MBR_ERROR_INVAL;
        return NULL;
    }
    
    if ((buf[510] != 0x55) && (buf[511] != 0xAA)) {
        error = MBR_ERROR_MAGIC;
        return NULL;
    }
    
    p = (mbr_t *) malloc(sizeof(mbr_t));
    
    if (!p) {
        error = MBR_ERROR_NOMEM;
        return NULL;
    }
    
    /* Signature */
    p->sig[0] = buf[440];
    p->sig[1] = buf[441];
    p->sig[2] = buf[442];
    p->sig[3] = buf[443];
    
    /* Partition 0 */
    p->part_tbl[0].boot = buf[446];
//     p->part_tbl[0].chs_start[0] = buf[447];
//     p->part_tbl[0].chs_start[1] = buf[448];
//     p->part_tbl[0].chs_start[2] = buf[449];
    p->part_tbl[0].type = buf[450];
//     p->part_tbl[0].chs_end[0] = buf[451];
//     p->part_tbl[0].chs_end[1] = buf[452];
//     p->part_tbl[0].chs_end[2] = buf[453];
    tmp = buf[454];
    tmp |= ((uint32_t) buf[455] << 8);
    tmp |= ((uint32_t) buf[456] << 16);
    tmp |= ((uint32_t) buf[457] << 24);
    p->part_tbl[0].lba_start = tmp;
    tmp = buf[458];
    tmp |= ((uint32_t) buf[459] << 8);
    tmp |= ((uint32_t) buf[460] << 16);
    tmp |= ((uint32_t) buf[461] << 24);
    p->part_tbl[0].lba_size = tmp;
    
    /* Partition 1 */
    p->part_tbl[1].boot = buf[462];
//     p->part_tbl[1].chs_start[0] = buf[463];
//     p->part_tbl[1].chs_start[1] = buf[464];
//     p->part_tbl[1].chs_start[2] = buf[465];
    p->part_tbl[1].type = buf[466];
//     p->part_tbl[1].chs_end[0] = buf[467];
//     p->part_tbl[1].chs_end[1] = buf[468];
//     p->part_tbl[1].chs_end[2] = buf[469];
    tmp = buf[470];
    tmp |= ((uint32_t) buf[471] << 8);
    tmp |= ((uint32_t) buf[472] << 16);
    tmp |= ((uint32_t) buf[473] << 24);
    p->part_tbl[1].lba_start = tmp;
    tmp = buf[474];
    tmp |= ((uint32_t) buf[475] << 8);
    tmp |= ((uint32_t) buf[476] << 16);
    tmp |= ((uint32_t) buf[477] << 24);
    p->part_tbl[1].lba_size = tmp;
    
    /* Partition 2 */
    p->part_tbl[2].boot = buf[478];
//     p->part_tbl[2].chs_start[0] = buf[479];
//     p->part_tbl[2].chs_start[1] = buf[480];
//     p->part_tbl[2].chs_start[2] = buf[481];
    p->part_tbl[2].type = buf[482];
//     p->part_tbl[2].chs_end[0] = buf[483];
//     p->part_tbl[2].chs_end[1] = buf[484];
//     p->part_tbl[2].chs_end[2] = buf[485];
    tmp = buf[486];
    tmp |= ((uint32_t) buf[487] << 8);
    tmp |= ((uint32_t) buf[488] << 16);
    tmp |= ((uint32_t) buf[489] << 24);
    p->part_tbl[2].lba_start = tmp;
    tmp = buf[490];
    tmp |= ((uint32_t) buf[491] << 8);
    tmp |= ((uint32_t) buf[492] << 16);
    tmp |= ((uint32_t) buf[493] << 24);
    p->part_tbl[2].lba_size = tmp;
    
    /* Partition 3 */
    p->part_tbl[3].boot = buf[494];
//     p->part_tbl[3].chs_start[0] = buf[495];
//     p->part_tbl[3].chs_start[1] = buf[496];
//     p->part_tbl[3].chs_start[2] = buf[497];
    p->part_tbl[3].type = buf[498];
//     p->part_tbl[3].chs_end[0] = buf[499];
//     p->part_tbl[3].chs_end[1] = buf[500];
//     p->part_tbl[3].chs_end[2] = buf[501];
    tmp = buf[502];
    tmp |= ((uint32_t) buf[503] << 8);
    tmp |= ((uint32_t) buf[504] << 16);
    tmp |= ((uint32_t) buf[505] << 24);
    p->part_tbl[3].lba_start = tmp;
    tmp = buf[506];
    tmp |= ((uint32_t) buf[507] << 8);
    tmp |= ((uint32_t) buf[508] << 16);
    tmp |= ((uint32_t) buf[509] << 24);
    p->part_tbl[3].lba_size = tmp;
    return p;
}

void mbr_close(mbr_t *mbr)
{
    if (!mbr)
        return;
    
    free(mbr);
}

int mbr_get_sig(mbr_t *mbr, uint32_t *sig)
{
    if (!mbr) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (!sig) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    (*sig) = mbr->sig[0];
    (*sig) += ((uint32_t) mbr->sig[1] << 8);
    (*sig) += ((uint32_t) mbr->sig[2] << 16);
    (*sig) += ((uint32_t) mbr->sig[3] << 24);
    return 0;
}

int mbr_part_get_num(mbr_t *mbr, int *num)
{
    int i;
    int ret = 0;
    
    if (!mbr) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    for (i = 0; i < 4; i++) {
        if (mbr->part_tbl[i].type != MBR_PART_TYPE_EMPTY)
            ret++;
    }
    
    if (!num) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    (*num) = ret;
    return 0;
}

int mbr_part_is_bootable(mbr_t *mbr, int part, int *bootable)
{
    if (!mbr) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (part < 0 && part > 3) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (!bootable) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (mbr->part_tbl[part].boot == 0x80)
        (*bootable) = 1;
    else
        (*bootable) = 0;
    
    return 0;
}

int mbr_part_get_type(mbr_t *mbr, int part, uint8_t *type)
{
    if (!mbr) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (part < 0 && part > 3) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (!type) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    (*type) = mbr->part_tbl[part].type;
    return 0;
}

int mbr_part_get_lba_start(mbr_t *mbr, int part, uint32_t *lba_start)
{
    if (!mbr) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (part < 0 && part > 3) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (!lba_start) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    (*lba_start) = mbr->part_tbl[part].lba_start;
    return 0;
}

int mbr_part_get_lba_end(mbr_t *mbr, int part, uint32_t *lba_end)
{
    if (!mbr) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (part < 0 && part > 3) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    if (!lba_end) {
        error = MBR_ERROR_INVAL;
        return -1;
    }
    
    (*lba_end) = mbr->part_tbl[part].lba_start + mbr->part_tbl[part].lba_size - 1;
    return 0;
}

int mbr_get_last_error(void)
{
    int err;
    err = error;
    error = MBR_ERROR_SUCCESS;
    return err;
}

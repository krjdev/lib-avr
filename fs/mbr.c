/**
 *
 * File Name: mbr.h
 * Title    : MBR (Master Boot Record) library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-05-05
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

#include "mbr.h"

mbr_t *mbr_open(uint8_t *buf, int len)
{
    mbr_t *p;
    uint32_t tmp;
    
    if (!buf)
        return NULL;
    
    if (len != 512)
        return NULL;
    
    if ((buf[510] != 0x55) && (buf[511] != 0xAA))
        return NULL;
    
    p = (mbr_t *) malloc(sizeof(mbr_t));
    
    if (!p)
        return NULL;
    
    /* Partition 0 */
    p->part_tbl[0].boot = buf[446];
    p->part_tbl[0].type = buf[450];
    tmp = buf[454];
    tmp |= (buf[455] << 8);
    tmp |= (buf[456] << 16);
    tmp |= (buf[457] << 24);
    p->part_tbl[0].lba_start = tmp;
    tmp = buf[458];
    tmp |= (buf[459] << 8);
    tmp |= (buf[460] << 16);
    tmp |= (buf[461] << 24);
    p->part_tbl[0].lba_size = tmp;
    
    /* Partition 1 */
    p->part_tbl[1].boot = buf[462];
    p->part_tbl[1].type = buf[466];
    tmp = buf[470];
    tmp |= (buf[471] << 8);
    tmp |= (buf[472] << 16);
    tmp |= (buf[473] << 24);
    p->part_tbl[1].lba_start = tmp;
    tmp = buf[474];
    tmp |= (buf[475] << 8);
    tmp |= (buf[476] << 16);
    tmp |= (buf[477] << 24);
    p->part_tbl[1].lba_size = tmp;
    
    /* Partition 2 */
    p->part_tbl[2].boot = buf[478];
    p->part_tbl[2].type = buf[482];
    tmp = buf[486];
    tmp |= (buf[487] << 8);
    tmp |= (buf[488] << 16);
    tmp |= (buf[489] << 24);
    p->part_tbl[2].lba_start = tmp;
    tmp = buf[490];
    tmp |= (buf[491] << 8);
    tmp |= (buf[492] << 16);
    tmp |= (buf[493] << 24);
    p->part_tbl[2].lba_size = tmp;
    
    /* Partition 3 */
    p->part_tbl[3].boot = buf[494];
    p->part_tbl[3].type = buf[498];
    tmp = buf[502];
    tmp |= (buf[503] << 8);
    tmp |= (buf[504] << 16);
    tmp |= (buf[505] << 24);
    p->part_tbl[3].lba_start = tmp;
    tmp = buf[506];
    tmp |= (buf[507] << 8);
    tmp |= (buf[508] << 16);
    tmp |= (buf[509] << 24);
    p->part_tbl[3].lba_size = tmp;
    return p;
}

void mbr_close(mbr_t *mbr)
{
    if (!mbr)
        return;
    
    free(mbr);
}

int mbr_part_get_num(mbr_t *mbr)
{
    int i;
    int ret = 0;
    
    if (!mbr)
        return -1;
    
    for (i = 0; i < 4; i++) {
        if (mbr->part_tbl[i].lba_size != 0x00000000)
            ret++;
    }
    
    return ret;
}

int mbr_part_is_boot(mbr_t *mbr, int part)
{
    if (!mbr)
        return -1;
    
    if (part < 0 && part > 3)
        return -1;
    
    if (mbr->part_tbl[part].boot == 0x80)
        return 1;
    else
        return 0;
}

uint8_t mbr_part_get_type(mbr_t *mbr, int part)
{
    if (!mbr)
        return 0;
    
    if (part < 0 && part > 3)
        return 0;
    
    return mbr->part_tbl[part].type;
}

uint32_t mbr_part_get_lba_start(mbr_t *mbr, int part)
{
    if (!mbr)
        return 0;
    
    if (part < 0 && part > 3)
        return 0;
    
    return mbr->part_tbl[part].lba_start;
}

uint32_t mbr_part_get_lba_size(mbr_t *mbr, int part)
{
    if (!mbr)
        return 0;
    
    if (part < 0 && part > 3)
        return 0;
    
    return mbr->part_tbl[part].lba_size;
}

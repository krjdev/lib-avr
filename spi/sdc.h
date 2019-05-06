/**
 *
 * File Name: sdc.h
 * Title    : SD card interface library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-04-14
 * Modified : 2019-05-06
 * Revised  : 
 * Version  : 0.3.0.1
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_SPI_SDC_H
#define LIBAVR_SPI_SDC_H

#include <stdint.h>
#include <avr/io.h>

#define SD_CS_CONFIG        (DDRJ = (1 << PJ1))
#define SD_CS_ENABLE        (PORTJ &= ~(1 << PJ1))
#define SD_CS_DISABLE       (PORTJ |= (1 << PJ1))

#define SD_IOCTL_GETTYPE    0

    #define SD_TYPE_SDSC        0
    #define SD_TYPE_SDHC        1
    #define SD_TYPE_SDXC        2

#define SD_IOCTL_GETSIZE    1

extern int sdc_init(void);
extern int sdc_rd_block(uint32_t addr, uint8_t *buf, int len);
extern int sdc_wr_block(uint32_t addr, uint8_t *buf, int len);
extern int sdc_rd(uint64_t addr, uint8_t *buf, int len);
extern int sdc_wr(uint64_t addr, uint8_t *buf, int len);
extern int sdc_ioctl(int type, void *unused, void **ret);

#endif

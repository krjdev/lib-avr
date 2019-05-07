/**
 *
 * File Name: sdc.c
 * Title    : SD card interface library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-04-14
 * Modified : 2019-05-07
 * Revised  : 
 * Version  : 0.4.0.0
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
#include <util/delay.h>

#include "../lib/crc7.h"
#include "../lib/crc16_ccitt.h"
#include "spi.h"
#include "sdc.h"

#define _HIGH(u16)          ((uint8_t) (((u16) & 0xFF00) >> 8))
#define _LOW(u16)           ((uint8_t) ((u16) & 0x00FF))

/* SD card commands */
#define SD_CMD0             0  /* GO_IDLE_STATE */
#define SD_CMD1             1  /* SEND_OP_COND */
#define SD_CMD8             8  /* SEND_IF_COND */
#define SD_CMD9             9  /* SEND_CSD */
#define SD_CMD10            10 /* SEND_CID */
#define SD_CMD13            13 /* SEND_STATUS */
#define SD_CMD16            16 /* SET_BLOCKLEN */
#define SD_CMD17            17 /* READ_SINGLE_BLOCK */
#define SD_CMD24            24 /* WRITE_BLOCK */
#define SD_CMD55            55 /* APP_CMD */
#define SD_CMD58            58 /* READ_OCR */
#define SD_CMD59            59 /* CRC_ON_OFF - currently not used */

#define SD_ACMD41           41 /* SD_SEND_OP_COND */

/* Response types */
#define SD_R1               0
#define SD_R1B              1 /* Currently not supported */
#define SD_R2               2
#define SD_R3               3
#define SD_R7               4

#define SD_TOKEN            0xFE

#define SD_BLOCK_SIZE       512

int init_done;

static int send_cmd(uint8_t cmd, uint8_t *arg, int arg_len, uint8_t *resp, int resp_type)
{
    uint8_t send[6];
    uint8_t recv;
    uint8_t dummy = 0xFF;
    int i;
    int timeout = 255;
    
    send[0] = 0x40 | cmd;
    
    if (arg_len > 4)
        return -1;
    
    if (arg_len > 0) {
        if (!arg)
            return -1;
        
        memcpy(&send[1], arg, arg_len);
    } else
        memset(&send[1], 0, 4);
    
    send[5] = crc7_calc(send, 5);
    
    for (i = 0; i < 10; i++)
        spi_master_send(&dummy, 1);
    
    SD_CS_ENABLE;
    spi_master_send(send, 6);
    
    while (timeout) {
        spi_master_recv(&recv, 1);
        
        if (recv != 0xFF) {
            switch (resp_type) {
            case SD_R1:
                resp[0] = recv;
                SD_CS_DISABLE;
                return 0;
            case SD_R2:
                resp[0] = recv;
                spi_master_recv(&resp[1], 1);
                SD_CS_DISABLE;
                return 0;
            case SD_R3:
                resp[0] = recv;
                spi_master_recv(&resp[1], 4);
                SD_CS_DISABLE;
                return 0;
            case SD_R7:
                resp[0] = recv;
                spi_master_recv(&resp[1], 4);
                SD_CS_DISABLE;
                return 0;
            default:
                return -1;
            }
            
        } else
            timeout--;
    }
    
    SD_CS_DISABLE;
    return -1;
}

static int send_acmd(uint8_t acmd, uint8_t *arg, int arg_len, uint8_t *resp, int resp_type)
{
    uint8_t recv;
    
    if (send_cmd(SD_CMD55, NULL, 0, &recv, SD_R1) == -1)
        return -1;
    
    if (init_done) {
        if (recv != 0x00)
            return -1;
    } else {
        if (recv != 0x01)
            return -1;
    }
    
    if (send_cmd(acmd, arg, arg_len, resp, resp_type) == -1)
        return -1;
    
    return 0;
}

int get_csd(uint8_t *buf, int len)
{
    uint8_t send[6];
    uint8_t recv;
    uint8_t dummy = 0xFF;
    uint8_t csd[18];
    uint16_t crc;
    int i;
    int timeout_out = 255;
    int timeout_in = 255;
    
    if (!buf)
        return -1;
    
    if (len != 16)
        return -1;
    
    send[0] = 0x40 | SD_CMD9;
    send[1] = 0x00;
    send[2] = 0x00;
    send[3] = 0x00;
    send[4] = 0x00;
    send[5] = crc7_calc(send, 5);
    
    for (i = 0; i < 3; i++)
        spi_master_send(&dummy, 1);
    
    SD_CS_ENABLE;
    spi_master_send(send, 6);
    
    while (timeout_out) {
        spi_master_recv(&recv, 1);
        
        if (recv != 0xFF) {
            while (timeout_in) {
                spi_master_recv(&recv, 1);
                    
                if (recv != 0xFF) {
                    if (recv == SD_TOKEN) {
                        spi_master_recv(csd, 18);
                        crc = ((uint16_t) csd[16] << 8);
                        crc |= csd[17];
                        
                        if (!crc16_ccitt_check(csd, 16, crc)) {
                            SD_CS_DISABLE;
                            return -1;
                        }
                        
                        memcpy(buf, csd, len);
                        SD_CS_DISABLE;
                        return 0;
                    }
                } else
                    timeout_in--;
                
                if (!timeout_in) {
                    SD_CS_DISABLE;
                    return -1;
                }
            }
        } else
            timeout_out--;
    }
    
    SD_CS_DISABLE;
    return -1;
}

int get_cid(uint8_t *buf, int len)
{
    uint8_t send[6];
    uint8_t recv;
    uint8_t dummy = 0xFF;
    uint8_t cid[18];
	uint16_t crc;
    int i;
    int timeout_out = 255;
    int timeout_in = 255;
    
	if (!buf)
		return -1;
	
	if (len != 16)
		return -1;
	
    send[0] = 0x40 | SD_CMD10;
    send[1] = 0x00;
    send[2] = 0x00;
    send[3] = 0x00;
    send[4] = 0x00;
    send[5] = crc7_calc(send, 5);
    
    for (i = 0; i < 3; i++)
        spi_master_send(&dummy, 1);
    
    SD_CS_ENABLE;
    spi_master_send(send, 6);
    
    while (timeout_out) {
        spi_master_recv(&recv, 1);
        
        if (recv != 0xFF) {
            while (timeout_in) {
                spi_master_recv(&recv, 1);
                    
                if (recv != 0xFF) {
                    if (recv == SD_TOKEN) {
                        spi_master_recv(cid, 18);
                        crc = ((uint16_t) cid[16] << 8);
                        crc |= cid[17];
                        
                        if (!crc16_ccitt_check(cid, 16, crc)) {
                            SD_CS_DISABLE;
                            return -1;
                        }
                        
                        memcpy(buf, cid, len);
                        SD_CS_DISABLE;
                        return 0;
                    }
                } else
                    timeout_in--;
                
                if (!timeout_in) {
                    SD_CS_DISABLE;
                    return -1;
                }
            }
        } else
            timeout_out--;
    }
    
    SD_CS_DISABLE;
    return -1;
}

int get_status(uint16_t *status)
{
    uint8_t resp[2];
    uint16_t tmp;
    
    if (send_cmd(SD_CMD13, NULL, 0, resp, SD_R2) == -1)
        return -1;
    
    tmp = resp[0] << 8;
    tmp |= resp[1];
    (*status) = tmp;
    return 0;
}

int get_ocr(uint32_t *ocr)
{
    uint8_t resp[5];
    uint32_t tmp;
    
    if (send_cmd(SD_CMD58, NULL, 0, resp, SD_R3) == -1)
        return -1;
    
    if (init_done) {
        if (resp[0] != 0x00)
            return -1;
    } else {
        if (resp[0] != 0x01)
            return -1;
    }
    
    tmp = (uint32_t) resp[1] << 24;
    tmp |= (uint32_t) resp[2] << 16;
    tmp |= (uint32_t) resp[3] << 8;
    tmp |= (uint32_t) resp[4];
    (*ocr) = tmp;
    return 0;
}

int rd_block(uint32_t addr, uint8_t *buf, int len)
{
    uint8_t send[6];
    uint8_t recv;
    uint8_t crc[2];
    uint16_t tmp;
    uint8_t dummy = 0xFF;
    int i;
    int timeout_out = 255;
    int timeout_in = 255;
    
    send[0] = 0x40 | SD_CMD17;
    send[1] = 0xFF & (uint8_t) (addr >> 24);
    send[2] = 0xFF & (uint8_t) (addr >> 16);
    send[3] = 0xFF & (uint8_t) (addr >> 8);
    send[4] = 0xFF & (uint8_t) addr;
    send[5] = crc7_calc(send, 5);
    
    for (i = 0; i < 10; i++)
        spi_master_send(&dummy, 1);
    
    SD_CS_ENABLE;
    spi_master_send(send, 6);
    
    while (timeout_out) {
        spi_master_recv(&recv, 1);
        
        if (recv != 0xFF) {
            if (recv == 0x00) {
                while (timeout_in) {
                    spi_master_recv(&recv, 1);
                    
                    if (recv != 0xFF) {
                        if (recv == SD_TOKEN) {
                            spi_master_recv(buf, len);
                            spi_master_recv(crc, 2);
                            SD_CS_DISABLE;
                            tmp = (uint16_t) crc[0] << 8;
                            tmp |= (uint16_t) crc[1];
                            
                            if (crc16_ccitt_check(buf, len, tmp))
                                return 0;
                            else
                                return -1;
                        } else {
                            SD_CS_DISABLE;
                            return -1;
                        }
                    } else
                        timeout_in--;
                }
                
                if (!timeout_in) {
                    SD_CS_DISABLE;
                    return -1;
                }
            } else {
                SD_CS_DISABLE;
                return -1;
            }
        } else
            timeout_out--;
    }
    
    SD_CS_DISABLE;
    return -1;
}

int wr_block(uint32_t addr, uint8_t *buf, int len)
{
    uint8_t send[6];
    uint8_t recv;
    uint8_t crc[2];
    uint16_t tmp;
    uint8_t dummy = 0xFF;
    int i;
    int timeout_out = 255;
    int timeout_in = 255;
    
    send[0] = 0x40 | SD_CMD24;
    send[1] = 0xFF & (uint8_t) (addr >> 24);
    send[2] = 0xFF & (uint8_t) (addr >> 16);
    send[3] = 0xFF & (uint8_t) (addr >> 8);
    send[4] = 0xFF & (uint8_t) addr;
    send[5] = crc7_calc(send, 5);
    tmp = crc16_ccitt_calc(buf, len);
    crc[0] = (uint8_t) (tmp >> 8);
    crc[1] = (uint8_t) tmp;
    
    for (i = 0; i < 10; i++)
        spi_master_send(&dummy, 1);
    
    SD_CS_ENABLE;
    spi_master_send(send, 6);
    
    while (timeout_out) {
        spi_master_recv(&recv, 1);
        
        if (recv != 0xFF) {
            if (recv == 0x00) {
                send[0] = SD_TOKEN;
                spi_master_send(send, 1);
                spi_master_send(buf, len);
                spi_master_send(crc, 2);
                spi_master_recv(&recv, 1);
                
                if ((recv & 0x1F) != 0x05) {
                    SD_CS_DISABLE;
                    return -1;
                } else {
                    while (timeout_in) {
                        spi_master_recv(&recv, 1);
                        
                        if (recv != 0x00) {
                            SD_CS_DISABLE;
                            return 0;
                        } else
                            timeout_in--;
                        
                        if (!timeout_in) {
                            SD_CS_DISABLE;
                            return -1;
                        }
                    }
                }
                
            } else {
                SD_CS_DISABLE;
                return -1;
            }
        } else
            timeout_out--;
    }
    
    SD_CS_DISABLE;
    return -1;
}


static int get_type(void)
{
    uint8_t csd[16];
    uint32_t c_size;
    
    if (!init_done)
        return -1;
    
    if (get_csd(csd, 16) == -1)
        return -1;
    
    if ((csd[0] & 0xC0) == 0x00)
        return SD_TYPE_SDSC;
    else if ((csd[0] & 0xC0) == 0x40) {
        c_size = (uint64_t) csd[7] << 16;
        c_size |= (uint64_t) csd[8] << 8;
        c_size |= (uint64_t) csd[9];
        
        if (c_size >= 0xFFFF)
            return SD_TYPE_SDXC;
        else
            return SD_TYPE_SDHC;
    } else
        return -1;
}

static uint64_t get_size(void)
{
    uint8_t csd[16];
    uint64_t size;
    uint8_t blk_len;
    uint8_t mul;
    
    if (!init_done)
        return 0;
    
    if (get_csd(csd, 16) == -1)
        return 0;
    
    if ((csd[0] & 0xC0) == 0x00) {
        blk_len = (csd[5] & 0x0F);
        mul = ((csd[9] & 0x03) << 2); 
        mul |= ((csd[10] & 0x80) >> 1);
        size = ((uint64_t) (csd[6] & 0x03) << 10);
        size |= ((uint64_t) csd[7] << 2);
        size |= (((uint64_t) csd[8] & 0xC0) >> 6);
        size += 1;
        size = ((size * (1 << (mul + 2))) * (1 << blk_len));
    } else if ((csd[0] & 0xC0) == 0x40) {
        size = (uint64_t) csd[7] << 16;
        size |= (uint64_t) csd[8] << 8;
        size |= (uint64_t) csd[9];
        size += 1;
        size *= 524288L;
    } else
        return 0;
    
    return size;
}

int sdc_init(void)
{
    uint8_t dummy = 0xFF;
    uint8_t args[4];
    uint8_t resp[5];
    int timeout = 4096;
    int i;
    
    init_done = 0;
    
    /* init SPI interface */
    SD_CS_CONFIG;
    SD_CS_DISABLE;
    spi_master_init(SPI_MODE_0, SPI_FOSC_128, SPI_ORDER_MSB);
    
    /* wait for SD card */
    _delay_ms(10);
    
    /* switch SD card to SPI mode */
    for (i = 0; i < 20; i++)
        spi_master_send(&dummy, 1);
    
    _delay_ms(1);
    
    /* reset SD card */
    if (send_cmd(SD_CMD0, NULL, 0, resp, SD_R1) == -1)
        return -1;
    
    if (resp[0] != 0x01)
        return -1;
    
    /* send operation condition */
    args[0] = 0x00;
    args[1] = 0x00;
    args[2] = 0x01;
    args[3] = 0xAA;
    
    if (send_cmd(SD_CMD8, args, 4, resp, SD_R7) == -1)
        return -1;
    
    if (resp[0] != 0x01)
        return -1;
    
    /* start init process */
    args[0] = 0x40;
    args[1] = 0x00;
    args[2] = 0x00;
    args[3] = 0x00;
    
    while (timeout) {
        if (!timeout)
            return -1;
        
        if (send_acmd(SD_ACMD41, args, 4, resp, SD_R1) == -1)
            return -1;
        
        if (resp[0] == 0x00)
            timeout = 0;
        else
            timeout--;
    }
    
    /* set block length */
    args[0] = 0x00;
    args[1] = 0x00;
    args[2] = _HIGH(SD_BLOCK_SIZE);
    args[3] = _LOW(SD_BLOCK_SIZE);
    
    if (send_cmd(SD_CMD16, args, 4, resp, SD_R1) == -1)
        return -1;
    
    if (resp[0] != 0x00)
        return -1;
    
    /* init done; now we can switch to max. SPI speed */
    spi_master_close();
    spi_master_init(SPI_MODE_0, SPI_FOSC_2, SPI_ORDER_MSB);
    init_done = 1;
    return 0;
}

int sdc_rd_block(uint32_t addr, uint8_t *buf, int len)
{
    if (!init_done)
        return -1;
    
    if (!buf)
        return -1;
    
    if (len != SD_BLOCK_SIZE)
        return -1;
    
    if (rd_block(addr, buf, len) == -1)
        return -1;
    
    return 0;
}


int sdc_wr_block(uint32_t addr, uint8_t *buf, int len)
{
    if (!init_done)
        return -1;
    
    if (!buf)
        return -1;
    
    if (len != SD_BLOCK_SIZE)
        return -1;
    
    if (wr_block(addr, buf, len) == -1)
        return -1;
    
    return 0;
}

int sdc_rd(uint64_t addr, uint8_t *buf, int len)
{
    uint8_t b[SD_BLOCK_SIZE];
    uint32_t blk_addr;
    int start;
    
    if (!init_done)
        return -1;
    
    if (!buf)
        return -1;
    
    if ((len < 1) && (len > SD_BLOCK_SIZE))
        return -1;
    
    if (addr < SD_BLOCK_SIZE) {
        blk_addr = 0x00000000;
        start = addr;
    } else {
        blk_addr = addr / SD_BLOCK_SIZE;
        start = addr % SD_BLOCK_SIZE;
    }
    
    if (rd_block(blk_addr, b, SD_BLOCK_SIZE) == -1)
        return -1;
    
    memcpy(buf, &b[start], len);
    return 0;
}

int sdc_wr(uint64_t addr, uint8_t *buf, int len)
{
    uint8_t b[SD_BLOCK_SIZE];
    uint32_t blk_addr;
    int start;
    
    if (!init_done)
        return -1;
    
    if (!buf)
        return -1;
    
    if ((len < 1) && (len > SD_BLOCK_SIZE))
        return -1;
    
    if (addr < SD_BLOCK_SIZE) {
        blk_addr = 0x00000000;
        start = addr;
    } else {
        blk_addr = addr / SD_BLOCK_SIZE;
        start = addr % SD_BLOCK_SIZE;
    }
    
    if (rd_block(blk_addr, b, SD_BLOCK_SIZE) == -1)
        return -1;
    
    memcpy(&b[start], buf, len);
    
    if (wr_block(blk_addr, b, SD_BLOCK_SIZE) == -1)
        return -1;
    
    return 0;
}

int sdc_ioctl(int type, void *unused, void *ret)
{
    if (!ret)
        return -1;
    
    switch (type) {
    case SD_IOCTL_GETINFO:
        ((struct sd_info *) ret)->type = get_type();
        ((struct sd_info *) ret)->size = get_size();
        get_status(&((struct sd_info *) ret)->status);
        get_ocr(&((struct sd_info *) ret)->ocr);
        break;
    case SD_IOCTL_GETCID:
        get_cid(((struct sd_cid *) ret)->data, 16);
        break;
    case SD_IOCTL_GETCSD:
        get_cid(((struct sd_csd *) ret)->data, 16);
        break;
    default:
        return -1;
    }
    
    return 0;
}


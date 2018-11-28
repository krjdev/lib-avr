/**
 *
 * File Name: enc28j60.c
 * Title    : SPI device Microchip ENC28J60 ethernet controller library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-22
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
#include <util/delay.h>

#include "enc28j60.h"
#include "spi.h"

#define ISCLR(reg, bit) 

#define _HIGH(u16)          ((uint8_t) (((u16) & 0xFF00) >> 8))
#define _LOW(u16)           ((uint8_t) ((u16) & 0x00FF))
#define _ISCLR(reg, bit)    (((reg) | (1 << (bit))) ^ (reg))
#define _ISSET(reg, bit)    ((reg) & (1 << (bit)))

#define MAX_FRAMESZ 1500

#define RX_START    0x0000
#define RX_END      0x1A00
#define TX_START    0x1A02
#define TX_END      0x1FFF

static int select_bank(uint8_t bank)
{
    uint8_t send[2];
    uint8_t recv;
    uint8_t tmp;
    
    /* read ECON1 register */
    send[0] = SPI_RCR | ECON1;
    ENC28J60_ENABLE;
    spi_master_send(&send[0], 1);
    spi_master_recv(&recv, 1);
    _delay_us(1);
    ENC28J60_DISABLE;
    
    /* select bank in ECON1 register */
    send[0] = SPI_WCR | ECON1;
    tmp = recv;
    
    switch (bank) {
    case BANK0:
        tmp &= ~(1 << ECON1_BSEL0);
        tmp &= ~(1 << ECON1_BSEL1);
        break;
    case BANK1:
        tmp |= (1 << ECON1_BSEL0);
        tmp &= ~(1 << ECON1_BSEL1);
        break;
    case BANK2:
        tmp &= ~(1 << ECON1_BSEL0);
        tmp |= (1 << ECON1_BSEL1);
        break;
    case BANK3:
        tmp |= (1 << ECON1_BSEL0);
        tmp |= (1 << ECON1_BSEL1);
        break;
    default:
        return -1;
    }
    
    send[1] = tmp;
    ENC28J60_ENABLE;
    spi_master_send(&send[0], 2);
    _delay_us(1);
    ENC28J60_DISABLE;
    return 0;
}

static void read_reg(uint8_t bank, uint8_t reg, uint8_t *val)
{
    uint8_t send;
    
    if (select_bank(bank) == -1)
        return;
    
    if (!val)
        return;
    
    send = SPI_RCR | reg;
    ENC28J60_ENABLE;
    spi_master_send(&send, 1);
    spi_master_recv(val, 1);
    _delay_us(1);
    ENC28J60_DISABLE;
}

static void read_mii_mac_reg(uint8_t bank, uint8_t reg, uint8_t *val)
{
    uint8_t send;
    uint8_t recv[2];
    
    if (select_bank(bank) == -1)
        return;
    
    if (!val)
        return;
    
    send = SPI_RCR | reg;
    ENC28J60_ENABLE;
    spi_master_send(&send, 1);
    spi_master_recv(&recv[0], 2);
    _delay_us(1);
    ENC28J60_DISABLE;
    (*val) = recv[1];
}

static void write_reg(uint8_t bank, uint8_t reg, uint8_t val)
{
    uint8_t send[2];
    
    if (select_bank(bank) == -1)
        return;
    
    send[0] = SPI_WCR | reg;
    send[1] = val;
    ENC28J60_ENABLE;
    spi_master_send(&send[0], 2);
    _delay_us(1);
    ENC28J60_DISABLE;
}

static void read_phy_reg(uint8_t reg, uint16_t *val)
{
    uint8_t tmp;
    
    if (!val)
        return;
    
    write_reg(BANK2, MIREGADR, reg);
    write_reg(BANK2, MICMD, (1 << MICMD_MIIRD));
    _delay_us(20);
    read_mii_mac_reg(BANK3, MISTAT, &tmp);
    
    while (tmp & (1 << MISTAT_BUSY))
        read_mii_mac_reg(BANK3, MISTAT, &tmp);
    
    write_reg(BANK2, MICMD, 0x00);
    read_mii_mac_reg(BANK2, MIRDH, &tmp);
    (*val) |= (tmp << 8);
    read_mii_mac_reg(BANK2, MIRDL, &tmp);
    (*val) |= tmp;
}

static void write_phy_reg(uint8_t reg, uint16_t val)
{
    uint8_t tmp;
    
    write_reg(BANK2, MIREGADR, reg);
    write_reg(BANK2, MIWRL, _LOW(val));
    write_reg(BANK2, MIWRH, _HIGH(val));
    _delay_us(100);
    read_mii_mac_reg(BANK3, MISTAT, &tmp);
    
    while (tmp & (1 << MISTAT_BUSY))
        read_mii_mac_reg(BANK3, MISTAT, &tmp);
}

static void read_buffer(uint16_t addr, uint8_t *buf, int len)
{
    uint8_t send;
    
    if (addr > (ENC28J60_BUFSZ - 1))
        return;
    
    if (!buf)
        return;
    
    if (len > ENC28J60_BUFSZ)
        return;
    
    write_reg(BANK0, ERDPTL, _LOW(addr));
    write_reg(BANK0, ERDPTH, _HIGH(addr));
    send = SPI_RBM;
    ENC28J60_ENABLE;
    spi_master_send(&send, 1);
    spi_master_recv(buf, len);
    _delay_us(1);
    ENC28J60_DISABLE;
}

static void write_buffer(uint16_t addr, uint8_t *buf, int len)
{
    uint8_t send;
    
    if (addr > (ENC28J60_BUFSZ - 1))
        return;
    
    if (!buf)
        return;
    
    if (len > ENC28J60_BUFSZ)
        return;
    
    write_reg(BANK0, EWRPTL, _LOW(addr));
    write_reg(BANK0, EWRPTH, _HIGH(addr));
    send = SPI_WBM;
    ENC28J60_ENABLE;
    spi_master_send(&send, 1);
    spi_master_send(buf, len);
    _delay_us(1);
    ENC28J60_DISABLE;
}

void enc28j60_set_mac(mac_addr_t *addr)
{
    write_reg(BANK3, MAADR1, addr->ma_byte0);
    write_reg(BANK3, MAADR2, addr->ma_byte1);
    write_reg(BANK3, MAADR3, addr->ma_byte2);
    write_reg(BANK3, MAADR4, addr->ma_byte3);
    write_reg(BANK3, MAADR5, addr->ma_byte4);
    write_reg(BANK3, MAADR6, addr->ma_byte5);
}

void enc28j60_get_mac(mac_addr_t *addr)
{
    uint8_t tmp;
    
    if (!addr)
        return;
    
    read_mii_mac_reg(BANK3, MAADR1, &tmp);
    addr->ma_byte0 = tmp;
    read_mii_mac_reg(BANK3, MAADR2, &tmp);
    addr->ma_byte1 = tmp;
    read_mii_mac_reg(BANK3, MAADR3, &tmp);
    addr->ma_byte2 = tmp;
    read_mii_mac_reg(BANK3, MAADR4, &tmp);
    addr->ma_byte3 = tmp;
    read_mii_mac_reg(BANK3, MAADR5, &tmp);
    addr->ma_byte4 = tmp;
    read_mii_mac_reg(BANK3, MAADR6, &tmp);
    addr->ma_byte5 = tmp;
}

uint8_t enc28j60_get_revision(void)
{
    uint8_t tmp;
    
    read_reg(BANK3, EREVID, &tmp);
    return tmp;
}

void enc28j60_init(int mode, mac_addr_t *addr)
{
    uint8_t tmp;
    
    /* Init SPI interface */
    ENC28J60_CSCONFIG;
    spi_master_init(SPI_FOSC_2);
    
    /* Reset controller */
    tmp = SPI_SRC;
    spi_master_send(&tmp, 1);
    _delay_ms(2);

    /* Receive Buffer */
    write_reg(BANK0, ERXSTL, _LOW(RX_START));
    write_reg(BANK0, ERXSTH, _HIGH(RX_START));
    write_reg(BANK0, ERXNDL, _LOW(RX_END));
    write_reg(BANK0, ERXNDH, _HIGH(RX_END));
    
    /* Transmit Buffer */
    write_reg(BANK0, ETXSTL, _LOW(TX_START));
    write_reg(BANK0, ETXSTH, _HIGH(TX_START));
    
    /* Receive Filters */
    
    /* Waiting for OST */
    read_reg(BANK0, ESTAT, &tmp);
    
    while (!(tmp & (1 << ESTAT_CLKRDY)))
        read_reg(BANK0, ESTAT, &tmp);
    
    /* MAC Initialization Settings */
    write_reg(BANK2, MACON1, (1 << MACON1_MARXEN) | 
                             (1 << MACON1_TXPAUS) | 
                             (1 << MACON1_RXPAUS));
    
    if (mode == MODE_FDPX)
        write_reg(BANK2, MACON3, (1 << MACON3_FULDPX) | 
                                 (1 << MACON3_FRMLNEN) | 
                                 (1 << MACON3_TXCRCEN) | 
                                 (1 << MACON3_PADCFG0) | 
                                 (1 << MACON3_PADCFG2));
    else
        write_reg(BANK2, MACON3, (1 << MACON3_FRMLNEN) |
                                 (1 << MACON3_TXCRCEN) | 
                                 (1 << MACON3_PADCFG0) |
                                 (1 << MACON3_PADCFG2));
    
    write_reg(BANK2, MACON4, (1 << MACON4_DEFER));
    write_reg(BANK2, MAMXFLL, _LOW(MAX_FRAMESZ));
    write_reg(BANK2, MAMXFLH, _HIGH(MAX_FRAMESZ));
    
    if (mode == MODE_FDPX)
        write_reg(BANK2, MABBIPG, 0x15);
    else
        write_reg(BANK2, MABBIPG, 0x12);
    
    write_reg(BANK2, MAIPGL, 0x12);
    
    if (mode == MODE_HDPX)
        write_reg(BANK2, MAIPGH, 0x0C);
    
    enc28j60_set_mac(addr);
    
    /* PHY Initialization Settings */
    if (mode == MODE_FDPX)
        write_phy_reg(PHCON1, (1 << PHCON1_PDPXMD));
    else
        write_phy_reg(PHCON1, 0x0000);
    
    /* Enable receiving packets */
    write_reg(BANK0, ECON1, (1 << ECON1_RXEN) | (1 << ECON1_CSUMEN));
}

int enc28j60_link_up(void)
{
    uint16_t tmp;
    
    read_phy_reg(PHSTAT1, &tmp);
    
    if (tmp & (1 << PHSTAT1_LLSTAT))
        return 1;
    else
        return 0;
}

int enc28j60_frame_count(void)
{
    uint8_t tmp;
    
    read_reg(BANK1, EPKTCNT, &tmp);
    return tmp;
}

uint16_t enc28j60_free_space(void)
{
    uint8_t tmp;
    uint16_t p, rdp, wrp;
    
    read_reg(BANK0, ERXWRPTL, &tmp);
    wrp = tmp;
    read_reg(BANK0, ERXWRPTH, &tmp);
    p = tmp;
    wrp |= (p << 8);
    read_reg(BANK0, ERXRDPTL, &tmp);
    rdp = tmp;
    read_reg(BANK0, ERXRDPTH, &tmp);
    p = tmp;
    rdp |= (p << 8);
    
    if (wrp > rdp)
        return ((RX_END - RX_START) - (wrp - rdp));
    else if (wrp == rdp)
        return (RX_END - RX_START);
    else
        return (rdp - wrp - 1);
}

int enc28j60_send(eth_frame_t *frame)
{
    int len;
    uint8_t *p;
    uint8_t tmp;
    uint8_t ctrl = (1 << PCRCEN);
    
    ethernet_frame_to_buffer(frame, &p, &len);
    
    write_buffer(TX_START, &ctrl, 1);
    write_buffer((TX_START + 1), p, len);
    free(p);
    
    write_reg(BANK0, ETXNDL, _LOW((TX_START + len)));
    write_reg(BANK0, ETXNDH, _HIGH((TX_START + len)));
    
    /* transmit packet */
    read_reg(BANK0, ECON1, &tmp);
    tmp |= (1 << ECON1_TXRTS);
    write_reg(BANK0, ECON1, tmp);
    
    read_reg(BANK0, ECON1, &tmp);
    
    while (_ISCLR(tmp, ECON1_TXRTS))
        read_reg(BANK0, ECON1, &tmp);

    read_reg(BANK0, ESTAT, &tmp);
    
    if (_ISSET(tmp, ESTAT_TXABRT))
        return -1;
    
    return 0;
}

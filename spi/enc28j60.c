/**
 *
 * File Name: enc28j60.c
 * Title    : SPI device Microchip ENC28J60 Ethernet controller library
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-22
 * Modified : 2019-05-04
 * Revised  : 
 * Version  : 0.4.2.0
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

#define VERSION             "ENC28J60 driver: 0.4.2.0"

#define _HIGH(u16)          ((uint8_t) (((u16) & 0xFF00) >> 8))
#define _LOW(u16)           ((uint8_t) ((u16) & 0x00FF))
#define _ISCLR(reg, bit)    (((reg) | (1 << (bit))) ^ (reg))
#define _ISSET(reg, bit)    ((reg) & (1 << (bit)))

#define MAX_FRAME_SIZE      1518

#define BUF_SIZE            8192
#define BUF_RX_START        0x0000
#define BUF_RX_END          0x19FF
#define BUF_RX_SIZE         (BUF_RX_END - BUF_RX_START + 1)
#define BUF_TX_START        0x1A00
#define BUF_TX_END          0x1FFF
#define BUF_TX_SIZE         (BUF_TX_END - BUF_TX_START + 1)
#define BUF_PTR_SIZE         2
#define BUF_PTR_LO           0
#define BUF_PTR_HI           1

#define TIMEOUT_CNT          10000

/* SPI Instruction Set */
#define SPI_RCR     0x00 /* Read Control Register */
#define SPI_RBM     0x3A /* Read Buffer Memory */
#define SPI_WCR     0x40 /* Write Control Register */
#define SPI_WBM     0x7A /* Write Buffer Memory */
#define SPI_BFS     0x80 /* Bit Field Set */
#define SPI_BFC     0xA0 /* Bit Field Clear */
#define SPI_SRC     0xFF /* System Reset Command (Soft Reset) */

/* Register banks */
#define BANK0       0x00
#define BANK1       0x01
#define BANK2       0x02
#define BANK3       0x03

/* Common Register (8-bit) */
#define EIE         0x1B /* Ethernet Interrupt Enable Register */

    /* EIE flags */
    #define EIE_RXERIE      0 /* Receive Error Interrupt Enable bit */
    #define EIE_TXERIE      1 /* Transmit Error Interrupt Enable bit */
    #define EIE_TXIE        3 /* Transmit Enable bit */
    #define EIE_LINKIE      4 /* Link Status Change Interrupt Enable bit */
    #define EIE_DMAIE       5 /* DMA Interrupt Enable bit */
    #define EIE_PKTIE       6 /* Receive Packet Pending Interrupt Enable bit */
    #define EIE_INTIE       7 /* Global INT Interrupt Enable bit */

#define EIR         0x1C /* Ethernet Interrupt Request (Flag) Register */

    /* EIR flags */
    #define EIR_RXERIF      0 /* Receive Error Interrupt Flag bit */
    #define EIR_TXERIF      1 /* Transmit Error Interrupt Flag bit */
    #define EIR_TXIF        3 /* Transmit Interrupt Flag bit */
    #define EIR_LINKIF      4 /* Link Change Interrupt Flag bit */
    #define EIR_DMAIF       5 /* DMA Interrupt Flag bit */
    #define EIR_PKTIF       6 /* Receive Packet Pending Interrupt Flag bit */

#define ESTAT       0x1D /* Ethernet Status Register */

    /* ESTAT flags */
    #define ESTAT_CLKRDY    0 /* Clock Ready bit */
    #define ESTAT_TXABRT    1 /* Transmit Abort Error bit */
    #define ESTAT_RXBUSY    2 /* Receive Busy bit */
    #define ESTAT_LATECOL   4 /* Late Collision Error bit */
    #define ESTAT_BUFER     6 /* Ethernet Buffer Error Status bit */
    #define ESTAT_INT       7 /* INT Interrupt Flag bit */

#define ECON2       0x1E /* Ethernet Control Register 2 */

    /* ECON2 flags */
    #define ECON2_VRPS      3 /* Voltage Regulator Power Save Enable bit */
    #define ECON2_PWRSV     5 /* Power Save Enable bit */
    #define ECON2_PKTDEC    6 /* Packet Decrement bit */
    #define ECON2_AUTOINC   7 /* Automatic Buffer Pointer Increment Enable bit */

#define ECON1       0x1F /* Ethernet Control Register 1 */

    /* ECON1 flags */
    #define ECON1_BSEL0     0 /* Bank Select bit 0 */
    #define ECON1_BSEL1     1 /* Bank Select bit 1 */
    #define ECON1_RXEN      2 /* Receive Enable bit */
    #define ECON1_TXRTS     3 /* Transmit Request to Send bit */
    #define ECON1_CSUMEN    4 /* DMA Checksum Enable bit */
    #define ECON1_DMAST     5 /* DMA Start and Busy Status bit */
    #define ECON1_RXRST     6 /* Receive Logic Reset bit */
    #define ECON1_TXRST     7 /* Transmit Logic Reset bit */

/* Bank 0 Register (8-bit) */
#define ERDPTL      0x00 /* Read Pointer Low Byte (ERDPT<7:0>) */
#define ERDPTH      0x01 /* Read Pointer High Byte (ERDPT<12:8>) */
#define EWRPTL      0x02 /* Write Pointer Low Byte (EWRPT<7:0>) */
#define EWRPTH      0x03 /* Write Pointer High Byte (EWRPT<12:8>) */
#define ETXSTL      0x04 /* TX Start Low Byte (ETXST<7:0>) */
#define ETXSTH      0x05 /* TX Start High Byte (ETXST<12:8>) */
#define ETXNDL      0x06 /* TX End Low Byte (ETXND<7:0>) */
#define ETXNDH      0x07 /* TX End High Byte (ETXND<12:8>) */
#define ERXSTL      0x08 /* RX Start Low Byte (ERXST<7:0>) */
#define ERXSTH      0x09 /* RX Start High Byte (ERXST<12:8>) */
#define ERXNDL      0x0A /* RX End Low Byte (ERXND<7:0>) */
#define ERXNDH      0x0B /* RX End High Byte (ERXND<12:8>) */
#define ERXRDPTL    0x0C /* RX RD Pointer Low Byte (ERXRDPT<7:0>) */
#define ERXRDPTH    0x0D /* RX RD Pointer High Byte (ERXRDPT<12:8>) */
#define ERXWRPTL    0x0E /* RX WR Pointer Low Byte (ERXWRPT<7:0>) */
#define ERXWRPTH    0x0F /* RX WR Pointer High Byte (ERXWRPT<12:8>) */
#define EDMASTL     0x10 /* DMA Start Low Byte (EDMAST<7:0>) */
#define EDMASTH     0x11 /* DMA Start High Byte (EDMAST<12:8>) */
#define EDMANDL     0x12 /* DMA End Low Byte (EDMAND<7:0>) */
#define EDMANDH     0x13 /* DMA End High Byte (EDMAND<12:8>) */
#define EDMADSTL    0x14 /* DMA Destination Low Byte (EDMADST<7:0>) */
#define EDMADSTH    0x15 /* DMA Destination High Byte (EDMADST<12:8>) */
#define EDMACSL     0x16 /* DMA Checksum Low Byte (EDMACS<7:0>) */
#define EDMACSH     0x17 /* DMA Checksum High Byte (EDMACS<15:8>) */

/* Bank 1 Register (8-bit) */
#define EHT0        0x00 /* Hash Table Byte 0 (EHT<7:0>) */
#define EHT1        0x01 /* Hash Table Byte 1 (EHT<15:8>) */
#define EHT2        0x02 /* Hash Table Byte 2 (EHT<23:16>) */
#define EHT3        0x03 /* Hash Table Byte 3 (EHT<31:24>) */
#define EHT4        0x04 /* Hash Table Byte 4 (EHT<39:32>) */
#define EHT5        0x05 /* Hash Table Byte 5 (EHT<47:40>) */
#define EHT6        0x06 /* Hash Table Byte 6 (EHT<55:48>) */
#define EHT7        0x07 /* Hash Table Byte 7 (EHT<63:56>) */
#define EPMM0       0x08 /* Pattern Match Mask Byte 0 (EPMM<7:0>) */
#define EPMM1       0x09 /* Pattern Match Mask Byte 1 (EPMM<15:8>) */
#define EPMM2       0x0A /* Pattern Match Mask Byte 2 (EPMM<23:16>) */
#define EPMM3       0x0B /* Pattern Match Mask Byte 3 (EPMM<31:24>) */
#define EPMM4       0x0C /* Pattern Match Mask Byte 4 (EPMM<39:32>) */
#define EPMM5       0x0D /* Pattern Match Mask Byte 5 (EPMM<47:40>) */
#define EPMM6       0x0E /* Pattern Match Mask Byte 6 (EPMM<55:48>) */
#define EPMM7       0x0F /* Pattern Match Mask Byte 7 (EPMM<63:56>) */
#define EPMCSL      0x10 /* Pattern Match Checksum Low Byte (EPMCS<7:0>) */
#define EPMCSH      0x11 /* Pattern Match Checksum High Byte (EPMCS<15:0>) */
#define EPMOL       0x14 /* Pattern Match Offset Low Byte (EPMO<7:0>) */
#define EPMOH       0x15 /* Pattern Match Offset High Byte (EPMO<12:8>) */
#define ERXFCON     0x18 /* Ethernet Receive Filter Control Register */

    /* ERXFCON flags */
    #define ERXFCON_BCEN    0 /* Broadcast Filter Enable bit */
    #define ERXFCON_MCEN    1 /* Multicast Filter Enable bit */
    #define ERXFCON_HTEN    2 /* Hash Table Filter Enable bit */
    #define ERXFCON_MPEN    3 /* Magic Packet Filter Enable bit */
    #define ERXFCON_PMEN    4 /* Pattern Match Filter Enable bit */
    #define ERXFCON_CRCEN   5 /* Post-Filter CRC Check Enable bit */
    #define ERXFCON_ANDOR   6 /* AND/OR Filter Select bit */
    #define ERXFCON_UCEN    7 /* Unicast Filter Enable bit */

#define EPKTCNT     0x19 /* Ethernet Packet Count */

/* Bank 2 Register (8-bit) */
#define MACON1      0x00 /* MAC Control Register 1 */

    /* MACON1 flags */
    #define MACON1_MARXEN   0 /* MAC Receive Enable bit */
    #define MACON1_PASSALL  1 /* Pass All Received Frames Enable bit */
    #define MACON1_RXPAUS   2 /* Pause Control Frame Reception Enable bit */
    #define MACON1_TXPAUS   3 /* Pause Control Frame Transmission Enable bit */

#define MACON3      0x02 /* MAC Control Register 3 */

    /* MACON3 flags */
    #define MACON3_FULDPX   0 /* MAC Full-Duplex Enable bit */
    #define MACON3_FRMLNEN  1 /* Frame Length Checking Enable bit */
    #define MACON3_HFRMEN   2 /* Huge Frame Enable bit */
    #define MACON3_PHDREN   3 /* Proprietary Header Enable bit */
    #define MACON3_TXCRCEN  4 /* Transmit CRC Enable bit */
    #define MACON3_PADCFG0  5 /* Automatic Pad and CRC Configuration bit 0 */
    #define MACON3_PADCFG1  6 /* Automatic Pad and CRC Configuration bit 1 */
    #define MACON3_PADCFG2  7 /* Automatic Pad and CRC Configuration bit 2 */

#define MACON4      0x03 /* MAC Control Register 4 */

    /* MACON4 flags */
    #define MACON4_NOBKOFF  4 /* No Backoff Enable bit (applies to half duplex only) */
    #define MACON4_BPEN     5 /* No Backoff During Backpressure Enable bit (applies to half duplex only) */
    #define MACON4_DEFER    6 /* Defer Transmission Enable bit (applies to half duplex only) */

#define MABBIPG     0x04 /* MAC Back-To-Back Inter-Packet Gap Register */
#define MAIPGL      0x06 /* Non-Back-to-Back Inter-Packet Gap Low Byte (MAIPGL<6:0>) */
#define MAIPGH      0x07 /* Non-Back-to-Back Inter-Packet Gap High Byte (MAIPGH<6:0>) */
#define MACLCON1    0x08 /* Retransmission Maximum (RETMAX<3:0>) */
#define MACLCON2    0x09 /* Collision Window (COLWIN<5:0>) */
#define MAMXFLL     0x0A /* Maximum Frame Length Low Byte (MAMXFL<7:0>) */
#define MAMXFLH     0x0B /* Maximum Frame Length High Byte (MAMXFL<15:8>) */
#define MICMD       0x12 /* MII Command Register */

    /* MICMD flags */
    #define MICMD_MIIRD     0 /* MII Read Enable bit */
    #define MICMD_MIISCAN   1 /* MII Scan Enable bit */

#define MIREGADR    0x14 /* MII Register Address (MIREGADR<4:0>) */
#define MIWRL       0x16 /* MII Write Data Low Byte (MIWR<7:0>) */
#define MIWRH       0x17 /* MII Write Data High Byte (MIWR<15:8>) */
#define MIRDL       0x18 /* MII Read Data Low Byte (MIRD<7:0>) */
#define MIRDH       0x19 /* MII Read Data High Byte(MIRD<15:8>) */

/* Bank 3 Register (8-bit) */
#define MAADR5      0x00 /* MAC Address Byte 5 (MAADR<15:8>) */
#define MAADR6      0x01 /* MAC Address Byte 6 (MAADR<7:0>) */
#define MAADR3      0x02 /* MAC Address Byte 3 (MAADR<31:24>), OUI Byte 3 */
#define MAADR4      0x03 /* MAC Address Byte 4 (MAADR<23:16>) */
#define MAADR1      0x04 /* MAC Address Byte 1 (MAADR<47:40>), OUI Byte 1 */
#define MAADR2      0x05 /* MAC Address Byte 2 (MAADR<39:32>), OUI Byte 2 */
#define EBSTSD      0x06 /* Built-in Self-Test Fill Seed (EBSTSD<7:0>) */
#define EBSTCON     0x07 /* Ethernet Self-Test Control Register */

    /* EBSTCON flags */
    #define EBSTCON_BISTST  0 /* Built-in Self-Test Start/Busy bit */
    #define EBSTCON_TME     1 /* Test Mode Enable bit */
    #define EBSTCON_TMSEL0  2 /* Test Mode Select bit 0 */
    #define EBSTCON_TMSEL1  3 /* Test Mode Select bit 1 */
    #define EBSTCON_PSEL    4 /* Port Select bit */
    #define EBSTCON_PSV0    5 /* Pattern Shift Value bit 0 */
    #define EBSTCON_PSV1    6 /* Pattern Shift Value bit 0 */
    #define EBSTCON_PSV2    7 /* Pattern Shift Value bit 0 */

#define EBSTCSL     0x08 /* Built-in Self-Test Checksum Low Byte (EBSTCS<7:0>) */
#define EBSTCSH     0x09 /* Built-in Self-Test Checksum High Byte (EBSTCS<15:8>) */
#define MISTAT      0x0A /* MII Status Register */

    /* MISTAT flags */
    #define MISTAT_BUSY     0 /* MII Management Busy bit */
    #define MISTAT_SCAN     1 /* MII Management Scan Operation bit */
    #define MISTAT_NVALID   2 /* MII Management Read Data Not Valid bit */

#define EREVID      0x12 /* Ethernet Revision ID (EREVID<4:0>) */
#define ECOCON      0x15 /* Clock Output Control Register */

    /* ECOCON flags */
    #define ECOCON_COCON0   0 /* Clock Output Configuration bit 0 */
    #define ECOCON_COCON1   1 /* Clock Output Configuration bit 1 */
    #define ECOCON_COCON2   2 /* Clock Output Configuration bit 2 */

#define EFLOCON     0x17 /* Ethernet Flow Control Register */

    /* EFLOCON flags */
    #define EFLOCON_FCEN0   0 /* Flow Control Enable bit 0 */
    #define EFLOCON_FCEN1   1 /* Flow Control Enable bit 1 */
    #define EFLOCON_FULDPXS 2 /* Read-Only MAC Full-Duplex Shadow bit */

#define EPAUSL      0x18 /* Pause Timer Value Low Byte (EPAUS<7:0>) */
#define EPAUSH      0x19 /* Pause Timer Value High Byte (EPAUS<15:8>) */

/* PHY Register (16-bit) */
#define PHCON1      0x00 /* PHY Control Register 1 */

    /* PHCON1 flags */
    #define PHCON1_PDPXMD   8 /* PHY Duplex Mode bit */
    #define PHCON1_PPWRSV   11 /* PHY Power-Down bit */
    #define PHCON1_PLOOPBK  14 /* PHY Loopback bit */
    #define PHCON1_PRST     15 /* PHY Software Reset bit */

#define PHSTAT1     0x01 /* Physical Layer Status Register 1 */

    /* PHSTAT1 flags */
    #define PHSTAT1_JBSTAT  1 /* PHY Latching Jabber Status bit */
    #define PHSTAT1_LLSTAT  2 /* PHY Latching Link Status bit */
    #define PHSTAT1_PHDPX   11 /* PHY Half-Duplex Capable bit */
    #define PHSTAT1_PFDPX   12 /* PHY Full-Duplex Capable bit */

#define PHID1       0x02 /* PHY ID Register 1 */
#define PHID2       0x03 /*  PHY ID Register 2 */
#define PHCON2      0x10 /* PHY Control Register 2 */

    /* PHCON2 flags */
    #define PHCON2_HDLDIS   8 /* PHY Half-Duplex Loopback Disable bit */
    #define PHCON2_JABBER   10 /* Jabber Correction Disable bit */
    #define PHCON2_TXDIS    13 /* Twisted-Pair Transmitter Disable bit */
    #define PHCON2_FRCLNK   14 /* PHY Force Linkup bit */

#define PHSTAT2     0x11 /* Physical Layer Status Register 2 */

    /* PHSTAT2 flags */
    #define PHSTAT2_PLRITY  5 /* Polarity Status bit */
    #define PHSTAT2_DPXSTAT 9 /* PHY Duplex Status bit */
    #define PHSTAT2_LSTAT   10 /* PHY Link Status bit (non-latching) */
    #define PHSTAT2_COLSTAT 11 /* PHY Collision Status bit */
    #define PHSTAT2_RXSTAT  12 /* PHY Receive Status bit */
    #define PHSTAT2_TXSTAT  13 /* PHY Transmit Status bit */

#define PHIE        0x12 /* PHY Interrupt Enable Register */

    /* PHIE flags */
    #define PHIE_PGEIE      1 /* PHY Global Interrupt Enable bit */
    #define PHIE_PLNKIE     4 /* PHY Link Change Interrupt Enable bit */

#define PHIR        0x13 /* PHY Interrupt Request (Flag) Register */

    /* PHIR flags */
    #define PHIR_PGIF       2 /* PHY Global Interrupt Flag bit */
    #define PHIR_PLNKIF     4 /* PHY Link Change Interrupt Flag bit */

#define PHLCON      0x14 /* PHY LED Control Register */

    /* PHLCON flags */
    #define PHLCON_STRCH    1 /* LED Pulse Stretching Enable bit */
    #define PHLCON_LFRQ0    2 /* LED Pulse Stretch Time Configuration bit 0 */
    #define PHLCON_LFRQ1    3 /* LED Pulse Stretch Time Configuration bit 1 */
    #define PHLCON_LBCFG0   4 /* LEDB Configuration bit 0 */
    #define PHLCON_LBCFG1   5 /* LEDB Configuration bit 1 */
    #define PHLCON_LBCFG2   6 /* LEDB Configuration bit 2 */
    #define PHLCON_LBCFG3   7 /* LEDB Configuration bit 3 */
    #define PHLCON_LACFG0   8 /* LEDA Configuration bit 0 */
    #define PHLCON_LACFG1   9 /* LEDA Configuration bit 1 */
    #define PHLCON_LACFG2   10 /* LEDA Configuration bit 2 */
    #define PHLCON_LACFG3   11 /* LEDA Configuration bit 3 */

/* Transmit control byte flags */
#define POVERRIDE   0 /* Per packet override bit */
#define PCRCEN      1 /* Per packet CRC enable bit */
#define PPADEN      2 /* Per packet padding enable bit */
#define PHUGEEN     3 /* Per packet huge frame enable bit */

/* Transmit status vector */
#define TSV_BCNTL   0 /* Transmit Byte Count Low Byte */
#define TSV_BCNTH   1 /* Transmit Byte Count High Byte */
#define TSV_BYTE2   2

    #define TSV_COLCNT_MASK     0x0F /* Transmit Collision Count Mask */
    #define TSV_CRCERR          4 /* Transmit CRC Error */
    #define TSV_LENCHK          5 /* Transmit Length Check Error */
    #define TSV_LENOOR          6 /* Transmit Length Out of Range */
    #define TSV_DONE            7 /* Transmit Done */

#define TSV_BYTE3   3

    #define TSV_MULTICAST       0 /* Transmit Multicast */
    #define TSV_BROADCAST       1 /* Transmit Broadcast */
    #define TSV_PKTDEFER        2 /* Transmit Packet Defer */
    #define TSV_EXCDEFER        3 /* Transmit Excessive Defer */
    #define TSV_EXCCOLLI        4 /* Transmit Excessive Collision */
    #define TSV_LATECOLL        5 /* Transmit Late Collision */
    #define TSV_GIANT           6 /* Transmit Giant */
    #define TSV_UNDERRUN        7 /* Transmit Underrun */

#define TSV_TB0     4 /* Total Bytes Transmitted on Wire Byte0 LSB */
#define TSV_TB1     5 /* Total Bytes Transmitted on Wire Byte1 */
#define TSV_TB2     6 /* Total Bytes Transmitted on Wire Byte2 */
#define TSV_TB3     7 /* Total Bytes Transmitted on Wire Byte3 MSB */
#define TSV_BYTE8   8

    #define TSV_CTRL            0 /* Transmit Control Frame */
    #define TSV_PAUSE           1 /* Transmit Pause Control Frame */
    #define TSV_BACKPRE         2 /* Backpressure Applied */
    #define TSV_VLAN            3 /* Transmit VLAN Tagged Frame */

/* Receive status vector */
#define RSV_BCNTL   0 /* Received Byte Count Low */
#define RSV_BCNTH   1 /* Received Byte Count High */
#define RSV_BYTE2   2

    #define RSV_LONGDROP        0 /* Long Event/Drop Event */
    #define RSV_CARRIER         2 /* Carrier Event Previously Seen */
    #define RSV_CRCERR          4 /* CRC Error */
    #define RSV_LENCHK          5 /* Length Check Error */
    #define RSV_LENOOR          6 /* Receive Multicast Packet */
    #define RSV_OK              7 /* Received Ok */
    
#define RSV_BYTE3   3

    #define RSV_MULTICAST       0 /* Receive Multicast Packet */
    #define RSV_BROADCAST       1 /* Receive Broadcast Packet */
    #define RSV_DRIBBLE         2 /* Dribble Nibble */
    #define RSV_CTRL            3 /* Receive Control Frame */
    #define RSV_PAUSE           4 /* Receive Pause Control Frame */
    #define RSV_UNKNOWN         5 /* Receive Unknown Opcode */
    #define RSV_VLAN            6 /* Receive VLAN Type Detected */

int init_done = 0;
int error = ERR_NOERR;

struct enc28j60_regs regs_cfg;
mac_addr_t mac;
nic_stats_t stats;

uint16_t ptr_pkg_next;

static int select_bank(uint8_t bank)
{
    uint8_t send[2];
    uint8_t tmp;
    
    /* read ECON1 register */
    send[0] = SPI_RCR | ECON1;
    ENC28J60_CS_ENABLE;
    spi_master_send(&send[0], 1);
    spi_master_recv(&tmp, 1);
    _delay_us(1);
    ENC28J60_CS_DISABL;
    
    /* select bank in ECON1 register */
    send[0] = SPI_WCR | ECON1;
    tmp &= 0xFC;
    
    switch (bank) {
    case BANK0:
        break;
    case BANK1:
        tmp |= (1 << ECON1_BSEL0);
        break;
    case BANK2:
        tmp |= (1 << ECON1_BSEL1);
        break;
    case BANK3:
        tmp |= (1 << ECON1_BSEL0) | (1 << ECON1_BSEL1);
        break;
    default:
        error = ERR_INTER;
        return -1;
    }
    
    send[1] = tmp;
    ENC28J60_CS_ENABLE;
    spi_master_send(send, 2);
    _delay_us(1);
    ENC28J60_CS_DISABL;
    return 0;
}

static int read_reg(uint8_t bank, uint8_t reg, uint8_t *val)
{
    uint8_t send;
    
    if (select_bank(bank) == -1) {
        error = ERR_INTER;
        return -1;
    }
    
    if (!val) {
        error = ERR_INTER;
        return -1;
    }
    
    send = SPI_RCR | reg;
    ENC28J60_CS_ENABLE;
    spi_master_send(&send, 1);
    spi_master_recv(val, 1);
    _delay_us(1);
    ENC28J60_CS_DISABL;
    return 0;
}

static int read_mii_mac_reg(uint8_t bank, uint8_t reg, uint8_t *val)
{
    uint8_t send;
    uint8_t recv[2];
    
    if (select_bank(bank) == -1) {
        error = ERR_INTER;
        return -1;
    }
    
    if (!val) {
        error = ERR_INTER;
        return -1;
    }
    
    send = SPI_RCR | reg;
    ENC28J60_CS_ENABLE;
    spi_master_send(&send, 1);
    spi_master_recv(recv, 2);
    _delay_us(1);
    ENC28J60_CS_DISABL;
    (*val) = recv[1];
    return 0;
}

static int write_reg(uint8_t bank, uint8_t reg, uint8_t val)
{
    uint8_t send[2];
    
    if (select_bank(bank) == -1) {
        error = ERR_INTER;
        return -1;
    }
    
    send[0] = SPI_WCR | reg;
    send[1] = val;
    ENC28J60_CS_ENABLE;
    spi_master_send(send, 2);
    _delay_us(1);
    ENC28J60_CS_DISABL;
    return 0;
}

static int read_phy_reg(uint8_t reg, uint16_t *val)
{
    uint8_t tmp;
    int timeout = TIMEOUT_CNT;
    
    if (!val) {
        error = ERR_INTER;
        return -1;
    }
    
    if (write_reg(BANK2, MIREGADR, reg) == -1)
        return -1;
    
    if (write_reg(BANK2, MICMD, (1 << MICMD_MIIRD)) == -1)
        return -1;
    
    if (read_mii_mac_reg(BANK3, MISTAT, &tmp) == -1)
        return -1;
    
    while (_ISSET(tmp, MISTAT_BUSY)) {
        if (read_mii_mac_reg(BANK3, MISTAT, &tmp))
            return -1;
        
        _delay_us(10);
        timeout--;
        
        if (timeout == 0) {
            error = ERR_INTER;
            return -1;
        }
    }
    
    if (write_reg(BANK2, MICMD, 0x00) == -1)
        return -1;
    
    if (read_mii_mac_reg(BANK2, MIRDH, &tmp) == -1)
        return -1;
    
    (*val) |= ((uint16_t) tmp << 8);
    read_mii_mac_reg(BANK2, MIRDL, &tmp);
    (*val) |= (uint16_t) tmp;
    return 0;
}

static int write_phy_reg(uint8_t reg, uint16_t val)
{
    uint8_t tmp;
    int timeout = TIMEOUT_CNT;
    
    if (write_reg(BANK2, MIREGADR, reg) == -1)
        return -1;
    
    if (write_reg(BANK2, MIWRL, _LOW(val)) == -1)
        return -1;
    
    if (write_reg(BANK2, MIWRH, _HIGH(val)) == -1)
        return -1;
    
    if (read_mii_mac_reg(BANK3, MISTAT, &tmp) == -1)
        return -1;
    
    while (_ISSET(tmp, MISTAT_BUSY)) {
        if (read_mii_mac_reg(BANK3, MISTAT, &tmp) == -1)
            return -1;
        
        _delay_us(10);
        timeout--;
        
        if (timeout == 0) {
            error = ERR_INTER;
            return -1;
        }
    }
    
    return 0;
}

static int read_buffer(uint16_t addr, uint8_t *buf, int len)
{
    uint8_t send;
    
    if (addr > (BUF_SIZE - len)) {
        error = ERR_INTER;
        return -1;
    }
    
    if (!buf) {
        error = ERR_INTER;
        return -1;
    }
    
    if (len > BUF_SIZE) {
        error = ERR_INTER;
        return -1;
    }
    
    if (write_reg(BANK0, ERDPTL, _LOW(addr)) == -1)
        return -1;
    
    if (write_reg(BANK0, ERDPTH, _HIGH(addr)) == -1)
        return -1;
    
    send = SPI_RBM;
    ENC28J60_CS_ENABLE;
    spi_master_send(&send, 1);
    spi_master_recv(buf, len);
    _delay_us(1);
    ENC28J60_CS_DISABL;
    return 0;
}

static int write_buffer(uint16_t addr, uint8_t *buf, int len)
{
    uint8_t send;
    
    if (addr > (BUF_SIZE - len)) {
        error = ERR_INTER;
        return -1;
    }
    
    if (!buf) {
        error = ERR_INTER;
        return -1;
    }
    
    if (len > BUF_SIZE) {
        error = ERR_INTER;
        return -1;
    }
    
    if (write_reg(BANK0, EWRPTL, _LOW(addr)) == -1)
        return -1;
    
    if (write_reg(BANK0, EWRPTH, _HIGH(addr)) == -1)
        return -1;
    
    send = SPI_WBM;
    ENC28J60_CS_ENABLE;
    spi_master_send(&send, 1);
    spi_master_send(buf, len);
    _delay_us(1);
    ENC28J60_CS_DISABL;
    return 0;
}

static int free_rx_memory(void)
{
    uint8_t tmp;
    
    if (read_reg(BANK0, ECON2, &tmp) == -1)
        return -1;
    
    tmp |= (1 << ECON2_PKTDEC);
    
    if (ptr_pkg_next == BUF_RX_START) {
        if (write_reg(BANK0, ERXRDPTL, _LOW(BUF_RX_END)) == -1)
            return -1;
        
        if (write_reg(BANK0, ERXRDPTH, _HIGH(BUF_RX_END)) == -1)
            return -1;
    } else {
        if (write_reg(BANK0, ERXRDPTL, _LOW(ptr_pkg_next - 1)) == -1)
            return -1;
        
        if (write_reg(BANK0, ERXRDPTH, _HIGH(ptr_pkg_next - 1)) == -1)
            return -1;
    }
    
    if (write_reg(BANK0, ECON2, tmp) == -1)
        return -1;
    
    return 0;
}

int enc28j60_init(int mode, mac_addr_t *addr)
{
    uint8_t tmp;
    int timeout = TIMEOUT_CNT;
    
    error = ERR_NOERR;
    
    if (!addr) {
        error = ERR_INVAL;
        return -1;
    }
    
    if (!init_done) {
        regs_cfg.mode = mode;
        regs_cfg.erxstl = _LOW(BUF_RX_START);
        regs_cfg.erxsth = _HIGH(BUF_RX_START);
        regs_cfg.erxrdptl = _LOW(BUF_RX_END);
        regs_cfg.erxrdpth = _HIGH(BUF_RX_END);
        regs_cfg.erxndl = _LOW(BUF_RX_END);
        regs_cfg.erxndh = _HIGH(BUF_RX_END);
        regs_cfg.etxstl = _LOW(BUF_TX_START);
        regs_cfg.etxsth = _HIGH(BUF_TX_START);
        regs_cfg.erxfcon = ((1 << ERXFCON_CRCEN) | 
                            (1 << ERXFCON_MCEN) | 
                            (1 << ERXFCON_UCEN) | 
                            (1 << ERXFCON_BCEN));
        regs_cfg.macon1 = ((1 << MACON1_MARXEN) | 
                           (1 << MACON1_TXPAUS) | 
                           (1 << MACON1_RXPAUS));
        
        if (regs_cfg.mode == MODE_FDPX) {
            regs_cfg.macon3 = ((1 << MACON3_FULDPX) | 
                               (1 << MACON3_FRMLNEN) | 
                               (1 << MACON3_TXCRCEN) | 
                               (1 << MACON3_PADCFG0));
            regs_cfg.mabbipg = 0x15;
            regs_cfg.maipgh = 0x00;
            regs_cfg.phcon1 = (1 << PHCON1_PDPXMD);
            
        } else {
            regs_cfg.macon3 = ((1 << MACON3_FRMLNEN) | 
                               (1 << MACON3_TXCRCEN) | 
                               (1 << MACON3_PADCFG0));
            regs_cfg.mabbipg = 0x12;
            regs_cfg.maipgh = 0x0C;
            regs_cfg.phcon1 = 0x0000;
        }
        
        regs_cfg.maipgl = 0x12;
        regs_cfg.macon4 = (1 << MACON4_DEFER);
        regs_cfg.econ1 = (1 << ECON1_RXEN);
        regs_cfg.econ2 = (1 << ECON2_AUTOINC);
        regs_cfg.mamxfll = _LOW(MAX_FRAME_SIZE);
        regs_cfg.mamxflh = _HIGH(MAX_FRAME_SIZE);
        
        stats.tx_frm = 0;
        stats.rx_frm = 0;
        stats.tx_byt = 0;
        stats.rx_byt = 0;
        stats.tx_err = 0;
        stats.rx_err = 0;
    }
    
    ethernet_addr_cpy(&mac, addr); 
    ptr_pkg_next = BUF_RX_START;
    ENC28J60_RS_CONFIG;
    ENC28J60_RS_HIGH;
    
    /* Init SPI interface */
    ENC28J60_CS_CONFIG;
    spi_master_init(SPI_MODE_0, SPI_FOSC_2, SPI_ORDER_MSB);
    
    /* Soft reset controller */
    tmp = SPI_SRC;
    spi_master_send(&tmp, 1);
    _delay_ms(2);

    /* Receive Buffer */
    if (write_reg(BANK0, ERXSTL, regs_cfg.erxstl) == -1)
        return -1;
    
    if (write_reg(BANK0, ERXSTH, regs_cfg.erxsth) == -1)
        return -1;
    
    if (write_reg(BANK0, ERXRDPTL, regs_cfg.erxrdptl) == -1)
        return -1;
    
    if (write_reg(BANK0, ERXRDPTH, regs_cfg.erxrdpth) == -1)
        return -1;
    
    if (write_reg(BANK0, ERXNDL, regs_cfg.erxndl) == -1)
        return -1;
    
    if (write_reg(BANK0, ERXNDH, regs_cfg.erxndh) == -1)
        return -1;
    
    /* Transmit Buffer */
    if (write_reg(BANK0, ETXSTL, regs_cfg.etxstl) == -1)
        return -1;
    
    if (write_reg(BANK0, ETXSTH, regs_cfg.etxsth) == -1)
        return -1;
    
    /* Receive Filter */
    if (write_reg(BANK1, ERXFCON, regs_cfg.erxfcon) == -1)
        return -1;
    
    /* Waiting for OST */
    if (read_reg(BANK0, ESTAT, &tmp) == -1)
        return -1;
    
    while (_ISCLR(tmp, ESTAT_CLKRDY)) {
        if (read_reg(BANK0, ESTAT, &tmp) == -1)
            return -1;
        
        _delay_us(10);
        timeout--;
        
        if (timeout == 0) {
            error = ERR_TIMEO;
            return -1;
        }
    }
    
    /* MAC Initialization Settings */
    if (write_reg(BANK2, MACON1, regs_cfg.macon1) == -1)
        return -1;
    
    if (write_reg(BANK2, MACON3, regs_cfg.macon3) == -1)
            return -1;
    
    if (write_reg(BANK2, MACON4, regs_cfg.macon4) == -1)
            return -1;
    
    if (write_reg(BANK2, MAMXFLL, regs_cfg.mamxfll) == -1)
        return -1;
    
    if (write_reg(BANK2, MAMXFLH, regs_cfg.mamxflh) == -1)
        return -1;
    

    if (write_reg(BANK2, MABBIPG, regs_cfg.mabbipg) == -1)
        return -1;
 
    
    if (write_reg(BANK2, MAIPGL, regs_cfg.maipgl) == -1)
        return -1;
    
    if (regs_cfg.mode == MODE_HDPX) {
        if (write_reg(BANK2, MAIPGH, regs_cfg.maipgh) == -1)
            return -1;
    }
    
    if (enc28j60_set_mac(&mac) == -1)
        return -1;
    
    /* PHY Initialization Settings */
    if (write_phy_reg(PHCON1, regs_cfg.phcon1) == -1)
        return -1;
    
    /* Enable increment */
    if (write_reg(BANK0, ECON2, regs_cfg.econ2) == -1)
        return -1;
    
    /* Enable receiving packets */
    if (write_reg(BANK0, ECON1, regs_cfg.econ1) == -1)
        return -1;
    
    init_done = 1;
    return 0;
}

int enc28j60_send(eth_frame_t *frame)
{
    int ret;
    int frm_len;
    int timeout = TIMEOUT_CNT;
    uint8_t *p;
    uint8_t tmp = 0x00;
    uint8_t tsv[7];
    
    if (!frame) {
        error = ERR_INVAL;
        return -1;
    }
    
    frm_len = ethernet_frame_get_len(frame);
    
    if (frm_len == -1) {
        error = ERR_ETLIB;
        return -1;
    }
    
    if (frm_len > MAX_FRAME_SIZE) {
        stats.tx_err++;
        error = ERR_FRMTB;
        return -1;
    }
    
    p = (uint8_t *) malloc(frm_len);
    
    if (!p) {
        error = ERR_NOMEM;
        return -1;
    }
    
    if (ethernet_frm_to_buf(frame, p) == -1) {
        error = ERR_ETLIB;
        free(p);
        return -1;
    }
    
    if (write_buffer(BUF_TX_START, &tmp, 1) == -1) {
        free(p);
        return -1;
    }
    
    if (write_buffer((BUF_TX_START + 1), p, frm_len) == -1) {
        free(p);
        return -1;
    }
    
    free(p);
    
    if (write_reg(BANK0, ETXNDL, _LOW((BUF_TX_START + frm_len))) == -1)
        return -1;
    
    if (write_reg(BANK0, ETXNDH, _HIGH((BUF_TX_START + frm_len))) == -1)
        return -1;
    
    /* transmit packet */
    if (read_reg(BANK0, ECON1, &tmp) == -1)
        return -1;
    
    tmp |= (1 << ECON1_TXRTS);
    
    if (write_reg(BANK0, ECON1, tmp) == -1)
        return -1;
    
    if (read_reg(BANK0, ECON1, &tmp) == -1)
        return -1;
    
    while (_ISSET(tmp, ECON1_TXRTS)) {
        read_reg(BANK0, ECON1, &tmp);
        _delay_us(10);
        timeout--;
        
        if (timeout == 0) {
            error = ERR_TIMEO;
            return -1;
        }
    }

    if (read_reg(BANK0, ESTAT, &tmp))
        return -1;
    
    if (_ISSET(tmp, ESTAT_TXABRT)) {
        stats.tx_err++;
        error = ERR_TXABT;
        return -1;
    }
    
    if (read_buffer((BUF_TX_START + frm_len + 1), tsv, 7) == -1)
        return -1;
    
    if (_ISCLR(tsv[TSV_BYTE2], TSV_DONE)) {
        stats.tx_err++;
        error = ERR_TXERR;
        return -1;
    }
    
    ret = (uint16_t) tsv[TSV_BCNTL];
    ret |= ((uint16_t) tsv[TSV_BCNTH] << 8);
    stats.tx_frm++;
    stats.tx_byt += ret;
    return ret;
}

int enc28j60_recv(eth_frame_t *frame)
{
    uint16_t ptr_pkg_start;
    uint16_t ptr_frm_start;
    uint16_t ptr_rsv_start;
    int frm_len_ptr;
    int frm_len_warp;
    int frm_len_rsv;
    int rsv_len_warp;
    uint8_t rsv[4];
    uint8_t *p;
    uint8_t tmp;
    
    if (!frame) {
        error = ERR_INVAL;
        return -1;
    }
    
    if (read_reg(BANK1, EPKTCNT, &tmp) == -1)
        return -1;
    
    if (tmp < 1)
        return 0;
    
    frm_len_warp = -1;
    ptr_pkg_start = ptr_pkg_next;
    
    if (read_buffer(ptr_pkg_start, rsv, BUF_PTR_SIZE) == -1)
        return -1;
    
    ptr_pkg_next = (uint16_t) rsv[BUF_PTR_LO];
    ptr_pkg_next |= ((uint16_t)rsv[BUF_PTR_HI] << 8);
    
    if (ptr_pkg_start < ptr_pkg_next) {
        ptr_rsv_start = ptr_pkg_start + 2;
        ptr_frm_start = ptr_rsv_start + 4;
        
        if (read_buffer(ptr_rsv_start, rsv, 4) == -1)
            return -1;
        
        frm_len_ptr = ptr_pkg_next - ptr_frm_start;
    } else {
        if ((ptr_pkg_start + 6) > BUF_RX_END) {
            ptr_rsv_start = ptr_pkg_start + 2;
            rsv_len_warp = BUF_RX_END - ptr_rsv_start + 1;
            ptr_frm_start = (4 - rsv_len_warp) - BUF_RX_START;
            frm_len_ptr = ptr_pkg_next - ptr_frm_start;
            
            if (read_buffer(ptr_rsv_start, rsv, rsv_len_warp) == -1)
                return -1;
            
            if (read_buffer(BUF_RX_START, &rsv[rsv_len_warp], 
                (4 - rsv_len_warp)) == -1)
                return -1;
        } else if ((ptr_pkg_start + 2) > BUF_RX_END) {
            ptr_rsv_start = BUF_RX_START;
            ptr_frm_start = ptr_rsv_start + 4;
            
            if (read_buffer(ptr_rsv_start, rsv, 4) == -1)
                return -1;
            
            frm_len_ptr = ptr_pkg_next - ptr_frm_start;
        } else {
            ptr_rsv_start = ptr_pkg_start + 2;
            ptr_frm_start = ptr_rsv_start + 4;
            frm_len_warp = BUF_RX_END - ptr_frm_start + 1;
            frm_len_ptr = frm_len_warp + ptr_pkg_next - BUF_RX_START;
            
            if (read_buffer(ptr_rsv_start, rsv, 4) == -1)
                return -1;
        }
    }
    
    frm_len_rsv = (uint16_t) rsv[RSV_BCNTL];
    frm_len_rsv |= ((uint16_t) rsv[RSV_BCNTH] << 8);
    
    if (_ISCLR(rsv[RSV_BYTE2], RSV_OK)) {
        stats.rx_err++;
        
        if (free_rx_memory() == -1)
            return -1;
        
        error = ERR_FRMIN;
        return -1;
    }
    
    if (frm_len_rsv & 1) {
        if ((frm_len_ptr - 1) != frm_len_rsv) {
            stats.rx_err++;
            error = ERR_INTER;
            return -1;
        }
    } else {
        if (frm_len_ptr != frm_len_rsv) {
            stats.rx_err++;
            error = ERR_INTER;
            return -1;
        }
    }
    
    p = (uint8_t *) malloc(frm_len_rsv);
    
    if (!p) {
        error = ERR_NOMEM;
        stats.rx_err++;
        return -1;
    }
    
    if (frm_len_warp != -1) {
        if (read_buffer(ptr_frm_start, p, frm_len_warp) == -1) {
            free(p);
            return -1;
        }
        
        if (read_buffer(BUF_RX_START, &p[frm_len_warp], frm_len_rsv - frm_len_warp) == -1) {
            free(p);
            return -1;
        }
    } else {
        if (read_buffer(ptr_frm_start, p, frm_len_rsv) == -1) {
            free(p);
            return -1;
        }
    }
    
    if (ethernet_buf_to_frm(p, frm_len_rsv - 4, frame) == -1) {
        error = ERR_ETLIB;
        stats.rx_err++;
        free(p);
        return -1;
    }
    
    free(p);
    
    if (free_rx_memory() == -1)
        return -1;
    
    stats.rx_frm++;
    stats.rx_byt += (frm_len_rsv - 4);
    return (frm_len_rsv - 4);
}

int enc28j60_set_mac(mac_addr_t *addr)
{
    if (!addr) {
        error = ERR_INVAL;
        return -1;
    }
    
    if (write_reg(BANK3, MAADR1, addr->ma_byte0) == -1)
        return -1;
    
    if (write_reg(BANK3, MAADR2, addr->ma_byte1) == -1)
        return -1;
    
    if (write_reg(BANK3, MAADR3, addr->ma_byte2) == -1)
        return -1;
    
    if (write_reg(BANK3, MAADR4, addr->ma_byte3) == -1)
        return -1;
    
    if (write_reg(BANK3, MAADR5, addr->ma_byte4) == -1)
        return -1;
    
    if (write_reg(BANK3, MAADR6, addr->ma_byte5) == -1)
        return -1;
    
    return 0;
}

int enc28j60_get_mac(mac_addr_t *addr)
{
    uint8_t tmp;
    
    if (!addr) {
        error = ERR_INVAL;
        return -1;
    }
    
    if (read_mii_mac_reg(BANK3, MAADR1, &tmp) == -1)
        return -1;
    
    addr->ma_byte0 = tmp;
    
    if (read_mii_mac_reg(BANK3, MAADR2, &tmp) == -1)
        return -1;
    
    addr->ma_byte1 = tmp;
    
    if (read_mii_mac_reg(BANK3, MAADR3, &tmp) == -1)
        return -1;
    
    addr->ma_byte2 = tmp;
    
    if (read_mii_mac_reg(BANK3, MAADR4, &tmp) == -1)
        return -1;
    
    addr->ma_byte3 = tmp;
    if (read_mii_mac_reg(BANK3, MAADR5, &tmp) == -1)
        return -1;
    
    addr->ma_byte4 = tmp;
    
    if (read_mii_mac_reg(BANK3, MAADR6, &tmp) == -1)
        return -1;
    
    addr->ma_byte5 = tmp;
    return 0;
}

int enc28j60_is_link_up(void)
{
    uint16_t l = 0;
    
    if (read_phy_reg(PHSTAT2, &l) == -1)
        return -1;
    
    if (_ISSET(l, PHSTAT2_LSTAT))
        return 1;
    else
        return 0;
}

int enc28j60_get_free_rx_space(void)
{
    uint8_t tmp;
    uint16_t p, rdp, wrp;
    
    if (read_reg(BANK0, ERXWRPTL, &tmp) == -1)
        return -1;
    
    wrp = tmp;
    
    if (read_reg(BANK0, ERXWRPTH, &tmp) == -1)
        return -1;
    
    p = tmp;
    wrp |= (p << 8);
    
    if (read_reg(BANK0, ERXRDPTL, &tmp) == -1)
        return -1;
    
    rdp = tmp;
    
    if (read_reg(BANK0, ERXRDPTH, &tmp) == -1)
        return -1;
    
    p = tmp;
    rdp |= (p << 8);
    
    if (wrp > rdp)
        return ((BUF_RX_END - BUF_RX_START) - (wrp - rdp));
    else if (wrp == rdp)
        return (BUF_RX_END - BUF_RX_START);
    
    return (rdp - wrp - 1);
}

char *enc28j60_get_ver(void)
{
    return VERSION;
}

char *enc28j60_get_rev(void)
{
    uint8_t tmp;
      
    if (read_reg(BANK3, EREVID, &tmp))
        error = -1;
    
    switch (tmp) {
    case 0x02:
        return "B1";
    case 0x04:
        return "B4";
    case 0x05:
        return "B5";
    case 0x06:
        return "B7";
    default:
        return "UK";
    }
}

int enc28j60_get_last_error(void)
{
    int err;
    
    err = error;
    error = ERR_NOERR;
    return err;
}

nic_stats_t enc28j60_get_stats(void)
{
    return stats;
}

struct enc28j60_regs enc28j60_dump_regs(void)
{
    struct enc28j60_regs regs;
    
    read_reg(BANK0, EIE, &regs.eie);
    read_reg(BANK0, EIR, &regs.eir);
    read_reg(BANK0, ESTAT, &regs.estat);
    read_reg(BANK0, ECON1, &regs.econ1);
    read_reg(BANK0, ECON2, &regs.econ2);
    read_reg(BANK0, ERXSTL, &regs.erxstl);
    read_reg(BANK0, ERXSTH, &regs.erxsth);
    read_reg(BANK0, ERXRDPTL, &regs.erxrdptl);
    read_reg(BANK0, ERXRDPTH, &regs.erxrdpth);
    read_reg(BANK0, ERXNDL, &regs.erxndl);
    read_reg(BANK0, ERXNDH, &regs.erxndh);
    read_reg(BANK0, ETXSTL, &regs.etxstl);
    read_reg(BANK0, ETXSTH, &regs.etxsth);
    read_reg(BANK1, ERXFCON, &regs.erxfcon);
    read_reg(BANK1, EPKTCNT, &regs.epktcnt);
    read_mii_mac_reg(BANK2, MACON1, &regs.macon1);
    read_mii_mac_reg(BANK2, MACON3, &regs.macon3);
    read_mii_mac_reg(BANK2, MACON4, &regs.macon4);
    read_mii_mac_reg(BANK2, MAMXFLL, &regs.mamxfll);
    read_mii_mac_reg(BANK2, MAMXFLH, &regs.mamxflh);
    read_mii_mac_reg(BANK2, MABBIPG, &regs.mabbipg);
    read_mii_mac_reg(BANK2, MAIPGL, &regs.maipgl);
    read_mii_mac_reg(BANK2, MAIPGH, &regs.maipgh);
    read_phy_reg(PHCON1, &regs.phcon1);
    return regs;
}

/**
 *
 * File Name: nic.c
 * Title    : Generic NIC wrapper library
 * Project  : lib-avr
 * Author   : Copyright (C) 2019 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-06-02
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

#include "nic.h"

#ifdef NIC_DEVICE_ENC28J60
#include "../spi/enc28j60.h"
#endif

static int error = NIC_ERROR_SUCCESS;

int nic_init(mac_addr_t *addr)
{
    int ret;
    
#ifdef NIC_DEVICE_ENC28J60
    ret = enc28j60_init(ENC28J60_MODE_FDPX, addr);
    
    if (ret == -1) {
        switch (enc28j60_get_last_error()) {
        case ENC28J60_ERR_INVAL:
            error = NIC_ERROR_INVAL;
            break;
        default:
            error = NIC_ERROR_DRIVER;
        }
    }
#endif
    
    return ret;
}

int nic_is_link_up(void)
{
    int ret;
    
#ifdef NIC_DEVICE_ENC28J60
    ret = enc28j60_is_link_up();
    
    if (ret == -1)
         error = NIC_ERROR_DRIVER;
#endif
    
    return ret;
}

int nic_send(eth_frame_t *frame)
{
    int ret;
    
#ifdef NIC_DEVICE_ENC28J60
    ret = enc28j60_send(frame);
    
    if (ret == -1) {
        switch (enc28j60_get_last_error()) {
        case ENC28J60_ERR_INVAL:
            error = NIC_ERROR_INVAL;
            break;
        case ENC28J60_ERR_NOMEM:
            error = NIC_ERROR_NOMEM;
            break;
        case ENC28J60_ERR_FRMTB:
            error = NIC_ERROR_TX_TOBIG;
            break;
        default:
            error = NIC_ERROR_DRIVER;
        }
    }
#endif
    
    return ret;
}

int nic_recv(eth_frame_t *frame)
{
    int ret;
    
#ifdef NIC_DEVICE_ENC28J60
    ret = enc28j60_recv(frame);
    
    if (ret == -1) {
        switch (enc28j60_get_last_error()) {
        case ENC28J60_ERR_INVAL:
            error = NIC_ERROR_INVAL;
            break;
        case ENC28J60_ERR_NOMEM:
            error = NIC_ERROR_NOMEM;
            break;
        case ENC28J60_ERR_ETLIB:
            error = NIC_ERROR_ETHLIB;
            break;
        case ENC28J60_ERR_RXCRC:
            error = NIC_ERROR_RX_CRC;
            break;
        default:
            error = NIC_ERROR_DRIVER;
        }
    }
#endif
    
    return ret;
}

char *nic_get_driver_name(void)
{
#ifdef NIC_DEVICE_ENC28J60
    return enc28j60_get_driver_name();
#endif
}

char *nic_get_driver_vers(void)
{
#ifdef NIC_DEVICE_ENC28J60
    return enc28j60_get_driver_vers();
#endif
}

nic_stats_t nic_get_stats(void)
{
    nic_stats_t ret;
    
#ifdef NIC_DEVICE_ENC28J60
    ret = enc28j60_get_stats();
#endif
    
    return ret;
}

int nic_get_last_error(void)
{
    int err;
    
    err = error;
    error = NIC_ERROR_SUCCESS;
    return err;
}

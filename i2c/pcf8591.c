/**
 *
 * File Name: pcf8591.c
 * Title    : NXP PCF8591 library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-12-03
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

#include "i2c.h"
#include "pcf8591.h"

#define PCF8591_ADDR        0x48

/* Channel select */
#define PCF8591_CTRL_CH0    0x00
#define PCF8591_CTRL_CH1    0x01
#define PCF8591_CTRL_CH2    0x02
#define PCF8591_CTRL_CH3    0x03

/* Auto increment bit */
#define PCF8591_CTRL_AINC   0x04

/* ADC Configuration */
#define PCF8591_CTRL_CFG0   0x00
#define PCF8591_CTRL_CFG1   0x10
#define PCF8591_CTRL_CFG2   0x20
#define PCF8591_CTRL_CFG3   0x30

/* DAC enable bit */
#define PCF8591_CTRL_DACE   0x40

void pcf8591_init(void)
{
    i2c_master_init(100000);
}

int pcf8591_get_adc(uint8_t subaddr, int config, int channel, uint8_t *value)
{
    uint8_t addr;
    uint8_t cmd;
    int ret;
    
    addr = PCF8591_ADDR | subaddr;
    
    switch (config) {
    case CONFIG_ADC0:
        cmd = PCF8591_CTRL_CFG0;
        break;
    case CONFIG_ADC1:
        cmd = PCF8591_CTRL_CFG1;
        break;
    case CONFIG_ADC2:
        cmd = PCF8591_CTRL_CFG2;
        break;
    case CONFIG_ADC3:
        cmd = PCF8591_CTRL_CFG3;
        break;
    default:
        return -1;
    }
    
    switch (channel) {
    case CHANNEL_ADC0:
        cmd |= PCF8591_CTRL_CH0;
        break;
    case CHANNEL_ADC1:
        cmd |= PCF8591_CTRL_CH1;
        break;
    case CHANNEL_ADC2:
        cmd |= PCF8591_CTRL_CH2;
        break;
    case CHANNEL_ADC3:
        cmd |= PCF8591_CTRL_CH3;
        break;
    default:
        return -1;
    }

    ret = i2c_master_recv(addr, I2C_OPT_NORMAL, &cmd, 1, value, 1);
    
    if (ret == -1)
        return -1;
    
    return 0;
}

int pcf8591_set_dac(uint8_t subaddr, uint8_t *value)
{
    uint8_t addr;
    uint8_t cmd;
    int ret;
    
    addr = PCF8591_ADDR | subaddr;
    cmd = PCF8591_CTRL_DACE;
    ret = i2c_master_send(addr, I2C_OPT_NORMAL, &cmd, 1, value, 1);
    
    if (ret == -1)
        return -1;
    
    return 0;
}

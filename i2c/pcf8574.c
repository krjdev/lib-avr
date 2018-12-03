/**
 *
 * File Name: pcf8574.c
 * Title    : I2C device NXP PCF8574(A) I/O port expander library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-09-22
 * Modified : 2018-12-03
 * Revised  : 
 * Version  : 0.1.1.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR Series
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdlib.h>

#include "i2c.h"
#include "pcf8574.h"

#define PCF8574_ADDR    0x20
#define PCF8574A_ADDR   0x38

int pcf8574_write(gpio_t *gpio, uint8_t data)
{
    int ret;
    uint8_t tmp = data;
    
    if (!gpio)
        return -1;
    
    ret = i2c_master_send(I2C_OPT_NORMAL, gpio->addr, NULL, 0, &tmp, 1);
    
    if (ret == -1)
        return -1;
    
    return 0;
}

int pcf8574_read(gpio_t *gpio, uint8_t *data)
{
    int ret;
    
    if (!gpio)
        return -1;
    
    ret = i2c_master_recv(I2C_OPT_NORMAL, gpio->addr, NULL, 0, data, 1);
    
    if (ret == -1)
        return -1;
    
    return 0;
}

gpio_t *pcf8574_init(int type, uint8_t subaddr)
{
    gpio_t *p;
    uint8_t addr;
    int i;
    int value;
    
    if (subaddr > 8)
        return NULL;
    
    switch (type) {
    case TYPE_PCF8574:
        addr = PCF8574_ADDR | subaddr;
        break;
    case TYPE_PCF8574A:
        addr = PCF8574A_ADDR | subaddr;
        break;
    default:
        return NULL;
    }
    
    p = (gpio_t *) malloc(sizeof(gpio_t));
    
    if (!p)
        return NULL;
    
    p->addr = addr;
    p->npins = NPINS;
    i2c_master_init(100000);
    
    /* set initial state of pins to input */
    pcf8574_write(p, 0xFF);
    
    for (i = 0; i < p->npins; i++) {
        p->pin_config[i] = GPIO_PIN_INPUT;
        pcf8574_get_pin(p, i, &value);
        p->pin_state[i] = value;
    }
    
    return p;
}

void pcf8574_free(gpio_t *gpio)
{
    if (gpio)
        free(gpio);
}

int pcf8574_set_config(gpio_t *gpio, int pin, int config)
{
    if (!gpio)
        return -1;
    
    if (pin >= gpio->npins)
        return -1;
    
    if (config != GPIO_PIN_INPUT || config != GPIO_PIN_OUTPUT)
        return -1;
    
    gpio->pin_config[pin] = config;
    
    if (config == GPIO_PIN_INPUT)
        pcf8574_set_pin(gpio, pin, GPIO_PIN_HIGH);
    
    return 0;
}

int pcf8574_get_config(gpio_t *gpio, int pin, int *config)
{
    if (!gpio)
        return -1;
    
    if (pin >= gpio->npins)
        return -1;
    
    (*config) = gpio->pin_config[pin];
    return 0;
}

int pcf8574_get_npins(gpio_t *gpio)
{
    if (!gpio)
        return -1;
    
    return gpio->npins;
}

int pcf8574_set_pin(gpio_t *gpio, int pin, int value)
{
    int i;
    uint8_t tmp = 0x00;
    
    if (!gpio)
        return -1;
    
    if (pin >= gpio->npins)
        return -1;
    
    for (i = 0; i < gpio->npins; i++)
		if (gpio->pin_state[i] == GPIO_PIN_LOW)
			tmp |= (1 << i);
        
    pcf8574_set_config(gpio, pin, GPIO_PIN_OUTPUT);
        
	if (value == GPIO_PIN_HIGH) {
		tmp &= ~(1 << pin);
		pcf8574_write(gpio, tmp);
		gpio->pin_state[pin] = GPIO_PIN_HIGH;
	} else {
		tmp |= 1 << pin;
		pcf8574_write(gpio, tmp);
		gpio->pin_state[pin] = GPIO_PIN_LOW;
	}
	
	return 0;
}

int pcf8574_get_pin(gpio_t *gpio, int pin, int *value)
{
    uint8_t tmp;
    
    if (!gpio)
        return -1;
    
    if (pin >= gpio->npins)
        return -1;
    
    if (!value)
        return -1;
    
    pcf8574_read(gpio, &tmp);
    
	if (tmp & (1 << pin))
		gpio->pin_state[pin] = GPIO_PIN_HIGH;
	else
		gpio->pin_state[pin] = GPIO_PIN_LOW;
    
    (*value) = gpio->pin_state[pin];
    return 0;
}

/**
 *
 * File Name: pcf8574.h
 * Title    : I2C device NXP PCF8574(A) I/O port expander library header
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

#ifndef LIBAVR_I2C_PCF8574_H
#define LIBAVR_I2C_PCF8574_H

#include <stdint.h>

#define TYPE_PCF8574    0
#define TYPE_PCF8574A   1

#define GPIO_PIN_LOW    0
#define GPIO_PIN_HIGH   1

#define GPIO_PIN_INPUT  0
#define GPIO_PIN_OUTPUT 1

#define NPINS           8

typedef struct gpio_s {
    uint8_t addr;
    int npins;
    int pin_config[NPINS];
    int pin_state[NPINS];
} gpio_t;

extern gpio_t *pcf8574_init(int type, uint8_t subaddr);
extern void pcf8574_free(gpio_t *gpio);
extern int pcf8574_write(gpio_t *gpio, uint8_t data);
extern int pcf8574_read(gpio_t *gpio, uint8_t *data);
extern int pcf8574_set_config(gpio_t *gpio, int pin, int config);
extern int pcf8574_get_config(gpio_t *gpio, int pin, int *config);
extern int pcf8574_get_npins(gpio_t *gpio);
extern int pcf8574_set_pin(gpio_t *gpio, int pin, int value);
extern int pcf8574_get_pin(gpio_t *gpio, int pin, int *value);

#endif

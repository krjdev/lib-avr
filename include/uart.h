/**
 *
 * File Name: include/uart.h
 * Title    : UART (RS232) interface
 * Project  : lib-avr
 * Author   : Copyright (C) 2018-2021 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-07-14
 * Modified : 2021-07-05
 * Revised  : 
 * Version  : 0.2.0.2
 * License  : ISC (see file LICENSE.txt)
 * Target   : Microchip AVR ATMEGA2560
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBAVR_UART_H
#define LIBAVR_UART_H

#include <stdint.h>

/* UART device (port) */
#define UART_DEV_UART0  0
#define UART_DEV_UART1  1
#define UART_DEV_UART2  2
#define UART_DEV_UART3  3

/* Data bits */
#define UART_DATA_5     5
#define UART_DATA_6     6
#define UART_DATA_7     7
#define UART_DATA_8     8
#define UART_DATA_9     9

/* Stop bits */
#define UART_STOP_1     1
#define UART_STOP_2     2

typedef struct uart {
    int u_dev;
    uint32_t u_baud;
    int u_databit;
    int u_stopbit;
} uart_t;

uart_t *uart_init(int dev, uint32_t baud, int datab, int stopb);
void uart_close(uart_t *uart);
int uart_send(uart_t *uart, uint8_t *data, int len);
int uart_recv(uart_t *uart, uint8_t *data, int len);
int uart_putc(uart_t *uart, char c);
int uart_puts(uart_t *uart, const char *str);
int uart_getc(uart_t *uart, char *c);

#endif

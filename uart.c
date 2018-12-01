/**
 *
 * File Name: uart.c
 * Title    : UART (RS232) library source
 * Project  : lib-avr
 * Author   : Copyright (C) 2018 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2018-07-14
 * Modified : 2018-12-01
 * Revised  : 
 * Version  : 0.1.1.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Atmel AVR ATMEGA2560
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdlib.h>
#include <avr/io.h>
#include "uart.h"

#define _HIGH(u16)  ((uint8_t) (((u16) & 0xFF00) >> 8))
#define _LOW(u16)   ((uint8_t) ((u16) & 0x00FF))

uart_t *uart_init(int dev, uint32_t baud, int databit, int stopbit)
{
    uint16_t ubrr;
    uart_t *p;
    
    if (databit < 5 || databit > 9)
        return NULL;
    
    if (stopbit < 1 || stopbit > 2)
        return NULL;
    
    p = (uart_t *) malloc(sizeof(uart_t));
    
    if (!p)
        return NULL;
    
    p->u_dev = dev;
    p->u_baud = baud;
    p->u_databit = databit;
    p->u_stopbit = stopbit;
    
    ubrr = (uint16_t) (F_OSC / (16 * p->u_baud) - 1);
    
    switch (p->u_dev) {
    case UART_DEV_UART0:
    {
        UBRR0H = _HIGH(ubrr);
        UBRR0L = _LOW(ubrr);
        
        switch (p->u_databit) {
        case UART_DATA_5:
            break;
        case UART_DATA_6:
            UCSR0C = (1 << UCSZ00);
            break;
        case UART_DATA_7:
            UCSR0C = (1 << UCSZ01);
            break;
        case UART_DATA_8:
            UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
            break;
        case UART_DATA_9:
            UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
            UCSR0B = (1 << UCSZ02);
            break;
        default:
            return NULL; /* should never be reached */
        }
        
        switch (p->u_stopbit) {
        case UART_STOP_1:
            break;
        case UART_STOP_2:
            UCSR0C |= (1 << USBS0);
            break;
        default:
            return NULL; /* should never be reached */
        }
        
        UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
        break;
    }
    case UART_DEV_UART1:
    {
        UBRR1H = _HIGH(ubrr);
        UBRR1L = _LOW(ubrr);
        
        switch (p->u_databit) {
        case UART_DATA_5:
            break;
        case UART_DATA_6:
            UCSR1C = (1 << UCSZ10);
            break;
        case UART_DATA_7:
            UCSR1C = (1 << UCSZ11);
            break;
        case UART_DATA_8:
            UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
            break;
        case UART_DATA_9:
            UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
            UCSR1B = (1 << UCSZ12);
            break;
        default:
            return NULL; /* should never be reached */
        }
        
        switch (p->u_stopbit) {
        case UART_STOP_1:
            break;
        case UART_STOP_2:
            UCSR1C |= (1 << USBS1);
            break;
        default:
            return NULL; /* should never be reached */
        }
        
        UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
        break;
    }
    case UART_DEV_UART2:
    {
        UBRR2H = _HIGH(ubrr);
        UBRR2L = _LOW(ubrr);
        
        switch (p->u_databit) {
        case UART_DATA_5:
            break;
        case UART_DATA_6:
            UCSR2C = (1 << UCSZ20);
            break;
        case UART_DATA_7:
            UCSR2C = (1 << UCSZ21);
            break;
        case UART_DATA_8:
            UCSR2C = (1 << UCSZ20) | (1 << UCSZ21);
            break;
        case UART_DATA_9:
            UCSR2C = (1 << UCSZ20) | (1 << UCSZ21);
            UCSR2B = (1 << UCSZ22);
            break;
        default:
            return NULL; /* should never be reached */
        }
        
        switch (p->u_stopbit) {
        case UART_STOP_1:
            break;
        case UART_STOP_2:
            UCSR2C |= (1 << USBS2);
            break;
        default:
            return NULL; /* should never be reached */
        }
        
        UCSR2B |= (1 << RXEN2) | (1 << TXEN2);
        break;
    }
    case UART_DEV_UART3:
    {
        UBRR3H = _HIGH(ubrr);
        UBRR3L = _LOW(ubrr);
        
        switch (p->u_databit) {
        case UART_DATA_5:
            break;
        case UART_DATA_6:
            UCSR3C = (1 << UCSZ30);
            break;
        case UART_DATA_7:
            UCSR3C = (1 << UCSZ31);
            break;
        case UART_DATA_8:
            UCSR3C = (1 << UCSZ30) | (1 << UCSZ31);
            break;
        case UART_DATA_9:
            UCSR3C = (1 << UCSZ30) | (1 << UCSZ31);
            UCSR3B = (1 << UCSZ32);
            break;
        default:
            return NULL; /* should never be reached */
        }
        
        switch (p->u_stopbit) {
        case UART_STOP_1:
            break;
        case UART_STOP_2:
            UCSR3C |= (1 << USBS3);
            break;
        default:
            return NULL; /* should never be reached */
        }
        
        UCSR3B |= (1 << RXEN3) | (1 << TXEN3);
        break;
    }
    default:
        free(p);
        return NULL;
    }
    
    return p;
}

void uart_close(uart_t *uart)
{
    if (!uart)
        return;
    
    switch (uart->u_dev) {
    case UART_DEV_UART0:
        UCSR0B &= ~(1 << RXEN0) | (1 << TXEN0); 
        break;
    case UART_DEV_UART1:
        UCSR1B &= ~(1 << RXEN1) | (1 << TXEN1);
        break;
    case UART_DEV_UART2:
        UCSR2B &= ~(1 << RXEN2) | (1 << TXEN2);
        break;
    case UART_DEV_UART3:
        UCSR3B &= ~(1 << RXEN3) | (1 << TXEN3);
        break;
    default:
        return;
    }
    
    free(uart);
    uart = NULL;
}

int uart_send(uart_t *uart, uint8_t *data, int len)
{
    int i;
    
    if (!uart)
        return -1;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    for (i = 0; i < len; i++)
        switch (uart->u_dev) {
        case UART_DEV_UART0:
            while (!(UCSR0A & (1 << UDRE0)))
                ;
            UDR0 = data[i];
            break;
        case UART_DEV_UART1:
            while (!(UCSR1A & (1 << UDRE1)))
                ;
            UDR1 = data[i];
            break;
        case UART_DEV_UART2:
            while (!(UCSR2A & (1 << UDRE2)))
                ;
            UDR2 = data[i];
            break;
        case UART_DEV_UART3:
            while (!(UCSR3A & (1 << UDRE3)))
                ;
            UDR3 = data[i];
            break;
        default:
            return -1;
        }
    
    return len;
}

int uart_recv(uart_t *uart, uint8_t *data, int len)
{
    int i;
    
    if (!uart)
        return -1;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    for (i = 0; i < len; i++)
        switch (uart->u_dev) {
        case UART_DEV_UART0:
            while (!(UCSR0A & (1 << RXC0)))
                ;
            *(data + i) = UDR0;
            break;
        case UART_DEV_UART1:
            while (!(UCSR1A & (1 << RXC1)))
                ;
            *(data + i) = UDR1;
            break;
        case UART_DEV_UART2:
            while (!(UCSR2A & (1 << RXC2)))
                ;
            *(data + i) = UDR2;
            break;
        case UART_DEV_UART3:
            while (!(UCSR3A & (1 << RXC3)))
                ;
            *(data + i) = UDR3;
            break;
        default:
            return -1;
        }
    
    return len;
}

void uart_putc(uart_t *uart, char c)
{
    uint8_t buf[1];
    
    if (!uart)
        return;
    
    buf[0] = (uint8_t) c;
    uart_send(uart, buf, 1);
}

void uart_puts(uart_t *uart, const char *str)
{
    int i = 0;
    uint8_t buf[1];
    
    if (!uart)
        return;
    
    if (!str)
        return;
    
    while (str[i] != '\0') {
        buf[0] = (uint8_t) str[i++];
        uart_send(uart, buf, 1);
    }
}

char uart_getc(uart_t *uart)
{
    uint8_t buf[1];
    
    if (!uart)
        return 0;
    
    uart_recv(uart, buf, 1);
    return (char) buf[0];
}

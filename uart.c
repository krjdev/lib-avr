/**
 *
 * File Name: uart.c
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

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include <fifo.h>
#include <uart.h>

#define _HIGH(u16)  ((uint8_t) (((u16) & 0xFF00) >> 8))
#define _LOW(u16)   ((uint8_t) ((u16) & 0x00FF))

#define RX_BUFSZ    32

fifo_t *rx_buf_uart0;
fifo_t *rx_buf_uart1;
fifo_t *rx_buf_uart2;
fifo_t *rx_buf_uart3;

ISR(USART0_RX_vect)
{
    uint32_t tmp_sreg;
    
    cli();
    tmp_sreg = SREG;
    fifo_enqueue(rx_buf_uart0, UDR0);
    SREG = tmp_sreg;
    sei();
}

ISR(USART1_RX_vect)
{
    uint32_t tmp_sreg;
    
    cli();
    tmp_sreg = SREG;
    fifo_enqueue(rx_buf_uart1, UDR1);
    SREG = tmp_sreg;
    sei();
}

ISR(USART2_RX_vect)
{
    uint32_t tmp_sreg;
    
    cli();
    tmp_sreg = SREG;
    fifo_enqueue(rx_buf_uart2, UDR2);
    SREG = tmp_sreg;
    sei();
}

ISR(USART3_RX_vect)
{
    uint32_t tmp_sreg;
    
    cli();
    tmp_sreg = SREG;
    fifo_enqueue(rx_buf_uart3, UDR3);
    SREG = tmp_sreg;
    sei();
}

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
            free(p);
            return NULL;
        }
        
        switch (p->u_stopbit) {
        case UART_STOP_1:
            break;
        case UART_STOP_2:
            UCSR0C |= (1 << USBS0);
            break;
        default:
            free(p);
            return NULL;
        }
        
        rx_buf_uart0 = fifo_init(RX_BUFSZ);
        
        if (!rx_buf_uart0) {
            free(p);
            return NULL;
        }
        
        UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
        sei();
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
            free(p);
            return NULL;
        }
        
        switch (p->u_stopbit) {
        case UART_STOP_1:
            break;
        case UART_STOP_2:
            UCSR1C |= (1 << USBS1);
            break;
        default:
            free(p);
            return NULL;
        }
        
        rx_buf_uart1 = fifo_init(RX_BUFSZ);
        
        if (!rx_buf_uart1) {
            free(p);
            return NULL;
        }
        
        UCSR1B |= (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);
        sei();
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
            free(p);
            return NULL;
        }
        
        switch (p->u_stopbit) {
        case UART_STOP_1:
            break;
        case UART_STOP_2:
            UCSR2C |= (1 << USBS2);
            break;
        default:
            free(p);
            return NULL;
        }
        
        rx_buf_uart2 = fifo_init(RX_BUFSZ);
        
        if (!rx_buf_uart2) {
            free(p);
            return NULL;
        }
        
        UCSR2B |= (1 << RXCIE2) | (1 << RXEN2) | (1 << TXEN2);
        sei();
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
            free(p);
            return NULL;
        }
        
        switch (p->u_stopbit) {
        case UART_STOP_1:
            break;
        case UART_STOP_2:
            UCSR3C |= (1 << USBS3);
            break;
        default:
            free(p);
            return NULL;
        }
        
        rx_buf_uart3 = fifo_init(RX_BUFSZ);
        
        if (!rx_buf_uart3) {
            free(p);
            return NULL;
        }
        
        UCSR3B |= (1 << RXCIE3) | (1 << RXEN3) | (1 << TXEN3);
        sei();
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
    
    cli();
    
    switch (uart->u_dev) {
    case UART_DEV_UART0:
        UCSR0B &= ~(1 << RXEN0) | (1 << TXEN0);
        fifo_free(rx_buf_uart0);
        break;
    case UART_DEV_UART1:
        UCSR1B &= ~(1 << RXEN1) | (1 << TXEN1);
        fifo_free(rx_buf_uart1);
        break;
    case UART_DEV_UART2:
        UCSR2B &= ~(1 << RXEN2) | (1 << TXEN2);
        fifo_free(rx_buf_uart2);
        break;
    case UART_DEV_UART3:
        UCSR3B &= ~(1 << RXEN3) | (1 << TXEN3);
        fifo_free(rx_buf_uart3);
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
    int ret = -1;
    int i;
    uint8_t tmp;
    
    if (!uart)
        return -1;
    
    if (!data)
        return -1;
    
    if (len < 1)
        return -1;
    
    for (i = 0; i < len; i++)
        switch (uart->u_dev) {
        case UART_DEV_UART0:
            if (fifo_dequeue(rx_buf_uart0, &tmp) != -1) {
                *(data + i) = tmp;
                
                if (ret == -1)
                    ret = 1;
                else
                    ret++;
            } else
                return ret;
            break;
        case UART_DEV_UART1:
            if (fifo_dequeue(rx_buf_uart1, &tmp) != -1) {
                *(data + i) = tmp;
                
                if (ret == -1)
                    ret = 1;
                else
                    ret++;
            } else
                return ret;
            break;
        case UART_DEV_UART2:
            if (fifo_dequeue(rx_buf_uart2, &tmp) != -1) {
                *(data + i) = tmp;
                
                if (ret == -1)
                    ret = 1;
                else
                    ret++;
            } else
                return ret;
            break;
        case UART_DEV_UART3:
            if (fifo_dequeue(rx_buf_uart3, &tmp) != -1) {
                *(data + i) = tmp;
                
                if (ret == -1)
                    ret = 1;
                else
                    ret++;
            } else
                return ret;
            break;
        default:
            return -1;
        }
    
    return ret;
}

int uart_putc(uart_t *uart, char c)
{
    uint8_t buf[1];
    
    if (!uart)
        return -1;
    
    buf[0] = (uint8_t) c;
    uart_send(uart, buf, 1);
    return 0;
}

int uart_puts(uart_t *uart, const char *str)
{
    int i = 0;
    uint8_t buf[1];
    
    if (!uart)
        return -1;
    
    if (!str)
        return -1;
    
    while (str[i] != '\0') {
        buf[0] = (uint8_t) str[i++];
        uart_send(uart, buf, 1);
    }
    
    return 0;
}

int uart_getc(uart_t *uart, char *c)
{
    uint8_t buf[1];
    
    if (!uart)
        return -1;
    
    if (uart_recv(uart, buf, 1) == -1)
        return -1;
    
    (*c) = (char) buf[0];
    return 0;
}

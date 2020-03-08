/**
 *
 * File Name: example/onewire/ds18x20/main.c
 * Title    : Maxim (former Dallas) DS18S20 Example
 * Project  : lib-avr
 * Author   : Copyright (C) 2020 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2020-03-08
 * Modified : 
 * Revised  : 
 * Version  : 0.1.0.0
 * License  : ISC (see file LICENSE.txt)
 * Target   : Microchip AVR ATmega2560
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "../../../uart.h"
#include "../../../onewire/ds18x20.h"

int main(void)
{
    int i, j;
    char buf[64];
    uart_t *uart;
    ow_rom_t roms[10];
    int ret = 0;
    uint8_t serial[6];
    float temp;
    char c = '\0';
    
    uart = uart_init(UART_DEV_UART0, 
                     500000, 
                     UART_DATA_8, 
                     UART_STOP_1);
    
    uart_puts(uart, "DS18x20 driver: Init...");
    ds18x20_init(); 
    uart_puts(uart, "[done]\r\n");
    
    while (c != 'q') {
        ret = uart_getc(uart, &c);
        
        if (ret != -1) {
            if (c == 's') {
                ret = ds18x20_search_rom(TYPE_DS18S20, roms, 10);
                sprintf(buf, "Found %d DS18S20 sensor(s):\r\n", ret);
                uart_puts(uart, buf);
                
                for (j = 0; j < ret; j++) {
                    onewire_get_serial(&roms[j], serial);
                    sprintf(buf, "Device %d: ", j);
                    uart_puts(uart, buf);
                    
                    for (i = 0; i < 6; i++) {
                        sprintf(buf, "%02X", serial[i]);
                        uart_puts(uart, buf);
                    }
                
                    uart_puts(uart, "\r\n");
                }
            } else if (c == 't') {
                if (ret) {
                    uart_puts(uart, "Temperature  (9 Bit):\r\n");
                    
                    for (j = 0; j < ret; j++) {
                        ds18x20_convert(&roms[j]);
                        ds18x20_get_temp(&roms[j], TYPE_DS18S20, RESOLUTION_9BIT, &temp);
                        sprintf(buf, "Device %d: %f Celsius\r\n", j, temp);
                        uart_puts(uart, buf);
                    }
                    
                    uart_puts(uart, "Temperature (12 Bit):\r\n");
                    
                    for (j = 0; j < ret; j++) {
                        ds18x20_convert(&roms[j]);
                        ds18x20_get_temp(&roms[j], TYPE_DS18S20, RESOLUTION_12BIT, &temp);
                        sprintf(buf, "Device %d: %f Celsius\r\n", j, temp);
                        uart_puts(uart, buf);
                    }
                } else 
                    uart_puts(uart, "No devices found, try search first\r\n");
            }
        }
    }
    
    uart_puts(uart, "quit\r\n");
    uart_close(uart);
    
    while (1) /* loop forever */
        ;
    
    return 0;
}

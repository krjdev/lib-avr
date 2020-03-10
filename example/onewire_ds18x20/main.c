/**
 *
 * File Name: example/onewire_ds18x20/main.c
 * Title    : Maxim (former Dallas) DS18S20 Example
 * Project  : lib-avr
 * Author   : Copyright (C) 2020 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2020-03-10
 * Modified : 
 * Revised  : 
 * Version  : 0.1.1.0
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

#include "../../uart.h"
#include "../../onewire/ds18x20.h"

#define BUF_LEN         64
#define OW_ROM_MAX      10
#define OW_SERIAL_LEN   6

int main(void)
{
    int i, j;
    char str[BUF_LEN];
    uart_t *uart;
    int uart_ret;
    ow_rom_t ow_roms[OW_ROM_MAX];
    int ow_ret = 0;
    uint8_t ow_serial[OW_SERIAL_LEN];
    float ow_temp;
    char c = '\0';
    
    uart = uart_init(UART_DEV_UART0, 
                     500000, 
                     UART_DATA_8, 
                     UART_STOP_1);
    
    uart_puts(uart, "DS18x20 driver: Init...");
    ds18x20_init();
    uart_puts(uart, "[done]\r\n");
    uart_puts(uart, "s -> Search for DS18S20 devices\r\n");
    uart_puts(uart, "t -> Get temperature from devices\r\n");
    
    while (c != 'q') {
        uart_ret = uart_getc(uart, &c);
        
        if (uart_ret != -1) {
            if (c == 's') {
                ow_ret = ds18x20_search_rom(TYPE_DS18S20, ow_roms, OW_ROM_MAX);
                sprintf(str, "Found %d DS18S20 sensor(s):\r\n", ow_ret);
                uart_puts(uart, str);
                
                for (j = 0; j < ow_ret; j++) {
                    onewire_get_serial(&ow_roms[j], ow_serial);
                    sprintf(str, "Device %d: \r\n", j);
                    uart_puts(uart, str);
                    
                    for (i = 0; i < OW_SERIAL_LEN; i++) {
                        sprintf(str, "%02X", ow_serial[i]);
                        uart_puts(uart, str);
                    }
                
                    uart_puts(uart, "\r\n");
                }
            } else if (c == 't') {
                if (ow_ret) {
                    uart_puts(uart, "Temperature  (9 Bit):\r\n");
                    
                    for (j = 0; j < ow_ret; j++) {
                        ds18x20_convert(&ow_roms[j]);
                        ds18x20_get_temp(&ow_roms[j], TYPE_DS18S20, RESOLUTION_9BIT, &ow_temp);
                        sprintf(str, "Device %d: %f Celsius\r\n", j, ow_temp);
                        uart_puts(uart, str);
                    }
                    
                    uart_puts(uart, "Temperature (12 Bit):\r\n");
                    
                    for (j = 0; j < ow_ret; j++) {
                        ds18x20_convert(&ow_roms[j]);
                        ds18x20_get_temp(&ow_roms[j], TYPE_DS18S20, RESOLUTION_12BIT, &ow_temp);
                        sprintf(str, "Device %d: %f Celsius\r\n", j, ow_temp);
                        uart_puts(uart, str);
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

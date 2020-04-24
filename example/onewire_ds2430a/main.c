/**
 *
 * File Name: example/onewire_ds2430a/main.c
 * Title    : Maxim (former Dallas) DS2430A Example
 * Project  : lib-avr
 * Author   : Copyright (C) 2020 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2020-04-24
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
#include <stdlib.h>
#include <stdint.h>

#include "../../uart.h"
#include "../../onewire/ds2430a.h"

#define BUF_LEN         64
#define OW_ROM_MAX      10
#define OW_SERIAL_LEN   6

int main(void)
{
    int i, j;
    char c = '\0';
    char str[BUF_LEN];
    uart_t *uart;
    int uart_ret;
    ow_rom_t ow_roms[OW_ROM_MAX];
    int ow_ret = 0;
    uint8_t ow_serial[OW_SERIAL_LEN];
    
    uart = uart_init(UART_DEV_UART0, 
                     500000, 
                     UART_DATA_8, 
                     UART_STOP_1);
    uart_puts(uart, "DS2430A driver: init...");
    ds2430a_init();
    uart_puts(uart, "[done]\r\n");
    uart_puts(uart, "s -> Search for DS2430A devices\r\n");
    uart_puts(uart, "w -> Write something into the EEPROM\r\n");
    uart_puts(uart, "r -> Read from EEPROM\r\n");
    
    while (c != 'q') {
        uart_ret = uart_getc(uart, &c);
        
        if (uart_ret != -1) {
            if (c == 's') {
                ow_ret = ds2430a_search_rom(ow_roms, OW_ROM_MAX);
                sprintf(str, "Found %d DS2430A EEPROM(s):\r\n", ow_ret);
                uart_puts(uart, str);
                
                for (j = 0; j < ow_ret; j++) {
                    onewire_get_serial(&ow_roms[j], ow_serial);
                    sprintf(str, "Device %d: ", j);
                    uart_puts(uart, str);
                    
                    for (i = 0; i < OW_SERIAL_LEN; i++) {
                        sprintf(str, "%02X", ow_serial[i]);
                        uart_puts(uart, str);
                    }
                    
                    uart_puts(uart, "\r\n");
                }
            } else if (c == 'w') {
                if (ow_ret) {
                    uint8_t data_in[] = { 0x0D, 0x0E, 0xAA, 0xDD };
                    
                    for (j = 0; j < ow_ret; j++) {
                        uart_puts(uart, "DS2430A driver: write memory...");
                        ds2430a_write_memory(&ow_roms[j], 0, data_in, sizeof(data_in));
                        uart_puts(uart, "[done]\r\n");
                    }
                } else
                    uart_puts(uart, "No devices found, try search first\r\n");
            } else if (c == 'r') {
                if (ow_ret) {
                    uint8_t data_out[4];
                    
                    for (j = 0; j < ow_ret; j++) {
                        uart_puts(uart, "DS2430A driver: read memory...");
                        ds2430a_read_memory(&ow_roms[j], 0, data_out, sizeof(data_out));
                        uart_puts(uart, "[done]\r\n");
                        uart_puts(uart, "Contents:\r\n");
                        
                        for (i = 0; i < 4; i++) {
                            sprintf(str, "%02X", data_out[i]);
                            uart_puts(uart, str);
                        }
                    
                        uart_puts(uart, "\r\n");
                    }
                } else
                    uart_puts(uart, "No devices found, try search first\r\n");
            }
        }
    }
    
    uart_puts(uart, "[quit]\r\n");
    uart_close(uart);
    
    while (1)
        ; /* loop forever */
    
    return 0;
}

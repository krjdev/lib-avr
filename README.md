# lib-avr

This repository contains generic libraries and device driver for Microchip (former Atmel) AVR microcontrollers.

**Requirements:**  
[avr-gcc](https://gcc.gnu.org/wiki/avr-gcc "avr-gcc Homepage")  
[avr-libc](https://www.nongnu.org/avr-libc/ "AVR Libc Homepage")


### UART (RS232) Interface (beta version)

**Files:**  
[uart.h](https://github.com/krjdev/lib-avr/blob/master/uart.h)  
[uart.c](https://github.com/krjdev/lib-avr/blob/master/uart.c)

**Functions:**  
```c
uart_t *uart_init(int dev, uint32_t baud, int datab, int stopb);
void uart_close(uart_t *uart);
int uart_send(uart_t *uart, uint8_t *data, int len);
int uart_recv(uart_t *uart, uint8_t *data, int len);
int uart_putc(uart_t *uart, char c);
int uart_puts(uart_t *uart, const char *str);
int uart_getc(uart_t *uart, char *c);
```

**Dependencies**  
* avr-libc  
* lib/fifo.h  
* lib/fifo.c

## I2C

**Directory:**  
[i2c](https://github.com/krjdev/lib-avr/tree/master/i2c)

### I2C Interface (beta version)

**Files:**  
[i2c/i2c.h](https://github.com/krjdev/lib-avr/blob/master/i2c/i2c.h)  
[i2c/i2c.c](https://github.com/krjdev/lib-avr/blob/master/i2c/i2c.c)

**Functions:**  
```c
int i2c_master_init(uint32_t speed);
int i2c_master_send(int opt, uint8_t addr, uint8_t *cmd, int cmd_len, uint8_t *data, int data_len);
int i2c_master_recv(int opt, uint8_t addr, uint8_t *cmd, int cmd_len, uint8_t *data, int data_len);
int i2c_get_error(void);
```

**Dependencies**  
* avr-libc

### I2C device: STmicroelectronics M24C01 - M24C64 I2C EEPROM driver (alpha version)

**Files:**  
[i2c/m24cxx.h](https://github.com/krjdev/lib-avr/blob/master/i2c/m24cxx.h)  
[i2c/m24cxx.c](https://github.com/krjdev/lib-avr/blob/master/i2c/m24cxx.c)

**Functions:**  
```c
void m24cxx_init(void);
int m24cxx_write(int type, uint8_t subaddr, uint16_t addr, uint8_t *buf, int len);
int m24cxx_read(int type, uint8_t subaddr, uint16_t addr, uint8_t *buf, int len);
```

**Dependencies**  
* avr-libc  
* i2c/i2c.h  
* i2c/i2c.c

### I2C device: NXP PCF8574(A) 8-Bit Remote I/O expander driver (beta version)

**Files:**  
[i2c/pcf8574.h](https://github.com/krjdev/lib-avr/blob/master/i2c/pcf8574.h)  
[i2c/pcf8574.c](https://github.com/krjdev/lib-avr/blob/master/i2c/pcf8574.c)


**Functions:**  
```c
gpio_t *pcf8574_init(int type, uint8_t subaddr);
void pcf8574_free(gpio_t *gpio);
int pcf8574_write(gpio_t *gpio, uint8_t data);
int pcf8574_read(gpio_t *gpio, uint8_t *data);
int pcf8574_set_config(gpio_t *gpio, int pin, int config);
int pcf8574_get_config(gpio_t *gpio, int pin, int *config);
int pcf8574_get_npins(gpio_t *gpio);
int pcf8574_set_pin(gpio_t *gpio, int pin, int value);
int pcf8574_get_pin(gpio_t *gpio, int pin, int *value);
```

**Dependencies**  
* avr-libc  
* i2c/i2c.h  
* i2c/i2c.c

### I2C device: NXP PCF8591 8-Bit A/D and D/A converter driver (alpha version)

**Files:**  
[i2c/pcf8591.h](https://github.com/krjdev/lib-avr/blob/master/i2c/pcf8591.h)  
[i2c/pcf8591.c](https://github.com/krjdev/lib-avr/blob/master/i2c/pcf8591.c)

**Functions:**  
```c
void pcf8591_init(void)  
int pcf8591_get_adc(uint8_t subaddr, int config, int channel, uint8_t *value)  
int pcf8591_set_dac(uint8_t subaddr, uint8_t *value)
```

**Dependencies**  
* avr-libc  
* i2c/i2c.h  
* i2c/i2c.c

## Generic Libraries

**Directory:**  
[lib](https://github.com/krjdev/lib-avr/tree/master/lib)

### BCD Library

**Files:**  
[lib/bcd.h](https://github.com/krjdev/lib-avr/blob/master/lib/bcd.h)  
[lib/bcd.c](https://github.com/krjdev/lib-avr/blob/master/lib/bcd.c)

**Functions:**  
```c
uint8_t bcd_to_int(uint8_t bcd);
uint8_t int_to_bcd(uint8_t i);
```

**Dependencies**  
* avr-libc

### CRC8 Maxim (former Dallas) Library

**Files:**  
[lib/crc8_dallas.h](https://github.com/krjdev/lib-avr/blob/master/lib/crc8_dallas.h)  
[lib/crc8_dallas.c](https://github.com/krjdev/lib-avr/blob/master/lib/crc8_dallas.c)

**Functions:**  
```c
int crc8_dallas_calc(uint8_t *data, int len);
int crc8_dallas_check(uint8_t *data, int len, uint8_t crc);
```

**Dependencies**  
* avr-libc

### FIFO buffer Library

**Files:**  
[lib/fifo.h](https://github.com/krjdev/lib-avr/blob/master/lib/fifo.h)  
[lib/fifo.c](https://github.com/krjdev/lib-avr/blob/master/lib/fifo.c)

**Functions:**  
```c
fifo_t *fifo_init(int len);
void fifo_free(fifo_t *fifo);
int fifo_enqueue(fifo_t *fifo, uint8_t data);
int fifo_dequeue(fifo_t *fifo, uint8_t *data);
int fifo_get_len(fifo_t *fifo);
int fifo_get_num(fifo_t *fifo);
```

**Dependencies**  
* avr-libc

### LIFO (Stack) buffer Library

**Files:**  
[lib/lifo.h](https://github.com/krjdev/lib-avr/blob/master/lib/lifo.h)  
[lib/lifo.c](https://github.com/krjdev/lib-avr/blob/master/lib/lifo.c)

**Functions:**  
```c
lifo_t *lifo_init(int len);
void lifo_free(lifo_t *lifo);
int lifo_enqueue(lifo_t *lifo, uint8_t data);
int lifo_dequeue(lifo_t *lifo, uint8_t *data);
int lifo_get_len(lifo_t *lifo);
int lifo_get_num(lifo_t *lifo);
```

**Dependencies**  
* avr-libc

## Network Libraries

**Directory:**  
[net](https://github.com/krjdev/lib-avr/tree/master/net)

### ARP Library (beta version)

**Files:**  
[net/arp.h](https://github.com/krjdev/lib-avr/blob/master/net/arp.h)  
[net/arp.c](https://github.com/krjdev/lib-avr/blob/master/net/arp.c)

**Functions:**  
```c
int arp_init(mac_addr_t *src_mac, ipv4_addr_t *src_ip);
int arp_pkt_set_oper(arp_packet_t *arp, uint16_t oper);
int arp_pkt_set_sha(arp_packet_t *arp, mac_addr_t *mac);
int arp_pkt_set_spa(arp_packet_t *arp, ipv4_addr_t *ip);
int arp_pkt_set_tha(arp_packet_t *arp, mac_addr_t *mac);
int arp_pkt_set_tpa(arp_packet_t *arp, ipv4_addr_t *ip);
int arp_pkt_get_oper(arp_packet_t *arp, uint16_t *oper);
int arp_pkt_get_sha(arp_packet_t *arp, mac_addr_t *mac);
int arp_pkt_get_spa(arp_packet_t *arp, ipv4_addr_t *ip);
int arp_pkt_get_tha(arp_packet_t *arp, mac_addr_t *mac);
int arp_pkt_get_tpa(arp_packet_t *arp, ipv4_addr_t *ip);
int arp_pkt_get_len(arp_packet_t *arp);
int arp_buf_to_pkt(uint8_t *buf, int len, arp_packet_t *arp);
int arp_pkt_to_buf(arp_packet_t *arp, uint8_t *buf);
int arp_pkt_valid(arp_packet_t *arp);
int arp_pkt_create(arp_packet_t *arp);
int arp_pkt_create_probe(arp_packet_t *arp);
int arp_pkt_create_query(ipv4_addr_t *dst_ip, arp_packet_t *arp);
int arp_pkt_create_answer(arp_packet_t *arp_in, arp_packet_t *arp_out);
```

**Dependencies**  
* avr-libc  
* net/ethernet.h  
* net/ethernet.c

### Ethernet Library (beta version)

**Files:**  
[net/ethernet.h](https://github.com/krjdev/lib-avr/blob/master/net/ethernet.h)  
[net/ethernet.c](https://github.com/krjdev/lib-avr/blob/master/net/ethernet.c)

**Functions:**  
```c
int ethernet_addr_aton(const char *str, mac_addr_t *mac);
int ethernet_addr_ntoa(mac_addr_t *mac, char *str);
int ethernet_addr_cpy(mac_addr_t *dst, mac_addr_t *src);
int ethernet_addr_equal(mac_addr_t *mac1, mac_addr_t *mac2);
int ethernet_frame_set_dst(eth_frame_t *frame, mac_addr_t *mac);
int ethernet_frame_set_src(eth_frame_t *frame, mac_addr_t *mac);
int ethernet_frame_set_type(eth_frame_t *frame, uint16_t type);
int ethernet_frame_set_payload(eth_frame_t *frame, uint8_t *buf, int len);
int ethernet_frame_get_dst(eth_frame_t *frame, mac_addr_t *mac);
int ethernet_frame_get_src(eth_frame_t *frame, mac_addr_t *mac);
int ethernet_frame_get_type(eth_frame_t *frame, uint16_t *type);
int ethernet_frame_get_payload_len(eth_frame_t *frame);
int ethernet_frame_get_payload(eth_frame_t *frame, uint8_t **buf);
int ethernet_frame_get_len(eth_frame_t *frame);
int ethernet_frame_payload_free(eth_frame_t *frame);
int ethernet_buf_to_frm(uint8_t *buf, int len, eth_frame_t *frame);
int ethernet_frm_to_buf(eth_frame_t *frame, uint8_t *buf);
```

**Dependencies**  
* avr-libc

### IPv4 Library (beta version)

**Files:**  
[net/ipv4.h](https://github.com/krjdev/lib-avr/blob/master/net/ipv4.h)  
[net/ipv4.c](https://github.com/krjdev/lib-avr/blob/master/net/ipv4.c)


**Functions:**  
```c
int ipv4_addr_aton(const char *str, ipv4_addr_t *ip);
int ipv4_addr_ntoa(ipv4_addr_t *ip, char *str);
int ipv4_addr_equal(ipv4_addr_t *ip1, ipv4_addr_t *ip2);
int ipv4_addr_cpy(ipv4_addr_t *dst, ipv4_addr_t *src);
int ipv4_addr_is_broadcast(ipv4_addr_t *ip);
int ipv4_pkt_create_empty(ipv4_packet_t *ip);
int ipv4_pkt_free(ipv4_packet_t *ip);
int ipv4_pkt_set_id(ipv4_packet_t *ip, uint16_t id);
int ipv4_pkt_set_ttl(ipv4_packet_t *ip, uint8_t ttl);
int ipv4_pkt_set_prot(ipv4_packet_t *ip, uint8_t prot);
int ipv4_pkt_set_src(ipv4_packet_t *ip, ipv4_addr_t *src);
int ipv4_pkt_set_dst(ipv4_packet_t *ip, ipv4_addr_t *dst);
int ipv4_pkt_set_options(ipv4_packet_t *ip, uint8_t *buf, int len);
int ipv4_pkt_set_payload(ipv4_packet_t *ip, uint8_t *buf, int len);
int ipv4_pkt_get_id(ipv4_packet_t *ip, uint16_t *id);
int ipv4_pkt_get_ttl(ipv4_packet_t *ip, uint8_t *ttl);
int ipv4_pkt_get_prot(ipv4_packet_t *ip, uint8_t *prot);
int ipv4_pkt_get_src(ipv4_packet_t *ip, ipv4_addr_t *src);
int ipv4_pkt_get_dst(ipv4_packet_t *ip, ipv4_addr_t *dst);
int ipv4_pkt_get_options_len(ipv4_packet_t *ip);
int ipv4_pkt_get_options(ipv4_packet_t *ip, uint8_t **buf);
int ipv4_pkt_get_payload_len(ipv4_packet_t *ip);
int ipv4_pkt_get_payload(ipv4_packet_t *ip, uint8_t **buf);
int ipv4_pkt_get_len(ipv4_packet_t *ip);
int ipv4_buf_to_pkt(uint8_t *buf, int len, ipv4_packet_t *ip);
int ipv4_pkt_to_buf(ipv4_packet_t *ip, uint8_t *buf);
```

**Dependencies**  
* avr-libc

### UDP Library (alpha version)

**Files:**  
[net/udp.h](https://github.com/krjdev/lib-avr/blob/master/net/udp.h)  
[net/udp.c](https://github.com/krjdev/lib-avr/blob/master/net/udp.c)

**Functions:**  
```c
int udp_set_port_src(udp_hdr_t *udp, uint16_t src);
int udp_set_port_dst(udp_hdr_t *udp, uint16_t dst);
int udp_set_len(udp_hdr_t *udp, uint16_t len);
int udp_set_chk(udp_hdr_t *udp, uint16_t chk);
int udp_get_port_src(udp_hdr_t *udp, uint16_t *src);
int udp_get_port_dst(udp_hdr_t *udp, uint16_t *dst);
int udp_get_len(udp_hdr_t *udp, uint16_t *len);
int udp_get_chk(udp_hdr_t *udp, uint16_t *chk);
```

**Dependencies**  
* avr-libc

## Maxim (former Dallas) 1-Wire

**Directory:**  
[onewire](https://github.com/krjdev/lib-avr/tree/master/onewire)


### 1-Wire Interface (Software-based)

**Files:**  
[onewire/sw-onewire.h](https://github.com/krjdev/lib-avr/blob/master/onewire/sw-onewire.h)  
[onewire/sw-onewire.c](https://github.com/krjdev/lib-avr/blob/master/onewire/sw-onewire.c)

**Functions:**  
```c
void onewire_init(void);
int onewire_reset(void);
int onewire_send(uint8_t *data, int len);
int onewire_recv(uint8_t *data, int len);
int onewire_read_rom(ow_rom_t *rom);
int onewire_search_rom(int type, ow_rom_t *roms, int num);
int onewire_search_family(int type, uint8_t family, ow_rom_t *roms, int num);
int onewire_match_rom(ow_rom_t *rom);
int onewire_skip_rom(void);
int onewire_get_family(ow_rom_t *rom, uint8_t *family);
int onewire_get_serial(ow_rom_t *rom, uint8_t *buf);
int onewire_get_crc(ow_rom_t *rom, uint8_t *crc);
```

**Dependencies**  
* avr-libc  
* lib/crc8_dallas.h  
* lib/crc8_dallas.c

### 1-Wire device: Maxim (former Dallas) DS18S20 and DS18B20 Digital Thermometer driver

**Files:**  
[onewire/ds18x20.h](https://github.com/krjdev/lib-avr/blob/master/onewire/ds18x20.h)  
[onewire/ds18x20.c](https://github.com/krjdev/lib-avr/blob/master/onewire/ds18x20.c)

**Functions:**  
```c
void ds18x20_init(void);
int ds18x20_read_rom(int type, ow_rom_t *rom);
int ds18x20_search_rom(int type, ow_rom_t *roms, int num);
int ds18x20_search_alarm(int type, ow_rom_t *roms, int num);
int ds18x20_convert(ow_rom_t *rom);
int ds18x20_get_temp(ow_rom_t *rom, int type, int res, float *temp);
int ds18x20_set_resolution(ow_rom_t *rom, int res); /* DS18B20 only */
int ds18x20_set_alarm(ow_rom_t *rom, int type, int8_t temp_high, int8_t temp_low);
```

**Dependencies**  
* avr-libc  
* lib/crc8_dallas.h  
* lib/crc8_dallas.c  
* onewire/sw-onewire.h  
* onewire/sw-onewire.c

### 1-Wire device: Dallas/Maxim DS2430A 256-Bit EEPROM driver

**Files:**  
[onewire/ds2430a.h](https://github.com/krjdev/lib-avr/blob/master/onewire/ds2430a.h)  
[onewire/ds2430a.c](https://github.com/krjdev/lib-avr/blob/master/onewire/ds2430a.c)

**Functions:**  
```c
void ds2430a_init(void);
int ds2430a_read_rom(ow_rom_t *rom);
int ds2430a_search_rom(ow_rom_t *roms, int num);
int ds2430a_write_memory(ow_rom_t *rom, uint8_t addr, uint8_t *buf, int len);
int ds2430a_read_memory(ow_rom_t *rom, uint8_t addr, uint8_t *buf, int len);
int ds2430a_write_app_reg(ow_rom_t *rom, uint8_t addr, uint8_t *buf, int len);
int ds2430a_read_app_reg(ow_rom_t *rom, uint8_t addr, uint8_t *buf, int len);
int ds2430a_lock_app_reg(ow_rom_t *rom);
int ds2430a_read_status(ow_rom_t *rom, uint8_t *status);
```

**Dependencies**  
* avr-libc  
* lib/crc8_dallas.h  
* lib/crc8_dallas.c  
* onewire/sw-onewire.h  
* onewire/sw-onewire.c

## SPI

**Directory:**  
[spi](https://github.com/krjdev/lib-avr/tree/master/spi)

### SPI Interface (beta version)

**Files:**  
[spi/spi.h](https://github.com/krjdev/lib-avr/blob/master/spi/spi.h)  
[spi/spi.c](https://github.com/krjdev/lib-avr/blob/master/spi/spi.c)

**Functions:**  
```c
void spi_master_init(int mode, int speed);
void spi_master_send(uint8_t *data, int len);
void spi_master_recv(uint8_t *data, int len);
```

**Dependencies**  
* avr-libc

### SPI device: Microchip ENC28J60 Ethernet controller driver

**Files:**  
[spi/enc28j60.h](https://github.com/krjdev/lib-avr/blob/master/spi/enc28j60.h)  
[spi/enc28j60.c](https://github.com/krjdev/lib-avr/blob/master/spi/enc28j60.c)

**Functions:**  
```c
int enc28j60_init(int mode, mac_addr_t *addr);
int enc28j60_send(eth_frame_t *frame);
int enc28j60_recv(eth_frame_t *frame);
int enc28j60_set_mac(mac_addr_t *addr);
int enc28j60_get_mac(mac_addr_t *addr);
int enc28j60_is_link_up(void);
uint32_t enc28j60_get_count_rx_frame(void);
uint32_t enc28j60_get_count_tx_frame(void);
uint16_t enc28j60_get_count_rx_err(void);
uint16_t enc28j60_get_count_tx_err(void);
int enc28j60_get_free_rx_space(void);
char *enc28j60_get_version(void);
char *enc28j60_get_chip_revision(void);
int enc28j60_get_last_error(void);
struct enc28j60_regs enc28j60_get_regs(void);
```

**Dependencies**  
* avr-libc  
* net/ethernet.h  
* net/ethernet.c  
* spi/spi.h  
* spi/spi.c

# LICENSE
> Copyright (c) 2018-2019 [Johannes Krottmayer](mailto:krjdev@gmail.com)  
>  
> Permission to use, copy, modify, and/or distribute this software for any  
> purpose with or without fee is hereby granted, provided that the above  
> copyright notice and this permission notice appear in all copies.  
>  
> THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES  
> WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF  
> MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR  
> ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES  
> WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN  
> ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF  
> OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

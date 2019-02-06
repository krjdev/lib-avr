# lib-avr

Device driver and libraries for Microchip (Atmel) AVR series

**Requires:**  
avr-gcc  
avr-libc

## I2C Interface (Master mode only)

**Files:**  
i2c/i2c.h  
i2c/i2c.c

**Functions:**  
int i2c\_master\_init(uint32\_t speed)  
int i2c\_master\_send(int opt, uint8\_t addr, uint8\_t cmd, int cmd\_len, uint8\_t *data, int data\_len)  
int i2c\_master\_recv(int opt, uint8\_t addr, uint8\_t cmd, int cmd\_len, uint8\_t *data, int data\_len)  
int i2c\_get\_error(void)

### NXP PCF8574(A) 8-Bit Remote I/O expander driver

**Files:**  
i2c/pcf8574.h  
i2c/pcf8574.c

**Functions:**  
gpio\_t *pcf8574\_init(int type, uint8\_t subaddr)  
void pcf8574\_free(gpio\_t *gpio)  
int pcf8574\_write(gpio\_t *gpio, uint8\_t data)  
int pcf8574\_read(gpio\_t *gpio, uint8\_t *data)  
int pcf8574\_set\_config(gpio\_t *gpio, int pin, int config)  
int pcf8574\_get\_config(gpio\_t *gpio, int pin, int *config)  
int pcf8574\_get\_npins(gpio\_t *gpio)  
int pcf8574\_set\_pin(gpio\_t *gpio, int pin, int value)  
int pcf8574\_get\_pin(gpio\_t *gpio, int pin, int *value)

### NXP PCF8591 8-Bit A/D and D/A converter driver (alpha version)

**Files:**  
i2c/pcf8591.h  
i2c/pcf8591.c

**Functions:**  
void pcf8591\_init(void)  
int pcf8591\_get\_adc(uint8\_t subaddr, int config, int channel, uint8\_t *value)  
int pcf8591\_set\_dac(uint8\_t subaddr, uint8\_t *value)

### STmicroelectronics M24C01 - M24C64 I2C EEPROM driver (alpha version)

**Files:**  
i2c/m24cxx.h  
i2c/m24cxx.c

**Functions:**  
void m24cxx\_init(void)  
int m24cxx\_write(int type, uint8\_t subaddr, uint16\_t addr, uint8\_t *buf, int len)  
int m24cxx_read(int type, uint8\_t subaddr, uint16\_t addr, uint8\_t *buf, int len)

## SPI Interface (Master mode only)

**Files:**  
spi/spi.h  
spi/spi.c

**Functions:**  
int spi\_master\_init(int mode, int speed)  
int spi\_master\_send(uint8\_t *data, int len)  
int spi\_master\_recv(uint8\_t *data, int len)

### Microchip ENC28J60 Ethernet controller driver

**Files:**  
spi/enc28j60.h  
spi/enc28j60.c

**Functions:**  
int enc28j60\_init(int mode, mac\_addr\_t *addr)  
int enc28j60\_send(eth\_frame\_t *frame)  
int enc28j60\_recv(eth\_frame\_t *frame)  
int enc28j60\_set\_mac(mac\_addr\_t *addr)  
int enc28j60\_get\_mac(mac\_addr\_t *addr)  
int enc28j60\_is\_link\_up(void)  
uint32\_t enc28j60\_get\_count\_rx\_frame(void)  
uint32\_t enc28j60\_get\_count\_tx\_frame(void)  
uint16\_t enc28j60\_get\_count\_rx\_err(void)  
uint16\_t enc28j60\_get\_count\_tx\_err(void)  
int enc28j60\_get\_free\_rx\_space(void)  
char *enc28j60\_get\_version(void)  
char *enc28j60\_get\_chip\_revision(void)  
int enc28j60\_get\_last\_error(void)

## Dallas/Maxim 1-Wire Interface (Software-based)

**Files:**  
onewire/sw-onewire.h  
onewire/sw-onewire.c

**Functions:**  
void onewire\_init(void)  
int onewire\_reset(void)  
int onewire\_send(uint8\_t *data, int len)  
int onewire\_recv(uint8\_t *data, int len)  
int onewire\_read\_rom(ow\_rom\_t *rom)  
int onewire\_search\_rom(int type, ow\_rom\_t *roms, int len)  
int onewire\_search\_family(int type, uint8\_t family, ow\_rom\_t *roms, int num)
int onewire\_match\_rom(ow\_rom\_t *rom)  
int onewire\_skip\_rom(void)  
int onewire\_get\_family(ow\_rom\_t *rom, uint8\_t *family)  
int onewire\_get\_serial(ow\_rom\_t *rom, uint8\_t *buf)  
int onewire\_get\_crc(ow\_rom\_t *rom, uint8\_t *crc)

### CRC check function for the 1-Wire interface

**Files:**  
lib/crc8\_dallas.h  
lib/crc8\_dallas.c

**Functions:**  
int crc8\_dallas\_calc(uint8\_t *data, int len)  
int crc8\_dallas\_check(uint8\_t *data, int len, uint8\_t crc)

### Dallas/Maxim DS18S20 and DS18B20 Digital Thermometer driver

**Files:**  
onewire/ds18x20.h  
onewire/ds18x20.c

**Functions:**  
void ds18x20\_init(void)  
int ds18x20\_read\_rom(int type, ow\_rom\_t *rom)  
int ds18x20\_search\_rom(int type, ow\_rom\_t *roms, int num)  
int ds18x20\_search\_alarm(int type, ow\_rom\_t *roms, int num)  
int ds18x20\_convert(ow\_rom\_t *rom)  
int ds18x20\_get\_temp(ow\_rom\_t *rom, int type, int res, float *temp)  
int ds18x20\_set\_resolution(ow\_rom\_t *rom, int res)  
int ds18x20\_set\_alarm(ow\_rom\_t *rom, int type, int8\_t temp\_high, int8\_t temp\_low)

### Dallas/Maxim DS2430A 256-Bit EEPROM driver

**Files:**  
onewire/ds2430a.h  
onewire/ds2430a.c

**Functions:**  
void ds2430a\_init(void)  
int ds2430a\_read\_rom(ow\_rom\_t *rom)  
int ds2430a\_search\_rom(ow\_rom\_t *roms, int num)  
int ds2430a\_write\_memory(ow\_rom\_t *rom, uint8\_t addr, uint8\_t *buf, int len)  
int ds2430a\_read\_memory(ow\_rom\_t *rom, uint8\_t addr, uint8\_t *buf, int len)  
int ds2430a\_write\_app\_reg(ow\_rom\_t *rom, uint8\_t addr, uint8\_t *buf, int len)  
int ds2430a\_read\_app\_reg(ow\_rom\_t *rom, uint8\_t addr, uint8\_t *buf, int len)  
int ds2430a\_lock\_app\_reg(ow\_rom\_t *rom)  
int ds2430a\_read\_status(ow\_rom\_t *rom, uint8\_t *status)

## Network libraries

### ARP definitins and helper functions

**Files:**  
net/arp.h  
net/arp.c

**Functions:**  
int arp_init(mac\_addr\_t *src\_mac, ipv4\_addr\_t *src_ip)  
int arp\_pkt\_set\_oper(arp\_packet\_t *arp, uint16\_t oper)  
int arp\_pkt\_set\_sha(arp\_packet\_t *arp, mac\_addr\_t *mac)  
int arp\_pkt\_set\_spa(arp\_packet\_t *arp, ipv4\_addr\_t *ip)  
int arp\_pkt\_set\_tha(arp\_packet\_t *arp, mac\_addr\_t *mac)  
int arp\_pkt\_set\_tpa(arp\_packet\_t *arp, ipv4\_addr\_t *ip)  
int arp\_pkt\_get\_oper(arp\_packet\_t *arp, uint16\_t *oper)  
int arp\_pkt\_get\_sha(arp\_packet\_t *arp, mac\_addr\_t *mac)  
int arp\_pkt\_get\_spa(arp\_packet\_t *arp, ipv4\_addr\_t *ip)  
int arp\_pkt\_get\_tha(arp\_packet\_t *arp, mac\_addr\_t *mac)  
int arp\_pkt\_get\_tpa(arp\_packet\_t *arp, ipv4\_addr\_t *ip)  
int arp\_pkt\_get\_len(arp\_packet\_t *arp)  
int arp\_buf\_to\_pkt(uint8\_t *buf, int len, arp\_packet\_t *arp)  
int arp\_pkt\_to\_buf(arp\_packet\_t *arp, uint8\_t *buf)  
int arp\_pkt\_valid(arp\_packet\_t *arp)  
int arp\_pkt\_create(arp\_packet\_t *arp)  
int arp\_pkt\_create_probe(arp\_packet\_t *arp)  
int arp\_pkt\_create\_query(ipv4\_addr\_t *dst\_ip, arp\_packet\_t *arp)  
int arp\_pkt\_create\_answer(arp\_packet\_t *arp\_in, arp\_packet\_t *arp\_out)


### Ethernet definitions and helper functions

**Files:**  
net/ethernet.h  
net/ethernet.c

**Functions:**  
int ethernet\_addr\_aton(const char *str, mac\_addr\_t *mac)  
int ethernet\_addr\_ntoa(mac\_addr\_t *mac, char *str)  
int ethernet\_addr\_cpy(mac\_addr\_t *dst, mac\_addr\_t *src)  
int ethernet\_addr\_equal(mac\_addr\_t *mac1, mac\_addr\_t *mac2)  
int ethernet\_frame\_set\_dst(eth\_frame\_t *frame, mac\_addr\_t *mac)  
int ethernet\_frame\_set\_src(eth\_frame\_t *frame, mac\_addr\_t *mac)  
int ethernet\_frame\_set\_type(eth\_frame\_t *frame, uint16\_t type)  
int ethernet\_frame\_set\_payload(eth\_frame\_t *frame, uint8\_t *buf, int len)  
int ethernet\_frame\_get\_dst(eth\_frame\_t *frame, mac\_addr\_t *mac)  
int ethernet\_frame\_get\_src(eth\_frame\_t *frame, mac\_addr\_t *mac)  
int ethernet\_frame\_get\_type(eth\_frame\_t *frame, uint16\_t *type)  
int ethernet\_frame\_get\_payload\_len(eth\_frame\_t *frame)  
int ethernet\_frame\_get\_payload(eth\_frame\_t *frame, uint8\_t **buf)  
int ethernet\_frame\_get\_len(eth\_frame\_t *frame)  
int ethernet\_frame\_payload\_free(eth\_frame\_t *frame)  
int ethernet\_buf\_to\_frm(uint8\_t *buf, int len, eth\_frame\_t *frame)  
int ethernet\_frm\_to\_buf(eth\_frame\_t *frame, uint8\_t *buf);

### IPv4 definitions and helper functions

**Files:**  
net/ipv4.h  
net/ipv4.c

**Functions:**  
ipv4\_addr\_t *ipv4\_str\_to\_ip(const char *str)  
char *ipv4\_ip\_to\_str(ipv4\_addr\_t *ip)

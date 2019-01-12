# lib-avr

Device driver and libraries for Microchip (Atmel) AVR Series

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
void m24cxx_init(void)  
int m24cxx_write(int type, uint8\_t subaddr, uint16\_t addr, uint8\_t *buf, int len)  
int m24cxx_read(int type, uint8\_t subaddr, uint16\_t addr, uint8\_t *buf, int len)

## SPI Interface (Master mode only)

**Files:**  
spi/spi.h  
spi/spi.c

**Functions:**  
int spi\_master\_init(int mode, int speed)  
int spi\_master\_send(uint8\_t *data, int len)  
int spi\_master\_recv(uint8\_t *data, int len)

### Microchip ENC28J60 Ethernet controller driver (alpha version - No receive function)

**Files:**  
spi/enc28j60.h  
spi/enc28j60.c

**Functions:**  
void enc28j60\_init(int mode, mac\_addr\_t *addr)  
uint8\_t enc28j60\_get\_revision(void)  
void enc28j60\_set\_mac(mac\_addr\_t *addr)  
void enc28j60\_get\_mac(mac\_addr\_t *addr)  
int enc28j60\_link\_up(void)  
int enc28j60\_frame\_count(void)  
uint16\_t enc28j60\_free\_space(void)  
int enc28j60\_send(eth\_frame\_t *frame)

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

### Dallas/Maxim DS2430A 256-Bit EEPROM driver (Alpha version - Single device)

**Files:**  
onewire/ds2430a.h  
onewire/ds2430a.c

**Functions:**  
void ds2430a\_init(void)  
int ds2430a\_write_\memory(uint8\_t addr, uint8\_t *buf, int len)  
int ds2430a_\read\_memory(uint8\_t addr, uint8\_t *buf, int len)  
int ds2430a\_write\_app\_reg(uint8\_t addr, uint8\_t *buf, int len)  
int ds2430a\_read\_app\_reg(uint8\_t addr, uint8\_t *buf, int len)  
int ds2430a\_lock\_app\_reg(void)  
int ds2430a\_read\_status(uint8\_t *status)

## Network libraries

### Ethernet definitions and helper functions

**Files:**  
net/ethernet.h  
net/ethernet.c

**Functions:**  
mac\_addr\_t *ethernet\_str\_to\_mac(const char *str)  
char *ethernet\_mac\_to\_str(mac\_addr\_t *mac)  
int ethernet\_frame\_set\_dst(eth\_frame\_t *frame, const char *addr)  
int ethernet\_frame\_get\_dst(eth\_frame\_t *frame, char **str)  
int ethernet\_frame\_set\_src(eth\_frame\_t *frame, const char *addr)  
int ethernet\_frame\_get\_src(eth\_frame_t *frame, char **str)  
int ethernet\_frame\_set\_type(eth\_frame\_t *frame, uint16\_t type)  
int ethernet\_frame\_get\_type(eth\_frame\_t *frame, uint16\_t *type)  
int ethernet\_frame\_set\_payload(eth\_frame\_t *frame, uint8\_t *buffer, int length)  
int ethernet\_frame\_get\_payload(eth\_frame\_t *frame, uint8\_t **buffer, int *length)  
int ethernet\_buffer\_to\_frame(uint8\_t *buffer, int length, eth\_frame\_t **frame)  
int ethernet\_frame\_to\_buffer(eth\_frame\_t *frame, uint8\_t **buffer, int *length)

### IPv4 definitions and helper functions

**Files:**  
net/ipv4.h  
net/ipv4.c

**Functions:**  
ipv4\_addr\_t *ipv4\_str\_to\_ip(const char *str)  
char *ipv4\_ip\_to\_str(ipv4\_addr\_t *ip)

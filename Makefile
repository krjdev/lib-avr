# lib-avr
# 
# Global Makefile

GLOBAL			= yes

ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE		= avr-
endif

# Target output
TARGET_LIBAVR		= lib-avr.a

ifeq ($(TARGET_MCU),)
TARGET_MCU		= atmega2560
endif

TARGET_F_OSC		= 16000000

# Generic commands
RM			= rm -rf
CP			= cp -Rf

# Toolchain
CC			= $(CROSS_COMPILE)gcc
LD			= $(CROSS_COMPILE)ld
AR			= $(CROSS_COMPILE)ar
OBJCOPY			= $(CROSS_COMPILE)objcopy

# Generic assembler flags
ASFLAGS			+= -Wa,-mmcu=$(TARGET_MCU)
ASFLAGS			+= -D_ASM_ASSEMBLER_

# lib-avr assembler flags
ASFLAGS_LIBAVR		= $(ASFLAGS)

# Generic C compiler flags
CCFLAGS			+= -Wall
CCFLAGS			+= -Wextra
CCFLAGS			+= -ffunction-sections
CCFLAGS			+= -mmcu=$(TARGET_MCU)

# lib-avr C compiler flags
CCFLAGS_LIBAVR		= $(CCFLAGS)
CCFLAGS_LIBAVR		+= -DF_OSC=$(TARGET_F_OSC)
CCFLAGS_LIBAVR		+= -DF_CPU=$(TARGET_F_OSC)
CCFLAGS_LIBAVR		+= -lm

include ./files.mk

AOBJ_LIBAVR		+= $(ASRC_LIBAVR:.S=.o)
COBJ_LIBAVR		+= $(CSRC_LIBAVR:.c=.o)

all: libavr

# lib-avr
# 
# Create static library
libavr: $(AOBJ_LIBAVR) $(COBJ_LIBAVR)
	@echo "[AR] $(TARGET_LIBAVR)"
	@$(AR) -ruc $(TARGET_LIBAVR) $(AOBJ_LIBAVR) $(COBJ_LIBAVR)

# Compile assembler sources
$(AOBJ_LIBAVR): %.o: %.S
	@echo "[AS] $@"
	@$(CC) -c $(CCFLAGS_LIBAVR) $(ASFLAGS_LIBAVR) -o $@ $<

# Compile C sources
$(COBJ_LIBAVR): %.o: %.c
	@echo "[CC] $@"
	@$(CC) -c $(CCFLAGS_LIBAVR) -o $@ $<

.PHONY: clean
clean:
	@echo "[CLEAN]"
	@$(RM) $(AOBJ_LIBAVR) $(COBJ_LIBAVR)
	@$(RM) $(TARGET_LIBAVR)

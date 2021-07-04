# lib-avr
#
# Local Makefile

CCFLAGS_LIBAVR		+= -I ./include

include ./lib/files.mk

CSRC_LIBAVR		+= ./uart.c

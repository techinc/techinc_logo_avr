# ======================================================================
# Template Makefile
#
# Copyright 2006-2010 Dick Streefland
#
# This is free software, licensed under the terms of the GNU General
# Public License as published by the Free Software Foundation.
# ======================================================================

TARGET_ARCH	= -mmcu=attiny85
OBJECTS		= main.o
FLASH_CMD	= avrdude -c usbtiny -pt85 -U flash:w:main.hex 
FUSES_CMD	= avrdude -c usbtiny -pt85 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
STACK		= 32
FLASH		= 8192
SRAM		= 512

include common.mk

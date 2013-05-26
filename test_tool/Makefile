# Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. -----------------#
# Allegro --------------------------------------------------------------------#

CC = gcc
CFLAGS = -m386 -msoft-float -fno-builtin
LD = ld
AS86 = as86
LD86 = ld86
LDFLAGS = -n -Ttext=0x3400 -static

MAINOBJS = fat.o

all: fat.o
	$(CC) fat.o
%.o	: %.c
	$(CC) -c $<

clean:
	rm -rf *.o

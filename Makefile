# Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. -----------------#
# Allegro --------------------------------------------------------------------#

CC = gcc
CFLAGS = -Wall
LD = ld
AS86 = as86
LD86 = ld86
LDFLAGS = -n -Ttext=0x3400 -static
ITRON_DIR = ../kernel
ITRON =	$(ITRON_DIR)/libkernel.a\
	$(ITRON_DIR)/net/libknet.a\
	$(ITRON_DIR)/net/libnet.a\
	../lib/libc.a

MAINOBJS = run.o main.o i8259.o video.o klib.o intr.o interrupt.o floppy.o\
	syscall.o keyboard.o pcmcia.o ne2000.o proc.o tss.o 386.o\
	timer.o kernelval.o $(ITRON)
#MAINOBJS = run.o main.o i8259.o video.o klib.o intr.o interrupt.o
SMPOBJS = $(MAINOBJS) smp.o

all: start genasm smpmain table
	cat boot/boot table start smpmain > smpi386
#	cat boot/boot table start main > i386

# start (16 bit boot) --------------------------------------------------------#
start: start.o
	$(LD86) -0 -s -o _start start.o
	dd if=_start of=start bs=16w skip=1
	rm -f _start
start.o: start.s
	$(AS86) -0 -a -o start.o start.s

smpmain	: elf $(SMPOBJS)
	$(LD) $(LDFLAGS) -e run -o _smpmain $(SMPOBJS)
	strip _smpmain
	cp	/dev/null smpmain
	./elf _smpmain smpmain
#	rm -f _smpmain

main	: elf $(MAINOBJS)
	$(LD) $(LDFLAGS) -e run -o _main $(MAINOBJS)
	strip _main
	./elf _main main
	rm -f _main
#	$(LD) $(LDFLAGS) -e bootstrap -o _u bootstrap.o u.o
#	strip _u
#	dd if=_u of=u bs=16w skip=1
#	rm _u
elf	: elf.c
	$(CC) -o elf elf.c
%.o	: %.c
	$(CC) -c $(CFLAGS) $<

table: table.o
	$(LD86) -0 -s -o _table table.o
	dd if=_table of=table bs=16w skip=1
	rm -f _table
table.o: table.s
	$(AS86) -0 -a -o table.o table.s
table.s: genasm
	./genasm

genasm: genasm.c
	$(CC) -o genasm genasm.c

clean:
	rm -rf *.o
	rm table.s
	rm genasm

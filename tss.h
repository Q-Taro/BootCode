/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#ifndef _TSS_H
#define _TSS_H

/* tss ----------------------------------------------------------------------*/
typedef struct tss {
	unsigned short	prev_link;
	unsigned short	dummy0;
	unsigned long	esp0;
	unsigned short	ss0;
	unsigned short	dummy1;
	unsigned long	esp1;
	unsigned short	ss1;
	unsigned short	dummy2;
	unsigned long	esp2;
	unsigned short	ss2;
	unsigned short	dummy3;
	unsigned long	cr3;
	unsigned long	eip;
	unsigned long	eflags;
	unsigned long	eax;
	unsigned long	ecx;
	unsigned long	edx;
	unsigned long	ebx;
	unsigned long	esp;
	unsigned long	ebp;
	unsigned long	esi;
	unsigned long	edi;
	unsigned short	es;
	unsigned short	dummy4;
	unsigned short	cs;
	unsigned short	dummy5;
	unsigned short	ss;
	unsigned short	dummy6;
	unsigned short	ds;
	unsigned short	dummy7;
	unsigned short	fs;
	unsigned short	dummy8;
	unsigned short	gs;
	unsigned short	dummy9;
	unsigned short	ldt;
	unsigned short	dummy10;
	unsigned short	t;
	unsigned short	io_base;
} tss_t;

/* function declarations ----------------------------------------------------*/
void tss_init();
void set_tss(tss_t*, int (*)(), unsigned short, unsigned short,
		unsigned short, unsigned char*,
		unsigned short, unsigned char*,
		unsigned long);
#endif

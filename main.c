/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* main ---------------------------------------------------------------------*/
#include "../include/itron.h"
#include "../include/types.h"
#include "../kernel/kernel.h"
#include "io.h"
#include "addr.h"
#include "klib.h"
#include "video.h"
#include "i8259.h"
#include "floppy.h"
#include "tss.h"
#include "smp.h"
#include "interrupt.h"
#include "timer.h"
#include "proc.h"
#include "pcmcia.h"
#include "ne2000.h"
#include "keyboard.h"
#include "mainP.h"

int	cpu_num = 0;		/* cpu id */

/* main ---------------------------------------------------------------------*/
int
main()
{
#if 0
	char*	buf = (char*)FDC_BUFFER;
	long	k;
	int	data;
#endif
	ccli();
	if (cpu_num == 0) {
		all_init();		/* i386 init */
		itron_init();		/* itron init */
		tss_init();		/* tss init */
		cltr(SEL_TSS_DUMMY0);
	} else
		cltr(SEL_TSS_DUMMY1);
	smp_init();			/* smp init */
#if 0
	fdc_start_motor();
	k = 0;
	while (1) {
		fdc_read(k, buf);
		data = (int)buf[0];
		printk(">>%x\n", data);
		k ++;
	}
	video_test2();
#endif
	return 0;
}

/* all_init -----------------------------------------------------------------*/
static void
all_init()
{
	idt_init();
	video_init();		/* 6845 */
	printk("SMP MicroITRON/386 for PC/AT\n");
	printk("Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved.\n");
	printk("\n");

	i8259_init();
	timer_init();

	proc_init();
#if 0
	fdc_init();		/* floppy disk drive */
#endif
	pcmcia_init();		/* only for note-pc */
	ne2000_init();
	key_init();		/* keyboard */
}

#if 0 /* it may or should by called when cpu is real mode -------------------*/
/* a20_init -----------------------------------------------------------------*/
/* don't call me while disabling interrupt. (or protect mode) ---------------*/
void
a20_init()
{
	while (inb(0x64) & 0x02);
	outb((unsigned long)0x64, (unsigned long)0xd1);
	while (inb(0x64) & 0x02);
	outb((unsigned long)0x60, (unsigned long)0xdf);
	while (inb(0x64) & 0x02);
/*
	outb((unsigned long)0x64, (unsigned long)0xff);
	while (inb(0x64) & 0x02);
*/
}
#endif

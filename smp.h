/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#ifndef _SMP_H
#define _SMP_H

#define APIC_ID		0x0fee00020
#define APIC_EOI	0x0fee000b0
#define TIMER		0x0fee00320
extern unsigned long*	apic_id; 	

#define	APIC_REQ()	unsigned char	acpi;\
			if (cpu_num)\
				apic = ((*apic_id) >> 24) & 0xff;\
			else\
				apic = 0;

void smp_init();
void smp_eoi();
void smp_lock(unsigned long);
void smp_unlock(unsigned long);
void smp_lvt_init();
void smp_timer(int);

#endif


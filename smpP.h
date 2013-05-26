/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#ifndef _SMPP_H
#define _SMPP_H

/* APIC registers -----------------------------------------------------------*/
#define ICR_LOW			0x0fee00300
#define ICR_HI			0x0fee00310
#define SVR			0x0fee000f0
#define LVT3			0x0fee00370
#define APIC_ENABLED		0x100
#define LVT1			0x0fee00350
#define LVT2			0x0fee00360
#define LVT3			0x0fee00370
#define TIMER_INIT_COUNT	0x0fee00380
#define TIMER_CURR_COUNT	0x0fee00390
#define APIC_VERSION		0x0fee00030

/* 18.2 Hz ------------------------------------------------------------------*/
#define BASE_CLOCK		(100 * 1024 * 1024)
#define MAX_TIMER_COUNT		5761406


/* cpu ----------------------------------------------------------------------*/
typedef struct cpu {
	unsigned long	id;
	unsigned long	id2;
} t_cpu;

t_cpu	cpu[2];	/* support only 2 processors */

#endif

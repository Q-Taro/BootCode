/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* SMP (Symmetric Multi Processor. only 2 CPUs supported.) ------------------*/
/* I don't know APIC ID of start up CPU, but 2nd CPU has 0 of APIC ID. ------*/
/* first CPU's APIC_ID is 0x01000000, second CPU's APIC_ID is 0 -------------*/
#include "interrupt.h"
#include "smp.h"
#include "smpP.h"

unsigned long*	apic_id = (unsigned long*)APIC_ID;
extern int	cpu_num;
static int	cpu_second = 0;

extern int	smp_cpu_lock[];

/* smp_lock -----------------------------------------------------------------*/
void
smp_lock(unsigned long p)
{
	while (cxchg(p, 1)) 
		;
}

/* smp_unlock ---------------------------------------------------------------*/
void
smp_unlock(unsigned long p)
{
	cxchg(p, 0);
}

/* smp_eoi ------------------------------------------------------------------*/
void
smp_eoi()
{
	unsigned long*	p = APIC_EOI;
	*p = 0L;
}

/* smp_init -----------------------------------------------------------------*/
void
smp_init()
{
	unsigned long*	p;
	unsigned long	a;

	/* pre-check --------------------------------------------------------*/
	p = (unsigned long*)APIC_VERSION; 
	*p = 0L;
	if (*p == 0 || *p == -1) {
		printk("Maybe Single Processor Model...\n");
		csti();
		start_first_task();
		return;		/* never executed */
	}

	ccli();
	/* step 3 -----------------------------------------------------------*/
	
	/* step 6 -----------------------------------------------------------*/
	p = (unsigned long*)APIC_ID;
	cpu[cpu_num].id = *p & 0xf000000;

	/* step 7 -----------------------------------------------------------*/
	cpu[cpu_num].id2 = cpu[cpu_num].id ^ 0x1000000;
	
	/* step 9 -----------------------------------------------------------*/
	p = (unsigned long*)SVR;
	a = *p;
	*p = (a | APIC_ENABLED);

	/* step 10 ----------------------------------------------------------*/
	p = (unsigned long*)LVT3;
	a = *p;
	*p = (a & (0xffffff00 | VECT_APIC));
#if 1
	/* step 11 --------------------------------------------------*/
	p = (unsigned long*)ICR_HI;
	a = *p;
#if 0
	/* this means broadcast.
	 * resutl is ok but, i won't use this.
	 */
	*p = ((a & 0xf0ffffff) | 0x0f000000);
#endif
	*p = ((a & 0xf0ffffff) | cpu[cpu_num].id2);
#endif
	if (cpu_num == 0) {
		/* first CPU ------------------------------------------------*/
		printk("First CPU initialize ...\n");

		/* step 14 --------------------------------------------------*/
		p = (unsigned long*)ICR_LOW;
		a = *p;	
		/* logical 
		*p = ((a & (0xfff0f800) | 0x603));
		*/
		/* physical -------------------------------------------------*/
		*p = ((a & (0xfff0f000) | 0x603));
		irq_mask_on(1);	/* mask RTC ---------------------------------*/
		smp_timer(cpu_num);
		cpu_num ++;
		while (cpu_second == 0);
		timer_start();
		csti();
		smp_cpu_lock[0] = smp_cpu_lock[1] = 0;
		if (*apic_id == 0)
			start_first_task();
		else
			start_second_task();
	} else {
		cpu_second = 1;
		p = (unsigned long*)ICR_LOW;
		a = *p;	

		/* physical -------------------------------------------------*/
		*p = ((a & (0xfff0f000) | 0x603));

		/* second CPU -----------------------------------------------*/
		printk("Second CPU initialize ...\n");
		smp_lvt_init();
		smp_timer(cpu_num);
		timer_start();
		csti();
		if (*apic_id == 0)
			start_first_task();
		else
			start_second_task();
	}
}

/* smp_lvt_init -------------------------------------------------------------*/
void
smp_lvt_init()
{
	unsigned long*	p;
	unsigned long	a;
	/* lvt1 -------------------------------------------------------------*/
	p = (unsigned long*)LVT1;
	a =  *p & 0xfffe58ff;
	a |= 0x700;	
	*p = a;
	/* lvt2 -------------------------------------------------------------*/
	p = (unsigned long*)LVT2;
	a =  *p & 0xfffe58ff;
	a |= 0x400;
	*p = a;
}

/* smp_timer ----------------------------------------------------------------*/
void
smp_timer(int cpu_num)
{
	unsigned long*	p = (unsigned long*)TIMER;
	if (*apic_id == 0)
		*p = (unsigned long)(0x00020000 | VECT_SMP_TIMER0);
	else
		*p = (unsigned long)(0x00020000 | VECT_SMP_TIMER1);

	p = (unsigned long)TIMER_INIT_COUNT;
#if 0
	*p = 10000000L;
#else
	*p = MAX_TIMER_COUNT;		/* 18.2 Hz --------------------------*/
#endif
}

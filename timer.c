/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#include "../include/itron.h"
#include "../include/types.h"
#include "proc.h"
#include "../kernel/types.h"
#include "../kernel/sched.h"
#include "video.h"
#include "klib.h"
#include "io.h"
#include "smp.h"
#include "interrupt.h"
#include "kernelval.h"
#include "timerP.h"

/* timer_init ---------------------------------------------------------------*/
void
timer_init()
{
	unsigned long	count;
	count = FREQ / HZ;
	outb(IO_TIMER_C, (unsigned long)SQUARE);
	outb(IO_TIMER_0, (unsigned long)(count & 0xff));
	outb(IO_TIMER_0, (unsigned long)((count >> 8) & 0xff));
	printk("timer: initialized\n");
}

/* timer_intr (must be called only once even if number of processor is 2) ---*/
void
timer_intr(unsigned char apic, unsigned long delta)
{
	sched_timeout(apic, delta);
}

/* timer_stop ---------------------------------------------------------------*/
void
timer_stop()
{
	unsigned long*	p = (unsigned long*)TIMER;
	if (cpu_num == 0) {
		irq_mask_on(1);
	} else {
		*p |= 0x00010000;
	}
}

/* timer_start --------------------------------------------------------------*/
void
timer_start()
{
	unsigned long*	p = (unsigned long*)TIMER;
	if (cpu_num == 0) {
		irq_mask_off(1);
	} else {
		if (*p & 0x00010000)
			*p ^= 0x00010000;
	}
}

/* timer_dump (for debug) ---------------------------------------------------*/
void
timer_dump()
{
#if 0
	extern T_TIMEOUT	timeout;
	T_TIMEOUT*	tp = timeout.next;
	T_TSK*	t;
	unsigned long k;
	int	i;
	i = 0;
#if 0
	while (tp != &timeout) {
		printk("TD%d: tskid(%x), delta(%d)\n", i, tp->tskid, tp->delta);
		tp = tp->next;
		i ++;
	for (k = 0 ; k < 100000000 ; k ++); 
	}
#else
	tp = &timeout;
	printk("TD-0: addr(%x),tskid(), delta(%d), next(%x), prev(%x)\n",tp, tp->delta, tp->next, tp->prev);
	tp = tp->next;
	t = tlink2tsk(tp);
	printk("TD-1: addr(%x),tskid(%x), delta(%d), next(%x), prev(%x)\n",tp, t->tskid, tp->delta, tp->next, tp->prev);

	for (k = 0 ; k < 1000000000 ; k ++); 
#endif
	
#endif
}

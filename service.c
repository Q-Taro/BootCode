/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#include "../include/itron.h"
#include "smp.h"
#include "interrupt.h"

extern int	cpu_num;
extern ID	c_tskid[];

/* cpu_lock -----------------------------------------------------------------*/
void
cpu_lock()
{
	if (cpu_num == 0)
		irq_mask_on(0xfffe);
	else {
		smp_cpu_lock();
	}
}

/* cpu_unlock ---------------------------------------------------------------*/
void
cpu_unlock()
{
	if (cpu_num == 0)
		irq_mask_off(0xfffe);
	else {
		smp_cpu_unlock();
	}
}

unsigned long sclk[2] = {0, 0};

/* smp_cpu_lock -------------------------------------------------------------*/
void
smp_cpu_lock()
{
#if 0
	if (*apic_id != 0) {	/* apic == 1 */
		if (sclk[0] == 0) {
			sclk[0] = 1;
			while (sclk[0] == 1)
				;
			proc_eflags_save(c_tskid[1]);
		}
	} else {
		if (sclk[1] == 0) {
			sclk[1] = 1;
			while (sclk[1] == 1)
				;
			proc_eflags_save(c_tskid[0]);
		}
	}
#if 0	/* don't stop timer */
	timer_stop();
#endif
#else
	if (*apic_id != 0) {	/* id 1 */
	if (sclk[0] == 0) {
		sclk[0] = 1;
		while (sclk[0] == 1)
			;
	}
	} else {
	if (sclk[1] == 0) {
		sclk[1] = 1;
		while (sclk[1] == 1)
			;
	}
	}
	if (*apic_id)
		proc_eflags_save(c_tskid[1]);
	else
		proc_eflags_save(c_tskid[0]);
#endif
}

/* smp_cpu_unlock -----------------------------------------------------------*/
void
smp_cpu_unlock()
{
#if 0
	if (*apic_id != 0) {	/* apic == 1 */	
		sclk[0] = 0;
		proc_eflags_restore(c_tskid[1]);
	} else {
		sclk[1] = 0;
		proc_eflags_restore(c_tskid[0]);
	}
#if 0	/* don't stop timer */
	timer_start();
#endif
#else
	sclk[0] = sclk[1] = 0;
	if (*apic_id)
		proc_eflags_restore(c_tskid[1]);
	else
		proc_eflags_restore(c_tskid[0]);
#endif
}

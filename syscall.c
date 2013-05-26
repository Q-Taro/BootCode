/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#include "klib.h" 
#include "../include/itron.h"
#include "../include/syscall.h"
#include "kernelval.h"

W tron_syscall(unsigned long, unsigned long,
		unsigned long, unsigned long, unsigned long,
		unsigned long, unsigned long, unsigned long,
		unsigned long);

/* c_intr_sysall ------------------------------------------------------------*/
W
c_intr_syscall(unsigned long apic, unsigned long sysid,
		unsigned long arg1, unsigned long arg2, unsigned long arg3,
		unsigned long arg4, unsigned long arg5, unsigned long arg6,
		unsigned long arg7)
{
#if 0
	extern unsigned long	k_nest;
	short	cs = get_cs();
	short	ds = get_ds();
	short	ss = get_ss();
#endif
	W	ret;

	if (apic)
		apic = 1;
	if (cpu_num == 0)
		apic = 0;
#if 0
	(*tron_syscall_p)(sysid, arg1, arg2, arg3);
	printk("SYSCALL(%x, %x, %x, %x, %x)\n", sysid, apic, arg1, arg2, arg3);
	printk("SYSCALL(%x, %x, %x, %x)\n", sysid, apic, c_tskid[apic], arg1);
	printk("c_tskid = %x, cuurentproc-id = %x(%x)\n", c_tskid[apic],
		current_proc[apic]->id, k_nest);
#else
	ret = tron_syscall(apic, sysid,
			arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	current_proc[apic]->reg[EAX] = ret;	/* set return value */
	return ret;
#endif
}

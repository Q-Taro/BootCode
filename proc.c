/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#include "../include/itron.h"
#include "../include/types.h"
#include "proc.h"
#include "../kernel/types.h"
#include "../kernel/sys_tsk.h"
#include "../kernel/pool.h"
#include "../kernel/sched.h"
#include "addr.h" 
#include "interrupt.h"
#include "procP.h"
#include "kernelval.h"

/* proc_init ----------------------------------------------------------------*/
void
proc_init()
{
	int	i;
	T_CTSK	ctsk;
	void first_task();
	void second_task();
	extern	T_TSK	tsk[];

	for (i = 0 ; i <= MAX_TSKID ; i ++) {
		proc[i].stack = (unsigned long)&proc[i].reg[0];
		proc[i].id = i;
	}

	/* first_task -------------------------------------------------------*/
	ctsk.task = (FP)first_task;
	ctsk.stk = stack_alloc(1024);
	ctsk.stksz = 1024;
	ctsk.itskpri = TMAX_TPRI;
	sys_cre_tsk(0, 1, &ctsk);
	sys_act_tsk(0, 1);
	tsk_stat_change(1, TTS_RUN);
	sched_ins(tsk[1].tskpri, &tsk[1].plink);

	/* second_task ------------------------------------------------------*/
	ctsk.task = (FP)second_task;
	ctsk.stk = stack_alloc(1024);
	ctsk.stksz = 1024;
	ctsk.itskpri = TMAX_TPRI;
	sys_cre_tsk(1, 2, &ctsk);
	sys_act_tsk(1, 2);
	tsk_stat_change(2, TTS_RUN);
	sched_ins(tsk[2].tskpri, &tsk[2].plink);

	/* for first CPU ----------------------------------------------------*/
	current_proc[0] = &proc[1];
	c_tskid[0] = 1;

	/* for second CPU ---------------------------------------------------*/
	current_proc[1] = &proc[2];
	c_tskid[1] = 2;

	/* timer variable initialize ----------------------------------------*/
	timer_return[0] = timer_return[1] = 0;
	clock_tick[0] = clock_tick[1] = 0;
	lost_tick[0] = lost_tick[1] = 0;
}

/* proc_create --------------------------------------------------------------*/
proc_t*
proc_create(ID tskid, T_CTSK* pk_ctsk)
{
	ER proc_set_tsk_arg(ID, VP_INT);
	char*	p = (char*)&(proc[tskid].reg[0]);
	/* what is 48? see intr.s -------------------------------------------*/
	proc[tskid].stack = (unsigned long)(p + 52);
	proc[tskid].reg[EAX] = 0;
	proc[tskid].reg[EBX] = 0;
	proc[tskid].reg[ECX] = 0;
	proc[tskid].reg[EDX] = 0;
	proc[tskid].reg[ESI] = 0;
	proc[tskid].reg[EDI] = 0;
	proc[tskid].reg[EBP] = 0;
	proc[tskid].reg[EFLAGS] = INIT_EFLAGS;
	proc[tskid].reg[EIP] = (unsigned long)pk_ctsk->task;
	proc[tskid].reg[ESP] = (unsigned long)pk_ctsk->stk & 0xfffffffc;
	proc[tskid].reg[DS] = SEL_U32_D;
	proc[tskid].reg[ES] = SEL_U32_D;

	proc[tskid].flag = FLAG_USE;
	proc_set_tsk_arg(tskid, pk_ctsk->exinf);

	return proc;
}

/* proc_set_tsk_arg ---------------------------------------------------------*/
ER
proc_set_tsk_arg(ID tskid, VP_INT arg)
{
	extern void sys_ext_tsk();
	unsigned long	p = (unsigned long)arg;
	unsigned long*	esp = (unsigned long*)proc[tskid].reg[ESP];
	*esp -- = (unsigned long)p;	/* 4 byte */
	*esp = (unsigned long)sys_ext_tsk;	/* return to ext_tsk */

	proc[tskid].reg[ESP] -= 4;
	return E_OK;
}

/* proc_delete --------------------------------------------------------------*/
ER
proc_delete(ID tskid)
{
	/* empty */	
	proc[tskid].flag = FLAG_NON;
	return E_OK;
}

/* proc_eflags_save ---------------------------------------------------------*/
void
proc_eflags_save(ID tskid)
{
printk("id[%d].eflags = %x\n", tskid, proc[tskid].reg[EFLAGS]);
return;
	if ((proc[tskid].reg[EFLAGS] & 0x00000200) == 1)
		proc[tskid].reg[SAV_EFLAGS] = proc[tskid].reg[EFLAGS];
	proc[tskid].reg[EFLAGS] &= 0xfffffdff;
}

/* proc_eflags_restore ------------------------------------------------------*/
void
proc_eflags_restore(ID tskid)
{
return;
	proc[tskid].reg[EFLAGS] = proc[tskid].reg[SAV_EFLAGS];
}

void
proc_switch()
{
	static int i = 0;
return;
	if (i == 0) {
		if (proc[2].flag == 0)
			return;
		current_proc[0] = &proc[2];
		printk("--------------task2 start ------------\n");
		i = 1;
	} else {
		printk("--------------task1 start ------------\n");
		current_proc[0] = &proc[1];
		i = 0;
	} 
}


/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#include "../include/itron.h"
#include "../include/types.h"
#include "proc.h"
#include "addr.h"
#include "386.h"
#include "tss.h"
#include "tssP.h"

/* tss_init -----------------------------------------------------------------*/
void
tss_init()
{
	void	first_task();
	void	second_task();
	/* first_task -------------------------------------------------------*/
	set_tss(&tss0, (int (*)())first_task, SEL_U32_C | 3, SEL_U32_D,
		SEL_U32_S | 3, (unsigned char*)CPU0_SP3,
		SEL_K32_S, (unsigned char*)CPU0_SP0, INIT_EFLAGS);
	set_tss(&tss_dummy0, (int (*)())first_task, SEL_K32_C, SEL_K32_D,
		SEL_K32_S, (unsigned char*)CPU0_SP3,
		SEL_K32_S, (unsigned char*)CPU0_SP0, INIT_EFLAGS);
	set_gdt(SEL_TSS0, (unsigned long)&tss0,
		(unsigned long)sizeof(tss_t), ST_TSS, 0, _32BIT);
	set_gdt(SEL_TSS_DUMMY0, (unsigned long)&tss_dummy0,
		(unsigned long)sizeof(tss_t), ST_TSS, 0, _32BIT);
	/* second_task ------------------------------------------------------*/
	set_tss(&tss1, (int (*)())second_task, SEL_U32_C | 3, SEL_U32_D,
		SEL_U32_S | 3, (unsigned char*)CPU1_SP3,
		SEL_K32_S, (unsigned char*)CPU1_SP0, INIT_EFLAGS);
	set_tss(&tss_dummy1, (int (*)())second_task, SEL_K32_C, SEL_K32_D,
		SEL_K32_S, (unsigned char*)CPU1_SP3,
		SEL_K32_S, (unsigned char*)CPU1_SP0, INIT_EFLAGS);
	set_gdt(SEL_TSS1, (unsigned long)&tss1,
		(unsigned long)sizeof(tss_t), ST_TSS, 0, _32BIT);
	set_gdt(SEL_TSS_DUMMY1, (unsigned long)&tss_dummy1,
		(unsigned long)sizeof(tss_t), ST_TSS, 0, _32BIT);
}


/* set_tss ------------------------------------------------------------------*/
void
set_tss(tss_t* t, int (*func)(),
		unsigned short cs, unsigned short ds,
		unsigned short ss, unsigned char* esp,
		unsigned short ss0, unsigned char* esp0,
		unsigned long eflags)
{
	int	i;
	char*	p = (char*)t;

	for (i = 0 ; i < sizeof(tss_t) ; i ++)
		*p ++ = '\0';

	t->eip = (unsigned long)func;
	t->cs = cs;
	t->ds = ds;
	t->esp = (unsigned long)esp;
	t->ss = ss;
	t->esp0 = (unsigned long)esp0;
	t->ss0 = ss0;
	t->eflags = eflags;
}

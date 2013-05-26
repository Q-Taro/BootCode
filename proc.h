/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#ifndef _PROC_H
#define _PROC_H

/* proc ---------------------------------------------------------------------*/
typedef struct proc {
	unsigned long	stack;		/* pints to stack */
	unsigned long	reg[256];	/* register pool */
#if 0
	unsigned long	eip;		/* program counter */
	unsigned long	stack;		/* stack pointer */
#endif
	int		flag;
	int		id;		/* equal to task id */
} proc_t;

/* register offset ----------------------------------------------------------*/
#define	ECX		0
#define EDX		1
#define ESP		2
#define EIP		3

#define EBP		4
#define ESI		5
#define EDI		6
#define EFLAGS		7

#define EAX		8
#define EBX		9
#define DS		10
#define ES		11
#define SAV_EFLAGS	12

#define FLAG_USE	1
#define FLAG_NON	0

/* function declarations ----------------------------------------------------*/
void proc_init();
proc_t* proc_create(ID, T_CTSK*);
ER proc_set_tsk_arg(ID, VP_INT);
ER proc_delete(ID);
void proc_eflags_save(ID);
void proc_eflags_restore(ID);
void proc_switch();
extern proc_t*	current_proc[];

#endif

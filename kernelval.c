/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#include "../include/config.h"
#include "kernelval.h"

/* pointer to proc ----------------------------------------------------------*/
proc_t*	current_proc[16];	/* points to current process */

/* proc ---------------------------------------------------------------------*/
proc_t	proc[MAX_TSKID];	/* process table */
int	cpu_num;		/* 0:single 1:double */
ID	c_tskid[MAX_TSKID];	/* current task id */

/* cpu_status ---------------------------------------------------------------*/
int	cpu_stat;		/* CPU_LOCK or CPU_UNLOCK */
int	dispatch_stat;		/* DISPATCH_ENABLE or DISPATCH_DISABLE */

/* timer --------------------------------------------------------------------*/
int		timer_return[MAX_APIC];
unsigned long	clock_tick[MAX_APIC];
unsigned long	lost_tick[MAX_APIC];

/* cpu lock -----------------------------------------------------------------*/
char	smp_cpu_lock[2];

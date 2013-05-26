/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/

#ifndef _KLIB_H
#define _KLIB_H

extern int inb(unsigned long);
extern int inw(unsigned long);
extern void outb(unsigned long, unsigned long);
extern void outw(unsigned long, unsigned long);
extern void ccli();
extern void csti();
extern void cwait();
extern void cxchg(unsigned long, unsigned long);
extern void cltr(unsigned long);
extern void jmp_task();
extern void resume(unsigned long);
extern short	get_cs();
extern short	get_ds();
extern short	get_ss();
extern long	get_esp();
extern long	get_eflags();	
extern long	iget_uesp();
extern long	iget_ueip();
extern int	syscall();
extern void	start_first_task();
extern void	start_second_task();

#endif

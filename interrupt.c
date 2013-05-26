/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* interrupt (32 bit protect mode) ------------------------------------------*/
#include "../include/itron.h"
#include "386.h"
#include "addr.h"
#include "proc.h"
#include "io.h"
#include "i8259.h"
#include "video.h"
#include "smp.h"
#include "timer.h"
#include "floppy.h"
#include "keyboard.h"
#include "ne2000.h"
#include "klib.h"
#include "kernelval.h"
#include "interrupt.h"
#include "interruptP.h"

#define MAX_NEST	5
unsigned long		nest;
unsigned long		old_stack[MAX_NEST];
unsigned long		old_eip[MAX_NEST];
unsigned long		baka;

extern int smp_cpu_lock[];

/* idt_init -----------------------------------------------------------------*/
void
idt_init()
{
	short	i;
	/* set default interrupt handler ------------------------------------*/
	for (i = 0 ; i < 256 ; i ++) {
		set_idt(i, (unsigned long)intr_default, SEL_K32_C, 0, GT_TRAP);
	}
	/* please  wirte other file. */
#if 1
	setup_trap();
	setup_irq();
	setup_syscall();
#endif
}

/* set_idt ------------------------------------------------------------------*/
void
set_idt(short n, unsigned long base, unsigned short sel,
			unsigned char count, unsigned char type)
{
	idt[n].offset_l = (unsigned short)(base & 0xffff);
	idt[n].offset_h = (unsigned short)((base >> 16) & 0xffff);
	idt[n].sel	= sel;
	idt[n].count	= count;
	idt[n].type	= type;
}

/* interrupt handlers -------------------------------------------------------*/
void
c_intr_default()
{
	printk("default intr\n");
	i8259_reenable();
}
void
c_intr_default2()
{
	printk("Double fault\n");
}

/* setup_trap ---------------------------------------------------------------*/
static void
setup_trap()
{
	set_idt(0, (unsigned long)intr_divide, SEL_K32_C, 0, GT_TRAP);
	set_idt(1, (unsigned long)intr_singlestep, SEL_K32_C, 0, GT_TRAP);
	set_idt(2, (unsigned long)intr_nmi, SEL_K32_C, 0, GT_TRAP);
	set_idt(3, (unsigned long)intr_breakpoint, SEL_K32_C, 0, GT_TRAP);
	set_idt(4, (unsigned long)intr_overflow, SEL_K32_C, 0, GT_TRAP);
	set_idt(5, (unsigned long)intr_bounds, SEL_K32_C, 0, GT_TRAP);
	set_idt(6, (unsigned long)intr_opcode, SEL_K32_C, 0, GT_TRAP);
	set_idt(7, (unsigned long)intr_copr_not_available, SEL_K32_C,0,GT_TRAP);
	set_idt(8, (unsigned long)intr_doublefault, SEL_K32_C, 0, GT_TRAP);
	set_idt(9, (unsigned long)intr_copr_seg_overrun, SEL_K32_C, 0, GT_TRAP);
	set_idt(10, (unsigned long)intr_tss, SEL_K32_C, 0, GT_TRAP);
	set_idt(11,(unsigned long)intr_segment_not_present,SEL_K32_C,0,GT_TRAP);
	set_idt(12, (unsigned long)intr_stack, SEL_K32_C, 0, GT_TRAP);
	set_idt(13, (unsigned long)intr_general, SEL_K32_C, 0, GT_TRAP);
	set_idt(14, (unsigned long)intr_page, SEL_K32_C, 0, GT_TRAP);
	set_idt(15, (unsigned long)intr_copr_error, SEL_K32_C, 0, GT_TRAP);
}

/* setup_irq ----------------------------------------------------------------*/
static void
setup_irq()
{
	set_idt(VECT_IRQ0, (unsigned long)intr_irq0, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ1, (unsigned long)intr_irq1, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ2, (unsigned long)intr_irq2, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ3, (unsigned long)intr_irq3, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ4, (unsigned long)intr_irq4, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ5, (unsigned long)intr_irq5, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ6, (unsigned long)intr_irq6, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ7, (unsigned long)intr_irq7, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ8, (unsigned long)intr_irq8, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ9, (unsigned long)intr_irq9, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ10, (unsigned long)intr_irq10, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ11, (unsigned long)intr_irq11, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ12, (unsigned long)intr_irq12, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ13, (unsigned long)intr_irq13, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ14, (unsigned long)intr_irq14, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_IRQ15, (unsigned long)intr_irq15, SEL_K32_C, 0, GT_INTR);
}

/* setup_syscall ------------------------------------------------------------*/
static void
setup_syscall()
{
	/* CAUTION !!! priority of system call segment must be 3 ------------*/
	set_idt(VECT_SYSCALL, (unsigned long)intr_syscall,
					SEL_K32_C, 0, GT_INTR | 0x60);

	set_idt(VECT_APIC, (unsigned long)intr_apic, SEL_K32_C, 0, GT_TRAP);
	/* smp timer interrupt ----------------------------------------------*/
	set_idt(VECT_SMP_TIMER0,
			(unsigned long)intr_smp_timer0, SEL_K32_C, 0, GT_INTR);
	set_idt(VECT_SMP_TIMER1,
			(unsigned long)intr_smp_timer1, SEL_K32_C, 0, GT_INTR);
}

/* c_intr_divide ------------------------------------------------------------*/
void
c_intr_divide()
{
#if 0	/* test code for exception handling */
	char	apic;
	extern void user_ex();
	unsigned long k;
	printk("divided by 0\n");
	baka --;
	if (*apic_id)
		apic = 1;
	else
		apic = 0;
	if (cpu_num == 0)
		apic = 0;
	sys_iras_tex(apic, c_tskid[apic], 0x0001);
	check_tex(apic);
#endif
}

/* c_intr_irq0 (timer) ------------------------------------------------------*/
void
c_intr_irq0()
{
	timer_intr(0, 1);
	i8259_reenable();
}

/* c_intr_irq1 (keyboard) ---------------------------------------------------*/
void
c_intr_irq1()
{
#if 0
printk("KEY\n");
#if 1
{
unsigned long k;
for (k = 0 ; k < 100000000; k ++);
}
#endif
printk("ENDKEY\n");
#endif
	key_intr();
	i8259_reenable();
}

/* c_intr_irq2 --------------------------------------------------------------*/
void
c_intr_irq2()
{
	printk("IRQ2\n");
	i8259_reenable();
}

/* c_intr_irq3 --------------------------------------------------------------*/
void
c_intr_irq3()
{
	printk("IRQ3\n");
	i8259_reenable();
}

void
c_intr_irq4()
{
	printk("IRQ4\n");
	i8259_reenable();
}

void
c_intr_irq5()
{
	printk("IRQ5\n");
	i8259_reenable();
}

/* c_intr_irq6 (floppy) -----------------------------------------------------*/
void
c_intr_irq6()
{
	printk("IRQ6: floppy interrupt\n");
	fdc_intr();
	i8259_reenable();
}

/* c_intr_irq7 (printer/incorrect interrupt) --------------------------------*/
void
c_intr_irq7()
{
	unsigned long a = 0xb;
	outb(IO_I8259_M, a);
	a = inb(IO_I8259_M);
	if (!(a & 0x80)) {
#if 0
		printk("incorrect interrupt \n");
#endif
		return;
	}
	printk("7");
	i8259_reenable();
}

void
c_intr_irq8()
{
	printk("IRQ8\n");
	i8259_reenable();
}

void
c_intr_irq9()
{
	printk("KEY\n");
	i8259_reenable();
}

void
c_intr_irq10()
{
	printk("IRQ10\n");
	i8259_reenable();
}

void
c_intr_irq11()
{
	printk("IRQ11\n");
	ne2000_intr();
	i8259_reenable();
}

/* c_intr_irq12 (ne2000) ----------------------------------------------------*/
void
c_intr_irq12()
{
/*
	printk("NE2000: interrupt\n");
*/
	ne2000_intr();
	i8259_reenable();
}

void
c_intr_irq13()
{
	printk("IRQ13\n");
	i8259_reenable();
}

void
c_intr_irq14()
{
	printk("IRQ14\n");
	i8259_reenable();
}

void
c_intr_irq15()
{
	printk("IRQ15\n");
	i8259_reenable();
}

/* c_intr_apic --------------------------------------------------------------*/
void
c_intr_apic()
{
}

/* c_intr_smp_timer0 (apic = 1) ---------------------------------------------*/
void
c_intr_smp_timer0()
{
extern long	k_nest;
static unsigned long tsk_tick = 0;
	int	apic;
#if 1
	if (*apic_id)
		apic = 1;
	else
		apic = 0;
	if (timer_return[apic]) {
		lost_tick[apic] ++;
		i8259_reenable();
		return;
	}

	if (smp_cpu_lock[0]) {
		smp_cpu_lock[0] ++;
		while (smp_cpu_lock[0])
			;
	}
/*
printk("timer0(%d,%d)\n", clock_tick[apic], lost_tick[apic]);
*/
clock_tick[apic] ++;
clock_tick[apic] += lost_tick[apic];
lost_tick[apic] = 0;
printk("0");
timer_intr(apic, 1);
#if 1
tsk_tick ++;
if (tsk_tick > 10) {
	tsk_tick = 0;
#if 0
	sched_hold_tsk(current_proc[apic]->id, TTS_RDY);
	sched_next_tsk(apic);	/* apic == 1 and timer == 0 */
#endif
}
#endif
#endif
	i8259_reenable();
}

/* c_intr_smp_timer1 (apic = 0) ---------------------------------------------*/
void
c_intr_smp_timer1()
{
extern unsigned long k_nest0, k_nest1;
static unsigned long tsk_tick = 5;
	int	apic;
	if (*apic_id)
		apic = 1;
	else
		apic = 0;
	if (timer_return[apic]) {
		lost_tick[apic] ++;
		i8259_reenable();
		return;
	}
	if (smp_cpu_lock[0]) {
		smp_cpu_lock[0] ++;
		while (smp_cpu_lock[0])
			;
	}
/*
printk("timer1(%d,%d)\n", clock_tick[apic], lost_tick[apic]);
*/
clock_tick[apic] ++;
clock_tick[apic] += lost_tick[apic];
lost_tick[apic] = 0;
printk("1");
#if 1
tsk_tick ++;
if (tsk_tick > 10) {
	tsk_tick = 0;
#if 0
	sched_hold_tsk(current_proc[apic]->id, TTS_RDY);
	sched_next_tsk(apic);	/* apic == 1 and timer == 0 */
#endif
}
#endif
	i8259_reenable();
}

/* irq_mask_on --------------------------------------------------------------*/
void
irq_mask_on(int n)
{
	unsigned long	a;
	if (n & 0xff) {
		a = inb(IO_I8259_MD);
		outb(IO_I8259_MD, a | (n & 0xff));
	}
	n >>= 8;
	if (n & 0xff) {
		a = inb(IO_I8259_SD);
		outb(IO_I8259_SD, a | (n & 0xff));
	} 
}

/* irq_mask_off -------------------------------------------------------------*/
void
irq_mask_off(int n)
{
	unsigned long	a;
	if (n & 0xff) {
		a = inb(IO_I8259_MD);
		n = n ^ 0xff;
		outb(IO_I8259_MD, a & n);
	}
	n >>= 8;
	if (n & 0xff) {
		a = inb(IO_I8259_SD);
		n = n ^ 0xff;
		outb(IO_I8259_SD, a & n);
	} 
}

/* irq_enter (enable timer interrupt) ---------------------------------------*/
void
irq_enter()
{
	if (*apic_id) {
		timer_return[1] = 1;
	} else {
		timer_return[0] = 1;
	}
	irq_mask_on(0xfffe);
}

/* irq_exit (reset interrupt mask) ------------------------------------------*/
void
irq_exit()
{
	if (*apic_id) {
		timer_return[1] = 0;
	} else {
		timer_return[0] = 0;
	}
	irq_mask_off(0xfffe);
}

/*
 *           [ ss  ]
 *           [ esp ] 
 *
 */

/* stack_adjust -------------------------------------------------------------*/
void
stack_adjust(W apic, void (*func)(), TEXPTN texptn, VP_INT exinf)
{
	proc_t*	p;
	unsigned long*	esp;
	extern void user_restore();

	p = current_proc[apic];
	esp =(unsigned long*)(p->reg[ESP]);
	*(-- esp) = p->reg[EIP];
	*(-- esp) = p->reg[EAX];
	*(-- esp) = p->reg[EBX];
	*(-- esp) = p->reg[ECX];
	*(-- esp) = p->reg[EDX];
	*(-- esp) = p->reg[ESI];
	*(-- esp) = p->reg[EDI];
	*(-- esp) = p->reg[EFLAGS];
	*(-- esp) = exinf;
	*(-- esp) = texptn;
	*(-- esp) = (unsigned long)user_restore;
	p->reg[ESP] = (unsigned long)esp;
	p->reg[EIP] = (unsigned long)func;
}

/* sched_next_tsk_check -----------------------------------------------------*/
int
sched_next_tsk_check()
{
	W	apic = 0;
	proc_t*	old_proc;
	extern INT	next_tsk_flag[];

	if (*apic_id)
		apic = 1;
	if (cpu_num == 0)
		apic = 0;
	if (next_tsk_flag[apic] != 0) {
		old_proc = current_proc[apic];
		sched_do_next_tsk(apic);
		next_tsk_flag[apic] = 0;
		if (old_proc != current_proc[apic]) {
			return 1;
		}
	}
	return 0;
}

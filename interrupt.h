/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* interrupt (32 bit protect mode) ------------------------------------------*/

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

void set_idt(short, unsigned long, unsigned short,
					unsigned char, unsigned char);


#if 0
/* normal MS-DOS real mode --------------------------------------------------*/
#define VECT_IRQ0	0x08
#define VECT_IRQ1	0x09
#define VECT_IRQ2	0x0a
#define VECT_IRQ3	0x0b
#define VECT_IRQ4	0x0c
#define VECT_IRQ5	0x0d
#define VECT_IRQ6	0x0e
#define VECT_IRQ7	0x0f
#define VECT_IRQ8	0x70
#define VECT_IRQ9	0x71
#define VECT_IRQ10	0x72
#define VECT_IRQ11	0x73
#define VECT_IRQ12	0x74
#define VECT_IRQ13	0x75
#define VECT_IRQ14	0x76
#define VECT_IRQ15	0x77
#else

/* interrupt vector ---------------------------------------------------------*/

#define VECT_IRQ0	0x80
#define VECT_IRQ1	0x81
#define VECT_IRQ2	0x82
#define VECT_IRQ3	0x83
#define VECT_IRQ4	0x84
#define VECT_IRQ5	0x85
#define VECT_IRQ6	0x86
#define VECT_IRQ7	0x87
#define VECT_IRQ8	0x90
#define VECT_IRQ9	0x91
#define VECT_IRQ10	0x92
#define VECT_IRQ11	0x93
#define VECT_IRQ12	0x94
#define VECT_IRQ13	0x95
#define VECT_IRQ14	0x96
#define VECT_IRQ15	0x97
#define VECT_APIC	0x98

#define VECT_SYSCALL	0x99
#define VECT_SMP_TIMER0	0x9a
#define VECT_SMP_TIMER1	0x9b

#endif

#if 1
void intr_default(void);

void intr_divide(void);
void intr_singlestep(void);
void intr_nmi(void);
void intr_breakpoint(void);
void intr_overflow(void);
void intr_bounds(void);
void intr_opcode(void);
void intr_copr_not_available(void);
void intr_doublefault(void);
void intr_copr_seg_overrun(void);
void intr_tss(void);
void intr_segment_not_present(void);
void intr_stack(void);
void intr_general(void);
void intr_page(void);
void intr_copr_error(void);

void intr_irq0(void);
void intr_irq1(void);
void intr_irq2(void);
void intr_irq3(void);
void intr_irq4(void);
void intr_irq5(void);
void intr_irq6(void);
void intr_irq7(void);
void intr_irq8(void);
void intr_irq9(void);
void intr_irq10(void);
void intr_irq11(void);
void intr_irq12(void);
void intr_irq13(void);
void intr_irq14(void);
void intr_irq15(void);
void intr_apic(void);
void intr_syscall(void);
void intr_smp_timer0(void);
void intr_smp_timer1(void);
#endif

extern unsigned long	nest;
extern unsigned long	old_stack[];
extern unsigned long	old_eip[];


void idt_init();
void irq_mask_on(int);
void irq_mask_off(int);
int sched_next_tsk_check();
#endif

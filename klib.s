# Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. -----------------#
# kernel library for MicroITRON ----------------------------------------------#

.globl	inb
.globl	inw
.globl	outb
.globl	outw
.globl	ccli
.globl	csti
.globl	cltr
.globl	cwait
.globl	cxchg
.globl	test
#.globl	switch_task
.globl	start_first_task
.globl	start_second_task
.globl	get_cs
.globl	get_ds
.globl	get_ss
.globl	get_esp
.globl	get_eflags
.globl	iget_uesp
.globl	iget_ueip
.globl	syscall
.text

# inb ------------------------------------------------------------------------#
inb:
		pushl	%ebp
		movl	%esp, %ebp
		pushl	%edx
		movl	8(%ebp), %edx
		movl	$0, %eax
		inb	%dx, %al
		popl	%edx
		popl	%ebp
		ret

# inw ------------------------------------------------------------------------#
inw:
		pushl	%ebp
		movl	%esp, %ebp
		pushl	%edx
		movl	8(%ebp), %edx
		movl	$0, %eax
		inw	%dx, %ax
		popl	%edx
		popl	%ebp
		ret
# outb -----------------------------------------------------------------------#
outb:
		pushl	%ebp
		movl	%esp, %ebp
		pushl	%edx
#		movl	8(%ebp), %dx
#		movl	10(%ebp), %ax
		movl	8(%ebp), %edx
		movl	12(%ebp), %eax
		andl	$0xffff, %edx	
#		andl	$0xff, %eax	
		outb	%al, %dx
#		out	%al, %dx
		popl	%edx
		popl	%ebp
		ret

# outw -----------------------------------------------------------------------#
outw:
		pushl	%ebp
		pushl	%edx
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movl	12(%ebp), %eax
		outw	%ax, %dx
		popl	%edx
		popl	%ebp
		ret
# cxchg ----------------------------------------------------------------------#
cxchg:
		pushl	%ebp
		movl	%esp, %ebp
		push	%ebx
		movl	8(%ebp), %ebx
		movl	12(%ebp), %eax
		xchgl	(%ebx), %eax

		popl	%ebx
		popl	%ebp
		ret

# ccli -----------------------------------------------------------------------#
ccli:
		cli
		ret
# csti -----------------------------------------------------------------------#
csti:
		sti
		ret

# cltr -----------------------------------------------------------------------#
cltr:
		push	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %eax
		ltr	%ax
		popl	%ebp
		ret
# cwait ----------------------------------------------------------------------#
cwait:
		wait
		ret
# start_first_task -----------------------------------------------------------#
start_first_task:
		ljmp	$0x38,$0x3400
		ret
# start_second_task ----------------------------------------------------------#
start_second_task:
		ljmp	$0x40,$0x3400
		ret

# switch_task-----------------------------------------------------------------#
#               |    task-1 stack |
#               |    psw          |
#               |    task-1 PC    |

#switch_task:
#		pushl	%ebp
#		movl	%esp, %ebp
#		pushl	%eax
#		pushl	%ebx
#		pushl	%ecx
#		pushl	%edx
#
#		movl	8(%ebp), %ebx
#		movl	%esp, (%ebx)
#
#		movl	12(%ebp), %esp
#
#		popl	%edx
#		popl	%ecx
#		popl	%ebx
#		popl	%eax
#		popl	%ebp
#		iret


# get_cs ---------------------------------------------------------------------#
get_cs:
		xorl	%eax, %eax
		mov	%cs, %ax
		ret
get_ds:
		xorl	%eax, %eax
		mov	%ds, %ax
		ret
get_ss:
		xorl	%eax, %eax
		mov	%ss, %ax
		ret
# get_esp --------------------------------------------------------------------#
get_esp:
		xorl	%eax, %eax
		movl	%esp, %eax
		ret
# get_eflags -----------------------------------------------------------------#
get_eflags:
		pushfl	
		popl	%eax
		ret

# interrupt function ---------------------------------------------------------#
# iget_uesp (no argument)
iget_uesp:
		xorl	%eax, %eax
#		movl	12(%esp), %eax

#		movl	28(%esp), %eax
		movl	32(%esp), %eax
		ret
iget_ueip:
		xorl	%eax, %eax
		movl	(%esp), %eax
		ret
		

# syscall --------------------------------------------------------------------#
syscall2:
		lcall	$0x70, $0
		ret
syscall:
#		pushl	%ebp
#		movl	%esp, %ebp
#		int	$64
#		leave
#		ret

		pushl	%ebp
		movl	%esp, %ebp
		int	$0x99
		popl	%ebp
		ret

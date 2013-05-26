# Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. -----------------#
# bootstrap (32 bit) ---------------------------------------------------------#

# environment ----------------------------------------------------------------#
# start ----------------------------------------------------------------------#
# auto generated code --------------------------------------------------------#
		.text
#		.org	0
#		.org	0x3400
.globl		run
.extern		cpu_num
run:
		cli
		mov	$0x28, %ax
		mov	%ax, %ds
		mov	%ax, %es
		mov	%ax, %fs
		mov	%ax, %gs

#		lss	stack_ptr, %esp
		mov	$0x30, %ax
		mov	%ax, %ss

		mov	cpu_num, %ax
		cmp	$1, %ax
		je	run_1
		# first CPU --------------------------------------------------#
		movl	$0x07a0000, %esp
		jmp	run_2
run_1:
		# second CPU -------------------------------------------------#
		movl	$0x0770000, %esp

run_2:
		cli
		call	main
#bootstrap_1:
#		jmp	bootstrap_1
		lss	stack_ptr, %esp

		pushl	$0
		pushl	$0
		pushl	$0
		cli
		cld
		call	main

.data	
.align	4
stack_ptr:
		.long	0x0070000	# offset
		.word	0x30		# selector
.align	4


# Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. -----------------#
# second boot loader (Real Mode) 16 bit to 32 bit ----------------------------#

# environment ----------------------------------------------------------------#
b_base	=	0x07c0
k_base	=	0x0300
ks_base =	0x2000
entry		start
		.text
# start ----------------------------------------------------------------------#
# 0x0200:0x1000 --------------------------------------------------------------#
# auto generated code --------------------------------------------------------#
		.org	0x0
# auto generated code --------------------------------------------------------#
start:
#		add	ax, #0x100
#		mov	ax, #0x2a0
#		mov	ax, #0x300
		mov	ax, #k_base
		mov	ds, ax
		mov	es, ax
		mov	ax, #0x1000
		mov	ss, ax
		mov	sp, #1024

#		mov	bx, #gdt_ptr
#		lgdt	(bx)
#		mov	bx, #idt_ptr
#		lidt	(bx)
#
		cli
		call	a20_init
		lidt	[idt_ptr]
		lgdt	[gdt_ptr]

# end of real mode -----------------------------------------------------------#
#		mov	ax, #0x0001
#		lmsw	ax
		mov	eax, cr0
		or	eax, #0x00000001
		mov	cr0, eax
		
# start protect mode ---------------------------------------------------------#
#		jmpi	#0x2c00, #0x20

		.byte	0xea
		.word	flush_start
		.word	0x08
flush_start:

		mov	ax, #0x10
		mov	ds, ax
		mov	es, ax
		mov	ax, #0x18
		mov	ss, ax

# jump test ------------------------------------------------------------------#
#		.byte	0xea
#		.word	jump_test
#		.word	0x08
#jump_test:

# jump 32 bit segment --------------------------------------------------------#
		.byte	0xea
		.word	0x3400
		.word	0x20
# no return ------------------------------------------------------------------#
loop0:
		jmp	loop0

# a20_init -------------------------------------------------------------------#
a20_init:
		call	a20_init_wait
		mov	al, #0xd1
		mov	dx, #0x64
		outb	dx, al
		call	a20_init_wait
		mov	al, #0xdf
		mov	dx, #0x60
		outb	dx, al
		call	a20_init_wait
		ret

# a20_init_wait --------------------------------------------------------------#
a20_init_wait:
		mov	dx, #0x64
		inb	dx
		and	al, #0x02
		jnz	a20_init_wait
		ret

# for debug ------------------------------------------------------------------#
hello:
		mov	ah, *0x0e
		mov	al, *0x39
		mov	bl, *2
		int	0x10	
		ret
error:
		jmp	error
.align	4
gdt_ptr:	.word	256-1
		.word	0x2000, 0x0
.align	4
idt_ptr:	.word	2047
		.word	0x2100
		.word	0x0

		.org	1024
		end

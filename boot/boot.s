# Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. -----------------#
# SMP MicroITRON First Boot --------------------------------------------------#
# 16 bit real mode -----------------------------------------------------------#

# segment environment --------------------------------------------------------#
b_base	=	0x07c0
m_base	=	0x7000
kt_base	=	0x0200
k_base	=	0x0300
delta_seg	=	0x1000

entry		start
		.text
		.org	0
# start ----------------------------------------------------------------------#
start:
		mov	ax, #b_base
		mov	ds, ax
		mov	ss, ax
		mov	sp, #1024
		
		# copy myself to 0x7000:0000
		mov	ax, #m_base	
		mov	es, ax
		mov	di, #0
		mov	si, #start
		mov	cx, #512
		rep
		movsb
		
		jmpi	#jumphere, #m_base
jumphere:
		mov	ax, #m_base
		mov	ds, ax
		mov	es, ax
		mov	ss, ax
		mov	sp, #1024

		mov	bx, #message
		push	bx
		call	puts
		pop	bx
		call	init_fd

		call	load

		mov	bx, #secondboot
		push	bx
		call	puts
		pop	bx
#		jmpi	#0, #k_base
#		jmpi	#0x200, #k_base
#--------------------------------------------------------(original)
#		jmpi	#0, #0x2a0
		jmpi	#0, #0x300
loop0:
		jmp	loop0

# puts -----------------------------------------------------------------------#
puts:
		push	bp
		mov	bp, sp
		push	bx
		push	cx

		mov	bx, (bp+4)
#		mov	bx, #message
puts_1:
		mov	al, (bx)
		cmp	al, *0x00
		je	puts_2
		call	putc
		inc	bx
		jmp	puts_1
puts_2:
		pop	cx
		pop	bx		
		pop	bp
		ret

# putc -----------------------------------------------------------------------#
putc:
		push	bx
		mov	ah, *0x0e
		mov	bl, *2
		int	0x10
		pop	bx
		ret

# load -----------------------------------------------------------------------#
# 2 - 10 sector to 00200:0000
load:
		mov	ax, #kt_base
		mov	es, ax
		# specify logical sector number 0-17
		mov	ax, #0x1
		mov	bx, #0x0000
load_1:
		push	bx
		push	ax
		call	readsect
		pop	ax
		pop	bx
		add	bx, #512
		inc	ax
#		cmp	ax, #32
#		cmp	ax, #48
# segment rotation check -----------------------------------------------------#
# if load count is over 128 (kernel size is over 64k) then segment rotation   #
# may occur. add 16k to es.

		test	bx, #0xffff
		jnz	over_1
		push	ax
		mov	ax, es
		add	ax, #delta_seg
		mov	es, ax
		pop	ax
over_1:

# load count -----------------------------------------------------------------#
		cmp	ax, #230
		je	load_2

#		push	ax
#		push	bx
#		seg	es
#		mov	al, (bx-512)
#		add	al, *0x41
#		mov	ah, *0x0e
#		mov	bl, *2
#		int	0x10
#		pop	bx
##		pop	ax
#


		jmp	load_1
load_2:
		ret

# readsect -------------------------------------------------------------------#
# sector 0-17
# cylinder 0-79
# head 0-1
# warning !!! this function ignores top 2 bit of cylinder.

readsect:
		push	bp
		mov	bp, sp

		mov	ax, (bp + 4)
		mov	dl, *36
		div	dl		! amari(ah) = head * sector
					! sho(al) = cylinder
		mov	ch, al
		mov	al, ah
		mov	ah, *0

		mov	dl, *18
		div	dl		! amari(ah) = sector
					! sho(al) = head
		mov	dh, al
		mov	cl, ah

		inc	cl		! sector count from 1

		mov	dl, *0		! drive number
		mov	ah, *2		! system call parameter
		mov	al, *1		! the number of sector to be read
		int	0x13

! display pattern start
		mov	ax, dot
		inc	ax
		mov	dot, ax
				! delete the following line if you want
				! to see correct sector accesses.
		shr	ax, 2
		and	ax, *0x0003
		mov	bx, #dot_data
		add	bx, ax
		mov	al, (bx)
		mov	dot1, al
		mov	bx, #dot0

		jnc	readsect_1
		mov	bx, #readerr
readsect_1:
		push	bx
		call	puts
		pop	bx

		pop	bp
		ret
# init_fd --------------------------------------------------------------------#
init_fd:
		push	es
#-----------------------------------------------------------------------------#
#		mov	0x1e * 4, #fd144
#		mov	0x1e * 4 + 2, ds
#-----------------------------------------------------------------------------#
		mov	bx, #0x78
		mov	ax, #0
		mov	es, ax
		seg	es	
		mov	(bx), #fd144
		seg	es
		mov	2(bx), ds
#		mov	dx, #0
#		sub	ax, ax
#		int	0x13

		pop	es
		ret

# to initialize fd controller ------------------------------------------------#
fd144:		.byte	0xaf, 2, 25, 2, 36, 0x1b, 0xff, 0x54, 0xf6, 15, 8

# messages -------------------------------------------------------------------#
secondboot:	.byte	0x0a, 0x0d
		.ascii	"Second Boot ..."
		.byte   0x0a, 0x0d, 0x00
message:
		.ascii	"Booting SMP MicroITRON 4.0.0 "
		.ascii	"Copyright (c) 2000 by Tsuyoshi Ishii"
		.byte   0x0a, 0x0d, 0x00
readerr:	.ascii	"Read Sector Error"
		.byte	0x0a, 0x0d, 0x00

dot0:		.byte	0x08
dot1:		.ascii	"-"
		.byte	0x00
dot_data:	.ascii	"-\\|/"
		.byte	0x00
dot:		.word	0x0000
		.org	510
		.word	0xaa55
		end

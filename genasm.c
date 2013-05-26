/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* generate GDT and IDT -----------------------------------------------------*/

#include <stdio.h>
#include "addr.h"
#include "386.h"

#define A_ASM	"table.s"

seg_t	gdt[32];
seg_t	idt[256];

/* set_gdt ------------------------------------------------------------------*/
void
set_gdt(short n, unsigned long base, unsigned long limit,
		unsigned char type, unsigned char dpl, unsigned char gda)
{
	n /= 8;
	gdt[n].base_l = (unsigned short)(base & 0xffff);
	gdt[n].base_m = (unsigned char)((base >> 16) & 0xff);
	gdt[n].base_h = (unsigned char)((base >> 24) & 0xff);
	gdt[n].limit_l = (unsigned short)(limit & 0xffff);
	gdt[n].limit_h = (unsigned char)
		((limit >> 16) & 0x0f) + gda;
	gdt[n].type = type | (dpl << 5);	
}

/* dump_gdt -----------------------------------------------------------------*/
void
dump_gdt()
{
	int	i;
	for (i = 0 ; i < 32 ; i ++) {
		printf("base:%x:%x:%x - ", gdt[i].base_h, gdt[i].base_m,
					gdt[i].base_l);
		printf("limit:%x:%x - ", gdt[i].limit_h, gdt[i].limit_l);
		printf("type:%x\n", gdt[i].type);

	}
}

/* gen_asm ------------------------------------------------------------------*/
void
gen_asm()
{
	int	i;	
	FILE*	fp;
	unsigned char*	p = (unsigned char*)gdt;

	if ((fp = fopen(A_ASM, "w")) == NULL) {
		fprintf(stderr, "Cannot open %s.\n", A_ASM);
		return;
	}

	fprintf(fp, "; This file is generated by genasm.c\n");
	fprintf(fp, "; Don't modify this file.\n");
	fprintf(fp, "\tentry\tstart\n");
	fprintf(fp, "start:\n");
	/* GDT --------------------------------------------------------------*/
	for (i = 0 ; i < 256 ; i += 8, p += 8) {
		fprintf(fp, "\t.byte ");
		fprintf(fp, "0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
			*p, *(p + 1), *(p + 2), *(p + 3), *(p + 4), *(p + 5),
			*(p + 6), *(p + 7));
	}
	/* IDT --------------------------------------------------------------*/


/*
	fprintf(fp, "\t.org\t2560\n");
*/
	fprintf(fp, "\t.org\t4096\n");
	fprintf(fp, "\tend\n");
	fclose(fp);
}

/* main ---------------------------------------------------------------------*/
int
main()
{
/*
	set_gdt(0x08, AL_KERNEL16, (unsigned long)0xffff, ST_CODE, 0, _16BIT);
	set_gdt(0x10, AL_KERNEL16, (unsigned long)0xffff, ST_DATA, 0, _16BIT);
*/
	set_gdt(0x08, (unsigned long)0x3000,
				(unsigned long)0xffff, ST_CODE, 0, _16BIT);
	set_gdt(0x10, (unsigned long)0xb8000,
				(unsigned long)0xffff, ST_DATA, 0, _16BIT);
	set_gdt(0x18, AL_KERNEL16, (unsigned long)0x0, ST_STACK, 0, _16BIT);

	set_gdt(SEL_K32_C, AL_KERNEL32,
			(unsigned long)0xfffff, ST_CODE, 0, _32BIT);
	set_gdt(SEL_K32_D, AL_KERNEL32,
			(unsigned long)0xfffff, ST_DATA, 0, _32BIT);
	set_gdt(SEL_K32_S, AL_KERNEL32,
			(unsigned long)0x0, ST_STACK, 0, _32BIT);

	set_gdt(SEL_U32_C, AL_KERNEL32,
			(unsigned long)0xfffff, ST_CODE, 3, _32BIT);
	set_gdt(SEL_U32_D, AL_KERNEL32,
			(unsigned long)0xfffff, ST_DATA, 3, _32BIT);
	set_gdt(SEL_U32_S, AL_KERNEL32,
			(unsigned long)0x0, ST_STACK, 3, _32BIT);

	gen_asm();
}
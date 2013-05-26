/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* pcmcia card configuration ------------------------------------------------*/
#include "klib.h"
#include "io.h"
#include "video.h"
#include "pcmcia.h"
#include "pcmciaP.h"

/* tuple --------------------------------------------------------------------*/
static char	tuple[512];
int		tuple_len = 0;

/* pcmcia_init --------------------------------------------------------------*/
void
pcmcia_init()
{
	char	c;
	char*	p;
	long	i;
	short	offset;
/*
	short	win_base = WIN_BASE;
*/

	/* reset card & supply Vcc ------------------------------------------*/
	pcmcia_outb(POWER_RESET, 0x70);
	pcmcia_outb(INT_GEN_CTRL, 0x20); pcmcia_sleep(1);	

	pcmcia_outb(POWER_RESET, 0xf1); pcmcia_sleep(1);	
	pcmcia_outb(INT_GEN_CTRL, 0x60);

	pcmcia_outb(ADDR_WIN_ENB, 0);
	pcmcia_outb(CSTS_CHG_INT, 0x08);

	c = pcmcia_inb(CARD_STS_CHG);
	for (i = 0 ; i < 2000 ; i ++) {
		c = pcmcia_inb(INTERFACESTS);
		if (c & 0x20) {
			printk("PCMCIA is ready \n");
			break;
		}
	}
	pcmcia_outb(SMEM_0_START_L, 0xc8);
	pcmcia_outb(SMEM_0_START_H, 0x80);
					/* win_base += 0xff; */
/*
	pcmcia_outb(SMEM_0_STOP_L, ((char)(win_base >> 0) & 0xff) + 1);
*/
#if 0	/* TISHI */
	pcmcia_outb(SMEM_0_STOP_L, 0xd0);
	pcmcia_outb(SMEM_0_STOP_H, 0xc0);
offset = (short)(- 0xcf00);	/* c800 */
	pcmcia_outb(CMEM_OFF_L, (offset >> 8) & 0xff);
	pcmcia_outb(CMEM_OFF_H, 0x40 | (offset & 0xff));	/* 4kb memory is attribute */
#else
	pcmcia_outb(SMEM_0_STOP_L, 0xd8);
	pcmcia_outb(SMEM_0_STOP_H, 0xc0);
offset = (short)(- 0xc800);	/* c800 */

	pcmcia_outb(CMEM_OFF_L, (offset >> 8) & 0xff);
	pcmcia_outb(CMEM_OFF_H, 0x40 | 0 );	/* 4kb memory is attribute */
#endif
/*
	pcmcia_outb(ADDR_WIN_ENB, MEM_WIN_0_EN);
*/
#if 0 /* TISHI */
	pcmcia_outb(ADDR_WIN_ENB, 0x01); /* enable memory */
#else
	pcmcia_outb(ADDR_WIN_ENB, 0x21); /* enable memory */
#endif

	for (i = 0 ; i < 2000 ; i ++) {
		c = pcmcia_inb(INTERFACESTS);
		if (c & 0x20) {
			printk("PCMCIA-2 is ready \n");
			break;
		}
	}
	for (i = 0 ; i < 512 ; i += 2) {
#if 0	/* TISHI */
		c = phys_get(0xcf00, i);
#else
		p = (char*)(0xc8000 + i);
		c = *p;
#endif
		tuple[tuple_len ++ ] = c;
		if (c == (char)0xff) {
			p = (char*)(0xc8000 + i + 2);
			c = *p;
			if (c == (char)0xff)
				break;
		}
	}
	dump_tuple();
	parse_tuple();

	/* set I/O port -----------------------------------------------------*/
	pcmcia_outb(IO_0_START_L, (IO_NE2000) & 0xff);
	pcmcia_outb(IO_0_START_H, (IO_NE2000 >> 8) & 0xff);
	pcmcia_outb(IO_0_STOP_L, (IO_NE2000 + 0x20) & 0xff);
	pcmcia_outb(IO_0_STOP_H, ((IO_NE2000 + 0x20) >> 8) & 0xff);

	pcmcia_outb(IO_CTRL, 0x10);	/* IO data bus is 16bit */

	/* set IRQ ----------------------------------------------------------*/
	c = pcmcia_inb(INT_GEN_CTRL);
	c &= 0xf0;
	pcmcia_outb(INT_GEN_CTRL, c | NE2000_IRQ);

	pcmcia_sleep(2);
	pcmcia_outb(ADDR_WIN_ENB, IO_WIN_0_EN); /* enable memory */
	pcmcia_sleep(2);
#if 0
	phys_set(0xcf00, 2 * 0x0fd0, 0x41);	/* write CCOR register */
	phys_set(0xcf00, 2 * 0x0fd0 + 2, 0);
					/* irq is level */
#else
	p = (char*)(0xc8000 + 2 * 0x0fd0);
	*p = 0x40;
	p = (char*)(0xc8000 + 2 * (0x0fd0 + 2));
	*p = 0x00;

#endif
	printk("pcmcia: initialized\n");
	return;
}

/* pcmcia_inb ---------------------------------------------------------------*/
static char
pcmcia_inb(char index)
{
	pcmcia_sleep(1);
	outb(IO_PCC_INDEX, index);
	pcmcia_sleep(1);
	return inb(IO_PCC_DATA);
}

/* pcmcia_outb --------------------------------------------------------------*/
static void
pcmcia_outb(char index, char data)
{
	pcmcia_sleep(1);
	outb(IO_PCC_INDEX, index);
	pcmcia_sleep(1);
	outb(IO_PCC_DATA, data);
}

/* dump_tuple (debut routine) -----------------------------------------------*/
static void
dump_tuple()
{
	int	i;
	printk("Tuple:");
	for (i = 0 ; i < tuple_len ; i ++) {
		printk("[%x]", tuple[i]);
	}
}

static void
parse_tuple()
{
	int	i;
	for (i = 0 ; i < tuple_len ;) {
		switch (tuple[i]) {
			case 0x01:
				i += (int)tuple[i + 1] & 0x00ff;
				break;
			case 0x1a:
				parse_tuple_1a(&tuple[i]);	
				i += (int)tuple[i + 1] & 0x00ff;
				break;
			case 0x1b:
				parse_tuple_1b(&tuple[i]);
				i += (int)tuple[i + 1] & 0x00ff;
				break;
			default:
				i += (int)tuple[i + 1] & 0x00ff;
		}
		i += 2;
	}
}

static void
parse_tuple_1a(char* t)
{
	char	c = *(t + 2);	/* field size byte */
	int	rsvd_len = (c >> 6) & 0x03;
	char	conf_base_low = *(t + 4 + rsvd_len);
	char	conf_base_hi = *(t + 4 + rsvd_len + 1);
/*
	int	rmask_len = (c >> 2) & 0x0f;
	int	radr_len = (c) & 0x3;
*/
	printk("config-base = %x%x", conf_base_hi & 0xff,
			conf_base_low & 0xff);
}

static void
parse_tuple_1b(char* t)
{
	char	cid = *(t + 2);
	int	i = 5;
	char	func;
/*
	char	iobase_low =  *(t + 6);
	char	iobase_hi =  *(t + 7);
*/

	if (cid & 0x80) {
		cid &= 0x3f;
		func = *(t + 4);
		if (func & 0x03) i ++;
		if (func & 0x04) i ++;
		if (func & 0x08) i ++;
		if (func & 0x10) { /* IRQ information */
			
			i += 3;
		}
		
	} else {
/*
		printf("\nI/O : %02x%02x, conf=%d\n", iobase_hi & 0xff, iobase_low &0xff, cid);
*/
	
	}
}

/* pcmcia_sleep -------------------------------------------------------------*/
static void
pcmcia_sleep(int n)
{
	long	k;
	int	i;
	for (i = 0 ; i < n ; i ++)
		for (k = 0 ; k < 6000 ; k ++);
}


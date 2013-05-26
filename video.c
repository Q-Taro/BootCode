/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* 6845 video controller ----------------------------------------------------*/
/* CAUTION (DON'T use from USER process, because scroll I/O may happen. -----*/

#include "smp.h"
#include "klib.h"
#include "io.h"
#include "videoP.h"
#include "video.h"
#include "../include/stdio.h"

static unsigned long video_lk = 0;

/* video_init ---------------------------------------------------------------*/
void
video_init()
{
	c_x = c_y = 0;
	c_y_max = 24;	
	scrolltop = 0;
	video_set_6845(G_VID_ORG, scrolltop);
/*
	outb((unsigned long)IO_6845, (unsigned long)(0x01));
	printk("6845: initialized\n");
*/
}
#if 0
static int lock = 0;
#endif
void
video_test1()
{
	static long	k;	
	static long	i = 0;

	i = 0;
video_puts("I'm One abc\n");
	while (1)  {
		for (k = 0 ; k < 65000000 ; k ++); 
		video_puts("abc\n");
	}
}

void
video_test2()
{
	long	k;
	extern	void	test();
video_puts("I'm Two abc\n");
	while (1) {
/*
		video_set_6845(G_VID_ORG, scrolltop);
		scrolltop += 80;
*/
		video_puts("--------------------------\n");
		for (k = 0 ; k < 65000000 ; k ++); 
	}
}

/* printk2 (for system call) ------------------------------------------------*/
void
printk2(char** sp)
{
	char*	s = *sp;
	va_list	ap;
	va_start2(ap, sp);
	smp_lock((unsigned long)&video_lk);
	while (*s != '\0') {
		if (*s == '%') {
			switch (*(++ s)) {
				case 's':
					video_puts(va_arg(ap, char*));
					break;
				case 'x':
					video_putn(va_arg(ap, long), 16);
					break;
				case 'd':
					video_putn(va_arg(ap, long), 10);
					break;
				case 'c':
					video_putc((char)va_arg(ap, long));
					break;
				default:
					video_putc('%');
					video_putc(*(s - 1));
			}
		} else {
			video_putc(*s);
		}
		s ++;
	}
	smp_unlock((unsigned long)&video_lk);
}

/* CAUTION (DON'T use from USER process, because scroll I/O may happen. -----*/
/* printk -------------------------------------------------------------------*/
void
printk(char* s, ...)
{
	va_list	ap;
	va_start(ap, s);
	smp_lock((unsigned long)&video_lk);
	while (*s != '\0') {
		if (*s == '%') {
			switch (*(++ s)) {
				case 's':
					video_puts(va_arg(ap, char*));
					break;
				case 'x':
					video_putn(va_arg(ap, long), 16);
					break;
				case 'd':
					video_putn(va_arg(ap, long), 10);
					break;
				case 'c':
					video_putc((char)va_arg(ap, long));
					break;
				default:
					video_putc('%');
					video_putc(*(s - 1));
			}
		} else {
			video_putc(*s);
		}
		s ++;
	}
	smp_unlock((unsigned long)&video_lk);
}

/* CAUTION (DON'T use from USER process, because scroll I/O may happen. -----*/
/* video_puts ---------------------------------------------------------------*/
void
video_puts(char* p)
{
	static char c = '#';
	while (*p != '\0') {
		video_putc(*p ++);
		if (c_y == 20)
			video_putc(c);
	}
}

/* video_putc ---------------------------------------------------------------*/
void
video_putc(char c)
{
	unsigned char*	p = (unsigned char*)G_BASE;	
	p += 2 * scrolltop + c_y * 160 + c_x * 2;
	if (c == '\n') {
		c_y ++; c_x = 0;
	} else {
		video_wait();
		*p = c;
		*(p + 1) = G_ATTR;
		c_x ++;
		if (c_x >= 80) {
			c_x = 0;
			c_y ++;
		}
	}
	if (c_y > 24) {
		video_scroll();
		c_y = 24;
	}
}

/* video_set_6845 -----------------------------------------------------------*/
static void
video_set_6845(unsigned short reg, unsigned short val)
{
	/* wait virtical return line */
	video_wait();
#if 0
	outb(IO_6845, (unsigned short)(reg & 0xff));
	outb(IO_6845 + 1, (unsigned short)(val >> 8) & 0xff);
	outb(IO_6845, (unsigned short)((reg + 1) & 0xff));
	outb(IO_6845 + 1, (unsigned short)(val) & 0xff);
#else
	outb((unsigned long)IO_6845, (unsigned long)(reg & 0xff));
	outb((unsigned long)(IO_6845 + 1), (unsigned long)(val >> 8) & 0xff);
	outb((unsigned long)IO_6845, (unsigned long)((reg + 1) & 0xff));
	outb((unsigned long)(IO_6845 + 1), (unsigned long)(val) & 0xff);
#endif
}

/* video_scroll -------------------------------------------------------------*/
static void
video_scroll()
{
	scrolltop += 80;
	c_y_max ++;
	video_set_6845(G_VID_ORG, scrolltop);
	video_clear_line();
	if (scrolltop > 80 * 25) {
		c_x = 0;
		video_wait();
		video_copy((unsigned long)(G_TOTAL + G_BASE),
				(unsigned long)G_BASE,
				(unsigned short)G_TOTAL); 
		c_y = 24;
		c_y_max = 24;
		scrolltop = 0;
		video_set_6845(G_VID_ORG, scrolltop);
		video_clear_line();
		c_x = 0;
	}
}

static void
video_clear_line()
{
	unsigned char*	p = (unsigned char*)G_BASE;	
	int	i;
	p += 2 * scrolltop + c_y * 160 + c_x * 2;
	for (i = 0 ; i < 80 * 2 ; i ++)
		*p ++ = '\0';
}

static void
video_cursor()
{
	unsigned short c = c_y * 160 + c_x;
	video_set_6845(G_CURSOR, c);
}

/* video_copy ---------------------------------------------------------------*/
static void
video_copy(unsigned long src, unsigned long dst, unsigned short len)
{
	unsigned short	i;
	unsigned char*	p_src = (unsigned char*)src;
	unsigned char*	p_dst = (unsigned char*)dst;
	for (i = 0 ; i < len ; i ++) {
		*p_dst = *p_src;
		p_dst ++; p_src ++;
	}
}

/* video_wait ---------------------------------------------------------------*/
static void
video_wait()
{
/*
	while (inb(IO_6845_V) != 0x08)
		; 
*/
}

/* video_putn ---------------------------------------------------------------*/
void
video_putn(unsigned int n, int mode)
{
	int	d;
	char	buf[64];
	char	c;
	int	i, j;

	if (n == 0) {
		buf[0] = '0';
		buf[1] = '\0';
		video_puts(buf);
		return;
	}
	
	if (mode == 16) {
		buf[0] = '\0';
		j = 0;
		while (n != 0) {
			for (i = j ; i >= 0 ; i --)
				buf[i + 2] = buf[i];
			j += 2;
			c = n & 0x0f;
			if (c <= 9)
				buf[1] = '0' + c;
			else
				buf[1] = 'a' + c - 10;			
			c = (n & 0xf0) >> 4;
			if (c <= 9)
				buf[0] = '0' + c;
			else
				buf[0] = 'a' + c - 10;			
			n >>= 8;
		}	
		video_puts(buf);
	} else if (mode == 10) {
		buf[0] = '\0';
		j = 0;
		while (n != 0) {
			for (i = j ; i >= 0 ; i --)
				buf[i + 1] = buf[i];

			d = n % 10;	/* amari */
			buf[0] = '0' + d;
			n = (n - d) / 10;
			j ++;
		}	
		video_puts(buf);
	}
}

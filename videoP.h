/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* 6845 video controller ----------------------------------------------------*/ 

#ifndef _VIDEOP_H
#define _VIDEOP_H

#define	G_BASE		0xb8000		/* linear address */
#define G_WIDTH		(80 * 2)
#define G_HEIGHT	26
/*
#define G_HEIGHT	25
*/
#define G_TOTAL		G_WIDTH * G_HEIGHT

#define G_VID_ORG	12
#define G_CURSOR	14

#define G_ATTR		0x02		/* character attribute */
static unsigned short	c_x, c_y, c_y_max;
static unsigned short	scrolltop;

/* function declarations ----------------------------------------------------*/
static void video_set_6845(unsigned short, unsigned short);
static void video_scroll();
static void video_cursor();
static void video_copy(unsigned long, unsigned long, unsigned short);
static void video_wait();
static void video_clear_line();

/* va -----------------------------------------------------------------------*/
typedef char*	va_list;
#define va_start(ap,param)	(ap=(char*)&param + sizeof(param))
#define va_start2(ap,param)	(ap=(char*)param + sizeof(param))
#define va_arg(ap,type)		((type*)(ap+=sizeof(type)))[-1]
#define va_end(ap)

#endif

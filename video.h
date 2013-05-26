/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/

#ifndef _VIDEO_H
#define _VIDEO_H

void video_init();
void video_puts(char*);
void video_putc(char);
void video_putn(unsigned int, int);
void printk2(char**);
void printk(char*, ...);

#endif

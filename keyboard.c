/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#include "video.h"
#include "io.h"
#include "klib.h"
#include "keyboardP.h"

int debug_key;
/* key_init -----------------------------------------------------------------*/
void
key_init()
{
	mode = 0;
	printk("keyboard: initialized\n");
}

/* key_intr -----------------------------------------------------------------*/
int
key_intr()
{
	unsigned char	c;
debug_key = 0;
	c = inb(IO_KEY);
	outb(IO_KEY_CS, 0xad);
	outb(IO_KEY_CS, 0xae);

	if (c & 0x80) {	/* key release */
		if (c == 0xaa || c == 0xb6)	/* SHIFT */
			mode &= ~SHIFT;
		else if (c == 0x9d)		/* CTRL */
			mode &= ~CTRL;
		
		return 0;
	}

	if (c == 0x2a || c == 0x36) {		/* SHIFT */
		mode |= SHIFT;
		return 0;
	} else if (c == 0x1d) {
		mode |= CTRL;	
		return 0;
	}
	if (mode & SHIFT)
		printk("%c", scode_sh[c]);
	else
		printk("%c", scode[c]);
	return 0;
}

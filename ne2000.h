/* original code from ...                                                    */
/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/

#ifndef _NE2000_H
#define _NE2000_H

void ne2000_init();
int ne2000_intr();
void r_int_ne2000();
void ne2000_reset();
void ne2000_memread(unsigned int, unsigned char*, unsigned int);
void ne2000_memwrite(unsigned char*, unsigned int, unsigned int);

/* struct ethernet ----------------------------------------------------------*/
typedef struct ethernet {
	unsigned char	dst[6];
	unsigned char	src[6];
	short	type;
} ethernet_t;

/* struct ring_buf ----------------------------------------------------------*/
typedef struct ring_buf {
	unsigned char	status;
	unsigned char	bound;
	unsigned short	len;
} ring_buf_t;



#endif

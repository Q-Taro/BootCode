/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/

#ifndef _PCMCIAP_H
#define _PCMCIAP_H

/*---------------------------------------------------------------------------*/
#define IDENT_REV	0	/* Identification & Revision */
#define INTERFACESTS	1	/* Interface Status */
#define POWER_RESET	2	/* Power & RESETDRV Control */
#define INT_GEN_CTRL	3	/* Interrupt & General Control */
#define CARD_STS_CHG	4	/* Card Status Change */
#define CSTS_CHG_INT	5	/* Card Status Change Interrupt Configuration*/
#define ADDR_WIN_ENB	6	/* Address Window Enable */

/* IO Window ----------------------------------------------------------------*/
#define IO_CTRL		7
#define IO_0_START_L	8
#define IO_0_START_H	9
#define IO_0_STOP_L	0x0a
#define IO_0_STOP_H	0x0b

/* memory window ------------------------------------------------------------*/
#define SMEM_0_START_L	0x10
#define SMEM_0_START_H	0x11
#define SMEM_0_STOP_L	0x12
#define SMEM_0_STOP_H	0x13
#define CMEM_OFF_L	0x14
#define CMEM_OFF_H	0x15

/* address window enable ----------------------------------------------------*/

#define MEM_WIN_0_EN	1
#define IO_WIN_0_EN	0x40

/* this means use 4k mem from a000:0000 (Graphic V-RAM) ---------------------*/
#define WIN_START_H	0x01
#define WIN_START_L	0x9e
#define WIN_BASE	0xcf00
/* tishi WIN_BASE = 0xc800 99.6.17 */

/* define I/O port and IRQ for PC card --------------------------------------*/
#define NE2000_IRQ		12

/* function declarations ----------------------------------------------------*/
static char pcmcia_inb(char);
static void pcmcia_outb(char, char);
static void dump_tuple();
static void parse_tuple();
static void parse_tuple_1a(char*);
static void parse_tuple_1b(char*);
static void pcmcia_sleep(int);

#endif

/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
#ifndef _NE2000P_H
#define _NE2000P_H

/* ne2000 ether network card ------------------------------------------------*/
#define NE_DP8390	0x00
#define NE_DATA		0x10
#define NE_RESET	0x1f
#if 0	/* 16 bit */
#define NE2000_START	0x4000
#define NE2000_SIZE	0x4000
#else	/* 8 bit */
#if 0	/* old */
#define NE2000_START	0x2000
#define NE2000_SIZE	0x2000
#else
#define NE2000_START	0x0000
#define NE2000_SIZE	0x2000
#endif
#endif

#if 0
#define PAGE_START	16
#define PAGE_TX_START	16
#define PAGE_TX_END	22
#define PAGE_RX_START	23
#define PAGE_RX_END	31
#define PAGE_END	31	
#else
#if 1
#define PAGE_START	64
#define PAGE_TX_START	64
#define PAGE_TX_END	69
#define PAGE_RX_START	70
#define PAGE_RX_END	128
#define PAGE_END	128
#else
#define PAGE_START	32
#define PAGE_TX_START	32
#define PAGE_TX_END	39
#define PAGE_RX_START	40
#define PAGE_RX_END	64
#define PAGE_END	64
#endif
#endif

/* ne2000 IRQ:10 I/O:300h you must check pcmcia code ------------------------*/
#define DP_DATA		(IO_DP8390 + NE_DATA)

/* dp8390 network interface controller --------------------------------------*/
/* page 0 read --------------------------------------------------------------*/
#define DP_CR		0x0
#define DP_CLDA0	0x1
#define DP_CLDA1	0x2
#define DP_BNRY		0x3
#define DP_TSR		0x4
#define DP_NCR		0x5
#define DP_FIFO		0x6
#define DP_ISR		0x7
#define DP_CRDA0	0x8
#define DP_CRDA1	0x9
#define DP_DUM1		0xa
#define DP_DUM2		0xb
#define DP_RSR		0xc
#define DP_CNTR0	0xd
#define DP_CNTR1	0xe
#define DP_CNTR2	0xf	

/* page 0 write -------------------------------------------------------------*/
#define DP_CR		0x0
#define DP_PSTART	0x1
#define DP_PSTOP	0x2
#define DP_BNRY		0x3
#define DP_TPSR		0x4
#define DP_TBCR0	0x5
#define DP_TBCR1	0x6
#define DP_ISR		0x7
#define DP_RSAR0	0x8
#define DP_RSAR1	0x9
#define DP_RBCR0	0xa
#define DP_RBCR1	0xb
#define DP_RCR		0xc
#define DP_TCR		0xd
#define DP_DCR		0xe
#define DP_IMR		0xf

/* page 1 read/write --------------------------------------------------------*/
#define DP_CR		0x0
#define DP_PAR0		0x1
#define DP_PAR1		0x2
#define DP_PAR2		0x3
#define DP_PAR3		0x4
#define DP_PAR4		0x5
#define DP_PAR5		0x6
#define DP_CURR		0x7
#define DP_MAR0		0x8
#define DP_MAR1		0x9
#define DP_MAR2		0xa
#define DP_MAR3		0xb
#define DP_MAR4		0xc
#define DP_MAR5		0xd
#define DP_MAR6		0xe
#define DP_MAR7		0xf


#define CR_STP		0x01
#define CR_STA		0x02
#define CR_TXP		0x04
#define CR_DMA		0x38
#define CR_DM_NOP	0x00
#define CR_DM_RR	0x08
#define CR_DM_RW	0x10
#define CR_DM_SP	0x18
#define CR_DM_ABORT	0x20
#define CR_PS		0xc0
#define CR_PS_P0	0x00
#define CR_PS_P1	0x40
#define CR_PS_P2	0x80
#define CR_PS_T0	0x80
#define CR_PS_T1	0xc0


#define ISR_MASK	0x3f
#define ISR_PRX		0x01
#define ISR_PTX		0x02
#define ISR_RXE		0x04
#define ISR_TXE		0x08
#define ISR_OVW		0x10
#define ISR_CNT		0x20
#define ISR_RDC		0x40
#define ISR_RST		0x80


#define IMR_PRXE	0x01
#define IMR_PTXE	0x02
#define IMR_RXEE	0x04
#define IMR_TXEE	0x08	
#define IMR_OVWE	0x10
#define IMR_CNTE	0x20
#define IMR_RDCE	0x40

#define DCR_WTS		0x01
#define DCR_BYTEWIDE	0x00
#define DCR_WORDWIDE	0x01
#define DCR_BOS		0x02
#define DCR_LTLENDIAN	0x00
#define DCR_BIGENDIAN	0x02
#define DCR_LAS		0x04
#define DCR_BMS		0x08
#define DCR_AR		0x10
#define DCR_FTS		0x60
#define DCR_2BYTES	0x00
#define DCR_4BYTES	0x20
#define DCR_8BYTES	0x40
#define DCR_12BYTES	0x60


#define TCR_CRC		0x01
#define TCR_ELC		0x06
#define TCR_NORMAL	0x00
#define TCR_INTERNAL	0x02
#define TCR_0EXTERNAL	0x04
#define TCR_1EXTERNAL	0x06
#define TCR_ATD		0x08
#define TCR_OFST	0x10

#define TSR_PTX		0x01
#define TSR_DFR		0x02
#define TSR_COL		0x04
#define TSR_ABT		0x08
#define TSR_CRS		0x10
#define TSR_FU		0x20
#define TSR_CDH		0x40

#define RCR_SEP		0x01
#define RCR_AR		0x02
#define RCR_AB		0x04
#define RCR_AM		0x08
#define RCR_PRO		0x10
#define RCR_MON		0x20

#define RSR_PRX		0x01
#define RSR_CRC		0x02
#define RSR_FAE		0x04
#define RSR_FO		0x08	/* FIFO over run */
#define RSR_MAP		0x10
#define RSR_PHY		0x20
#define RSR_DIS		0x40

#define SENDQ_PAGES	6
#define DP_PAGESIZE	256

typedef struct ed {
	char	ether[6];
	int	startpage;
	int	stoppage;
	int	sendpage;
/*
	char	rbuf[10][1600];
	char	wbuf[10][1600];
*/
} ed_t;

ed_t	ed;

typedef struct rcv_header {
	unsigned char	status;
	unsigned char	next;
	unsigned char	rbcl;
	unsigned char	rbch;
} rcv_header_t;

/* function declarations ----------------------------------------------------*/
static int ne2000_test(int);

#endif

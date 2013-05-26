/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* this code is written by refering to b-free project build_boot2.c          */
/* elf ----------------------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>

static char		buf[640000]; /* only 640k of kernel size supported */

/* write_space --------------------------------------------------------------*/
void
write_space(int fd_d, int len)
{
	char	buf[2];
	lseek(fd_d, len - 1, SEEK_CUR);
	writen(fd_d, buf, 1);
printf("# write_space: %d\n", len);
}

/* elf ----------------------------------------------------------------------*/
void
elf(int fd_s, int fd_d, Elf32_Word p_type, Elf32_Word p_flags,
					Elf32_Addr* addr, int* size, int flag)
{
	int		i;	
	Elf32_Ehdr	e_eh;
	static Elf32_Phdr	e_ph[32];

	lseek(fd_s, 0, SEEK_SET);	/* rewind */

	readn(fd_s, &e_eh, sizeof(Elf32_Ehdr));
	lseek(fd_s, e_eh.e_phoff, SEEK_SET);
	readn(fd_s, (char*)e_ph, e_eh.e_phentsize * e_eh.e_phnum);

printf("#-----------------------------------------------------------------#\n");
printf("# [t]ype, [f]lag, [v]addr, [m]emsz, [s]filesz, [o]ffset           #\n");
printf("# e_eh.e_phnum = %d\n", e_eh.e_phnum);
	for (i = 0 ; i < e_eh.e_phnum ; i ++) {
printf("# ph[%d]: t = 0x%x, f = 0x%x, v = 0x%x, m = 0x%x, s = 0x%x o = %d\n", 
	i, e_ph[i].p_type, e_ph[i].p_flags, e_ph[i].p_vaddr, e_ph[i].p_memsz,
		e_ph[i].p_filesz, e_ph[i].p_offset);
		if (e_ph[i].p_type == p_type &&
				e_ph[i].p_flags == p_flags) {
			/* do everything ------------------------------------*/
			if (flag) {
				write_space(fd_d,
					(int)(e_ph[i].p_vaddr - *addr) - *size);
			}
			lseek(fd_s, e_ph[i].p_offset, SEEK_SET);
			readn(fd_s, buf, e_ph[i].p_filesz);
			writen(fd_d, buf, e_ph[i].p_filesz);
			*addr = e_ph[i].p_vaddr;
			*size = e_ph[i].p_memsz;
printf("# virtual address = 0x%x, size = 0x%x\n", *addr, *size);
			break;
		}
	}
}

/* main ---------------------------------------------------------------------*/
int
main(int argc, char* argv[])
{
	int		fd_s, fd_d;
	int		i;
	struct stat	s;	
	Elf32_Ehdr	e_eh;
	Elf32_Phdr	e_ph[32];
	Elf32_Addr	addr;
	int		size;

	if (argc < 3) {
		fprintf(stderr, "elf error 0\n");
		exit(1);
	}
	
	if (((fd_s = open(argv[1], O_RDWR)) < 0) ||
			(fd_d = open(argv[2], O_RDWR | O_CREAT)) < 0) {
		fprintf(stderr, "elf error 1\n");
		exit(1);
	}

	elf(fd_s, fd_d, PT_LOAD, PF_R | PF_X, &addr, &size, 0);
	elf(fd_s, fd_d, PT_LOAD, PF_R | PF_W, &addr, &size, 1);
	exit(0);
}

/* readn --------------------------------------------------------------------*/
int
readn(int fd, char* buf, int len)
{
	int	ret;
	while (len > 0) {
		ret = read(fd, buf, len);
		if (ret < 0)
			return -1;
		buf += ret;
		len -= ret;
	}
}

/* writen -------------------------------------------------------------------*/
int
writen(int fd, char* buf, int len)
{
	int	ret;
	while (len > 0) {
		ret = write(fd, buf, len);
		if (ret < 0)
			return -1;
		buf += ret;
		len -= ret;
	}
}

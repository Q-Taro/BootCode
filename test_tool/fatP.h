/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/

#ifndef _FATP_H
#define _FATP_H

typedef fat_file {
	int	fat_id;
	long	offset;
	int	mode;
} fat_file_t;

#define MAX_FAT_FILE	16
fat_file_t	fat_file[MAX_FAT_FILE];

#endif

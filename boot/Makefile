# Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. -----------------#
# boot -----------------------------------------------------------------------#

AS = as86
LD = ld86
ASFLAGS = -0 -a
LDFLAGS = -0 -s

boot:	boot.s
	$(AS) $(ASFLAGS) -o boot.o boot.s
	$(LD) $(LDFLAGS) -o _boot boot.o
	tail --bytes=512 _boot > boot
	rm -f _boot
clean:
	rm -f boot.o boot

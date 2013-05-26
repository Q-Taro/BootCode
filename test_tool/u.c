#include <stdio.h>
main()
{
	unsigned long a;
	a = 0x101;
	a ^= 0x01;
	printf("a= %x\n", a);
}

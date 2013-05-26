/* floppy disk test tool */
int
main()
{
	int	i;
	int	k;
	printf(".org	0\n");
	printf("entry	start\n");
	printf("start:\n");
	for (k = 0 ; k < 128 ; k ++) {
		for (i = 0 ; i < 512 ; i ++)
			printf("\t.byte\t\t%d\n", k);
	}
	
}

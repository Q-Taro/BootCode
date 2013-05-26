main()
{
	char	buf[] = "abcde";
	char	buf1[] = "abcde";
	char*	p = buf;
	printf("++*p = %c\n", ++*p);
	printf("buf = %s\n", buf);
	p = buf1;
	printf("*(++p) = %c\n", *(++p));
	printf("buf = %s\n", buf1);
}

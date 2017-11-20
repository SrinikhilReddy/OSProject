#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
int puts(const char *s)
{
	int len=strlen(s);
	_syscall3(int,write,int,stdout,char*,s,int,len);
	return 0;
}

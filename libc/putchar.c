#include "../include/syscall.h"

int putchar(int c)
{
	int size = 1;
	// write character to stdout
	_syscall3(int,write,int,1,char,&c,int,size);
  	return c;
}

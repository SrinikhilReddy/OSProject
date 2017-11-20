#include <syscall.h>
#include <stdio.h>

int fputchar(int c,int fd)
{
        int size = 1;
        _syscall3(int,write,int,fd,int*,&c,int,size);
	return 0;
}

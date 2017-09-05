#include <syscall.h>
#include <stdio.h>
int fputchar(int c,FILE* fd)
{
        int size = 1;
	
        // write character to stdout
        _syscall3(int,write,int,fd->fd,char,&c,int,size);
        return c;
}

#include <syscall.h>
#include <sys/defs.h>
int writecall(int* c){
	_syscall3(int,write,int,stdout,int*,c,int,1);
	return 0;
}	
int putchar(int c)
{
return	writecall(&c);
}

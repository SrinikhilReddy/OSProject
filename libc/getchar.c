#include "../include/syscall.h"
#include "../include/sys/defs.h"
int readCall();
int c;
int getchar()
{
    readCall();
    return (int)c;
}	
int readCall(){

	_syscall3(int, read, int, stdin,int*,&c, int, 1);
	return 0;
}

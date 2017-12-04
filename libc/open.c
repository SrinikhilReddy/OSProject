#include <syscall.h>
#include <sys/defs.h>
#include <stdio.h>
int open(const char *path,int flags);
//FILE filepointer;
int fopen(char *name, char *mode)
{

    int flag;
    if(*mode == 'r'){
        flag = O_RDONLY;
    }   
    int fd = open(name,flag);
    return fd;   
}
int open(const char *path,int flags)
{
	_syscall2(int, open, char*, path, int, flags);
	return 0;
}

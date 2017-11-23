#include <syscall.h>
#include <sys/defs.h>
#include <stdio.h>
int open(const char *path,int flags);
FILE filepointer;
FILE *fopen(char *name, char *mode)
{
   // FILE fp;
    FILE *f;
    int flag;
    if(*mode == 'r'){
        flag = O_RDONLY;
    }   
    filepointer.fd = open(name,flag);
 //   putchar(fp.fd);
    f = &filepointer;
    return f;   
}
int open(const char *path,int flags)
{
	_syscall2(int, open, char*, path, int, flags);
	return 0;
}

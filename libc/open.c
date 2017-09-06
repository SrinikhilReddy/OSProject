#include <syscall.h>
#include "../include/sys/defs.h"
#include <stdio.h>
int open(const char *path,int flags);
FILE *fopen(char *name, char *mode)
{
    FILE fp;
    FILE *f;
    int flag;
    if(*mode == 'r'){
        flag = O_RDONLY;
    }   
    int i  = open(name,flag);
    fp.fd = i;
    f = &fp;
    puts("============");
    putchar(i+65);
    return f;   
}
int open(const char *path,int flags)
{
	_syscall2(int, open, char*, path, int, flags);
}

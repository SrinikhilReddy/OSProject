#include <syscall.h>
#include "../include/sys/defs.h"
#include <stdio.h>
int open(const char *path,int flags);
FILE *fopen(char *name, char *mode)
{
    FILE fp = {1};
    FILE* f;
    int flag;
    if(*mode == 'r'){
        flag = O_RDONLY;
    }   
    fp.fd = open(name,flag);
    f = &fp;
    return f;   
}
int open(const char *path,int flags)
{
	_syscall2(int, open, char, path, int, flags);
}

#include <sys/defs.h>
#include <stdio.h>
#include "open.c"
FILE* fopen(char *name, char *mode)
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


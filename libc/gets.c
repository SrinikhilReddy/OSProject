#include "../include/syscall.h"
#include <sys/defs.h>
#include <stdio.h>
#include "getchar.c"
int readcall1(char* s){
    _syscall3(int, read, int, stdin,char* ,s, int, 4096);
}
char* gets(char *string)
{
       char *s=string;
     readcall1(s);
      return string;
}



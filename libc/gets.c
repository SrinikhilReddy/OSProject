#include "../include/syscall.h"
#include <sys/defs.h>
#include <stdio.h>
#include "getchar.c"
char* gets(char *string)
{
	int n;
       char *s=string;
       while(((n=getchar())!='\n')&&(n!=EOF))
           {
                   *s++=n;
          }
          *s='\n';
          return string;
}

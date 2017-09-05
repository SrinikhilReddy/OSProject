#include "../include/syscall.h"
#include "../include/CONSTANTS.h"
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
	//	fgets(string,stdin);
}

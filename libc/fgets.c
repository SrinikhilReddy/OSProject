#include "../include/syscall.h"
#include <stdio.h>
#include "fgetchar.c"
char* fgets(char* string,int n,FILE *f)
{
	
	char *s=string;
	while(((n=fgetchar(f))!='\n')&&(n!=EOF))
	{
		*s++=n;			
	}
	*s='\n';
	return string;
}

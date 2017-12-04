#include "../include/syscall.h"
#include <stdio.h>
#include "fgetchar.c"
char* fgets(char* string,int n,int f)
{
	
	char *s=string;
//	putchar(65 + f->fd);
	do{
	n = getchar(f);
	   *s++ = n;  
	}while((n!='\n')&&(n!=EOF));
	*s='\n';
	return string;
}

#include <syscall.h>
#include <stdio.h>
#include "fputchar.c"
int fputs(const char *s, FILE *f )
{
	for(;*s;++s) if(fputchar(*s,f)!=*s) return EOF;
	return fputchar('\n',f)=='\n' ? 0 : EOF;	
}

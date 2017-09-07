#include <syscall.h>
#include <stdio.h>
int fputs(const char *s,int fd )
{
	return 	puts(s);
/*	for(;*s;++s) if(fputchar(*s,fd)!=*s) return EOF;
	return fputchar('\n',fd)=='\n' ? 0 : EOF;	
*/
}

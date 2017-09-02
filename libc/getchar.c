#include "syscalls.h"

int getchar()
{
	char c;
	if(read(0, &c, 1)==1)
		return c;
	else
		EOF;
}

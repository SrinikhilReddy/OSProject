#include "../include/syscall.h"
#include <stdio.h>
int close(int fd)
{
	_syscall1(int, close, int, fd);
}
int fclose(FILE* fp)
{
	return 	close(fp->fd);
}

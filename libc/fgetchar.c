#include <syscall.h>
#include <stdio.h>

int readcall(FILE* f);
char c;
char fgetchar(FILE *f)
{
	readcall(f);
	return c;
}
int readcall(FILE* f)
{
	_syscall3(int, read, int, f->fd, char, &c, int, 1);
}


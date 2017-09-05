#include <syscall.h>
#include <stdio.h>

size_t readcall(FILE* f);
char c;
char fgetchar(FILE *f)
{
	readcall(f);
	return c;
}
size_t readcall(FILE* f)
{
	_syscall3(size_t, read, int, f->fd, char, &c, int, 1);
}


#include <syscall.h>
#include <stdio.h>

int readcall(FILE* f);
char c;
char fgetchar(FILE *f)
{
	readcall(f);
	putchar(c);
	return c;
}
int readcall(FILE* f)
{
	//putchar(f->fd);
	_syscall3(int, read, int, f->fd, char*, &c, int, 1);
}


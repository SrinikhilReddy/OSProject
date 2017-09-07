#include <syscall.h>
#include <stdio.h>

int readcall(int fd);
char c;
char fgetc(FILE* f)
{
	readcall(f->fd);
	//putchar(*`c);
	return c;
}
int readcall(int fd)
{
	//putchar(f->fd);
	_syscall3(int, read, int, fd, char*, &c, int, 1);
}


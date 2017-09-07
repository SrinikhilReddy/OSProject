#include <syscall.h>
#include <stdio.h>

int readcall(int fd);
char c[1];
char fgetchar(int fd)
{
	readcall(fd);
	//putchar(*`c);
	return *c;
}
int readcall(int fd)
{
	//putchar(f->fd);
	_syscall3(int, read, int, fd, char*, c, int, 1);
}


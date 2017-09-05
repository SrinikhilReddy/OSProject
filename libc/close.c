#include "../include/syscall.h"

int close(int fd)
{
	_syscall1(int, close, int, fd);
}

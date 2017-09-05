#include "../include/syscall.h"

int dup2(int fd1, int fd2)
{
	_syscall2(int, dup2, int, fd1, int, fd2);
}


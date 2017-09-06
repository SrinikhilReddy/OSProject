#include "../include/syscall.h"

int pipe(int fd[2])
{
	_syscall1(int, pipe, int*, &fd[0]);
}

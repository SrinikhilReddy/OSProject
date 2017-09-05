#include "../include/syscall.h"

int exit(int status)
{
	_syscall1(int, exit, int, status);
}

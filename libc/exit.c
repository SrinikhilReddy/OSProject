#include <syscall.h>

void exit(int status)
{
	_syscall1(void, exit, int, status);
}

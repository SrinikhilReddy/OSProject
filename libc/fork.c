#include "../include/syscall.h"

int fork()
{
	_syscall(int, fork);
}

#include "../include/syscall.h"

void ps_call()
{
	_syscall(void, ps);
}

void ps()
{
	ps_call();
}

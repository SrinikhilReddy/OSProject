#include <stdio.h>
#include <syscall.h>
#include <sys/defs.h>
pid_t forkcall()
{
	_syscall(pid_t,fork);
}

pid_t fork()
{
	pid_t l  = forkcall();
//	putchar((int)l);
	return l;
}

#include <syscall.h>
#include <sys/defs.h>

pid_t fork()
{
	_syscall(pid_t, fork);
}

#include<syscall.h>

int lseek(int fd, long offset, int p)
{
	_syscall3(int, lseek, int, fd, long, offset, int, p);
}

#include <syscall.h>

int open(const char *path,int flags)
{
	_syscall2(int, open, char, path, int, flags);
}

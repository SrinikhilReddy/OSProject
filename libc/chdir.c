#include "../include/syscall.h"

int chdir(const char *path)
{
	_syscall1(int,chdir, char, path);
}

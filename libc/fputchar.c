#include "../include/syscall.h"

int fputchar(const char c, FILE *f)
{
	return _syscall3(int, write, int, *f, char, &c, int, 1);	
}

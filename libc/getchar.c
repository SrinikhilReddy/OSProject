#include "../include/syscall.h"
#include "../include/CONSTANTS.h"

int getchar()
{
	char c;
	_syscall3(int, read, int, stdin, char, &c, int, 1);
//	fgetchar(stdin);
}

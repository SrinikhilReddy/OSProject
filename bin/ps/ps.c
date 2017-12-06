#include <sys/defs.h>
#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <sys/process.c>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[])
{
	ps();
	return 0;
}

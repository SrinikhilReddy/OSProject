#include "../include/syscall.h"
#include "putchar.c"
int execvp(const char *file, char *const argv[],char *const env[]){
//	  putchar(file[0]);
    _syscall3(int,execve,char*, file,char*,argv,char*, env);
	return 0;
}

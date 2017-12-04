#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
typedef struct rusage r;
pid_t wait(int* status){
	_syscall4(pid_t,wait4,pid_t,-1,int*,status,int,0,rusage,NULL);
}
pid_t waitpid(pid_t pid,pid_t *status){
	_syscall2(pid_t,waitid,int,pid,int*,status);
}

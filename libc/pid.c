#include <stdio.h>
#include <sys/defs.h>
#include <syscall.h>

pid_t getpid_call(){
        _syscall(pid_t, getpid);
}

pid_t getppid_call(){
        _syscall(pid_t, getppid);
}

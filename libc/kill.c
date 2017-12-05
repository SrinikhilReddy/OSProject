//
// Created by Naga Srinikhil Reddy on 12/4/17.
//

#include <sys/defs.h>
#include <syscall.h>
#include <unistd.h>
int kill_call(pid_t pid)
{
    _syscall1(int,kill,pid_t,pid);
}
void kill(pid_t pid)
{
    kill_call(pid);
}
//
// Created by Naga Srinikhil Reddy on 12/4/17.
//

#include <sys/defs.h>
#include <syscall.h>
#include <unistd.h>
unsigned int sleep_call(unsigned int seconds)
{
    _syscall1(int,sleep,int,seconds);
}
unsigned int sleep(unsigned int seconds)
{
    return sleep_call(seconds);
}
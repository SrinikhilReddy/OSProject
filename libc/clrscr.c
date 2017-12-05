#include "../include/syscall.h"
#include <stdio.h>
int clrscr_call()
{
    _syscall(int, clrscr);
}
void clrscr()
{
    clrscr_call();
    return;
}
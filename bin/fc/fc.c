#include <sys/defs.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void fn(int n){
    if(n == 0){
        return;
    }
   fn(n-1);
}
int main(int argc, char *argv[], char* envp[])
{
    pid_t pid;
    for (int i = 0; i < 100000; ++i) {
        if((pid = fork()) == 0) {
            fn(i);
            puts("In child\n");
            exit(1);
        }
        else{
            pid_t k = 1999900;
            waitpid(pid, &k);
            puts("In parent\n");
        }
    }
}

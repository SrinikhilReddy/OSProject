#include <sys/defs.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[], char* envp[])
{
    pid_t pid;
    for (int i = 0; i < 100000; ++i) {
        if((pid = fork()) == 0) {
            puts("in child\n");
            int a = 10;
            exit(a+1);
        }
        else{
            pid_t k = 1999900;
            waitpid(pid, &k);
            puts("In parent\n");
        }
    }
}

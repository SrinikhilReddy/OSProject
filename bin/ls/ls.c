#include <sys/defs.h>
#include <syscall.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[], char* envp[])
{
    char dir[50];
    DIR* dp;
    if(argc == 1){
        getcwd(dir,-1);
        if(strlen(dir)>1) {
            strcat(dir, "/");
        }
        dp = opendir(dir);
    }
    else{
        strcpy(dir,argv[1]);
        int l = strlen(dir);
        if(dir[l-1] != '/'){
            strcat(dir,"/");
        }
        dp = opendir(dir);
    }

    if (dp->fd == -1) {
        puts("Unknown file path passed\n");
        puts(dir);
        return 0;
    }
    for ( ; ; ) {
        struct dirent *p = readdir(dp);
		if (p == NULL)
			break;
        puts(p->d_name);
        puts("     ");
    }
    puts("\n");
    close(dp->fd);
    return 0;
}

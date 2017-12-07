#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
#include<unistd.h>
char buf[4096];
int readstring(int fd,char* buf,int size){
	_syscall3(int,read,int,fd,char*,buf,int,size);
	return 0;
}
int main(int argc, char* argv[], char* envp[])
{
	int fp ;
    if(argc <2){
        puts("-cat: No file name given\n");
        return -1;
    }
    fp=fopen(argv[1],"r");
    if(fp < 0){
        puts("cat: ");
        puts(argv[1]);
        puts(": No such file\n");
        return -1;
    }
//    puts("\n");
    readstring(fp,buf,4096);
	puts(buf);
	close(fp);
}


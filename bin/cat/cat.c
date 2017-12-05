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
	fp=fopen(argv[1],"r");
	readstring(fp,buf,4096);
	puts(buf);
	//close(fp);
}


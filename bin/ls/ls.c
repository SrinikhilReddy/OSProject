#include <stdio.h>
#include <sys/defs.h>
#include <syscall.h>
#include <sys/tarfs.h>
#include <sys/file.h>
#include <sys/kprintf.h>
#define BUF_SIZE 1024

int direccall(int fd,char* buff,int size){
	_syscall3(int,getdents,int, fd,char*, buff,int,size);
	return 0;
}
int main(int argc, char *argv[])
{
	puts("-----------From Execve-----------\n");
	int fd;// nread;
	char buf[BUF_SIZE];

	fd = open("usr/", O_RDONLY | O_DIRECTORY);
	if (fd == -1)
		puts("Invalid argument passed");

	for ( ; ; ) {
		int l = direccall(fd,&buf[0],BUF_SIZE);
		puts(&buf[0]);
		puts("\n");		
		if (l == -1)
			puts("Error. Check arguments passed");

		if (l == 0)
			break;

//		for (int i = 0; i < nread;) 
//		{
	//		puts(&buf[0]);
        //    break;
//		}
	//	puts("\n");
	} 
	puts("\n");
	puts("=========================");
	return 0;
}

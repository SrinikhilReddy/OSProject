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
	puts(argv[1]);
	fd = open("bin/", O_RDONLY | O_DIRECTORY);
	if (fd == -1)
		puts("Invalid argument passed");

	for ( ; ; ) {
		 direccall(fd,&buf[0],BUF_SIZE);
/*		if (nread == -1)
			puts("Error. Check arguments passed");

		if (nread == 0)
			break;
*/
//		for (int i = 0; i < nread;) 
//		{
			puts(&buf[0]);
            break;
//		}

	} 
	puts("\n");
	puts("=========================");
	return 0;
}

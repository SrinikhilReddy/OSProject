#include <stdio.h>
#include <sys/defs.h>
#include <syscall.h>

#define BUF_SIZE 1024

int direccall(int fd,char* buff,int size){
	_syscall3(int,getdents,int, fd,char*, buff,int,size);
}
int main(int argc, char *argv[])
{
	int fd, nread;
	char buf[BUF_SIZE];
	struct dirent_n *d;

	fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY);
	if (fd == -1)
		puts("Invalid argument passed");

	for ( ; ; ) {
		nread = direccall(fd,&buf[0],BUF_SIZE);
		if (nread == -1)
			puts("Error. Check arguments passed");

		if (nread == 0)
			break;

		for (int i = 0; i < nread;) 
		{
			d = (struct dirent_n *) (buf + i);
			puts(d->d_name);
			puts("\t");
			i += d->d_reclen;
		}

	} 
	puts("\n");
	return 0;
}

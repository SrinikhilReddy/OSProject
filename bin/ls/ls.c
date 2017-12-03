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
	/*int fd, nread;
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
	puts("\n");*/

	/*init_tarfs();
	char *list[100];
        readdir_tarfs("bin",list);
        int i=0;
	while(*(list+i))
	{
		kprintf("%s \t", list[i]);
		i++;
	}
	kprintf("\n");
	return 0;*/
}

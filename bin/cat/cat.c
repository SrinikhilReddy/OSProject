#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
char buf[4096];
int readstring(int fd,char* buf,int size){
	_syscall3(int,read,int,fd,char*,buf,int,size);
}
int main(int argc, char* argv[], char* envp[])
{
	FILE *fp ;
	fp=fopen(argv[1],"r");
	/*	while(fgets(line,255,fp)!=NULL)
		{   
		puts(line);
		}   
		return 0;
	 */	
	int i=0;
	i = readstring(fp->fd,buf+i,4096);
	while(i>0)
	{
		lseek(fp->fd,buf+i,SEEK_SET);		
		i=readstring(fp->fd,buf+i,4096);
	}
	puts(buf);
	close(fp->fd);
}


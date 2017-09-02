#include "syscalls.h"

#define PERMS 0666;

extern FILE _iob[10];

FILE *fopen(char *name, char *mode)
{
	if((*mode!='a')&&(*mode!='r')&&(*mode!='w'))
		return NULL;
	FILE *f;
	for(f=_iob;f<_iob+10;f++)
	{
		if((fp->flag)&(_READ|_WRITE)==0)
			break; 			
	}
	if(f>_iob+10)
		return NULL;
	
	if(*mode=='a')
	{
		if((f=open(name, O_WRONLY, 0))==-1)
			f=creat(name,PERMS);
		lseek(fd, 0L, 2);
	}
	else if(*mode=='r')
	{
		int d=open(name, O_RDONLY, 0);
	}
	else
	{
		f=creat(name,PERMS);
	}
	

	return f;
	
	
}

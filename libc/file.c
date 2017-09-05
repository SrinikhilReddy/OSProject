#include "open.c"
#include "../include/CONSTANTS.h"
#define PERMS 0666
#define NULL 0

typedef struct file
{
	int char_count;
	int access_mode;
	int fdesc;
	char *base;
	char *next;
}FILE; 	

enum flag
{
	READ=1,
	WRITE=2
};


FILE *fopen(char *name, char *mode)
{
	if((*mode!='a')&&(*mode!='r')&&(*mode!='w'))
		return NULL;
	FILE *f;

	int d;	
	if(*mode=='r')
	{
		open(name, O_RDONLY);
		f->access_mode=READ;
	}
	else
	{
		//throw error
	}
	f->char_count=0;
	f->fdesc=d;
	f->base=NULL;
		
	return f;
}

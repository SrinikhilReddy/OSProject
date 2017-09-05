#include <CONSTANTS.h>
#include <stdio.h>

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

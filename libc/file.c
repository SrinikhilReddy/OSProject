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

extern FILE iob[10];

FILE *fopen(char *name, char *mode)
{
	if((*mode!='a')&&(*mode!='r')&&(*mode!='w'))
		return NULL;
	FILE *f;
	for(f=iob;f<iob+10;f++)
	{
		if((f->access_mode)&(READ|WRITE)==0)
			break; 			
	}
	if(f>iob+10)
		return NULL;

	int d;	
	if(*mode=='r')
	{
		int d=open(name, O_RDONLY, 0);
		f->access_mode=READ;
	}
	else
	{
		f=creat(name,PERMS);
		f->access_mode=WRITE;
	}
	f->char_count=0;
	f->fdesc=d;
	f->base=NULL;
		
	return f;
}

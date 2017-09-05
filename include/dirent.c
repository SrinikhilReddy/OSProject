#include "dirent.h"

#define SIZE 20

typedef struct
{
	long ino;
	char name[SIZE]; 
}Dirent;

typedef struct
{
	int fd;
	dirent d;
}DIR;

struct directory
{
	ino_t dir_ino;
	char dir_name[SIZE];
}

DIR *opendir(const char *name)
{
	DIR *d=0;
	int fd;
	if((fd=open(name, O_RDONLY, 0))==-1)
	{
		return NULL;
	}
	d->fd=fd;
	return d;
}

Dirent *readdir(DIR *dir)
{
	Dirent r;
	struct directory dir;
	while(read(dir->fd, (char*) &dir, sizeof(dir))==sizeof(dir))
	{
		if(dir.dir_ino==0)
			continue;
		r.ino=dir.dir_ino;
		strcpy(r.name,dir.dir_name,SIZE);
		r.name[SIZE]='\0';
		return &r;
	}
	return NULL;
}

#include <errno.h>
#include "dirent.h"

struct DIR
{
	char* name;
	struct dirent dir;
	HANDLE handle;
}

DIR *opendir(const char *name)
{
	DIR *d=0;
	d=malloc(sizeof(DIR));
	size_t dir_length=strlen(name);
	d->name=malloc(strlen(name)+4);
	strcpy(d->name,name);
	strcat(d->name,"*");
	return d;
}

struct dirent *readdir(DIR *dir)
{
	struct dirent *r=0;




	return r;
}

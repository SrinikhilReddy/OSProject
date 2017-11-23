#ifndef _DIRENT_H
#define _DIRENT_H

#define NAME_MAX 255

#include "stdlib.h"

/*typedef struct{
	unsigned long  d_ino;   
	unsigned long  d_off;    
	unsigned short d_reclen;
	char d_name[NAME_MAX+1];
}dirent;

typedef struct {
	int fd;
	dirent d;
} DIR;*/

DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);

#endif

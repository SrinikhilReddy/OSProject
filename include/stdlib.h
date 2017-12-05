#ifndef _STDLIB_H
#define _STDLIB_H

#include "sys/defs.h"

struct dirent1 {
           long           d_ino;
           off_t          d_off;
           unsigned short d_reclen;
           char*          d_name;
};

typedef struct{
	uint64_t fd;
	size_t size;
	char* buf;
	struct dirent *next;
}DIR;

struct dir_stream {
	int dd;
	uint64_t size;
	off_t offset;
	uint64_t address;
	char buf[2048];	
};

//typedef struct DIR DIR;

uint64_t open_dir(char* name);
uint64_t read_dir(DIR* dir);

int main(int argc, char *argv[], char *envp[]);
void exit(int status);

void *malloc(size_t size);
//void free(void *ptr);

void pushenvs(char* envp[]);
char* getallenv(int i);
int getenvlength();
char* getenv(const char* name);
int setenv(char* name,char* value,int overwrite);
int execvp(const char *file, char *const argv[],char* env[]);
#endif

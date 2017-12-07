#ifndef _STDLIB_H
#define _STDLIB_H

#include "sys/defs.h"

int main(int argc, char *argv[], char *envp[]);
void exit(int status);

void *malloc(size_t size);
void free(void *ptr);

void pushenvs(char* envp[]);
char* getallenv(int i);
int getenvlength();
char* getenv(const char* name);
int setenv(char* name,char* value,int overwrite);
int execvp(const char *file, char *const argv[],char* env[]);
#endif

#ifndef _STRING_H
#define _STRING_H

#include "stdio.h"
int strcmp(char* s, char* t);
void strcpy(char* s, char* t);
int strlen(const char* s);
char* strcat(char* s,char* t);
int starts_with(char*, char*);
char* substring(char*, int);
#endif

#include<stdio.h>
#include<stdlib.h>

char*  getenv_()
{
	return getenv("PATH");
}

void setenv_(const char* name, const char* value, int overwrite)
{
	setenv(name, value, overwrite);
}

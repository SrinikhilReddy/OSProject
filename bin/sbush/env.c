#include<stdio.h>
#include<stdlib.h>

int main()
{
	
}

char* c getenv_()
{
	return getenv("PATH");
}

void setenv_(const char* name, const char* value, int overwrite)
{
	setenv(name, value, overwrite);
}

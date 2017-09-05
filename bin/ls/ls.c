#include "../include/dirent.h"
#include "../include/stdio.h"
#include "../include/stdlib.h"

int main(int argc, char* argv[], char* envp[])
{
	char *curr=(char*) getenv("PWD");
	DIR *dir=opendir((const char*)curr);
	struct dirent *r ;
	while((r=readdir(dir)))
	{
		puts(r->d_name);	
	}
	return 0;
}

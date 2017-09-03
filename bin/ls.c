#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	char *curr=getenv("PWD");
	DIR *dir=opendir((const char*)curr);
	struct dirent *r;
	while(r=readdir(dir))
	{
		puts(r->d_name);	
	}
	return 0;
}


#include <stdio.h>
#include "../../libc/open.c"
#include "../../libc/fgets.c"
#include "../../libc/puts.c"
int cat(int argc, char *argv[])
{
	FILE *fp;
	char line[255];	
	fp=fopen(argv[1],"r");
	while(fgets(line,255,fp)!=NULL)
	{
		puts(line);
	}
	return 0;
}

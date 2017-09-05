#include<stdlib.h>
#include<stdio.h>
int main(int argc, char* argv[], char* envp[])
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

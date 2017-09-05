#include<stdio.h>
#include<stdlib.h>

int main(int argc,char* argv[],char* env[])
{
//	char* c="s";
//	gets(c);
	//puts(c);
	 int i=65;
	 i =  putchar(putchar(i));
	 i = i - putchar(i);
	 i = i * putchar(i);
}

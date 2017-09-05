#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int main(int argc,char* argv[],char* env[]){
	putchar(65+chdir(".."));
//	putchar(64+chdir("bin/"));
	char* argb[2] = {"ls",""};
	argb[1] = NULL;
	execvp("/bin/ls",argb);
}

#include "../include/syscall.h"
int execvp(const char *file, char *const argv[]){
	return _syscall2(int,execve,char*,&file,char**, argv);
}
void main(){
	char* ag[3]={"mkdir","rose",""};
	ag[2] = 0;
	execvp("/bin/mkdir",ag);
}

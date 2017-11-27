#include<syscall.h>
#include<stdlib.h>
#include<sys/defs.h>
int a = 0xAAA3FA;

int call(int k){
	_syscall3(int,write,int,1,int*,&k,int,1);
}
int fork(){
	_syscall(pid_t,fork);
}
int main(int argc,char* argv[],char *envp[]){
	int a = 10;
	int c = 40;
	c = c+a;
	call(c);
	call(c);
	fork();
	c = c + 40;
	call(c);
	while(c);
}

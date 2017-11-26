#include<syscall.h>
#include<sys/defs.h>
int a = 0xAAA3FA;

int call(){
	int k =90;
	_syscall3(int,write,int,1,int*,&k,int,1);
}
int fork(){
	_syscall(pid_t,fork);
}
int main(){
	int a = 10;
	int c = 40;
	c = c+a;
	call();
	call();
	fork();
	while(c);
}

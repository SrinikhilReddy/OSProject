#include<syscall.h>
#include<sys/defs.h>
int a = 10;

int call(int k){
	_syscall3(int,write,int,1,int*,&k,int,1);
}
int fork(){
	_syscall(pid_t,fork);
}
int main(){
	int b = 10;
	int c = 40;
	c = c+a;
	call(c);
	call(b);
	fork();
	c = c + 40;
	a = 0;
	call(c);
	while(a);
}

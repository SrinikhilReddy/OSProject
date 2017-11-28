#include<syscall.h>
#include<sys/defs.h>
int a = 8990;

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
	call(a);
	call(b);
	fork();
	a = c + 40;
	a = 0;
	call(a);
	while(a);
}

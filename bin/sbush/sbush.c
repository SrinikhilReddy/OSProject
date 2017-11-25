#include<syscall.h>
int a = 0xAAA3FA;

int call(){
	int k =90;
	_syscall3(int,write,int,1,int*,&k,int,1);
}
int main(){
	int a = 10;
	int c = 40;
	c = c+a;
	call();
	while(c);
}

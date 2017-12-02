#include<syscall.h>
#include<sys/defs.h>
uint64_t a = 8990;
char k[40];
int call(uint64_t k){
	_syscall3(int,write,int,1,int*,&k,int,1);
}
int fork(){
	_syscall(pid_t,fork);
}
int switc(){
//	switc();
	//_syscall(int,pipe);	
	k[0]= 'b';
	k[1]= 'i';
	k[2]= 'n';
	k[3]= '/';
	k[4]= 'l';
	k[5]= 's';
	k[6]= '\0';
	call((uint64_t)&k);
	_syscall1(int,execve,char*,&k[0]);
}
int main(){
	int b = 10;
	int c = 40;
	c = c+a;
	call(5);
	call(b);
	a = fork();
	call(a);
	if(a == 0){
		call(67474);
		switc();
		call(9999);
	}
	else{
		call(900000);
		switc();
		a = c + 40;
		//a = 0;
//		call(9867);
	}
//	while(1);
}

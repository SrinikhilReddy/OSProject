#include<syscall.h>
#include<sys/defs.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
uint64_t a = 8990;
//char k[40];
/*int fork(){
	_syscall(pid_t,fork);
}*/
int call(uint64_t k){
	_syscall3(int,write,int,1,int*,&k,int,1);
}
int switc(){
//	switc();
	//_syscall(int,pipe);	
	char k[40];
	k[0]= 'b';
	k[1]= 'i';
	k[2]= 'n';
	k[3]= '/';
	k[4]= 'l';
	k[5]= 's';
	k[6]= '\0';
//	k[7]= '\0';
//	call((uint64_t)&k);
	char m[10][30];
	m[0][0] = '\0';
	puts("Calling execve......");
	_syscall2(int,execve,char*,&k[0],char**,m);
//	return 1;
}
int main(int argc,char* argv[],char* env[]){
//	int b = 10;
	int c = 40;
	c = c+a;
//	call(5);
//	call(b);
	puts("Calling fork..");
	c = fork();
//	call(c);
	if(c == 0){
		puts("In fork");
	//	call(67474);
		switc();
		puts("After exec - No print here");
	}
	else{
		puts("90000\n");
//		switc();
//		a = c + 40;
//		a = 0;
		puts("dddd\n");
		puts("4433\n");
		puts("--------");
		int*q = &c;
		waitpid(c,q);
		puts("Child exited\n");
		puts("++++++++++++++++++\n");
	}
//	while(1);
}

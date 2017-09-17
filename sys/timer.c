#include <../sys/idt.h>
#include <sys/kprintf.h>
static int count = 0,sec=0;
uint16_t PIT_reload_value = 1193;
void timer_handler(){
	outportb(0x20,0x20);
	count++;
	if(count == 18){
		kprintf("%d",++sec);
		count = 0;
	}
}
void init_timer(){
	outportb(0x43, 0x36);
uint8_t lobyte = (uint8_t)(PIT_reload_value & 0xFF);
uint8_t hibyte = (uint8_t)((PIT_reload_value >> 8) & 0xFF);
outportb(0x40, lobyte);
outportb(0x40, hibyte);
}
/*#include<stdio.h>
#include<time.h>
#include<stdbool.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

volatile sig_atomic_t flag = false;

void handler(int s)
{
	flag = true;
}

int main()
{

	signal(SIGALRM, handler);
	alarm(1);
	
	int i=0;	

	time_t start;
	struct tm *t;
	time(&start);
	t = localtime(&start);
	while(1)
	{
		if(flag)
		{
			system("clear");
			printf("%d\n",i);
			++i;
			flag = false;
			alarm(1);
		}
		printf("%d",t->tm_hour);
		printf(":");
		printf("%d",t->tm_min);
		printf(":");
		printf("%d",t->tm_sec);
	}
	//return 0;
}*/

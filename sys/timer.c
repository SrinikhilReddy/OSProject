#include <../sys/idt.h>
#include <sys/kprintf.h>
static int count = 0,sec=0;
//char* reg = (char*)0xB8F9E;
//uint16_t PIT_reload_value = 1193;
//static char *reg = (char*)0xB8F9E;
void timer_handler(){
	//outportb(0x20,0x20);
	count++;
	if(count == 18){
		sec++;
		int temp = sec;
		char *reg = (char*)0xB8F9E;
		while(temp>0)
		{
			*reg = '0'+temp%10; 
			temp = temp/10;
			reg-=2;			
		}
		count = 0;
	}
	outportb(0x20,0x20);
}
/*void init_timer(){
uint8_t lobyte = (uint8_t)(PIT_reload_value & 0xFF);
uint8_t hibyte = (uint8_t)((PIT_reload_value >> 8) & 0xFF);
outportb(0x40, lobyte);
outportb(0x40, hibyte);
}*/

#include <../sys/idt.h>
#include <sys/kprintf.h>
static int count = 0,sec=0;
//uint16_t PIT_reload_value = 1193;
void timer_handler(){
	outportb(0x20,0x20);
	count++;
	if(count == 18){
		kprintf("%d \r",++sec);
		count = 0;
	}
}
/*void init_timer(){
uint8_t lobyte = (uint8_t)(PIT_reload_value & 0xFF);
uint8_t hibyte = (uint8_t)((PIT_reload_value >> 8) & 0xFF);
outportb(0x40, lobyte);
outportb(0x40, hibyte);
}*/

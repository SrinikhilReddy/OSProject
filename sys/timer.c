#include <sys/idt.h>
#include <sys/kprintf.h>
#include <sys/process.h>
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
		//0xffffffff800b8000
		char *reg = (char*)0xffffffff800B8F9E;
		while(temp>0)
		{
			*reg = '0'+temp%10; 
			temp = temp/10;
			reg-=2;			
		}
		count = 0;
        /*for(int i=0;i<MAX;i++){
            if(q[i].state == SUSPENDED && q[i].time >0){
                q[i].time--;
            }
            if(q[i].time == 0){
                q[i].state = RUNNING;
            }
        }*/
	}
	outportb(0x20,0x20);
}
/*void init_timer(){
uint8_t lobyte = (uint8_t)(PIT_reload_value & 0xFF);
uint8_t hibyte = (uint8_t)((PIT_reload_value >> 8) & 0xFF);
outportb(0x40, lobyte);
outportb(0x40, hibyte);
}*/

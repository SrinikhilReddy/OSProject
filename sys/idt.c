#include <../sys/idt.h>
#include <sys/kprintf.h>
static struct idt idt_table[256];
static struct idt_ptr pr;
void isr0();

extern void timer();
//extern void kb();


static void inline load_idt(void* ptr){
	__asm__(
			"lidt (%0)"
			:
			:"r"(ptr)	
			);
}

void set_value(uint16_t intNum,uint64_t handler)
{
	idt_table[intNum].selector  = 0x08;
	idt_table[intNum].lower_offset = handler & 0xFFFF ;
	idt_table[intNum].mid_offset = (handler >> 16) & 0xFFFF;
	idt_table[intNum].high_offset = (handler >> 32) & 0xFFFFFFFF;
	idt_table[intNum].zero_1 = 0;
	idt_table[intNum].type = 0x8E;
	idt_table[intNum].zero_2 = 0;
}
void init_idt(){
//	for(int i = 0;i<255;i++){
		set_value(32,(uint64_t)&timer);
//	}
	pr.size = (sizeof(struct idt) * 256) - 1 ;
	pr.base = (uint64_t)idt_table;
//	set_value(0,(uint64_t)&isr0);
//	set_value(0x20,(uint64_t)&timer);
	load_idt(&pr);
}

void isr0(){	
	kprintf("=================================");
	kprintf("This is a general exception");
}

void outportb(uint16_t port,uint8_t data){
	__asm__ volatile("outb %1,%0"
			:
			:"dN"(port),"a"(data));
}

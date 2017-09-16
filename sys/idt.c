#include<../sys/idt.h>

static struct idt idt_table[256];
static struct idt_ptr pr;
void *hardware_inter[16];
void isr0();
void isr32();
extern void timer();
extern void kb();
void load_idt(void* ptr){
	__asm__ volatile(
		"lidt (%0)"
		:
		:"r"(ptr)	
	);
}

void set_value(uint16_t intNum,uint64_t handler)
{
	idt_table[intNum].selector  = 8;
	idt_table[intNum].lower_offset = handler & 0xFFFF ;
	idt_table[intNum].mid_offset = (handler >> 16) & 0xFFFF;
	idt_table[intNum].high_offset = (handler >> 32) & 0xFFFFFFFF;
	idt_table[intNum].zero_1 = 0;
	idt_table[intNum].zero_2 = 0;
}
void init_idt(){
	pr.size = sizeof(idt_table);
	pr.base = (uint64_t)idt_table;
	set_value(0,(uint64_t)&isr0);
	set_value(32,(uint64_t)&isr32);
	//32 interrupts for processor
	load_idt(&pr);
	hardware_inter[0] = &timer;
	hardware_inter[1] = &kb;
}

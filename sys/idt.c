#include <sys/idt.h>
#include <sys/kprintf.h>
static struct idt idt_table[256];
static struct idt_ptr pr;

void isr();

extern void timer();
extern void kb1();
//extern void kb();

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

static inline uint8_t inportb(uint64_t port)
{
        uint8_t r;
        __asm__ volatile( 
                        "inb %1, %0"
                        : "=a"(r)
                        : "Nd"(port)
                        );  
        return r;
}
void init_idt(){
	for(int i=0;i<32;i++){
		set_value(i,(uint64_t)&isr);
	}
	set_value(32,(uint64_t)&timer);
	pr.size = (sizeof(struct idt) * 256) - 1 ;
	pr.base = (uint64_t)idt_table;
	set_value(33,(uint64_t)&kb1);
	for(int i = 34;i<255;i++){
		set_value(i,(uint64_t)&isr);
	}
//	set_value(0x20,(uint64_t)&timer);
	__asm__("lidt (%0)"::"r"(&pr));
}

void isr0(){	
	kprintf(" This is a general exception");
	outportb(0x20,0x20);
}

void outportb(uint16_t port,uint8_t data){
	__asm__ volatile("outb %1,%0"
			:
			:"dN"(port),"a"(data));
}


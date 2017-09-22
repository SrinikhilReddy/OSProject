#ifndef _IDT_H_
#define _IDT_H_
#include<sys/defs.h>
void init_idt();
extern void init_timer();
void outportb(uint16_t port,uint8_t data);
//uint8_t inportb(uint64_t port);
extern void timer_handler();

struct idt{
	uint16_t lower_offset;
	uint16_t selector;
	uint8_t zero_1;
	uint8_t type;
	uint16_t mid_offset;
	uint32_t high_offset;
	uint32_t zero_2;
}__attribute__((packed));

struct idt_ptr{
	uint16_t size;
	uint64_t base;
}__attribute__((packed));

#endif

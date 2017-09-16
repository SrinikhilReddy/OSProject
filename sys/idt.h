#ifndef _IDT_H_
#define _IDT_H_
#include<sys/defs.h>
#define call_interrupt(int_no)\
	__asm__ volatile ("    pushq %%rax;" \
"    pushq %%rcx;" \
"    pushq %%rdx;" \
"    pushq %%%rsi;" \
"    pushq %%rdi;" \
"    pushq %%r8;" \
"    pushq %%r9;" \
"    pushq %%r10;" \
"    pushq %%r11;" \
"    call isr"##int_no ";" \
"    popq %%r11;" \
"    popq %%r10;" \
"    popq %%r9;" \
"    popq %%r8;" \
"    popq %%rdi;" \
"    popq %%rsi;" \
"    popq %%rdx;" \
"    popq %%rcx;" \
"    popq %%rax;" \
"    iretq;"\
);
void init_idt();
struct idt{
	uint16_t lower_offset;
	uint16_t selector;
	uint8_t zero_1;
	uint8_t type;
	uint16_t mid_offset;
	uint32_t high_offset;
	uint32_t zero_2;
};

struct idt_ptr{
	uint16_t size;
	uint64_t base;
};

#endif

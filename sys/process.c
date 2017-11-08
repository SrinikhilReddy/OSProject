#include<sys/defs.h>
#include<sys/process.h>
#include<sys/mem.h>
#include<sys/kprintf.h>

static task_struct *curr_task;
static task_struct main_task;
static task_struct next_task;
//extern void push_all();
//extern void pop_all();
static void next_main() {
	kprintf("Test Context Switching");
	yield();
}

void init_task() {
	__asm__ volatile("movq %%cr3, %%rax; movq %%rax, %0;":"=m"(main_task.regs.cr3)::"%rax");
    	__asm__ volatile("pushq %%rax; movq (%%rsp), %%rax; movq %%rax, %0; popq %%rax;":"=m"(main_task.regs.eflags)::"%rax");
	kprintf("===========%p",main_task.regs.cr3);
	create_task(&next_task, next_main, main_task.regs.eflags, (uint64_t)main_task.regs.cr3);
    	
	main_task.next = &next_task;
    	next_task.next = &main_task;
    	curr_task = &main_task;
}

void create_task(task_struct *task, void (*main)(), uint64_t flags, uint64_t pagedir) {
	task->regs.eax = 0;
	task->regs.ebx = 0;
    	task->regs.ecx = 0;
    	task->regs.edx = 0;
    	task->regs.esi = 0;
    	task->regs.edi = 0;
    	task->regs.eflags = flags;
    	task->regs.eip = (uint64_t) main;
    	task->regs.cr3 = (uint32_t) pagedir;
    	//task->regs.esp = (uint32_t) allocPage() + 0x1000; // Not implemented here
    	task->next = 0;
}

void yield() {
	task_struct *last = curr_task;
	curr_task = curr_task->next;
//	push_all();	
//	switch_task(&(last->regs), &(curr_task->regs));
	__asm__ volatile("pushq %%rax ;pushq %%rcx ;pushq %%rdx ;pushq %%rsi ;pushq %%rdi ;pushq %%r8 ;pushq %%r9 ;pushq %%r10;pushq %%r11;":::);
	__asm__ volatile("movq %%rsp, %0;":"=g"((last->rsp))::"memory");
	__asm__ volatile("movq %0,%%rsp;"::"r"(curr_task->rsp):"memory");
	__asm__ volatile("popq %%r11; popq %%r10;popq %%r9 ;popq %%r8 ;popq %%rdi ;popq %%rsi ;popq %%rdx ;popq %%rcx ; popq %%rax;":::);
//	__asm__ volatile("movq %%rsp, %0":"=g"(curr_task->rsp)::);
	__asm__ volatile("iretq");
//	pop_all();
}

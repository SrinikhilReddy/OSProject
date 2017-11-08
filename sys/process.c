#include<sys/defs.h>
#include<sys/process.h>
#include<sys/mem.h>
#include<sys/kprintf.h>

static task_struct *curr_task;
static task_struct main_task;
static task_struct next_task;

static void next_main() {
	kprintf("Test Context Switching");
	yield();
}

void init_task() {
	__asm__ volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(main_task.regs.cr3)::"%eax");
    	__asm__ volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(main_task.regs.eflags)::"%eax");
	create_task(&next_task, next_main, main_task.regs.eflags, (uint64_t*)main_task.regs.cr3);
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
    	//task->regs.cr3 = (uint32_t) pagedir;
    	//task->regs.esp = (uint32_t) allocPage() + 0x1000; // Not implemented here
    	task->next = 0;
}

void yield() {
	task_struct *last = curr_task;
	curr_task = curr_task->next;
	switch_task(&(last->regs), &(curr_task->regs));
}

#include<sys/process.h>
#include<sys/gdt.h>
#include<sys/mem.h>

//struct task_struct q[MAX];//running;
int np = 0;
void addToQ(task_struct* a){
	
	a->next = r->next;
	r->next = a;
	
	np++;
}

/**void yield(){
        if(np<2){
                return;
        }
        else{
		task_struct *l = r;
		r = r->next;
	
	}
}*/

void yield(){
	if(np<2){
		return;
	}
	else{
		task_struct *last = r;
		r = r->next;

		set_tss_rsp((uint64_t*)r->regs.rsp);
		__asm__ volatile("movq %0,%%cr3;"::"r"(r->pml4e):);
//		__asm__ volatile("pushq %%rax ;pushq %%rcx ;pushq %%rdx ;pushq %%rsi ;pushq %%rdi ;pushq %%r8 ;pushq %%r9 ;pushq %%r10;pushq %%r11;":::);
		__asm__ volatile("movq %%rsp, %0;":"=g"((last->regs.rsp))::"memory");
		__asm__ volatile("movq %0,%%rsp;"::"r"(r->regs.rsp):"memory");
//		__asm__ volatile("popq %%r11; popq %%r10;popq %%r9 ;popq %%r8 ;popq %%rdi ;popq %%rsi ;popq %%rdx ;popq %%rcx ;popq %%rax":::);
//		__asm__ volatile("movq %%rsp, %0;":"=g"((r->regs.rsp))::"memory");
		__asm__ volatile("pushq %0;"::"r"(r->regs.rip):"memory");
		__asm__ volatile("retq");
	}
}

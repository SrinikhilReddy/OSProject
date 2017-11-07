#ifndef _PROCESS_H

#include<sys/defs.h>

enum state {
	RUNNING,
	SLEEPING,
	ZOMBIE,
	IDLE,
	READY,
	WAIT
};

typedef vm_area_struct {
	uint64_t vm_start;
	uint64_t vm_end;
	uint64_t vm_flags;
	struct mm_struct *vm_mm;
	struct vm_area_struct *vm;
};

typedef struct mm_struct {
	struct vma_area_struct *mmap;
	uint64_t arg_start, arg_end;
	uint64_t env_start, env_end;
	uint64_t pml4_t;
	uint64_t start_brk, end_brk;
	uint64_t start_stack;
	uint64_t start_code, end_code;
	uint64_t start_data, end_data; 
};

typedef struct task_struct {
	char name[128];
	char kstack[4096];
	uint64_t pid;
	uint64_t ppid;
	uint64_t rsp;
	task_struct *prev;
	task_struct *next;
	task_struct *parent;
	task_struct *child;
	Registers regs;
}process;

typedef struct {
    uint64_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;

void create();
void schedule();
void process1();
void process2();

int get_pid();
int get_ppid();

#endif

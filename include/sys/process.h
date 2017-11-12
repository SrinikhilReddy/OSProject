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

typedef struct vm_area_struct {
	uint64_t vm_start;
	uint64_t vm_end;
	uint64_t vm_flags;
	struct mm_struct *vm_mm;
	struct vm_area_struct *vm;
}vma;

typedef struct Register{
        uint64_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
}r;

typedef struct mm_struct {
	struct vma_area_struct *mmap;
	uint64_t arg_start, arg_end;
	uint64_t env_start, env_end;
	uint64_t pml4_t;
	uint64_t start_brk, end_brk;
	uint64_t start_code, end_code;
	uint64_t start_data, end_data; 
}mm;

typedef struct task_struct {
	char name[128];
	char kstack[4096];
	uint64_t pid;
	uint64_t ppid;
	uint64_t rsp;
	struct task_struct *prev;
	struct task_struct *next;
	struct task_struct *parent;
	struct task_struct *child;
	struct Register regs;
}task_struct;

extern void switch_task(r*, r*);
extern void init_task();
extern void create_task(task_struct*,uint64_t main, uint64_t flags, uint64_t pagedir);
extern void yield();

int get_pid();
int get_ppid();

#endif

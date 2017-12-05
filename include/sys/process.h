#ifndef _PROCESS_H
#define _PROCESS_H

#define MAX 100
#define STACK_S = 0x100FFFFF0000;

/**

VMA & Task struct References:
	 http://venkateshabbarapu.blogspot.com/2012/09/process-segments-and-vma.html
	    http://duartes.org/gustavo/blog/post/how-the-kernel-manages-your-memory/
**/
#include<sys/defs.h>
#include<sys/file.h>

int pid[MAX];
typedef struct vm_area_struct {
	uint64_t vm_start;
	uint64_t vm_end;
	uint64_t vm_flags;
	
	uint64_t vm_file;
	uint64_t offset;

	struct mm_struct *vm_mm;
	struct vm_area_struct *next;
}vma;

typedef struct Register{
	uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rip, rflags, cr3;
}reg;

typedef struct mm_struct {
	struct vma_area_struct *mmap, *mmap_active;
	uint64_t arg_start, arg_end;
	uint64_t env_start, env_end;
	uint64_t pml4_t;
	uint64_t start_brk, end_brk;
	uint64_t start_code, end_code;
	uint64_t start_data, end_data; 
}mm;

typedef struct task_struct {
	char name[128];
	uint64_t kstack[512];
	uint64_t *ustack;
	
	enum {
		RUNNING,
		SLEEPING,
		ZOMBIE,
		IDLE,
		READY,
		WAIT
	} state;
	uint64_t pid;
	uint64_t ppid;

	uint64_t *rsp;

	uint64_t pml4e;

	struct task_struct *prev;
	struct task_struct *next;

	struct task_struct *parent;
	
	struct task_struct *child;
	int child_count;

	struct mm_struct *mm;
	struct vm_area_struct *vm;
	char* curr_dir;
	struct file_t fd[25];
	int fd_c;
	
	struct Register regs;
}task_struct;

void switch_task(reg*, reg*);
void init_task();
void create_task(task_struct*,uint64_t main, uint64_t flags, uint64_t pagedir);
void yield();
void switchtor3();
int execvpe(char* file, char *argv[]);
void *memcpy(void *dst,void *src, uint64_t count);
int get_pid();
int get_ppid();
int fork();
void create_process(char* filename);
void addToQ(task_struct* q);
int get_fd(struct task_struct*);
task_struct* r;
struct task_struct q[MAX];
void init_proc();
void exit();
void init_p();
int wait();
int waitpid(int pid);
pid_t getpid(void);
pid_t getppid(void);
int kill(int pid);
#endif
